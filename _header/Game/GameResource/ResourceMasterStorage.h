#pragma once

#include "DirectoryStorage.h"
#include "ManifestStorage.h"
#include "BitmapStorage.h"
#include "MidiSequenceStorage.h"

namespace wasp::game::gameresource {
	struct ResourceMasterStorage {
		DirectoryStorage directoryStorage{};
		ManifestStorage manifestStorage{};
		BitmapStorage bitmapStorage{};
		MidiSequenceStorage midiSequenceStorage{};
	};
}