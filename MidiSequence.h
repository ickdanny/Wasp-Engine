#pragma once

#include <cstdint>
#include <istream>

namespace wasp::sound::midi {
	struct MidiSequence {

		#pragma pack(push, 1)
		struct MidiHeader {
			std::uint32_t id{};		// identifier "MThd"
			std::uint32_t size{};	// always 6 in big-endian format
			std::uint16_t format{};	// big-endian format
			std::uint16_t tracks{};	// number of tracks, big-endian
			std::uint16_t ticks{};	// number of ticks per quarter note, big-endian
		} header;
		#pragma pack(pop)

		friend std::istream& operator>>(
			std::istream& inStream, 
			MidiSequence& midiSequence
		);
	};
}