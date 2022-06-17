#pragma once

#include "Channel/Topic.h"
#include "Game/Components/MenuCommand.h"
#include "Game/Systems/MenuNavigationCommands.h"
#include "ECS/Entity/EntityHandle.h"
#include "Game/Systems/GameState.h"
#include "Game/Systems/GameCommands.h"
#include "Game/Systems/PlayerStates.h"

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
		static const Topic<> writeSettingsFlag;
		static const Topic<> toggleSoundFlag;
		static const Topic<> toggleFullscreenFlag;

		//handled by Game::updateSceneList
		static const Topic<> exitFlag;

		//set by GameBuilderSystem; persistent
		static const Topic<systems::GameState> gameState;	
	};

	//scene topics (1 channel per scene)
	struct SceneTopics {
	private:
		using EntityHandle = ecs::entity::EntityHandle;

	public:
		//set and cleared by InputParserSystem
		static const Topic<systems::MenuNavigationCommands> menuNavigationCommands;

		//set by InitSystem; persistent
		static const Topic<components::MenuCommand> keyboardBackMenuCommand;

		//set and cleared by MenuNavigationSystem; init by InitSystem
		static const Topic<EntityHandle> currentSelectedElement;

		//set and cleared by MenuNavigationSystem
		//true = select, false = unselect
		static const Topic<std::tuple<EntityHandle, bool>>
			elementSelection;

		//set by MenuNavigationSystem, cleared by GameBuilderSystem
		//todo: when we restart campaign, need to set stage back to 1 and prng to clock
		//todo: have a GameBuilderCommand for reset
		static const Topic<systems::GameBuilderCommands> gameBuilderCommands;

		//set and cleared by InputParserSystem
		static const Topic<systems::GameCommands> gameCommands;

		//set and cleared by PlayerStateSystem
		static const Topic<std::tuple<EntityHandle, systems::PlayerStates>> 
			playerStateEntry;

		//set and cleared by CollisionHandlerSystem
		static const Topic<EntityHandle> playerHits;

		//set by CollisionHandlerSystem and PlayerDeathDetectorSystem,
		//cleared by MiscellaneousSystem
		static const Topic<EntityHandle> deaths;

		//todo: not cleared!
		//set by DeathHandlerSystem
		static const Topic<EntityHandle> bossDeaths;

		//set by ContinueSystem and PauseSystem, cleared by MiscellaneousSystem
		static const Topic<> pauseFlag;

		//set and cleared by RenderSystem and MiscellaneousSystem
		static const Topic<float> lastDeltaTime;
	};
}