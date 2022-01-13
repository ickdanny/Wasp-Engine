#pragma once

#include <cstdint>

namespace wasp::sound::midi::constants {
	//all midi files should begin with "MThd"
	constexpr uint32_t requiredHeaderID{ 0x4d546864 };
	//currently all midi files have a header size of 6
	constexpr uint32_t minimumHeaderSize{ 0x00000006 };

	enum fileFormat : std::uint16_t {
		formatSingleTrack = 0x0000,
		formatMultiTrackSync = 0x0001,
		formatMultiTrackAsync = 0x0002
	};

	//all midi tracks should begin with "MTrk"
	constexpr uint32_t requiredTrackHeaderID{ 0x4d54726b };

	//status codes
	constexpr uint8_t statusMask{ 0b1111'0000 };
	//midi event last 4 bits are channel #
	enum midiEvents : uint8_t {
		noteOff =					0b1000'0000,
		noteOn =					0b1001'0000,
		polyphonicKeyPressure =		0b1010'0000,
		controlChange =				0b1011'0000,
		programChange =				0b1100'0000,
		channelPressure =			0b1101'0000,
		pitchBendChange =			0b1110'0000
	};
	//meta events should not be sent to the synth
	constexpr uint8_t metaEvent{ 0b1111'1111 };
	//system exclusive events should be sent to the synth including the byte F0
	constexpr uint8_t systemExclusiveStart{ 0b1111'0000 };
	//used to denote both continuation events as well as escape sequences
	constexpr uint8_t systemExclusiveEnd{ 0b1111'0111 };

	//meta events have a secondary status code
	enum metaEvents : uint8_t {
		sequenceNumber =		0b0000'0000,
		text =					0b0000'0001,
		copyright =				0b0000'0010,
		sequenceOrTrackName =	0b0000'0011,
		instrumentName =		0b0000'0100,
		lyric =					0b0000'0101,
		marker =				0b0000'0110,
		cuePoint =				0b0000'0111,
		programName =			0b0000'1000,
		deviceName =			0b0000'1001,
		midiChannelPrefix =		0b0010'0000,
		midiPort =				0b0010'0001,
		endOfTrack =			0b0010'1111,
		tempo =					0b0101'0001,
		smpteOffset =			0b0101'0100,
		timeSignature =			0b0101'1000,
		keySignature =			0b0101'1001,
		sequencerSpecific =		0b0101'1001
	};

	//midi defaults to 120 BPM
	constexpr uint32_t defaultMicrosecondsPerBeat{ 500'000 };
}