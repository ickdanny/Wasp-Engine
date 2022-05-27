#pragma once

#include "Scene/SceneList.h"
#include "SystemChains.h"

namespace wasp::game::scenes{

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

	scene::SceneList<systemchains::SystemChainIDs, SceneNames> makeSceneList() {
		using systemchains::SystemChainIDs;
		using namespace scene;

		SceneStorage<systemchains::SystemChainIDs, SceneNames> sceneStorage{};

		std::vector<std::pair<SystemChainIDs, bool>> transparency{
			{ SystemChainIDs::update, false},
			{ SystemChainIDs::graphics, false}
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