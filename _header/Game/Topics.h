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
		static const Topic<SceneNames> sceneEntry;
		static const Topic<SceneNames> sceneExitTo;
		static const Topic<std::wstring> startMusic;
		static const Topic<> stopMusicFlag;
		static const Topic<> writeSettingsFlag;
		static const Topic<> soundToggleFlag;
		static const Topic<> fullscreenToggleFlag;
		static const Topic<> exitFlag;
		static const Topic<> gameOverFlag;
	};

	//scene topics (1 channel per scene)
	struct SceneTopics {
		static const Topic<systems::MenuNavigationCommands> menuNavigationCommands;
		static const Topic<components::MenuCommand> keyboardBackMenuCommand;
		static const Topic<ecs::entity::EntityHandle> currentSelectedElement;
		static const Topic<std::tuple<ecs::entity::EntityHandle, bool>> 
			elementSelection;	//true = select, false = unselect

		//todo: use these for delta time shenanigans
		static const Topic<> pauseFlag;
		static const Topic<float> lastDeltaTime;
	};
}