#include "Game/SceneUpdater.h"

namespace wasp::game {
	SceneUpdater::SceneUpdater(
		resources::ResourceMasterStorage* resourceMasterStoragePointer,
		input::IKeyInputTable* keyInputTablePointer,
		channel::ChannelSet* globalChannelSetPointer
	)
		: initSystem{ 
			globalChannelSetPointer, 
			&(resourceMasterStoragePointer->bitmapStorage) 
		}
		, inputParserSystem{ keyInputTablePointer }
		, menuNavigationSystem{ globalChannelSetPointer }
		, buttonSpriteSystem{ &(resourceMasterStoragePointer->bitmapStorage) }
		, gameBuilderSystem { globalChannelSetPointer }
	{
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
		collisionSystem(scene);
		playerMovementSystem(scene);
		playerShotSystem(scene);

		
		//gameDamageParserSystem = new DamageParserSystem(GAME_COMPONENT_TYPES);
        //gameDamageHandlingSystem = new DamageHandlingSystem(GAME_COMPONENT_TYPES);
        //gamePlayerStateSystem = new PlayerStateSystem();
        //gamePlayerBombSystem = new PlayerBombSystem(GAME_COMPONENT_TYPES);
        //gamePlayerDeathParserSystem = new PlayerDeathParserSystem();
        //gameContinueAndGameOverSystem = new ContinueAndGameOverSystem();
        //gamePlayerRespawnSystem = new PlayerRespawnSystem(GAME_COMPONENT_TYPES);
        //gamePlayerReactivateSystem = new PlayerReactivateSystem(GAME_COMPONENT_TYPES);
        //gameDeathHandlingSystem = new DeathHandlingSystem(GAME_COMPONENT_TYPES);
        //gameSpawnSystem = new SpawnSystem(GAME_COMPONENT_TYPES);
        //
        //gamePlayerUISystem = new PlayerUISystem(imageManager, GAME_COMPONENT_TYPES);
        //
        //gamePauseMenuEntrySystem = new PauseMenuEntrySystem();
        //gameDialogueScreenEntrySystem = new DialogueScreenEntrySystem();
        //gameNextStageEntrySystem = new NextStageEntrySystem();
        //
        //gameAnimationSystem = new AnimationSystem(GAME_COMPONENT_TYPES);
        //gameRotateSpriteForwardSystem = new RotateSpriteForwardSystem(GAME_COMPONENT_TYPES);
        //gameConstantSpriteRotationSystem = new ConstantSpriteRotationSystem(GAME_COMPONENT_TYPES);
        //gameSinusoidalSpriteVerticalOffsetSystem = new SinusoidalSpriteVerticalOffsetSystem(GAME_COMPONENT_TYPES);
        //gameScrollingSubImageSystem = new ScrollingSubImageSystem(GAME_COMPONENT_TYPES);
        //gameSpriteSubImageFlagUpdateSystem = new SpriteSubImageFlagUpdateSystem(GAME_COMPONENT_TYPES);
        //gameSpriteInstructionSystem = new SpriteInstructionSystem(imageManager, GAME_COMPONENT_TYPES);
        //gameSpriteRemovalSystem = new SpriteRemovalSystem(GAME_COMPONENT_TYPES);

        inboundSystem(scene);
        outboundSystem(scene);

        //gameGameOverSystem = new GameOverSystem();
        //gameWinSystem = new GameWinSystem();
	}
}