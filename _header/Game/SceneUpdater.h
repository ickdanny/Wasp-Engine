#pragma once

#include "Resources/ResourceMasterStorage.h"
#include "Game/Scenes.h"
#include "Game/Systems/InitSystem.h"

namespace wasp::game {

	class SceneUpdater {
	private:
		systems::InitSystem initSystem;	//not initialized!
		std::function<void()> exitCallback{};

	public:
		SceneUpdater(
			resources::ResourceMasterStorage& resourceMasterStorage
		);

		void setExitCallback(const std::function<void()>& exitCallback) {
			this->exitCallback = exitCallback;
		}

		void operator()(Scene& scene);
	};
}