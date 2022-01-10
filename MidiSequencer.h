#pragma once

#include "framework.h"

namespace wasp::sound::midi {
	class MidiSequencer {
	private:
		HMIDIOUT midiOutHandle{};

	public:
		MidiSequencer();

		MidiSequencer(const MidiSequencer& other) = delete;
		void operator=(const MidiSequencer& other) = delete;

		~MidiSequencer();
	};
}