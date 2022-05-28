#include "Game/Scenes.h"

namespace wasp::game {
	scene::SceneList<SystemChainIDs, SceneNames> makeSceneList() {
		using namespace scene;

		SceneStorage<SystemChainIDs, SceneNames> sceneStorage{};

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