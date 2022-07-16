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

		//Returns a script node representing a timer
		static ScriptNodeSharedPointer makeTimerNode(
			int timer,
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
			const math::Angle initAngle,
			int ticks,
			ScriptNodeSharedPointer next = nullptr
		);

		//Returns a script node representing a long turning velocity shift period
		//with the given values of targetVelocity, initAngle, and ticks
		static ScriptNodeSharedPointer makeShiftVelocityTurnLongPeriodNode(
			const Velocity& targetVelocity,
			const math::Angle initAngle,
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

		//Returns a script node that removes the entity.
		static ScriptNodeSharedPointer makeRemoveEntityNode();

		static ScriptProgram makeShootOnceAndLeaveProgram(
			int preSlowTimer,
			int slowDuration,
			int preTimer,
			const components::SpawnProgram& spawnProgram,
			int postTimer,
			const Velocity& finalVelocity,
			int speedDuration
		);

		static ScriptProgram makeShootOnceAndLeaveTurningProgram(
			int preSlowTimer,
			int slowDuration,
			int preTimer,
			const components::SpawnProgram& spawnProgram,
			int postTimer,
			const Velocity& finalVelocity,
			const math::Angle& initAngle,
			int speedDuration
		);
	};
}