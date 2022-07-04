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

		using SpawnInstructions = components::SpawnInstructions;
		using SpawnNode = components::SpawnNode;

		template <typename T>
		using SpawnNodeData = components::SpawnNodeData<T>;
		using SpawnProgram = components::SpawnProgram;

		using ComponentTupleSharedPointer = std::shared_ptr<ComponentTupleBase>;
		using SpawnNodeSharedPointer = std::shared_ptr<SpawnNode>;

		//fields
		resources::BitmapStorage* bitmapStoragePointer{};

		ComponentTupleSharedPointer smallBubblePrototype;	//uninitialized!
		ComponentTupleSharedPointer largeBubblePrototype;	//uninitialized!
		SpawnNodeSharedPointer spawnSingleSmallBubbleNode;	//uninitialized!
		SpawnNodeSharedPointer spawnDoubleSmallBubbleNode;	//uninitialized!
		SpawnNodeSharedPointer spawnTripleSmallBubbleNode;	//uninitialized!
		SpawnNodeSharedPointer shotAPowerBucket0Node;		//uninitialized!
		SpawnNodeSharedPointer shotAPowerBucket1Node;		//uninitialized!
		SpawnNodeSharedPointer shotAPowerBucket2Node;		//uninitialized!

		SpawnNodeSharedPointer shotANode;					//uninitialized!

	public:
		const SpawnProgram shotA;							//uninitialized!

		//Constructors
		PlayerSpawnPrograms(
			resources::BitmapStorage* bitmapStoragePointer
		);
	};
}