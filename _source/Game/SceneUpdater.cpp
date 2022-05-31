#include "Game/GameUpdater.h"

namespace wasp::game {
	SceneUpdater::SceneUpdater(
		resources::ResourceMasterStorage& resourceMasterStorage
	)
		: initSystem{ &resourceMasterStorage.bitmapStorage }
	{
	}

	void SceneUpdater::operator()(Scene& scene) {
		initSystem(scene);
	}
}