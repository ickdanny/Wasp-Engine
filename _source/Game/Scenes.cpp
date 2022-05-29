#include "Game/Scenes.h"

namespace wasp::game {

	using SceneStorage = scene::SceneStorage<SystemChainIDs, SceneNames>;

	SceneList makeSceneList() {
		SceneStorage sceneStorage{};

		std::vector<std::pair<SystemChainIDs, bool>> transparency{
			{ SystemChainIDs::update, false},
			{ SystemChainIDs::render, false}
		};

		sceneStorage.makeScene(
			SceneNames::main,	//name
			20,					//initEntityCapacity
			20,					//initComponentCapacity
			transparency,		//transparency
			false				//refresh
		);

		return SceneList(std::move(sceneStorage));
	}
}