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

		void test(const MidiSequence& midiSequence);
	};
}