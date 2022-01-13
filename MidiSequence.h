#pragma once

#include <cstdint>
#include <istream>
#include <vector>

namespace wasp::sound::midi {
	struct MidiSequence {

		uint16_t ticks{};

		#pragma pack(push, 1)
		struct EventUnit {
			uint32_t deltaTime{};
			uint32_t event{};
		};
		#pragma pack(pop)

		std::vector<EventUnit> compiledTrack{};

		friend std::istream& operator>>(
			std::istream& inStream, 
			MidiSequence& midiSequence
		);
	};
}