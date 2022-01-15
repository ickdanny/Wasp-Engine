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
		void openMidiOut();
		void closeMidiOut();

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

		void outputShortMsg(uint32_t output);
		
		void outputShortMsgOnAllChannels(uint32_t output);

		void outputControlChangeOnAllChannels(uint32_t data);

		void outputReset();
	};
}