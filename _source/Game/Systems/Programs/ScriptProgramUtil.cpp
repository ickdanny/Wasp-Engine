#include "Game/Systems/Programs/ScriptProgramUtil.h"

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

	ScriptProgramUtil::ScriptNodeSharedPointer ScriptProgramUtil::makeBoundaryYLowNode(
		float boundary
	){
		ScriptNode* boundaryNodePointer{
			new ScriptNodeData<float, utility::Void>{
				ScriptInstructions::boundaryYLow,
				boundary
			}
		};
		return ScriptNodeSharedPointer{ boundaryNodePointer };
	}

	ScriptProgramUtil::ScriptNodeSharedPointer ScriptProgramUtil::makeBoundaryYHighNode(
		float boundary
	) {
		ScriptNode* boundaryNodePointer{
			new ScriptNodeData<float, utility::Void>{
				ScriptInstructions::boundaryYHigh,
				boundary
			}
		};
		return ScriptNodeSharedPointer{ boundaryNodePointer };
	}

	ScriptProgramUtil::ScriptNodeSharedPointer ScriptProgramUtil::makeBoundaryXLowNode(
		float boundary
	) {
		ScriptNode* boundaryNodePointer{
			new ScriptNodeData<float, utility::Void>{
				ScriptInstructions::boundaryXLow,
				boundary
			}
		};
		return ScriptNodeSharedPointer{ boundaryNodePointer };
	}

	ScriptProgramUtil::ScriptNodeSharedPointer ScriptProgramUtil::makeBoundaryXHighNode(
		float boundary
	) {
		ScriptNode* boundaryNodePointer{
			new ScriptNodeData<float, utility::Void>{
				ScriptInstructions::boundaryXHigh,
				boundary
			}
		};
		return ScriptNodeSharedPointer{ boundaryNodePointer };
	}

	ScriptProgramUtil::ScriptNodeSharedPointer ScriptProgramUtil::makeBoundaryYNode(
		float boundary
	) {
		ScriptNode* boundaryNodePointer{
			new ScriptNodeData<float, utility::Void>{
				ScriptInstructions::boundaryY,
				boundary
			}
		};
		return ScriptNodeSharedPointer{ boundaryNodePointer };
	}

	ScriptProgramUtil::ScriptNodeSharedPointer ScriptProgramUtil::makeBoundaryXNode(
		float boundary
	) {
		ScriptNode* boundaryNodePointer{
			new ScriptNodeData<float, utility::Void>{
				ScriptInstructions::boundaryX,
				boundary
			}
		};
		return ScriptNodeSharedPointer{ boundaryNodePointer };
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

	ScriptProgramUtil::ScriptNodeSharedPointer ScriptProgramUtil::makeRoutineNode(
		const ScriptNodeSharedPointer& scriptBaseNodePointer,
		const ScriptNodeSharedPointer& predicateNodePointer,
		ScriptNodeSharedPointer next
	) {
		ScriptNode* routineNodePointer{
			new ScriptNodeData<utility::Void, ScriptProgram>{ 
				ScriptInstructions::routine,
				{}
			}
		};
		routineNodePointer->link(
			scriptBaseNodePointer,
			predicateNodePointer
		);
		if (next) {
			routineNodePointer->link(next);
		}
		return ScriptNodeSharedPointer{ routineNodePointer };
	}

	ScriptProgramUtil::ScriptNodeSharedPointer ScriptProgramUtil::makeRoutineNode(
		const ScriptNodeSharedPointer& scriptBaseNodePointer,
		ScriptInstructions predicateInstruction,
		ScriptNodeSharedPointer next
	) {
		return makeRoutineNode(
			scriptBaseNodePointer,
			std::make_shared<ScriptNode>(predicateInstruction),
			next
		);
	}

	ScriptProgramUtil::ScriptNodeSharedPointer ScriptProgramUtil::makeRoutineNode(
		const ScriptNodeSharedPointer& scriptBaseNodePointer,
		ScriptNodeSharedPointer next
	) {
		return makeRoutineNode(
			scriptBaseNodePointer,
			nullptr,
			next
		);
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

	ScriptProgramUtil::ScriptNodeSharedPointer ScriptProgramUtil::makeSetCollidableNode(
		ScriptNodeSharedPointer next
	) {
		ScriptNode* setCollidableNodePointer{
			new ScriptNode{ ScriptInstructions::setCollidable }
		};
		if (next) {
			setCollidableNodePointer->link(next);
		}
		return ScriptNodeSharedPointer{ setCollidableNodePointer };
	}

	ScriptProgramUtil::ScriptNodeSharedPointer 
		ScriptProgramUtil::makeRemoveCollidableNode(
			ScriptNodeSharedPointer next
	) {
		ScriptNode* removeCollidableNodePointer{
			new ScriptNode{ ScriptInstructions::removeCollidable }
		};
		if (next) {
			removeCollidableNodePointer->link(next);
		}
		return ScriptNodeSharedPointer{ removeCollidableNodePointer };
	}

	ScriptProgramUtil::ScriptNodeSharedPointer ScriptProgramUtil::makeSetHealthNode(
		int health,
		ScriptNodeSharedPointer next
	) {
		ScriptNode* setHealthNodePointer{
			new ScriptNodeData<int, utility::Void>{
				ScriptInstructions::setHealth,
				health
			}
		};
		if (next) {
			setHealthNodePointer->link(next);
		}
		return ScriptNodeSharedPointer{ setHealthNodePointer };
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

	ScriptProgramUtil::ScriptNodeSharedPointer ScriptProgramUtil::makeAddSpawnNode(
		const components::SpawnProgram& spawnProgram,
		ScriptNodeSharedPointer next
	) {
		ScriptNode* addSpawnNodePointer{
			new ScriptNodeData<components::SpawnProgram, utility::Void>{
				ScriptInstructions::addSpawn,
				spawnProgram
			}
		};
		if (next) {
			addSpawnNodePointer->link(next);
		}
		return ScriptNodeSharedPointer{ addSpawnNodePointer };
	}

	ScriptProgramUtil::ScriptNodeSharedPointer ScriptProgramUtil::makeClearSpawnNode(
		ScriptNodeSharedPointer next
	) {
		ScriptNode* clearSpawnNodePointer{
			new ScriptNode{ ScriptInstructions::clearSpawn }
		};
		if (next) {
			clearSpawnNodePointer->link(next);
		}
		return ScriptNodeSharedPointer{ clearSpawnNodePointer };
	}

	ScriptProgramUtil::ScriptNodeSharedPointer ScriptProgramUtil::makeSetInboundNode(
		float inbound,
		ScriptNodeSharedPointer next
	) {
		ScriptNode* setInboundNodePointer{
			new ScriptNodeData<float, utility::Void>{
				ScriptInstructions::setInbound,
				inbound
			}
		};
		if (next) {
			setInboundNodePointer->link(next);
		}
		return ScriptNodeSharedPointer{ setInboundNodePointer };
	}

	ScriptProgramUtil::ScriptNodeSharedPointer ScriptProgramUtil::makeRemoveInboundNode(
		ScriptNodeSharedPointer next
	) {
		ScriptNode* removeInboundNodePointer{
			new ScriptNode{ ScriptInstructions::removeInbound }
		};
		if (next) {
			removeInboundNodePointer->link(next);
		}
		return ScriptNodeSharedPointer{ removeInboundNodePointer };
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
		const math::Angle& initAngle,
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
		const math::Angle& initAngle,
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
		ScriptProgramUtil::makeShiftAnglePeriodNode
		(
			const math::Angle& targetAngle,
			const math::Angle& initAngle,
			int ticks,
			ScriptNodeSharedPointer next
		) {
		ScriptNode* shiftAnglePeriodNodePointer{
			new ScriptNodeData<std::tuple<math::Angle, math::Angle, int>, float>{
				ScriptInstructions::shiftAnglePeriod,
				std::tuple{ targetAngle, initAngle, ticks }
			}
		};
		if (next) {
			shiftAnglePeriodNodePointer->link(next);
		}
		return ScriptNodeSharedPointer{ shiftAnglePeriodNodePointer };
	}

	ScriptProgramUtil::ScriptNodeSharedPointer
		ScriptProgramUtil::makeShiftAngleLongPeriodNode
		(
			const math::Angle& targetAngle,
			const math::Angle& initAngle,
			int ticks,
			ScriptNodeSharedPointer next
		) {
		ScriptNode* shiftAngleLongPeriodNodePointer{
			new ScriptNodeData<std::tuple<math::Angle, math::Angle, int>, float>{
				ScriptInstructions::shiftAngleLongPeriod,
				std::tuple{ targetAngle, initAngle, ticks }
			}
		};
		if (next) {
			shiftAngleLongPeriodNodePointer->link(next);
		}
		return ScriptNodeSharedPointer{ shiftAngleLongPeriodNodePointer };
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
		ScriptNode* setVelocityNodePointer{
			new ScriptNode{
				ScriptInstructions::setVelocity,
			}
		};
		setVelocityNodePointer->link(makeVelocityValueNode(velocity));
		if (next) {
			setVelocityNodePointer->link(next);
		}
		return ScriptNodeSharedPointer{ setVelocityNodePointer };
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
		int speedDuration,
		ScriptNodeSharedPointer next
	) {
		return {
			makeTimerNode(preSlowTimer,
			makeShiftSpeedPeriodNode(0.0f, slowDuration,
			makeTimerNode(preTimer,
			makeSetSpawnNode(spawnProgram,
			makeTimerNode(2,
			makeStallingIfNode(ScriptInstructions::isNotSpawning,
			makeTimerNode(postTimer,
			makeShiftVelocityPeriodNode(finalVelocity, speedDuration, next
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
		int speedDuration,
		ScriptNodeSharedPointer next
	) {
		return {
			makeTimerNode(preSlowTimer,
			makeShiftSpeedPeriodNode(0.0f, slowDuration,
			makeTimerNode(preTimer,
			makeSetSpawnNode(spawnProgram,
			makeTimerNode(2,
			makeStallingIfNode(ScriptInstructions::isNotSpawning,
			makeTimerNode(postTimer,
			makeShiftVelocityTurnPeriodNode(finalVelocity, initAngle, speedDuration, next
			))))))))
		};
	}

	ScriptProgramUtil::ScriptProgram ScriptProgramUtil::makeTurnEntryAndShootProgram(
		int preSlowTimer,
		int slowDuration,
		const Velocity& finalVelocity,
		const math::Angle& initAngle,
		const components::SpawnProgram& spawnProgram,
		ScriptNodeSharedPointer next
	) {
		return {
			makeTimerNode(preSlowTimer,
			makeShiftVelocityTurnPeriodNode(finalVelocity, initAngle, slowDuration,
			makeSetSpawnNode(spawnProgram, next)))
		};
	}

	ScriptProgramUtil::ScriptNodeSharedPointer 
		ScriptProgramUtil::makeGotoDeceleratingNode
	(
		const math::Point2& targetPos,
		float maxSpeed,
		ScriptNodeSharedPointer next
	) {
		ScriptNode* gotoDeceleratingNodePointer{
			new ScriptNodeData<std::tuple<math::Point2, float>, float>{
				ScriptInstructions::gotoDecelerating,
				{ targetPos, maxSpeed }
			}
		};
		if (next) {
			gotoDeceleratingNodePointer->link(next);
		}
		return ScriptNodeSharedPointer{ gotoDeceleratingNodePointer };
	}

	ScriptProgramUtil::ScriptNodeSharedPointer
		ScriptProgramUtil::makeBoundRadiusGotoDeceleratingNode
		(
			const math::AABB bounds,
			float minRadius,
			float maxRadius,
			float maxSpeed,
			ScriptNodeSharedPointer next
		) {
		ScriptNode* boundRadiusGotoDeceleratingNodePointer{
			new ScriptNodeData<
				std::tuple<math::AABB, float, float, float>,
				std::tuple<math::Point2, float>
			>{
				ScriptInstructions::boundRadiusGotoDecelerating,
				{ bounds, minRadius, maxRadius, maxSpeed }
			}
		};
		if (next) {
			boundRadiusGotoDeceleratingNodePointer->link(next);
		}
		return ScriptNodeSharedPointer{ boundRadiusGotoDeceleratingNodePointer };
	}

	ScriptProgramUtil::ScriptNodeSharedPointer ScriptProgramUtil::makeShowDialogueNode(
		const std::wstring& dialogueID,
		ScriptNodeSharedPointer next
	) {
		ScriptNode* showDialogueNodePointer{
			new ScriptNodeData<std::wstring, utility::Void>{
				ScriptInstructions::showDialogue,
				dialogueID
			}
		};
		if (next) {
			showDialogueNodePointer->link(next);
		}
		return ScriptNodeSharedPointer{ showDialogueNodePointer };
	}

	ScriptProgramUtil::ScriptNodeSharedPointer 
		ScriptProgramUtil::makeIsDialogueOverNode() 
	{
		ScriptNode* isDialogueOverNodePointer{
			new ScriptNode{
				ScriptInstructions::isDialogueOver
			}
		};
		return ScriptNodeSharedPointer{ isDialogueOverNodePointer };
	}

	ScriptProgramUtil::ScriptNodeSharedPointer ScriptProgramUtil::makeIsBossDeadNode() {
		ScriptNode* isBossDeadNodePointer{
			new ScriptNode{
				ScriptInstructions::isBossDead
			}
		};
		return ScriptNodeSharedPointer{ isBossDeadNodePointer };
	}

	ScriptProgramUtil::ScriptNodeSharedPointer ScriptProgramUtil::makeBossEntryNode(
		int preTimer,
		const std::wstring& dialogueID,
		ScriptNodeSharedPointer next
	) {
		return{
			makeGotoDeceleratingNode(config::bossMidpoint, config::bossSpeed,
			makeSetInboundNode(config::bossInbound,
			makeTimerNode(preTimer,
			makeShowDialogueNode(dialogueID,
			makeStallingIfNode(
				makeIsDialogueOverNode(),
				makeSetCollidableNode(next)
			)))))
		};
	}

	ScriptProgramUtil::ScriptNodeSharedPointer 
		ScriptProgramUtil::makeBossAttackAndMoveNode
	(
		const components::SpawnProgram& spawnProgram,
		int preTimer,
		int postTimer,
		ScriptNodeSharedPointer next
	) {
		ScriptNodeSharedPointer scriptBaseNodePointer{
			makeSetSpawnNode(spawnProgram)
		};
		scriptBaseNodePointer->link(
			makeTimerNode(preTimer,
			makeBoundRadiusGotoDeceleratingNode(
				config::bossBounds,
				config::bossGotoRadiusMin,
				config::bossGotoRadiusMax,
				config::bossGotoSpeed,
			makeStallingIfNode(
				ScriptInstructions::isNotSpawning,
				makeTimerNode(postTimer,
				makeSetVelocityNode(Velocity{},
				scriptBaseNodePointer
				))
			)))
		);
		return{
			makeRoutineNode(
				scriptBaseNodePointer,
				makeIsBossDeadNode(),		//routine breaks when boss dies
				next
			)
		};
	}

	ScriptProgramUtil::ScriptNodeSharedPointer ScriptProgramUtil::makeBossMoveNode (
		int preTimer,
		int postTimer,
		float speed,
		ScriptNodeSharedPointer next
	) {
		ScriptNodeSharedPointer scriptBaseNodePointer{
			makeTimerNode(preTimer)
		};
		scriptBaseNodePointer->link(
			makeBoundRadiusGotoDeceleratingNode(
				config::bossBounds,
				config::bossGotoRadiusMin,
				config::bossGotoRadiusMax,
				speed,
			makeTimerNode(postTimer,
			makeSetVelocityNode(Velocity{},
			scriptBaseNodePointer
			)))
		);
		return{
			makeRoutineNode(
				scriptBaseNodePointer,
				makeIsBossDeadNode(),		//routine breaks when boss dies
				next
			)
		};
	}

	ScriptProgramUtil::ScriptNodeSharedPointer ScriptProgramUtil::makeBossMoveNode(
		int preTimer,
		int postTimer,
		ScriptNodeSharedPointer next
	) {
		return makeBossMoveNode(preTimer, postTimer, config::bossGotoSpeed, next);
	}
}