#include "Sound\MidiSequencer.h"

//#ifdef _DEBUG
#include <iostream>
//#endif

#include <stdexcept>
#include <thread>
#include <chrono>

#include "Sound\MidiConstants.h"
#include "Sound\MidiError.h"
#include "Utility\ByteUtil.h"
#include "Utility\PreciseChrono.h"
#include "HResultError.h"

#include "Logging.h"

namespace wasp::sound::midi {

	constexpr long long destructorWaitTime100ns{ 10'000ll };
	constexpr uint64_t ratio100nsToSeconds{ 10'000'000ull };

	using wasp::utility::byteSwap32;
	using wasp::utility::getByte;
	using wasp::utility::sleep100ns;

	namespace {
		uint32_t convertMicrosecondsPerBeatToTimePerTick100ns(
			uint32_t microsecondsPerBeat,
			uint16_t ticks
		){
			return (10 * microsecondsPerBeat) / ticks;
		}

		uint32_t calculateInitialTimePerTick100ns(uint16_t ticks) {
			//a leading 1 means SMPTE FPS time
			if (ticks & 0b1000'0000'0000'0000) {
				uint32_t fps{ smpteFpsDecode(getByte(ticks, 2)) };
				uint8_t subframeResolution{ static_cast<uint8_t>(ticks) };
				auto subframesPerSecond{ fps * subframeResolution };
				return ratio100nsToSeconds / subframesPerSecond;
			}
			//a leading 0 means ticks per beat
			else {
				return convertMicrosecondsPerBeatToTimePerTick100ns(
					defaultMicrosecondsPerBeat, 
					ticks
				);
			}
		}
	}
	
	MidiSequencer::~MidiSequencer(){
		//try to turn off all notes
		try {
			midiOut->outputReset();
		}
		catch (const std::exception& error) {
			debug::log(error.what());
		}
		catch(...){
			//swallow error
		}

		//sleep for a bit for reset to get sent
		sleep100ns(destructorWaitTime100ns);
	}


	//todo: use std::steady_clock for timing (it wraps the performanceCounter)
	void MidiSequencer::test(MidiSequence& midiSequence) {
		//timing variables
		uint32_t microsecondsPerBeat{ defaultMicrosecondsPerBeat };
		uint32_t timePerTick100ns{
			calculateInitialTimePerTick100ns(midiSequence.ticks)
		};
		LARGE_INTEGER prevTimeStamp{};
		queryPerformanceCounter(&prevTimeStamp);
		LARGE_INTEGER currentTimeStamp{};
		LONGLONG previousSleepDuration100ns{ 0 };

		//loop variables
		auto iter{ midiSequence.compiledTrack.begin() };
		auto endIter{ midiSequence.compiledTrack.end() };
		auto loopPointIter{ endIter };

		//helper function for calculating sleep duration
		auto calculateSleepDuration100ns{ [&]() {
			//calculate how long we should wait
			LONGLONG sleepDuration100ns{
				static_cast<LONGLONG>(iter->deltaTime) * timePerTick100ns
			};

			//account for the time we spent already
			queryPerformanceCounter(&currentTimeStamp);
			LONGLONG timeElapsed{
					currentTimeStamp.QuadPart - prevTimeStamp.QuadPart
			};
			(timeElapsed *= ratio100nsToSeconds) /= performanceFrequency.QuadPart;
			LONGLONG timeLost{ timeElapsed - previousSleepDuration100ns};
			sleepDuration100ns -= timeLost;

			prevTimeStamp = currentTimeStamp;
			return sleepDuration100ns;
		} };

		//begin playback
		while (iter != endIter) {
			//sleep for delta time
			if (iter->deltaTime != 0) {
				LONGLONG sleepDuration100ns{
					calculateSleepDuration100ns()
				};
				sleep100ns(sleepDuration100ns);
				previousSleepDuration100ns = sleepDuration100ns;
			}

			//handle event
			uint8_t status{ getByte(iter->event, 1) };
			//midi event case
			if ((status & statusMask) != metaEventOrSystemExclusive) {
				outputMidiEvent(midiSequence, iter);
			}
			//meta event or system exclusive cases
			else {
				switch (status) {
					case metaEvent:
						//may change tempo or loop back
						handleMetaEvent(
							midiSequence,
							iter,
							loopPointIter,
							microsecondsPerBeat,
							timePerTick100ns
						);
						break;
					case systemExclusiveStart:
					//continuation events and escape sequences start with sysEx end
					//the data is encoded the same way
					case systemExclusiveEnd:
						outputSystemExclusiveEvent(midiSequence, iter);
						break;
					default:
						throw std::runtime_error("Error unrecognized MIDI status");
				}
			}
		}
	}

	void MidiSequencer::outputMidiEvent(
		MidiSequence& midiSequence, 
		MidiSequence::EventUnitTrack::iterator& iter
	) {
		try {
			midiOut->outputShortMsg(iter->event);
		}
		catch (const std::runtime_error&) {
			++iter;
			throw;
		}
		++iter;
	}

	void MidiSequencer::outputSystemExclusiveEvent(
		MidiSequence& midiSequence,
		MidiSequence::EventUnitTrack::iterator& iter
	) {
		++iter;
		//index now points to the length block

		uint32_t byteLength{ iter->deltaTime };
		uint32_t indexLength{ iter->event };
		++iter;
		//index now points to the first data entry

		//prepare data to be output from MIDIHDR
		MIDIHDR midiHDR{};
		midiHDR.lpData = reinterpret_cast<char*>(&(*iter)); //probably UB
		midiHDR.dwBufferLength = byteLength;
		midiHDR.dwBytesRecorded = byteLength;

		iter += indexLength;

		midiOut->outputSystemExclusive(&midiHDR);

		//index now points to 1 past the last data entry
	}

	void MidiSequencer::handleMetaEvent(
		MidiSequence& midiSequence,
		MidiSequence::EventUnitTrack::iterator& iter,
		MidiSequence::EventUnitTrack::iterator& loopPointIter,
		uint32_t& microsecondsPerBeat,
		uint32_t& timePerTick100ns
	) {
		uint8_t metaEventStatus{ getByte(iter->event, 2) };
		++iter;
		//index now points to the length block

		uint32_t byteLength{ iter->deltaTime };
		uint32_t indexLength{ iter->event };
		++iter;
		//index now points to the first data entry

		if (metaEventStatus == tempo) {
			//test if this is actually a loop event
			if (
				(byteLength == MidiSequence::loopEncoding.deltaTime) 
				&& (indexLength == MidiSequence::loopEncoding.event)
			) {
				//if this is the loop start, set it
				if (loopPointIter == midiSequence.compiledTrack.end()) {
					loopPointIter = iter;
				}
				//if this is the loop end, bring us back to the loop start
				else {
					iter = loopPointIter;
				}
			}
			//otherwise this is a real tempo event
			else {
				microsecondsPerBeat = byteSwap32(iter->deltaTime) >> 8;
				timePerTick100ns = convertMicrosecondsPerBeatToTimePerTick100ns(
					microsecondsPerBeat,
					midiSequence.ticks
				);
			}
		}

		iter += indexLength;
		//index now points to 1 past the last data entry
	}
}