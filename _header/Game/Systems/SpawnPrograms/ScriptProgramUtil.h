#pragma once

#include "Game/Systems/systemInclude.h"

namespace wasp::game::systems {
	struct ScriptProgramUtil {
	private:
		//typedefs
		using ScriptInstructions = components::ScriptInstructions;
		using ScriptNode = components::ScriptNode;

		template <typename Internal, typename External>
		using ScriptNodeData = components::ScriptNodeData<Internal, External>;
		using ScriptProgram = components::ScriptProgram;

		using ScriptNodeSharedPointer = std::shared_ptr<ScriptNode>;

	public:

		//Returns a script node representing a timer
		static ScriptNodeSharedPointer makeTimerNode(
			int timer,
			ScriptNodeSharedPointer next = nullptr
		) {
			if (timer <= 0) {
				throw std::runtime_error{ " timer <- 0!" };
			}
			ScriptNode* timerNodePointer{
				new ScriptNodeData<int, utility::Ticker>{
					ScriptInstructions::timer,
					timer
				}
			};
			if (next) {
				timerNodePointer->link(next);
			}
			return ScriptNodeSharedPointer{ timerNodePointer };
		}

		//Returns a script node representing a speed shift period.
		static ScriptNodeSharedPointer makeShiftSpeedPeriodNode(
			float targetSpeed,
			int ticks,
			ScriptNodeSharedPointer next = nullptr
		) {
			ScriptNode* shiftSpeedPeriodNodePointer{
				new ScriptNodeData<std::tuple<float, int>, float>{
					ScriptInstructions::shiftSpeedPeriod,
					std::tuple{ targetSpeed, ticks }
				}
			};
			if (next) {
				shiftSpeedPeriodNodePointer->link(next);
			}
			return ScriptNodeSharedPointer{ shiftSpeedPeriodNodePointer };
		}
	};
}