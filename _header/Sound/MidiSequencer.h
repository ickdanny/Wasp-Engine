#pragma once

#include <chrono>
#include "windowsInclude.h"
#include "windowsMMInclude.h"

#include "MidiSequence.h"
#include "MidiOut.h"

namespace wasp::sound::midi {
	class MidiSequencer { //todo: inherit IMidiSequencer
	private:
		//on Windows, steady_clock wraps the performance counter
		using clockType = std::chrono::steady_clock;
		using timePointType = clockType::time_point;

		MidiOut* midiOut{ nullptr };

	public:
		MidiSequencer(MidiOut* midiOut)
			: midiOut{ midiOut } {
		}

		MidiSequencer(const MidiSequencer& other) = delete;
		void operator=(const MidiSequencer& other) = delete;

		~MidiSequencer();

		void test(MidiSequence& midiSequence);

	private:

		void outputMidiEvent(
			MidiSequence& midiSequence, 
			MidiSequence::EventUnitTrack::iterator& iter
		);
		void outputSystemExclusiveEvent(
			MidiSequence& midiSequence,
			MidiSequence::EventUnitTrack::iterator& iter
		);
		void handleMetaEvent(
			MidiSequence& midiSequence, 
			MidiSequence::EventUnitTrack::iterator& iter,
			MidiSequence::EventUnitTrack::iterator& loopPointIter,
			uint32_t& microsecondsPerBeat,
			uint32_t& hundredNanosecondsPerTick
		);

		static timePointType getCurrentTime();
	};
}