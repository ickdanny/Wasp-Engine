#pragma once

#include "Channel/Topic.h"

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
	};

	//scene topics (1 channel per scene)
	struct SceneTopics {
	public:

	};
}