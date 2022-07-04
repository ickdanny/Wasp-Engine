#pragma once

#include "Game/Systems/systemInclude.h"
#include "Game/Systems/EntityBuilder.h"

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
		//Returns a spawn node representing the given int value.
		static SpawnNodeSharedPointer makeIntValueSpawnNode(int value) {
			return SpawnNodeSharedPointer{
				new SpawnNodeData<int>{
					SpawnInstructions::value,
					value
				}
			};
		}

		//Returns a spawn node representing the given float value.
		static SpawnNodeSharedPointer makeFloatValueSpawnNode(float value) {
			return SpawnNodeSharedPointer{
				new SpawnNodeData<float>{
					SpawnInstructions::value,
					value
				}
			};
		}

		//Returns a spawn node representing the given velocity value.
		static SpawnNodeSharedPointer makeVelocityValueSpawnNode(
			const Velocity& value
		) {
			return SpawnNodeSharedPointer{
				new SpawnNodeData<Velocity>{
					SpawnInstructions::value,
					value
				}
			};
		}

		//Returns a spawn node representing the velocity specified by the given vector.
		static SpawnNodeSharedPointer makeVelocityValueSpawnNode(
			const math::Vector2& vector2
		) {
			return SpawnNodeSharedPointer{
				new SpawnNodeData<Velocity>{
					SpawnInstructions::value,
					Velocity{ vector2 }
				}
			};
		}

		//Returns a spawn node representing a tick modulo predicate.
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

		//Returns a spawn node representing an if statement.
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

		//Returns a spawn node representing a sequential list of instructions.
		template <typename... Ts>
		static SpawnNodeSharedPointer makeListNode(Ts... args) {
			SpawnNode* listNodePointer{ new SpawnNode{ SpawnInstructions::list } };
			listNodePointer->link(args...);
			return SpawnNodeSharedPointer{ listNodePointer };
		}

		//Returns a spawn node representing the current position of the entity.
		static SpawnNodeSharedPointer makeEntityPositionNode() {
			return std::make_shared<SpawnNode>(SpawnInstructions::entityPosition);
		}

		//Returns a spawn node representing the current x coordinate of the entity.
		static SpawnNodeSharedPointer makeEntityXNode() {
			return std::make_shared<SpawnNode>(SpawnInstructions::entityX);
		}

		//Returns a spawn node representing the current y coordinate of the entity.
		static SpawnNodeSharedPointer makeEntityYNode() {
			return std::make_shared<SpawnNode>(SpawnInstructions::entityY);
		}

		//Returns a spawn node representing the current position of the entity plus
		//an offset given by the velocity-valued offsetNode.
		static SpawnNodeSharedPointer makeEntityOffsetNode(
			const SpawnNodeSharedPointer& offsetNode
		) {
			SpawnNode* entityOffsetNodePointer{
				new SpawnNode{SpawnInstructions::entityOffset}
			};
			entityOffsetNodePointer->link(offsetNode);
			return SpawnNodeSharedPointer{ entityOffsetNodePointer };
		}

		//Returns a spawn node that constructs an entity based on the given spawn
		//prototype at the position specified by the given position node.
		static SpawnNodeSharedPointer makeSpawnPosNode(
			std::shared_ptr<ComponentTupleBase>& spawnPrototype,
			const SpawnNodeSharedPointer& posNode
		) {
			SpawnNode* spawnPosNodePointer{
				new SpawnNodeData<std::shared_ptr<ComponentTupleBase>>{
					SpawnInstructions::spawnPos,
					spawnPrototype
				}
			};
			spawnPosNodePointer->link(posNode);
			return SpawnNodeSharedPointer{ spawnPosNodePointer };
		}

		//Returns a spawn node that passes both a base pos/vel pair and one mirrored
		//about the given axis to a position/velocity consumer node.
		static SpawnNodeSharedPointer makeMirrorFormationNode(
			const SpawnNodeSharedPointer& posNodePointer,
			const SpawnNodeSharedPointer& velNodePointer,
			const SpawnNodeSharedPointer& axisNodePointer,
			const SpawnNodeSharedPointer& posVelConsumerNodePointer
		) {
			SpawnNode* mirrorFormationNodePointer{
				new SpawnNode{ SpawnInstructions::mirrorFormation }
			};
			mirrorFormationNodePointer->link(
				posNodePointer,
				velNodePointer,
				axisNodePointer,
				posVelConsumerNodePointer
			);
			return SpawnNodeSharedPointer{ mirrorFormationNodePointer };
		}

		//Returns a spawn node that passes both a base position and one mirrored about
		//the given axis to a position consumer node.
		static SpawnNodeSharedPointer makeMirrorPosFormationNode(
			const SpawnNodeSharedPointer& posNodePointer,
			const SpawnNodeSharedPointer& axisNodePointer,
			const SpawnNodeSharedPointer& posConsumerNodePointer
		) {
			SpawnNode* mirrorPosFormationNodePointer{
				new SpawnNode{ SpawnInstructions::mirrorPosFormation }
			};
			mirrorPosFormationNodePointer->link(
				posNodePointer,
				axisNodePointer,
				posConsumerNodePointer
			);
			return SpawnNodeSharedPointer{ mirrorPosFormationNodePointer };
		}
	};
}