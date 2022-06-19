#include "Game/Systems/SpawnPrograms/PlayerSpawnPrograms.h"

namespace wasp::game::systems {

	using namespace math;
	using namespace components;

	namespace a {
		//small bullet data
		constexpr float smallSpeed{ 8.0f };
		constexpr AABB smallHitbox{ 8.0f };
		constexpr int smallDamage{ 10 };
		constexpr float smallOutbound{ -20.0f };

		constexpr int smallMod0{ 8 };
		constexpr int smallMod1{ 5 };

		constexpr float largeSpeed{ smallSpeed * 0.8f };
		constexpr float largeHitbox{ 14.0f };
		constexpr Angle unfocusedLargeAngle{ 70.0f };
		constexpr Angle focusedLargeAngle{ 80.0f };
	}

	PlayerSpawnPrograms::PlayerSpawnPrograms(
		resources::BitmapStorage* bitmapStoragePointer
	)
		: bitmapStoragePointer{ bitmapStoragePointer }
		, smallBulletPrototype{
			EntityBuilder::makePosPrototype(
				Velocity{ Vector2{ 0.0f, -a::smallSpeed } },
				a::smallHitbox,
				EnemyCollisions::Source{ components::CollisionCommands::death },
				Damage{ a::smallDamage },
				Outbound{ a::smallOutbound },
				SpriteInstruction{
					bitmapStoragePointer->get(L"temp_player")->d2dBitmap
				},
				DrawOrder{ config::playerBulletDrawOrder }
			).heapClone()
		}
		, largeBulletPrototype{
			EntityBuilder::makePosVelPrototype(
				a::largeHitbox
				//todo: large bullet prototype
			).heapClone()
		}
		, spawnSmallBulletNode{
			&(new SpawnNodeData<std::shared_ptr<ComponentTupleBase>>{
				SpawnInstructions::spawnPos,
				smallBulletPrototype
			})->link(
				std::make_shared<SpawnNode>(SpawnInstructions::entityPosition)
			)
		}
		, powerBucket0Node{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, a::smallMod0),
				spawnSmallBulletNode
			)
		}
		, shotA{
			powerBucket0Node, 
			config::playerShotMaxTick, 
			false
		} {
		}
}