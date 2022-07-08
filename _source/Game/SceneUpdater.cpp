#include "Game/SceneUpdater.h"

#include "Logging.h"

namespace wasp::game {
	SceneUpdater::SceneUpdater(
		resources::ResourceMasterStorage* resourceMasterStoragePointer,
		input::IKeyInputTable* keyInputTablePointer,
		channel::ChannelSet* globalChannelSetPointer
	)
		: spawnPrograms{ &(resourceMasterStoragePointer->bitmapStorage) }

		, initSystem{ 
			globalChannelSetPointer, 
			&(resourceMasterStoragePointer->bitmapStorage),
			&spawnPrograms
		}
		, inputParserSystem{ keyInputTablePointer }
		, menuNavigationSystem{ globalChannelSetPointer }
		, buttonSpriteSystem{ &(resourceMasterStoragePointer->bitmapStorage) }
		, gameBuilderSystem { globalChannelSetPointer }
		, playerShotSystem{ &spawnPrograms }
		, playerBombSystem{ &spawnPrograms }
		, continueSystem{ globalChannelSetPointer }

		, overlaySystem{ &(resourceMasterStoragePointer->bitmapStorage) }
		, pauseSystem{ globalChannelSetPointer }

		, gameOverSystem{ globalChannelSetPointer }
		, winSystem{ globalChannelSetPointer } {
	}

	void SceneUpdater::operator()(Scene& scene) {
		initSystem(scene);
		miscellaneousSystem(scene);
		inputParserSystem(scene);
		menuNavigationSystem(scene);
		buttonSpriteSystem(scene);
		gameBuilderSystem(scene);
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

        //gameDialogueScreenEntrySystem = new DialogueScreenEntrySystem();
        //gameNextStageEntrySystem = new NextStageEntrySystem();
        //
        //gameAnimationSystem = new AnimationSystem(GAME_COMPONENT_TYPES);

		rotateSpriteForwardSystem(scene);
		spriteSpinSystem(scene);

        //gameSinusoidalSpriteVerticalOffsetSystem = new SinusoidalSpriteVerticalOffsetSystem(GAME_COMPONENT_TYPES);
        //gameScrollingSubImageSystem = new ScrollingSubImageSystem(GAME_COMPONENT_TYPES);
        //gameSpriteSubImageFlagUpdateSystem = new SpriteSubImageFlagUpdateSystem(GAME_COMPONENT_TYPES);
        //gameSpriteInstructionSystem = new SpriteInstructionSystem(imageManager, GAME_COMPONENT_TYPES);
        //gameSpriteRemovalSystem = new SpriteRemovalSystem(GAME_COMPONENT_TYPES);

        inboundSystem(scene);
        outboundSystem(scene);
		gameOverSystem(scene);
		winSystem(scene);
	}
}