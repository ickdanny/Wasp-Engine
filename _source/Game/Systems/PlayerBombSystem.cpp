#include "Game/Systems/PlayerBombSystem.h"

#include "Game/Systems/SpawnPrograms/PlayerSpawnPrograms.h"
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
		if (!dataStorage.containsComponent<SpawnProgramList>(playerHandle)) {
			throw std::runtime_error{ "player has no spawn data at bomb!" };
		}

		auto& playerData{ dataStorage.getComponent<PlayerData>(playerHandle) };
		auto& spawnProgramList{ 
			dataStorage.getComponent<SpawnProgramList>(playerHandle) 
		};
		if (playerData.shotType == ShotType::shotA) {
			spawnProgramList.push_back(
				{ spawnProgramsPointer->playerSpawnPrograms.bombA }
			);
		}
		else if (playerData.shotType == ShotType::shotB) {
			spawnProgramList.push_back(
				{ spawnProgramsPointer->playerSpawnPrograms.bombB }
			);
		}
		else {
			throw std::runtime_error("unexpected player shot type!");
		}
		debug::log("spawn bomb");
		--playerData.bombs;
	}
}