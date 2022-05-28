#pragma once

#include "Scene/SceneList.h"
#include "SystemChainIDs.h"

namespace wasp::game{

	enum class SceneNames {
		main,
		shot,
		difficulty,
		level,
		options,
		game,
		pause,
		continues,
		credits
	};

	scene::SceneList<SystemChainIDs, SceneNames> makeSceneList();
}