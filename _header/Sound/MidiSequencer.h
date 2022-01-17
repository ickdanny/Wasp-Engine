#pragma once

#include <chrono>
#include <mutex>
#include "windowsInclude.h"
#include "windowsMMInclude.h"

#include "IMidiSequencer.h"
#include "MidiSequence.h"
#include "MidiOut.h"

namespace wasp::sound::midi {
	//NOT threadsafe
	class MidiSequencer : public IMidiSequencer{
	private:
		//on Windows, steady_clock wraps the performance counter
		using clockType = std::chrono::steady_clock;
		using timePointType = clockType::time_point;

		using EventUnitTrack = MidiSequence::EventUnitTrack;

		MidiOut* midiOut{};

		//playback fields
		std::shared_ptr<MidiSequence> midiSequencePointer{};
		EventUnitTrack::iterator iter{};
		EventUnitTrack::iterator loopPointIter{};
		uint32_t microsecondsPerBeat{ defaultMicrosecondsPerBeat };
		uint32_t timePerTick100ns{};

		//threading fields
		std::atomic<uint_fast16_t> threadSafetyCounter{};
		std::mutex outputMutex{};
		std::atomic<bool> threadWaiting{ false };
		std::atomic<bool> running{ false };
	public:
		MidiSequencer(MidiOut* midiOut)
			: midiOut{ midiOut } {
		}

		MidiSequencer(const MidiSequencer& other) = delete;
		void operator=(const MidiSequencer& other) = delete;

		~MidiSequencer();

		void start(std::shared_ptr<MidiSequence> midiSequencePointer) override;
		void stop() override;

	private:
		void playbackThread(std::shared_ptr<MidiSequence> midiSequencePointer);
		void outputMidiEvent();
		void outputSystemExclusiveEvent();
		void handleMetaEvent();

		void resetPlaybackFields();

		static timePointType getCurrentTime();
	};
}