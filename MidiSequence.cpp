#include "MidiSequence.h"

#include "ByteSwap.h"
#include "MidiConstants.h"

namespace wasp::sound::midi {

	using wasp::utility::byteSwap16;
	using wasp::utility::byteSwap32;
	//using wasp::utility::byteSwap64;

	using namespace wasp::sound::midi::constants;

	#pragma pack(push, 1)
	struct MidiFileHeader {
		uint32_t id{};		// identifier "MThd"
		uint32_t size{};	// always 6 in big-endian format
		uint16_t format{};	// big-endian format
		uint16_t tracks{};	// number of tracks, big-endian
		uint16_t ticks{};	// number of ticks per quarter note, big-endian
	};
	#pragma pack(pop)

	#pragma pack(push, 1)
	struct MidiTrackHeader {
		uint32_t id{};		// identifier "MTrk"
		uint32_t length{};	// track length, big-endian
	};
	#pragma pack(pop)

	static MidiFileHeader readFileHeader(std::istream& inStream) {
		MidiFileHeader header{};
		//read in file header
		inStream.read(
			reinterpret_cast<char*>(&header),
			sizeof(MidiFileHeader)
		);

		//swap file header endianness
		header.id = byteSwap32(header.id);
		header.size = byteSwap32(header.size);
		header.format = byteSwap16(header.format);
		header.tracks = byteSwap16(header.tracks);
		header.ticks = byteSwap16(header.ticks);

		//check file header validity
		if (header.id != requiredHeaderID) {
			throw new std::runtime_error{ "Error MIDI file invalid header identifier" };
		}
		if (header.size < minimumHeaderSize) {
			throw new std::runtime_error{ "Error MIDI file header too small" };
		}

		//handle the case when the file header is longer than expected
		if (header.size > minimumHeaderSize) {
			inStream.ignore(header.size - minimumHeaderSize);
		}

		//todo: we block multi-track here
		if (header.format != formatSingleTrack) {
			throw new std::runtime_error{ "for this test only take single track" };
		}

		return header;
	}

	static MidiTrackHeader readTrackHeader(std::istream& inStream) {
		MidiTrackHeader header{};
		//read in header
		inStream.read(
			reinterpret_cast<char*>(&header),
			sizeof(MidiTrackHeader)
		);

		//swap track header endianness
		header.id = byteSwap32(header.id);
		header.length = byteSwap32(header.length);

		//check track header validity
		if (header.id != requiredTrackHeaderID) {
			throw new std::runtime_error{ "Error MIDI file invalid track identifier" };
		}

		return header;
	}

	static uint32_t readVariableLength(std::istream& inStream) {
		uint32_t toRet{};
		uint8_t byte{};

		//read variable length loop
		do {
			inStream.read(reinterpret_cast<char*>(&byte), sizeof(byte));
			toRet = (toRet << 7) + (byte & 0b0111'1111);
		} while (byte & 0b1000'0000);

		//DON'T SWAP ENDIANNESS
		//toRet = byteSwap32(deltaTime);

		return toRet;
	}

	template<typename T>
	static T ceilingIntegerDivide(T x, T y) {
		return x / y + (x % y != 0);
	}

	//encoded as byteLength / indexLength
	static MidiSequence::EventUnit encodeLength(uint32_t byteLength) {
		MidiSequence::EventUnit eventUnit{};
		eventUnit.deltaTime = byteLength;
		uint32_t indexLength{ ceilingIntegerDivide(
			byteLength,
			static_cast<uint32_t>(sizeof(MidiSequence::EventUnit))
		) };
		eventUnit.event = indexLength;
		return eventUnit;
	}

	static std::vector<MidiSequence::EventUnit> loadTrack(std::istream& inStream) {
		//read in track header, assuming 1 track
		MidiTrackHeader trackHeader{ readTrackHeader(inStream) };

		//load track
		size_t vectorLength{ trackHeader.length * 3 / sizeof(MidiSequence::EventUnit) };
		std::vector<MidiSequence::EventUnit> translatedTrack(vectorLength);

		uint8_t lastStatus{ 0 };
		uint32_t index{ 0 };
		bool encounteredEndOfTrack{ false };
		while (!encounteredEndOfTrack) {
			//read in delta time
			translatedTrack[index].deltaTime = readVariableLength(inStream);

			//grab command byte
			uint8_t status{};
			inStream.read(reinterpret_cast<char*>(&status), sizeof(status));

			//handle running status
			if (status < 0b1000'0000) {
				status = lastStatus;
				inStream.seekg(-1, std::ios_base::cur);
			}

			uint8_t maskedStatus{ static_cast<uint8_t>(status & statusMask) };
			//handle midi events
			if (maskedStatus != 0b1111'0000) {

				//read in first byte
				uint8_t temp{};
				inStream.read(reinterpret_cast<char*>(&temp), sizeof(temp));
				translatedTrack[index].event = static_cast<uint32_t>(status) |
					(static_cast<uint32_t>(temp) << 8);

				//read second byte if has one
				if (maskedStatus != programChange && maskedStatus != channelPressure) {
					inStream.read(reinterpret_cast<char*>(&temp), sizeof(temp));
					translatedTrack[index].event |= (static_cast<uint32_t>(temp) << 16);
				}

				//DON'T SWAP ENDIANNESS
				//translatedTrack[index].event = byteSwap32(translatedTrack[index].event);

				lastStatus = status;
				++index;
			}
			//handle meta events
			else if (status == metaEvent) {
				//read in the meta event
				uint8_t metaEventStatus{};
				inStream.read(
					reinterpret_cast<char*>(&metaEventStatus),
					sizeof(metaEventStatus)
				);

				//read in length 
				uint32_t length = readVariableLength(inStream);

				//do not insert end of track events into our translated track
				if (metaEventStatus == endOfTrack) {
					//remove the delta time we set at the start
					translatedTrack[index] = {};
					encounteredEndOfTrack = true;
				}

				//insert everything else
				else {
					//first block = deltaTime / 00 - 00 - event - FF
					translatedTrack[index++].event = (metaEventStatus << 8) | status;

					//second block = length / index length
					translatedTrack[index] = encodeLength(length);
					uint32_t indexLength{ translatedTrack[index++].event };

					if (length > 0) {
						//read binary data into our translated track
						inStream.read(
							reinterpret_cast<char*>(&translatedTrack[index]),
							length
						);
						//advance index by necessary amount
						index += indexLength;
					}
				}
				lastStatus = 0;
			}
			//handle system exclusive events
			else if (status == systemExclusiveStart) {
				//read in length
				uint32_t length = readVariableLength(inStream);

				//first block = deltaTime / 00 - 00 - 00 - F0
				translatedTrack[index++].event = status;

				//second block = length / index length
				//add 1 to length for our inserted F0 byte
				if (length > 0) {
					++length;
				}
				translatedTrack[index] = encodeLength(length);
				uint32_t indexLength{ translatedTrack[index++].event };

				if (length > 0) {
					//insert F0 at beginning of data dump
					translatedTrack[index].deltaTime = systemExclusiveStart << 24;
					//read binary data into our translated track
					inStream.read(
						reinterpret_cast<char*>(&translatedTrack[index]) + 1,
						length
					);
					//advance index by necessary amount
					index += indexLength;
				}
				lastStatus = 0;
			}
			else if (status == systemExclusiveEnd) {
				//irrelevant whether continuation packet or escape sequence

				//read in length 
				uint32_t length = readVariableLength(inStream);

				//first block = deltaTime / 00 - 00 - 00 - F7
				translatedTrack[index].event = status;
				++index;

				//second block = length / index length
				translatedTrack[index] = encodeLength(length);
				uint32_t indexLength{ translatedTrack[index++].event };

				if (length > 0) {
					//read binary data into our translated track
					inStream.read(
						reinterpret_cast<char*>(&translatedTrack[index]),
						length
					);
					//advance index by necessary amount
					index += indexLength;
				}
				lastStatus = 0;
			}
		}

		translatedTrack.erase(translatedTrack.begin() += index, translatedTrack.end());
		translatedTrack.shrink_to_fit();

		return translatedTrack;
	}

	std::istream& operator>>(std::istream& inStream, MidiSequence& midiSequence) {
		//read in header file
		MidiFileHeader header{ readFileHeader(inStream) };
		midiSequence.ticks = header.ticks;

		midiSequence.compiledTrack = loadTrack(inStream);

		return inStream;
	}
}