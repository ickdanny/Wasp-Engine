#pragma once

#include "framework.h"

#include "MidiSequence.h"

namespace wasp::sound::midi {
	class MidiSequencer { //todo: inherit IMidiSequencer
	private:
		HMIDIOUT midiOutHandle{};
		LARGE_INTEGER performanceFrequency{};

	public:
		MidiSequencer();

		MidiSequencer(const MidiSequencer& other) = delete;
		void operator=(const MidiSequencer& other) = delete;

		~MidiSequencer();

		void test(MidiSequence& midiSequence);

	private:
		void outputMidiEvent(
			MidiSequence& midiSequence, 
			MidiSequence::EventUnitTrack::iterator& iter
		);
		void handleMetaEvent(
			MidiSequence& midiSequence, 
			MidiSequence::EventUnitTrack::iterator& iter,
			uint32_t& microsecondsPerBeat,
			uint32_t& hundredNanosecondsPerTick
		);
		void outputSystemExclusiveEvent(
			MidiSequence midiSequence, 
			MidiSequence::EventUnitTrack::iterator& iter
		);

		void outputAllNotesOff();
	};
}