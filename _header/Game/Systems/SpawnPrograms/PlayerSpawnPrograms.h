#pragma once

#include "spawnProgramInclude.h"

namespace wasp::game::systems {

	class PlayerSpawnPrograms {
	private:
		//typedefs
		using Angle = math::Angle;
		using AABB = math::AABB;
		using Point2 = math::Point2;
		using Vector2 = math::Vector2;

		using ComponentTupleSharedPointer = std::shared_ptr<ComponentTupleBase>;

		using SpawnInstructions = components::SpawnInstructions;
		using SpawnNode = components::SpawnNode;

		template <typename T>
		using SpawnNodeData = components::SpawnNodeData<T>;
		using SpawnProgram = components::SpawnProgram;

		using SpawnNodeSharedPointer = std::shared_ptr<SpawnNode>;

		using ScriptInstructions = components::ScriptInstructions;
		using ScriptNode = components::ScriptNode;

		template <typename Internal, typename External>
		using ScriptNodeData = components::ScriptNodeData<Internal, External>;
		using ScriptProgram = components::ScriptProgram;

		using ScriptNodeSharedPointer = std::shared_ptr<ScriptNode>;

		//fields
		resources::BitmapStorage* bitmapStoragePointer{};

		//A shot
		ComponentTupleSharedPointer smallBubblePrototype;	//uninitialized!
		ComponentTupleSharedPointer largeBubblePrototype;	//uninitialized!
		SpawnNodeSharedPointer spawnSingleSmallBubbleNode;	//uninitialized!
		SpawnNodeSharedPointer spawnDoubleSmallBubbleNode;	//uninitialized!
		SpawnNodeSharedPointer spawnTripleSmallBubbleNode;	//uninitialized!
		SpawnNodeSharedPointer spawnLargeBubbleNode;		//uninitialized!
		SpawnNodeSharedPointer shotAPowerBucket0Node;		//uninitialized!
		SpawnNodeSharedPointer shotAPowerBucket1Node;		//uninitialized!
		SpawnNodeSharedPointer shotAPowerBucket2Node;		//uninitialized!
		SpawnNodeSharedPointer shotAPowerBucket3Node;		//uninitialized!
		SpawnNodeSharedPointer shotAPowerBucket4Node;		//uninitialized!
		SpawnNodeSharedPointer shotAPowerBucket5Node;		//uninitialized!
		SpawnNodeSharedPointer shotAPowerBucket6Node;		//uninitialized!
		SpawnNodeSharedPointer shotANode;					//uninitialized!

		//A bomb
		ScriptNodeSharedPointer bombBubbleScriptNode;		//uninitialized!
		ComponentTupleSharedPointer bombBubblePrototype;	//uninitialized!
		SpawnNodeSharedPointer spawnSingleBombBubbleNode;	//uninitialized!
		SpawnProgram bombGhostSpawnProgram;					//uninitialized!
		ComponentTupleSharedPointer bombGhostPrototype;		//uninitialized!
		SpawnNodeSharedPointer bombANode;					//uninitialized!
		
		//B shot
		ComponentTupleSharedPointer iceShardPrototype;		//uninitialized!
		SpawnNodeSharedPointer spawnSingleIceShardNode;		//uninitialized!
		SpawnNodeSharedPointer shotBPowerBucket0Node;		//uninitialized!
		SpawnNodeSharedPointer shotBPowerBucket1Node;		//uninitialized!
		SpawnNodeSharedPointer shotBPowerBucket2Node;		//uninitialized!
		SpawnNodeSharedPointer shotBPowerBucket3Node;		//uninitialized!
		SpawnNodeSharedPointer shotBPowerBucket4Node;		//uninitialized!
		SpawnNodeSharedPointer shotBPowerBucket5Node;		//uninitialized!
		SpawnNodeSharedPointer shotBPowerBucket6Node;		//uninitialized!
		SpawnNodeSharedPointer shotBNode;					//uninitialized!

		//B bomb
		ComponentTupleSharedPointer snowflakePrototype;		//uninitialized!
		SpawnNodeSharedPointer bombBNode;					//uninitialized!

		//death
		ComponentTupleSharedPointer clearPrototype;			//uninitialized!
		//todo: implement player death spawns
		SpawnNodeSharedPointer spawnClearNode;				//uninitialized!
		SpawnNodeSharedPointer spawnPowerNode;				//uninitialized!
		SpawnNodeSharedPointer deathSpawnNode;				//uninitialized!

	public:
		const SpawnProgram shotA;							//uninitialized!
		const SpawnProgram bombA;							//uninitialized!
		const SpawnProgram shotB;							//uninitialized!
		const SpawnProgram bombB;							//uninitialized!

		//Constructors
		PlayerSpawnPrograms(
			resources::BitmapStorage* bitmapStoragePointer
		);
	};
}