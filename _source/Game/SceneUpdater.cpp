#include "Game/SceneUpdater.h"

namespace wasp::game {
	SceneUpdater::SceneUpdater(
		resources::ResourceMasterStorage* resourceMasterStoragePointer,
		input::IKeyInputTable* keyInputTablePointer,
		channel::ChannelSet* globalChannelSetPointer
	)
		: initSystem{ &(resourceMasterStoragePointer->bitmapStorage) }
		, inputParserSystem{ keyInputTablePointer }
		, menuNavigationSystem{ globalChannelSetPointer }
		, buttonSpriteSystem{ &(resourceMasterStoragePointer->bitmapStorage) }
	{
	}

	void SceneUpdater::operator()(Scene& scene) {
		initSystem(scene);
		inputParserSystem(scene);
		menuNavigationSystem(scene);
		buttonSpriteSystem(scene);

		//todo: need a game builder system to build a gameConfig, also needs to
		//todo: send start_difficulty and practice_difficulty to the difficulty scene
		//todo: via replacing the message
	}
}