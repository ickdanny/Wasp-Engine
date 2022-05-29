#include "Game\GameLoop.h"

namespace wasp::game {

	void GameLoop::run() {
		durationType timeBetweenUpdates{
			static_cast<durationType::rep>(
				((1.0 / updatesPerSecond) * clockType::period::den)
				/ clockType::period::num
			)
		};

		timePointType nextUpdate{ getCurrentTime() };
		timePointType timeOfLastUpdate{ getCurrentTime() };
		int updatesWithoutFrame{ 0 };

		running = true;
		while (running) {
			//force draw every few updates
			if (updatesWithoutFrame >= maxUpdatesWithoutFrame) {
				renderFunction(
					calcDeltaTime(timeOfLastUpdate, timeBetweenUpdates)
				);
				updatesWithoutFrame = 0;
			}
			//update if time
			if (getCurrentTime() >= nextUpdate) {
				updateFunction();
				nextUpdate += timeBetweenUpdates;
				if (nextUpdate < getCurrentTime()) {
					nextUpdate = getCurrentTime();
				}
				timeOfLastUpdate = getCurrentTime();
			}
			//draw frames if possible
			if (getCurrentTime() < nextUpdate) {
				//todo: max fps (min time between updates?)
				while (getCurrentTime() < nextUpdate && running) {
					renderFunction(
						calcDeltaTime(timeOfLastUpdate, timeBetweenUpdates)
					);
				}
			}
			else {
				++updatesWithoutFrame;
			}
		}
	}

	void GameLoop::stop() {
		running = false;
	}

	//static
	GameLoop::timePointType GameLoop::getCurrentTime() {
		return clockType::now();
	}

	//static
	float GameLoop::calcDeltaTime(
		timePointType timeOfLastUpdate,
		durationType timeBetweenUpdates
	) {
		durationType timeSinceLastUpdate{ calcTimeSinceLastUpdate(timeOfLastUpdate) };
		float deltaTime{
			static_cast<float>(
				durationType{timeSinceLastUpdate / timeBetweenUpdates}.count()
			)
		};
		if (deltaTime < 0.0) {
			throw std::runtime_error{ "Error deltaTime < 0" };
		}
		if (deltaTime > 1.0) {
			return 1.0;
		}
		return deltaTime;
	}

	//static
	GameLoop::durationType GameLoop::calcTimeSinceLastUpdate(
		timePointType timeOfLastUpdate
	) {
		return getCurrentTime() - timeOfLastUpdate;
	}
}