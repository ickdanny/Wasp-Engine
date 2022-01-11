#include "MidiSequence.h"

#include "ByteSwap.h"

namespace wasp::sound::midi {

	using wasp::utility::byteSwap16;
	using wasp::utility::byteSwap32;

	constexpr std::uint32_t requiredHeaderID{ 0x4d546864 };		//"MThd"
	constexpr std::uint32_t requiredHeaderSize{ 0x00000006 };	//always 6

	constexpr std::uint16_t formatSingleTrack{ 0x0000 };
	constexpr std::uint16_t formatMultiTrackSync{ 0x0001 };
	constexpr std::uint16_t formatMultiTrackAsync{ 0x0002 };

	constexpr std::uint32_t requiredTrackHeaderID{ 0x4d54726b };//"MTrk

	#pragma pack(push, 1)
	struct MidiTrackHeader {
		std::uint32_t id{};		// identifier "MTrk"
		std::uint32_t length{};	// track length, big-endian
	};
	#pragma pack(pop)

	uint64_t read_var_long(std::istream& inStream)
	{
		uint64_t var{};
		uint16_t c{};
		do
		{
			inStream.read(reinterpret_cast<char*>(&c), sizeof(c));
			var = (var << 7) + (c & 0x7f);
		} while (c & 0x80);
		return var;
	}

	std::istream& operator>>(std::istream& inStream, MidiSequence& midiSequence) {
		//read in header
		inStream.read(
			reinterpret_cast<char*>(&midiSequence),
			sizeof(MidiSequence::MidiHeader)
		);

		//swap header endianness
		midiSequence.header.id = byteSwap32(midiSequence.header.id);
		midiSequence.header.size = byteSwap32(midiSequence.header.size);
		midiSequence.header.format = byteSwap16(midiSequence.header.format);
		midiSequence.header.tracks = byteSwap16(midiSequence.header.tracks);
		midiSequence.header.ticks = byteSwap16(midiSequence.header.ticks);

		if (midiSequence.header.id != requiredHeaderID
			|| midiSequence.header.size != requiredHeaderSize) {
			throw new std::runtime_error{ "Error invalid MIDI file header" };
		}

		if (midiSequence.header.format != formatSingleTrack) {
			throw new std::runtime_error{ "for this test only take single track" };
		}

		//read in track header, assuming 1 track
		MidiTrackHeader trackHeader{};
		inStream.read(
			reinterpret_cast<char*>(&trackHeader),
			sizeof(MidiTrackHeader)
		);

		trackHeader.id = byteSwap32(trackHeader.id);
		trackHeader.length = byteSwap32(trackHeader.length);

		if (trackHeader.id != requiredTrackHeaderID) {
			throw new std::runtime_error{ "Error invalid MIDI track header" };
		}

		return inStream;
	}
}