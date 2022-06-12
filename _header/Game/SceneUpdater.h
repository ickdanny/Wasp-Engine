#pragma once

#include "Resources/ResourceMasterStorage.h"
#include "Input/IKeyInputTable.h"
#include "Game/Scenes.h"
#include "Game/Systems/InitSystem.h"
#include "Game/Systems/InputParserSystem.h"
#include "Game/Systems/MenuNavigationSystem.h"
#include "Game/Systems/ButtonSpriteSystem.h"
#include "Game/Systems/GameBuilderSystem.h"
#include "Game/Systems/VelocitySystem.h"
#include "Game/Systems/ScriptSystem.h"
#include "Game/Systems/CollisionSystem.h"
#include "Game/Systems/PlayerMovementSystem.h"
#include "Game/Systems/PlayerShotSystem.h"

#include "Game/Systems/InboundSystem.h"

namespace wasp::game {

	class SceneUpdater {
	private:
		systems::InitSystem initSystem;							//not initialized!
		systems::InputParserSystem inputParserSystem;			//not initialized!
		systems::MenuNavigationSystem menuNavigationSystem;		//not initialized!
		systems::ButtonSpriteSystem buttonSpriteSystem;			//not initialized!
		systems::GameBuilderSystem gameBuilderSystem;			//not initialized!
		systems::VelocitySystem velocitySystem{};
		systems::ScriptSystem scriptSystem{};
		systems::CollisionSystem<int, int> collisionSystem{};	//todo: correct template
		systems::PlayerMovementSystem playerMovementSystem{};
		systems::PlayerShotSystem playerShotSystem{};

		systems::InboundSystem inboundSystem{};

	public:
		SceneUpdater(
			resources::ResourceMasterStorage* resourceMasterStoragePointer,
			input::IKeyInputTable* keyInputTablePointer,
			channel::ChannelSet* globalChannelSetPointer
		);

		void operator()(Scene& scene);
	};
}