#pragma once

#include "Resources/ResourceMasterStorage.h"
#include "Game/Scenes.h"
#include "Game/Systems/InitSystem.h"

namespace wasp::game {

	class UpdateSystemChain {
	private:
		systems::InitSystem initSystem;	//not initialized!

	public:
		UpdateSystemChain(
			resources::ResourceMasterStorage& resourceMasterStorage
		)
			: initSystem{ &resourceMasterStorage.bitmapStorage }
		{
		}

		void operator()(Scene& scene) {
			initSystem(scene);
		}
	};
}