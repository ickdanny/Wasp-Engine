#include "Game/Systems/DeathHandlerSystem.h"

#include "Game/Systems/EntityBuilder.h"
#include "Game/Systems/SpawnPrograms/ScriptProgramUtil.h"

#include "Logging.h"

namespace wasp::game::systems {

	DeathHandlerSystem::DeathHandlerSystem()
		: ghostProgram{
			ScriptProgramUtil::makeStallingIfNode(
				std::make_shared<ScriptNode>(ScriptInstructions::isNotSpawning),
				std::make_shared<ScriptNode>(ScriptInstructions::removeEntity)
			)
		} 
	{
	}

	void DeathHandlerSystem::operator()(Scene& scene) {
		if (scene.hasChannel(SceneTopics::deaths)) {
			const auto& deathChannel{ scene.getChannel(SceneTopics::deaths) };
			auto& dataStorage{ scene.getDataStorage() };

			for (const auto& entityHandle : deathChannel.getMessages()) {
				//if the entity has a special deathCommand, defer to that
				if (dataStorage.containsComponent<DeathCommand>(entityHandle)) {
					const DeathCommand deathCommand{
						dataStorage.getComponent<DeathCommand>(entityHandle)
					};
					handleDeath(scene, entityHandle, deathCommand.command);
				}
				//otherwise, remove the entity
				else {
					dataStorage.removeEntity({ entityHandle });
				}
			}
		}
	}

	void DeathHandlerSystem::handleDeath(
		Scene& scene,
		const EntityHandle& entityHandle,
		const DeathCommand::Commands command
	) {
		switch (command) {
			case DeathCommand::Commands::playerDeath:
				handlePlayerDeath(scene, entityHandle);
				break;
			case DeathCommand::Commands::bossDeath:
				handleBossDeath(scene, entityHandle);
				break;
			case DeathCommand::Commands::deathSpawn:
				handleDeathSpawn(scene, entityHandle);
				break;
		}
	}

	void DeathHandlerSystem::handlePlayerDeath(
		Scene& scene,
		const EntityHandle& playerHandle
	) {
		//todo: handle player death
		debug::log("player death");
	}

	void DeathHandlerSystem::handleBossDeath(
		Scene& scene, 
		const EntityHandle& bossHandle
	) {
		scene.getChannel(SceneTopics::bossDeaths).addMessage(bossHandle);
	}

	void DeathHandlerSystem::handleDeathSpawn(
		Scene& scene,
		const EntityHandle& entityHandle
	) {
		auto& dataStorage{ scene.getDataStorage() };
		if (dataStorage.containsComponent<DeathSpawn>(entityHandle)) {
			DeathSpawn& deathSpawn{
				dataStorage.getComponent<DeathSpawn>(entityHandle)
			};
			//add a ghost with the death spawn program list and possibly position
			if (dataStorage.containsComponent<Position>(entityHandle)) {
				Position& position{
					dataStorage.getComponent<Position>(entityHandle)
				};
				auto ghostTuple{
					EntityBuilder::makeEntity(
						position, 
						deathSpawn.spawnProgramList,
						ScriptProgramList{ ghostProgram }
					)
				};
				dataStorage.addEntity(ghostTuple.package());
			}
			else {
				auto ghostTuple{ 
					EntityBuilder::makeEntity(
						deathSpawn.spawnProgramList,
						ScriptProgramList{ ghostProgram }
					)
				};
				dataStorage.addEntity(ghostTuple.package());
			}
		}
		dataStorage.removeEntity(entityHandle);
	}

	

	
}