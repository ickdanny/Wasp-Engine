#include "MidiSequencer.h"

#ifdef _DEBUG
#include <iostream>
#endif

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

		size_t index{ 0 };
		while(index < midiSequence.compiledTrack.size()) {
			//sleep for delta time
			if (midiSequence.compiledTrack[index].deltaTime != 0) {
				LONGLONG hundredNanosecondSleepDuration{
					static_cast<LONGLONG>(midiSequence.compiledTrack[index].deltaTime)
					* hundredNanosecondsPerTick
				};
				std::cout << "sleep for: " 
					<< hundredNanosecondSleepDuration/10'000'000.0 << "s\n";
				hundredNanosecondSleep(hundredNanosecondSleepDuration);
			}
			//midi event
			if ((midiSequence.compiledTrack[index].event & 0xF0) != 0xF0) {
				outputMidiEvent(midiSequence, index);
			}
			//meta event or system exclusive
			else {
				uint8_t status{ midiSequence.compiledTrack[index].event & 0xFF };
				switch (status) {
					case metaEvent:
						handleMetaEvent(
							midiSequence,
							index,
							microsecondsPerBeat,
							hundredNanosecondsPerTick
						);
						break;
					case systemExclusiveStart:
					case systemExclusiveEnd:
						outputSystemExclusiveEvent(midiSequence, index);
						break;
					default:
						throw std::runtime_error("Error unrecognized MIDI status");
				}
			}
		}
	}

	void MidiSequencer::outputMidiEvent(MidiSequence& midiSequence, size_t& index) {
		auto result{
			midiOutShortMsg(midiOutHandle, midiSequence.compiledTrack[index++].event)
		};
		if (result != MMSYSERR_NOERROR) {
			throw std::runtime_error{ "Error outputting MIDI short msg" };
		}
	}

	void MidiSequencer::handleMetaEvent(
		MidiSequence& midiSequence,
		size_t& index,
		uint32_t& microsecondsPerBeat,
		uint32_t& hundredNanosecondsPerTick
	) {
		uint8_t metaEventStatus{
			(midiSequence.compiledTrack[index++].event >> 8) & 0xFF
		};
		//index now points to the length block
		uint32_t byteLength{ midiSequence.compiledTrack[index].deltaTime };
		uint32_t indexLength{ midiSequence.compiledTrack[index++].event };
		//index now points to the first data entry

		if (metaEventStatus == tempo) {
			microsecondsPerBeat = byteSwap32(
				midiSequence.compiledTrack[index].deltaTime
			) >> 8;
			hundredNanosecondsPerTick =
				(10 * microsecondsPerBeat) / midiSequence.ticks;
		}

		index += indexLength;
		//index now points to 1 past the last data entry
		std::cout << "handled meta event\n";
	}

	void MidiSequencer::outputSystemExclusiveEvent(
		MidiSequence midiSequence, 
		size_t& index
	) {
		++index;
		//index now points to the length block
		uint32_t byteLength{ midiSequence.compiledTrack[index].deltaTime };
		uint32_t indexLength{ midiSequence.compiledTrack[index++].event };
		//index now points to the first data entry

		MIDIHDR midiHDR{};
		midiHDR.lpData = reinterpret_cast<char*>(
			&midiSequence.compiledTrack[index]
		);
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

		index += indexLength;
		//index now points to 1 past the last data entry
		std::cout << "output sysex\n";
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

		if (hundredNanoseconds == 0) {
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