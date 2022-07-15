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

		constexpr float angleEquivalenceEpsilon{ .01f };

		void clearExternalDataForNode(
			std::shared_ptr<ScriptNode>& currentScriptNodePointer,
			std::unordered_map<ScriptNode*, void*>& externalData
		) {
			if (externalData.find(currentScriptNodePointer.get())
				!= externalData.end()
			) {
				currentScriptNodePointer->clearData(
					externalData[currentScriptNodePointer.get()]
				);
				externalData.erase(currentScriptNodePointer.get());
			}
		}

		//goes to the node specified by nextIndex if it exists
		void gotoNextNode(
			std::shared_ptr<ScriptNode>& currentScriptNodePointer,
			std::size_t nextIndex
		) {
			if (currentScriptNodePointer->linkedNodePointers.size() > nextIndex) {
				currentScriptNodePointer
					= currentScriptNodePointer->linkedNodePointers[nextIndex];
			}
			else {
				currentScriptNodePointer = nullptr;
			}
		}

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
				case ScriptInstructions::isNotSpawning: {
					const auto& dataStorage{ scene.getDataStorage() };
					if (dataStorage.containsComponent<SpawnProgramList>(entityID)) {
						const auto& spawnProgramList{
							dataStorage.getComponent<SpawnProgramList>(entityID)
						};
						return spawnProgramList.size() == 0;
					}
					return true;
				}
				default:
					throw std::runtime_error{ "not a predicate instruction!" };
			}
		}

		Velocity evaluateVelocityNode(NODE_HANDLER_ARGS) {
			switch (currentScriptNodePointer->scriptInstruction) {
				case ScriptInstructions::value: {
					auto dataNodePointer{
						dynamic_cast<ScriptNodeData<Velocity, utility::Void>*>(
							currentScriptNodePointer.get()
						)
					};
					return dataNodePointer->internalData;
				}
				default:
					throw std::runtime_error{ "not a velocity instruction!" };
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
					//move on to next node if present
					if (ticker.stepAndGetTick() <= 0) {
						clearExternalDataForNode(currentScriptNodePointer, externalData);
						gotoNextNode(currentScriptNodePointer, 0);
						return true;
					}
					else {
						return false;
					}
				}
				case ScriptInstructions::removeVisible: {
					EntityHandle entityHandle{
						scene.getDataStorage().makeHandle(entityID)
					};
					componentOrderQueue.queueRemoveComponent<VisibleMarker>(
						entityHandle
					);
					gotoNextNode(currentScriptNodePointer, 0);
					return true;
				}
				case ScriptInstructions::setSpriteInstruction: {
					auto dataNodePointer{
						dynamic_cast<ScriptNodeData<SpriteInstruction, utility::Void>*>(
							currentScriptNodePointer.get()
						)
					};
					EntityHandle entityHandle{
						scene.getDataStorage().makeHandle(entityID)
					};
					componentOrderQueue.queueSetComponent<SpriteInstruction>(
						entityHandle,
						dataNodePointer->internalData
					);
					gotoNextNode(currentScriptNodePointer, 0);
					return true;
				}
				case ScriptInstructions::setCollidable: {
					EntityHandle entityHandle{
						scene.getDataStorage().makeHandle(entityID)
					};
					componentOrderQueue.queueSetComponent<CollidableMarker>(
						entityHandle, {}
					);
					gotoNextNode(currentScriptNodePointer, 0);
					return true;
				}
				case ScriptInstructions::removeCollidable: {
					EntityHandle entityHandle{
						scene.getDataStorage().makeHandle(entityID)
					};
					componentOrderQueue.queueRemoveComponent<CollidableMarker>(
						entityHandle
						);
					gotoNextNode(currentScriptNodePointer, 0);
					return true;
				}
				case ScriptInstructions::setHealth: {
					auto dataNodePointer{
						dynamic_cast<ScriptNodeData<int, utility::Void>*>(
							currentScriptNodePointer.get()
						)
					};
					EntityHandle entityHandle{
						scene.getDataStorage().makeHandle(entityID)
					};
					componentOrderQueue.queueSetComponent<Health>(
						entityHandle, { dataNodePointer->internalData }
					);
					gotoNextNode(currentScriptNodePointer, 0);
					return true;
				}
				case ScriptInstructions::removeHealth: {
					EntityHandle entityHandle{
						scene.getDataStorage().makeHandle(entityID)
					};
					componentOrderQueue.queueRemoveComponent<Health>(
						entityHandle
						);
					gotoNextNode(currentScriptNodePointer, 0);
					return true;
				}
				case ScriptInstructions::setDamage: {
					auto dataNodePointer{
						dynamic_cast<ScriptNodeData<int, utility::Void>*>(
							currentScriptNodePointer.get()
						)
					};
					EntityHandle entityHandle{
						scene.getDataStorage().makeHandle(entityID)
					};
					componentOrderQueue.queueSetComponent<Damage>(
						entityHandle, { dataNodePointer->internalData }
					);
					gotoNextNode(currentScriptNodePointer, 0);
					return true;
				}
				case ScriptInstructions::removeDamage: {
					EntityHandle entityHandle{
						scene.getDataStorage().makeHandle(entityID)
					};
					componentOrderQueue.queueRemoveComponent<Damage>(
						entityHandle
						);
					gotoNextNode(currentScriptNodePointer, 0);
					return true;
				}
				case ScriptInstructions::addSpawn: {
					if (scene.getDataStorage()
						.containsComponent<SpawnProgramList>(entityID)
					) {
						auto dataNodePointer{
							dynamic_cast<
								ScriptNodeData<components::SpawnProgram, utility::Void>*
							>(
								currentScriptNodePointer.get()
							)
						};
						scene.getDataStorage().getComponent<SpawnProgramList>(entityID)
							.push_back(dataNodePointer->internalData);
						gotoNextNode(currentScriptNodePointer, 0);
						return true;
					}
					//else fallthrough to setSpawn
				}
				case ScriptInstructions::setSpawn: {
					auto dataNodePointer{
						dynamic_cast<
							ScriptNodeData<components::SpawnProgram, utility::Void>*
						>(
							currentScriptNodePointer.get()
						)
					};
					EntityHandle entityHandle{
						scene.getDataStorage().makeHandle(entityID)
					};
					componentOrderQueue.queueSetComponent<SpawnProgramList>(
						entityHandle, { dataNodePointer->internalData }
					);
					gotoNextNode(currentScriptNodePointer, 0);
					return true;
				}
				case ScriptInstructions::clearSpawn: {
					if (scene.getDataStorage()
						.containsComponent<SpawnProgramList>(entityID)
					) {
						scene.getDataStorage().getComponent<SpawnProgramList>(entityID)
							.clear();
					}
					gotoNextNode(currentScriptNodePointer, 0);
					return true;
				}
				case ScriptInstructions::setVelocity: {
					Velocity velocity{
						evaluateVelocityNode(
							scene,
							entityID,
							currentScriptNodePointer->linkedNodePointers[0],
							externalData,
							componentOrderQueue
						)
					};
					EntityHandle entityHandle{
						scene.getDataStorage().makeHandle(entityID)
					};
					componentOrderQueue.queueSetComponent<Velocity>(
						entityHandle,
						velocity
						);
					gotoNextNode(currentScriptNodePointer, 1);
					return true;
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

					const auto [targetSpeed, ticks] = dataNodePointer->internalData;

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
					//move on to next node if present
					if (hasReachedSpeed) {
						clearExternalDataForNode(currentScriptNodePointer, externalData);
						gotoNextNode(currentScriptNodePointer, 0);
						return true;
					}
					else {
						return false;
					}
				}
				case ScriptInstructions::shiftVelocityPeriod: {
					Velocity& velocity{
						scene.getDataStorage().getComponent<Velocity>(entityID)
					};
					float oldMagnitude{ velocity.getMagnitude() };

					auto dataNodePointer{
						dynamic_cast<ScriptNodeData<std::tuple<Velocity, int>, float>*>(
							currentScriptNodePointer.get()
						)
					};

					const auto [targetVelocity, ticks] = dataNodePointer->internalData;
					float targetSpeed{ targetVelocity.getMagnitude() };

					//do not turn; set our angle to target angle
					if (externalData.find(currentScriptNodePointer.get())
						== externalData.end()
					) {
						velocity.setAngle(targetVelocity.getAngle());
					}

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
					//move on to next node if present
					if (hasReachedSpeed) {
						clearExternalDataForNode(currentScriptNodePointer, externalData);
						gotoNextNode(currentScriptNodePointer, 0);
						return true;
					}
					else {
						return false;
					}
				}
				case ScriptInstructions::shiftVelocityTurnPeriod: {
					Velocity& velocity{
						scene.getDataStorage().getComponent<Velocity>(entityID)
					};
					float oldMagnitude{ velocity.getMagnitude() };
					auto oldAngle{ velocity.getAngle() };

					auto dataNodePointer{
						dynamic_cast<ScriptNodeData<
							std::tuple<Velocity, math::Angle, int>, 
							std::tuple<float, float>
						>*>(
							currentScriptNodePointer.get()
						)
					};

					const auto [targetVelocity, initAngle, ticks] 
						= dataNodePointer->internalData;
					const float targetSpeed{ targetVelocity.getMagnitude() };
					const auto targetAngle{ targetVelocity.getAngle() };

					bool hasReachedSpeed{ false };
					bool hasReachedAngle{ false };

					if (oldMagnitude == targetSpeed) {
						hasReachedSpeed = true;
					}
					if (oldAngle == targetAngle) {
						hasReachedAngle = true;
					}

					if(!hasReachedSpeed || !hasReachedAngle) {
						//retrieve or create increment
						float speedIncrement{};
						float angleIncrement{};
						if (externalData.find(currentScriptNodePointer.get())
							!= externalData.end()
						) {
							auto [storedSpeedIncrement, storedAngleIncrement] 
								= *dataNodePointer->getDataPointer(
									externalData[currentScriptNodePointer.get()]
								);
							speedIncrement = storedSpeedIncrement;
							angleIncrement = storedAngleIncrement;
						}
						else {
							//create and store increments
							float magnitudeDiff{ targetSpeed - oldMagnitude };
							speedIncrement = ticks > 0
								? magnitudeDiff / static_cast<float>(ticks)
								: magnitudeDiff;

							float angleDiff{ targetAngle.smallerDifference(initAngle) };
							angleIncrement = ticks > 0
								? angleDiff / static_cast<float>(ticks)
								: angleDiff;

							externalData[currentScriptNodePointer.get()]
								= new std::tuple<float, float>{ 
									speedIncrement, 
									angleIncrement 
								};
						}

						if (!hasReachedSpeed) {
							//increment speed
							float newMagnitude{ oldMagnitude + speedIncrement };

							//check to see if we have reached target speed
							if (speedIncrement > 0 && newMagnitude > targetSpeed) {
								hasReachedSpeed = true;
								velocity.setMagnitude(targetSpeed);
							}
							else if (speedIncrement < 0 && newMagnitude < targetSpeed) {
								speedIncrement = true;
								velocity.setMagnitude(targetSpeed);
							}
							else {
								velocity.setMagnitude(newMagnitude);
							}
						}

						if (!hasReachedAngle) {
							//increment angle
							math::Angle newAngle{
								static_cast<float>(oldAngle) + angleIncrement
							};

							//check to see if we have reached target angle
							if (
								std::abs(static_cast<float>(
									targetAngle.smallerDifference(newAngle)
									)) <= angleEquivalenceEpsilon
								) {
								hasReachedAngle = true;
								velocity.setAngle(targetAngle);
							}
							else {
								velocity.setAngle(newAngle);
							}
						}
					}
					//move on to next node if present
					if (hasReachedSpeed && hasReachedAngle) {
						clearExternalDataForNode(currentScriptNodePointer, externalData);
						gotoNextNode(currentScriptNodePointer, 0);
						return true;
					}
					else {
						return false;
					}
				}
				case ScriptInstructions::shiftVelocityTurnLongPeriod: {
					Velocity& velocity{
						scene.getDataStorage().getComponent<Velocity>(entityID)
					};
					float oldMagnitude{ velocity.getMagnitude() };
					auto oldAngle{ velocity.getAngle() };

					auto dataNodePointer{
						dynamic_cast<ScriptNodeData<
							std::tuple<Velocity, math::Angle, int>,
							std::tuple<float, float>
						>*>(
							currentScriptNodePointer.get()
						)
					};

					const auto [targetVelocity, initAngle, ticks]
						= dataNodePointer->internalData;
					const float targetSpeed{ targetVelocity.getMagnitude() };
					const auto targetAngle{ targetVelocity.getAngle() };

					bool hasReachedSpeed{ false };
					bool hasReachedAngle{ false };

					if (oldMagnitude == targetSpeed) {
						hasReachedSpeed = true;
					}
					if (oldAngle == targetAngle) {
						hasReachedAngle = true;
					}

					if (!hasReachedSpeed || !hasReachedAngle) {
						//retrieve or create increment
						float speedIncrement{};
						float angleIncrement{};
						if (externalData.find(currentScriptNodePointer.get())
							!= externalData.end()
							) {
							auto [storedSpeedIncrement, storedAngleIncrement]
								= *dataNodePointer->getDataPointer(
									externalData[currentScriptNodePointer.get()]
								);
							speedIncrement = storedSpeedIncrement;
							angleIncrement = storedAngleIncrement;
						}
						else {
							//create and store increments
							float magnitudeDiff{ targetSpeed - oldMagnitude };
							speedIncrement = ticks > 0
								? magnitudeDiff / static_cast<float>(ticks)
								: magnitudeDiff;

							float angleDiff{ targetAngle.largerDifference(initAngle) };
							angleIncrement = ticks > 0
								? angleDiff / static_cast<float>(ticks)
								: angleDiff;

							externalData[currentScriptNodePointer.get()]
								= new std::tuple<float, float>{
									speedIncrement,
									angleIncrement
							};
						}

						if (!hasReachedSpeed) {
							//increment speed
							float newMagnitude{ oldMagnitude + speedIncrement };

							//check to see if we have reached target speed
							if (speedIncrement > 0 && newMagnitude > targetSpeed) {
								hasReachedSpeed = true;
								velocity.setMagnitude(targetSpeed);
							}
							else if (speedIncrement < 0 && newMagnitude < targetSpeed) {
								speedIncrement = true;
								velocity.setMagnitude(targetSpeed);
							}
							else {
								velocity.setMagnitude(newMagnitude);
							}
						}

						if (!hasReachedAngle) {
							//increment angle
							math::Angle newAngle{
								static_cast<float>(oldAngle) + angleIncrement
							};

							//check to see if we have reached target angle
							if (
								std::abs(static_cast<float>(
									targetAngle.smallerDifference(newAngle)
									)) <= angleEquivalenceEpsilon
								) {
								hasReachedAngle = true;
								velocity.setAngle(targetAngle);
							}
							else {
								velocity.setAngle(newAngle);
							}
						}
					}
					//move on to next node if present
					if (hasReachedSpeed && hasReachedAngle) {
						clearExternalDataForNode(currentScriptNodePointer, externalData);
						gotoNextNode(currentScriptNodePointer, 0);
						return true;
					}
					else {
						return false;
					}
				}
				case ScriptInstructions::shiftSpeedIncrement: {
					Velocity& velocity{
						scene.getDataStorage().getComponent<Velocity>(entityID)
					};
					float oldMagnitude{ velocity.getMagnitude() };

					auto dataNodePointer{
						dynamic_cast<ScriptNodeData<
							std::tuple<float, float>, 
							utility::Void
						>*>(
							currentScriptNodePointer.get()
						)
					};

					auto [targetSpeed, increment] = dataNodePointer->internalData;

					bool hasReachedSpeed{ false };

					//if we are at speed already, move to next instruction
					if (oldMagnitude == targetSpeed) {
						hasReachedSpeed = true;
					}
					else {
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
					//move on to next node if present
					if (hasReachedSpeed) {
						clearExternalDataForNode(currentScriptNodePointer, externalData);
						gotoNextNode(currentScriptNodePointer, 0);
						return true;
					}
					else {
						return false;
					}
				}
				
				case ScriptInstructions::removeEntity: {
					componentOrderQueue.queueRemoveEntity(
						scene.getDataStorage().makeHandle(entityID)
					);
					gotoNextNode(currentScriptNodePointer, 0);
					return true;
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