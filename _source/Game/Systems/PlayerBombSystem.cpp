#include "Game/Systems/PlayerBombSystem.h"

#include "Logging.h"

namespace wasp::game::systems {

	void PlayerBombSystem::operator()(Scene& scene) {
		if (scene.hasChannel(SceneTopics::playerStateEntry)) {
			const auto& playerStateEntryChannel{
				scene.getChannel(SceneTopics::playerStateEntry)
			};
			for (const auto& [playerHandle, playerState]
				: playerStateEntryChannel.getMessages()
			) {
				if (playerState == PlayerStates::bombing) {
					bomb(scene, playerHandle);
				}
			}
		}
	}

	void PlayerBombSystem::bomb(Scene& scene, const EntityHandle& playerHandle) {
		auto& dataStorage{ scene.getDataStorage() };

		if (!dataStorage.containsComponent<PlayerData>(playerHandle)) {
			throw std::runtime_error{ "cannot find player data for bomb!" };
		}
		if (!dataStorage.containsComponent<SpawnData>(playerHandle)) {
			throw std::runtime_error{ "player has no spawn data at bomb!" };
		}

		auto& playerData{ dataStorage.getComponent<PlayerData>(playerHandle) };
		auto& spawnData{ dataStorage.getComponent<SpawnData>(playerHandle) };

		//todo: add the bomb spawn to the spawn data
		debug::log("spawn bomb");
		--playerData.bombs;
	}
}