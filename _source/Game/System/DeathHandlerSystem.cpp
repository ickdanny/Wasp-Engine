#include "Game/Systems/DeathHandlerSystem.h"

namespace wasp::game::systems {

	namespace {
		void handleBossDeath(
			Scene& scene, 
			const ecs::entity::EntityHandle& entityHandle
		) {
			scene.getChannel(SceneTopics::bossDeaths).addMessage(entityHandle);
		}

		void handleDeath(
			Scene& scene, 
			const ecs::entity::EntityHandle& entityHandle,
			const DeathCommand::Commands command
		) {
			switch (command) {
				case DeathCommand::Commands::playerDeath:
					//todo: handle player death
					break;
				case DeathCommand::Commands::pickupDeath:
					//todo: handle pickup death
					break;
				case DeathCommand::Commands::bossDeath:
					handleBossDeath(scene, entityHandle);
					break;
				case DeathCommand::Commands::deathSpawn:
					//todo: handle death spawn
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