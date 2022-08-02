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

			//2 large bullets that spawn horizontal sharps
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

			//wave 2: loop flames -> spine shot and drop shot
		SpawnNodeSharedPointer s3d2Node;						//uninitialized!
		SpawnProgram s3d2;										//uninitialized!
		
		ComponentTupleSharedPointer s3e2Prototype;				//uninitialized!
		SpawnNodeSharedPointer s3e2Node;						//uninitialized!
		SpawnProgram s3e2;										//uninitialized!

			//wave 3: clouds from the left, column rings
		SpawnNodeSharedPointer s3d3Node;						//uninitialized!
		SpawnProgram s3d3;										//uninitialized!

		ComponentTupleSharedPointer s3e3Prototype;				//uninitialized!
		SpawnNodeSharedPointer s3e3Node;						//uninitialized!
		SpawnProgram s3e3;										//uninitialized!

			//wave 4: same thing but from right and red
		SpawnNodeSharedPointer s3d4Node;						//uninitialized!
		SpawnProgram s3d4;										//uninitialized!

		ComponentTupleSharedPointer s3e4Prototype;				//uninitialized!
		SpawnNodeSharedPointer s3e4Node;						//uninitialized!
		SpawnProgram s3e4;										//uninitialized!

			//wave 5: some more bats but from the left
		ComponentTupleSharedPointer s3e5Prototype;				//uninitialized!
		SpawnNodeSharedPointer s3e5Node;						//uninitialized!
		SpawnProgram s3e5;										//uninitialized!

			//wave 6: same thing as wave 1

			//wave 7: clouds from right, blue rings this time
		SpawnNodeSharedPointer s3e7Node;						//uninitialized!
		SpawnProgram s3e7;										//uninitialized!

			//wave 8: random spread from the left
		ComponentTupleSharedPointer s3e8Prototype;				//uninitialized!
		SpawnNodeSharedPointer s3e8aNode;						//uninitialized!
		SpawnNodeSharedPointer s3e8bNode;						//uninitialized!
		SpawnNodeSharedPointer s3e8cNode;						//uninitialized!
		SpawnProgram s3e8a;										//uninitialized!
		SpawnProgram s3e8b;										//uninitialized!
		SpawnProgram s3e8c;										//uninitialized!

			//wave 9: midboss 5 clouds, random spray (middle drops life)
				//side
		SpawnNodeSharedPointer s3d9aNode;						//uninitialized!
		SpawnProgram s3d9a;										//uninitialized!
		ComponentTupleSharedPointer s3e9aPrototype;				//uninitialized!
				//center
		SpawnNodeSharedPointer s3d9bNode;						//uninitialized!
		SpawnProgram s3d9b;										//uninitialized!
		ComponentTupleSharedPointer s3e9bPrototype;				//uninitialized!

		SpawnNodeSharedPointer s3e9Node;						//uninitialized!
		SpawnProgram s3e9;										//uninitialized!

			//wave 10: wisp from side, dropping bomb,
		SpawnNodeSharedPointer s3d10Node;						//uninitialized!
		SpawnProgram s3d10;										//uninitialized!

		ComponentTupleSharedPointer s3e10Prototype;				//uninitialized!
		SpawnNodeSharedPointer s3e10Node;						//uninitialized!
		SpawnProgram s3e10;										//uninitialized!

			//wave 11: bats; streaming
		SpawnNodeSharedPointer s3d11Node;						//uninitialized!
		SpawnProgram s3d11;										//uninitialized!

		ComponentTupleSharedPointer s3e11Prototype;				//uninitialized!
		SpawnNodeSharedPointer s3e11Node;						//uninitialized!
		SpawnProgram s3e11;										//uninitialized!

			//wave 12: bats from other side
		SpawnNodeSharedPointer s3e12Node;						//uninitialized!
		SpawnProgram s3e12;										//uninitialized!

			//wave 13: flames on left, whip
		SpawnNodeSharedPointer s3d13Node;						//uninitialized!
		SpawnProgram s3d13;										//uninitialized!

		ComponentTupleSharedPointer s3e13Prototype;				//uninitialized!
		SpawnNodeSharedPointer s3e13Node;						//uninitialized!
		SpawnProgram s3e13;										//uninitialized!

			//wave 14: flames on right
		SpawnNodeSharedPointer s3e14Node;						//uninitialized!
		SpawnProgram s3e14;										//uninitialized!

			//wave 15: clouds from both sides
		SpawnNodeSharedPointer s3e15Node;						//uninitialized!
		SpawnProgram s3e15;										//uninitialized!

			//wave 16: wave 10 but reversed (and slightly different spiral)
		SpawnNodeSharedPointer s3d16Node;						//uninitialized!
		SpawnProgram s3d16;										//uninitialized!

		ComponentTupleSharedPointer s3e16Prototype;				//uninitialized!
		SpawnNodeSharedPointer s3e16Node;						//uninitialized!
		SpawnProgram s3e16;										//uninitialized!

		// BOSS 3 // BOSS 3 // BOSS 3 // BOSS 3 // BOSS 3 // BOSS 3 // BOSS 3 // BOSS 3 

			//1 spiral with arc walls and medium
		SpawnNodeSharedPointer b3d1Node;						//uninitialized!
		SpawnProgram b3d1;										//uninitialized!

			//2 column spirals
		SpawnNodeSharedPointer b3d2aNode;						//uninitialized!
		SpawnProgram b3d2a;										//uninitialized!
		SpawnNodeSharedPointer b3d2bNode;						//uninitialized!
		SpawnProgram b3d2b;										//uninitialized!

			//3 large ring that shoot
				//the secondary shot
		SpawnNodeSharedPointer b3d3aNode;						//uninitialized!
		SpawnProgram b3d3a;										//uninitialized!

				//primary shot
		ComponentTupleSharedPointer b3d3bPrototype;				//uninitialized!
		SpawnNodeSharedPointer b3d3bNode;						//uninitialized!
		SpawnProgram b3d3b;										//uninitialized!

			//4 5-ring that shoots randomly
				//secondary shot
		SpawnNodeSharedPointer b3d4aNode;						//uninitialized!
		SpawnProgram b3d4a;										//uninitialized!

				//primary shot
		ComponentTupleSharedPointer b3d4bPrototype;				//uninitialized!
		SpawnNodeSharedPointer b3d4bNode;						//uninitialized!
		SpawnProgram b3d4b;										//uninitialized!

			//5 spiral of "splitters"
				//tertiary shot
		SpawnNodeSharedPointer b3d5aNode;						//uninitialized!
		SpawnProgram b3d5a;										//uninitialized!

				//secondary shot
		ComponentTupleSharedPointer b3d5bPrototype;				//uninitialized!
		SpawnNodeSharedPointer b3d5bNode;						//uninitialized!
		SpawnProgram b3d5b;										//uninitialized!

				//primary shot
		ComponentTupleSharedPointer b3d5cPrototype;				//uninitialized!
		SpawnNodeSharedPointer b3d5cNode;						//uninitialized!
		SpawnProgram b3d5c;										//uninitialized!

			//6 "random" dense arcs
		SpawnNodeSharedPointer b3d6Node;						//uninitialized!
		SpawnProgram b3d6;										//uninitialized!


			//the boss
		ComponentTupleSharedPointer b3e1Prototype;				//uninitialized!
		SpawnNodeSharedPointer b3e1Node;						//uninitialized!
		SpawnProgram b3e1;										//uninitialized!

		// STAGE 4 // STAGE 4 // STAGE 4 // STAGE 4 // STAGE 4 // STAGE 4 // STAGE 4 //

			//wave 1: clouds across top -> "lightning"
		SpawnNodeSharedPointer s4d1Node;						//uninitialized!
		SpawnProgram s4d1;										//uninitialized!

		ComponentTupleSharedPointer s4e1Prototype;				//uninitialized!
		SpawnNodeSharedPointer s4e1Node;						//uninitialized!
		SpawnProgram s4e1;										//uninitialized!

			//wave 2: shooting wisps
		ComponentTupleSharedPointer s4d2Prototype;				//uninitialized!
		SpawnNodeSharedPointer s4d2Node;						//uninitialized!
		SpawnProgram s4d2;										//uninitialized!

		ComponentTupleSharedPointer s4e2Prototype;				//uninitialized!
		SpawnNodeSharedPointer s4e2aNode;						//uninitialized!
		SpawnProgram s4e2a;										//uninitialized!
		SpawnNodeSharedPointer s4e2bNode;						//uninitialized!
		SpawnProgram s4e2b;										//uninitialized!

			//wave 3: slow flames from left, random angle offset
		SpawnNodeSharedPointer s4d3Node;						//uninitialized!
		SpawnProgram s4d3;										//uninitialized!

		ComponentTupleSharedPointer s4e3Prototype;				//uninitialized!
		SpawnNodeSharedPointer s4e3Node;						//uninitialized!
		SpawnProgram s4e3;										//uninitialized!

			//wave 4: flames from the right
		SpawnNodeSharedPointer s4e4Node;						//uninitialized!
		SpawnProgram s4e4;										//uninitialized!

			//wave 5: blue crystal (life)
		SpawnNodeSharedPointer s4d5Node;						//uninitialized!
		SpawnProgram s4d5;										//uninitialized!

		ComponentTupleSharedPointer s4e5Prototype;				//uninitialized!
		SpawnNodeSharedPointer s4e5Node;						//uninitialized!
		SpawnProgram s4e5;										//uninitialized!

			//wave 6: double green crystals, different size rings
		SpawnNodeSharedPointer s4d6Node;						//uninitialized!
		SpawnProgram s4d6;										//uninitialized!

		ComponentTupleSharedPointer s4e6Prototype;				//uninitialized!
		SpawnNodeSharedPointer s4e6Node;						//uninitialized!
		SpawnProgram s4e6;										//uninitialized!

			//wave 7: red crystal (bomb)
		SpawnNodeSharedPointer s4d7Node;						//uninitialized!
		SpawnProgram s4d7;										//uninitialized!

		ComponentTupleSharedPointer s4e7Prototype;				//uninitialized!
		SpawnNodeSharedPointer s4e7Node;						//uninitialized!
		SpawnProgram s4e7;										//uninitialized!

			//wave 8: repeat wave 6

			//wave 9: purple flames down right side
		SpawnNodeSharedPointer s4e9Node;						//uninitialized!
		SpawnProgram s4e9;										//uninitialized!

			//wave 10: flames down left side
		SpawnNodeSharedPointer s4e10Node;						//uninitialized!
		SpawnProgram s4e10;										//uninitialized!

			//wave 11: quad wisps; variation wave 2
		ComponentTupleSharedPointer s4d11Prototype;				//uninitialized!
		SpawnNodeSharedPointer s4d11Node;						//uninitialized!
		SpawnProgram s4d11;										//uninitialized!

		ComponentTupleSharedPointer s4e11Prototype;				//uninitialized!
		SpawnNodeSharedPointer s4e11Node;						//uninitialized!
		SpawnProgram s4e11;										//uninitialized!

		// BOSS 4 // BOSS 4 // BOSS 4 // BOSS 4 // BOSS 4 // BOSS 4 // BOSS 4 //

			//1 hexagon spawners -> sharps at 90 degrees
				//the secondary shot
		SpawnNodeSharedPointer b4d1aNode;						//uninitialized!
		SpawnProgram b4d1a;										//uninitialized!

				//primary shot
		ComponentTupleSharedPointer b4d1bPrototype;				//uninitialized!
		SpawnNodeSharedPointer b4d1bNode;						//uninitialized!
		SpawnProgram b4d1b;										//uninitialized!

			//2 medium ring -> explode small columns
				//secondary shot
		SpawnNodeSharedPointer b4d2aNode;						//uninitialized!
		SpawnProgram b4d2a;										//uninitialized!

				//primary shot
		ComponentTupleSharedPointer b4d2bPrototype;				//uninitialized!
		SpawnNodeSharedPointer b4d2bNode;						//uninitialized!
		SpawnProgram b4d2b;										//uninitialized!

			//3 hexagon spawners -> small aimed arc
				//the secondary shot
		SpawnNodeSharedPointer b4d3aNode;						//uninitialized!
		SpawnProgram b4d3a;										//uninitialized!

				//primary shot
		ComponentTupleSharedPointer b4d3bPrototype;				//uninitialized!
		SpawnNodeSharedPointer b4d3bNode;						//uninitialized!
		SpawnProgram b4d3b;										//uninitialized!

			//4 spiral w/ rand speed
		SpawnNodeSharedPointer b4d4aNode;						//uninitialized!
		SpawnProgram b4d4a;										//uninitialized!

		SpawnNodeSharedPointer b4d4bNode;						//uninitialized!
		SpawnProgram b4d4b;										//uninitialized!

			//the boss
		ComponentTupleSharedPointer b4e1Prototype;				//uninitialized!
		SpawnNodeSharedPointer b4e1Node;						//uninitialized!
		SpawnProgram b4e1;										//uninitialized!


	public:

		ScriptProgram stage1ScriptProgram;						//uninitialized!
		ScriptProgram stage2ScriptProgram;						//uninitialized!
		ScriptProgram stage3ScriptProgram;						//uninitialized!
		ScriptProgram stage4ScriptProgram;						//uninitialized!
		//ScriptProgram stage5ScriptProgram;						//uninitialized!

		//Constructors
		EnemyPrograms(
			resources::BitmapStorage* bitmapStoragePointer,
			PickupPrograms const* pickupProgramsPointer
		);
	};
}