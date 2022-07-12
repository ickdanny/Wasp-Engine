#include "Game/Systems/SpawnPrograms/PickupSpawnPrograms.h"

namespace wasp::game::systems {

	using namespace math;
	using namespace components;

	using ScriptProgramList = game::ScriptProgramList;

	namespace {
		constexpr float smallHitbox{ 8.0f };
		constexpr float largeHitbox{ 12.0f };

		constexpr float finalSpeed{ -2.0f };
		constexpr float deceleration{ -0.07f };
		constexpr float outbound{ -100.0f };
	}

	PickupSpawnPrograms::PickupSpawnPrograms(
		resources::BitmapStorage* bitmapStoragePointer
	)
		: bitmapStoragePointer{ bitmapStoragePointer }

		, pickupScriptNode{
			ScriptProgramUtil::makeShiftSpeedIncrementNode(
				finalSpeed, 
				deceleration
			)
		}

		//small power
		, smallPowerPrototype{
			EntityBuilder::makePosVelPrototype(
				smallHitbox,
				PickupCollisions::Source{ components::CollisionCommands::pickup },
				game::PickupType{ components::PickupType::Types::powerSmall },
				Outbound{ outbound },
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
				SpawnProgramUtil::makeVelocityFromPolarSpawnNode(
					SpawnProgramUtil::makePickupInitSpeedNode(),
					SpawnProgramUtil::makeFloatValueSpawnNode(90.0f)
				)
			)
		}
		, smallPower{
			spawnSmallPowerNode,
			1,
			false
		}

		//large power
		, largePowerPrototype{
			EntityBuilder::makePosVelPrototype(
				smallHitbox,
				PickupCollisions::Source{ components::CollisionCommands::pickup },
				game::PickupType{ components::PickupType::Types::powerLarge },
				Outbound{ outbound },
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
				SpawnProgramUtil::makeVelocityFromPolarSpawnNode(
					SpawnProgramUtil::makePickupInitSpeedNode(),
					SpawnProgramUtil::makeFloatValueSpawnNode(90.0f)
				)
			)
		}
		, largePower{
			spawnLargePowerNode,
			1,
			false
		}

		//life
		, lifePrototype{
			EntityBuilder::makePosVelPrototype(
				smallHitbox,
				PickupCollisions::Source{ components::CollisionCommands::pickup },
				game::PickupType{ components::PickupType::Types::life },
				Outbound{ outbound },
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
				SpawnProgramUtil::makeVelocityFromPolarSpawnNode(
					SpawnProgramUtil::makePickupInitSpeedNode(),
					SpawnProgramUtil::makeFloatValueSpawnNode(90.0f)
				)
			)
		}
		, life{
			spawnLifeNode,
			1,
			false
		}

		//bomb
		, bombPrototype{
			EntityBuilder::makePosVelPrototype(
				smallHitbox,
				PickupCollisions::Source{ components::CollisionCommands::pickup },
				game::PickupType{ components::PickupType::Types::bomb },
				Outbound{ outbound },
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
				SpawnProgramUtil::makeVelocityFromPolarSpawnNode(
					SpawnProgramUtil::makePickupInitSpeedNode(),
					SpawnProgramUtil::makeFloatValueSpawnNode(90.0f)
				)
			)
		}
		, bomb{
			spawnBombNode,
			1,
			false
		}
	{

	}
}