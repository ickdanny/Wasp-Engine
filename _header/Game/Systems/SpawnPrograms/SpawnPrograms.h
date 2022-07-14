#pragma once

#include "PlayerSpawnPrograms.h"
#include "PickupSpawnPrograms.h"
#include "EnemySpawnPrograms.h"

namespace wasp::game::systems {

	struct SpawnPrograms {
	private:
		//fields
		resources::BitmapStorage* bitmapStoragePointer{};

	public:
		const PlayerSpawnPrograms playerSpawnPrograms;	//uninitialized!
		const PickupSpawnPrograms pickupSpawnPrograms;	//uninitialized!
		const EnemySpawnPrograms enemySpawnPrograms;	//uninitialized!

		SpawnPrograms(
			resources::BitmapStorage* bitmapStoragePointer
		)
			: playerSpawnPrograms{ bitmapStoragePointer }
			, pickupSpawnPrograms{ bitmapStoragePointer }
			, enemySpawnPrograms{ bitmapStoragePointer } 
		{
		}
	};
}