#pragma once

#include "programInclude.h"

namespace wasp::game::systems {

	class PickupPrograms {
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
		ScriptNodeSharedPointer maxPowerClearScriptNode;	//uninitialized!
		ComponentTupleSharedPointer maxPowerClearPrototype;	//uninitialized!
		SpawnNodeSharedPointer spawnMaxPowerClearNode;		//uninitialized!

	public:
		const SpawnProgram smallPowerSpawnProgram;			//uninitialized!
		const SpawnProgram largePowerSpawnProgram;			//uninitialized!
		const SpawnProgram lifeSpawnProgram;				//uninitialized!
		const SpawnProgram bombSpawnProgram;				//uninitialized!
		const SpawnProgram maxPowerClearSpawnProgram;		//uninitialized!

		//Constructors
		PickupPrograms(resources::BitmapStorage* bitmapStoragePointer);
	};
}