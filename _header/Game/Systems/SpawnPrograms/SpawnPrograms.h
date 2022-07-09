#pragma once

#include "PlayerSpawnPrograms.h"
#include "PickupSpawnPrograms.h"

namespace wasp::game::systems {

	struct SpawnPrograms {
	private:
		//fields
		resources::BitmapStorage* bitmapStoragePointer{};

	public:
		const PlayerSpawnPrograms playerSpawnPrograms;	//uninitialized!
		const PickupSpawnPrograms pickupSpawnPrograms;	//uninitialized!

		SpawnPrograms(
			resources::BitmapStorage* bitmapStoragePointer
		)
			: playerSpawnPrograms{ bitmapStoragePointer }
			, pickupSpawnPrograms{ bitmapStoragePointer } {
		}
	};
}