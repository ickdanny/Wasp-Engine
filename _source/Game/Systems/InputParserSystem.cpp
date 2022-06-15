#include "Game/Systems/InputParserSystem.h"

namespace wasp::game::systems {

	void InputParserSystem::operator()(Scene& scene) {
		switch (scene.getName()){
			case SceneNames::main:
			case SceneNames::difficulty:
			case SceneNames::shot:
			case SceneNames::stage:
			case SceneNames::music:
			case SceneNames::options:
			case SceneNames::dialogue:
			case SceneNames::pause:
			case SceneNames::continues:
				parseMenuInput(scene);
				break;
			case SceneNames::game:
				parseGameInput(scene);
				break;
			case SceneNames::credits:
				return;
			default:
				throw std::runtime_error{ "default case reached in InputParserSystem" };
		}
	}

	void InputParserSystem::parseMenuInput(Scene& scene) {
		auto& menuNavigationCommandChannel{ 
			scene.getChannel(SceneTopics::menuNavigationCommands) 
		};
		menuNavigationCommandChannel.clear();

		if (isJustPressed(KeyValues::k_escape) || isJustPressed(KeyValues::k_x)) {
			menuNavigationCommandChannel.addMessage(MenuNavigationCommands::back);
		}
		if (isJustPressed(KeyValues::k_z)) {
			menuNavigationCommandChannel.addMessage(MenuNavigationCommands::select);
		}
		if (isJustPressed(KeyValues::k_up)) {
			menuNavigationCommandChannel.addMessage(MenuNavigationCommands::up);
		}
		if (isJustPressed(KeyValues::k_down)) {
			menuNavigationCommandChannel.addMessage(MenuNavigationCommands::down);
		}
		if (isJustPressed(KeyValues::k_left)) {
			menuNavigationCommandChannel.addMessage(MenuNavigationCommands::left);
		}
		if (isJustPressed(KeyValues::k_right)) {
			menuNavigationCommandChannel.addMessage(MenuNavigationCommands::right);
		}

		keyInputTablePointer->lockAll();	//menues have no input transparency
	}

	void InputParserSystem::parseGameInput(Scene& scene) {
		auto& gameCommandChannel{
			scene.getChannel(SceneTopics::gameCommands)
		};
		gameCommandChannel.clear();

		if (isJustPressed(KeyValues::k_escape)) {
			gameCommandChannel.addMessage(GameCommands::pause);
		}
		if (isBeingPressed(KeyValues::k_shift)) {
			gameCommandChannel.addMessage(GameCommands::focus);
		}
		if (isBeingPressed(KeyValues::k_z)) {
			gameCommandChannel.addMessage(GameCommands::shoot);
		}
		if (isJustPressed(KeyValues::k_x)) {
			gameCommandChannel.addMessage(GameCommands::bomb);
		}
		if (isBeingPressed(KeyValues::k_up)) {
			gameCommandChannel.addMessage(GameCommands::up);
		}
		if (isBeingPressed(KeyValues::k_down)) {
			gameCommandChannel.addMessage(GameCommands::down);
		}
		if (isBeingPressed(KeyValues::k_left)) {
			gameCommandChannel.addMessage(GameCommands::left);
		}
		if (isBeingPressed(KeyValues::k_right)) {
			gameCommandChannel.addMessage(GameCommands::right);
		}

		keyInputTablePointer->lockAll();	//game has no input transparency
	}

	bool InputParserSystem::isJustPressed(KeyValues key) {
		if (keyInputTablePointer->isNotLocked(key)) {
			return keyInputTablePointer->get(key) == KeyState::Press;
		}
		return false;
	}

	bool InputParserSystem::isBeingPressed(KeyValues key) {
		if (keyInputTablePointer->isNotLocked(key)) {
			KeyState keyState{ keyInputTablePointer->get(key) };
			return keyState == KeyState::Press || keyState == KeyState::Down;
		}
		return false;
	}
}