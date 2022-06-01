#pragma once

#include "Channel/Topic.h"
#include "Game/Components/MenuCommand.h"
#include "Game/Systems/MenuNavigationCommands.h"
#include "ECS/Entity/EntityHandle.h"

namespace wasp::game {

	//forward declarations
	enum class SceneNames;

	//typedefs
	template <typename T = utility::Void>
	using Topic = channel::Topic<T>;

	//global topics (1 channel per game)
	struct GlobalTopics {
		//handled by Game::updateSceneList
		static const Topic<SceneNames> sceneEntry;
		static const Topic<SceneNames> sceneExitTo;

		//handled by Game::updateMusic
		static const Topic<std::wstring> startMusic;
		static const Topic<> stopMusicFlag;

		//handled by Game::updateSettings
		static const Topic<> writeSettingsFlag;		//todo: not handled!
		static const Topic<> soundToggleFlag;
		static const Topic<> fullscreenToggleFlag;

		//handled by Game::updateSceneList
		static const Topic<> exitFlag;

		//todo: not handled!
		static const Topic<> gameOverFlag;
	};

	//scene topics (1 channel per scene)
	struct SceneTopics {
		//set and cleared by InputParserSystem
		static const Topic<systems::MenuNavigationCommands> menuNavigationCommands;

		//set by InitSystem; persistent
		static const Topic<components::MenuCommand> keyboardBackMenuCommand;

		//set and cleared by MenuNavigationSystem; initially by InitSystem
		static const Topic<ecs::entity::EntityHandle> currentSelectedElement;

		//set and cleared by MenuNavigationSystem
		//true = select, false = unselect
		static const Topic<std::tuple<ecs::entity::EntityHandle, bool>>
			elementSelection;

		//todo: use these for delta time shenanigans
		static const Topic<> pauseFlag;
		static const Topic<float> lastDeltaTime;
	};
}