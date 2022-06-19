#pragma once

#include "Game/Systems/systemInclude.h"

namespace wasp::game::systems {

	struct SpawnProgramUtil {
	private:
		//typedefs
		using SpawnInstructions = components::SpawnInstructions;
		using SpawnNode = components::SpawnNode;

		template <typename T>
		using SpawnNodeData = components::SpawnNodeData<T>;
		using SpawnProgram = components::SpawnProgram;

		using SpawnNodeSharedPointer = std::shared_ptr<SpawnNode>;

	public:
		static SpawnNodeSharedPointer makeIntValueSpawnNode(int value) {
			return SpawnNodeSharedPointer{
				new SpawnNodeData<int>{
					SpawnInstructions::value,
					value
				}
			};
		}

		static SpawnNodeSharedPointer makeTickModNode(int add, int mod) {
			SpawnNode* tickModNodePointer{
				new SpawnNode{ SpawnInstructions::tickMod }
			};
			tickModNodePointer->link(
				makeIntValueSpawnNode(add),
				makeIntValueSpawnNode(mod)
			);
			return SpawnNodeSharedPointer{ tickModNodePointer };
		}

		static SpawnNodeSharedPointer makeIfNode(
			const SpawnNodeSharedPointer& predicateNodePointer,
			const SpawnNodeSharedPointer& trueNodePointer
		) {
			SpawnNode* ifNodePointer{
				new SpawnNode{ SpawnInstructions::condition }
			};
			ifNodePointer->link(
				predicateNodePointer,
				trueNodePointer
			);
			return SpawnNodeSharedPointer{ ifNodePointer };
		}
	};
}