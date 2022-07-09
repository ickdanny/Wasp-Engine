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
		, smallPowerPrototype{
			EntityBuilder::makePosVelPrototype(
				smallHitbox,
				PickupCollisions::Source{ components::CollisionCommands::pickup },
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
	{

	}
}