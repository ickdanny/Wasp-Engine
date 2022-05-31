#pragma once

#include "Resources/ResourceMasterStorage.h"
#include "Scenes.h"
#include "Topics.h"
#include "SceneUpdater.h"
#include "SceneRenderer.h"

namespace wasp::game {

	//represents the logical state of the game
	class Game {
	private:
		//typedefs
		using ChannelSet = channel::ChannelSet;

		//fields
		SceneList sceneList;	//not initialized!
		ChannelSet globalChannelSet{};

		SceneUpdater sceneUpdater;	//not initialized!
		SceneRenderer sceneRenderer;	//not initialized!
		
	public:
		//constructor
		Game(
			resources::ResourceMasterStorage& resourceMasterStorage,
			window::WindowPainter* windowPainterPointer
		);

		void update();

		void render(float deltaTime);

		void setExitCallback(const std::function<void()>& exitCallback) {
			sceneUpdater.setExitCallback(exitCallback);
		}

	private:
		void updateSceneList();

		void recursiveRenderHelper(
			float deltaTime, 
			const SceneList::ReverseIterator& itr
		);
	};
}