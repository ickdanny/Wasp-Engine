#include "Game/Systems/ScriptSystem.h"

#include "Game/Systems/ComponentOrderQueue.h"

#include "Logging.h"

namespace wasp::game::systems {

	using namespace ecs;
	using namespace ecs::entity;

	using ScriptInstructions = components::ScriptInstructions;
	using ScriptNode = components::ScriptNode;
	template <typename Internal, typename External>
	using ScriptNodeData = components::ScriptNodeData<Internal, External>;

	namespace {

		#define NODE_HANDLER_ARGS \
			Scene& scene, \
			EntityID entityID, \
			std::shared_ptr<ScriptNode>& currentScriptNodePointer, \
			std::unordered_map<ScriptNode*, void*>& externalData, \
			ComponentOrderQueue& componentOrderQueue

		bool evaluatePredicateNode(NODE_HANDLER_ARGS) {
			switch (currentScriptNodePointer->scriptInstruction) {
				case ScriptInstructions::isSpawning: {
					const auto& dataStorage{ scene.getDataStorage() };
					if (dataStorage.containsComponent<SpawnProgramList>(entityID)) {
						const auto& spawnProgramList{
							dataStorage.getComponent<SpawnProgramList>(entityID)
						};
						return spawnProgramList.size() > 0;
					}
					return false;
				}
				default:
					throw std::runtime_error{ "not a predicate instruction!" };
			}
		}

		bool runScriptNode(NODE_HANDLER_ARGS) {
			switch (currentScriptNodePointer->scriptInstruction) {
				case ScriptInstructions::error:
					throw std::runtime_error{ "error script instruction" };
				case ScriptInstructions::condition: {
					if (evaluatePredicateNode(
						scene,
						entityID,
						currentScriptNodePointer->linkedNodePointers[0],
						externalData,
						componentOrderQueue
					)) {
						currentScriptNodePointer 
							= currentScriptNodePointer->linkedNodePointers[1];
						return true;
					}
					//this instruction ends the whole script segment if false
					else {
						currentScriptNodePointer = nullptr;
						return false;
					}
				}
				case ScriptInstructions::stallCondition: {
					if (evaluatePredicateNode(
						scene,
						entityID,
						currentScriptNodePointer->linkedNodePointers[0],
						externalData,
						componentOrderQueue
					)) {
						currentScriptNodePointer
							= currentScriptNodePointer->linkedNodePointers[1];
						return true;
					}
					//this instruction stalls until predicate is true
					else {
						return false;
					}
				}
				case ScriptInstructions::conditionElse: {
					if (evaluatePredicateNode(
						scene,
						entityID,
						currentScriptNodePointer->linkedNodePointers[0],
						externalData,
						componentOrderQueue
					)) {
						currentScriptNodePointer
							= currentScriptNodePointer->linkedNodePointers[1];
					}
					//this instruction either continues with trueNode or falseNode
					else {
						currentScriptNodePointer
							= currentScriptNodePointer->linkedNodePointers[2];
					}
					return true;
				}
				case ScriptInstructions::timer: {
					auto dataNodePointer{
						dynamic_cast<ScriptNodeData<int, utility::Ticker>*>(
							currentScriptNodePointer.get()
						)
					};

					if (externalData.find(currentScriptNodePointer.get())
						== externalData.end()
					) {
						int maxTick{ dataNodePointer->internalData };
						externalData[currentScriptNodePointer.get()]
							= new utility::Ticker{ maxTick, false };
					}
					utility::Ticker& ticker{
						*dataNodePointer->getDataPointer(
							externalData[currentScriptNodePointer.get()]
						)
					};
					if (ticker.stepAndGetTick() <= 0) {
						if (currentScriptNodePointer->linkedNodePointers.size() > 0) {
							currentScriptNodePointer
								= dataNodePointer->linkedNodePointers[0];
						}
						else {
							currentScriptNodePointer = nullptr;
						}
						return true;
					}
					else {
						return false;
					}
				}
				case ScriptInstructions::shiftSpeedPeriod: {
					Velocity& velocity{
						scene.getDataStorage().getComponent<Velocity>(entityID)
					};
					float oldMagnitude{ velocity.getMagnitude() };

					auto dataNodePointer{
						dynamic_cast<ScriptNodeData<std::tuple<float, int>, float>*>(
							currentScriptNodePointer.get()
						)
					};

					auto [targetSpeed, ticks] = dataNodePointer->internalData;

					bool hasReachedSpeed{ false };

					//if we are at speed already, move to next instruction
					if (oldMagnitude == targetSpeed) {
						hasReachedSpeed = true;
					}
					else {
						//retrieve or create increment
						float increment{};
						if (externalData.find(currentScriptNodePointer.get())
							!= externalData.end()
						) {
							increment = *dataNodePointer->getDataPointer(
								externalData[currentScriptNodePointer.get()]
							);
						}
						else {
							float magnitudeDiff{ targetSpeed - oldMagnitude };
							increment = ticks > 0
								? magnitudeDiff / static_cast<float>(ticks)
								: magnitudeDiff;
							externalData[currentScriptNodePointer.get()]
								= new float{ increment };
						}

						//increment velocity
						float newMagnitude{ oldMagnitude + increment };

						//check to see if we have reached target speed
						if (increment > 0 && newMagnitude > targetSpeed) {
							hasReachedSpeed = true;
							velocity.setMagnitude(targetSpeed);
						}
						else if (increment < 0 && newMagnitude < targetSpeed) {
							hasReachedSpeed = true;
							velocity.setMagnitude(targetSpeed);
						}
						else {
							velocity.setMagnitude(newMagnitude);
						}
					}
					if (hasReachedSpeed) {
						if (currentScriptNodePointer->linkedNodePointers.size() > 0) {
							currentScriptNodePointer
								= dataNodePointer->linkedNodePointers[0];
						}
						else {
							currentScriptNodePointer = nullptr;
						}
						return true;
					}
					else {
						return false;
					}
				}
				default:
					throw std::runtime_error{ "unhandled script instruction!" };
			}
		}

		void runScriptProgramList(
			Scene& scene,
			ScriptProgramList& scriptProgramList,
			EntityID entityID,
			ComponentOrderQueue& componentOrderQueue
		) {
			for (auto itr{ scriptProgramList.begin() }; itr != scriptProgramList.end();){
				auto& scriptProgram{ *itr };
				while (scriptProgram.currentNodePointer) {
					if (!runScriptNode(
						scene,
						entityID,
						scriptProgram.currentNodePointer,
						scriptProgram.externalData,
						componentOrderQueue
					)) {
						break;
					}
				}
				if(!scriptProgram.currentNodePointer) {
					itr = scriptProgramList.erase(itr);
				}
				else {
					++itr;
				}
			}
		}

		#undef NODE_HANDLER_ARGS
	}

	void ScriptSystem::operator()(Scene& scene) {
		//create our component order queue
		ComponentOrderQueue componentOrderQueue{};

		//get the group iterator for ScriptProgramList
		static const Topic<ecs::component::Group*> groupPointerStorageTopic{};
		auto groupPointer{
			getGroupPointer<ScriptProgramList>(
				scene,
				groupPointerStorageTopic
			)
		};
		auto groupIterator{ groupPointer->groupIterator<ScriptProgramList>() };

		//populate our component order queue with every component order this tick
		while (groupIterator.isValid()) {
			auto [scriptProgramList] = *groupIterator;
			EntityID entityID{ groupIterator.getEntityID() };
			runScriptProgramList(
				scene, 
				scriptProgramList, 
				entityID, 
				componentOrderQueue
			);
			++groupIterator;
		}

		//apply all our queued component orders to the ecs world
		componentOrderQueue.apply(scene.getDataStorage());
	}
}