#include "Game/Game.h"

namespace wasp::game {

	Game::Game(
		resources::ResourceMasterStorage& resourceMasterStorage,
		window::WindowPainter* windowPainterPointer
	)
		: sceneList{ std::move(makeSceneList()) }
		, sceneUpdater{ resourceMasterStorage }
		, sceneRenderer{ windowPainterPointer }
	{
		sceneList.pushScene(SceneNames::main);
	}

	void Game::update() {
		for (auto itr{ sceneList.rbegin() }; itr != sceneList.rend(); ++itr) {
			auto& scene{ *(*itr) };	//dereference itr and shared_ptr
			sceneUpdater(scene);
			if (!scene.isTransparent(SystemChainIDs::update)) {
				break;
			}
		}
		updateSceneList();
		//todo: reset input and shit
		//todo: global flags and music
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
		auto& scene{ *(*itr) };	//dereference itr and shared_ptr
		if (itr != sceneList.rend() && scene.isTransparent(SystemChainIDs::render)) {
			recursiveRenderHelper(deltaTime, itr + 1);
		}
		sceneRenderer(scene, deltaTime);
	}
}