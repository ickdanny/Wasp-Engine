#include "Game/Systems/GameBuilderSystem.h"

#include <chrono>

namespace wasp::game::systems {

	void GameBuilderSystem::operator()(Scene& scene) {
		auto& gameBuilderCommandChannel{
			scene.getChannel(SceneTopics::gameBuilderCommands)
		};
		if (gameBuilderCommandChannel.hasMessages()) {
			for (auto gameBuilderCommand : gameBuilderCommandChannel.getMessages()) {
				handleGameBuilderCommand(gameBuilderCommand);
			}
			gameBuilderCommandChannel.clear();
		}
	}

	void GameBuilderSystem::handleGameBuilderCommand(
		GameBuilderCommands gameBuilderCommand
	) {
		GameState* gameStatePointer{ retrieveOrInitGameStatePointer() };

		switch (gameBuilderCommand) {
			case GameBuilderCommands::start:
				gameStatePointer->gameMode = GameMode::campaign;
				break;
			case GameBuilderCommands::practice:
				gameStatePointer->gameMode = GameMode::practice;
				break;
			case GameBuilderCommands::easy:
				gameStatePointer->difficulty = Difficulty::easy;
				break;
			case GameBuilderCommands::normal:
				gameStatePointer->difficulty = Difficulty::normal;
				break;
			case GameBuilderCommands::hard:
				gameStatePointer->difficulty = Difficulty::hard;
				break;
			case GameBuilderCommands::lunatic:
				gameStatePointer->difficulty = Difficulty::lunatic;
				break;
			case GameBuilderCommands::shotA:
				gameStatePointer->shotType = ShotType::shotA;
				break;
			case GameBuilderCommands::shotB:
				gameStatePointer->shotType = ShotType::shotB;
				break;
			case GameBuilderCommands::stage1:
				gameStatePointer->stage = 1;
				break;
			case GameBuilderCommands::stage2:
				gameStatePointer->stage = 2;
				break;
			case GameBuilderCommands::stage3:
				gameStatePointer->stage = 3;
				break;
			case GameBuilderCommands::stage4:
				gameStatePointer->stage = 4;
				break;
			case GameBuilderCommands::stage5:
				gameStatePointer->stage = 5;
				break;
			case GameBuilderCommands::beginGame:
				finalizeGameState(gameStatePointer);
				break;
			case GameBuilderCommands::none:
				throw std::runtime_error{ "'none' in handleGameBuilderCommand" };
			default:
				throw std::runtime_error{
					"default case reached in handleGameBuilderCommand"
				};
		}
	}

	GameState* GameBuilderSystem::retrieveOrInitGameStatePointer() {
		auto& gameStateChannel{
			globalChannelSetPointer->getChannel(GlobalTopics::gameState)
		};

		if (!gameStateChannel.hasMessages()) {
			gameStateChannel.emplaceMessage(GameState{});
		}
		return &(gameStateChannel.getMessages().front());
	}

	void GameBuilderSystem::finalizeGameState(GameState* gameStatePointer) {
		if (gameStatePointer->gameMode == GameMode::campaign) {
			gameStatePointer->stage = 1;
		}
		gameStatePointer->setPrngSeedToClock();
	}
}