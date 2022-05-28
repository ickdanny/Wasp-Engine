#pragma once

#include "Scenes.h"

namespace wasp::game {

	//represents the logical state of the game
	class Game {
	private:
		//typedefs
		using SceneList = scene::SceneList<SystemChainIDs, SceneNames>;

		//fields
		SceneList sceneList;	//not initialized!


	};
}