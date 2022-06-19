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

		ComponentTupleSharedPointer smallBulletPrototype;	//uninitialized!
		ComponentTupleSharedPointer largeBulletPrototype;	//uninitialized!
		SpawnNodeSharedPointer spawnSmallBulletNode;		//uninitialized!
		SpawnNodeSharedPointer powerBucket0Node;			//uninitialized!

	public:
		const SpawnProgram shotA;							//uninitialized!

		//Constructors
		PlayerSpawnPrograms(
			resources::BitmapStorage* bitmapStoragePointer
		);
	};
}