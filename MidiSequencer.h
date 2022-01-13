#pragma once

#include "framework.h"

#include "MidiSequence.h"

namespace wasp::sound::midi {
	class MidiSequencer { //todo: inherit IMidiSequencer
	private:
		HMIDIOUT midiOutHandle{};

	public:
		MidiSequencer();

		MidiSequencer(const MidiSequencer& other) = delete;
		void operator=(const MidiSequencer& other) = delete;

		~MidiSequencer();

		void test(MidiSequence& midiSequence);

	private:
		void outputMidiEvent(MidiSequence& midiSequence, size_t& index);
		void handleMetaEvent(
			MidiSequence& midiSequence, 
			size_t& index,
			uint32_t& microsecondsPerBeat,
			uint32_t& hundredNanosecondsPerTick
		);
		void outputSystemExclusiveEvent(MidiSequence midiSequence, size_t& index);
	};
}