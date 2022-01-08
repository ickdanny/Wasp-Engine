#include "GameLoop.h"

namespace wasp::game::gameloop {

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
				drawFunction(
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
					drawFunction(
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

	GameLoop::timePointType GameLoop::getCurrentTime() {
		return std::chrono::steady_clock::now();
	}

	double GameLoop::calcDeltaTime(
		timePointType timeOfLastUpdate,
		durationType timeBetweenUpdates
	) {
		durationType timeSinceLastUpdate{ calcTimeSinceLastUpdate(timeOfLastUpdate) };
		double deltaTime{
			static_cast<double>(
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

	GameLoop::durationType GameLoop::calcTimeSinceLastUpdate(
		timePointType timeOfLastUpdate
	) {
		return getCurrentTime() - timeOfLastUpdate;
	}
}