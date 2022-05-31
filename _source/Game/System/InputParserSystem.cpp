#include "Game/Systems/InputParserSystem.h"

namespace wasp::game::systems {

	void InputParserSystem::operator()(Scene& scene) {
		switch (scene.getName()){
			case SceneNames::main:
			case SceneNames::difficulty:
			case SceneNames::start_difficulty:
			case SceneNames::practice_difficulty:
			case SceneNames::shot:
			case SceneNames::level:
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
		auto& channel{ scene.getChannel(SceneTopics::menuNavigationCommands) };
		if (isJustPressed(KeyValues::K_escape) || isJustPressed(KeyValues::K_X)) {
			channel.addMessage(MenuNavigationCommands::back);
		}
		if (isJustPressed(KeyValues::K_Z)) {
			channel.addMessage(MenuNavigationCommands::select);
		}
		if (isJustPressed(KeyValues::K_up)) {
			channel.addMessage(MenuNavigationCommands::up);
		}
		if (isJustPressed(KeyValues::K_down)) {
			channel.addMessage(MenuNavigationCommands::down);
		}
		if (isJustPressed(KeyValues::K_left)) {
			channel.addMessage(MenuNavigationCommands::left);
		}
		if (isJustPressed(KeyValues::K_right)) {
			channel.addMessage(MenuNavigationCommands::right);
		}
		keyInputTablePointer->lockAll();	//menues have no input transparency
	}

	void InputParserSystem::parseGameInput(Scene& scene) {
		//todo: parse game input
	}

	bool InputParserSystem::isJustPressed(KeyValues key) {
		if (keyInputTablePointer->isNotLocked(key)) {
			return keyInputTablePointer->get(key) == KeyState::Press;
		}
		return false;
	}
}