#pragma once

#include <array>

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
		static SpawnNodeSharedPointer makeIntValueNode(int value) {
			return SpawnNodeSharedPointer{
				new SpawnNodeData<int>{
					SpawnInstructions::value,
					value
				}
			};
		}

		//Returns a spawn node representing the given float value.
		static SpawnNodeSharedPointer makeFloatValueNode(float value) {
			return SpawnNodeSharedPointer{
				new SpawnNodeData<float>{
					SpawnInstructions::value,
					value
				}
			};
		}

		//Returns a spawn node representing a difficulty based int value.
		static SpawnNodeSharedPointer makeIntValueDiffNode(
			const std::array<int, 4> values
		) {
			return SpawnNodeSharedPointer{
				new SpawnNodeData<std::array<int, 4>>{
					SpawnInstructions::valueDifficulty,
					values
				}
			};
		}

		//Returns a spawn node representing a difficulty based float value.
		static SpawnNodeSharedPointer makeFloatValueDiffNode(
			const std::array<float, 4> values
		) {
			return SpawnNodeSharedPointer{
				new SpawnNodeData<std::array<float, 4>>{
					SpawnInstructions::valueDifficulty,
					values
				}
			};
		}

		//Returns a spawn node representing the addition of two given nodes.
		static SpawnNodeSharedPointer makeAddNode(
			const SpawnNodeSharedPointer& nodeA,
			const SpawnNodeSharedPointer& nodeB
		) {
			SpawnNode* addNodePointer{
				new SpawnNode{ SpawnInstructions::add }
			};
			addNodePointer->link(
				nodeA,
				nodeB
			);
			return SpawnNodeSharedPointer{ addNodePointer };
		}

		//Returns a spawn node representing the multiplication of two given nodes.
		static SpawnNodeSharedPointer makeMultiplyNode(
			const SpawnNodeSharedPointer& nodeA,
			const SpawnNodeSharedPointer& nodeB
		) {
			SpawnNode* multiplyNodePointer{
				new SpawnNode{ SpawnInstructions::multiply }
			};
			multiplyNodePointer->link(
				nodeA,
				nodeB
			);
			return SpawnNodeSharedPointer{ multiplyNodePointer };
		}

		//Returns a spawn node representing the max of two given nodes.
		static SpawnNodeSharedPointer makeMaxNode(
			const SpawnNodeSharedPointer& nodeA,
			const SpawnNodeSharedPointer& nodeB
		) {
			SpawnNode* maxNodePointer{
				new SpawnNode{ SpawnInstructions::max }
			};
			maxNodePointer->link(
				nodeA,
				nodeB
			);
			return SpawnNodeSharedPointer{ maxNodePointer };
		}

		//Returns a spawn node representing the angle from the entity to the player.
		static SpawnNodeSharedPointer makeAngleToPlayerNode() {
			SpawnNode* angleToPlayerNodePointer{
				new SpawnNode{ SpawnInstructions::angleToPlayer }
			};
			return SpawnNodeSharedPointer{ angleToPlayerNodePointer };
		}

		//Returns a spawn node representing the given velocity value.
		static SpawnNodeSharedPointer makeVelocityValueNode(
			const Velocity& value
		) {
			return SpawnNodeSharedPointer{
				new SpawnNodeData<Velocity>{
					SpawnInstructions::value,
					value
				}
			};
		}

		//Returns a spawn node representing the given point value.
		static SpawnNodeSharedPointer makePointValueNode(const math::Point2& value) {
			return SpawnNodeSharedPointer{
				new SpawnNodeData<math::Point2>{
					SpawnInstructions::value,
					value
				}
			};
		}

		//Returns a spawn node representing the velocity specified by the given vector.
		static SpawnNodeSharedPointer makeVelocityValueNode(
			const math::Vector2& vector2
		) {
			return SpawnNodeSharedPointer{
				new SpawnNodeData<Velocity>{
					SpawnInstructions::value,
					Velocity{ vector2 }
				}
			};
		}

		//Returns a spawn node representing the velocity specified by the given
		//magnitude and angle (float) nodes.
		static SpawnNodeSharedPointer makeVelocityFromPolarNode(
			const SpawnNodeSharedPointer& magnitudeNodePointer,
			const SpawnNodeSharedPointer& angleNodePointer
		) {
			SpawnNode* velocityFromPolarNodePointer{
				new SpawnNode{ SpawnInstructions::velocityFromPolar }
			};
			velocityFromPolarNodePointer->link(
				magnitudeNodePointer,
				angleNodePointer
			);
			return SpawnNodeSharedPointer{ velocityFromPolarNodePointer };
		}

		//Returns a spawn node representing the velocity required to get from
		//point A to point B.
		static SpawnNodeSharedPointer makeVelocityToPointNode(
			const SpawnNodeSharedPointer& pointANodePointer,
			const SpawnNodeSharedPointer& pointBNodePointer
		) {
			SpawnNode* velocityToPointNodePointer{
				new SpawnNode{ SpawnInstructions::velocityToPoint }
			};
			velocityToPointNodePointer->link(
				pointANodePointer,
				pointBNodePointer
			);
			return SpawnNodeSharedPointer{ velocityToPointNodePointer };
		}

		//Returns a spawn node representing the point specified by the given
		//x and y (float) nodes.
		static SpawnNodeSharedPointer makePointFromFloatsNode(
			const SpawnNodeSharedPointer& xNodePointer,
			const SpawnNodeSharedPointer& yNodePointer
		) {
			SpawnNode* pointFromFloatsNodePointer{
				new SpawnNode{ SpawnInstructions::pointFromFloats }
			};
			pointFromFloatsNodePointer->link(
				xNodePointer,
				yNodePointer
			);
			return SpawnNodeSharedPointer{ pointFromFloatsNodePointer };
		}

		//Returns a spawn node representing a uniformly random float and the given
		//bound nodes.
		static SpawnNodeSharedPointer makeUniformRandomFloatNode(
			const SpawnNodeSharedPointer& minNode,
			const SpawnNodeSharedPointer& maxNode
		) {
			SpawnNode* uniformRandomFloatNodePointer{
				new SpawnNode{ SpawnInstructions::uniformRandom }
			};
			uniformRandomFloatNodePointer->link(minNode, maxNode);
			return SpawnNodeSharedPointer{ uniformRandomFloatNodePointer };
		}

		//Returns a spawn node representing a uniformly random float and the given
		//bound values.
		static SpawnNodeSharedPointer makeUniformRandomFloatNode(
			float min,
			float max
		) {
			return makeUniformRandomFloatNode(
				makeFloatValueNode(min),
				makeFloatValueNode(max)
			);
		}

		//Returns a spawn node representing a uniformly random int based on entityID
		//and the given bound nodes.
		static SpawnNodeSharedPointer makeEntityUniformRandomIntNode(
			const SpawnNodeSharedPointer& minNode,
			const SpawnNodeSharedPointer& maxNode
		) {
			SpawnNode* entityUniformRandomIntNodePointer{
				new SpawnNode{ SpawnInstructions::entityUniformRandom }
			};
			entityUniformRandomIntNodePointer->link(minNode, maxNode);
			return SpawnNodeSharedPointer{ entityUniformRandomIntNodePointer };
		}

		//Returns a spawn node representing a uniformly random int based on entityID
		//and the given bound values.
		static SpawnNodeSharedPointer makeEntityUniformRandomIntNode(int min, int max) {
			return makeEntityUniformRandomIntNode(
				makeIntValueNode(min),
				makeIntValueNode(max)
			);
		}

		//Returns a spawn node representing a uniformly random float based on entityID
		//and the given bound nodes.
		static SpawnNodeSharedPointer makeEntityUniformRandomFloatNode(
			const SpawnNodeSharedPointer& minNode,
			const SpawnNodeSharedPointer& maxNode
		) {
			SpawnNode* entityUniformRandomFloatNodePointer{
				new SpawnNode{ SpawnInstructions::entityUniformRandom }
			};
			entityUniformRandomFloatNodePointer->link(minNode, maxNode);
			return SpawnNodeSharedPointer{ entityUniformRandomFloatNodePointer };
		}

		//Returns a spawn node representing a uniformly random float based on entityID
		//and the given bound values.
		static SpawnNodeSharedPointer makeEntityUniformRandomFloatNode(
			float min, 
			float max
		) {
			return makeEntityUniformRandomFloatNode(
				makeFloatValueNode(min),
				makeFloatValueNode(max)
			);
		}

		//Returns a spawn node representing a uniformly random velocity in a circle
		//based on the given float nodes for min and max radius.
		static SpawnNodeSharedPointer makeUniformRandomCircleVelocityNode(
			const SpawnNodeSharedPointer& minRadiusNode,
			const SpawnNodeSharedPointer& maxRadiusNode
		) {
			//credit to nubDotDev
			SpawnNodeSharedPointer uniformRandomFloatNode{
				makeUniformRandomFloatNode(
					minRadiusNode,
					maxRadiusNode
				)
			};
			//calculate radius by taking max of two uniformly random floats
			SpawnNodeSharedPointer maxNode{
				makeMaxNode(
					uniformRandomFloatNode,
					uniformRandomFloatNode
				)
			};
			return makeVelocityFromPolarNode(
				maxNode,
				randomAngleNode
			);
		}

		//Returns a spawn node representing a uniformly random velocity in a circle
		//based on the given float values for min and max radius.
		static SpawnNodeSharedPointer makeUniformRandomCircleVelocityNode(
			float minRadius,
			float maxRadius
		) {
			return makeUniformRandomCircleVelocityNode(
				makeFloatValueNode(minRadius),
				makeFloatValueNode(maxRadius)
			);
		}

		//Returns a spawn node representing a uniformly random point in a rectangle.
		static SpawnNodeSharedPointer makeUniformRandomRectanglePointNode(
			float xLow,
			float xHigh,
			float yLow,
			float yHigh
		) {
			SpawnNodeSharedPointer xNode{
				makeUniformRandomFloatNode(
					makeFloatValueNode(xLow),
					makeFloatValueNode(xHigh)
				)
			};
			SpawnNodeSharedPointer yNode{
				makeUniformRandomFloatNode(
					makeFloatValueNode(yLow),
					makeFloatValueNode(yHigh)
				)
			};
			return makePointFromFloatsNode(
				xNode,
				yNode
			);
		}

		//Returns a spawn node representing a tick modulo predicate.
		static SpawnNodeSharedPointer makeTickModNode(int add, int mod) {
			SpawnNode* tickModNodePointer{
				new SpawnNode{ SpawnInstructions::tickMod }
			};
			tickModNodePointer->link(
				makeIntValueNode(add),
				makeIntValueNode(mod)
			);
			return SpawnNodeSharedPointer{ tickModNodePointer };
		}

		//Returns a spawn node representing a tick modulo predicate for the given
		//values depending on difficulty.
		static SpawnNodeSharedPointer makeTickModNode(
			int add, 
			const std::array<int, 4>& mods
		) {
			SpawnNode* tickModNodePointer{
				new SpawnNode{ SpawnInstructions::tickMod }
			};
			tickModNodePointer->link(
				makeIntValueNode(add),
				makeIntValueDiffNode(mods)
			);
			return SpawnNodeSharedPointer{ tickModNodePointer };
		}

		//Returns a spawn node representing a tick modulo predicate with the given add
		//node.
		static SpawnNodeSharedPointer makeTickModNode(
			const SpawnNodeSharedPointer& addNode, 
			int mod
		) {
			SpawnNode* tickModNodePointer{
				new SpawnNode{ SpawnInstructions::tickMod }
			};
			tickModNodePointer->link(
				addNode,
				makeIntValueNode(mod)
			);
			return SpawnNodeSharedPointer{ tickModNodePointer };
		}

		//Returns a spawn node representing a tick modulo predicate for the given
		//values depending on difficulty.
		static SpawnNodeSharedPointer makeTickModNode(
			const SpawnNodeSharedPointer& addNode,
			const std::array<int, 4>& mods
		) {
			SpawnNode* tickModNodePointer{
				new SpawnNode{ SpawnInstructions::tickMod }
			};
			tickModNodePointer->link(
				addNode,
				makeIntValueDiffNode(mods)
			);
			return SpawnNodeSharedPointer{ tickModNodePointer };
		}

		//Returns a spawn node representing tick == 1 predicate.
		static SpawnNodeSharedPointer makeIsLastTickNode() {
			SpawnNode* isLastTickNodePointer{
				new SpawnNode{ SpawnInstructions::isLastTick }
			};
			return SpawnNodeSharedPointer{ isLastTickNodePointer };
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

		//Returns a spawn node representing an if statement for the given predicate.
		static SpawnNodeSharedPointer makeIfNode(
			SpawnInstructions predicateInstruction,
			const SpawnNodeSharedPointer& trueNodePointer
		) {
			SpawnNode* ifNodePointer{
				new SpawnNode{ SpawnInstructions::condition }
			};
			ifNodePointer->link(
				std::make_shared<SpawnNode>(predicateInstruction),
				trueNodePointer
			);
			return SpawnNodeSharedPointer{ ifNodePointer };
		}

		//Returns a spawn node representing an if-else statement.
		static SpawnNodeSharedPointer makeIfElseNode(
			const SpawnNodeSharedPointer& predicateNodePointer,
			const SpawnNodeSharedPointer& trueNodePointer,
			const SpawnNodeSharedPointer& falseNodePointer
		) {
			SpawnNode* ifElseNodePointer{
				new SpawnNode{ SpawnInstructions::conditionElse }
			};
			ifElseNodePointer->link(
				predicateNodePointer,
				trueNodePointer,
				falseNodePointer
			);
			return SpawnNodeSharedPointer{ ifElseNodePointer };
		}

		//Returns a spawn node representing an if-else statement for the given 
		//predicate.
		static SpawnNodeSharedPointer makeIfElseNode(
			SpawnInstructions predicateInstruction,
			const SpawnNodeSharedPointer& trueNodePointer,
			const SpawnNodeSharedPointer& falseNodePointer
		) {
			SpawnNode* ifElseNodePointer{
				new SpawnNode{ SpawnInstructions::conditionElse }
			};
			ifElseNodePointer->link(
				std::make_shared<SpawnNode>(predicateInstruction),
				trueNodePointer,
				falseNodePointer
			);
			return SpawnNodeSharedPointer{ ifElseNodePointer };
		}

		//Returns a spawn node representing whether or not the difficulty is more than
		//or equal to the given value
		static SpawnNodeSharedPointer makeIsDifficultyNode(Difficulty difficulty) {
			SpawnNode* isDifficultyNodePointer{
				new SpawnNodeData<Difficulty>{
					SpawnInstructions::isDifficulty,
					difficulty
				}
			};
			return SpawnNodeSharedPointer{ isDifficultyNodePointer };
		}

		//Returns a spawn node representing a random chance to be true based on the
		//given probability node (float from 0-1).
		static SpawnNodeSharedPointer makeChanceNode(
			const SpawnNodeSharedPointer& probabilityNodePointer
		) {
			SpawnNode* chanceNodePointer{
				new SpawnNode{ SpawnInstructions::chance }
			};
			chanceNodePointer->link(probabilityNodePointer);
			return SpawnNodeSharedPointer{ chanceNodePointer };
		}

		//Returns a spawn node representing a random chance to be true based on the
		//given probability value (float from 0-1)
		static SpawnNodeSharedPointer makeChanceNode(float probability) {
			return makeChanceNode(makeFloatValueNode(probability));
		}

		//Returns a spawn node representing a sequential list of instructions.
		template <typename... Ts>
		static SpawnNodeSharedPointer makeListNode(Ts... args) {
			SpawnNode* listNodePointer{ new SpawnNode{ SpawnInstructions::list } };
			listNodePointer->link(args...);
			return SpawnNodeSharedPointer{ listNodePointer };
		}

		//Returns a spawn node representing repeating a node n times, where n is given
		//by a specified int node.
		static SpawnNodeSharedPointer makeRepeatNode(
			const SpawnNodeSharedPointer& indexNodePointer,
			const SpawnNodeSharedPointer& bodyNodePointer
		) {
			SpawnNode* repeatNodePointer{ new SpawnNode{ SpawnInstructions::repeat } };
			repeatNodePointer->link(indexNodePointer, bodyNodePointer);
			return SpawnNodeSharedPointer{ repeatNodePointer };
		}

		//Returns a spawn node representing repeating a node n times, where n is given
		//by value.
		static SpawnNodeSharedPointer makeRepeatNode(
			int n,
			const SpawnNodeSharedPointer& bodyNodePointer
		) {
			SpawnNode* repeatNodePointer{ new SpawnNode{ SpawnInstructions::repeat } };
			repeatNodePointer->link(
				makeIntValueNode(n), 
				bodyNodePointer
			);
			return SpawnNodeSharedPointer{ repeatNodePointer };
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

		//Returns a spawn node representing the pickup init speed function
		static SpawnNodeSharedPointer makePickupInitSpeedNode() {
			return std::make_shared<SpawnNode>(SpawnInstructions::pickupInitSpeed);
		}

		//Returns a spawn node representing the current position of the entity plus
		//an offset given by the velocity-valued offsetNode.
		//Alternatively, can be used as a velocity consumer which forwards to
		//a position consumer.
		static SpawnNodeSharedPointer makeEntityOffsetNode(
			const SpawnNodeSharedPointer& offsetNode
		) {
			SpawnNode* entityOffsetNodePointer{
				new SpawnNode{SpawnInstructions::entityOffset}
			};
			entityOffsetNodePointer->link(offsetNode);
			return SpawnNodeSharedPointer{ entityOffsetNodePointer };
		}

		//Returns a spawn node which takes the current position of the entity plus
		//an offset given by the velocity-valued offsetNode, and passes it to the
		//given posConsumerNode.
		static SpawnNodeSharedPointer makeEntityOffsetNode(
			const SpawnNodeSharedPointer& offsetNode,
			const SpawnNodeSharedPointer& posConsumerNode
		) {
			SpawnNode* entityOffsetNodePointer{
				new SpawnNode{SpawnInstructions::entityOffset}
			};
			entityOffsetNodePointer->link(offsetNode, posConsumerNode);
			return SpawnNodeSharedPointer{ entityOffsetNodePointer };
		}

		//Returns a spawn node that constructs an entity based on the given spawn
		//prototype.
		static SpawnNodeSharedPointer makeSpawnNode(
			std::shared_ptr<ComponentTupleBase>& spawnPrototype
		) {
			SpawnNode* spawnNodePointer{
				new SpawnNodeData<std::shared_ptr<ComponentTupleBase>>{
					SpawnInstructions::spawn,
					spawnPrototype
				}
			};
			return SpawnNodeSharedPointer{ spawnNodePointer };
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

		//Returns a spawn node that constructs an entity based on the given spawn
		//prototype at the position specified by a passed position.
		static SpawnNodeSharedPointer makeSpawnPosNode(
			std::shared_ptr<ComponentTupleBase>& spawnPrototype
		) {
			SpawnNode* spawnPosNodePointer{
				new SpawnNodeData<std::shared_ptr<ComponentTupleBase>>{
					SpawnInstructions::spawnPos,
					spawnPrototype
				}
			};
			return SpawnNodeSharedPointer{ spawnPosNodePointer };
		}

		//Returns a spawn node that constructs an entity based on the given spawn
		//prototype at the position of the spawning entity
		static SpawnNodeSharedPointer makeSpawnAtPosNode(
			std::shared_ptr<ComponentTupleBase>& spawnPrototype
		) {
			SpawnNode* spawnPosNodePointer{
				new SpawnNodeData<std::shared_ptr<ComponentTupleBase>>{
					SpawnInstructions::spawnPos,
					spawnPrototype
				}
			};
			spawnPosNodePointer->link(makeEntityPositionNode());
			return SpawnNodeSharedPointer{ spawnPosNodePointer };
		}

		//Returns a spawn node that constructs an entity based on the given spawn
		//prototype with a passed position and velocity.
		static SpawnNodeSharedPointer makeSpawnPosVelNode(
			std::shared_ptr<ComponentTupleBase>& spawnPrototype
		) {
			SpawnNode* spawnPosVelNodePointer{
				new SpawnNodeData<std::shared_ptr<ComponentTupleBase>>{
					SpawnInstructions::spawnPosVel,
					spawnPrototype
				}
			};
			return SpawnNodeSharedPointer{ spawnPosVelNodePointer };
		}


		//Returns a spawn node that constructs an entity based on the given spawn
		//prototype with the position specified by the given node and a passed
		//velocity.
		static SpawnNodeSharedPointer makeSpawnPosVelNode(
			std::shared_ptr<ComponentTupleBase>& spawnPrototype,
			const SpawnNodeSharedPointer& posNode
		) {
			SpawnNode* spawnPosVelNodePointer{
				new SpawnNodeData<std::shared_ptr<ComponentTupleBase>>{
					SpawnInstructions::spawnPosVel,
					spawnPrototype
				}
			};
			spawnPosVelNodePointer->link(posNode);
			return SpawnNodeSharedPointer{ spawnPosVelNodePointer };
		}

		//Returns a spawn node that constructs an entity based on the given spawn
		//prototype with the position and velocity specified by the given nodes.
		static SpawnNodeSharedPointer makeSpawnPosVelNode(
			std::shared_ptr<ComponentTupleBase>& spawnPrototype,
			const SpawnNodeSharedPointer& posNode,
			const SpawnNodeSharedPointer& velNode
		) {
			SpawnNode* spawnPosVelNodePointer{
				new SpawnNodeData<std::shared_ptr<ComponentTupleBase>>{
					SpawnInstructions::spawnPosVel,
					spawnPrototype
				}
			};
			spawnPosVelNodePointer->link(posNode, velNode);
			return SpawnNodeSharedPointer{ spawnPosVelNodePointer };
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

		//Returns a spawn node that passes both a base pos/vel pair and one mirrored
		//about the spawning entity to a position/velocity consumer node.
		static SpawnNodeSharedPointer makeMirrorFormationNode(
			const SpawnNodeSharedPointer& posNodePointer,
			const SpawnNodeSharedPointer& velNodePointer,
			const SpawnNodeSharedPointer& posVelConsumerNodePointer
		) {
			return makeMirrorFormationNode(
				posNodePointer,
				velNodePointer,
				makeEntityXNode(),
				posVelConsumerNodePointer
			);
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

		//Returns a spawn node that passes both a base position and one mirrored about
		//the spawning entity to a position consumer node.
		static SpawnNodeSharedPointer makeMirrorPosFormationNode(
			const SpawnNodeSharedPointer& posNodePointer,
			const SpawnNodeSharedPointer& posConsumerNodePointer
		) {
			return makeMirrorPosFormationNode(
				posNodePointer,
				makeEntityXNode(),
				posConsumerNodePointer
			);
		}

		//Returns a spawn node that passes a velocity arc to a velocity consumer node
		//based on the provided base velocity, symmetry, and angle offset nodes.
		static SpawnNodeSharedPointer makeArcFormationNode(
			const SpawnNodeSharedPointer& baseVelNodePointer,
			const SpawnNodeSharedPointer& symmetryNodePointer,
			const SpawnNodeSharedPointer& angleIncrementNodePointer,
			const SpawnNodeSharedPointer& velConsumerNodePointer
		) {
			SpawnNode* arcFormationNodePointer{
				new SpawnNode{ SpawnInstructions::arcFormation }
			};
			arcFormationNodePointer->link(
				baseVelNodePointer,
				symmetryNodePointer,
				angleIncrementNodePointer,
				velConsumerNodePointer
			);
			return SpawnNodeSharedPointer{ arcFormationNodePointer };
		}

		//Returns a spawn node that passes a velocity arc to a velocity consumer node
		//based on the provided base velocity node, symmetry value, and angle offset 
		//value.
		static SpawnNodeSharedPointer makeArcFormationNode(
			const SpawnNodeSharedPointer& baseVelNodePointer,
			int symmetry,
			float angleIncrement,
			const SpawnNodeSharedPointer& velConsumerNodePointer
		) {
			return makeArcFormationNode(
				baseVelNodePointer,
				makeIntValueNode(symmetry),
				makeFloatValueNode(angleIncrement),
				velConsumerNodePointer
			);
		}

		//Returns a spawn node that passes a velocity arc to a velocity consumer node
		//based on the provided base velocity, symmetry, and angle values.
		static SpawnNodeSharedPointer makeArcFormationNode(
			const Velocity& baseVel,
			int symmetry,
			float angleIncrement,
			const SpawnNodeSharedPointer& velConsumerNodePointer
		) {
			return makeArcFormationNode(
				makeVelocityValueNode(baseVel),
				symmetry,
				angleIncrement,
				velConsumerNodePointer
			);
		}

		//Returns a spawn node that passes a velocity arc to a velocity consumer node
		//based on the provided base velocity, symmetry, and angle values.
		static SpawnNodeSharedPointer makeArcFormationNode(
			const math::Vector2& baseVel,		//vec2 version
			int symmetry,
			float angleIncrement,
			const SpawnNodeSharedPointer& velConsumerNodePointer
		) {
			return makeArcFormationNode(
				makeVelocityValueNode(baseVel),
				symmetry,
				angleIncrement,
				velConsumerNodePointer
			);
		}

		//Returns a spawn node that passes a velocity ring to a velocity consumer node
		//based on the provided base velocity and symmetry nodes.
		static SpawnNodeSharedPointer makeRingFormationNode(
			const SpawnNodeSharedPointer& baseVelNodePointer,
			const SpawnNodeSharedPointer& symmetryNodePointer,
			const SpawnNodeSharedPointer& velConsumerNodePointer
		) {
			SpawnNode* ringFormationNodePointer{
				new SpawnNode{ SpawnInstructions::ringFormation }
			};
			ringFormationNodePointer->link(
				baseVelNodePointer,
				symmetryNodePointer,
				velConsumerNodePointer
			);
			return SpawnNodeSharedPointer{ ringFormationNodePointer };
		}

		//Returns a spawn node that passes a velocity ring to a velocity consumer node
		//based on the provided base velocity node and symmetry value.
		static SpawnNodeSharedPointer makeRingFormationNode(
			const SpawnNodeSharedPointer& baseVelNodePointer,
			int symmetry,
			const SpawnNodeSharedPointer& velConsumerNodePointer
		) {
			SpawnNode* ringFormationNodePointer{
				new SpawnNode{ SpawnInstructions::ringFormation }
			};
			ringFormationNodePointer->link(
				baseVelNodePointer,
				makeIntValueNode(symmetry),
				velConsumerNodePointer
			);
			return SpawnNodeSharedPointer{ ringFormationNodePointer };
		}

		//Returns a spawn node that passes a velocity ring to a velocity consumer node
		//based on the provided base velocity node and difficulty based symmetry.
		static SpawnNodeSharedPointer makeRingFormationNode(
			const SpawnNodeSharedPointer& baseVelNodePointer,
			const std::array<int, 4> symmetries,
			const SpawnNodeSharedPointer& velConsumerNodePointer
		) {
			SpawnNode* ringFormationNodePointer{
				new SpawnNode{ SpawnInstructions::ringFormation }
			};
			ringFormationNodePointer->link(
				baseVelNodePointer,
				makeIntValueDiffNode(symmetries),
				velConsumerNodePointer
			);
			return SpawnNodeSharedPointer{ ringFormationNodePointer };
		}

		//Returns a spawn node that passes a velocity ring to a velocity consumer node
		//based on the provided base velocity and symmetry values.
		static SpawnNodeSharedPointer makeRingFormationNode(
			const Velocity& baseVel,
			int symmetry,
			const SpawnNodeSharedPointer& velConsumerNodePointer
		) {
			return makeRingFormationNode(
				makeVelocityValueNode(baseVel),
				makeIntValueNode(symmetry),
				velConsumerNodePointer
			);
		}

		//Returns a spawn node that passes a velocity ring to a velocity consumer node
		//based on the provided base velocity and symmetry values.
		static SpawnNodeSharedPointer makeRingFormationNode(
			const math::Vector2& baseVel,			//vec2 version
			int symmetry,
			const SpawnNodeSharedPointer& velConsumerNodePointer
		) {
			return makeRingFormationNode(
				makeVelocityValueNode(baseVel),
				makeIntValueNode(symmetry),
				velConsumerNodePointer
			);
		}

		//Returns a spawn node that represents a spiral based on the given nodes.
		static SpawnNodeSharedPointer makeSpiralNode(
			const SpawnNodeSharedPointer& maxTickNodePointer,
			const SpawnNodeSharedPointer& baseAngleNodePointer,
			const SpawnNodeSharedPointer& angularVelocityNodePointer
		) {
			SpawnNode* spiralNodePointer{
				new SpawnNode{ SpawnInstructions::spiral }
			};
			spiralNodePointer->link(
				maxTickNodePointer,
				baseAngleNodePointer,
				angularVelocityNodePointer
			);
			return SpawnNodeSharedPointer{ spiralNodePointer };
		}

		//Returns a spawn node that represents a spiral based on the given values.
		static SpawnNodeSharedPointer makeSpiralNode(
			int maxTick,
			float baseAngle,
			float angularVelocity
		) {
			return makeSpiralNode(
				makeIntValueNode(maxTick),
				makeFloatValueNode(baseAngle),
				makeFloatValueNode(angularVelocity)
			);
		}

		//Returns a spawn node that represents a whip based on the given nodes.
		static SpawnNodeSharedPointer makeWhipNode(
			const SpawnNodeSharedPointer& maxTickNodePointer,
			const SpawnNodeSharedPointer& speedLowNodePointer,
			const SpawnNodeSharedPointer& speedHighNodePointer
		) {
			SpawnNode* whipNodePointer{
				new SpawnNode{ SpawnInstructions::whip }
			};
			whipNodePointer->link(
				maxTickNodePointer,
				speedLowNodePointer,
				speedHighNodePointer
			);
			return SpawnNodeSharedPointer{ whipNodePointer };
		}

		//Returns a spawn node that represents a whip based on the given values.
		static SpawnNodeSharedPointer makeWhipNode(
			int maxTick,
			float speedLow,
			float speedHigh
		) {
			return makeWhipNode(
				makeIntValueNode(maxTick),
				makeFloatValueNode(speedLow),
				makeFloatValueNode(speedHigh)
			);
		}

		//Returns a spawn node that passes a velocity column to a velocity consumer
		//node based on the given velocity, count, and increment nodes
		static SpawnNodeSharedPointer makeColumnFormationNode(
			const SpawnNodeSharedPointer& baseVelNodePointer,
			const SpawnNodeSharedPointer& countNodePointer,
			const SpawnNodeSharedPointer& incrementNodePointer,
			const SpawnNodeSharedPointer& velConsumerNodePointer
		) {
			SpawnNode* columnFormationNodePointer{
				new SpawnNode{ SpawnInstructions::columnFormation }
			};
			columnFormationNodePointer->link(
				baseVelNodePointer,
				countNodePointer,
				incrementNodePointer,
				velConsumerNodePointer
			);
			return SpawnNodeSharedPointer{ columnFormationNodePointer };
		}

		//Returns a spawn node that passes a velocity column to a velocity consumer
		//node based on the given velocity and increment nodes, as well as the given
		//count value.
		static SpawnNodeSharedPointer makeColumnFormationNode(
			const SpawnNodeSharedPointer& baseVelNodePointer,
			int count,
			const SpawnNodeSharedPointer& incrementNodePointer,
			const SpawnNodeSharedPointer& velConsumerNodePointer
		) {
			return makeColumnFormationNode(
				baseVelNodePointer,
				makeIntValueNode(count),
				incrementNodePointer,
				velConsumerNodePointer
			);
		}

		//Returns a spawn node that passes a velocity column to a velocity consumer
		//node based on the given velocity node, as well as the given count and
		//increment values.
		static SpawnNodeSharedPointer makeColumnFormationNode(
			const SpawnNodeSharedPointer& baseVelNodePointer,
			int count,
			float increment,
			const SpawnNodeSharedPointer& velConsumerNodePointer
		) {
			return makeColumnFormationNode(
				baseVelNodePointer,
				makeIntValueNode(count),
				makeFloatValueNode(increment),
				velConsumerNodePointer
			);
		}

		//Returns a spawn node that passes a velocity column to a velocity consumer
		//node based on the given velocity node, as well as the given difficulty based
		//count and increment values.
		static SpawnNodeSharedPointer makeColumnFormationNode(
			const SpawnNodeSharedPointer& baseVelNodePointer,
			const std::array<int, 4>& counts,
			const std::array<float, 4>& increments,
			const SpawnNodeSharedPointer& velConsumerNodePointer
		) {
			return makeColumnFormationNode(
				baseVelNodePointer,
				makeIntValueDiffNode(counts),
				makeFloatValueDiffNode(increments),
				velConsumerNodePointer
			);
		}

		//Returns a spawn node that, receiving a velocity, passes a velocity column 
		//to a velocity consumer node based on the given count and increment nodes.
		static SpawnNodeSharedPointer makeColumnFormationNode(
			const SpawnNodeSharedPointer& countNodePointer,
			const SpawnNodeSharedPointer& incrementNodePointer,
			const SpawnNodeSharedPointer& velConsumerNodePointer
		) {
			SpawnNode* columnFormationNodePointer{
				new SpawnNode{ SpawnInstructions::columnFormation }
			};
			columnFormationNodePointer->link(
				countNodePointer,
				incrementNodePointer,
				velConsumerNodePointer
			);
			return SpawnNodeSharedPointer{ columnFormationNodePointer };
		}

		//Returns a spawn node that, receiving a velocity, passes a velocity column 
		//to a velocity consumer node based on the given increment node and the given
		//count value.
		static SpawnNodeSharedPointer makeColumnFormationNode(
			int count,
			const SpawnNodeSharedPointer& incrementNodePointer,
			const SpawnNodeSharedPointer& velConsumerNodePointer
		) {
			return makeColumnFormationNode(
				makeIntValueNode(count),
				incrementNodePointer,
				velConsumerNodePointer
			);
		}

		//Returns a spawn node that, receiving a velocity, passes a velocity column 
		//to a velocity consumer node based on the given count and increment values.
		static SpawnNodeSharedPointer makeColumnFormationNode(
			int count,
			float increment,
			const SpawnNodeSharedPointer& velConsumerNodePointer
		) {
			return makeColumnFormationNode(
				makeIntValueNode(count),
				makeFloatValueNode(increment),
				velConsumerNodePointer
			);
		}

		//Returns a spawn node that, receiving a velocity, passes a velocity column 
		//to a velocity consumer node based on the given difficulty based
		//count and increment values.
		static SpawnNodeSharedPointer makeColumnFormationNode(
			const std::array<int, 4>& counts,
			const std::array<float, 4>& increments,
			const SpawnNodeSharedPointer& velConsumerNodePointer
		) {
			return makeColumnFormationNode(
				makeIntValueDiffNode(counts),
				makeFloatValueDiffNode(increments),
				velConsumerNodePointer
			);
		}

		//Returns a spawn node that, receiving a velocity, passes a velocity column 
		//to a velocity consumer node based on the given difficulty based
		//count values and single increment value.
		static SpawnNodeSharedPointer makeColumnFormationNode(
			const std::array<int, 4>& counts,
			float increment,
			const SpawnNodeSharedPointer& velConsumerNodePointer
		) {
			return makeColumnFormationNode(
				makeIntValueDiffNode(counts),
				makeFloatValueNode(increment),
				velConsumerNodePointer
			);
		}

		

		//some useful nodes
		static const SpawnNodeSharedPointer randomAngleNode;
		static const SpawnNodeSharedPointer randomTopOutPointNode;
		static const SpawnNodeSharedPointer entityRandomAngleNode;
	};
}