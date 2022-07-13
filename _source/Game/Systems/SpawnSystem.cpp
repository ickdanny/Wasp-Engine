#include "Game/Systems/SpawnSystem.h"

#include <array>

#include "Game/Systems/EntityBuilder.h"

namespace wasp::game::systems {

	using namespace ecs;
	using namespace ecs::entity;

	using SpawnList = std::vector<std::shared_ptr<ComponentTupleBase>>;

	namespace {

		Difficulty getDifficulty(const Scene& scene) {
			return scene.getChannel(GlobalTopics::gameState).getMessages()[0].difficulty;
		}

		#define NODE_HANDLER_ARGS \
			Scene& scene, \
			EntityID entityID, \
			std::shared_ptr<components::SpawnNode>& currentSpawnNodePointer, \
			int tick, \
			SpawnList& spawnList

		int evaluateIntNode(NODE_HANDLER_ARGS) {
			switch (currentSpawnNodePointer->spawnInstruction) {
				case components::SpawnInstructions::value: {
					const auto [value] =
						dynamic_cast<const components::SpawnNodeData<int>*>(
							currentSpawnNodePointer.get()
						)->data;
					return value;
				}
				case components::SpawnInstructions::valueDifficulty: {
					const auto& [valueArray] =
						dynamic_cast<
							const components::SpawnNodeData<std::array<int, 4>>*
						>(
							currentSpawnNodePointer.get()
						)->data;
					return valueArray[static_cast<int>(getDifficulty(scene))];
				}
				case components::SpawnInstructions::add: {
					int a{ evaluateIntNode(
						scene,
						entityID,
						currentSpawnNodePointer->linkedNodePointers[0],
						tick,
						spawnList
					) };
					int b{ evaluateIntNode(
						scene,
						entityID,
						currentSpawnNodePointer->linkedNodePointers[1],
						tick,
						spawnList
					) };
					return a + b;
				}
				default:
					throw std::runtime_error{ "not an int instruction!" };
			}
		}

		bool evaluatePredicateNode(NODE_HANDLER_ARGS);	//forward declare

		float evaluateFloatNode(NODE_HANDLER_ARGS) {
			switch (currentSpawnNodePointer->spawnInstruction) {
				case components::SpawnInstructions::value: {
					const auto [value] =
						dynamic_cast<const components::SpawnNodeData<float>*>(
							currentSpawnNodePointer.get()
						)->data;
					return value;
				}
				case components::SpawnInstructions::valueDifficulty: {
					const auto& [valueArray] =
						dynamic_cast<
						const components::SpawnNodeData<std::array<float, 4>>*
						>(
							currentSpawnNodePointer.get()
						)->data;
					return valueArray[static_cast<int>(getDifficulty(scene))];
				}
				case components::SpawnInstructions::add: {
					float a{ evaluateFloatNode(
						scene,
						entityID,
						currentSpawnNodePointer->linkedNodePointers[0],
						tick,
						spawnList
					) };
					float b{ evaluateFloatNode(
						scene,
						entityID,
						currentSpawnNodePointer->linkedNodePointers[1],
						tick,
						spawnList
					) };
					return a + b;
				}
				case components::SpawnInstructions::max: {
					float a{ evaluateFloatNode(
						scene,
						entityID,
						currentSpawnNodePointer->linkedNodePointers[0],
						tick,
						spawnList
					) };
					float b{ evaluateFloatNode(
						scene,
						entityID,
						currentSpawnNodePointer->linkedNodePointers[1],
						tick,
						spawnList
					) };
					return std::max(a, b);
				}
				case components::SpawnInstructions::entityX: {
					math::Point2 pos{
						scene.getDataStorage().getComponent<Position>(entityID)
					};
					return pos.x;
				}
				case components::SpawnInstructions::entityY: {
					math::Point2 pos{
						scene.getDataStorage().getComponent<Position>(entityID)
					};
					return pos.y;
				}
				case components::SpawnInstructions::uniformRandom: {
					float min{ evaluateFloatNode(
						scene,
						entityID,
						currentSpawnNodePointer->linkedNodePointers[0],
						tick,
						spawnList
					) };
					float max{ evaluateFloatNode(
						scene,
						entityID,
						currentSpawnNodePointer->linkedNodePointers[1],
						tick,
						spawnList
					) };
					auto& prng{ scene.getChannel(SceneTopics::random).getMessages()[0] };
					std::uniform_real_distribution<float> distribution{ min, max };
					return distribution(prng);
				}
				case components::SpawnInstructions::pickupInitSpeed: {
					math::Point2 pos{
						scene.getDataStorage().getComponent<Position>(entityID)
					};

					//higher = lower speed
					float heightRatio{ 
						(pos.y - config::gameOffset.y) / config::gameHeight 
					};
					float speedMulti{ 1 + (heightRatio * config::pickupInitSpeedMulti) };
					return config::pickupInitSpeedBase * speedMulti;
				}
				case components::SpawnInstructions::conditionElse: {
					//if our predicate is met, evaluate truenode
					if (evaluatePredicateNode(
						scene,
						entityID,
						currentSpawnNodePointer->linkedNodePointers[0],
						tick,
						spawnList
					)) {
						return evaluateFloatNode(
							scene,
							entityID,
							currentSpawnNodePointer->linkedNodePointers[1],
							tick,
							spawnList
						);
					}
					//otherwise, evaluate falseNode
					else {
						return evaluateFloatNode(
							scene,
							entityID,
							currentSpawnNodePointer->linkedNodePointers[2],
							tick,
							spawnList
						);
					}
					break;
				}
				default:
					throw std::runtime_error{ "not a float instruction!" };
			}
		}

		bool evaluatePredicateNode(NODE_HANDLER_ARGS) {
			switch (currentSpawnNodePointer->spawnInstruction) {
				case components::SpawnInstructions::tickMod: {
					//returns (tick + int1) % int2 == 0
					int add = evaluateIntNode(
						scene,
						entityID,
						currentSpawnNodePointer->linkedNodePointers[0],
						tick,
						spawnList
					);
					int mod = evaluateIntNode(
						scene,
						entityID,
						currentSpawnNodePointer->linkedNodePointers[1],
						tick,
						spawnList
					);
					return (tick + add) % mod == 0;
				}
				case components::SpawnInstructions::lastTick: {
					//returns true if this is the last tick of the SpawnProgram
					return tick == 1;
				}
				case components::SpawnInstructions::isPlayerFocused: {
					//returns true if the focus gameCommand is found
					const auto& gameCommandChannel{
						scene.getChannel(SceneTopics::gameCommands)
					};
					for (GameCommands gameCommand : gameCommandChannel.getMessages()) {
						if (gameCommand == GameCommands::focus) {
							return true;
						}
					}
					return false;
				}
				default:
					throw std::runtime_error{ "not a predicate instruction!" };
			}
		}

		Velocity evaluateVelocityNode(NODE_HANDLER_ARGS);	//forward declare

		math::Point2 evaluatePointNode(NODE_HANDLER_ARGS) {
			switch (currentSpawnNodePointer->spawnInstruction) {
				case components::SpawnInstructions::value: {
					const auto [value] =
						dynamic_cast<const components::SpawnNodeData<math::Point2>*>(
							currentSpawnNodePointer.get()
						)->data;
					return value;
				}
				case components::SpawnInstructions::valueDifficulty: {
					const auto& [valueArray] =
						dynamic_cast<
							const components::SpawnNodeData<std::array<math::Point2, 4>>*
						>(
							currentSpawnNodePointer.get()
						)->data;
					return valueArray[static_cast<int>(getDifficulty(scene))];
				}
				case components::SpawnInstructions::entityPosition: {
					return scene.getDataStorage().getComponent<Position>(entityID);
				}
				case components::SpawnInstructions::entityOffset: {
					math::Point2 basePos{
						scene.getDataStorage().getComponent<Position>(entityID)
					};
					Velocity offset{ evaluateVelocityNode(
						scene,
						entityID,
						currentSpawnNodePointer->linkedNodePointers[0],
						tick,
						spawnList
					) };
					return basePos + offset;
				}
				case components::SpawnInstructions::pointFromFloats: {
					float x{ evaluateFloatNode(
							scene,
							entityID,
							currentSpawnNodePointer->linkedNodePointers[0],
							tick,
							spawnList
					) };
					float y{ evaluateFloatNode(
							scene,
							entityID,
							currentSpawnNodePointer->linkedNodePointers[1],
							tick,
							spawnList
					) };
					return math::Point2{ x, y };
				}
				default:
					throw std::runtime_error{ "not a point instruction!" };
			}
		}

		Velocity evaluateVelocityNode(NODE_HANDLER_ARGS) {
			switch (currentSpawnNodePointer->spawnInstruction) {
				case components::SpawnInstructions::value: {
					const auto [value] =
						dynamic_cast<const components::SpawnNodeData<Velocity>*>(
							currentSpawnNodePointer.get()
						)->data;
					return value;
				}
				case components::SpawnInstructions::valueDifficulty: {
					const auto& [valueArray] =
						dynamic_cast<
						const components::SpawnNodeData<std::array<Velocity, 4>>*
						>(
							currentSpawnNodePointer.get()
						)->data;
					return valueArray[static_cast<int>(getDifficulty(scene))];
				}
				case components::SpawnInstructions::add: {
					Velocity a{ evaluateVelocityNode(
						scene,
						entityID,
						currentSpawnNodePointer->linkedNodePointers[0],
						tick,
						spawnList
					) };
					Velocity b{ evaluateVelocityNode(
						scene,
						entityID,
						currentSpawnNodePointer->linkedNodePointers[1],
						tick,
						spawnList
					) };
					return a + b;
				}
				case components::SpawnInstructions::multiply: {
					Velocity a{ evaluateVelocityNode(
						scene,
						entityID,
						currentSpawnNodePointer->linkedNodePointers[0],
						tick,
						spawnList
					) };
					float b{ evaluateFloatNode(
						scene,
						entityID,
						currentSpawnNodePointer->linkedNodePointers[1],
						tick,
						spawnList
					) };
					return a * b;
				}
				case components::SpawnInstructions::velocityFromPolar: {
					float magnitude{ evaluateFloatNode(
						scene,
						entityID,
						currentSpawnNodePointer->linkedNodePointers[0],
						tick,
						spawnList
					) };
					float angle{ evaluateFloatNode(
						scene,
						entityID,
						currentSpawnNodePointer->linkedNodePointers[1],
						tick,
						spawnList
					) };
					return Velocity{ magnitude, angle };
				}
				case components::SpawnInstructions::velocityToPoint: {
					math::Point2 a{ evaluatePointNode(
						scene,
						entityID,
						currentSpawnNodePointer->linkedNodePointers[0],
						tick,
						spawnList
					) };
					math::Point2 b{ evaluatePointNode(
						scene,
						entityID,
						currentSpawnNodePointer->linkedNodePointers[1],
						tick,
						spawnList
					) };
					return math::vectorFromAToB(a, b);
				}
				case components::SpawnInstructions::conditionElse: {
					//if our predicate is met, evaluate truenode
					if (evaluatePredicateNode(
						scene,
						entityID,
						currentSpawnNodePointer->linkedNodePointers[0],
						tick,
						spawnList
					)) {
						return evaluateVelocityNode(
							scene,
							entityID,
							currentSpawnNodePointer->linkedNodePointers[1],
							tick,
							spawnList
						);
					}
					//otherwise, evaluate falseNode
					else {
						return evaluateVelocityNode(
							scene,
							entityID,
							currentSpawnNodePointer->linkedNodePointers[2],
							tick,
							spawnList
						);
					}
					break;
				}
				default:
					throw std::runtime_error{ "not a velocity instruction!" };
			}
		}

		void runSpawnNodePassingPos(NODE_HANDLER_ARGS, const math::Point2& pos) {
			switch (currentSpawnNodePointer->spawnInstruction) {
				case components::SpawnInstructions::error:
					//throw an error
					throw std::runtime_error{ "spawn program error instruction" };
				case components::SpawnInstructions::spawnPos: {
					//cast the node data to a ComponentTupleSharedPtr
					const auto& [componentTupleBaseSharedPtr] =
						dynamic_cast<
							const components::SpawnNodeData<
								std::shared_ptr<ComponentTupleBase>
							>*
						>(
							currentSpawnNodePointer.get()
						)->data;

					//add position
					spawnList.emplace_back(
						componentTupleBaseSharedPtr->addPosition(pos)
					);

					currentSpawnNodePointer = nullptr;
					break;
				}
				default:
					throw std::runtime_error{ "cannot pass pos!" };
			}
		}

		void runSpawnNodePassingVel(NODE_HANDLER_ARGS, const Velocity& vel) {
			switch (currentSpawnNodePointer->spawnInstruction) {
				case components::SpawnInstructions::error:
					//throw an error
					throw std::runtime_error{ "spawn program error instruction" };
				case components::SpawnInstructions::spawnPosVel: {
					//cast the node data to a ComponentTupleSharedPtr
					const auto& [componentTupleBaseSharedPtr] =
						dynamic_cast<
							const components::SpawnNodeData<
								std::shared_ptr<ComponentTupleBase>
							>*
						>(
							currentSpawnNodePointer.get()
						)->data;

					//evaluate pos node
					math::Point2 pos{ evaluatePointNode(
						scene,
						entityID,
						currentSpawnNodePointer->linkedNodePointers[0],
						tick,
						spawnList
					) };

					//add position and velocity
					spawnList.emplace_back(
						componentTupleBaseSharedPtr->addPosVel(pos, vel)
					);

					currentSpawnNodePointer = nullptr;
					break;
				}
				default:
					throw std::runtime_error{ "cannot pass pos!" };
			}
		}

		void runSpawnNodePassingPosVel(
			NODE_HANDLER_ARGS,
			const math::Point2& pos,
			const Velocity& vel
		) {
			switch (currentSpawnNodePointer->spawnInstruction) {
				case components::SpawnInstructions::error:
					//throw an error
					throw std::runtime_error{ "spawn program error instruction" };
				case components::SpawnInstructions::spawnPosVel: {
					//cast the node data to a ComponentTupleSharedPtr
					const auto& [componentTupleBaseSharedPtr] =
						dynamic_cast<
							const components::SpawnNodeData<
								std::shared_ptr<ComponentTupleBase>
							>*
						>(
							currentSpawnNodePointer.get()
						)->data;
					//add position and velocity
					spawnList.emplace_back(
						componentTupleBaseSharedPtr->addPosVel(pos, vel)
					);

					currentSpawnNodePointer = nullptr;
					break;
				}
				default:
					throw std::runtime_error{ "cannot pass pos and vel!" };
			}
		}

		void runSpawnNode(NODE_HANDLER_ARGS) {
			switch (currentSpawnNodePointer->spawnInstruction) {
				case components::SpawnInstructions::error:
					//throw an error
					throw std::runtime_error{ "spawn program error instruction" };
				case components::SpawnInstructions::list: {
					//run every node contained in the list
					for (auto linkedNodeSharedPointer	//intentional copy
						: currentSpawnNodePointer->linkedNodePointers
					) {
						while (linkedNodeSharedPointer) {
							runSpawnNode(
								scene,
								entityID,
								linkedNodeSharedPointer,
								tick,
								spawnList
							);
						}
					}
					currentSpawnNodePointer = nullptr;
					break;
				}
				case components::SpawnInstructions::repeat: {
					int times{ evaluateIntNode(
						scene,
						entityID,
						currentSpawnNodePointer->linkedNodePointers[0],
						tick,
						spawnList
					) };
					for (int i{ 0 }; i < times; ++i) {
						auto linkedNodeSharedPointer{	//intentional copy
							currentSpawnNodePointer->linkedNodePointers[1]
						};
						while (linkedNodeSharedPointer) {
							runSpawnNode(
								scene,
								entityID,
								linkedNodeSharedPointer,
								tick,
								spawnList
							);
						}
					}
					currentSpawnNodePointer = nullptr;
					break;
				}
				case components::SpawnInstructions::condition: {
					//if our predicate is met, set the current pointer to trueNode
					if (evaluatePredicateNode(
						scene,
						entityID,
						currentSpawnNodePointer->linkedNodePointers[0],
						tick,
						spawnList
					)) {
						currentSpawnNodePointer =
							currentSpawnNodePointer->linkedNodePointers[1];
					}
					else {
						currentSpawnNodePointer = nullptr;
					}
					break;
				}
				case components::SpawnInstructions::conditionElse: {
					//if our predicate is met, set the current pointer to trueNode
					if (evaluatePredicateNode(
						scene,
						entityID,
						currentSpawnNodePointer->linkedNodePointers[0],
						tick,
						spawnList
					)) {
						currentSpawnNodePointer =
							currentSpawnNodePointer->linkedNodePointers[1];
					}
					//otherwise, set the current pointer to falseNode
					else {
						currentSpawnNodePointer =
							currentSpawnNodePointer->linkedNodePointers[2];
					}
					break;
				}
				case components::SpawnInstructions::playerPowerSplit: {
					//chooses node based on player power
					//buckets are as follows
					//0  - 7	bucket0
					//8  - 15	bucket1
					//16 - 31	bucket2
					//32 - 47	bucket3
					//48 - 63	bucket4
					//64 - 79	bucket5
					//80		bucket6
					const PlayerData& playerData{
						scene.getDataStorage().getComponent<PlayerData>(entityID)
					};
					int bucket{ -1 };
					switch (playerData.power / 8) {
						case 0:
							bucket = 0;
							break;
						case 1:
							bucket = 1;
							break;
						case 2:
						case 3:
							bucket = 2;
							break;
						case 4:
						case 5:
							bucket = 3;
							break;
						case 6:
						case 7:
							bucket = 4;
							break;
						case 8:
						case 9:
							bucket = 5;
							break;
						case 10:
							bucket = 6;
							break;
						default:
							throw std::runtime_error{
								"unexpected power: " + std::to_string(playerData.power)
							};
					}
					currentSpawnNodePointer =
						currentSpawnNodePointer->linkedNodePointers[bucket];
					break;
				}
				case components::SpawnInstructions::spawn: {
					//cast the node data to a ComponentTupleSharedPtr and add to our list
					const auto& [componentTupleBaseSharedPtr] =
						dynamic_cast<
							const components::SpawnNodeData<
								std::shared_ptr<ComponentTupleBase>
							>*
						>(
							currentSpawnNodePointer.get()
						)->data;
					spawnList.emplace_back(componentTupleBaseSharedPtr);
					currentSpawnNodePointer = nullptr;
					break;
				}
				case components::SpawnInstructions::spawnPos: {
					//cast the node data to a ComponentTupleSharedPtr
					const auto& [componentTupleBaseSharedPtr] =
						dynamic_cast<
							const components::SpawnNodeData<
								std::shared_ptr<ComponentTupleBase>
							>*
						>(
							currentSpawnNodePointer.get()
						)->data;
					//get the position
					math::Point2 pos{ evaluatePointNode(
						scene,
						entityID,
						currentSpawnNodePointer->linkedNodePointers[0],
						tick,
						spawnList
					) };
					//add position
					spawnList.emplace_back(
						componentTupleBaseSharedPtr->addPosition(pos)
					);
					currentSpawnNodePointer = nullptr;
					break;
				}
				case components::SpawnInstructions::spawnPosVel: {
					//cast the node data to a ComponentTupleSharedPtr
					const auto& [componentTupleBaseSharedPtr] =
						dynamic_cast<
							const components::SpawnNodeData<
								std::shared_ptr<ComponentTupleBase>
							>*
						>(
							currentSpawnNodePointer.get()
						)->data;
					//get the position
					math::Point2 pos{ evaluatePointNode(
						scene,
						entityID,
						currentSpawnNodePointer->linkedNodePointers[0],
						tick,
						spawnList
					) };
					//get the velocity
					Velocity vel{ evaluateVelocityNode(
						scene,
						entityID,
						currentSpawnNodePointer->linkedNodePointers[1],
						tick,
						spawnList
					) };
					//add position and velocity
					spawnList.emplace_back(
						componentTupleBaseSharedPtr->addPosVel(pos, vel)
					);
					currentSpawnNodePointer = nullptr;
					break;
				}
				case components::SpawnInstructions::mirrorFormation: {
					//get the base position
					math::Point2 basePos{ evaluatePointNode(
						scene,
						entityID,
						currentSpawnNodePointer->linkedNodePointers[0],
						tick,
						spawnList
					) };
					//get the base velocity
					Velocity baseVel{ evaluateVelocityNode(
						scene,
						entityID,
						currentSpawnNodePointer->linkedNodePointers[1],
						tick,
						spawnList
					) };
					//get the axis of symmetry (x coordinate to reflect around)
					float axis{ evaluateFloatNode(
						scene,
						entityID,
						currentSpawnNodePointer->linkedNodePointers[2],
						tick,
						spawnList
					) };

					//in point slope form, our new X n follows the equation
					// n - a = -(o - a)
					// where a is our axis and o is the old X
					math::Point2 mirrorPos{ -basePos.x + (2 * axis), basePos.y };
					Velocity mirrorVel{
						baseVel.getMagnitude(),
						baseVel.getAngle().flipY() 
					};

					//pass both normal and reflected pos/vel pairs
					auto posVelConsumerSharedPointer{
						currentSpawnNodePointer->linkedNodePointers[3]
					};
					while (posVelConsumerSharedPointer) {
						runSpawnNodePassingPosVel(
							scene,
							entityID,
							posVelConsumerSharedPointer,
							tick,
							spawnList,
							basePos,
							baseVel
						);
					}
					posVelConsumerSharedPointer =
						currentSpawnNodePointer->linkedNodePointers[3];
					while (posVelConsumerSharedPointer) {
						runSpawnNodePassingPosVel(
							scene,
							entityID,
							posVelConsumerSharedPointer,
							tick,
							spawnList,
							mirrorPos,
							mirrorVel
						);
					}

					currentSpawnNodePointer = nullptr;
					break;
				}
				case components::SpawnInstructions::mirrorPosFormation: {
					//get the base position
					math::Point2 basePos{ evaluatePointNode(
						scene,
						entityID,
						{ currentSpawnNodePointer->linkedNodePointers[0] },
						tick,
						spawnList
					) };
					//get the axis of symmetry (x coordinate to reflect around)
					float axis{ evaluateFloatNode(
						scene,
						entityID,
						{ currentSpawnNodePointer->linkedNodePointers[1] },
						tick,
						spawnList
					) };

					//in point slope form, our new X n follows the equation
					// n - a = -(o - a)
					// where a is our axis and o is the old X
					math::Point2 mirrorPos{ -basePos.x + (2 * axis), basePos.y };

					//pass both normal and reflected pos
					auto posConsumerSharedPointer{
						currentSpawnNodePointer->linkedNodePointers[2]
					};
					while (posConsumerSharedPointer) {
						runSpawnNodePassingPos(
							scene,
							entityID,
							posConsumerSharedPointer,
							tick,
							spawnList,
							basePos
						);
					}
					posConsumerSharedPointer =
						currentSpawnNodePointer->linkedNodePointers[2];
					while (posConsumerSharedPointer) {
						runSpawnNodePassingPos(
							scene,
							entityID,
							posConsumerSharedPointer,
							tick,
							spawnList,
							mirrorPos
						);
					}

					currentSpawnNodePointer = nullptr;
					break;
				}
				case components::SpawnInstructions::arcFormation: {
					//get the base velocity
					Velocity baseVel{ evaluateVelocityNode(
						scene,
						entityID,
						currentSpawnNodePointer->linkedNodePointers[0],
						tick,
						spawnList
					) };
					int symmetry{ evaluateIntNode(
						scene,
						entityID,
						currentSpawnNodePointer->linkedNodePointers[1],
						tick,
						spawnList
					) };
					float angleIncrement{ evaluateFloatNode(
						scene,
						entityID,
						currentSpawnNodePointer->linkedNodePointers[2],
						tick,
						spawnList
					) };
					float speed{ baseVel.getMagnitude() };
					math::Angle baseAngle{ baseVel.getAngle() };
					math::Angle angle{
						static_cast<float>(baseAngle) -
						((symmetry - 1) * angleIncrement / 2.0f)
					};
					for (int i{ 0 }; i < symmetry; ++i) {
						auto velConsumerSharedPointer{
							currentSpawnNodePointer->linkedNodePointers[3]
						};
						while (velConsumerSharedPointer) {
							runSpawnNodePassingVel(
								scene,
								entityID,
								velConsumerSharedPointer,
								tick,
								spawnList,
								Velocity{ speed, angle }
							);
						}
						angle += angleIncrement;
					}

					currentSpawnNodePointer = nullptr;
					break;
				}
				case components::SpawnInstructions::ringFormation: {
					//get the base velocity
					Velocity baseVel{ evaluateVelocityNode(
						scene,
						entityID,
						currentSpawnNodePointer->linkedNodePointers[0],
						tick,
						spawnList
					) };
					int symmetry{ evaluateIntNode(
						scene,
						entityID,
						currentSpawnNodePointer->linkedNodePointers[1],
						tick,
						spawnList
					) };

					float speed{ baseVel.getMagnitude() };
					math::Angle angle{ baseVel.getAngle() };

					float angleIncrement{ math::fullAngleDivide(symmetry) };

					for (int i{ 0 }; i < symmetry; ++i) {
						auto velConsumerSharedPointer{
							currentSpawnNodePointer->linkedNodePointers[2]
						};
						while (velConsumerSharedPointer) {
							runSpawnNodePassingVel(
								scene,
								entityID,
								velConsumerSharedPointer,
								tick,
								spawnList,
								Velocity{ speed, angle }
							);
						}
						angle += angleIncrement;
					}

					currentSpawnNodePointer = nullptr;
					break;
				}
				default:
					throw std::runtime_error{ "unknown spawn instruction" };
			}
		}
		
		#undef NODE_HANDLER_ARGS

		//Returns true if the program can continue, false if the program is over.
		bool runSpawnProgram(
			Scene& scene,
			components::SpawnProgram& spawnProgram,
			EntityID entityID,
			SpawnList& spawnList
		) {
			int tick{ spawnProgram.stepAndGetTick() };
			if (tick <= 0) {
				return false;
			}
			auto currentSpawnNodePointer { spawnProgram.baseSpawnNodePointer };

			while (currentSpawnNodePointer) {
				runSpawnNode(
					scene,
					entityID,
					currentSpawnNodePointer,
					tick,
					spawnList
				);
			}

			return true;
		}

		void runSpawnProgramList(
			Scene& scene,
			SpawnProgramList& spawnProgramList,
			EntityID entityID,
			SpawnList& spawnList
		) {
			for (auto itr{ spawnProgramList.begin() }; itr != spawnProgramList.end();) {
				if (runSpawnProgram(scene, *itr, entityID, spawnList)) {
					++itr;
				}
				else {
					itr = spawnProgramList.erase(itr);
				}
			}
		}
	}

	void SpawnSystem::operator()(Scene& scene) {
		//create our spawnlist
		SpawnList spawnList{};

		//get the group iterator for SpawnProgramList
		static const Topic<ecs::component::Group*> groupPointerStorageTopic{};
		auto groupPointer{
			getGroupPointer<SpawnProgramList>(
				scene,
				groupPointerStorageTopic
			)
		};
		auto groupIterator{ groupPointer->groupIterator<SpawnProgramList>() };

		//populate our spawnList with every spawn this tick
		while (groupIterator.isValid()) {
			auto [spawnProgramList] = *groupIterator;
			EntityID entityID{ groupIterator.getEntityID() };
			runSpawnProgramList(scene, spawnProgramList, entityID, spawnList);
			++groupIterator;
		}

		//add our spawnList to the ecs world
		auto& dataStorage{ scene.getDataStorage() };
		for (const auto& spawnPointer : spawnList) {
			spawnPointer->addTo(dataStorage);
		}
	}
}