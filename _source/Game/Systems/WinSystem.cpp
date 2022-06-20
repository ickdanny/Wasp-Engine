#include "Game/Systems/WinSystem.h"

namespace wasp::game::systems {

	namespace {
		constexpr int winDelay{ 100 };

		
	}

	void WinSystem::operator()(Scene& scene) {
		auto& dataStorage{ scene.getDataStorage() };

		//if there is already a win timer, handle that
		static const Topic<utility::Ticker> timerTopic{};
		auto& timerChannel{ scene.getChannel(timerTopic) };
		if (timerChannel.hasMessages()) {
			auto& timerVector{ timerChannel.getMessages() };
			for (auto itr{ timerVector.begin() }; itr != timerVector.end();) {
				auto& timer{ *itr };
				//if our timer has hit 0, enter the credits screen
				if (timer.stepAndGetTick() <= 0) {
					enterCreditsScreen();
					timerChannel.clear();
					break;
				}
				++itr;
			}
		}

		//otherwise, check to see if we need to begin a win timer
		else {
			if (scene.hasChannel(SceneTopics::winFlag)) {
				auto& winFlagChannel {
					scene.getChannel(SceneTopics::winFlag)
				};
				if (winFlagChannel.hasMessages()) {
					winFlagChannel.clear();
					timerChannel.addMessage(utility::Ticker{ winDelay, false });
				}
			}
		}
	}

	void WinSystem::enterCreditsScreen() {
		//push a scene entry message to the credits screen
		globalChannelSetPointer->getChannel(GlobalTopics::sceneEntry)
			.addMessage(SceneNames::credits);
	}
}