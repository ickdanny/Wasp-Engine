#include "Game/SceneUpdater.h"

#include "Logging.h"

namespace wasp::game {
	SceneUpdater::SceneUpdater(
		resources::ResourceMasterStorage* resourceMasterStoragePointer,
		input::IKeyInputTable* keyInputTablePointer,
		channel::ChannelSet* globalChannelSetPointer
	)
		: programs{ &(resourceMasterStoragePointer->bitmapStorage) }

		, initSystem{ 
			globalChannelSetPointer, 
			&(resourceMasterStoragePointer->bitmapStorage),
			&programs
		}
		, inputParserSystem{ keyInputTablePointer }
		, menuNavigationSystem{ globalChannelSetPointer }
		, buttonSpriteSystem{ &(resourceMasterStoragePointer->bitmapStorage) }
		, gameBuilderSystem { globalChannelSetPointer }
		, loadSystem{ globalChannelSetPointer }
		, dialogueSystem{ 
			globalChannelSetPointer,
			&(resourceMasterStoragePointer->bitmapStorage),
			&(resourceMasterStoragePointer->dialogueStorage)
		}
		, scriptSystem{ globalChannelSetPointer }
		, playerShotSystem{ &programs }
		, collisionHandlerSystem{ &programs }
		, playerBombSystem{ &programs }
		, continueSystem{ globalChannelSetPointer }
		, spawnSystem{ globalChannelSetPointer }
		, overlaySystem{ &(resourceMasterStoragePointer->bitmapStorage) }
		, pauseSystem{ globalChannelSetPointer }
		, animationSystem{ &(resourceMasterStoragePointer->bitmapStorage) }
		, gameOverSystem{ globalChannelSetPointer }
		, creditsSystem{ globalChannelSetPointer } {
	}

	void SceneUpdater::operator()(Scene& scene) {
		initSystem(scene);
		miscellaneousSystem(scene);
		inputParserSystem(scene);
		menuNavigationSystem(scene);
		buttonSpriteSystem(scene);
		gameBuilderSystem(scene);
		loadSystem(scene);
		dialogueSystem(scene);
		velocitySystem(scene);
		scriptSystem(scene);
		collisionDetectorSystem(scene);
		playerMovementSystem(scene);
		playerShotSystem(scene);
		collisionHandlerSystem(scene);
		playerStateSystem(scene);
		playerBombSystem(scene);
		playerDeathDetectorSystem(scene);
		continueSystem(scene);
		playerRespawnSystem(scene);
		playerReactivateSystem(scene);
		deathHandlerSystem(scene);
		spawnSystem(scene);
		overlaySystem(scene);
		pauseSystem(scene);
		animationSystem(scene);
		rotateSpriteForwardSystem(scene);
		spriteSpinSystem(scene);
		subImageScrollSystem(scene);
        inboundSystem(scene);
        outboundSystem(scene);
		gameOverSystem(scene);
		creditsSystem(scene);
	}
}