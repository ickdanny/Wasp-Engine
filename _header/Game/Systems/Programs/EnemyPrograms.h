#pragma once

#include "programInclude.h"
#include "PickupPrograms.h"

namespace wasp::game::systems {

	class EnemyPrograms {
	private:
		//typedefs
		SPAWN_PROGRAM_TYPEDEFS

		//fields
		resources::BitmapStorage* bitmapStoragePointer{};
		PickupPrograms const* pickupProgramsPointer{};

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

		//clear for bosses
		ScriptNodeSharedPointer fieldClearScriptNode;			//uninitialized!
		ComponentTupleSharedPointer fieldClearPrototype;		//uninitialized!
		SpawnNodeSharedPointer fieldClearNode;					//uninitialized!

		SpawnProgram fieldClearProgram;							//uninitialized!

		//naming convention:
		//s		- stage 
		//1-5	- stage number
		//x		- type (d-danmaku, e-enemy)
		//n		- id number
		//a-z	- sub id
		

		// STAGE 1 // STAGE 1 // STAGE 1 // STAGE 1 // STAGE 1 // STAGE 1 // STAGE 1 //

			//wave 1: 2 wisps on either side, ring of sharp cyans
		SpawnNodeSharedPointer s1d1Node;						//uninitialized!
		SpawnProgram s1d1;										//uninitialized!

				//first wisp on the left
		ComponentTupleSharedPointer s1e1aPrototype;				//uninitialized!
		SpawnNodeSharedPointer s1e1aNode;						//uninitialized!
		SpawnProgram s1e1a;										//uninitialized!

				//second wisp on the right
		ComponentTupleSharedPointer s1e1bPrototype;				//uninitialized!
		SpawnNodeSharedPointer s1e1bNode;						//uninitialized!
		SpawnProgram s1e1b;										//uninitialized!

			//wave 2: bats from the left, shooting small bullets, and central wisp
			
				//bats
		SpawnNodeSharedPointer s1d2aNode;						//uninitialized!
		SpawnProgram s1d2a;										//uninitialized!

		ComponentTupleSharedPointer s1e2aPrototype;				//uninitialized!
		SpawnNodeSharedPointer s1e2aNode;						//uninitialized!
		SpawnProgram s1e2a;										//uninitialized!

				//central wisp
		ComponentTupleSharedPointer s1e2bPrototype;				//uninitialized!
		SpawnNodeSharedPointer s1e2bNode;						//uninitialized!
		SpawnProgram s1e2b;										//uninitialized!

			//wave 3: 2 wisps from the right, alternating spirals
		SpawnNodeSharedPointer s1d3aNode;						//uninitialized!
		SpawnNodeSharedPointer s1d3bNode;						//uninitialized!
		SpawnProgram s1d3a;										//uninitialized!
		SpawnProgram s1d3b;										//uninitialized!

		ComponentTupleSharedPointer s1e3aPrototype;				//uninitialized!
		ComponentTupleSharedPointer s1e3bPrototype;				//uninitialized!
		SpawnNodeSharedPointer s1e3Node;						//uninitialized!
		SpawnProgram s1e3;										//uninitialized!

			//wave 4: 2 wisps on either side, rings of sharp cyan
		SpawnNodeSharedPointer s1d4Node;						//uninitialized!
		SpawnProgram s1d4;										//uninitialized!

				//first wisp on the right
		ComponentTupleSharedPointer s1e4aPrototype;				//uninitialized!
		SpawnNodeSharedPointer s1e4aNode;						//uninitialized!
		SpawnProgram s1e4a;										//uninitialized!

				//second wisp on the left
		ComponentTupleSharedPointer s1e4bPrototype;				//uninitialized!
		SpawnNodeSharedPointer s1e4bNode;						//uninitialized!
		SpawnProgram s1e4b;										//uninitialized!

			//wave 5: single spiral from the left
		SpawnNodeSharedPointer s1d5Node;						//uninitialized!
		SpawnProgram s1d5;										//uninitialized!

		ComponentTupleSharedPointer s1e5Prototype;				//uninitialized!
		SpawnNodeSharedPointer s1e5Node;						//uninitialized!
		SpawnProgram s1e5;										//uninitialized!

			//wave 6: more bats
		SpawnNodeSharedPointer s1e6Node;						//uninitialized!
		SpawnProgram s1e6;										//uninitialized!

			//wave 7: simultaneous rings of sharp cyan from left and right
		SpawnNodeSharedPointer s1d7Node;						//uninitialized!
		SpawnProgram s1d7;										//uninitialized!

		ComponentTupleSharedPointer s1e7aPrototype;				//uninitialized!
		ComponentTupleSharedPointer s1e7bPrototype;				//uninitialized!
		SpawnNodeSharedPointer s1e7Node;						//uninitialized!
		SpawnProgram s1e7;

			//wave 8: wisp with spiral of arc columns from the right
		SpawnNodeSharedPointer s1d8Node;						//uninitialized!
		SpawnProgram s1d8;										//uninitialized!

		ComponentTupleSharedPointer s1e8Prototype;				//uninitialized!
		SpawnNodeSharedPointer s1e8Node;						//uninitialized!
		SpawnProgram s1e8;										//uninitialized!

			//wave 9: bats down middle then to sides
		ComponentTupleSharedPointer s1e9aPrototype;				//uninitialized!
		ComponentTupleSharedPointer s1e9bPrototype;				//uninitialized!
		SpawnNodeSharedPointer s1e9Node;						//uninitialized!
		SpawnProgram s1e9;										//uninitialized!

			//wave 10: 3 wisps from the left spiraling
		SpawnNodeSharedPointer s1d10aNode;						//uninitialized!
		SpawnNodeSharedPointer s1d10bNode;						//uninitialized!
		SpawnNodeSharedPointer s1d10cNode;						//uninitialized!
		SpawnProgram s1d10a;									//uninitialized!
		SpawnProgram s1d10b;									//uninitialized!
		SpawnProgram s1d10c;									//uninitialized!

		ComponentTupleSharedPointer s1e10aPrototype;			//uninitialized!
		ComponentTupleSharedPointer s1e10bPrototype;			//uninitialized!
		ComponentTupleSharedPointer s1e10cPrototype;			//uninitialized!
		SpawnNodeSharedPointer s1e10Node;						//uninitialized!
		SpawnProgram s1e10;										//uninitialized!

			//wave 11: simultaneous rings of sharp cyan, this time 1 shot
		ComponentTupleSharedPointer s1e11aPrototype;			//uninitialized!
		ComponentTupleSharedPointer s1e11bPrototype;			//uninitialized!
		SpawnNodeSharedPointer s1e11Node;						//uninitialized!
		SpawnProgram s1e11;										//uninitialized!

			//wave 12: same thing but closer to the center
		SpawnNodeSharedPointer s1e12Node;						//uninitialized!
		SpawnProgram s1e12;										//uninitialized!

			//wave 13: bats from left
		SpawnProgram s1e13;		//use s1e2aNode					//uninitialized!

			//wave 14 and 15: same as 11 and 12

			//wave 16: bats from right
		SpawnNodeSharedPointer s1e16Node;						//uninitialized!
		SpawnProgram s1e16;

		// BOSS 1 // BOSS 1 // BOSS 1 // BOSS 1 // BOSS 1 // BOSS 1 // BOSS 1 // 

			//1 6-symmetry counter spiral
		SpawnNodeSharedPointer b1d1Node;						//uninitialized!
		SpawnProgram b1d1;										//uninitialized!

			//2 rain + large aimed
		SpawnNodeSharedPointer b1d2Node;						//uninitialized!
		SpawnProgram b1d2;										//uninitialized!

			//3 3-symmetry 2-symmetry spirals
		SpawnNodeSharedPointer b1d3Node;						//uninitialized!
		SpawnProgram b1d3;										//uninitialized!

			//4 snowflakes
		ComponentTupleSharedPointer b1d4Prototype;				//uninitialized!
		SpawnNodeSharedPointer b1d4Node;						//uninitialized!
		SpawnProgram b1d4;

			//the boss
		ComponentTupleSharedPointer b1e1Prototype;				//uninitialized!
		SpawnNodeSharedPointer b1e1Node;						//uninitialized!
		SpawnProgram b1e1;										//uninitialized!


		// STAGE 2 // STAGE 2 // STAGE 2 // STAGE 2 // STAGE 2 // STAGE 2 // STAGE 2 //

			//wave 1: mid wisp whip ring
		SpawnNodeSharedPointer s2d1Node;						//uninitialized!
		SpawnProgram s2d1;										//uninitialized!

		ComponentTupleSharedPointer s2e1Prototype;				//uninitialized!
		SpawnNodeSharedPointer s2e1Node;						//uninitialized!
		SpawnProgram s2e1;										//uninitialized!

			//wave 2: wisp on the left 
		SpawnNodeSharedPointer s2e2Node;						//uninitialized!
		SpawnProgram s2e2;										//uninitialized!

			//wave 3: flames from both sides
		SpawnNodeSharedPointer s2d3aNode;						//uninitialized!
		SpawnProgram s2d3a;										//uninitialized!
		ComponentTupleSharedPointer s2e3aPrototype;				//uninitialized!

		SpawnNodeSharedPointer s2d3bNode;						//uninitialized!
		SpawnProgram s2d3b;										//uninitialized!
		ComponentTupleSharedPointer s2e3bPrototype;				//uninitialized!

		SpawnNodeSharedPointer s2e3Node;						//uninitialized!

		SpawnProgram s2e3;										//uninitialized!

			//wave 4: wisp to the right, spiral 
		SpawnNodeSharedPointer s2d4Node;						//uninitialized!
		SpawnProgram s2d4;										//uninitialized!
		
		ComponentTupleSharedPointer s2e4Prototype;				//uninitialized!
		SpawnNodeSharedPointer s2e4Node;						//uninitialized!
		SpawnProgram s2e4;										//uninitialized!
			
			//wave 5: shell chain
		SpawnNodeSharedPointer s2d5Node;						//uninitialized!
		SpawnProgram s2d5;										//uninitialized!

		ComponentTupleSharedPointer s2e5Prototype;				//uninitialized!
		SpawnNodeSharedPointer s2e5Node;						//uninitialized!
		SpawnProgram s2e5;										//uninitialized!

			//wave 6: other side shell chain + wisp
		ComponentTupleSharedPointer s2e6aPrototype;				//uninitialized!
		SpawnNodeSharedPointer s2e6aNode;						//uninitialized!
		SpawnProgram s2e6a;										//uninitialized!

				//wisp w/ whip spiral
		SpawnNodeSharedPointer s2d6bNode;						//uninitialized!
		SpawnProgram s2d6b;										//uninitialized!

		ComponentTupleSharedPointer s2e6bPrototype;				//uninitialized!
		SpawnNodeSharedPointer s2e6bNode;						//uninitialized!
		SpawnProgram s2e6b;										//uninitialized!

			//wave 7: flames again
		SpawnNodeSharedPointer s2e7Node;						//uninitialized!
		SpawnProgram s2e7;										//uninitialized!

			//wave 8: wisp whip rings
		SpawnNodeSharedPointer s2d8Node;						//uninitialized!
		SpawnProgram s2d8;										//uninitialized!

		ComponentTupleSharedPointer s2e8Prototype;				//uninitialized!
		SpawnNodeSharedPointer s2e8aNode;						//uninitialized!
		SpawnProgram s2e8a;										//uninitialized!

		SpawnNodeSharedPointer s2e8bNode;						//uninitialized!
		SpawnProgram s2e8b;										//uninitialized!

		SpawnNodeSharedPointer s2e8cNode;						//uninitialized!
		SpawnProgram s2e8c;										//uninitialized!

			//wave 9: double spiral from 2 sides
		SpawnNodeSharedPointer s2d9aNode;						//uninitialized!
		SpawnProgram s2d9a;										//uninitialized!
		SpawnNodeSharedPointer s2d9bNode;						//uninitialized!
		SpawnProgram s2d9b;										//uninitialized!

		ComponentTupleSharedPointer s2e9aPrototype;				//uninitialized!
		ComponentTupleSharedPointer s2e9bPrototype;				//uninitialized!
		SpawnNodeSharedPointer s2e9Node;						//uninitialized!
		SpawnProgram s2e9;										//uninitialized!

			//wave 10: flames with slow barrage; yellow and green
		SpawnNodeSharedPointer s2d10aNode;						//uninitialized!
		SpawnProgram s2d10a;									//uninitialized!
		SpawnNodeSharedPointer s2d10bNode;						//uninitialized!
		SpawnProgram s2d10b;									//uninitialized!

		ComponentTupleSharedPointer s2e10aPrototype;			//uninitialized!
		ComponentTupleSharedPointer s2e10bPrototype;			//uninitialized!
		SpawnNodeSharedPointer s2e10Node;						//uninitialized!
		SpawnProgram s2e10;										//uninitialized!

			//wave 10: shells down sides and out
		ComponentTupleSharedPointer s2e11aPrototype;			//uninitialized!
		ComponentTupleSharedPointer s2e11bPrototype;			//uninitialized!
		SpawnNodeSharedPointer s2e11Node;						//uninitialized!
		SpawnProgram s2e11;										//uninitialized!

			//wave 11: double spiral wisp super fast
		SpawnNodeSharedPointer s2d12Node;						//uninitialized!
		SpawnProgram s2d12;										//uninitialized!

		ComponentTupleSharedPointer s2e12Prototype;				//uninitialized!
		SpawnNodeSharedPointer s2e12Node;						//uninitialized!
		SpawnProgram s2e12;										//uninitialized!

		// BOSS 2 // BOSS 2 // BOSS 2 // BOSS 2 // BOSS 2 // BOSS 2 // BOSS 2 //
		
			//1 arrows / slow medium
		ComponentTupleSharedPointer b2d1aPrototype;				//uninitialized!
		SpawnNodeSharedPointer b2d1aNode;						//uninitialized!
		SpawnProgram b2d1a;										//uninitialized!

		SpawnNodeSharedPointer b2d1bNode;						//uninitialized!
		SpawnProgram b2d1b;										//uninitialized!

			//2 bullets that spawn horizontal sharps
		ComponentTupleSharedPointer b2d2aPrototype;	//spawned	//uninitialized!
		SpawnNodeSharedPointer b2d2aNode;						//uninitialized!
		SpawnProgram b2d2a;										//uninitialized!

		ComponentTupleSharedPointer b2d2bPrototype;	//spawner 	//uninitialized!
		SpawnNodeSharedPointer b2d2bNode;						//uninitialized!
		SpawnProgram b2d2b;										//uninitialized!

			//3 fast spiral, 2 directions
		ComponentTupleSharedPointer b2d3aPrototype;				//uninitialized!
		ComponentTupleSharedPointer b2d3bPrototype;				//uninitialized!
		SpawnNodeSharedPointer b2d3aNode;						//uninitialized!
		SpawnProgram b2d3a;										//uninitialized!
		SpawnNodeSharedPointer b2d3bNode;						//uninitialized!
		SpawnProgram b2d3b;										//uninitialized!

		ScriptNodeSharedPointer b2d3ActScriptEnd;				//uninitialized!
		ScriptNodeSharedPointer b2d3ActScriptStart;				//uninitialized!

			//4 slow spiral + homing arrows
		ComponentTupleSharedPointer b2d4bPrototype;				//uninitialized!
		SpawnNodeSharedPointer b2d4Node;						//uninitialized!
		SpawnProgram b2d4;										//uninitialized!

			//5 mega arrow -> rings
		ComponentTupleSharedPointer b2d5aPrototype;	//spawned	//uninitialized!
		SpawnNodeSharedPointer b2d5aNode;						//uninitialized!
		SpawnProgram b2d5a;										//uninitialized!

		ComponentTupleSharedPointer b2d5bPrototype;	//spwaner	//uninitialized!
		SpawnNodeSharedPointer b2d5bNode;						//uninitialized!
		SpawnProgram b2d5b;										//uninitialized!

			//the boss
		ComponentTupleSharedPointer b2e1Prototype;				//uninitialized!
		SpawnNodeSharedPointer b2e1Node;						//uninitialized!
		SpawnProgram b2e1;										//uninitialized!

		// STAGE 3 // STAGE 3 // STAGE 3 // STAGE 3 // STAGE 3 // STAGE 3 // STAGE 3 //

			//wave 1: 3 pairs of wisps (?) random spray shot
		SpawnNodeSharedPointer s3d1aNode;						//uninitialized!
		SpawnNodeSharedPointer s3d1bNode;						//uninitialized!
		SpawnNodeSharedPointer s3d1cNode;						//uninitialized!
		SpawnProgram s3d1a;										//uninitialized!
		SpawnProgram s3d1b;										//uninitialized!
		SpawnProgram s3d1c;										//uninitialized!

		ComponentTupleSharedPointer s3e1aPrototype;				//uninitialized!
		ComponentTupleSharedPointer s3e1bPrototype;				//uninitialized!
		ComponentTupleSharedPointer s3e1cPrototype;				//uninitialized!
		SpawnNodeSharedPointer s3e1aNode;						//uninitialized!
		SpawnNodeSharedPointer s3e1bNode;						//uninitialized!
		SpawnNodeSharedPointer s3e1cNode;						//uninitialized!
		SpawnProgram s3e1a;										//uninitialized!
		SpawnProgram s3e1b;										//uninitialized!
		SpawnProgram s3e1c;										//uninitialized!

	public:

		ScriptProgram stage1ScriptProgram;						//uninitialized!
		ScriptProgram stage2ScriptProgram;						//uninitialized!
		ScriptProgram stage3ScriptProgram;						//uninitialized!
		//ScriptProgram stage4ScriptProgram;						//uninitialized!
		//ScriptProgram stage5ScriptProgram;						//uninitialized!

		//Constructors
		EnemyPrograms(
			resources::BitmapStorage* bitmapStoragePointer,
			PickupPrograms const* pickupProgramsPointer
		);
	};
}