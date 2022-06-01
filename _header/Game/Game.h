#pragma once

#include "Resources/ResourceMasterStorage.h"
#include "Scenes.h"
#include "Topics.h"
#include "SceneUpdater.h"
#include "SceneRenderer.h"
#include "Sound/MidiHub.h"

namespace wasp::game {

	//represents the logical state of the game
	class Game {
	private:
		//typedefs
		using ChannelSet = channel::ChannelSet;

		//fields
		SceneList sceneList;			//not initialized!
		ChannelSet globalChannelSet{};

		SceneUpdater sceneUpdater;		//not initialized!
		SceneRenderer sceneRenderer;	//not initialized!

		resources::ResourceMasterStorage* resourceMasterStoragePointer{};
		input::IKeyInputTable* keyInputTablePointer{};
		sound::midi::MidiHub* midiHubPointer{};

		std::function<void()> exitCallback{};
		
	public:
		//constructor
		Game(
			resources::ResourceMasterStorage* resourceMasterStoragePointer,
			window::WindowPainter* windowPainterPointer,
			input::IKeyInputTable* keyInputTablePointer,
			sound::midi::MidiHub* midiHubPointer
		);

		void update();

		void render(float deltaTime);

		void setExitCallback(const std::function<void()>& exitCallback) {
			this->exitCallback = exitCallback;
		}

	private:
		bool wasExitFlagRaised();
		void updateSceneList();
		void updateInput();
		void updateMusic();
		void updateSettings();

		void recursiveRenderHelper(
			float deltaTime, 
			const SceneList::ReverseIterator& itr
		);
	};
}