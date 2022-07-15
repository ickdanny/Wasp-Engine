#include "Game/Systems/SpawnPrograms/ScriptProgramUtil.h"

namespace wasp::game::systems {

	ScriptProgramUtil::ScriptNodeSharedPointer ScriptProgramUtil::makeVelocityValueNode(
		const Velocity& velocity
	) {
		ScriptNode* velocityValuePointer{
			new ScriptNodeData<Velocity, utility::Void>{
				ScriptInstructions::value,
				velocity
			}
		};
		return ScriptNodeSharedPointer{ velocityValuePointer };
	}

	ScriptProgramUtil::ScriptNodeSharedPointer ScriptProgramUtil::makeIfNode(
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

	ScriptProgramUtil::ScriptNodeSharedPointer ScriptProgramUtil::makeIfNode(
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

	ScriptProgramUtil::ScriptNodeSharedPointer ScriptProgramUtil::makeStallingIfNode(
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

	ScriptProgramUtil::ScriptNodeSharedPointer ScriptProgramUtil::makeStallingIfNode(
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

	ScriptProgramUtil::ScriptNodeSharedPointer ScriptProgramUtil::makeIfElseNode(
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

	ScriptProgramUtil::ScriptNodeSharedPointer ScriptProgramUtil::makeIfElseNode(
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

	ScriptProgramUtil::ScriptNodeSharedPointer ScriptProgramUtil::makeTimerNode(
		int timer,
		ScriptNodeSharedPointer next
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

	ScriptProgramUtil::ScriptNodeSharedPointer ScriptProgramUtil::makeSetSpawnNode(
		const components::SpawnProgram& spawnProgram,
		ScriptNodeSharedPointer next
	) {
		ScriptNode* setSpawnNodePointer{
			new ScriptNodeData<components::SpawnProgram, utility::Void>{
				ScriptInstructions::setSpawn,
				spawnProgram
			}
		};
		if (next) {
			setSpawnNodePointer->link(next);
		}
		return ScriptNodeSharedPointer{ setSpawnNodePointer };
	}

	ScriptProgramUtil::ScriptNodeSharedPointer 
		ScriptProgramUtil::makeShiftSpeedPeriodNode
	(
		float targetSpeed,
		int ticks,
		ScriptNodeSharedPointer next
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

	ScriptProgramUtil::ScriptNodeSharedPointer 
		ScriptProgramUtil::makeShiftVelocityPeriodNode
	(
		const Velocity& targetVelocity,
		int ticks,
		ScriptNodeSharedPointer next
	) {
		ScriptNode* shiftVelocityPeriodNodePointer{
			new ScriptNodeData<std::tuple<Velocity, int>, float>{
				ScriptInstructions::shiftVelocityPeriod,
				std::tuple{ targetVelocity, ticks }
			}
		};
		if (next) {
			shiftVelocityPeriodNodePointer->link(next);
		}
		return ScriptNodeSharedPointer{ shiftVelocityPeriodNodePointer };
	}

	ScriptProgramUtil::ScriptNodeSharedPointer 
		ScriptProgramUtil::makeShiftVelocityTurnPeriodNode
	(
		const Velocity& targetVelocity,
		const math::Angle initAngle,
		int ticks,
		ScriptNodeSharedPointer next
	) {
		ScriptNode* shiftVelocityTurnPeriodNodePointer{
			new ScriptNodeData<
				std::tuple<Velocity, math::Angle, int>,
				std::tuple<float, float>
			>{
				ScriptInstructions::shiftVelocityTurnPeriod,
				std::tuple{ targetVelocity, initAngle, ticks }
			}
		};
		if (next) {
			shiftVelocityTurnPeriodNodePointer->link(next);
		}
		return ScriptNodeSharedPointer{ shiftVelocityTurnPeriodNodePointer };
	}

	ScriptProgramUtil::ScriptNodeSharedPointer 
		ScriptProgramUtil::makeShiftVelocityTurnLongPeriodNode
	(
		const Velocity& targetVelocity,
		const math::Angle initAngle,
		int ticks,
		ScriptNodeSharedPointer next
	) {
		ScriptNode* shiftVelocityTurnLongPeriodNodePointer{
			new ScriptNodeData<
				std::tuple<Velocity, math::Angle, int>,
				std::tuple<float, float>
			>{
				ScriptInstructions::shiftVelocityTurnLongPeriod,
				std::tuple{ targetVelocity, initAngle, ticks }
			}
		};
		if (next) {
			shiftVelocityTurnLongPeriodNodePointer->link(next);
		}
		return ScriptNodeSharedPointer{ shiftVelocityTurnLongPeriodNodePointer };
	}

	ScriptProgramUtil::ScriptNodeSharedPointer 
		ScriptProgramUtil::makeShiftSpeedIncrementNode
	(
		float targetSpeed,
		float increment,
		ScriptNodeSharedPointer next
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

	ScriptProgramUtil::ScriptNodeSharedPointer ScriptProgramUtil::makeSetVelocityNode(
		ScriptNodeSharedPointer velocityNode,
		ScriptNodeSharedPointer next
	) {
		ScriptNode* setVelocityNode{
			new ScriptNode{
				ScriptInstructions::setVelocity,
			}
		};
		setVelocityNode->link(velocityNode);
		if (next) {
			setVelocityNode->link(next);
		}
		return ScriptNodeSharedPointer{ setVelocityNode };
	}

	ScriptProgramUtil::ScriptNodeSharedPointer ScriptProgramUtil::makeSetVelocityNode(
		const Velocity& velocity,
		ScriptNodeSharedPointer next
	) {
		ScriptNode* setVelocityNode{
			new ScriptNode{
				ScriptInstructions::setVelocity,
			}
		};
		setVelocityNode->link(makeVelocityValueNode(velocity));
		if (next) {
			setVelocityNode->link(next);
		}
		return ScriptNodeSharedPointer{ setVelocityNode };
	}

	ScriptProgramUtil::ScriptNodeSharedPointer 
		ScriptProgramUtil::makeRemoveEntityNode() 
	{
		ScriptNode* removeEntityNodePointer{
			new ScriptNode{ ScriptInstructions::removeEntity }
		};
		return ScriptNodeSharedPointer{ removeEntityNodePointer };
	}


	ScriptProgramUtil::ScriptProgram ScriptProgramUtil::makeShootOnceAndLeaveProgram(
		int preSlowTimer,
		int slowDuration,
		int preTimer,
		const components::SpawnProgram& spawnProgram,
		int postTimer,
		const Velocity& finalVelocity,
		int speedDuration
	) {
		return {
			makeTimerNode(preSlowTimer,
			makeShiftSpeedPeriodNode(0.0f, slowDuration,
			makeTimerNode(preTimer,
			makeSetSpawnNode(spawnProgram,
			makeTimerNode(2,
			makeStallingIfNode(ScriptInstructions::isNotSpawning,
			makeTimerNode(postTimer,
			makeShiftVelocityPeriodNode(finalVelocity, speedDuration
			))))))))
		};
	}

	ScriptProgramUtil::ScriptProgram 
		ScriptProgramUtil::makeShootOnceAndLeaveTurningProgram
	(
		int preSlowTimer,
		int slowDuration,
		int preTimer,
		const components::SpawnProgram& spawnProgram,
		int postTimer,
		const Velocity& finalVelocity,
		const math::Angle& initAngle,
		int speedDuration
	) {
		return {
			makeTimerNode(preSlowTimer,
			makeShiftSpeedPeriodNode(0.0f, slowDuration,
			makeTimerNode(preTimer,
			makeSetSpawnNode(spawnProgram,
			makeTimerNode(200,
			makeStallingIfNode(ScriptInstructions::isNotSpawning,
			makeTimerNode(postTimer,
			makeShiftVelocityTurnPeriodNode(finalVelocity, initAngle, speedDuration
			))))))))
		};
	}
}