#pragma once

#include "Resources/ResourceMasterStorage.h"
#include "Scenes.h"
#include "Topics.h"
#include "UpdateSystemChain.h"
#include "RenderSystemChain.h"

namespace wasp::game {

	//represents the logical state of the game
	class Game {
	private:
		//typedefs
		using ChannelSet = channel::ChannelSet;

		//fields
		SceneList sceneList;	//not initialized!
		ChannelSet globalChannelSet{};

		UpdateSystemChain updateSystemChain;	//not initialized!

	public:
		//constructor
		Game(resources::ResourceMasterStorage& resourceMasterStorage)
			: sceneList{ std::move(makeSceneList()) } 
			, updateSystemChain{ resourceMasterStorage }
		{
		}

		void update();

		void render(float deltaTime);

	private:
		void updateSceneList();

		void recursiveRenderHelper(
			float deltaTime, 
			const SceneList::ReverseIterator& itr
		);
	};
}