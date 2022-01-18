#pragma once

#include <chrono>
#include <mutex>
#include "windowsInclude.h"
#include "windowsMMInclude.h"

#include "IMidiSequencer.h"
#include "MidiSequence.h"
#include "MidiOut.h"
#include "Utility/PreciseChrono.h"

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
		std::thread playbackThread{};
		utility::EventHandle wakeupSwitch{};
		std::atomic_bool running{ false };		//our flag
		std::mutex mutex{};
		std::condition_variable conditionVariable{};

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
		void playback();
		void outputMidiEvent();
		void outputSystemExclusiveEvent();
		void handleMetaEvent();

		void stopPlaybackThread();

		void resetPlaybackFields();

		static timePointType getCurrentTime();
	};
}