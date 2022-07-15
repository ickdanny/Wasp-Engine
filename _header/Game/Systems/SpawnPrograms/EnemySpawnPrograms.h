#pragma once

#include "spawnProgramInclude.h"

namespace wasp::game::systems {

	class EnemySpawnPrograms {
	private:
		//typedefs
		SPAWN_PROGRAM_TYPEDEFS

		//fields
		resources::BitmapStorage* bitmapStoragePointer{};

		//prototypes
		ComponentTupleSharedPointer smallRedPrototype;			//uninitialized!
		ComponentTupleSharedPointer smallOrangePrototype;		//uninitialized!
		ComponentTupleSharedPointer smallYellowPrototype;		//uninitialized!
		ComponentTupleSharedPointer smallChartreusePrototype;	//uninitialized!
		ComponentTupleSharedPointer smallGreenPrototype;		//uninitialized!
		ComponentTupleSharedPointer smallSpringPrototype;		//uninitialized!
		ComponentTupleSharedPointer smallCyanPrototype;			//uninitialized!
		ComponentTupleSharedPointer smallAzurePrototype;		//uninitialized!
		ComponentTupleSharedPointer smallBluePrototype;			//uninitialized!
		ComponentTupleSharedPointer smallVioletPrototype;		//uninitialized!
		ComponentTupleSharedPointer smallMagentaPrototype;		//uninitialized!
		ComponentTupleSharedPointer smallRosePrototype;			//uninitialized!

		ComponentTupleSharedPointer mediumRedPrototype;			//uninitialized!
		ComponentTupleSharedPointer mediumOrangePrototype;		//uninitialized!
		ComponentTupleSharedPointer mediumYellowPrototype;		//uninitialized!
		ComponentTupleSharedPointer mediumChartreusePrototype;	//uninitialized!
		ComponentTupleSharedPointer mediumGreenPrototype;		//uninitialized!
		ComponentTupleSharedPointer mediumSpringPrototype;		//uninitialized!
		ComponentTupleSharedPointer mediumCyanPrototype;		//uninitialized!
		ComponentTupleSharedPointer mediumAzurePrototype;		//uninitialized!
		ComponentTupleSharedPointer mediumBluePrototype;		//uninitialized!
		ComponentTupleSharedPointer mediumVioletPrototype;		//uninitialized!
		ComponentTupleSharedPointer mediumMagentaPrototype;		//uninitialized!
		ComponentTupleSharedPointer mediumRosePrototype;		//uninitialized!

		ComponentTupleSharedPointer largeRedPrototype;			//uninitialized!
		ComponentTupleSharedPointer largeOrangePrototype;		//uninitialized!
		ComponentTupleSharedPointer largeYellowPrototype;		//uninitialized!
		ComponentTupleSharedPointer largeChartreusePrototype;	//uninitialized!
		ComponentTupleSharedPointer largeGreenPrototype;		//uninitialized!
		ComponentTupleSharedPointer largeSpringPrototype;		//uninitialized!
		ComponentTupleSharedPointer largeCyanPrototype;			//uninitialized!
		ComponentTupleSharedPointer largeAzurePrototype;		//uninitialized!
		ComponentTupleSharedPointer largeBluePrototype;			//uninitialized!
		ComponentTupleSharedPointer largeVioletPrototype;		//uninitialized!
		ComponentTupleSharedPointer largeMagentaPrototype;		//uninitialized!
		ComponentTupleSharedPointer largeRosePrototype;			//uninitialized!

		ComponentTupleSharedPointer sharpRedPrototype;			//uninitialized!
		ComponentTupleSharedPointer sharpOrangePrototype;		//uninitialized!
		ComponentTupleSharedPointer sharpYellowPrototype;		//uninitialized!
		ComponentTupleSharedPointer sharpChartreusePrototype;	//uninitialized!
		ComponentTupleSharedPointer sharpGreenPrototype;		//uninitialized!
		ComponentTupleSharedPointer sharpSpringPrototype;		//uninitialized!
		ComponentTupleSharedPointer sharpCyanPrototype;			//uninitialized!
		ComponentTupleSharedPointer sharpAzurePrototype;		//uninitialized!
		ComponentTupleSharedPointer sharpBluePrototype;			//uninitialized!
		ComponentTupleSharedPointer sharpVioletPrototype;		//uninitialized!
		ComponentTupleSharedPointer sharpMagentaPrototype;		//uninitialized!
		ComponentTupleSharedPointer sharpRosePrototype;			//uninitialized!

		//naming convention:
		//s		- stage 
		//n		- stage number
		//x		- type (d-danmaku, e-enemy)
		//n		- id number
		

		// STAGE 1 // STAGE 1 // STAGE 1 // STAGE 1 // STAGE 1 // STAGE 1 // STAGE 1 //

		SpawnNodeSharedPointer s1d1Node;						//uninitialized!
		SpawnProgram s1d1;										//uninitialized!
		ComponentTupleSharedPointer s1e1Prototype;				//uninitialized!
		SpawnNodeSharedPointer s1e1Node;						//uninitialized!

	public:

		SpawnProgram s1e1;										//uninitialized!

		//Constructors
		EnemySpawnPrograms(resources::BitmapStorage* bitmapStoragePointer);
	};
}