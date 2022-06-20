#include "Game/Systems/PlayerReactivateSystem.h"

#include "Logging.h"

namespace wasp::game::systems {

	void PlayerReactivateSystem::operator()(Scene& scene) {
		if (scene.hasChannel(SceneTopics::playerStateEntry)) {
			const auto& playerStateEntryChannel{
				scene.getChannel(SceneTopics::playerStateEntry)
			};
			for (const auto& [playerHandle, playerState]
				: playerStateEntryChannel.getMessages()
				) {
				if (playerState == PlayerStates::respawnInvulnerable) {
					reactivate(scene, playerHandle);
				}
			}
		}
	}

	void PlayerReactivateSystem::reactivate(
		Scene& scene, 
		const EntityHandle& playerHandle
	) {
		//reactivate player by adding a spawn program list
		auto& dataStorage{ scene.getDataStorage() };
		dataStorage.addComponent<SpawnProgramList>({ playerHandle, {} });
	}
}