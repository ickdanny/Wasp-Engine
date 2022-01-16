#pragma once

#include <functional>
#include <chrono>
#include <stdexcept>
#include <cstdint>

namespace wasp::game::gameloop {

	//making this a class because i want to be able to stop it
	class GameLoop {
	private:
		using clockType = std::chrono::steady_clock;
		using timePointType = clockType::time_point;
		using durationType = clockType::duration;

		bool running{};
		int updatesPerSecond{};
		int maxUpdatesWithoutFrame{};
		std::function<void()> updateFunction{};
		std::function<void(double)> drawFunction{};

	public:
		GameLoop(
			int updatesPerSecond, 
			int maxUpdatesWithoutFrame,
			const std::function<void()>& updateFunction,
			const std::function<void(double)>& drawFunction
		)
			: running{ false }
			, updatesPerSecond { updatesPerSecond}
			, maxUpdatesWithoutFrame{ maxUpdatesWithoutFrame }
			, updateFunction{ updateFunction }
			, drawFunction{ drawFunction }{
		};

		void run();

		void stop();

	private:
		static timePointType getCurrentTime();

		static double calcDeltaTime(
			timePointType timeOfLastUpdate,
			durationType timeBetweenUpdates
		);

		static durationType calcTimeSinceLastUpdate(timePointType timeOfLastUpdate);
	};
}