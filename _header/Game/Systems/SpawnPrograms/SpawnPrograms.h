#pragma once

#include "PlayerSpawnPrograms.h"

namespace wasp::game::systems {

	struct SpawnPrograms {
	private:
		//fields
		resources::BitmapStorage* bitmapStoragePointer{};

	public:
		const PlayerSpawnPrograms playerSpawnPrograms;	//uninitialized!

		SpawnPrograms(
			resources::BitmapStorage* bitmapStoragePointer
		)
			: playerSpawnPrograms{ bitmapStoragePointer } {
		}
	};
}