#include "Game/Game.h"

namespace wasp::game {

	void Game::update() {
		for (auto itr{ sceneList.rbegin() }; itr != sceneList.rend(); ++itr) {
			auto& scene{ **itr };	//dereference itr and shared_ptr
			//todo: update on scene
			if (!scene.isTransparent(SystemChainIDs::update)) {
				break;
			}
		}
		updateSceneList();
	}

	void Game::updateSceneList() {
		//todo: read scene push and pop channels and update; call update on pushed
	}

	void Game::render(float deltaTime) {
		recursiveRenderHelper(deltaTime, sceneList.rbegin());
	}

	void Game::recursiveRenderHelper(
		float deltaTime,
		const SceneList::ReverseIterator& itr
	) {
		auto& scene{ **itr };	//dereference itr and shared_ptr
		if (itr != sceneList.rend() && scene.isTransparent(SystemChainIDs::render)) {
			recursiveRenderHelper(deltaTime, itr + 1);
		}
		//todo: render on scene
	}
}