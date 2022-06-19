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
				default:
					throw std::runtime_error{ "not an int instruction!" };
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
				case components::SpawnInstructions::entityPosition:
					return scene.getDataStorage().getComponent<Position>(entityID);
				default:
					throw std::runtime_error{ "not a predicate instruction!" };
			}
		}

		void runSpawnNode(NODE_HANDLER_ARGS) {
			switch (currentSpawnNodePointer->spawnInstruction) {
				case components::SpawnInstructions::error:
					//throw an error
					throw std::runtime_error{ "spawn program error instruction" };
				case components::SpawnInstructions::list: {
					//run every node contained in the list
					for (auto& linkedNodeSharedPointer
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
						case 2:
						case 3:
							bucket = 2;
						case 4:
						case 5:
							bucket = 3;
						case 6:
						case 7:
							bucket = 4;
						case 8:
						case 9:
							bucket = 5;
						case 10:
							bucket = 6;
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