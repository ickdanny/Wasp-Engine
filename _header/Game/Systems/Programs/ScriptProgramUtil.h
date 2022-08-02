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

		//Returns a script node representing a velocity value.
		static ScriptNodeSharedPointer makeVelocityValueNode(
			const Velocity& velocity
		);

		//Returns a spawn node representing a boundary predicate for the given 
		//value.

		static ScriptNodeSharedPointer makeBoundaryYLowNode(float boundary);
		static ScriptNodeSharedPointer makeBoundaryYHighNode(float boundary);
		static ScriptNodeSharedPointer makeBoundaryXLowNode(float boundary);
		static ScriptNodeSharedPointer makeBoundaryXHighNode(float boundary);

		//Returns a script node representing an if statement.
		static ScriptNodeSharedPointer makeIfNode(
			const ScriptNodeSharedPointer& predicateNodePointer,
			const ScriptNodeSharedPointer& trueNodePointer
		);

		//Returns a spawn node representing an if statement for the given predicate.
		static ScriptNodeSharedPointer makeIfNode(
			ScriptInstructions predicateInstruction,
			const ScriptNodeSharedPointer& trueNodePointer
		);

		//Returns a script node representing a stalling if statement.
		static ScriptNodeSharedPointer makeStallingIfNode(
			const ScriptNodeSharedPointer& predicateNodePointer,
			const ScriptNodeSharedPointer& trueNodePointer
		);

		//Returns a spawn node representing a stalling if statement for the given 
		//predicate.
		static ScriptNodeSharedPointer makeStallingIfNode(
			ScriptInstructions predicateInstruction,
			const ScriptNodeSharedPointer& trueNodePointer
		);

		//Returns a spawn node representing an if-else statement.
		static ScriptNodeSharedPointer makeIfElseNode(
			const ScriptNodeSharedPointer& predicateNodePointer,
			const ScriptNodeSharedPointer& trueNodePointer,
			const ScriptNodeSharedPointer& falseNodePointer
		);

		//Returns a spawn node representing an if-else statement for the given 
		//predicate.
		static ScriptNodeSharedPointer makeIfElseNode(
			ScriptInstructions predicateInstruction,
			const ScriptNodeSharedPointer& trueNodePointer,
			const ScriptNodeSharedPointer& falseNodePointer
		);

		//Returns a spawn node representing a routine with the given base and predicate.
		static ScriptNodeSharedPointer makeRoutineNode(
			const ScriptNodeSharedPointer& scriptBaseNodePointer,
			const ScriptNodeSharedPointer& predicateNodePointer,
			ScriptNodeSharedPointer next = nullptr
		);

		//Returns a spawn node representing a routine with the given base and predicate.
		static ScriptNodeSharedPointer makeRoutineNode(
			const ScriptNodeSharedPointer& scriptBaseNodePointer,
			ScriptInstructions predicateInstruction,
			ScriptNodeSharedPointer next = nullptr
		);

		//Returns a spawn node representing a routine with the given base and no
		//predicate.
		static ScriptNodeSharedPointer makeRoutineNode(
			const ScriptNodeSharedPointer& scriptBaseNodePointer,
			ScriptNodeSharedPointer next = nullptr
		);

		//Returns a script node representing a timer.
		static ScriptNodeSharedPointer makeTimerNode(
			int timer,
			ScriptNodeSharedPointer next = nullptr
		);

		//Returns a script node representing a set collidable command.
		static ScriptNodeSharedPointer makeSetCollidableNode(
			ScriptNodeSharedPointer next = nullptr
		);

		//Returns a script node representing a remove collidable command.
		static ScriptNodeSharedPointer makeRemoveCollidableNode(
			ScriptNodeSharedPointer next = nullptr
		);

		//Returns a script node representing a set health command.
		static ScriptNodeSharedPointer makeSetHealthNode(
			int health,
			ScriptNodeSharedPointer next = nullptr
		);

		//Returns a script node representing a set spawn command.
		static ScriptNodeSharedPointer makeSetSpawnNode(
			const components::SpawnProgram& spawnProgram,
			ScriptNodeSharedPointer next = nullptr
		);

		//Returns a script node representing an add spawn command.
		static ScriptNodeSharedPointer makeAddSpawnNode(
			const components::SpawnProgram& spawnProgram,
			ScriptNodeSharedPointer next = nullptr
		);

		//Returns a script node representing a clear spawn command.
		static ScriptNodeSharedPointer makeClearSpawnNode(
			ScriptNodeSharedPointer next = nullptr
		);

		//Returns a script node representing a set inbound command.
		static ScriptNodeSharedPointer makeSetInboundNode(
			float inbound,
			ScriptNodeSharedPointer next = nullptr
		);

		//Returns a script node representing a remove inbound command.
		static ScriptNodeSharedPointer makeRemoveInboundNode(
			ScriptNodeSharedPointer next = nullptr
		);

		//Returns a script node representing a speed shift period with the given values
		//of targetSpeed and ticks.
		static ScriptNodeSharedPointer makeShiftSpeedPeriodNode(
			float targetSpeed,
			int ticks,
			ScriptNodeSharedPointer next = nullptr
		);

		//Returns a script node representing a velocity shift period with the given
		//values of targetVelocity and ticks
		static ScriptNodeSharedPointer makeShiftVelocityPeriodNode(
			const Velocity& targetVelocity,
			int ticks,
			ScriptNodeSharedPointer next = nullptr
		);

		//Returns a script node representing a short turning velocity shift period
		//with the given values of targetVelocity, initAngle, and ticks
		static ScriptNodeSharedPointer makeShiftVelocityTurnPeriodNode(
			const Velocity& targetVelocity,
			const math::Angle& initAngle,
			int ticks,
			ScriptNodeSharedPointer next = nullptr
		);

		//Returns a script node representing a long turning velocity shift period
		//with the given values of targetVelocity, initAngle, and ticks
		static ScriptNodeSharedPointer makeShiftVelocityTurnLongPeriodNode(
			const Velocity& targetVelocity,
			const math::Angle& initAngle,
			int ticks,
			ScriptNodeSharedPointer next = nullptr
		);

		//Returns a script node reprsenting a short turn period with the given values
		//of targetAngle, initAngle, and ticks.
		static ScriptNodeSharedPointer makeShiftAnglePeriodNode(
			const math::Angle& targetAngle,
			const math::Angle& initAngle,
			int ticks,
			ScriptNodeSharedPointer next = nullptr
		);

		//Returns a script node reprsenting a long turn period with the given values
		//of targetAngle, initAngle, and ticks.
		static ScriptNodeSharedPointer makeShiftAngleLongPeriodNode(
			const math::Angle& targetAngle,
			const math::Angle& initAngle,
			int ticks,
			ScriptNodeSharedPointer next = nullptr
		);

		//Returns a script node representing a speed increment shift with the given
		//values of targetSpeed and increment.
		static ScriptNodeSharedPointer makeShiftSpeedIncrementNode(
			float targetSpeed,
			float increment,
			ScriptNodeSharedPointer next = nullptr
		);

		//Returns a script node representing a set velocity order.
		static ScriptNodeSharedPointer makeSetVelocityNode(
			ScriptNodeSharedPointer velocityNode,
			ScriptNodeSharedPointer next = nullptr
		);

		//Returns a script node representing a set velocity order to the specified
		//velocity.
		static ScriptNodeSharedPointer makeSetVelocityNode(
			const Velocity& velocity,
			ScriptNodeSharedPointer next = nullptr
		);

		//Returns a script node representing a set velocity to player instruction.
		static ScriptNodeSharedPointer makeSetVelocityToPlayerNode(
			float speed,
			ScriptNodeSharedPointer next = nullptr
		);

		//Returns a script node that sends a death message for the entity.
		static ScriptNodeSharedPointer makeDieNode();

		//Returns a script node that removes the entity.
		static ScriptNodeSharedPointer makeRemoveEntityNode();

		//Returns a program where the entity shoots its attack once and leaves
		//in a line.
		static ScriptProgram makeShootOnceAndLeaveProgram(
			int preSlowTimer,
			int slowDuration,
			int preTimer,
			const components::SpawnProgram& spawnProgram,
			int postTimer,
			const Velocity& finalVelocity,
			int speedDuration,
			ScriptNodeSharedPointer next = nullptr
		);

		//Returns a program where the entity shoots its attack once and leaves
		//turning short side.
		static ScriptProgram makeShootOnceAndLeaveTurningProgram(
			int preSlowTimer,
			int slowDuration,
			int preTimer,
			const components::SpawnProgram& spawnProgram,
			int postTimer,
			const Velocity& finalVelocity,
			const math::Angle& initAngle,
			int speedDuration,
			ScriptNodeSharedPointer next = nullptr
		);

		//Returns a program where the entity shoots its attack once and leaves
		//turning long side.
		static ScriptProgram makeTurnEntryAndShootProgram(
			int preSlowTimer,
			int slowDuration,
			const Velocity& finalVelocity,
			const math::Angle& initAngle,
			const components::SpawnProgram& spawnProgram,
			ScriptNodeSharedPointer next = nullptr
		);

		//boss stuff

		//Returns a script node that has the entity go to the specified point while
		//decelerating.
		static ScriptNodeSharedPointer makeGotoDeceleratingNode(
			const math::Point2& targetPos,
			float maxSpeed,
			ScriptNodeSharedPointer next = nullptr
		);

		//Returns a script node that has the entity go to a randomly chosen node
		//within the given bounds while decelerating.
		static ScriptNodeSharedPointer makeBoundRadiusGotoDeceleratingNode(
			const math::AABB bounds,
			float minRadius,
			float maxRadius,
			float maxSpeed,
			ScriptNodeSharedPointer next = nullptr
		);

		//Returns a script node that tells the scene manager to pop up a specific
		//dialogue scene.
		static ScriptNodeSharedPointer makeShowDialogueNode(
			const std::wstring& dialogueID,
			ScriptNodeSharedPointer next = nullptr
		);

		//Returns a predicate that returns true when the dialogue is over.
		static ScriptNodeSharedPointer makeIsDialogueOverNode();

		//Returns a predicate that returns true when a boss death is detected.
		static ScriptNodeSharedPointer makeIsBossDeadNode();

		//Returns a script node that represents a boss's entry routine.
		static ScriptNodeSharedPointer makeBossEntryNode(
			int preTimer,
			const std::wstring& dialogueID,
			ScriptNodeSharedPointer next = nullptr
		);

		//Returns a script node for resetting a boss after death.
		static ScriptNodeSharedPointer makeBossResetNode(
			int postTimer,
			ScriptNodeSharedPointer next = nullptr
		);

		//Returns a script node for resetting a boss after death, having her go to mid.
		static ScriptNodeSharedPointer makeBossResetMidNode(
			int postTimer,
			ScriptNodeSharedPointer next = nullptr
		);

		//Returns a routine for an attack/move phase
		static ScriptNodeSharedPointer makeBossAttackAndMoveNode(
			const components::SpawnProgram& spawnProgram,
			int preTimer,
			int postTimer,
			ScriptNodeSharedPointer next = nullptr
		);

		//Returns a routine for a 2 part attack/move phase
		static ScriptNodeSharedPointer makeDoubleBossAttackAndMoveNode(
			const components::SpawnProgram& spawnProgram1,
			int preTimer1,
			int postTimer1,
			const components::SpawnProgram& spawnProgram2,
			int preTimer2,
			int postTimer2,
			ScriptNodeSharedPointer next = nullptr
		);

		//Returns a routine for a move phase where attack is looping
		static ScriptNodeSharedPointer makeBossMoveNode(
			int preTimer,
			int postTimer,
			float speed,
			ScriptNodeSharedPointer next = nullptr
		);

		//Returns a routine for a move phase where attack is looping
		static ScriptNodeSharedPointer makeBossMoveNode(
			int preTimer,
			int postTimer,
			ScriptNodeSharedPointer next = nullptr
		);

		//Returns a routine for a move phase where the given linear script is ran in 
		//the middle, passed as start and end nodes.
		static ScriptNodeSharedPointer makeBossActAndMoveNode(
			int preTimer,
			int postTimer,
			float speed,
			ScriptNodeSharedPointer scriptStartNodePointer,
			ScriptNodeSharedPointer scriptEndNodePointer,
			ScriptNodeSharedPointer next = nullptr
		);

		//Returns a routine for a move phase where the given linear script is ran in 
		//the middle, passed as start and end nodes.
		static ScriptNodeSharedPointer makeBossActAndMoveNode(
			int preTimer,
			int postTimer,
			ScriptNodeSharedPointer scriptStartNodePointer,
			ScriptNodeSharedPointer scriptEndNodePointer,
			ScriptNodeSharedPointer next = nullptr
		);

		/*
		//post timer must be long enough
    public static InstructionList makeStrictBossMoveProgram(int preTimer, int postTimer, double speed) {
        return ProgramBuilder.linearLink(
                new InstructionNode<>(TIMER, preTimer)
        ).linkAppend(
                ProgramBuilder.linearLink(
                        new InstructionNode<>(BOUND_RADIUS_GOTO_DECELERATING, new Tuple4<>(
                                BOSS_BOUNDS,
                                BOSS_GOTO_RADIUS_MIN,
                                BOSS_GOTO_RADIUS_MAX,
                                speed
                        ))
                ).linkInject(
                        ProgramBuilder.linearLink(
                                new InstructionNode<>(TIMER, postTimer),
                                new InstructionNode<>(SET_VELOCITY, new PolarVector(0, 0))
                        )
                )
        ).linkBackToFront();
    }

    public static InstructionList makeStrictBossMoveProgramNoLoop(int preTimer, int postTimer, double speed) {
        return ProgramBuilder.linearLink(
                new InstructionNode<>(TIMER, preTimer)
        ).linkAppend(
                ProgramBuilder.linearLink(
                        new InstructionNode<>(BOUND_RADIUS_GOTO_DECELERATING, new Tuple4<>(
                                BOSS_BOUNDS,
                                BOSS_GOTO_RADIUS_MIN,
                                BOSS_GOTO_RADIUS_MAX,
                                speed
                        ))
                ).linkInject(
                        ProgramBuilder.linearLink(
                                new InstructionNode<>(TIMER, postTimer),
                                new InstructionNode<>(SET_VELOCITY, new PolarVector(0, 0))
                        )
                )
        );
    }
	*/
	};
}