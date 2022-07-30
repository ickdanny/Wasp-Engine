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
		, winSystem{ globalChannelSetPointer } {
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

        //gameDialogueScreenEntrySystem = new DialogueScreenEntrySystem();
        //gameNextStageEntrySystem = new NextStageEntrySystem();
        
		animationSystem(scene);
		rotateSpriteForwardSystem(scene);
		spriteSpinSystem(scene);

        //gameSinusoidalSpriteVerticalOffsetSystem = new SinusoidalSpriteVerticalOffsetSystem(GAME_COMPONENT_TYPES);

		subImageScrollSystem(scene);

        //gameSpriteSubImageFlagUpdateSystem = new SpriteSubImageFlagUpdateSystem(GAME_COMPONENT_TYPES);
        //gameSpriteInstructionSystem = new SpriteInstructionSystem(imageManager, GAME_COMPONENT_TYPES);
        //gameSpriteRemovalSystem = new SpriteRemovalSystem(GAME_COMPONENT_TYPES);

        inboundSystem(scene);
        outboundSystem(scene);
		gameOverSystem(scene);
		winSystem(scene);//todo: should win handle next stage and credits?
	}
}