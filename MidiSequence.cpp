#include "MidiSequence.h"

#include <iostream>

#include "ByteSwap.h"
#include "MidiConstants.h"

namespace wasp::sound::midi {

	constexpr int trackSizeByteMultiplier = 4;

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

	static MidiSequence::EventUnitTrack loadTrack(std::istream& inStream) {
		//read in track header, assuming 1 track
		MidiTrackHeader trackHeader{ readTrackHeader(inStream) };

		//load track
		size_t vectorLength{ 
			trackHeader.length * trackSizeByteMultiplier 
			/ sizeof(MidiSequence::EventUnit) 
		};
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
					//don't advance index
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
					translatedTrack[index].deltaTime = systemExclusiveStart;
					//read binary data into our translated track
					inStream.read(
						reinterpret_cast<char*>(&translatedTrack[index]) + 1,
						length - 1 //we modified length so here we subtract 1
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

	static MidiSequence::EventUnitTrack compileTracks(
		std::vector<MidiSequence::EventUnitTrack>& individualTracks
	) {
		if (individualTracks.size() == 1) {
			return std::move(individualTracks[0]);
		}
		//create track to hold all elements in individualTracks
		size_t totalSize{ 0 };
		for (const auto& track : individualTracks) {
			totalSize += track.size();
		}
		MidiSequence::EventUnitTrack compiledTrack(totalSize);

		//initiate delta times
		std::vector<uint32_t> deltaTimes(individualTracks.size());
		for (size_t index{ 0 }; index < individualTracks.size(); ++index) {
			deltaTimes[index] = individualTracks[index][0].deltaTime;
		}

		//find and insert events by chronological order
		size_t compiledIndex{ 0 };
		uint64_t realTime{ 0 };
		std::vector<size_t> indices(individualTracks.size());
		while (individualTracks.size() > 1) {
			//find next event to insert
			uint32_t lowestDeltaTime{std::numeric_limits<uint32_t>::max()};
			size_t lowestDeltaTimeIndex{0};

			for (size_t index{ 0 }; index < individualTracks.size(); ++index) {
				if (deltaTimes[index] < lowestDeltaTime) {
					lowestDeltaTime = deltaTimes[index];
					lowestDeltaTimeIndex = index;
				}
			}

			std::cout << "lowest delta time: " << lowestDeltaTime << '\n';

			//todo: loop meta events go here
			
			//handle deltaTimes and realTime
			for (auto& deltaTime : deltaTimes) {
				deltaTime -= lowestDeltaTime;
			}
			realTime += lowestDeltaTime;

			auto& trackIndex{ indices[lowestDeltaTimeIndex] };
			auto& individualTrack{ individualTracks[lowestDeltaTimeIndex] };

			//truncate to just the status byte
			uint8_t status{ static_cast<uint8_t>(individualTrack[trackIndex].event) };

			//handle normal midi message case
			if ((status & statusMask) != 0b1111'0000) {
				//copy into our compiledTrack and advance indices
				compiledTrack[compiledIndex].deltaTime = lowestDeltaTime;
				compiledTrack[compiledIndex++].event 
					= individualTrack[trackIndex++].event;
			}
			//handle meta and sysex i.e. message with length
			else {
				//copy status block into our compiledTrack and advance indices
				compiledTrack[compiledIndex].deltaTime = lowestDeltaTime;
				compiledTrack[compiledIndex++].event
					= individualTrack[trackIndex++].event;
				//trackIndex is now pointing to the length block
				//copy the length block, store index length, and advance indices
				uint32_t indexLength{ 
					(
						compiledTrack[compiledIndex++] = individualTrack[trackIndex++]
					).event
				};
				//trackIndex is now pointing to the first block after the length
				//copy as many blocks as required by indexLength
				for (uint32_t i{ 0 }; i < indexLength; ++i) {
					compiledTrack[compiledIndex++] = individualTrack[trackIndex++];
				}
			}

			//either track is over, in which case remove
			//or update new delta time for next event on the track
			if (trackIndex >= individualTrack.size()) {
				deltaTimes.erase(deltaTimes.begin() + lowestDeltaTimeIndex);
				indices.erase(indices.begin() + lowestDeltaTimeIndex);
				individualTracks.erase(
					individualTracks.begin() + lowestDeltaTimeIndex
				);
			}
			else {
				deltaTimes[lowestDeltaTimeIndex]
					= individualTrack[trackIndex].deltaTime;
			}
		}
		//1 track left, append all into our compiled track
		auto& individualTrack{ individualTracks[0] };
		while (indices[0] < individualTrack.size()) {
			compiledTrack[compiledIndex++] = individualTrack[indices[0]++];
		}

		return compiledTrack;
	}

	std::istream& operator>>(std::istream& inStream, MidiSequence& midiSequence) {
		//read in header file
		MidiFileHeader header{ readFileHeader(inStream) };
		midiSequence.ticks = header.ticks;

		if (header.format == formatSingleTrack) {
			midiSequence.compiledTrack = loadTrack(inStream);
		}
		else if (header.format == formatMultiTrackSync) {
			std::vector<MidiSequence::EventUnitTrack> individualTracks(header.tracks);

			for (uint16_t i{ 0 }; i < header.tracks; ++i) {
				individualTracks[i] = loadTrack(inStream);
			}

			midiSequence.compiledTrack = compileTracks(individualTracks);
		}
		else {
			throw std::runtime_error("Error unsupported MIDI format");
		}

		return inStream;
	}
}