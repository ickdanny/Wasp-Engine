#include "Game/Systems/SpawnPrograms/EnemySpawnPrograms.h"

namespace wasp::game::systems {

	namespace {
		constexpr float smallHitbox{ 3.0f };
		constexpr float mediumHitbox{ 6.5f };
		constexpr float largeHitbox{ 12.0f };
		constexpr float sharpHitbox{ 5.0f };
		constexpr float outbound{ -20.0f };

		constexpr float spawnOut{ 20.0f };
		constexpr float topOut{ -spawnOut + config::gameOffset.y };
		constexpr float leftOut{ -spawnOut + config::gameOffset.x };
		constexpr float rightOut{ config::gameWidth + spawnOut + config::gameOffset.x };

		constexpr float enemyOut{ -30.0f };

	#define BASIC_BULLET_ARGS(hitbox, outbound, spriteName, drawOrderOffset) \
		hitbox, \
		PlayerCollisions::Source{ components::CollisionCommands::death }, \
		BulletCollisions::Target{ components::CollisionCommands::death }, \
		Damage{ 1 }, \
		Outbound{ outbound }, \
		SpriteInstruction{ \
			bitmapStoragePointer->get(spriteName)->d2dBitmap \
		}, \
		DrawOrder{ config::enemyBulletDrawOrder + drawOrderOffset } 

	#define SHARP_BULLET_ARGS(hitbox, outbound, spriteName, drawOrderOffset) \
		hitbox, \
		PlayerCollisions::Source{ components::CollisionCommands::death }, \
		BulletCollisions::Target{ components::CollisionCommands::death }, \
		Damage{ 1 }, \
		Outbound{ outbound }, \
		SpriteInstruction{ \
			bitmapStoragePointer->get(spriteName)->d2dBitmap \
		}, \
		DrawOrder{ config::enemyBulletDrawOrder + drawOrderOffset }, \
		RotateSpriteForwardMarker{}

	#define WISP_ARGS(health, deathSpawnNode) \
		Hitbox{ 12.0f }, \
		SpriteInstruction{ \
			bitmapStoragePointer->get(L"wisp")->d2dBitmap \
		}, \
		SpriteSpin{ -1.5f }, \
		PlayerCollisions::Source{}, \
		EnemyCollisions::Target{ components::CollisionCommands::damage }, \
		Health{ health }, \
		Outbound{ enemyOut }, \
		DeathCommand{ DeathCommand::Commands::deathSpawn }, \
		DeathSpawn{ {deathSpawnNode} }, \
		DrawOrder{ config::enemyDrawOrder }


	}	//end of anonymous namespace

	EnemySpawnPrograms::EnemySpawnPrograms(
		resources::BitmapStorage* bitmapStoragePointer
	)
		: bitmapStoragePointer{ bitmapStoragePointer }

		//prototypes

		, smallRedPrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(smallHitbox, outbound, L"small_red", -100)
			).heapClone()
		}
		, smallOrangePrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(smallHitbox, outbound, L"small_orange", -100)
			).heapClone()
		}
		, smallYellowPrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(smallHitbox, outbound, L"small_yellow", -100)
			).heapClone()
		}
		, smallChartreusePrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(smallHitbox, outbound, L"small_chartreuse", -100)
			).heapClone()
		}
		, smallGreenPrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(smallHitbox, outbound, L"small_green", -100)
			).heapClone()
		}
		, smallSpringPrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(smallHitbox, outbound, L"small_spring", -100)
			).heapClone()
		}
		, smallCyanPrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(smallHitbox, outbound, L"small_cyan", -100)
			).heapClone()
		}
		, smallAzurePrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(smallHitbox, outbound, L"small_azure", -100)
			).heapClone()
		}
		, smallBluePrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(smallHitbox, outbound, L"small_blue", -100)
			).heapClone()
		}
		, smallVioletPrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(smallHitbox, outbound, L"small_violet", -100)
			).heapClone()
		}
		, smallMagentaPrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(smallHitbox, outbound, L"small_magenta", -100)
			).heapClone()
		}
		, smallRosePrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(smallHitbox, outbound, L"small_rose", -100)
			).heapClone()
		}
		, mediumRedPrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(mediumHitbox, outbound, L"medium_red", 0)
			).heapClone()
		}
		, mediumOrangePrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(mediumHitbox, outbound, L"medium_orange", 0)
			).heapClone()
		}
		, mediumYellowPrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(mediumHitbox, outbound, L"medium_yellow", 0)
			).heapClone()
		}
		, mediumChartreusePrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(mediumHitbox, outbound, L"medium_chartreuse", 0)
			).heapClone()
		}
		, mediumGreenPrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(mediumHitbox, outbound, L"medium_green", 0)
			).heapClone()
		}
		, mediumSpringPrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(mediumHitbox, outbound, L"medium_spring", 0)
			).heapClone()
		}
		, mediumCyanPrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(mediumHitbox, outbound, L"medium_cyan", 0)
			).heapClone()
		}
		, mediumAzurePrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(mediumHitbox, outbound, L"medium_azure", 0)
			).heapClone()
		}
		, mediumBluePrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(mediumHitbox, outbound, L"medium_blue", 0)
			).heapClone()
		}
		, mediumVioletPrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(mediumHitbox, outbound, L"medium_violet", 0)
			).heapClone()
		}
		, mediumMagentaPrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(mediumHitbox, outbound, L"medium_magenta", 0)
			).heapClone()
		}
		, mediumRosePrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(mediumHitbox, outbound, L"medium_rose", 0)
			).heapClone()
		}
		, largeRedPrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(largeHitbox, outbound, L"large_red", 100)
			).heapClone()
		}
		, largeOrangePrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(largeHitbox, outbound, L"large_orange", 100)
			).heapClone()
		}
		, largeYellowPrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(largeHitbox, outbound, L"large_yellow", 100)
			).heapClone()
		}
		, largeChartreusePrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(largeHitbox, outbound, L"large_chartreuse", 100)
			).heapClone()
		}
		, largeGreenPrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(largeHitbox, outbound, L"large_green", 100)
			).heapClone()
		}
		, largeSpringPrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(largeHitbox, outbound, L"large_spring", 100)
			).heapClone()
		}
		, largeCyanPrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(largeHitbox, outbound, L"large_cyan", 100)
			).heapClone()
		}
		, largeAzurePrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(largeHitbox, outbound, L"large_azure", 100)
			).heapClone()
		}
		, largeBluePrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(largeHitbox, outbound, L"large_blue", 100)
			).heapClone()
		}
		, largeVioletPrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(largeHitbox, outbound, L"large_violet", 100)
			).heapClone()
		}
		, largeMagentaPrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(largeHitbox, outbound, L"large_magenta", 100)
			).heapClone()
		}
		, largeRosePrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(largeHitbox, outbound, L"large_rose", 100)
			).heapClone()
		}
		, sharpRedPrototype{
			EntityBuilder::makePosVelPrototype(
				SHARP_BULLET_ARGS(sharpHitbox, outbound, L"sharp_red", -50)
			).heapClone()
		}
		, sharpOrangePrototype{
			EntityBuilder::makePosVelPrototype(
				SHARP_BULLET_ARGS(sharpHitbox, outbound, L"sharp_orange", -50)
			).heapClone()
		}
		, sharpYellowPrototype{
			EntityBuilder::makePosVelPrototype(
				SHARP_BULLET_ARGS(sharpHitbox, outbound, L"sharp_yellow", -50)
			).heapClone()
		}
		, sharpChartreusePrototype{
			EntityBuilder::makePosVelPrototype(
				SHARP_BULLET_ARGS(sharpHitbox, outbound, L"sharp_chartreuse", -50)
			).heapClone()
		}
		, sharpGreenPrototype{
			EntityBuilder::makePosVelPrototype(
				SHARP_BULLET_ARGS(sharpHitbox, outbound, L"sharp_green", -50)
			).heapClone()
		}
		, sharpSpringPrototype{
			EntityBuilder::makePosVelPrototype(
				SHARP_BULLET_ARGS(sharpHitbox, outbound, L"sharp_spring", -50)
			).heapClone()
		}
		, sharpCyanPrototype{
			EntityBuilder::makePosVelPrototype(
				SHARP_BULLET_ARGS(sharpHitbox, outbound, L"sharp_cyan", -50)
			).heapClone()
		}
		, sharpAzurePrototype{
			EntityBuilder::makePosVelPrototype(
				SHARP_BULLET_ARGS(sharpHitbox, outbound, L"sharp_azure", -50)
			).heapClone()
		}
		, sharpBluePrototype{
			EntityBuilder::makePosVelPrototype(
				SHARP_BULLET_ARGS(sharpHitbox, outbound, L"sharp_blue", -50)
			).heapClone()
		}
		, sharpVioletPrototype{
			EntityBuilder::makePosVelPrototype(
				SHARP_BULLET_ARGS(sharpHitbox, outbound, L"sharp_violet", -50)
			).heapClone()
		}
		, sharpMagentaPrototype{
			EntityBuilder::makePosVelPrototype(
				SHARP_BULLET_ARGS(sharpHitbox, outbound, L"sharp_magenta", -50)
			).heapClone()
		}
		, sharpRosePrototype{
			EntityBuilder::makePosVelPrototype(
				SHARP_BULLET_ARGS(sharpHitbox, outbound, L"sharp_rose", -50)
			).heapClone()
		}

		// STAGE 1 // STAGE 1 // STAGE 1 // STAGE 1 // STAGE 1 // STAGE 1 // STAGE 1 //

		, s1d1Node{
			SpawnProgramUtil::makeRingFormationNode(
				SpawnProgramUtil::makeVelocityFromPolarNode(
					SpawnProgramUtil::makeFloatValueNode(1.0f),
					SpawnProgramUtil::makeAngleToPlayerNode()
				),
				18,
				SpawnProgramUtil::makeSpawnPosVelNode(
					sharpCyanPrototype,
					SpawnProgramUtil::makeEntityPositionNode()
				)
			)
		}
		, s1d1{ s1d1Node, 1, false }
		, s1e1Prototype{
			EntityBuilder::makeLinearCollidable(
				{50.0f, topOut}, 
				{1.0f, -90.0f}, 
				WISP_ARGS(100, s1d1),
				ScriptProgramList{
					ScriptProgramUtil::makeShootOnceAndLeaveTurningProgram(
						50,
						50,
						20,
						s1d1,
						50,
						{ 1.0f, -45.0f},
						-90.0f,
						100
					)
				}
			).heapClone()
		}
		, s1e1Node{ SpawnProgramUtil::makeSpawnNode(s1e1Prototype) }
		, s1e1{ s1e1Node, 1, false }
		

		{
		}
}