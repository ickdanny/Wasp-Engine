#include "Game/Systems/MiscellaneousSystem.h"

namespace wasp::game::systems {

	namespace {
		void resetLastDeltaTime(Scene& scene) {
			auto& lastDeltaTimeChannel{ 
				scene.getChannel(SceneTopics::lastDeltaTime) 
			};
			lastDeltaTimeChannel.clear();
			lastDeltaTimeChannel.addMessage(0.0f);
		}

		//clears death channel
		void clearChannels(Scene& scene) {
			scene.getChannel(SceneTopics::deaths).clear();
		}
	}

	void MiscellaneousSystem::operator()(Scene& scene) {
		resetLastDeltaTime(scene);
		clearChannels(scene);
	}
}