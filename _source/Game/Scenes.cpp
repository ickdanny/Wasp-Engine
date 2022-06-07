#include "Game/Scenes.h"

namespace wasp::game {

	using SceneStorage = scene::SceneStorage<SystemChainIDs, SceneNames>;

	SceneList makeSceneList() {
		SceneStorage sceneStorage{};

		std::vector<std::pair<SystemChainIDs, bool>> noTransparency{
			{ SystemChainIDs::update, false},
			{ SystemChainIDs::render, false}
		};

		std::vector<std::pair<SystemChainIDs, bool>> renderTransparency{
			{ SystemChainIDs::update, false},
			{ SystemChainIDs::render, true}
		};

		sceneStorage.makeScene(
			SceneNames::main,		//name
			20,						//initEntityCapacity
			20,						//initComponentCapacity
			noTransparency,			//transparency
			false					//refresh
		);

		sceneStorage.makeScene(
			SceneNames::options,	//name
			20,						//initEntityCapacity
			20,						//initComponentCapacity
			renderTransparency,		//transparency
			false					//refresh
		);

		return SceneList(std::move(sceneStorage));
	}
}