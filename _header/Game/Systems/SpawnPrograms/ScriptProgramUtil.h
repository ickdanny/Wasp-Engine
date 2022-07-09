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
		//Returns a script node representing an if statement.
		static ScriptNodeSharedPointer makeIfNode(
			const ScriptNodeSharedPointer& predicateNodePointer,
			const ScriptNodeSharedPointer& trueNodePointer
		) {
			ScriptNode* ifNodePointer{
				new ScriptNode{ ScriptInstructions::condition }
			};
			ifNodePointer->link(
				predicateNodePointer,
				trueNodePointer
			);
			return ScriptNodeSharedPointer{ ifNodePointer };
		}

		//Returns a spawn node representing an if statement for the given predicate.
		static ScriptNodeSharedPointer makeIfNode(
			ScriptInstructions predicateInstruction,
			const ScriptNodeSharedPointer& trueNodePointer
		) {
			ScriptNode* ifNodePointer{
				new ScriptNode{ ScriptInstructions::condition }
			};
			ifNodePointer->link(
				std::make_shared<ScriptNode>(predicateInstruction),
				trueNodePointer
			);
			return ScriptNodeSharedPointer{ ifNodePointer };
		}

		//Returns a script node representing a stalling if statement.
		static ScriptNodeSharedPointer makeStallingIfNode(
			const ScriptNodeSharedPointer& predicateNodePointer,
			const ScriptNodeSharedPointer& trueNodePointer
		) {
			ScriptNode* ifNodePointer{
				new ScriptNode{ ScriptInstructions::stallCondition }
			};
			ifNodePointer->link(
				predicateNodePointer,
				trueNodePointer
			);
			return ScriptNodeSharedPointer{ ifNodePointer };
		}

		//Returns a spawn node representing a stalling if statement for the given 
		//predicate.
		static ScriptNodeSharedPointer makeStallingIfNode(
			ScriptInstructions predicateInstruction,
			const ScriptNodeSharedPointer& trueNodePointer
		) {
			ScriptNode* ifNodePointer{
				new ScriptNode{ ScriptInstructions::stallCondition }
			};
			ifNodePointer->link(
				std::make_shared<ScriptNode>(predicateInstruction),
				trueNodePointer
			);
			return ScriptNodeSharedPointer{ ifNodePointer };
		}

		//Returns a spawn node representing an if-else statement.
		static ScriptNodeSharedPointer makeIfElseNode(
			const ScriptNodeSharedPointer& predicateNodePointer,
			const ScriptNodeSharedPointer& trueNodePointer,
			const ScriptNodeSharedPointer& falseNodePointer
		) {
			ScriptNode* ifElseNodePointer{
				new ScriptNode{ ScriptInstructions::conditionElse }
			};
			ifElseNodePointer->link(
				predicateNodePointer,
				trueNodePointer,
				falseNodePointer
			);
			return ScriptNodeSharedPointer{ ifElseNodePointer };
		}

		//Returns a spawn node representing an if-else statement for the given 
		//predicate.
		static ScriptNodeSharedPointer makeIfElseNode(
			ScriptInstructions predicateInstruction,
			const ScriptNodeSharedPointer& trueNodePointer,
			const ScriptNodeSharedPointer& falseNodePointer
		) {
			ScriptNode* ifElseNodePointer{
				new ScriptNode{ ScriptInstructions::conditionElse }
			};
			ifElseNodePointer->link(
				std::make_shared<ScriptNode>(predicateInstruction),
				trueNodePointer,
				falseNodePointer
			);
			return ScriptNodeSharedPointer{ ifElseNodePointer };
		}

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

		//Returns a script node representing a speed shift period with the given values
		//of targetSpeed and ticks.
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

		//Returns a script node representing a speed increment shift with the given
		//values of targetSpeed and increment.
		static ScriptNodeSharedPointer makeShiftSpeedIncrementNode(
			float targetSpeed,
			float increment,
			ScriptNodeSharedPointer next = nullptr
		) {
			ScriptNode* shiftSpeedIncrementNodePointer{
				new ScriptNodeData<std::tuple<float, float>, utility::Void>{
					ScriptInstructions::shiftSpeedIncrement,
					std::tuple{ targetSpeed, increment }
				}
			};
			if (next) {
				shiftSpeedIncrementNodePointer->link(next);
			}
			return ScriptNodeSharedPointer{ shiftSpeedIncrementNodePointer };
		}
	};
}