#include "Game/Systems/PlayerShotSystem.h"

#include "Game/Systems/SpawnPrograms/PlayerSpawnPrograms.h"

namespace wasp::game::systems {

	void PlayerShotSystem::operator()(Scene& scene) {

		//this system only works if there are game commands
		if (scene.hasChannel(SceneTopics::gameCommands)) {
			auto& gameCommandChannel{ scene.getChannel(SceneTopics::gameCommands) };
			if (gameCommandChannel.hasMessages()) {

				//if there is a shoot command, add player shot
				for (auto& gameCommand : gameCommandChannel.getMessages()) {
					if (gameCommand == GameCommands::shoot) {
						addPlayerShot(scene);
						break;
					}
				}
			}
		}
	}

	void PlayerShotSystem::addPlayerShot(Scene& scene) {
		//get the iterator for players
		static const Topic<ecs::component::Group*> groupPointerStorageTopic{};
		auto groupPointer{
			getGroupPointer<PlayerData, Position, SpawnProgramList>(
				scene,
				groupPointerStorageTopic
			)
		};
		auto groupIterator{ 
			groupPointer->groupIterator<SpawnProgramList>() 
		};
		while (groupIterator.isValid()) {
			auto [spawnProgramList] = *groupIterator;

			//check to see if the player already has a shot spawn program by
			//checking each active spawn program's max tick
			bool isPlayerAlreadyShooting{ false };
			for (const auto& spawnProgram : spawnProgramList) {
				if (spawnProgram.getMaxTick() == config::playerShotMaxTick) {
					isPlayerAlreadyShooting = true;
					break;
				}
			}

			//if there is no pre-existing shot program, add one
			if (!isPlayerAlreadyShooting) {
				spawnProgramList.push_back(
					{ spawnProgramsPointer->playerSpawnPrograms.shotA }
				);
			}
			++groupIterator;
		}
	}
}