#include "Game/Topics.h"

namespace wasp::game {

	template <typename T = utility::Void>
	using Topic = channel::Topic<T>;

	//global topics
	const Topic<SceneNames> GlobalTopics::sceneEntry{};
	const Topic<SceneNames> GlobalTopics::sceneExitTo{};
	const Topic<std::wstring> GlobalTopics::startMusic{};
	const Topic<> GlobalTopics::stopMusicFlag{};
	const Topic<> GlobalTopics::writeSettingsFlag{};
	const Topic<> GlobalTopics::toggleSoundFlag{};
	const Topic<> GlobalTopics::toggleFullscreenFlag{};
	const Topic<> GlobalTopics::exitFlag{};
	const Topic<systems::GameState> GlobalTopics::gameState{};
	const Topic<> GlobalTopics::gameOverFlag{};

	//scene topics
	const Topic<systems::MenuNavigationCommands> 
		SceneTopics::menuNavigationCommands{};
	const Topic<components::MenuCommand> SceneTopics::keyboardBackMenuCommand{};
	const Topic<ecs::entity::EntityHandle> SceneTopics::currentSelectedElement{};
	const Topic<std::tuple<ecs::entity::EntityHandle, bool>>
		SceneTopics::elementSelection{};
	const Topic<systems::GameBuilderCommands> SceneTopics::gameBuilderCommands{};
	const Topic<systems::GameCommands> SceneTopics::gameCommands{};
	const Topic<std::tuple<ecs::entity::EntityHandle, systems::PlayerStates>> 
		SceneTopics::playerStateEntry{};
	const Topic<ecs::entity::EntityHandle> SceneTopics::playerHits{};
	const Topic<ecs::entity::EntityHandle> SceneTopics::deaths{};
	const Topic<ecs::entity::EntityHandle> SceneTopics::bossDeaths{};
	const Topic<> SceneTopics::pauseFlag{};
	const Topic<float> SceneTopics::lastDeltaTime{};
}