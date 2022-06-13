#include "Game/Systems/SceneStateSystem.h"

namespace wasp::game::systems {

	namespace {
		void resetLastDeltaTime(Scene& scene) {
			auto& lastDeltaTimeChannel{ 
				scene.getChannel(SceneTopics::lastDeltaTime) 
			};
			lastDeltaTimeChannel.clear();
			lastDeltaTimeChannel.addMessage(0.0f);
		}
	}

	void SceneStateSystem::operator()(Scene& scene) {
		resetLastDeltaTime(scene);
	}
}