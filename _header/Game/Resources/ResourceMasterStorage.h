#pragma once

#include "DirectoryStorage.h"
#include "ManifestStorage.h"
#include "BitmapStorage.h"
#include "MidiSequenceStorage.h"
#include "DialogueStorage.h"

namespace wasp::game::resources {
	struct ResourceMasterStorage {
		DirectoryStorage directoryStorage{};
		ManifestStorage manifestStorage{};
		BitmapStorage bitmapStorage{};
		MidiSequenceStorage midiSequenceStorage{};
		DialogueStorage dialogueStorage{};
	};
}