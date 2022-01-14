#include "MidiSequencer.h"

//#ifdef _DEBUG
#include <iostream>
//#endif

#include <stdexcept>
#include <thread>
#include <chrono>

#include "MidiConstants.h"
#include "ByteSwap.h"

namespace wasp::sound::midi {

	using wasp::utility::byteSwap32;

	using namespace constants;

	bool hundredNanosecondSleep(LONGLONG nanoseconds);

	MidiSequencer::MidiSequencer() {
		auto result{ midiOutOpen(&midiOutHandle, MIDI_MAPPER, 0, 0, CALLBACK_NULL) };
		if (result != MMSYSERR_NOERROR) {
			throw std::runtime_error{ "Error opening MIDI Mapper" };
		}
		while (!QueryPerformanceFrequency(&performanceFrequency));
	}
	
	MidiSequencer::~MidiSequencer(){
		try {
			outputAllNotesOff();
		}
		catch (const std::runtime_error& error) {
			#ifdef _DEBUG
			std::cerr << error.what();
			#endif
		}
		hundredNanosecondSleep(10'000ll);
		try {
			auto result{ midiOutClose(midiOutHandle) };
			if (result != MMSYSERR_NOERROR) {
				throw std::runtime_error{ "Error closing MIDI out" };
			}
		}
		catch (const std::runtime_error& error) {
			#ifdef _DEBUG
			std::cerr << error.what();
			#endif
		}
	}

	void MidiSequencer::test(MidiSequence& midiSequence) {
		uint32_t microsecondsPerBeat{ defaultMicrosecondsPerBeat };
		if (midiSequence.ticks & (0b1 << 15)) {
			throw std::runtime_error{ "Error does not support MIDI FPS" };
		}
		uint32_t hundredNanosecondsPerTick{ 
			(10 * microsecondsPerBeat) / midiSequence.ticks 
		};

		LARGE_INTEGER prevTimeStamp{};
		while (!QueryPerformanceCounter(&prevTimeStamp));
		LARGE_INTEGER currentTimeStamp{};
		LONGLONG previousHundredNanosecondSleepDuration{ 0 };

		auto iter{ midiSequence.compiledTrack.begin() };
		auto endIter{ midiSequence.compiledTrack.end() };
		while (iter != endIter) {
			//sleep for delta time
			if ((*iter).deltaTime != 0) {
				LONGLONG hundredNanosecondSleepDuration{
					static_cast<LONGLONG>((*iter).deltaTime)
					* hundredNanosecondsPerTick
				};
				if (QueryPerformanceCounter(&currentTimeStamp)) {
					LONGLONG timeElapsed{ 
							currentTimeStamp.QuadPart - prevTimeStamp.QuadPart
					};
					prevTimeStamp = currentTimeStamp;
					(timeElapsed *= 10'000'000ll) /= performanceFrequency.QuadPart;
					LONGLONG timeLost{
						timeElapsed - previousHundredNanosecondSleepDuration
					};
					std::cout << "lost:" << (timeLost / 10'000'000.0) << "s\n";
					hundredNanosecondSleepDuration -= timeLost;
				}
				hundredNanosecondSleep(hundredNanosecondSleepDuration);
				previousHundredNanosecondSleepDuration = hundredNanosecondSleepDuration;
			}
			//midi event
			if (((*iter).event & 0xF0) != 0xF0) {
				outputMidiEvent(midiSequence, iter);
			}
			//meta event or system exclusive
			else {
				uint8_t status{ (*iter).event & 0xFF };
				switch (status) {
					case metaEvent:
						handleMetaEvent(
							midiSequence,
							iter,
							microsecondsPerBeat,
							hundredNanosecondsPerTick
						);
						break;
					case systemExclusiveStart:
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
		auto result{
			midiOutShortMsg(midiOutHandle, (*iter).event)
		};
		++iter;
		if (result != MMSYSERR_NOERROR) {
			throw std::runtime_error{ "Error outputting MIDI short msg" };
		}
	}

	void MidiSequencer::handleMetaEvent(
		MidiSequence& midiSequence,
		MidiSequence::EventUnitTrack::iterator& iter,
		uint32_t& microsecondsPerBeat,
		uint32_t& hundredNanosecondsPerTick
	) {
		uint8_t metaEventStatus{((*iter).event >> 8) & 0xFF};
		++iter;
		//index now points to the length block
		uint32_t byteLength{ (*iter).deltaTime };
		uint32_t indexLength{ (*iter).event };
		++iter;
		//index now points to the first data entry

		if (metaEventStatus == tempo) {
			microsecondsPerBeat = byteSwap32(
				(*iter).deltaTime
			) >> 8;
			hundredNanosecondsPerTick =
				(10 * microsecondsPerBeat) / midiSequence.ticks;
		}

		iter += indexLength;
		//index now points to 1 past the last data entry
	}

	void MidiSequencer::outputSystemExclusiveEvent(
		MidiSequence midiSequence, 
		MidiSequence::EventUnitTrack::iterator& iter
	) {
		++iter;
		//index now points to the length block
		uint32_t byteLength{ (*iter).deltaTime };
		uint32_t indexLength{ (*iter).event };
		++iter;
		//index now points to the first data entry

		MIDIHDR midiHDR{};
		midiHDR.lpData = reinterpret_cast<char*>(&(*iter));
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

	void MidiSequencer::outputAllNotesOff() {
		for (int i{ 0 }; i <= 0b1111; ++i) {
			auto result{
				midiOutShortMsg(midiOutHandle, 0x00007BB0 + i)
			};
			if (result != MMSYSERR_NOERROR) {
				throw std::runtime_error{ "Error outputting MIDI short msg all notes off" };
			}
		}
	}

	//https://gist.github.com/Youka/4153f12cf2e17a77314c
	bool hundredNanosecondSleep(LONGLONG hundredNanoseconds) {

		if (hundredNanoseconds <= 0) {
			return true;
		}

		HANDLE timerHandle{ CreateWaitableTimer(NULL, TRUE, NULL) };
		if (!timerHandle) {
			return false;
		}

		LARGE_INTEGER time{};
		time.QuadPart = -hundredNanoseconds;

		if (!SetWaitableTimer(timerHandle, &time, 0, NULL, NULL, FALSE)) {
			CloseHandle(timerHandle);
			return false;
		}
		/* Start & wait for timer */
		WaitForSingleObject(timerHandle, INFINITE);
		/* Clean resources */
		CloseHandle(timerHandle);
		/* Slept without problems */
		return true;
	}
}