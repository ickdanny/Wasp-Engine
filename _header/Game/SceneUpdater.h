#pragma once

#include "Resources/ResourceMasterStorage.h"
#include "Input/IKeyInputTable.h"
#include "Game/Scenes.h"

#include "Game/Systems/SpawnPrograms/SpawnPrograms.h"

#include "Game/Systems/InitSystem.h"
#include "Game/Systems/MiscellaneousSystem.h"
#include "Game/Systems/InputParserSystem.h"
#include "Game/Systems/MenuNavigationSystem.h"
#include "Game/Systems/ButtonSpriteSystem.h"
#include "Game/Systems/GameBuilderSystem.h"
#include "Game/Systems/VelocitySystem.h"
#include "Game/Systems/ScriptSystem.h"
#include "Game/Systems/CollisionDetectorSystem.h"
#include "Game/Systems/PlayerMovementSystem.h"
#include "Game/Systems/PlayerShotSystem.h"
#include "Game/Systems/CollisionHandlerSystem.h"
#include "Game/Systems/PlayerStateSystem.h"
#include "Game/Systems/PlayerBombSystem.h"
#include "Game/Systems/PlayerDeathDetectorSystem.h"
#include "Game/Systems/ContinueSystem.h"
#include "Game/Systems/PlayerRespawnSystem.h"
#include "Game/Systems/PlayerReactivateSystem.h"
#include "Game/Systems/DeathHandlerSystem.h"
#include "Game/Systems/SpawnSystem.h"
#include "Game/Systems/OverlaySystem.h"
#include "Game/Systems/PauseSystem.h"

#include "Game/Systems/RotateSpriteForwardSystem.h"
#include "Game/Systems/SpriteSpinSystem.h"

#include "Game/Systems/InboundSystem.h"
#include "Game/Systems/OutboundSystem.h"
#include "Game/Systems/GameOverSystem.h"
#include "Game/Systems/WinSystem.h"

namespace wasp::game {

	class SceneUpdater {
	private:
		//fields
		systems::SpawnPrograms spawnPrograms;					//not initialized!

		//systems
		systems::InitSystem initSystem;							//not initialized!
		systems::MiscellaneousSystem miscellaneousSystem;
		systems::InputParserSystem inputParserSystem;			//not initialized!
		systems::MenuNavigationSystem menuNavigationSystem;		//not initialized!
		systems::ButtonSpriteSystem buttonSpriteSystem;			//not initialized!
		systems::GameBuilderSystem gameBuilderSystem;			//not initialized!
		systems::VelocitySystem velocitySystem{};
		systems::ScriptSystem scriptSystem{};
		systems::CollisionDetectorSystem collisionDetectorSystem{};
		systems::PlayerMovementSystem playerMovementSystem{};
		systems::PlayerShotSystem playerShotSystem;				//not initialized!
		systems::CollisionHandlerSystem collisionHandlerSystem{};
		systems::PlayerStateSystem playerStateSystem{};
		systems::PlayerBombSystem playerBombSystem;				//not initialized!
		systems::PlayerDeathDetectorSystem playerDeathDetectorSystem{};
		systems::ContinueSystem continueSystem;					//not initialized!
		systems::PlayerRespawnSystem playerRespawnSystem{};
		systems::PlayerReactivateSystem playerReactivateSystem{};
		systems::DeathHandlerSystem deathHandlerSystem{};
		systems::SpawnSystem spawnSystem{};
		systems::OverlaySystem overlaySystem;					//not initialized!
		systems::PauseSystem pauseSystem;						//not initialized!

		systems::RotateSpriteForwardSystem rotateSpriteForwardSystem{};
		systems::SpriteSpinSystem spriteSpinSystem{};

		systems::InboundSystem inboundSystem{};
		systems::OutboundSystem outboundSystem{};
		systems::GameOverSystem gameOverSystem;					//not initialized!
		systems::WinSystem winSystem;							//not initialized!

	public:
		SceneUpdater(
			resources::ResourceMasterStorage* resourceMasterStoragePointer,
			input::IKeyInputTable* keyInputTablePointer,
			channel::ChannelSet* globalChannelSetPointer
		);

		void operator()(Scene& scene);
	};
}