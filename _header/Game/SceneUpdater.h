#pragma once

#include "Resources/ResourceMasterStorage.h"
#include "Input/IKeyInputTable.h"
#include "Game/Scenes.h"
#include "Game/Systems/InitSystem.h"
#include "Game/Systems/InputParserSystem.h"
#include "Game/Systems/MenuNavigationSystem.h"
#include "Game/Systems/ButtonSpriteSystem.h"

namespace wasp::game {

	class SceneUpdater {
	private:
		systems::InitSystem initSystem;							//not initialized!
		systems::InputParserSystem inputParserSystem;			//not initialized!
		systems::MenuNavigationSystem menuNavigationSystem;		//not initialized!
		systems::ButtonSpriteSystem buttonSpriteSystem;			//not initialized!

	public:
		SceneUpdater(
			resources::ResourceMasterStorage* resourceMasterStoragePointer,
			input::IKeyInputTable* keyInputTablePointer,
			channel::ChannelSet* globalChannelSetPointer
		);

		void operator()(Scene& scene);
	};
}