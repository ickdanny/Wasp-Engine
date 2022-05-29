#include "Game/Topics.h"

namespace wasp::game {

	template <typename T = utility::Void>
	using Topic = channel::Topic<T>;

	//global topics
	const Topic<SceneNames> GlobalTopics::sceneEntry{};
	const Topic<SceneNames> GlobalTopics::sceneExitTo{};

	//scene topics
}