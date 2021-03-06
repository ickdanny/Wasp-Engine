#pragma once

#include "systemInclude.h"
#include "Game/Systems/EntityBuilder.h"

namespace wasp::game::systems {

	class SpawnSystem {

	private:
		//typedefs
		using EntityID = ecs::entity::EntityID;
		using SpawnList = std::vector<std::shared_ptr<ComponentTupleBase>>;
		using SpawnProgram = components::SpawnProgram;
		using Point2 = math::Point2;
		using SpawnInstructions = components::SpawnInstructions;
		template <typename T>
		using SpawnNodeData = components::SpawnNodeData<T>;
		using SpawnNode = components::SpawnNode;

		//fields
		channel::ChannelSet* globalChannelSetPointer{};

	public:
		SpawnSystem(channel::ChannelSet* globalChannelSetPointer);
		void operator()(Scene& scene);

	private:
		//helper functions
		void runSpawnProgramList(
			Scene& scene,
			SpawnProgramList& spawnProgramList,
			EntityID entityID,
			SpawnList& spawnList
		);

		//Returns true if the program can continue, false if the program is over.
		bool runSpawnProgram(
			Scene& scene,
			SpawnProgram& spawnProgram,
			EntityID entityID,
			SpawnList& spawnList
		);

		#define NODE_HANDLER_ARGS \
			Scene& scene, \
			EntityID entityID, \
			std::shared_ptr<components::SpawnNode>& currentSpawnNodePointer, \
			int tick, \
			SpawnList& spawnList

		void runSpawnNode(NODE_HANDLER_ARGS);
		void runSpawnNodePassingPos(NODE_HANDLER_ARGS, const Point2& pos);
		void runSpawnNodePassingVel(NODE_HANDLER_ARGS, const Velocity& vel);
		void runSpawnNodePassingPosVel(
			NODE_HANDLER_ARGS,
			const math::Point2& pos,
			const Velocity& vel
		);
		int evaluateIntNode(NODE_HANDLER_ARGS);
		float evaluateFloatNode(NODE_HANDLER_ARGS);
		bool evaluatePredicateNode(NODE_HANDLER_ARGS);
		Velocity evaluateVelocityNode(NODE_HANDLER_ARGS);
		Point2 evaluatePointNode(NODE_HANDLER_ARGS);

		#undef NODE_HANDLER_ARGS

		Difficulty getDifficulty();
	};
}