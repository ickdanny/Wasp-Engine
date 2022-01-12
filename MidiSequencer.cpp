#include "MidiSequencer.h"

#include <stdexcept>
#include <thread>
#include <chrono>

namespace wasp::sound::midi {

	bool hundredNanosecondSleep(LONGLONG nanoseconds);

	MidiSequencer::MidiSequencer() {
		auto result{ midiOutOpen(&midiOutHandle, MIDI_MAPPER, 0, 0, CALLBACK_NULL) };
		if (result != MMSYSERR_NOERROR) {
			throw std::runtime_error{ "Error opening MIDI Mapper" };
		}
	}
	
	MidiSequencer::~MidiSequencer(){
		midiOutClose(midiOutHandle);
	}

	void MidiSequencer::test(const MidiSequence& midiSequence) {
		int bpm{ 120 }; //default to 120
		if (midiSequence.header.ticks & (0b1 << 15)) {
			throw std::runtime_error{ "Error does not support MIDI FPS" };
		}
		int tpm{ bpm * midiSequence.header.ticks };

		for (size_t index{ 0 }; index < midiSequence.translatedTrack.size(); ++index) {
			if (midiSequence.translatedTrack[index].deltaTime != 0) {
				LONGLONG hundredNanosecondSleepDuration{
					(midiSequence.translatedTrack[index].deltaTime
					* 600'000'000ll) //convert minutes to units of 100 nanoseconds
					/ tpm
				};
				hundredNanosecondSleep(hundredNanosecondSleepDuration);
			}
			midiOutShortMsg(midiOutHandle, midiSequence.translatedTrack[index].event);
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