#pragma once

#include <functional>
#include <chrono>
#include <stdexcept>

namespace wasp::game::gameloop {

	using clockType = std::chrono::steady_clock;
	using timePointType = clockType::time_point;
	using durationType = clockType::duration;

	//helper function forward declarations
	inline timePointType getCurrentTime();
	inline durationType calcTimeSinceLastUpdate(timePointType timeOfLastUpdate);
	inline double calcDeltaTime(
		timePointType timeOfLastUpdate,
		durationType secondsBetweenUpdates
	);

	//gameloop
	template<
		int updatesPerSecond,
		int maxUpdatesWithoutFrame,
		void(*updateFuncion)(),
		void(*drawFunction)(double)
	>
	void run() {
		durationType timeBetweenUpdates{ 
			static_cast<__int64>(
				((1.0 / updatesPerSecond) * clockType::period::den)
				/ clockType::period::num
			)
		};

		timePointType nextUpdate{ getCurrentTime() };
		timePointType timeOfLastUpdate{ getCurrentTime() };
		int updatesWithoutFrame{ 0 };

		while (true) {
			if (updatesWithoutFrame >= maxUpdatesWithoutFrame) {
				drawFunction(
					calcDeltaTime(timeOfLastUpdate, timeBetweenUpdates)
				);
				updatesWithoutFrame = 0;
			}
			if (getCurrentTime() >= nextUpdate) {
				updateFuncion();
				nextUpdate += timeBetweenUpdates;
				timeOfLastUpdate = getCurrentTime();
			}
			if (getCurrentTime() < nextUpdate) {
				//todo: max fps (min time between updates)
				while (getCurrentTime() < nextUpdate /* && running*/) {
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

	inline timePointType getCurrentTime() {
		return std::chrono::steady_clock::now();
	}

	inline double calcDeltaTime(
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

	inline durationType calcTimeSinceLastUpdate(timePointType timeOfLastUpdate) {
		return getCurrentTime() - timeOfLastUpdate;
	}
}