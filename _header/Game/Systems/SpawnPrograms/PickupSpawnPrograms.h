#pragma once

#include "spawnProgramInclude.h"

namespace wasp::game::systems {

	class PickupSpawnPrograms {
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

	public:
		const SpawnProgram smallPower;						//uninitialized!
		const SpawnProgram largePower;						//uninitialized!
		const SpawnProgram life;							//uninitialized!
		const SpawnProgram bomb;							//uninitialized!

		//Constructors
		PickupSpawnPrograms(
			resources::BitmapStorage* bitmapStoragePointer
		);
	};
}