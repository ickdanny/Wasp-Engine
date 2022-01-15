#pragma once

#include "MidiSequence.h"

namespace wasp::sound::midi {
	class IMidiSequencer {
	public:
		virtual void stop() = 0;
		virtual void start() = 0;
		virtual void setSequence(MidiSequence& midiSequence) = 0;
		virtual bool isRunning() = 0;
	};
}