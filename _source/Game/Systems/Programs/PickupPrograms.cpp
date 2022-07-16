#include "Game/Systems/Programs/PickupPrograms.h"

namespace wasp::game::systems {

	using namespace math;
	using namespace components;

	using ScriptProgramList = game::ScriptProgramList;

	namespace {
		constexpr float deceleration{ -0.07f };
		constexpr int clearLifetime{ 5 };
	}

	PickupPrograms::PickupPrograms(
		resources::BitmapStorage* bitmapStoragePointer
	)
		: bitmapStoragePointer{ bitmapStoragePointer }

		, pickupScriptNode{
			ScriptProgramUtil::makeShiftSpeedIncrementNode(
				config::pickupFinalSpeed, 
				deceleration
			)
		}

		//small power
		, smallPowerPrototype{
			EntityBuilder::makePosVelPrototype(
				config::smallPickupHitbox,
				PickupCollisions::Source{ components::CollisionCommands::pickup },
				game::PickupType{ components::PickupType::Types::powerSmall },
				Outbound{ config::pickupOutbound },
				SpriteInstruction{
					bitmapStoragePointer->get(L"power_small")->d2dBitmap,
				},
				ScriptProgramList{ pickupScriptNode },
				DrawOrder{ config::pickupDrawOrder }
			).heapClone()
		}
		, spawnSmallPowerNode{
			SpawnProgramUtil::makeSpawnPosVelNode(
				smallPowerPrototype,
				SpawnProgramUtil::makeEntityPositionNode(),
				SpawnProgramUtil::makeVelocityFromPolarNode(
					SpawnProgramUtil::makePickupInitSpeedNode(),
					SpawnProgramUtil::makeFloatValueNode(90.0f)
				)
			)
		}
		, smallPowerSpawnProgram{ spawnSmallPowerNode, 1, false }

		//large power
		, largePowerPrototype{
			EntityBuilder::makePosVelPrototype(
				config::largePickupHitbox,
				PickupCollisions::Source{ components::CollisionCommands::pickup },
				game::PickupType{ components::PickupType::Types::powerLarge },
				Outbound{ config::pickupOutbound },
				SpriteInstruction{
					bitmapStoragePointer->get(L"power_large")->d2dBitmap,
				},
				ScriptProgramList{ pickupScriptNode },
				DrawOrder{ config::pickupDrawOrder + 1 }
			).heapClone()
		}
		, spawnLargePowerNode{
			SpawnProgramUtil::makeSpawnPosVelNode(
				largePowerPrototype,
				SpawnProgramUtil::makeEntityPositionNode(),
				SpawnProgramUtil::makeVelocityFromPolarNode(
					SpawnProgramUtil::makePickupInitSpeedNode(),
					SpawnProgramUtil::makeFloatValueNode(90.0f)
				)
			)
		}
		, largePowerSpawnProgram{ spawnLargePowerNode, 1, false }

		//life
		, lifePrototype{
			EntityBuilder::makePosVelPrototype(
				config::largePickupHitbox,
				PickupCollisions::Source{ components::CollisionCommands::pickup },
				game::PickupType{ components::PickupType::Types::life },
				Outbound{ config::pickupOutbound },
				SpriteInstruction{
					bitmapStoragePointer->get(L"life")->d2dBitmap,
				},
				ScriptProgramList{ pickupScriptNode },
				DrawOrder{ config::pickupDrawOrder + 2 }
			).heapClone()
		}
		, spawnLifeNode{
			SpawnProgramUtil::makeSpawnPosVelNode(
				lifePrototype,
				SpawnProgramUtil::makeEntityPositionNode(),
				SpawnProgramUtil::makeVelocityFromPolarNode(
					SpawnProgramUtil::makePickupInitSpeedNode(),
					SpawnProgramUtil::makeFloatValueNode(90.0f)
				)
			)
		}
		, lifeSpawnProgram{ spawnLifeNode, 1, false }

		//bomb
		, bombPrototype{
			EntityBuilder::makePosVelPrototype(
				config::largePickupHitbox,
				PickupCollisions::Source{ components::CollisionCommands::pickup },
				game::PickupType{ components::PickupType::Types::bomb },
				Outbound{ config::pickupOutbound },
				SpriteInstruction{
					bitmapStoragePointer->get(L"bomb")->d2dBitmap,
				},
				ScriptProgramList{ pickupScriptNode },
				DrawOrder{ config::pickupDrawOrder + 2 }
			).heapClone()
		}
		, spawnBombNode{
			SpawnProgramUtil::makeSpawnPosVelNode(
				bombPrototype,
				SpawnProgramUtil::makeEntityPositionNode(),
				SpawnProgramUtil::makeVelocityFromPolarNode(
					SpawnProgramUtil::makePickupInitSpeedNode(),
					SpawnProgramUtil::makeFloatValueNode(90.0f)
				)
			)
		}
		, bombSpawnProgram{ spawnBombNode, 1, false }

		//clear for max power
		, maxPowerClearScriptNode{
			ScriptProgramUtil::makeTimerNode(
				clearLifetime,
				ScriptProgramUtil::makeRemoveEntityNode()
			)
		}
		, maxPowerClearPrototype{
			EntityBuilder::makeStationaryCollidable(
				Point2{
					config::gameWidth / 2,
					config::gameHeight / 2
				} + config::gameOffset,
				AABB{ config::gameWidth / 2, config::gameHeight / 2},
				PlayerCollisions::Target{ components::CollisionCommands::player },
				ScriptProgramList{ maxPowerClearScriptNode }
			).heapClone()
		}
		, spawnMaxPowerClearNode{
			SpawnProgramUtil::makeSpawnNode(maxPowerClearPrototype)
		}
		, maxPowerClearSpawnProgram{ spawnMaxPowerClearNode, 1, false }
	{

	}
}