#pragma once

#include "spawnProgramInclude.h"

namespace wasp::game::systems {

	class PickupSpawnPrograms {
	private:
		//typedefs
		SPAWN_PROGRAM_TYPEDEFS

		//fields
		resources::BitmapStorage* bitmapStoragePointer{};

		//programs
		ScriptNodeSharedPointer pickupScriptNode;			//uninitialized!

		//pickups
		ComponentTupleSharedPointer smallPowerPrototype;	//uninitialized!
		SpawnNodeSharedPointer spawnSmallPowerNode;			//uninitialized!
		ComponentTupleSharedPointer largePowerPrototype;	//uninitialized!
		SpawnNodeSharedPointer spawnLargePowerNode;			//uninitialized!
		ComponentTupleSharedPointer lifePrototype;			//uninitialized!
		SpawnNodeSharedPointer spawnLifeNode;				//uninitialized!
		ComponentTupleSharedPointer bombPrototype;			//uninitialized!
		SpawnNodeSharedPointer spawnBombNode;				//uninitialized!

		//clear for max power
		ScriptNodeSharedPointer clearScriptNode;			//uninitialized!
		ComponentTupleSharedPointer clearPrototype;			//uninitialized!
		SpawnNodeSharedPointer spawnClearNode;				//uninitialized!

	public:
		const SpawnProgram smallPower;						//uninitialized!
		const SpawnProgram largePower;						//uninitialized!
		const SpawnProgram life;							//uninitialized!
		const SpawnProgram bomb;							//uninitialized!
		const SpawnProgram clear;							//uninitialized!

		//Constructors
		PickupSpawnPrograms(
			resources::BitmapStorage* bitmapStoragePointer
		);
	};
}