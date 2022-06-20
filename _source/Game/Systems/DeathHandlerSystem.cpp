#include "Game/Systems/DeathHandlerSystem.h"

#include "Logging.h"

namespace wasp::game::systems {

	namespace {

		void handlePlayerDeath(
			Scene& scene,
			const ecs::entity::EntityHandle& playerHandle
		) {
			//todo: handle player death
			debug::log("player death");
		}

		void handleBossDeath(
			Scene& scene, 
			const ecs::entity::EntityHandle& bossHandle
		) {
			scene.getChannel(SceneTopics::bossDeaths).addMessage(bossHandle);
		}

		void handleDeathSpawn(
			Scene& scene,
			const ecs::entity::EntityHandle& entityHandle
		) {
			//todo: handle death spawn (ghost -> remove instruction?)
		}

		void handleDeath(
			Scene& scene, 
			const ecs::entity::EntityHandle& entityHandle,
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
}