#include "MidiSequencer.h"

//#ifdef _DEBUG
#include <iostream>
//#endif

#include <stdexcept>
#include <thread>
#include <chrono>

#include "MidiConstants.h"
#include "ByteSwap.h"
#include "PreciseChrono.h"
#include "HResultError.h"
#include "MidiError.h"

namespace wasp::sound::midi {

	constexpr long long destructorWaitTime100ns{ 10'000ll };
	constexpr uint64_t ratio100nsToSeconds{ 10'000'000ull };

	using wasp::utility::byteSwap32;
	using wasp::utility::sleep100ns;

	using namespace constants;

	namespace {
		uint32_t calculateInitialTimePerTick100ns(
			uint16_t ticks
		) {
			//a leading 1 means SMPTE time
			if (ticks & 0b1000'0000'0000'0000) {
				uint32_t fps{ smpteFpsDecode(ticks >> 8) };
				uint8_t subframeResolution{ static_cast<uint8_t>(ticks) };
				auto subframesPerSecond{ fps * subframeResolution };
				return ratio100nsToSeconds / subframesPerSecond;
			}
			//a leading 0 means ticks per beat
			else {
				return (10 * defaultMicrosecondsPerBeat) / ticks;
			}
		}

		void queryPerformanceFrequency(LARGE_INTEGER* lpFrequency) {
			if (!QueryPerformanceFrequency(lpFrequency)) {
				HRESULT result{ HRESULT_FROM_WIN32(GetLastError()) };
				throw win32adaptor::HResultError{
					"Error query performance frequency ", result
				};
			}
		}

		void queryPerformanceCounter(LARGE_INTEGER* lpPerformanceCount) {
			if (!QueryPerformanceCounter(lpPerformanceCount)) {
				HRESULT result{ HRESULT_FROM_WIN32(GetLastError()) };
				throw win32adaptor::HResultError{
					"Error query performance counter ", result
				};
			}
		}
	}

	MidiSequencer::MidiSequencer() {
		openMidiOut();
		queryPerformanceFrequency(&performanceFrequency);
	}

	void MidiSequencer::openMidiOut() {
		auto result{
			midiOutOpen(&midiOutHandle, MIDI_MAPPER, 0, 0, CALLBACK_NULL)
		};
		if (result != MMSYSERR_NOERROR) {
			throw std::runtime_error{ "Error opening MIDI Mapper" };
		}
	}
	
	MidiSequencer::~MidiSequencer(){
		//try to turn off all notes
		try {
			outputReset();
		}
		catch (const std::exception& error) {
			#ifdef _DEBUG
			std::cerr << error.what();
			#endif
		}
		catch(...){
			//swallow error
		}

		//sleep for a bit for reset to get sent
		sleep100ns(destructorWaitTime100ns);

		//close midi out
		try {
			closeMidiOut();
		}
		catch (const std::exception& error) {
			#ifdef _DEBUG
			std::cerr << error.what();
			#endif
		}
		catch (...) {
			//swallow error
		}
	}

	void MidiSequencer::closeMidiOut() {
		auto result{ midiOutClose(midiOutHandle) };
		if (result != MMSYSERR_NOERROR) {
			throw std::runtime_error{ "Error closing MIDI out" };
		}
	}

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

			uint8_t status{ static_cast<uint8_t>(iter->event) };
			//midi event
			if ((status & statusMask) != metaEventOrSystemExclusive) {
				outputMidiEvent(midiSequence, iter);
			}
			//meta event or system exclusive
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
			outputShortMsg(iter->event);
		}
		catch (const std::runtime_error& runtimeError) {
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

		auto result{ midiOutPrepareHeader(midiOutHandle, &midiHDR, sizeof(MIDIHDR)) };
		if (result != MMSYSERR_NOERROR) {
			throw std::runtime_error{ "Error preparing sysEx" };
		}
		result = midiOutLongMsg(midiOutHandle, &midiHDR, sizeof(MIDIHDR));
		if (result != MMSYSERR_NOERROR) {
			throw std::runtime_error{ "Error outputting sysEx" };
		}
		result = midiOutUnprepareHeader(midiOutHandle, &midiHDR, sizeof(MIDIHDR));
		if (result != MMSYSERR_NOERROR) {
			throw std::runtime_error{ "Error unpreparing sysEx" };
		}

		iter += indexLength;
		//index now points to 1 past the last data entry
	}

	void MidiSequencer::handleMetaEvent(
		MidiSequence& midiSequence,
		MidiSequence::EventUnitTrack::iterator& iter,
		MidiSequence::EventUnitTrack::iterator& loopPointIter,
		uint32_t& microsecondsPerBeat,
		uint32_t& hundredNanosecondsPerTick
	) {
		//todo: refactor
		uint8_t metaEventStatus{((*iter).event >> 8) & 0xFF};
		++iter;
		//index now points to the length block
		uint32_t byteLength{ (*iter).deltaTime };
		uint32_t indexLength{ (*iter).event };
		++iter;
		//index now points to the first data entry

		if (metaEventStatus == tempo) {
			//test if this is actually a loop event
			if ((byteLength == 0) && (indexLength == 0)) {
				//if this is the loop start, set it
				if (loopPointIter == midiSequence.compiledTrack.end()) {
					loopPointIter = iter;
				}
				//if this is the loop end, bring us back to the loop start
				else {
					iter = loopPointIter;
				}
			}
			else {
				microsecondsPerBeat = byteSwap32(
					(*iter).deltaTime
				) >> 8;
				hundredNanosecondsPerTick =
					(10 * microsecondsPerBeat) / midiSequence.ticks;
			}
		}

		iter += indexLength;
		//index now points to 1 past the last data entry
	}

	void MidiSequencer::outputShortMsg(uint32_t output) {
		auto result{
			midiOutShortMsg(midiOutHandle, output)
		};
		if (result != MMSYSERR_NOERROR) {
			throw std::runtime_error{ "Error outputting MIDI short msg" };
		}
	}

	void MidiSequencer::outputShortMsgOnAllChannels(uint32_t output) {
		//make sure the last 4 bits are empty (it's where we put the channel)
		if (!(output << 28)) {
			for (int i{ 0 }; i <= 0b1111; ++i) {
				outputShortMsg(output + i);
			}
		}
		else {
			throw std::runtime_error{ "Error MIDI short msg must end in 0b0000" };
		}
	}

	void MidiSequencer::outputControlChangeOnAllChannels(uint32_t data) {
		outputShortMsgOnAllChannels(
			controlChange + (data << 16)
		);
	}

	void MidiSequencer::outputReset() {
		outputControlChangeOnAllChannels(allSoundOff);
		auto result{
			midiOutReset(midiOutHandle)
		};
		if (result == MMSYSERR_INVALHANDLE) {
			throw std::runtime_error{ "Error invalid MIDI out handle" };
		}
		else {
			throw std::runtime_error{ "Error resetting MIDI out" };
		}
		
		//Terminating a system - exclusive message without sending an 
		//EOX(end - of - exclusive) byte might cause problems for the receiving device.
		//The midiOutReset function does not send an EOX byte when it terminates 
		//a system - exclusive message — applications are responsible for doing this.
		outputShortMsg(systemExclusiveEnd);
	}
}