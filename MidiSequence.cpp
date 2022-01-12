#include "MidiSequence.h"

#include "ByteSwap.h"

namespace wasp::sound::midi {

	using wasp::utility::byteSwap16;
	using wasp::utility::byteSwap32;
	//using wasp::utility::byteSwap64;

	constexpr std::uint32_t requiredHeaderID{ 0x4d546864 };		//"MThd"
	constexpr std::uint32_t requiredHeaderSize{ 0x00000006 };	//always 6

	enum fileFormat : std::uint16_t {
		formatSingleTrack =		0x0000,
		formatMultiTrackSync =	0x0001,
		formatMultiTrackAsync = 0x0002
	};

	constexpr std::uint32_t requiredTrackHeaderID{ 0x4d54726b };//"MTrk

	constexpr std::uint8_t commandMask{ 0b1111'0000 };

	//midi command codes, for non-meta events first 4 bits are channel #
	enum channelVoiceMessages : std::uint8_t {
		noteOff =				0b1000'0000,
		noteOn =				0b1001'0000,
		polyphonicKeyPressure = 0b1010'0000,
		controlChange =			0b1011'0000,
		programChange =			0b1100'0000,
		channelPressure =		0b1101'0000,
		pitchBendChange =		0b1110'0000,
		systemExclusive =		0b1111'0000 
	};

	constexpr std::uint8_t fullMetaEvent{ 0b1111'1111 };

	constexpr std::uint8_t endOfTrack{ 0b0010'1111 };

	#pragma pack(push, 1)
	struct MidiTrackHeader {
		std::uint32_t id{};		// identifier "MTrk"
		std::uint32_t length{};	// track length, big-endian
	};
	#pragma pack(pop)

	uint32_t readDeltaTime(std::istream& inStream)
	{
		uint32_t deltaTime{};
		uint8_t byte{};

		//read variable length loop
		do{
			inStream.read(reinterpret_cast<char*>(&byte), sizeof(byte));
			deltaTime = (deltaTime << 7) + (byte & 0b0111'1111);
		} while (byte & 0b1000'0000);

		//DON'T SWAP ENDIANNESS
		//deltaTime = byteSwap32(deltaTime);

		return deltaTime;
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

		//check header validity
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

		//swap track header endianness
		trackHeader.id = byteSwap32(trackHeader.id);
		trackHeader.length = byteSwap32(trackHeader.length);

		//check track header validity
		if (trackHeader.id != requiredTrackHeaderID) {
			throw new std::runtime_error{ "Error invalid MIDI track header" };
		}

		//load track
		auto vectorLength{ trackHeader.length * 2 / sizeof(MidiSequence::MidiEventUnit) };
		std::vector<MidiSequence::MidiEventUnit> translatedTrack(vectorLength);
		int index{ 0 };
		while(true) {
			//read in delta time
			translatedTrack[index].deltaTime = readDeltaTime(inStream);

			//grab command byte
			uint8_t command{};
			inStream.read(reinterpret_cast<char*>(&command), sizeof(command));
			uint8_t maskedCommand{ static_cast<uint8_t>(command & commandMask) };

			if (maskedCommand != systemExclusive) // normal command
			{
				//read in first byte
				uint8_t temp{};
				inStream.read(reinterpret_cast<char*>(&temp), sizeof(temp));
				translatedTrack[index].event = static_cast<uint64_t>(command) |
					(static_cast<uint64_t>(temp) << 8);

				//read second byte if has one
				if (maskedCommand != programChange && maskedCommand != channelPressure){
					inStream.read(reinterpret_cast<char*>(&temp), sizeof(temp));
					translatedTrack[index].event |= (static_cast<uint64_t>(temp) << 16);
				}

				//DON'T SWAP ENDIANNESS
				//translatedTrack[index].event = byteSwap32(translatedTrack[index].event);

				++index;
			}
			else if (command == fullMetaEvent)
			{
				//read in the meta command
				inStream.read(reinterpret_cast<char*>(&command), sizeof(command));

				//read in length of meta command
				uint8_t length{};
				inStream.read(reinterpret_cast<char*>(&length), sizeof(length));

				//todo: ignoring meta events
				inStream.ignore(length);
				

				if (command == endOfTrack) {
					translatedTrack[index].event = endOfTrack;
					++index; //point to the first non-event
					break;
				}

				else {
					//todo: ignore by remove the deltaTime we set
					translatedTrack[index] = {};
				}
			}
		}
		translatedTrack.erase(translatedTrack.begin() += index, translatedTrack.end());
		translatedTrack.shrink_to_fit();

		midiSequence.translatedTrack = std::move(translatedTrack);

		return inStream;
	}
}