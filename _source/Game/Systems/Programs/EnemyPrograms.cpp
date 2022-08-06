#include "Game/Systems/Programs/EnemyPrograms.h"

namespace wasp::game::systems {

	namespace {
		constexpr float smallHitbox{ 3.0f };
		constexpr float mediumHitbox{ 6.15f };
		constexpr float largeHitbox{ 10.0f };
		constexpr float sharpHitbox{ 3.75f };
		constexpr float outbound{ -21.0f };

		constexpr float enemyOut{ -30.0f };

		constexpr int fieldClearLifetime{ 30 };

		constexpr int waitEndTime{ 60 };

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
		SpriteSpin{ -2.25f }, \
		PlayerCollisions::Source{}, \
		EnemyCollisions::Target{ components::CollisionCommands::damage }, \
		Health{ health }, \
		Outbound{ enemyOut }, \
		DeathCommand{ DeathCommand::Commands::deathSpawn }, \
		DeathSpawn{ {deathSpawnNode} }, \
		DrawOrder{ config::enemyDrawOrder }

	#define BAT_ARGS(health, deathSpawnNode) \
		Hitbox{ 11.0f }, \
		SpriteInstruction{ \
			bitmapStoragePointer->get(L"bat_1")->d2dBitmap \
		}, \
		AnimationList{ { L"bat_1", L"bat_2" }, 10 }, \
		RotateSpriteForwardMarker{}, \
		PlayerCollisions::Source{}, \
		EnemyCollisions::Target{ components::CollisionCommands::damage }, \
		Health{ health }, \
		Outbound{ enemyOut }, \
		DeathCommand{ DeathCommand::Commands::deathSpawn }, \
		DeathSpawn{ {deathSpawnNode} }, \
		DrawOrder{ config::enemyDrawOrder + 1 }

	#define FLAME_ARGS(health, deathSpawnNode) \
		Hitbox{ 9.0f }, \
		SpriteInstruction{ \
			bitmapStoragePointer->get(L"flame_1")->d2dBitmap \
		}, \
		AnimationList{ { L"flame_1", L"flame_2", L"flame_3", L"flame_4" }, 6 }, \
		PlayerCollisions::Source{}, \
		EnemyCollisions::Target{ components::CollisionCommands::damage }, \
		Health{ health }, \
		Outbound{ enemyOut }, \
		DeathCommand{ DeathCommand::Commands::deathSpawn }, \
		DeathSpawn{ {deathSpawnNode} }, \
		DrawOrder{ config::enemyDrawOrder + 2 }

	#define SHELL_ARGS(health, deathSpawnNode) \
		Hitbox{ 10.0f }, \
		SpriteInstruction{ \
			bitmapStoragePointer->get(L"shell")->d2dBitmap \
		}, \
		RotateSpriteForwardMarker{}, \
		PlayerCollisions::Source{}, \
		EnemyCollisions::Target{ components::CollisionCommands::damage }, \
		Health{ health }, \
		Outbound{ enemyOut }, \
		DeathCommand{ DeathCommand::Commands::deathSpawn }, \
		DeathSpawn{ {deathSpawnNode} }, \
		DrawOrder{ config::enemyDrawOrder + 1 }

	#define CLOUD_ARGS(health, deathSpawnNode) \
		Hitbox{ 9.0f, 6.5f }, \
		SpriteInstruction{ \
			bitmapStoragePointer->get(L"cloud_1")->d2dBitmap \
		}, \
		AnimationList{ { L"cloud_1", L"cloud_2", L"cloud_3" }, 8 }, \
		PlayerCollisions::Source{}, \
		EnemyCollisions::Target{ components::CollisionCommands::damage }, \
		Health{ health }, \
		Outbound{ enemyOut }, \
		DeathCommand{ DeathCommand::Commands::deathSpawn }, \
		DeathSpawn{ {deathSpawnNode} }, \
		DrawOrder{ config::enemyDrawOrder + 3 }

	#define CRYSTAL_RED_ARGS(health, deathSpawnNode) \
		Hitbox{ 5.0f, 11.5f }, \
		SpriteInstruction{ \
			bitmapStoragePointer->get(L"crystal_red_1")->d2dBitmap \
		}, \
		AnimationList{ { L"crystal_red_1", L"crystal_red_2", L"crystal_red_3" }, 5 }, \
		PlayerCollisions::Source{}, \
		EnemyCollisions::Target{ components::CollisionCommands::damage }, \
		Health{ health }, \
		Outbound{ enemyOut }, \
		DeathCommand{ DeathCommand::Commands::deathSpawn }, \
		DeathSpawn{ {deathSpawnNode} }, \
		DrawOrder{ config::enemyDrawOrder + 4 }

	#define CRYSTAL_GREEN_ARGS(health, deathSpawnNode) \
		Hitbox{ 5.0f, 11.5f }, \
		SpriteInstruction{ \
			bitmapStoragePointer->get(L"crystal_green_1")->d2dBitmap \
		}, \
		AnimationList{ { L"crystal_green_1", L"crystal_green_2", \
			L"crystal_green_3" }, 5 }, \
		PlayerCollisions::Source{}, \
		EnemyCollisions::Target{ components::CollisionCommands::damage }, \
		Health{ health }, \
		Outbound{ enemyOut }, \
		DeathCommand{ DeathCommand::Commands::deathSpawn }, \
		DeathSpawn{ {deathSpawnNode} }, \
		DrawOrder{ config::enemyDrawOrder + 4 }

	#define CRYSTAL_BLUE_ARGS(health, deathSpawnNode) \
		Hitbox{ 5.0f, 11.5f }, \
		SpriteInstruction{ \
			bitmapStoragePointer->get(L"crystal_blue_1")->d2dBitmap \
		}, \
		AnimationList{ { L"crystal_blue_1", L"crystal_blue_2", \
			L"crystal_blue_3" }, 5 }, \
		PlayerCollisions::Source{}, \
		EnemyCollisions::Target{ components::CollisionCommands::damage }, \
		Health{ health }, \
		Outbound{ enemyOut }, \
		DeathCommand{ DeathCommand::Commands::deathSpawn }, \
		DeathSpawn{ {deathSpawnNode} }, \
		DrawOrder{ config::enemyDrawOrder + 4 }

	//remember to use makeLinearUncollidable()
	#define BOSS_ARGS \
		Position{ (config::gameWidth / 2.0f) + config::gameOffset.x, config::topOut }, \
		Velocity{ math::Vector2{ 0.0f, config::bossSpeed } }, \
		Hitbox{ config::bossHitbox }, \
		PlayerCollisions::Source{}, \
		EnemyCollisions::Target{ components::CollisionCommands::damage }, \
		Health{ 10000 }, \
		SpawnProgramList{}, \
		DeathCommand{ DeathCommand::Commands::bossDeath }, \
		DeathSpawn{ { fieldClearProgram } }, \
		DrawOrder{ config::enemyDrawOrder + 10 }



	}	//end of anonymous namespace

	EnemyPrograms::EnemyPrograms(
		resources::BitmapStorage* bitmapStoragePointer,
		PickupPrograms const* pickupProgramsPointer
	)
		: bitmapStoragePointer{ bitmapStoragePointer }
		, pickupProgramsPointer{ pickupProgramsPointer }

		//prototypes

		, smallRedPrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(smallHitbox, outbound, L"small_red", 100)
			).heapClone()
		}
		, smallOrangePrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(smallHitbox, outbound, L"small_orange", 100)
			).heapClone()
		}
		, smallYellowPrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(smallHitbox, outbound, L"small_yellow", 100)
			).heapClone()
		}
		, smallChartreusePrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(smallHitbox, outbound, L"small_chartreuse", 100)
			).heapClone()
		}
		, smallGreenPrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(smallHitbox, outbound, L"small_green", 100)
			).heapClone()
		}
		, smallSpringPrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(smallHitbox, outbound, L"small_spring", 100)
			).heapClone()
		}
		, smallCyanPrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(smallHitbox, outbound, L"small_cyan", 100)
			).heapClone()
		}
		, smallAzurePrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(smallHitbox, outbound, L"small_azure", 100)
			).heapClone()
		}
		, smallBluePrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(smallHitbox, outbound, L"small_blue", 100)
			).heapClone()
		}
		, smallVioletPrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(smallHitbox, outbound, L"small_violet", 100)
			).heapClone()
		}
		, smallMagentaPrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(smallHitbox, outbound, L"small_magenta", 100)
			).heapClone()
		}
		, smallRosePrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(smallHitbox, outbound, L"small_rose", 100)
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
				BASIC_BULLET_ARGS(largeHitbox, outbound, L"large_red", -100)
			).heapClone()
		}
		, largeOrangePrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(largeHitbox, outbound, L"large_orange", -100)
			).heapClone()
		}
		, largeYellowPrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(largeHitbox, outbound, L"large_yellow", -100)
			).heapClone()
		}
		, largeChartreusePrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(largeHitbox, outbound, L"large_chartreuse", -100)
			).heapClone()
		}
		, largeGreenPrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(largeHitbox, outbound, L"large_green", -100)
			).heapClone()
		}
		, largeSpringPrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(largeHitbox, outbound, L"large_spring", -100)
			).heapClone()
		}
		, largeCyanPrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(largeHitbox, outbound, L"large_cyan", -100)
			).heapClone()
		}
		, largeAzurePrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(largeHitbox, outbound, L"large_azure", -100)
			).heapClone()
		}
		, largeBluePrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(largeHitbox, outbound, L"large_blue", -100)
			).heapClone()
		}
		, largeVioletPrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(largeHitbox, outbound, L"large_violet", -100)
			).heapClone()
		}
		, largeMagentaPrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(largeHitbox, outbound, L"large_magenta", -100)
			).heapClone()
		}
		, largeRosePrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(largeHitbox, outbound, L"large_rose", -100)
			).heapClone()
		}
		, sharpRedPrototype{
			EntityBuilder::makePosVelPrototype(
				SHARP_BULLET_ARGS(sharpHitbox, outbound, L"sharp_red", 50)
			).heapClone()
		}
		, sharpOrangePrototype{
			EntityBuilder::makePosVelPrototype(
				SHARP_BULLET_ARGS(sharpHitbox, outbound, L"sharp_orange", 50)
			).heapClone()
		}
		, sharpYellowPrototype{
			EntityBuilder::makePosVelPrototype(
				SHARP_BULLET_ARGS(sharpHitbox, outbound, L"sharp_yellow", 50)
			).heapClone()
		}
		, sharpChartreusePrototype{
			EntityBuilder::makePosVelPrototype(
				SHARP_BULLET_ARGS(sharpHitbox, outbound, L"sharp_chartreuse", 50)
			).heapClone()
		}
		, sharpGreenPrototype{
			EntityBuilder::makePosVelPrototype(
				SHARP_BULLET_ARGS(sharpHitbox, outbound, L"sharp_green", 50)
			).heapClone()
		}
		, sharpSpringPrototype{
			EntityBuilder::makePosVelPrototype(
				SHARP_BULLET_ARGS(sharpHitbox, outbound, L"sharp_spring", 50)
			).heapClone()
		}
		, sharpCyanPrototype{
			EntityBuilder::makePosVelPrototype(
				SHARP_BULLET_ARGS(sharpHitbox, outbound, L"sharp_cyan", 50)
			).heapClone()
		}
		, sharpAzurePrototype{
			EntityBuilder::makePosVelPrototype(
				SHARP_BULLET_ARGS(sharpHitbox, outbound, L"sharp_azure", 50)
			).heapClone()
		}
		, sharpBluePrototype{
			EntityBuilder::makePosVelPrototype(
				SHARP_BULLET_ARGS(sharpHitbox, outbound, L"sharp_blue", 50)
			).heapClone()
		}
		, sharpVioletPrototype{
			EntityBuilder::makePosVelPrototype(
				SHARP_BULLET_ARGS(sharpHitbox, outbound, L"sharp_violet", 50)
			).heapClone()
		}
		, sharpMagentaPrototype{
			EntityBuilder::makePosVelPrototype(
				SHARP_BULLET_ARGS(sharpHitbox, outbound, L"sharp_magenta", 50)
			).heapClone()
		}
		, sharpRosePrototype{
			EntityBuilder::makePosVelPrototype(
				SHARP_BULLET_ARGS(sharpHitbox, outbound, L"sharp_rose", 50)
			).heapClone()
		}

		//field clear
		, fieldClearScriptNode{
			ScriptProgramUtil::makeTimerNode(
				fieldClearLifetime,
				ScriptProgramUtil::makeRemoveEntityNode()
			)
		}
		, fieldClearPrototype{
			EntityBuilder::makeStationaryCollidable(
				Point2{
					config::gameWidth / 2,
					config::gameHeight / 2
				} + config::gameOffset,
				AABB{ (config::gameWidth / 2) + 20.0f, (config::gameHeight / 2) + 20.0f },
				PlayerCollisions::Target{ components::CollisionCommands::none },
				ScriptProgramList{ fieldClearScriptNode }
			).heapClone()
		}
		, fieldClearNode{
			SpawnProgramUtil::makeSpawnNode(fieldClearPrototype)
		}
		, fieldClearProgram{ fieldClearNode, 1, false }

		// STAGE 1 // STAGE 1 // STAGE 1 // STAGE 1 // STAGE 1 // STAGE 1 // STAGE 1 //

			//wave 1
		, s1d1Node{
			SpawnProgramUtil::makeRingFormationNode(
				SpawnProgramUtil::makeVelocityFromPolarNode(
					SpawnProgramUtil::makeFloatValueNode(1.0f),
					SpawnProgramUtil::makeAngleToPlayerNode()
				),
				{ 6, 12, 18, 24 },
				SpawnProgramUtil::makeSpawnPosVelNode(
					sharpCyanPrototype,
					SpawnProgramUtil::makeEntityPositionNode()
				)
			)
		}
		, s1d1{ s1d1Node, 1, false }
		, s1e1aPrototype{
			EntityBuilder::makeLinearCollidable(
				{40.0f + config::gameOffset.x, config::topOut}, 
				{1.5f, -90.0f}, 
				WISP_ARGS(130, pickupProgramsPointer->smallPowerSpawnProgram),
				ScriptProgramList{
					ScriptProgramUtil::makeShootOnceAndLeaveTurningProgram(
						20,
						25,
						5,
						s1d1,
						20,
						{ 1.5f, -25.0f},
						-90.0f,
						100
					)
				}
			).heapClone()
		}
		, s1e1aNode{ SpawnProgramUtil::makeSpawnNode(s1e1aPrototype) }
		, s1e1a{ s1e1aNode, 1, false }
		, s1e1bPrototype{
			EntityBuilder::makeLinearCollidable(
				{config::gameWidth - 40.0f + config::gameOffset.x, config::topOut}, 
				{1.5f, -90.0f}, 
				WISP_ARGS(130, pickupProgramsPointer->smallPowerSpawnProgram),
				ScriptProgramList{
					ScriptProgramUtil::makeShootOnceAndLeaveTurningProgram(
						20,
						25,
						5,
						s1d1,
						20,
						{ 1.5f, 180.0f + 25.0f },
						-90.0f,
						100
					)
				}
			).heapClone()
		}
		, s1e1bNode{ SpawnProgramUtil::makeSpawnNode(s1e1bPrototype) }
		, s1e1b{ s1e1bNode, 1, false }
			//wave 2
		, s1d2aNode{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(
					SpawnProgramUtil::makeEntityUniformRandomIntNode(
						SpawnProgramUtil::makeIntValueNode(0),
						SpawnProgramUtil::makeIntValueDiffNode({ 170, 130, 100, 70 })
					),
					{ 170, 130, 100, 70 }
				),
				SpawnProgramUtil::makeSpawnPosVelNode(
					smallAzurePrototype,
					SpawnProgramUtil::makeEntityPositionNode(),
					SpawnProgramUtil::makeVelocityFromPolarNode(
						SpawnProgramUtil::makeFloatValueDiffNode(
							{1.5f, 1.8f, 2.1f, 2.5f}
						),
						SpawnProgramUtil::makeAngleToPlayerNode()
					)
				)
			)
		}
		, s1d2a{ s1d2aNode, 1000, true }
		, s1e2aPrototype{
			EntityBuilder::makePosVelPrototype(
				BAT_ARGS(20, pickupProgramsPointer->smallPowerThirdSpawnProgram),
				SpawnProgramList{ s1d2a }
			).heapClone()
		}
		, s1e2aNode{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, 14),
				SpawnProgramUtil::makeSpawnPosVelNode(
					s1e2aPrototype,
					SpawnProgramUtil::makePointValueNode({ config::leftOut, 40.0f }),
					SpawnProgramUtil::makeVelocityValueNode(Velocity{ 2.0f, -15.0f })
				)
			)
		}
		, s1e2a{ s1e2aNode, 290, false }

		, s1e2bPrototype{
			EntityBuilder::makeLinearCollidable(
				{config::gameWidth / 2 + config::gameOffset.x, config::topOut}, 
				{1.5f, -90.0f}, 
				WISP_ARGS(130, pickupProgramsPointer->smallPowerSpawnProgram),
				ScriptProgramList{
					ScriptProgramUtil::makeShootOnceAndLeaveProgram(
						20,
						25,
						5,
						s1d1,
						20,
						{ 1.5f, -90.0f},
						100
					)
				}
			).heapClone()
		}
		, s1e2bNode{ SpawnProgramUtil::makeSpawnNode(s1e2bPrototype) }
		, s1e2b{ s1e2bNode, 1, false }

			//wave 3
		, s1d3aNode{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, {12, 6, 3, 2}),
				SpawnProgramUtil::makeSpawnPosVelNode(
					sharpSpringPrototype,
					SpawnProgramUtil::makeEntityPositionNode(),
					SpawnProgramUtil::makeVelocityFromPolarNode(
						SpawnProgramUtil::makeFloatValueDiffNode(
							{ 1.8f, 1.9f, 2.0f, 2.1f}
						),
						SpawnProgramUtil::makeSpiralNode(
							SpawnProgramUtil::makeIntValueNode(2400),
							SpawnProgramUtil::entityRandomAngleNode,
							SpawnProgramUtil::makeFloatValueDiffNode(
								{ 5.5f, 6.0f, 6.5f, 7.0f }
							)
						)						
					)
				)
			)
		}
		, s1d3bNode{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, {24, 12, 6, 3}),
				SpawnProgramUtil::makeSpawnPosVelNode(
					sharpVioletPrototype,
					SpawnProgramUtil::makeEntityPositionNode(),
					SpawnProgramUtil::makeVelocityFromPolarNode(
						SpawnProgramUtil::makeFloatValueDiffNode(
							{ 1.4f, 1.5f, 1.6f, 1.7f}
						),
						SpawnProgramUtil::makeSpiralNode(
							SpawnProgramUtil::makeIntValueNode(2400),
							SpawnProgramUtil::entityRandomAngleNode,
							SpawnProgramUtil::makeFloatValueDiffNode(
								{ -5.5f, -6.0f, -6.5f, -7.0f }
							)
						)
					)
				)
			)
		}
		, s1d3a{ s1d3aNode, 2400, false }
		, s1d3b{ s1d3bNode, 2400, false }
		, s1e3aPrototype{
			EntityBuilder::makeLinearCollidable(
				{ config::rightOut, config::topOut },
				{ 1.5f, -135.0f },
				WISP_ARGS(220, pickupProgramsPointer->smallPowerSpawnProgram),
				ScriptProgramList{
					ScriptProgramUtil::makeTurnEntryAndShootProgram(
						20,
						45,
						{ 0.5f, 192.0f},
						-135.0f,
						s1d3a,
						ScriptProgramUtil::makeStallingIfNode(
							ScriptProgramUtil::makeBoundaryXLowNode(config::gameOffset.x),
							ScriptProgramUtil::makeClearSpawnNode()
						)
					)
				}
			).heapClone()
		}
		, s1e3bPrototype{
			EntityBuilder::makeLinearCollidable(
				{ config::rightOut, 15.0f },
				{ 1.5f, -145.0f },
				WISP_ARGS(220, pickupProgramsPointer->smallPowerSpawnProgram),
				ScriptProgramList{
					ScriptProgramUtil::makeTurnEntryAndShootProgram(
						20,
						45,
						{ 0.5f, 198.0f},
						-145.0f,
						s1d3b,
						ScriptProgramUtil::makeStallingIfNode(
							ScriptProgramUtil::makeBoundaryXLowNode(config::gameOffset.x),
							ScriptProgramUtil::makeClearSpawnNode()
						)
					)
				}
			).heapClone()
		}
		, s1e3Node{
			SpawnProgramUtil::makeListNode(
				SpawnProgramUtil::makeSpawnNode(s1e3aPrototype),
				SpawnProgramUtil::makeSpawnNode(s1e3bPrototype)
			)
		}
		, s1e3{ s1e3Node, 1, false }

			//wave 4
		, s1d4Node{
			SpawnProgramUtil::makeRingFormationNode(
				SpawnProgramUtil::makeVelocityFromPolarNode(
					SpawnProgramUtil::makeFloatValueNode(1.0f),
					SpawnProgramUtil::makeAngleToPlayerNode()
				),
				{ 6, 12, 18, 24 },
				SpawnProgramUtil::makeColumnFormationNode(
					{ 1, 1, 2, 2 },
					-0.3f,
					SpawnProgramUtil::makeSpawnPosVelNode(
						sharpCyanPrototype,
						SpawnProgramUtil::makeEntityPositionNode()
					)
				)
			)
		}
		, s1d4{ s1d4Node, 1, false }
		, s1e4aPrototype{
			EntityBuilder::makeLinearCollidable(
				{ config::gameWidth - 40.0f + config::gameOffset.x, config::topOut }, 
				{ 1.5f, -90.0f }, 
				WISP_ARGS(130, pickupProgramsPointer->smallPowerSpawnProgram),
				ScriptProgramList{
					ScriptProgramUtil::makeShootOnceAndLeaveTurningProgram(
						20,
						25,
						5,
						s1d4,
						20,
						{ 1.5f, 180.0f + 25.0f },
						-90.0f,
						100
					)
				}
			).heapClone()
		}
		, s1e4aNode{ SpawnProgramUtil::makeSpawnNode(s1e4aPrototype) }
		, s1e4a{ s1e4aNode, 1, false }
		, s1e4bPrototype{
			EntityBuilder::makeLinearCollidable(
				{40.0f + config::gameOffset.x, config::topOut}, 
				{1.5f, -90.0f}, 
				WISP_ARGS(130, pickupProgramsPointer->smallPowerSpawnProgram),
				ScriptProgramList{
					ScriptProgramUtil::makeShootOnceAndLeaveTurningProgram(
						20,
						25,
						5,
						s1d4,
						20,
						{ 1.5f, -25.0f},
						-90.0f,
						100
					)
				}
			).heapClone()
		}
		, s1e4bNode{ SpawnProgramUtil::makeSpawnNode(s1e4bPrototype) }
		, s1e4b{ s1e4bNode, 1, false }

			//wave 5
		, s1d5Node{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, {12, 6, 3, 2}),
				SpawnProgramUtil::makeSpawnPosVelNode(
					sharpBluePrototype,
					SpawnProgramUtil::makeEntityPositionNode(),
					SpawnProgramUtil::makeVelocityFromPolarNode(
						SpawnProgramUtil::makeFloatValueDiffNode(
							{ 1.8f, 1.9f, 2.0f, 2.1f}
						),
						SpawnProgramUtil::makeSpiralNode(
							SpawnProgramUtil::makeIntValueNode(2400),
							SpawnProgramUtil::entityRandomAngleNode,
							SpawnProgramUtil::makeFloatValueDiffNode(
								{ 5.5f, 6.0f, 6.5f, 7.0f }
							)
						)
					)
				)
			)
		}
		, s1d5{ s1d5Node, 2400, false }
		, s1e5Prototype{
			EntityBuilder::makeLinearCollidable(
				{ config::leftOut, config::topOut },
				{ 1.5f, -45.0f },
				WISP_ARGS(220, pickupProgramsPointer->smallPowerSpawnProgram),
				ScriptProgramList{
					ScriptProgramUtil::makeTurnEntryAndShootProgram(
						20,
						45,
						{ 0.5f, -15.0f},
						-45.0f,
						s1d5,
						ScriptProgramUtil::makeStallingIfNode(
							ScriptProgramUtil::makeBoundaryXHighNode(
								config::gameOffset.x
							),
							ScriptProgramUtil::makeClearSpawnNode()
						)
					)
				}
			).heapClone()
		}
		, s1e5Node{ SpawnProgramUtil::makeSpawnNode(s1e5Prototype) }
		, s1e5{ s1e5Node, 1, false }

			//wave 6
		, s1e6Node{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, 14),
				SpawnProgramUtil::makeSpawnPosVelNode(
					s1e2aPrototype,
					SpawnProgramUtil::makePointValueNode({ config::rightOut, 40.0f }),
					SpawnProgramUtil::makeVelocityValueNode(Velocity{ 2.0f, 195.0f })
				)
			)
		}
		, s1e6{ s1e6Node, 290, false }

			//wave 7
		, s1d7Node{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(-1, 84),
				s1d1Node
			)
		}
		, s1d7{ s1d7Node, 85, false }
		, s1e7aPrototype{
			EntityBuilder::makeLinearCollidable(
				{ 40.0f + config::gameOffset.x, config::topOut },
				{ 1.5f, -90.0f },
				WISP_ARGS(160, pickupProgramsPointer->smallPowerSpawnProgram),
				ScriptProgramList{
					ScriptProgramUtil::makeShootOnceAndLeaveTurningProgram(
						20,
						25,
						5,
						s1d7,
						20,
						{ 1.5f, -25.0f},
						-90.0f,
						100
					)
				}
			).heapClone()
		}
		, s1e7bPrototype{
			EntityBuilder::makeLinearCollidable(
				{ config::gameWidth - 40.0f + config::gameOffset.x, config::topOut },
				{ 1.5f, -90.0f },
				WISP_ARGS(160, pickupProgramsPointer->smallPowerSpawnProgram),
				ScriptProgramList{
					ScriptProgramUtil::makeShootOnceAndLeaveTurningProgram(
						20,
						25,
						5,
						s1d7,
						20,
						{ 1.5f, 180.0f + 25.0f },
						-90.0f,
						100
					)
				}
			).heapClone()
		}
		, s1e7Node{
			SpawnProgramUtil::makeListNode(
				SpawnProgramUtil::makeSpawnNode(s1e7aPrototype),
				SpawnProgramUtil::makeSpawnNode(s1e7bPrototype)
			)
		}
		, s1e7{ s1e7Node, 1, false }

			//wave 8
		, s1d8Node{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, {24, 18, 12, 8}),
				SpawnProgramUtil::makeArcFormationNode(
					SpawnProgramUtil::makeVelocityFromPolarNode(
						SpawnProgramUtil::makeFloatValueDiffNode(
							{ 1.6f, 1.7f, 1.8f, 1.9f}
						),
						SpawnProgramUtil::makeSpiralNode(
							SpawnProgramUtil::makeIntValueNode(2400),
							SpawnProgramUtil::entityRandomAngleNode,
							SpawnProgramUtil::makeFloatValueDiffNode(
								{ 4.5f, 5.5f, 6.5f, 7.0f }
							)
						)
					),
					SpawnProgramUtil::makeIntValueDiffNode({ 1, 2, 2, 3 }),
					SpawnProgramUtil::makeFloatValueNode(8.5f),
					SpawnProgramUtil::makeColumnFormationNode(
						2, 
						-0.8f,
						SpawnProgramUtil::makeSpawnPosVelNode(
							mediumBluePrototype,
							SpawnProgramUtil::makeEntityPositionNode()
						)
					)
				)
			)
		}
		, s1d8{ s1d8Node, 2400, false }
		, s1e8Prototype{
			EntityBuilder::makeLinearCollidable(
				{ config::rightOut, config::topOut },
				{ 1.5f, -135.0f },
				WISP_ARGS(10000, pickupProgramsPointer->bombSpawnProgram),
				ScriptProgramList{
					ScriptProgramUtil::makeTurnEntryAndShootProgram(
						20,
						45,
						{ 0.5f, 192.0f},
						-135.0f,
						s1d8,
						ScriptProgramUtil::makeSetHealthNode(
							380,
							ScriptProgramUtil::makeStallingIfNode(
								ScriptProgramUtil::makeBoundaryXLowNode(
									config::gameOffset.x
								),
								ScriptProgramUtil::makeClearSpawnNode()
							)
						)
					)
				}
			).heapClone()
		}
		, s1e8Node{ SpawnProgramUtil::makeSpawnNode(s1e8Prototype)}
		, s1e8{ s1e8Node, 1, false }
			//wave 9
		, s1e9aPrototype{
			EntityBuilder::makePosVelPrototype(
				BAT_ARGS(10000, pickupProgramsPointer->smallPowerFourthSpawnProgram),
				SpawnProgramList{ s1d2a },
				ScriptProgramList{
					ScriptProgramUtil::makeStallingIfNode(
						ScriptProgramUtil::makeBoundaryYHighNode(10),
						ScriptProgramUtil::makeSetHealthNode(
							20,
							ScriptProgramUtil::makeStallingIfNode(
								ScriptProgramUtil::makeBoundaryYHighNode(50),
								ScriptProgramUtil::makeShiftAnglePeriodNode(
									Angle{ -165.0f },
									-90.0f,
									40
								)
							)
						)
					)
				}
			).heapClone()
		}
		, s1e9bPrototype{
			EntityBuilder::makePosVelPrototype(
				BAT_ARGS(10000, pickupProgramsPointer->smallPowerFourthSpawnProgram),
				SpawnProgramList{ s1d2a },
				ScriptProgramList{
					ScriptProgramUtil::makeStallingIfNode(
						ScriptProgramUtil::makeBoundaryYHighNode(10),
						ScriptProgramUtil::makeSetHealthNode(
							20,
							ScriptProgramUtil::makeStallingIfNode(
								ScriptProgramUtil::makeBoundaryYHighNode(50),
								ScriptProgramUtil::makeShiftAnglePeriodNode(
									Angle{ -165.0f }.flipY(),
									-90.0f,
									40
								)
							)
						)
					)
				}
			).heapClone()
		}
		, s1e9Node{
			SpawnProgramUtil::makeListNode(
				SpawnProgramUtil::makeIfNode(
					SpawnProgramUtil::makeTickModNode(0, 20),
					SpawnProgramUtil::makeSpawnPosVelNode(
						s1e9aPrototype,
						SpawnProgramUtil::makePointValueNode(
							{ 60.0f + config::gameOffset.x, config::topOut }
						),
						SpawnProgramUtil::makeVelocityValueNode(
							Vector2{ 0, 2.0f }
						)
					)
				),
				SpawnProgramUtil::makeIfNode(
					SpawnProgramUtil::makeTickModNode(10, 20),
					SpawnProgramUtil::makeSpawnPosVelNode(
						s1e9bPrototype,
						SpawnProgramUtil::makePointValueNode({ 
							config::gameWidth - 60.0f + config::gameOffset.x, 
							config::topOut 
						}),
						SpawnProgramUtil::makeVelocityValueNode(
							Vector2{ 0, 2.0f }
						)
					)
				)
			)
		}
		, s1e9{ s1e9Node, 290, false }

			//wave 10
		, s1d10aNode{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, {12, 6, 3, 2}),
				SpawnProgramUtil::makeSpawnPosVelNode(
					mediumRosePrototype,
					SpawnProgramUtil::makeEntityPositionNode(),
					SpawnProgramUtil::makeVelocityFromPolarNode(
						SpawnProgramUtil::makeFloatValueDiffNode(
							{ 1.5f, 1.6f, 1.7f, 1.8f}
						),
						SpawnProgramUtil::makeSpiralNode(
							SpawnProgramUtil::makeIntValueNode(2400),
							SpawnProgramUtil::entityRandomAngleNode,
							SpawnProgramUtil::makeFloatValueDiffNode(
								{ 5.5f, 6.0f, 6.5f, 7.0f }
							)
						)						
					)
				)
			)
		}
		, s1d10bNode{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, {24, 12, 6, 3}),
				SpawnProgramUtil::makeSpawnPosVelNode(
					mediumMagentaPrototype,
					SpawnProgramUtil::makeEntityPositionNode(),
					SpawnProgramUtil::makeVelocityFromPolarNode(
						SpawnProgramUtil::makeFloatValueDiffNode(
							{ 1.2f, 1.3f, 1.4f, 1.5f}
						),
						SpawnProgramUtil::makeSpiralNode(
							SpawnProgramUtil::makeIntValueNode(2400),
							SpawnProgramUtil::entityRandomAngleNode,
							SpawnProgramUtil::makeFloatValueDiffNode(
								{ -5.5f, -6.0f, -6.5f, -7.0f }
							)
						)
					)
				)
			)
		}
		, s1d10cNode{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, {24, 12, 6, 3}),
				SpawnProgramUtil::makeSpawnPosVelNode(
					mediumBluePrototype,
					SpawnProgramUtil::makeEntityPositionNode(),
					SpawnProgramUtil::makeVelocityFromPolarNode(
						SpawnProgramUtil::makeFloatValueDiffNode(
							{ 0.8f, 0.9f, 1.0f, 1.1f}
						),
						SpawnProgramUtil::makeSpiralNode(
							SpawnProgramUtil::makeIntValueNode(2400),
							SpawnProgramUtil::entityRandomAngleNode,
							SpawnProgramUtil::makeFloatValueDiffNode(
								{ 5.5f, 6.0f, 6.5f, 7.0f }
							)
						)
					)
				)
			)
		}
		, s1d10a{ s1d10aNode, 2400, false }
		, s1d10b{ s1d10bNode, 2400, false }
		, s1d10c{ s1d10cNode, 2400, false }
		, s1e10aPrototype{
			EntityBuilder::makeLinearCollidable(
				{ config::leftOut, config::topOut },
				{ 1.5f, Angle{ -135.0f }.flipY() },
				WISP_ARGS(300, pickupProgramsPointer->smallPowerSpawnProgram),
				ScriptProgramList{
					ScriptProgramUtil::makeTurnEntryAndShootProgram(
						20,
						45,
						{ 0.5f, Angle{ 188.0f }.flipY() },
						Angle{ -135.0f }.flipY(),
						s1d10a,
						ScriptProgramUtil::makeStallingIfNode(
							ScriptProgramUtil::makeBoundaryXHighNode(
								config::gameWidth + config::gameOffset.x
							),
							ScriptProgramUtil::makeClearSpawnNode()
						)
					)
				}
			).heapClone()
		}
		, s1e10bPrototype{
			EntityBuilder::makeLinearCollidable(
				{ config::leftOut, 15.0f },
				{ 1.5f, Angle{ -140.0f }.flipY() },
				WISP_ARGS(300, pickupProgramsPointer->smallPowerSpawnProgram),
				ScriptProgramList{
					ScriptProgramUtil::makeTurnEntryAndShootProgram(
						20,
						45,
						{ 0.5f, Angle{ 192.0f }.flipY() },
						Angle{ -140.0f }.flipY(),
						s1d10b,
						ScriptProgramUtil::makeStallingIfNode(
							ScriptProgramUtil::makeBoundaryXHighNode(
								config::gameWidth + config::gameOffset.x
							),
							ScriptProgramUtil::makeClearSpawnNode()
						)
					)
				}
			).heapClone()
		}
		, s1e10cPrototype{
			EntityBuilder::makeLinearCollidable(
				{ config::leftOut, 35.0f },
				{ 1.5f, Angle{ -145.0f }.flipY() },
				WISP_ARGS(300, pickupProgramsPointer->smallPowerSpawnProgram),
				ScriptProgramList{
					ScriptProgramUtil::makeTurnEntryAndShootProgram(
						20,
						45,
						{ 0.5f, Angle{ 196.0f }.flipY() },
						Angle{ -145.0f }.flipY(),
						s1d10c,
						ScriptProgramUtil::makeStallingIfNode(
							ScriptProgramUtil::makeBoundaryXHighNode(
								config::gameWidth + config::gameOffset.x
							),
							ScriptProgramUtil::makeClearSpawnNode()
						)
					)
				}
			).heapClone()
		}
		, s1e10Node{
			SpawnProgramUtil::makeListNode(
				SpawnProgramUtil::makeSpawnNode(s1e10aPrototype),
				SpawnProgramUtil::makeSpawnNode(s1e10bPrototype),
				SpawnProgramUtil::makeSpawnNode(s1e10cPrototype)
			)
		}
		, s1e10{ s1e10Node, 1, false }

			//wave 11
		, s1e11aPrototype{
			EntityBuilder::makePosVelPrototype(
				WISP_ARGS(200, pickupProgramsPointer->smallPowerSpawnProgram),
				ScriptProgramList{
					ScriptProgramUtil::makeShootOnceAndLeaveTurningProgram(
						20,
						25,
						5,
						s1d4,
						20,
						{ 1.5f, -25.0f},
						-90.0f,
						100
					)
				}
			).heapClone()
		}
		, s1e11bPrototype{
			EntityBuilder::makePosVelPrototype(
				WISP_ARGS(200, pickupProgramsPointer->smallPowerSpawnProgram),
				ScriptProgramList{
					ScriptProgramUtil::makeShootOnceAndLeaveTurningProgram(
						20,
						25,
						5,
						s1d4,
						20,
						{ 1.5f, 180.0f + 25.0f },
						-90.0f,
						100
					)
				}
			).heapClone()
		}
		, s1e11Node{
			SpawnProgramUtil::makeListNode(
				SpawnProgramUtil::makeSpawnPosVelNode(
					s1e11aPrototype,
					SpawnProgramUtil::makePointValueNode(
						{ 30.0f + config::gameOffset.x, config::topOut }
					),
					SpawnProgramUtil::makeVelocityValueNode(Velocity{ 1.5f, -90.0f })
				),
				SpawnProgramUtil::makeSpawnPosVelNode(
					s1e11bPrototype,
					SpawnProgramUtil::makePointValueNode(
						{ 
							config::gameWidth - 30.0f + config::gameOffset.x, 
							config::topOut 
						}
					),
					SpawnProgramUtil::makeVelocityValueNode(Velocity{ 1.5f, -90.0f })
				)
			)
		}
		, s1e11{ s1e11Node, 1, false }
			//wave 12
		, s1e12Node{
			SpawnProgramUtil::makeListNode(
				SpawnProgramUtil::makeSpawnPosVelNode(
					s1e11aPrototype,
					SpawnProgramUtil::makePointValueNode(
						{ 70.0f + config::gameOffset.x, config::topOut }
					),
					SpawnProgramUtil::makeVelocityValueNode(Velocity{ 1.5f, -90.0f })
				),
				SpawnProgramUtil::makeSpawnPosVelNode(
					s1e11bPrototype,
					SpawnProgramUtil::makePointValueNode(
						{ 
							config::gameWidth - 70.0f + config::gameOffset.x, 
							config::topOut 
						}
					),
					SpawnProgramUtil::makeVelocityValueNode(Velocity{ 1.5f, -90.0f })
				)
			)
		}
		, s1e12{ s1e12Node, 1, false }

			//wave 13
		, s1e13{ s1e2aNode, 140, false }

			//wave 14 and 15 = repeat 11 and 12

			//wave 16
		, s1e16Node{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, 14),
				SpawnProgramUtil::makeSpawnPosVelNode(
					s1e2aPrototype,
					SpawnProgramUtil::makePointValueNode({ config::rightOut, 40.0f }),
					SpawnProgramUtil::makeVelocityValueNode(
						Velocity{ 2.0f, Angle{ -15.0f }.flipY() }
					)
				)
			)
		}
		, s1e16{ s1e16Node, 140, false }

			//BOSS 1

			//attack 1, 6-symmetry counter spiral
		, b1d1Node{
			SpawnProgramUtil::makeListNode(
				SpawnProgramUtil::makeIfNode(
					SpawnProgramUtil::makeTickModNode(0, {30, 20, 15, 12}),
					SpawnProgramUtil::makeRingFormationNode(
						SpawnProgramUtil::makeVelocityFromPolarNode(
							SpawnProgramUtil::makeFloatValueNode(0.75f),
							SpawnProgramUtil::makeSpiralNode(
								SpawnProgramUtil::makeIntValueNode(2400),
								SpawnProgramUtil::makeFloatValueNode(0),
								SpawnProgramUtil::makeFloatValueNode(1.2125f)
							)
						),
						6,	//symmetry
						SpawnProgramUtil::makeSpawnPosVelNode(
							sharpMagentaPrototype,
							SpawnProgramUtil::makeEntityPositionNode()
						)
					)
				),
				SpawnProgramUtil::makeIfNode(
					SpawnProgramUtil::makeTickModNode(0, {30, 20, 15, 12}),
					SpawnProgramUtil::makeRingFormationNode(
						SpawnProgramUtil::makeVelocityFromPolarNode(
							SpawnProgramUtil::makeFloatValueNode(0.75f),
							SpawnProgramUtil::makeSpiralNode(
								SpawnProgramUtil::makeIntValueNode(2400),
								SpawnProgramUtil::makeFloatValueNode(0),
								SpawnProgramUtil::makeFloatValueNode(-1.2125f)
							)
						),
						6,	//symmetry
						SpawnProgramUtil::makeSpawnPosVelNode(
							sharpCyanPrototype,
							SpawnProgramUtil::makeEntityPositionNode()
						)
					)
				)
			)
		}
		, b1d1{ b1d1Node, 2400, true }

			//attack 2, rain + large aimed
		, b1d2Node{
			SpawnProgramUtil::makeListNode(
				SpawnProgramUtil::makeIfNode(
					SpawnProgramUtil::makeTickModNode(0, {8, 6, 4, 3}),
					SpawnProgramUtil::makeSpawnPosVelNode(
						sharpAzurePrototype,
						SpawnProgramUtil::randomTopOutPointNode,
						SpawnProgramUtil::makeVelocityFromPolarNode(
							SpawnProgramUtil::makeUniformRandomFloatNode(0.7f, 1.9f),
							SpawnProgramUtil::makeUniformRandomFloatNode(-100.0f, -80.0f)
						)
					)
				),
				SpawnProgramUtil::makeIfNode(
					SpawnProgramUtil::makeIsLastTickNode(),
					SpawnProgramUtil::makeRingFormationNode(
						SpawnProgramUtil::makeVelocityFromPolarNode(
							SpawnProgramUtil::makeFloatValueNode(0.8f),
							SpawnProgramUtil::makeAngleToPlayerNode()
						),
						3,
						SpawnProgramUtil::makeSpawnPosVelNode(
							largeAzurePrototype,
							SpawnProgramUtil::makeEntityPositionNode()
						)
					)
					
				)
			)
		}
		, b1d2{ b1d2Node, 96, true }

			//attack 3, 3 pos 2 arm spirals
		, b1d3Node{
			#define SPEED 0.85f
			#define AV 1.8423f
			#define OFFSET 30.0f
			#define OFFSET_AV 1.2384f
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, {20, 15, 10, 6}),
				SpawnProgramUtil::makeListNode(
					SpawnProgramUtil::makeRingFormationNode(
						SpawnProgramUtil::makeVelocityFromPolarNode(
							SpawnProgramUtil::makeFloatValueNode(SPEED),
							SpawnProgramUtil::makeSpiralNode(
								SpawnProgramUtil::makeIntValueNode(2400),
								SpawnProgramUtil::makeFloatValueNode(0.0f),
								SpawnProgramUtil::makeFloatValueNode(AV)
							)
						),
						2,	//symmetry
						SpawnProgramUtil::makeSpawnPosVelNode(
							sharpSpringPrototype,
							SpawnProgramUtil::makeEntityOffsetNode(
								SpawnProgramUtil::makeVelocityFromPolarNode(
									SpawnProgramUtil::makeFloatValueNode(OFFSET),
									SpawnProgramUtil::makeSpiralNode(
										SpawnProgramUtil::makeIntValueNode(2400),
										SpawnProgramUtil::makeFloatValueNode(0.0f),
										SpawnProgramUtil::makeFloatValueNode(OFFSET_AV)
									)
								)
							)
						)
					),
					SpawnProgramUtil::makeRingFormationNode(
						SpawnProgramUtil::makeVelocityFromPolarNode(
							SpawnProgramUtil::makeFloatValueNode(SPEED),
							SpawnProgramUtil::makeSpiralNode(
								SpawnProgramUtil::makeIntValueNode(2400),
								SpawnProgramUtil::makeFloatValueNode(120.0f),
								SpawnProgramUtil::makeFloatValueNode(AV)
							)
						),
						2,	//symmetry
						SpawnProgramUtil::makeSpawnPosVelNode(
							sharpSpringPrototype,
							SpawnProgramUtil::makeEntityOffsetNode(
								SpawnProgramUtil::makeVelocityFromPolarNode(
									SpawnProgramUtil::makeFloatValueNode(OFFSET),
									SpawnProgramUtil::makeSpiralNode(
										SpawnProgramUtil::makeIntValueNode(2400),
										SpawnProgramUtil::makeFloatValueNode(120.0f),
										SpawnProgramUtil::makeFloatValueNode(OFFSET_AV)
									)
								)
							)
						)
					),
					SpawnProgramUtil::makeRingFormationNode(
						SpawnProgramUtil::makeVelocityFromPolarNode(
							SpawnProgramUtil::makeFloatValueNode(SPEED),
							SpawnProgramUtil::makeSpiralNode(
								SpawnProgramUtil::makeIntValueNode(2400),
								SpawnProgramUtil::makeFloatValueNode(240.0f),
								SpawnProgramUtil::makeFloatValueNode(AV)
							)
						),
						2,	//symmetry
						SpawnProgramUtil::makeSpawnPosVelNode(
							sharpSpringPrototype,
							SpawnProgramUtil::makeEntityOffsetNode(
								SpawnProgramUtil::makeVelocityFromPolarNode(
									SpawnProgramUtil::makeFloatValueNode(OFFSET),
									SpawnProgramUtil::makeSpiralNode(
										SpawnProgramUtil::makeIntValueNode(2400),
										SpawnProgramUtil::makeFloatValueNode(240.0f),
										SpawnProgramUtil::makeFloatValueNode(OFFSET_AV)
									)
								)
							)
						)
					)
				)
			)
			#undef SPEED
			#undef AV
			#undef OFFSET
			#undef OFFSET_AV
		}
		, b1d3{ b1d3Node, 2400, true }

		, b1d4Prototype{
			EntityBuilder::makePosVelPrototype(
				SHARP_BULLET_ARGS(sharpHitbox, outbound, L"sharp_blue", 50),
				ScriptProgramList{
					ScriptProgramUtil::makeTimerNode(50,
					ScriptProgramUtil::makeShiftSpeedPeriodNode(1.1f, 30))
				}
			).heapClone()
		}
		, b1d4Node{
			SpawnProgramUtil::makeListNode(
				SpawnProgramUtil::makeIfNode(
					SpawnProgramUtil::makeTickModNode(0, {10, 6, 4, 3}),
					SpawnProgramUtil::makeEntityOffsetNode(
						SpawnProgramUtil::makeUniformRandomCircleVelocityNode(
							SpawnProgramUtil::makeFloatValueNode(0.0f),
							SpawnProgramUtil::makeFloatValueDiffNode(
								{30.0f, 40.0f, 50.0f, 60.0f}
							)
						),
						SpawnProgramUtil::makeRingFormationNode(
							SpawnProgramUtil::makeVelocityFromPolarNode(
								SpawnProgramUtil::makeFloatValueNode(0.0f),
								SpawnProgramUtil::randomAngleNode
							),
							3,
							SpawnProgramUtil::makeSpawnPosVelNode(
								b1d4Prototype
							)
						)
					)
				),
				SpawnProgramUtil::makeIfNode(
					SpawnProgramUtil::makeTickModNode(0, 96),
					SpawnProgramUtil::makeRingFormationNode(
						SpawnProgramUtil::makeVelocityFromPolarNode(
							SpawnProgramUtil::makeFloatValueNode(0.87f),
							SpawnProgramUtil::makeAngleToPlayerNode()
						),
						SpawnProgramUtil::makeIntValueDiffNode({ 6, 12, 18, 24 }),
						SpawnProgramUtil::makeSpawnPosVelNode(
							mediumRedPrototype,
							SpawnProgramUtil::makeEntityPositionNode()
						)
					)
				)
			)
		}
		, b1d4{ b1d4Node, 2400, true }

		, b1e1Prototype{
			EntityBuilder::makeLinearUncollidable(
				BOSS_ARGS,
				SpriteInstruction{
					bitmapStoragePointer->get(L"b1_idle_1")->d2dBitmap,
					math::Vector2{ 0.0f, 4.0f }			//offset
				},
				DrawOrder{ config::playerDrawOrder },
				AnimationList{ 
					{
						components::Animation{ {
							L"b1_idle_1", L"b1_idle_2", L"b1_idle_3", L"b1_idle_4"
						} }
					},
					0,	//idle index
					4	//ticks
				},
				ScriptProgramList{
					ScriptProgramUtil::makeBossEntryNode(120, L"1_1",
					ScriptProgramUtil::makeSetHealthNode(2500,
					ScriptProgramUtil::makeSetSpawnNode(b1d1,
					ScriptProgramUtil::makeBossMoveNode(90, 90,
					ScriptProgramUtil::makeBossResetNode(90,
					ScriptProgramUtil::makeSetHealthNode(2700,
					ScriptProgramUtil::makeAddSpawnNode(b1d2,
					ScriptProgramUtil::makeBossMoveNode(110, 110,
					ScriptProgramUtil::makeAddSpawnNode(
						pickupProgramsPointer->largePowerSpawnProgram,
					ScriptProgramUtil::makeTimerNode(1,
					ScriptProgramUtil::makeBossResetNode(90,
					ScriptProgramUtil::makeSetHealthNode(2500,
					ScriptProgramUtil::makeAddSpawnNode(b1d3,
					ScriptProgramUtil::makeBossMoveNode(90, 90,
					ScriptProgramUtil::makeBossResetMidNode(90,
					ScriptProgramUtil::makeSetHealthNode(3500,
					ScriptProgramUtil::makeAddSpawnNode(b1d4,
					ScriptProgramUtil::makeStallingIfNode(
						ScriptProgramUtil::makeIsBossDeadNode(),
						ScriptProgramUtil::makeBossEndNode()
					)
					)))))))))))))))))
				}
			).heapClone()
		}
		, b1e1Node{
			SpawnProgramUtil::makeSpawnNode(b1e1Prototype)
		}
		, b1e1{ b1e1Node, 1, false }

		
		, stage1ScriptProgram{
			ScriptProgramUtil::makeTimerNode(130,
			ScriptProgramUtil::makeSetSpawnNode(s1e1a,
			ScriptProgramUtil::makeTimerNode(330,
			ScriptProgramUtil::makeSetSpawnNode(s1e1b,
			ScriptProgramUtil::makeTimerNode(200,
			ScriptProgramUtil::makeAddSpawnNode(s1e2a,
			ScriptProgramUtil::makeTimerNode(130,
			ScriptProgramUtil::makeAddSpawnNode(s1e2b,
			ScriptProgramUtil::makeTimerNode(200,
			ScriptProgramUtil::makeAddSpawnNode(s1e3,
			ScriptProgramUtil::makeTimerNode(300,
			ScriptProgramUtil::makeAddSpawnNode(s1e4a,
			ScriptProgramUtil::makeTimerNode(80,
			ScriptProgramUtil::makeAddSpawnNode(s1e4b,
			ScriptProgramUtil::makeTimerNode(160,
			ScriptProgramUtil::makeAddSpawnNode(s1e5,
			ScriptProgramUtil::makeTimerNode(110,
			ScriptProgramUtil::makeAddSpawnNode(s1e6,
			ScriptProgramUtil::makeTimerNode(300,
			ScriptProgramUtil::makeAddSpawnNode(s1e7,
			ScriptProgramUtil::makeTimerNode(300,
			ScriptProgramUtil::makeAddSpawnNode(s1e8,
			ScriptProgramUtil::makeTimerNode(400,
			ScriptProgramUtil::makeAddSpawnNode(s1e9,
			ScriptProgramUtil::makeTimerNode(330,
			ScriptProgramUtil::makeAddSpawnNode(s1e10,
			ScriptProgramUtil::makeTimerNode(370,
			ScriptProgramUtil::makeAddSpawnNode(s1e11,
			ScriptProgramUtil::makeTimerNode(90,
			ScriptProgramUtil::makeAddSpawnNode(s1e12,
			ScriptProgramUtil::makeTimerNode(100,
			ScriptProgramUtil::makeAddSpawnNode(s1e13,
			ScriptProgramUtil::makeTimerNode(140,
			ScriptProgramUtil::makeAddSpawnNode(s1e11,	//14
			ScriptProgramUtil::makeTimerNode(90,
			ScriptProgramUtil::makeAddSpawnNode(s1e12,	//15
			ScriptProgramUtil::makeTimerNode(100,
			ScriptProgramUtil::makeAddSpawnNode(s1e16,
			ScriptProgramUtil::makeTimerNode(200,
			ScriptProgramUtil::makeAddSpawnNode(b1e1,
			ScriptProgramUtil::makeStallingIfNode(
				ScriptProgramUtil::makeIsWinNode(),
				ScriptProgramUtil::makeTimerNode(waitEndTime,
				ScriptProgramUtil::makeShowDialogueNode(L"1_2",
				ScriptProgramUtil::makeStallingIfNode(
					ScriptProgramUtil::makeIsDialogueOverNode(),
					ScriptProgramUtil::makeTimerNode(1,
					ScriptProgramUtil::makeEndStageNode()
				)))))
			)
			)))))))))))))))))))))))))))))))))))))))
		}

		// STAGE 2 // STAGE 2 // STAGE 2 // STAGE 2 // STAGE 2 // STAGE 2 // STAGE 2 //

			//wave 1
		, s2d1Node{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, { 6, 6, 4, 4 }),
				SpawnProgramUtil::makeRingFormationNode(
					SpawnProgramUtil::makeVelocityFromPolarNode(
						SpawnProgramUtil::makeWhipNode(42, 0.4f, 3.5f),
						SpawnProgramUtil::makeAngleToPlayerNode()
					),
					{ 12, 16, 20, 26 },
					SpawnProgramUtil::makeSpawnPosVelNode(
						mediumBluePrototype,
						SpawnProgramUtil::makeEntityPositionNode()
					)
				)
			)
		}
		, s2d1{ s2d1Node, 42, false }
		, s2e1Prototype{
			EntityBuilder::makePosPrototype( 
				{ 1.5f, -90.0f }, 
				WISP_ARGS(500, pickupProgramsPointer->smallPowerSpawnProgram),
				ScriptProgramList{
					ScriptProgramUtil::makeShootOnceAndLeaveProgram(
						20,
						30,
						5,
						s2d1,
						50,
						{ 1.0f, 90.0f},
						100
					)
				}
			).heapClone()
		}
		, s2e1Node{ 
			SpawnProgramUtil::makeSpawnPosNode(
				s2e1Prototype,
				SpawnProgramUtil::makePointValueNode(
					{ (config::gameWidth / 2.0f) + config::gameOffset.x, config::topOut}
				) 
			)
		}
		, s2e1{ s2e1Node, 1, false }

			//wave 2
		, s2e2Node{ 
			SpawnProgramUtil::makeSpawnPosNode(
				s2e1Prototype,
				SpawnProgramUtil::makePointValueNode({ 35.0f, config::topOut }) 
			)
		}
		, s2e2{ s2e2Node, 1, false }

			//wave 3
		#define BULLET_SPEED 1.2f
		, s2d3aNode{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(
					SpawnProgramUtil::makeEntityUniformRandomIntNode(
						SpawnProgramUtil::makeIntValueNode(0),
						SpawnProgramUtil::makeIntValueDiffNode({ 170, 130, 100, 70 })
					),
					{ 170, 130, 100, 70 }
				),
				SpawnProgramUtil::makeArcFormationNode(
					SpawnProgramUtil::makeVelocityFromPolarNode(
						SpawnProgramUtil::makeFloatValueNode(BULLET_SPEED),
						SpawnProgramUtil::makeAngleToPlayerNode()
					),
					SpawnProgramUtil::makeIntValueDiffNode({ 3, 3, 5, 5 }),
					SpawnProgramUtil::makeFloatValueNode(15.0f),
					SpawnProgramUtil::makeSpawnPosVelNode(
						smallBluePrototype,
						SpawnProgramUtil::makeEntityPositionNode()
					)
				)
			)
		}
		, s2d3a{ s2d3aNode, 1000, true }

		, s2e3aPrototype{
			EntityBuilder::makePosVelPrototype(
				FLAME_ARGS(30, pickupProgramsPointer->smallPowerThirdSpawnProgram),
				SpawnProgramList{ s2d3a }
			).heapClone()
		}
		, s2d3bNode{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(
					SpawnProgramUtil::makeEntityUniformRandomIntNode(
						SpawnProgramUtil::makeIntValueNode(0),
						SpawnProgramUtil::makeIntValueDiffNode({ 170, 130, 100, 70 })
					),
					{ 170, 130, 100, 70 }
				),
				SpawnProgramUtil::makeArcFormationNode(
					SpawnProgramUtil::makeVelocityFromPolarNode(
						SpawnProgramUtil::makeFloatValueNode(BULLET_SPEED),
						SpawnProgramUtil::makeAngleToPlayerNode()
					),
					SpawnProgramUtil::makeIntValueDiffNode({ 3, 3, 5, 5 }),
					SpawnProgramUtil::makeFloatValueNode(15.0f),
					SpawnProgramUtil::makeSpawnPosVelNode(
						smallRedPrototype,
						SpawnProgramUtil::makeEntityPositionNode()
					)
				)
			)
		}
		#undef BULLET_SPEED
		, s2d3b{ s2d3bNode, 1000, true }

		, s2e3bPrototype{
			EntityBuilder::makePosVelPrototype(
				FLAME_ARGS(30, pickupProgramsPointer->smallPowerThirdSpawnProgram),
				SpawnProgramList{ s2d3b }
			).heapClone()
		}
		, s2e3Node{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, 14),
				SpawnProgramUtil::makeListNode(
					SpawnProgramUtil::makeSpawnPosVelNode(
						s2e3aPrototype,
						SpawnProgramUtil::makePointValueNode({ config::rightOut, 25.0f }),
						SpawnProgramUtil::makeVelocityValueNode(Velocity{ 1.8f, 188.0f })
					),
					SpawnProgramUtil::makeSpawnPosVelNode(
						s2e3bPrototype,
						SpawnProgramUtil::makePointValueNode({ config::leftOut, 60.0f }),
						SpawnProgramUtil::makeVelocityValueNode(Velocity{ 1.8f, -8.0f })
					)
				)
			)
		}
		, s2e3{ s2e3Node, 250, false }

			//wave 4
		, s2d4Node{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, { 6, 6, 4, 4 }),
				SpawnProgramUtil::makeRingFormationNode(
					SpawnProgramUtil::makeVelocityFromPolarNode(
						SpawnProgramUtil::makeWhipNode(80, 0.4f, 3.5f),
						SpawnProgramUtil::makeSpiralNode(
							SpawnProgramUtil::makeIntValueNode(80),
							SpawnProgramUtil::entityRandomAngleNode,
							SpawnProgramUtil::makeSpiralNode(
								SpawnProgramUtil::makeIntValueNode(80),
								SpawnProgramUtil::makeFloatValueNode(0.0f),
								SpawnProgramUtil::makeFloatValueNode(1.8f)
							)
						)
					),
					{ 5, 7, 7, 9 },
					SpawnProgramUtil::makeSpawnPosVelNode(
						mediumBluePrototype,
						SpawnProgramUtil::makeEntityPositionNode()
					)
				)
			)
		}
		, s2d4{ s2d4Node, 80, false }
		, s2e4Prototype{
			EntityBuilder::makePosPrototype( 
				{ 1.5f, -90.0f }, 
				WISP_ARGS(800, pickupProgramsPointer->largePowerSpawnProgram),
				ScriptProgramList{
					ScriptProgramUtil::makeShootOnceAndLeaveProgram(
						20,
						30,
						5,
						s2d4,
						80,
						{ 1.0f, 90.0f},
						100
					)
				}
			).heapClone()
		}
		, s2e4Node{ 
			SpawnProgramUtil::makeSpawnPosNode(
				s2e4Prototype,
				SpawnProgramUtil::makePointValueNode(
					{ config::gameWidth - 35.0f, config::topOut }
				) 
			)
		}
		, s2e4{ s2e4Node, 1, false }

		#define SHELL_SPEED 0.87f

			//wave 5
		, s2d5Node{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(
					SpawnProgramUtil::makeEntityUniformRandomIntNode(
						SpawnProgramUtil::makeIntValueNode(0),
						SpawnProgramUtil::makeIntValueDiffNode({ 200, 170, 130, 100 })
					),
					{ 200, 170, 130, 100 }
				),
				SpawnProgramUtil::makeSpawnPosVelNode(
					sharpRedPrototype,
					SpawnProgramUtil::makeEntityPositionNode(),
					SpawnProgramUtil::makeVelocityFromPolarNode(
						SpawnProgramUtil::makeFloatValueDiffNode(
							{ 2.0f, 2.5f, 3.0f, 3.5f }
						),
						SpawnProgramUtil::makeAngleToPlayerNode()
					)
				)
			)
		}
		, s2d5{ s2d5Node, 10000, false }
		, s2e5Prototype{
			EntityBuilder::makePosVelPrototype(
				SHELL_ARGS(300, pickupProgramsPointer->smallPowerThirdSpawnProgram),
				SpawnProgramList{ s2d5 },
				ScriptProgramList{
					ScriptProgramUtil::makeStallingIfNode(
						ScriptProgramUtil::makeBoundaryYHighNode(40.0f),
						ScriptProgramUtil::makeShiftVelocityTurnPeriodNode(
							Velocity{ SHELL_SPEED, -160.0f },
							-90.0f,
							50,
							ScriptProgramUtil::makeStallingIfNode(
								ScriptProgramUtil::makeBoundaryXLowNode(
									15.0f + config::gameOffset.x
								),
								ScriptProgramUtil::makeClearSpawnNode()
							)
						)
					)
				}
			).heapClone()
		}
		, s2e5Node{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, 24),
				SpawnProgramUtil::makeSpawnPosVelNode(
					s2e5Prototype,
					SpawnProgramUtil::makePointValueNode(
						{ 135.0f + config::gameOffset.x, config::topOut }
					),
					SpawnProgramUtil::makeVelocityValueNode(
						Velocity{ SHELL_SPEED, -90.0f }
					)
				)
			)
		}
		, s2e5{ s2e5Node, 250, false }

			//wave 6
		, s2e6aPrototype{
			EntityBuilder::makePosVelPrototype(
				SHELL_ARGS(300, pickupProgramsPointer->smallPowerThirdSpawnProgram),
				SpawnProgramList{ s2d5 },
				ScriptProgramList{
					ScriptProgramUtil::makeStallingIfNode(
						ScriptProgramUtil::makeBoundaryYHighNode(40.0f),
						ScriptProgramUtil::makeShiftVelocityTurnPeriodNode(
							Velocity{ SHELL_SPEED, Angle{ -160.0f }.flipY() },
							-90.0f,
							50,
							ScriptProgramUtil::makeStallingIfNode(
								ScriptProgramUtil::makeBoundaryXHighNode(
									config::gameWidth - 15.0f + config::gameOffset.x
								),
								ScriptProgramUtil::makeClearSpawnNode()
							)
						)
					)
				}
			).heapClone()
		}
		, s2e6aNode{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, 24),
				SpawnProgramUtil::makeSpawnPosVelNode(
					s2e6aPrototype,
					SpawnProgramUtil::makePointValueNode({ 
						config::gameWidth - 135.0f + config::gameOffset.x, 
						config::topOut 
					} ),
					SpawnProgramUtil::makeVelocityValueNode(
						Velocity{ SHELL_SPEED, -90.0f }
					)
				)
			)
		}
		, s2e6a{ s2e6aNode, 250, false }

		, s2d6bNode{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, { 22, 20, 18, 16 }),
				SpawnProgramUtil::makeRingFormationNode(
					SpawnProgramUtil::makeVelocityFromPolarNode(
						SpawnProgramUtil::makeWhipNode(390, 0.4f, 3.5f),
						SpawnProgramUtil::makeSpiralNode(
							SpawnProgramUtil::makeIntValueNode(390),
							SpawnProgramUtil::entityRandomAngleNode,
							SpawnProgramUtil::makeFloatValueNode(-0.8f)
						)
					),
					{ 5, 6, 7, 8 },
					SpawnProgramUtil::makeSpawnPosVelNode(
						smallVioletPrototype,
						SpawnProgramUtil::makeEntityPositionNode()
					)
				)
			)
		}
		, s2d6b{ s2d6bNode, 390, false }

		, s2e6bPrototype{
			EntityBuilder::makePosPrototype( 
				{ 1.5f, -90.0f }, 
				WISP_ARGS(900, pickupProgramsPointer->lifeSpawnProgram),
				ScriptProgramList{
					ScriptProgramUtil::makeShootOnceAndLeaveProgram(
						20,
						30,
						5,
						s2d6b,
						80,
						{ 1.0f, 90.0f},
						100
					)
				}
			).heapClone()
		}
		, s2e6bNode{ 
			SpawnProgramUtil::makeSpawnPosNode(
				s2e6bPrototype,
				SpawnProgramUtil::makePointValueNode(
					{ (config::gameWidth / 2.0f) + config::gameOffset.x, config::topOut}
				) 
			)
		}
		, s2e6b{ s2e6bNode, 1, false }

			//wave 7
		, s2e7Node{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, 14),
				SpawnProgramUtil::makeListNode(
					SpawnProgramUtil::makeSpawnPosVelNode(
						s2e3aPrototype,
						SpawnProgramUtil::makePointValueNode({ config::rightOut, 60.0f }),
						SpawnProgramUtil::makeVelocityValueNode(Velocity{ 1.8f, 188.0f })
					),
					SpawnProgramUtil::makeSpawnPosVelNode(
						s2e3bPrototype,
						SpawnProgramUtil::makePointValueNode({ config::leftOut, 25.0f }),
						SpawnProgramUtil::makeVelocityValueNode(Velocity{ 1.8f, -8.0f })
					)
				)
			)
		}
		, s2e7{ s2e3Node, 150, false }

			//wave 8
		, s2d8Node{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, { 8, 8, 6, 6 }),
				SpawnProgramUtil::makeRingFormationNode(
					SpawnProgramUtil::makeVelocityFromPolarNode(
						SpawnProgramUtil::makeWhipNode(42, 1.1f, 3.5f),
						SpawnProgramUtil::makeAngleToPlayerNode()
					),
					{ 8, 12, 16, 20 },
					SpawnProgramUtil::makeSpawnPosVelNode(
						mediumBluePrototype,
						SpawnProgramUtil::makeEntityPositionNode()
					)
				)
			)
		}
		, s2d8{ s2d8Node, 32, false }
		, s2e8Prototype{
			EntityBuilder::makePosPrototype(
				{ 1.5f, -90.0f },
				WISP_ARGS(500, pickupProgramsPointer->smallPowerSpawnProgram),
				ScriptProgramList{
					ScriptProgramUtil::makeShootOnceAndLeaveProgram(
						20,
						30,
						5,
						s2d8,
						50,
						{ 1.0f, 90.0f},
						100
					)
				}
			).heapClone()
		}
		, s2e8aNode{
			SpawnProgramUtil::makeListNode(
				SpawnProgramUtil::makeSpawnPosNode(
					s2e8Prototype,
					SpawnProgramUtil::makePointValueNode({ 
						10.0f + config::gameOffset.x, 
						config::topOut
					} )
				),
				SpawnProgramUtil::makeSpawnPosNode(
					s2e8Prototype,
					SpawnProgramUtil::makePointValueNode({
						config::gameWidth - 10.0f + config::gameOffset.x,
						config::topOut
					} )
				)
			)
		}
		, s2e8a{ s2e8aNode, 1, false }

		, s2e8bNode{
			SpawnProgramUtil::makeListNode(
				SpawnProgramUtil::makeSpawnPosNode(
					s2e8Prototype,
					SpawnProgramUtil::makePointValueNode({ 
						40.0f + config::gameOffset.x, 
						config::topOut
					} )
				),
				SpawnProgramUtil::makeSpawnPosNode(
					s2e8Prototype,
					SpawnProgramUtil::makePointValueNode({
						config::gameWidth - 40.0f + config::gameOffset.x,
						config::topOut
					} )
				)
			)
		}
		, s2e8b{ s2e8bNode, 1, false }

		, s2e8cNode{
			SpawnProgramUtil::makeListNode(
				SpawnProgramUtil::makeSpawnPosNode(
					s2e8Prototype,
					SpawnProgramUtil::makePointValueNode({ 
						70.0f + config::gameOffset.x, 
						config::topOut
					} )
				),
				SpawnProgramUtil::makeSpawnPosNode(
					s2e8Prototype,
					SpawnProgramUtil::makePointValueNode({
						config::gameWidth - 70.0f + config::gameOffset.x,
						config::topOut
					} )
				)
			)
		}
		, s2e8c{ s2e8cNode, 1, false }

			//wave 9
		, s2d9aNode{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, { 20, 16, 14, 8 }),
				SpawnProgramUtil::makeRingFormationNode(
					SpawnProgramUtil::makeVelocityFromPolarNode(
						SpawnProgramUtil::makeWhipNode(120, 0.4f, 3.5f),
						SpawnProgramUtil::makeSpiralNode(
							SpawnProgramUtil::makeIntValueNode(120),
							SpawnProgramUtil::entityRandomAngleNode,
							SpawnProgramUtil::makeFloatValueNode(-0.8f)
						)
					),
					{ 5, 6, 7, 8 },
					SpawnProgramUtil::makeSpawnPosVelNode(
						smallVioletPrototype,
						SpawnProgramUtil::makeEntityPositionNode()
					)
				)
			)
		}
		, s2d9a{ s2d9aNode, 120, false }
		, s2d9bNode{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, { 20, 16, 14, 8 }),
				SpawnProgramUtil::makeRingFormationNode(
					SpawnProgramUtil::makeVelocityFromPolarNode(
						SpawnProgramUtil::makeWhipNode(120, 0.4f, 3.5f),
						SpawnProgramUtil::makeSpiralNode(
							SpawnProgramUtil::makeIntValueNode(120),
							SpawnProgramUtil::entityRandomAngleNode,
							SpawnProgramUtil::makeFloatValueNode(0.8f)
						)
					),
					{ 5, 6, 7, 8 },
					SpawnProgramUtil::makeSpawnPosVelNode(
						smallVioletPrototype,
						SpawnProgramUtil::makeEntityPositionNode()
					)
				)
			)
		}
		, s2d9b{ s2d9bNode, 120, false }

		, s2e9aPrototype{
			EntityBuilder::makePosPrototype( 
				{ 1.5f, -90.0f }, 
				WISP_ARGS(700, pickupProgramsPointer->largePowerSpawnProgram),
				ScriptProgramList{
					ScriptProgramUtil::makeShootOnceAndLeaveProgram(
						20,
						30,
						5,
						s2d9a,
						30,
						{ 1.0f, 90.0f},
						100
					)
				}
			).heapClone()
		}
		, s2e9bPrototype{
			EntityBuilder::makePosPrototype(
				{ 1.5f, -90.0f },
				WISP_ARGS(700, pickupProgramsPointer->largePowerSpawnProgram),
				ScriptProgramList{
					ScriptProgramUtil::makeShootOnceAndLeaveProgram(
						20,
						30,
						5,
						s2d9b,
						30,
						{ 1.0f, 90.0f},
						100
					)
				}
			).heapClone()
		}
		, s2e9Node{ 
			SpawnProgramUtil::makeListNode(
				SpawnProgramUtil::makeSpawnPosNode(
					s2e9aPrototype,
					SpawnProgramUtil::makePointValueNode({ 
						30.0f + config::gameOffset.x, 
						config::topOut
					} )
				),
				SpawnProgramUtil::makeSpawnPosNode(
					s2e9bPrototype,
					SpawnProgramUtil::makePointValueNode({ 
						config::gameWidth - 30.0f + config::gameOffset.x, 
						config::topOut
					} )
				)
			)
			
		}
		, s2e9{ s2e9Node, 1, false }

			//wave 10
		, s2d10aNode{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(
					SpawnProgramUtil::makeEntityUniformRandomIntNode(0,	50),
					50
				),
				SpawnProgramUtil::makeRingFormationNode(
					SpawnProgramUtil::makeVelocityFromPolarNode(
						SpawnProgramUtil::makeUniformRandomFloatNode(
							0.5f, 1.0f
						),
						SpawnProgramUtil::randomAngleNode
					),
					SpawnProgramUtil::makeIntValueDiffNode({ 4, 6, 8, 10 }),
					SpawnProgramUtil::makeSpawnPosVelNode(
						sharpYellowPrototype,
						SpawnProgramUtil::makeEntityPositionNode()
					)
				)
			)
		}
		, s2d10a{ s2d10aNode, 10000, true }
		, s2d10bNode{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(
					SpawnProgramUtil::makeEntityUniformRandomIntNode(0,	50),
					50
				),
				SpawnProgramUtil::makeRingFormationNode(
					SpawnProgramUtil::makeVelocityFromPolarNode(
						SpawnProgramUtil::makeUniformRandomFloatNode(
							0.5f, 1.0f
						),
						SpawnProgramUtil::randomAngleNode
					),
					SpawnProgramUtil::makeIntValueDiffNode({ 4, 6, 8, 10 }),
					SpawnProgramUtil::makeSpawnPosVelNode(
						sharpChartreusePrototype,
						SpawnProgramUtil::makeEntityPositionNode()
					)
				)
			)
		}
		, s2d10b{ s2d10bNode, 10000, true }
		, s2e10aPrototype{
			EntityBuilder::makePosVelPrototype(
				FLAME_ARGS(30, pickupProgramsPointer->smallPowerThirdSpawnProgram),
				SpawnProgramList{ s2d10a }
			).heapClone()
		}
		, s2e10bPrototype{
			EntityBuilder::makePosVelPrototype(
				FLAME_ARGS(30, pickupProgramsPointer->smallPowerThirdSpawnProgram),
				SpawnProgramList{ s2d10b }
			).heapClone()
		}

		, s2e10Node{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, 14),
				SpawnProgramUtil::makeListNode(
					SpawnProgramUtil::makeSpawnPosVelNode(
						s2e10aPrototype,
						SpawnProgramUtil::makePointValueNode({ config::rightOut, 25.0f }),
						SpawnProgramUtil::makeVelocityValueNode(Velocity{ 1.8f, 188.0f })
					),
					SpawnProgramUtil::makeSpawnPosVelNode(
						s2e10bPrototype,
						SpawnProgramUtil::makePointValueNode({ config::leftOut, 60.0f }),
						SpawnProgramUtil::makeVelocityValueNode(Velocity{ 1.8f, -8.0f })
					)
				)
			)
		}
		, s2e10{ s2e10Node, 700, false }

			//wave 11
		, s2e11aPrototype{	//left shell
			EntityBuilder::makePosVelPrototype(
				SHELL_ARGS(300, pickupProgramsPointer->smallPowerThirdSpawnProgram),
				SpawnProgramList{ s2d5 },
				ScriptProgramList{
					ScriptProgramUtil::makeStallingIfNode(
						ScriptProgramUtil::makeBoundaryYHighNode(40.0f),
						ScriptProgramUtil::makeShiftVelocityTurnPeriodNode(
							Velocity{ SHELL_SPEED, -160.0f },
							-90.0f,
							50,
							ScriptProgramUtil::makeStallingIfNode(
								ScriptProgramUtil::makeBoundaryXLowNode(
									15.0f + config::gameOffset.x
								),
								ScriptProgramUtil::makeClearSpawnNode()
							)
						)
					)
				}
			).heapClone()
		}

		, s2e11bPrototype{	//right shell
			EntityBuilder::makePosVelPrototype(
				SHELL_ARGS(300, pickupProgramsPointer->smallPowerThirdSpawnProgram),
				SpawnProgramList{ s2d5 },
				ScriptProgramList{
					ScriptProgramUtil::makeStallingIfNode(
						ScriptProgramUtil::makeBoundaryYHighNode(40.0f),
						ScriptProgramUtil::makeShiftVelocityTurnPeriodNode(
							Velocity{ SHELL_SPEED, Angle{ -160.0f }.flipY() },
							-90.0f,
							50,
							ScriptProgramUtil::makeStallingIfNode(
								ScriptProgramUtil::makeBoundaryXHighNode(
									config::gameWidth - 15.0f + config::gameOffset.x
								),
								ScriptProgramUtil::makeClearSpawnNode()
							)
						)
					)
				}
			).heapClone()
		}

		, s2e11Node{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, 24),
				SpawnProgramUtil::makeListNode(
					SpawnProgramUtil::makeSpawnPosVelNode(
						s2e5Prototype,
						SpawnProgramUtil::makePointValueNode(
							{ 48.0f + config::gameOffset.x, config::topOut }
						),
						SpawnProgramUtil::makeVelocityValueNode(
							Velocity{ SHELL_SPEED, -90.0f }
						)
					),
					SpawnProgramUtil::makeSpawnPosVelNode(
						s2e6aPrototype,
						SpawnProgramUtil::makePointValueNode({
							config::gameWidth - 48.0f + config::gameOffset.x,
							config::topOut
						}),
						SpawnProgramUtil::makeVelocityValueNode(
							Velocity{ SHELL_SPEED, -90.0f }
						)
					)
				)
				
			)
		}
		, s2e11{ s2e11Node, 300, false }

			//wave 12
		, s2d12Node{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, { 20, 16, 14, 8 }),
				SpawnProgramUtil::makeListNode(
					SpawnProgramUtil::makeRingFormationNode(
						SpawnProgramUtil::makeVelocityFromPolarNode(
							SpawnProgramUtil::makeWhipNode(300, 0.4f, 4.0f),
							SpawnProgramUtil::makeSpiralNode(
								SpawnProgramUtil::makeIntValueNode(300),
								SpawnProgramUtil::entityRandomAngleNode,
								SpawnProgramUtil::makeFloatValueDiffNode(
									{ 1.4f, 1.3f, 1.4f, 1.4f }
								)
							)
						),
						{ 4, 4, 5, 5 },
						SpawnProgramUtil::makeSpawnPosVelNode(
							smallRosePrototype,
							SpawnProgramUtil::makeEntityPositionNode()
						)
					),
					SpawnProgramUtil::makeRingFormationNode(
						SpawnProgramUtil::makeVelocityFromPolarNode(
							SpawnProgramUtil::makeWhipNode(300, 0.4f, 5.0f),
							SpawnProgramUtil::makeSpiralNode(
								SpawnProgramUtil::makeIntValueNode(300),
								SpawnProgramUtil::makeAddNode(
									SpawnProgramUtil::entityRandomAngleNode,
									SpawnProgramUtil::makeFloatValueNode(12.0f)
								),
								SpawnProgramUtil::makeFloatValueDiffNode(
									{ -1.4f, -1.3f, -1.4f, -1.4f }
								)
							)
						),
						{ 4, 4, 5, 5 },
						SpawnProgramUtil::makeSpawnPosVelNode(
							smallVioletPrototype,
							SpawnProgramUtil::makeEntityPositionNode()
						)
					)
				)
			)
		}
		, s2d12{ s2d12Node, 300, false }
		, s2e12Prototype{
			EntityBuilder::makePosPrototype( 
				{ 1.5f, -90.0f }, 
				WISP_ARGS(2000, pickupProgramsPointer->bombSpawnProgram),
				ScriptProgramList{
					ScriptProgramUtil::makeShootOnceAndLeaveProgram(
						20,
						30,
						5,
						s2d12,
						20,
						{ 1.0f, 90.0f},
						100
					)
				}
			).heapClone()
		}
		, s2e12Node{ 
			SpawnProgramUtil::makeSpawnPosNode(
				s2e12Prototype,
				SpawnProgramUtil::makePointValueNode(
					{ (config::gameWidth / 2.0f) + config::gameOffset.x, config::topOut}
				) 
			)
		}
		, s2e12{ s2e12Node, 1, false }

		// BOSS 2 // BOSS 2 // BOSS 2 // BOSS 2 // BOSS 2 // BOSS 2 // BOSS 2 // BOSS 2 

			//1 arrows / slow medium
		, b2d1aPrototype{
			EntityBuilder::makePosVelPrototype(
				SHARP_BULLET_ARGS(sharpHitbox, outbound, L"sharp_orange", 50),
				ScriptProgramList{
					ScriptProgramUtil::makeShiftSpeedPeriodNode(
						0.0f,
						50,
						ScriptProgramUtil::makeShiftSpeedPeriodNode(
							5.0f,
							5
						)
					)
				}
			).heapClone()
		}
		, b2d1aNode{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, { 5, 3, 2, 1 }),
				SpawnProgramUtil::makeSpawnPosVelNode(
					b2d1aPrototype,
					SpawnProgramUtil::makeEntityPositionNode(),
					SpawnProgramUtil::makeVelocityFromPolarNode(
						SpawnProgramUtil::makeFloatValueNode(2.0f),
						SpawnProgramUtil::randomAngleNode
					)
				)
			)
		}
		, b2d1a{ b2d1aNode, 100, false }

		, b2d1bNode{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, 25),
				SpawnProgramUtil::makeRingFormationNode(
					SpawnProgramUtil::makeVelocityFromPolarNode(
						SpawnProgramUtil::makeFloatValueNode(0.6f),
						SpawnProgramUtil::makeAngleToPlayerNode()
					),
					SpawnProgramUtil::makeIntValueDiffNode({ 10, 14, 17, 20 }),
					SpawnProgramUtil::makeSpawnPosVelNode(
						mediumVioletPrototype,
						SpawnProgramUtil::makeEntityPositionNode()
					)
				)
			)
		}
		, b2d1b{ b2d1bNode, 50, false }

			//2 bullets that spawn horizontal sharps
				//spawned
		, b2d2aPrototype{
			EntityBuilder::makePosVelPrototype(
				SHARP_BULLET_ARGS(sharpHitbox, outbound, L"sharp_violet", 50),
				ScriptProgramList{
					ScriptProgramUtil::makeTimerNode(110,
					ScriptProgramUtil::makeShiftSpeedPeriodNode(
						0.776f,
						30
					))
				}
			).heapClone()
		}
		, b2d2aNode{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(12, { 22, 18, 16, 13 }),
				SpawnProgramUtil::makeMirrorFormationNode(
					SpawnProgramUtil::makeEntityOffsetNode(
						SpawnProgramUtil::makeVelocityValueNode(Vector2{ 5.0f, 0.0f })
					),
					SpawnProgramUtil::makeVelocityValueNode(Velocity{ 0.0f, 0.0f }),
					SpawnProgramUtil::makeSpawnPosVelNode(b2d2aPrototype)
				)
			)
		}
		, b2d2a{ b2d2aNode, 3000, true }

				//spawner
		, b2d2bPrototype{
			EntityBuilder::makePosVelPrototype(
				//for this specific danmaku, large is drawn over sharp
				BASIC_BULLET_ARGS(largeHitbox, outbound, L"large_rose", 100),
				SpawnProgramList{ b2d2a }
			).heapClone()
		}
		, b2d2bNode{
			SpawnProgramUtil::makeRingFormationNode(
				SpawnProgramUtil::makeVelocityFromPolarNode(
					SpawnProgramUtil::makeFloatValueNode(1.85f),
					SpawnProgramUtil::randomAngleNode
				),
				SpawnProgramUtil::makeIntValueDiffNode({ 10, 12, 14, 16 }),
				SpawnProgramUtil::makeSpawnPosVelNode(
					b2d2bPrototype,
					SpawnProgramUtil::makeEntityPositionNode()
				)
			)
		}
		, b2d2b{ b2d2bNode, 1, false }

			//3 fast spiral, 2 directions
		, b2d3aPrototype{
			EntityBuilder::makePosVelPrototype(
				SHARP_BULLET_ARGS(sharpHitbox, outbound, L"sharp_red", 50),
				ScriptProgramList{
					ScriptProgramUtil::makeShiftSpeedPeriodNode(
						0.0f,
						50,
						ScriptProgramUtil::makeShiftSpeedPeriodNode(
							6.5f,
							5
						)
					)
				}
			).heapClone()
		}
		, b2d3bPrototype{
			EntityBuilder::makePosVelPrototype(
				SHARP_BULLET_ARGS(sharpHitbox, outbound, L"sharp_blue", 50),
				ScriptProgramList{
					ScriptProgramUtil::makeShiftSpeedPeriodNode(
						0.0f,
						50,
						ScriptProgramUtil::makeShiftSpeedPeriodNode(
							6.5f,
							5
						)
					)
				}
			).heapClone()
		}
		#define TICKMOD 6
		#define INIT_VEL 1.4f
		#define A_VEL 0.5f
		#define SYM_E 7
		#define SYM_N 12
		#define SYM_H 17
		#define SYM_L 22
		, b2d3aNode{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, TICKMOD),
				SpawnProgramUtil::makeRingFormationNode(
					SpawnProgramUtil::makeVelocityFromPolarNode(
						SpawnProgramUtil::makeFloatValueNode(INIT_VEL),
						SpawnProgramUtil::makeSpiralNode(40, 12.4f, A_VEL)
					),
					SpawnProgramUtil::makeIntValueDiffNode(
						{ SYM_E, SYM_N, SYM_H, SYM_L }
					),
					SpawnProgramUtil::makeSpawnPosVelNode(
						b2d3aPrototype,
						SpawnProgramUtil::makeEntityPositionNode()
					)
				)
			)
		}
		, b2d3a{ b2d3aNode, 40, false }
		, b2d3bNode{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, TICKMOD),
				SpawnProgramUtil::makeRingFormationNode(
					SpawnProgramUtil::makeVelocityFromPolarNode(
						SpawnProgramUtil::makeFloatValueNode(INIT_VEL),
						SpawnProgramUtil::makeSpiralNode(40, 155.4f, -A_VEL)
					),
					SpawnProgramUtil::makeIntValueDiffNode(
						{ SYM_E, SYM_N, SYM_H, SYM_L }
					),
					SpawnProgramUtil::makeSpawnPosVelNode(
						b2d3bPrototype,
						SpawnProgramUtil::makeEntityPositionNode()
					)
				)
			)
		}
		, b2d3b{ b2d3bNode, 40, false }
		#undef TICKMOD
		#undef INIT_VEL
		#undef A_VEL
		#undef SYM_E
		#undef SYM_N
		#undef SYM_H
		#undef SYM_L

		, b2d3ActScriptEnd{
			ScriptProgramUtil::makeAddSpawnNode(b2d3b)
		}
		, b2d3ActScriptStart{
			ScriptProgramUtil::makeAddSpawnNode(b2d3a,
			ScriptProgramUtil::makeStallingIfNode(
				ScriptInstructions::isNotSpawning,
				ScriptProgramUtil::makeTimerNode(25,
				b2d3ActScriptEnd
			))
		)}

			//4 slow spirals + homing arrows
		, b2d4bPrototype{
			EntityBuilder::makePosVelPrototype(
				//this sharp bullet goes above small
				SHARP_BULLET_ARGS(sharpHitbox, outbound, L"sharp_cyan", 101),
				ScriptProgramList{
					ScriptProgramUtil::makeShiftSpeedPeriodNode(
						0.0f,
						100,
						ScriptProgramUtil::makeSetVelocityToPlayerNode(1.5f)
					)
				}
			).heapClone()
		}

		, b2d4Node{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, 12),
				SpawnProgramUtil::makeListNode(
					SpawnProgramUtil::makeRingFormationNode(
						SpawnProgramUtil::makeVelocityFromPolarNode(
							SpawnProgramUtil::makeFloatValueNode(0.69f),
							SpawnProgramUtil::makeSpiralNode(1800, 234.0f, -17.626f)
						),
						SpawnProgramUtil::makeIntValueNode(2),
						SpawnProgramUtil::makeArcFormationNode(
							SpawnProgramUtil::makeIntValueDiffNode({ 1, 2, 3, 4 }),
							SpawnProgramUtil::makeFloatValueNode(18.9f),
							SpawnProgramUtil::makeColumnFormationNode(
								2,
								-0.2f,
								SpawnProgramUtil::makeSpawnPosVelNode(
									smallAzurePrototype,
									SpawnProgramUtil::makeEntityPositionNode()
								)
							)
						)
					),
					SpawnProgramUtil::makeIfNode(
						SpawnProgramUtil::makeTickModNode(0, 11),
						SpawnProgramUtil::makeArcFormationNode(
							Velocity{ 1.45f, 0.0f },
							5,
							12.8f,
							SpawnProgramUtil::makeColumnFormationNode(
								{ 1, 2, 3, 4 },
								-0.37f,
								SpawnProgramUtil::makeVelConsumerMirrorFormationNode(
									SpawnProgramUtil::makeEntityPositionNode(),
									SpawnProgramUtil::makeEntityXNode(),
									SpawnProgramUtil::makeSpawnPosVelNode(
										b2d4bPrototype
									)
								)
							)
						)
					)
				)
			)
		}

		, b2d4{ b2d4Node, 1800, true }

			//5 mega arrow
				//spawned
		, b2d5aPrototype{
			EntityBuilder::makePosVelPrototype(
				SHARP_BULLET_ARGS(sharpHitbox, outbound, L"sharp_red", 50),
				ScriptProgramList{
					ScriptProgramUtil::makeTimerNode(110,
					ScriptProgramUtil::makeShiftSpeedPeriodNode(
						0.976f,
						40
					))
				}
			).heapClone()
		}
		, b2d5aNode{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(12, { 24, 20, 17, 14 }),
				SpawnProgramUtil::makeRingFormationNode(
					SpawnProgramUtil::makeVelocityFromPolarNode(
						SpawnProgramUtil::makeFloatValueNode(0.2f),
						SpawnProgramUtil::randomAngleNode
					),
					SpawnProgramUtil::makeIntValueDiffNode({ 3, 4, 5, 6 }),
					SpawnProgramUtil::makeSpawnPosVelNode(
						b2d5aPrototype,
						SpawnProgramUtil::makeEntityPositionNode()
					)
				)
			)
		}
		, b2d5a{ b2d5aNode, 3000, true }

				//spawner
		, b2d5bPrototype{
			EntityBuilder::makePosVelPrototype(
				SHARP_BULLET_ARGS(sharpHitbox, outbound, L"sharp_red", 51),
				SpawnProgramList{ b2d5a }
			).heapClone()
		}
		, b2d5bNode{
			SpawnProgramUtil::makeRingFormationNode(
				SpawnProgramUtil::makeVelocityFromPolarNode(
					SpawnProgramUtil::makeFloatValueNode(1.85f),
					SpawnProgramUtil::makeAngleToPlayerNode()
				),
				SpawnProgramUtil::makeIntValueNode(6),
				SpawnProgramUtil::makeSpawnPosVelNode(
					b2d5bPrototype,
					SpawnProgramUtil::makeEntityPositionNode()
				)
			)
		}
		, b2d5b{ b2d5bNode, 1, false }

		, b2e1Prototype{
			EntityBuilder::makeLinearUncollidable(
				BOSS_ARGS,
				SpriteInstruction{
					bitmapStoragePointer->get(L"b2_idle_1")->d2dBitmap,
					math::Vector2{ 0.0f, 4.0f }			//offset
				},
				DrawOrder{ config::playerDrawOrder },
				AnimationList{ 
					{
						components::Animation{ {
							L"b2_idle_1", L"b2_idle_2", L"b2_idle_3", L"b2_idle_4"
						} }
					},
					0,	//idle index
					4	//ticks
				},
				ScriptProgramList{
					ScriptProgramUtil::makeBossEntryNode(120, L"2_1",
					ScriptProgramUtil::makeSetHealthNode(4500,
					ScriptProgramUtil::makeDoubleBossAttackAndMoveNode(
						b2d1a,
						30, 
						30,
						b2d1b,
						10,
						60,
					ScriptProgramUtil::makeBossResetNode(90,
					ScriptProgramUtil::makeSetHealthNode(4800,
					ScriptProgramUtil::makeBossAttackAndMoveNode(
						b2d2b,
						40,
						150,
					ScriptProgramUtil::makeAddSpawnNode(
						pickupProgramsPointer->largePowerSpawnProgram,
					ScriptProgramUtil::makeTimerNode(1,
					ScriptProgramUtil::makeBossResetMidNode(90,
					ScriptProgramUtil::makeSetHealthNode(2000,
					ScriptProgramUtil::makeBossActAndMoveNode(
						10,
						30,
						b2d3ActScriptStart,
						b2d3ActScriptEnd,
					ScriptProgramUtil::makeBossResetMidNode(90,
					ScriptProgramUtil::makeSetHealthNode(6000,
					ScriptProgramUtil::makeAddSpawnNode(b2d4,	//<---- 4 is here
					ScriptProgramUtil::makeStallingIfNode(
						ScriptProgramUtil::makeIsBossDeadNode(),
						ScriptProgramUtil::makeAddSpawnNode(
							pickupProgramsPointer->largePowerSpawnProgram,
						ScriptProgramUtil::makeTimerNode(1,
						ScriptProgramUtil::makeBossResetNode(120,
						ScriptProgramUtil::makeSetHealthNode(5500,
						ScriptProgramUtil::makeBossAttackAndMoveNode(
							b2d5b,
							40,
							150,
						ScriptProgramUtil::makeBossEndNode()
							
							)
					)))
					
					))))))))))))))))
				}
			).heapClone()
		}
		, b2e1Node{
			SpawnProgramUtil::makeSpawnNode(b2e1Prototype)
		}
		, b2e1{ b2e1Node, 1, false }


		, stage2ScriptProgram{
			ScriptProgramUtil::makeTimerNode(290,
			ScriptProgramUtil::makeSetSpawnNode(s2e1,
			ScriptProgramUtil::makeTimerNode(220,
			ScriptProgramUtil::makeAddSpawnNode(s2e2,
			ScriptProgramUtil::makeTimerNode(250,
			ScriptProgramUtil::makeAddSpawnNode(s2e3,
			ScriptProgramUtil::makeTimerNode(330,
			ScriptProgramUtil::makeAddSpawnNode(s2e4,
			ScriptProgramUtil::makeTimerNode(700,
			ScriptProgramUtil::makeAddSpawnNode(s2e5,
			ScriptProgramUtil::makeTimerNode(480,
			ScriptProgramUtil::makeAddSpawnNode(s2e6a,
			ScriptProgramUtil::makeTimerNode(30,
			ScriptProgramUtil::makeAddSpawnNode(s2e6b,
			ScriptProgramUtil::makeTimerNode(460,
			ScriptProgramUtil::makeAddSpawnNode(s2e7,
			ScriptProgramUtil::makeTimerNode(180,
			ScriptProgramUtil::makeAddSpawnNode(s2e8a,
			ScriptProgramUtil::makeTimerNode(50,
			ScriptProgramUtil::makeAddSpawnNode(s2e8b,
			ScriptProgramUtil::makeTimerNode(50,
			ScriptProgramUtil::makeAddSpawnNode(s2e8c,
			ScriptProgramUtil::makeTimerNode(250,
			ScriptProgramUtil::makeAddSpawnNode(s2e9,
			ScriptProgramUtil::makeTimerNode(360,
			ScriptProgramUtil::makeAddSpawnNode(s2e10,
			ScriptProgramUtil::makeTimerNode(760,
			ScriptProgramUtil::makeAddSpawnNode(s2e11,
			ScriptProgramUtil::makeTimerNode(400,
			ScriptProgramUtil::makeAddSpawnNode(s2e12,
			ScriptProgramUtil::makeTimerNode(570,
			ScriptProgramUtil::makeAddSpawnNode(b2e1,
			ScriptProgramUtil::makeStallingIfNode(
				ScriptProgramUtil::makeIsWinNode(),
				ScriptProgramUtil::makeTimerNode(waitEndTime,
				ScriptProgramUtil::makeShowDialogueNode(L"2_2",
				ScriptProgramUtil::makeStallingIfNode(
					ScriptProgramUtil::makeIsDialogueOverNode(),
					ScriptProgramUtil::makeTimerNode(1,
					ScriptProgramUtil::makeEndStageNode()
				)))
			)
			)))))))))))))))))))))))))))))))))

			
		}

		// STAGE 3 // STAGE 3 // STAGE 3 // STAGE 3 // STAGE 3 // STAGE 3 // STAGE 3 //

		#define COUNT_E 25
		#define COUNT_N 50
		#define COUNT_H 70
		#define COUNT_L 90
		#define SPEED_MIN 0.4f
		#define SPEED_MAX 1.8f

		, s3d1aNode{
			SpawnProgramUtil::makeRepeatNode(
				SpawnProgramUtil::makeIntValueDiffNode(
					{ COUNT_E, COUNT_N, COUNT_H, COUNT_L }
				),
				SpawnProgramUtil::makeSpawnPosVelNode(
					smallSpringPrototype,
					SpawnProgramUtil::makeEntityPositionNode(),
					SpawnProgramUtil::makeUniformRandomCircleVelocityNode(
						SPEED_MIN, SPEED_MAX
					)
				)
			)
		}
		, s3d1bNode{
			SpawnProgramUtil::makeRepeatNode(
				SpawnProgramUtil::makeIntValueDiffNode(
					{ COUNT_E, COUNT_N, COUNT_H, COUNT_L }
				),
				SpawnProgramUtil::makeSpawnPosVelNode(
					smallGreenPrototype,
					SpawnProgramUtil::makeEntityPositionNode(),
					SpawnProgramUtil::makeUniformRandomCircleVelocityNode(
						SPEED_MIN, SPEED_MAX
					)
				)
			)
		}
		, s3d1cNode{
			SpawnProgramUtil::makeRepeatNode(
				SpawnProgramUtil::makeIntValueDiffNode(
					{ COUNT_E, COUNT_N, COUNT_H, COUNT_L }
				),
				SpawnProgramUtil::makeSpawnPosVelNode(
					smallYellowPrototype,
					SpawnProgramUtil::makeEntityPositionNode(),
					SpawnProgramUtil::makeUniformRandomCircleVelocityNode(
						SPEED_MIN, SPEED_MAX
					)
				)
			)
		}
		, s3d1a{ s3d1aNode, 1, false }
		, s3d1b{ s3d1bNode, 1, false }
		, s3d1c{ s3d1cNode, 1, false }
		#undef COUNT_E
		#undef COUNT_N
		#undef COUNT_H
		#undef COUNT_L
		#undef SPEED_MIN
		#undef SPEED_MAX

		#define HP 700
		, s3e1aPrototype{
			EntityBuilder::makePosPrototype(
				{ 2.0f, -90.0f },
				WISP_ARGS(HP, pickupProgramsPointer->smallPowerSpawnProgram),
				ScriptProgramList{
					ScriptProgramUtil::makeShootOnceAndLeaveProgram(
						20,
						30,
						5,
						s3d1a,
						50,
						{ 1.0f, 90.0f},
						100
					)
				}
			).heapClone()
		}
		, s3e1bPrototype{
			EntityBuilder::makePosPrototype(
				{ 1.7f, -90.0f },
				WISP_ARGS(HP, pickupProgramsPointer->smallPowerSpawnProgram),
				ScriptProgramList{
					ScriptProgramUtil::makeShootOnceAndLeaveProgram(
						20,
						30,
						5,
						s3d1b,
						50,
						{ 1.0f, 90.0f},
						100
					)
				}
			).heapClone()
		}
		, s3e1cPrototype{
			EntityBuilder::makePosPrototype(
				{ 1.4f, -90.0f },
				WISP_ARGS(HP, pickupProgramsPointer->smallPowerSpawnProgram),
				ScriptProgramList{
					ScriptProgramUtil::makeShootOnceAndLeaveProgram(
						20,
						30,
						5,
						s3d1c,
						50,
						{ 1.0f, 90.0f},
						100
					)
				}
			).heapClone()
		}
		#undef HP
		, s3e1aNode{
			SpawnProgramUtil::makeMirrorPosFormationNode(
				SpawnProgramUtil::makePointValueNode({
					20.0f + config::gameOffset.x,
					config::topOut
				}),
				SpawnProgramUtil::makeFloatValueNode(
					config::gameWidth / 2.0f + config::gameOffset.x
				),
				SpawnProgramUtil::makeSpawnPosNode(s3e1aPrototype)
			)
		}
		, s3e1bNode{
			SpawnProgramUtil::makeMirrorPosFormationNode(
				SpawnProgramUtil::makePointValueNode({
					35.0f + config::gameOffset.x,
					config::topOut
				}),
				SpawnProgramUtil::makeFloatValueNode(
					config::gameWidth / 2.0f + config::gameOffset.x
				),
				SpawnProgramUtil::makeSpawnPosNode(s3e1bPrototype)
			)
		}
		, s3e1cNode{
			SpawnProgramUtil::makeMirrorPosFormationNode(
				SpawnProgramUtil::makePointValueNode({
					50.0f + config::gameOffset.x,
					config::topOut
				}),
				SpawnProgramUtil::makeFloatValueNode(
					config::gameWidth / 2.0f + config::gameOffset.x
				),
				SpawnProgramUtil::makeSpawnPosNode(s3e1cPrototype)
			)
		}
		, s3e1a{ s3e1aNode, 1, false }
		, s3e1b{ s3e1bNode, 1, false }
		, s3e1c{ s3e1cNode, 1, false }

			//wave 2
		, s3d2Node{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(
					SpawnProgramUtil::makeEntityUniformRandomIntNode(
						SpawnProgramUtil::makeIntValueNode(0),
						SpawnProgramUtil::makeIntValueDiffNode({ 50, 40, 32, 26 })
					), 
					{ 50, 40, 32, 26 }
				),
				SpawnProgramUtil::makeIfElseNode(
					SpawnProgramUtil::makeTickModNode(0, 2),
					SpawnProgramUtil::makeSpawnPosVelNode(
						smallMagentaPrototype,
						SpawnProgramUtil::makeEntityPositionNode(),
						SpawnProgramUtil::makeVelocityFromPolarNode(
							SpawnProgramUtil::makeUniformRandomFloatNode(1.0f, 1.5f),
							SpawnProgramUtil::makeAddNode(
								SpawnProgramUtil::makeFloatValueNode(-90.0f),
								SpawnProgramUtil::makeUniformRandomFloatNode(-2.0f, 2.0f)
							)
						)
					),
					SpawnProgramUtil::makeListNode(
						SpawnProgramUtil::makeSpawnPosVelNode(
							sharpRosePrototype,
							SpawnProgramUtil::makeEntityPositionNode(),
							SpawnProgramUtil::makeVelocityFromPolarNode(
								SpawnProgramUtil::makeFloatValueNode(1.5f),
								SpawnProgramUtil::makeAddNode(
									SpawnProgramUtil::makeEntityAngleNode(),
									SpawnProgramUtil::makeFloatValueNode(90.0f)
								)
							)
						),
						SpawnProgramUtil::makeSpawnPosVelNode(
							sharpRosePrototype,
							SpawnProgramUtil::makeEntityPositionNode(),
							SpawnProgramUtil::makeVelocityFromPolarNode(
								SpawnProgramUtil::makeFloatValueNode(1.5f),
								SpawnProgramUtil::makeAddNode(
									SpawnProgramUtil::makeEntityAngleNode(),
									SpawnProgramUtil::makeFloatValueNode(-90.0f)
								)
							)
						)
					)
				)
			)
		}
		, s3d2{ s3d2Node, 5000, false }

		#define SPEED 4.0f
		#define INIT_ANGLE -150.0f
		, s3e2Prototype{
			EntityBuilder::makePosVelPrototype(
				FLAME_ARGS(120, pickupProgramsPointer->smallPowerThirdSpawnProgram),
				SpawnProgramList{ s3d2 },
				ScriptProgramList{
					ScriptProgramUtil::makeStallingIfNode(
						ScriptProgramUtil::makeBoundaryXLowNode(
							100.0f + config::gameOffset.x
						),
						ScriptProgramUtil::makeShiftVelocityTurnLongPeriodNode(
							Velocity{ SPEED, 0.0f },
							INIT_ANGLE,
							30,
							ScriptProgramUtil::makeShiftVelocityTurnLongPeriodNode(
								Velocity{ SPEED, math::Angle{ INIT_ANGLE }.flipX() },
								0.0f,
								30
							)
						)
					)
				}
			).heapClone()
		}
		, s3e2Node{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, 7),
				SpawnProgramUtil::makeSpawnPosVelNode(
					s3e2Prototype,
					SpawnProgramUtil::makePointValueNode({ config::rightOut, 40.0f }),
					SpawnProgramUtil::makeVelocityValueNode(Velocity{ SPEED, INIT_ANGLE })
				)
			)
		}
		, s3e2{ s3e2Node, 260, false }
		#undef SPEED
		#undef INIT_ANGLE

		#define SYM_E 14
		#define SYM_N 19
		#define SYM_H 23
		#define SYM_L 27
		, s3d3Node{
			SpawnProgramUtil::makeRingFormationNode(
				SpawnProgramUtil::makeVelocityFromPolarNode(
					SpawnProgramUtil::makeFloatValueNode(1.25f),
					SpawnProgramUtil::randomAngleNode
				),
				SpawnProgramUtil::makeIntValueDiffNode({ SYM_E, SYM_N, SYM_H, SYM_L }),
				SpawnProgramUtil::makeColumnFormationNode(
					{ 1, 1, 2, 2 },
					-0.29f,
					SpawnProgramUtil::makeSpawnPosVelNode(
						smallBluePrototype,
						SpawnProgramUtil::makeEntityPositionNode()
					)
				)
			)
		}
		, s3d3{ s3d3Node, 1, false }

		#define SPEED 1.8f
		, s3e3Prototype{
			EntityBuilder::makePosVelPrototype(
				CLOUD_ARGS(400, pickupProgramsPointer->smallPowerSpawnProgram),
				ScriptProgramList{
					ScriptProgramUtil::makeTimerNode(10,
					ScriptProgramUtil::makeShiftSpeedPeriodNode(0.3f, 50,
					ScriptProgramUtil::makeSetSpawnNode(s3d3,
					ScriptProgramUtil::makeTimerNode(2,
					ScriptProgramUtil::makeStallingIfNode(
						ScriptInstructions::isNotSpawning,
						ScriptProgramUtil::makeShiftSpeedPeriodNode(SPEED, 90
					))))))
				}
			).heapClone()
		}
		, s3e3Node{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, 25),
				SpawnProgramUtil::makeSpawnPosVelNode(
					s3e3Prototype,
					SpawnProgramUtil::makePointFromFloatsNode(
						SpawnProgramUtil::makeFloatValueNode(config::leftOut),
						SpawnProgramUtil::makeUniformRandomFloatNode(30.0f, 70.0f)
					),
					SpawnProgramUtil::makeVelocityFromPolarNode(
						SpawnProgramUtil::makeAddNode(
							SpawnProgramUtil::makeFloatValueNode(SPEED),
							SpawnProgramUtil::makeUniformRandomFloatNode(-0.3f, 0.3f)
						),
						SpawnProgramUtil::makeFloatValueNode(0.0f)
					)
				)
			)
		}
		, s3e3{ s3e3Node, 100, false }

			//wave 4

		, s3d4Node{
			SpawnProgramUtil::makeRingFormationNode(
				SpawnProgramUtil::makeVelocityFromPolarNode(
					SpawnProgramUtil::makeFloatValueNode(1.25f),
					SpawnProgramUtil::randomAngleNode
				),
				SpawnProgramUtil::makeIntValueDiffNode({ SYM_E, SYM_N, SYM_H, SYM_L }),
				SpawnProgramUtil::makeColumnFormationNode(
					{ 1, 1, 2, 2 },
					-0.29f,
					SpawnProgramUtil::makeSpawnPosVelNode(
						smallRedPrototype,
						SpawnProgramUtil::makeEntityPositionNode()
					)
				)
			)
		}
		, s3d4{ s3d4Node, 1, false }

		, s3e4Prototype{
			EntityBuilder::makePosVelPrototype(
				CLOUD_ARGS(400, pickupProgramsPointer->smallPowerSpawnProgram),
				ScriptProgramList{
					ScriptProgramUtil::makeTimerNode(10,
					ScriptProgramUtil::makeShiftSpeedPeriodNode(0.3f, 50,
					ScriptProgramUtil::makeSetSpawnNode(s3d4,
					ScriptProgramUtil::makeTimerNode(2,
					ScriptProgramUtil::makeStallingIfNode(
						ScriptInstructions::isNotSpawning,
						ScriptProgramUtil::makeShiftSpeedPeriodNode(SPEED, 90
					))))))
				}
			).heapClone()
		}
		, s3e4Node{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, 25),
				SpawnProgramUtil::makeSpawnPosVelNode(
					s3e4Prototype,
					SpawnProgramUtil::makePointFromFloatsNode(
						SpawnProgramUtil::makeFloatValueNode(config::rightOut),
						SpawnProgramUtil::makeUniformRandomFloatNode(30.0f, 70.0f)
					),
					SpawnProgramUtil::makeVelocityFromPolarNode(
						SpawnProgramUtil::makeAddNode(
							SpawnProgramUtil::makeFloatValueNode(SPEED),
							SpawnProgramUtil::makeUniformRandomFloatNode(-0.3f, 0.3f)
						),
						SpawnProgramUtil::makeFloatValueNode(180.0f)
					)
				)
			)
		}
		, s3e4{ s3e4Node, 175, false }


		#undef SPEED
		#undef SYM_E
		#undef SYM_N
		#undef SYM_H
		#undef SYM_L

			//wave 5
		#define SPEED 4.0f
		#define INIT_ANGLE -30.0f
		, s3e5Prototype{
			EntityBuilder::makePosVelPrototype(
				FLAME_ARGS(120, pickupProgramsPointer->smallPowerThirdSpawnProgram),
				SpawnProgramList{ s3d2 },
				ScriptProgramList{
					ScriptProgramUtil::makeStallingIfNode(
						ScriptProgramUtil::makeBoundaryXHighNode(
							config::gameWidth - 100.0f + config::gameOffset.x
						),
						ScriptProgramUtil::makeShiftVelocityTurnLongPeriodNode(
							Velocity{ SPEED, 180.0f },
							INIT_ANGLE,
							30,
							ScriptProgramUtil::makeShiftVelocityTurnLongPeriodNode(
								Velocity{ SPEED, math::Angle{ INIT_ANGLE }.flipX() },
								180.0f,
								30
							)
						)
					)
				}
			).heapClone()
		}
		, s3e5Node{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, 7),
				SpawnProgramUtil::makeSpawnPosVelNode(
					s3e5Prototype,
					SpawnProgramUtil::makePointValueNode({ config::leftOut, 40.0f }),
					SpawnProgramUtil::makeVelocityValueNode(Velocity{ SPEED, INIT_ANGLE })
				)
			)
		}
		, s3e5{ s3e5Node, 140, false }
		#undef SPEED
		#undef INIT_ANGLE

			//wave 6: same as wave 1

			//wave 7

		#define SPEED 1.8f
		, s3e7Node{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, 25),
				SpawnProgramUtil::makeSpawnPosVelNode(
					s3e3Prototype,
					SpawnProgramUtil::makePointFromFloatsNode(
						SpawnProgramUtil::makeFloatValueNode(config::rightOut),
						SpawnProgramUtil::makeUniformRandomFloatNode(30.0f, 70.0f)
					),
					SpawnProgramUtil::makeVelocityFromPolarNode(
						SpawnProgramUtil::makeAddNode(
							SpawnProgramUtil::makeFloatValueNode(SPEED),
							SpawnProgramUtil::makeUniformRandomFloatNode(-0.3f, 0.3f)
						),
						SpawnProgramUtil::makeFloatValueNode(180.0f)
					)
				)
			)
		}
		#undef SPEED
		, s3e7{ s3e7Node, 150, false }

			//wave 8
		#define INIT_ANGLE -45.0f
		, s3e8Prototype{
			EntityBuilder::makePosVelPrototype(
				WISP_ARGS(600, pickupProgramsPointer->smallPowerSpawnProgram),
				ScriptProgramList{
					ScriptProgramUtil::makeShootOnceAndLeaveTurningProgram(
						20,
						30,
						5,
						s3d1a,
						30,
						{ 1.0f, 45.0f},
						INIT_ANGLE,
						80
					)
				}
			).heapClone()
		}


		, s3e8aNode{
			SpawnProgramUtil::makeSpawnPosVelNode(
				s3e8Prototype,
				SpawnProgramUtil::makePointValueNode(Point2{ config::leftOut, 40.0f }),
				SpawnProgramUtil::makeVelocityValueNode(Velocity{ 2.0f, INIT_ANGLE})
			)
		}
		, s3e8bNode{
			SpawnProgramUtil::makeSpawnPosVelNode(
				s3e8Prototype,
				SpawnProgramUtil::makePointValueNode(Point2{ config::leftOut, 20.0f }),
				SpawnProgramUtil::makeVelocityValueNode(Velocity{ 1.7f, INIT_ANGLE})
			)
		}
		, s3e8cNode{
			SpawnProgramUtil::makeSpawnPosVelNode(
				s3e8Prototype,
				SpawnProgramUtil::makePointValueNode(Point2{ config::leftOut, 0.0f }),
				SpawnProgramUtil::makeVelocityValueNode(Velocity{ 1.4f, INIT_ANGLE})
			)
		}
		#undef INIT_ANGLE
		, s3e8a{ s3e8aNode, 1, false }
		, s3e8b{ s3e8bNode, 1, false }
		, s3e8c{ s3e8cNode, 1, false }

			//wave 9 - midboss
		#define SHOOT_TIME 520
				//side
		, s3d9aNode{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, { 5, 3, 2, 1 }),
				SpawnProgramUtil::makeSpawnPosVelNode(
					smallAzurePrototype,
					SpawnProgramUtil::makeEntityPositionNode(),
					SpawnProgramUtil::makeUniformRandomCircleVelocityNode(0.35f, 2.0f)
				)
			)
		}
		, s3d9a{ s3d9aNode, SHOOT_TIME, false }
		, s3e9aPrototype{
			EntityBuilder::makePosPrototype(
				{ 1.1f, -90.0f },
				CLOUD_ARGS(500, pickupProgramsPointer->largePowerSpawnProgram),
				ScriptProgramList{
					ScriptProgramUtil::makeShootOnceAndLeaveProgram(
						20,
						40,
						60,
						s3d9a,
						80,
						{ 1.0f, 90.0f},
						120
					)
				}
			).heapClone()
		}
				//center
		, s3d9bNode{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, { 7, 5, 3, 2 }),
				SpawnProgramUtil::makeSpawnPosVelNode(
					mediumBluePrototype,
					SpawnProgramUtil::makeEntityPositionNode(),
					SpawnProgramUtil::makeUniformRandomCircleVelocityNode(0.35f, 2.0f)
				)
			)
		}
		, s3d9b{ s3d9bNode, SHOOT_TIME, false }
		#undef SHOOT_TIME
		, s3e9bPrototype{
			EntityBuilder::makePosPrototype(
				{ 1.1f, -90.0f },
				CLOUD_ARGS(2800, pickupProgramsPointer->lifeSpawnProgram),
				ScriptProgramList{
					ScriptProgramUtil::makeShootOnceAndLeaveProgram(
						20,
						40,
						60,
						s3d9b,
						80,
						{ 1.0f, 90.0f},
						120
					)
				}
			).heapClone()
		}
		#define OFFSET 30.0f
		, s3e9Node{
			SpawnProgramUtil::makeListNode(
				SpawnProgramUtil::makeSpawnPosNode(
					s3e9bPrototype,
					SpawnProgramUtil::makePointValueNode(
						Point2{ 
							config::gameWidth / 2.0f + config::gameOffset.x,
							config::topOut
						}
					)
				),
				SpawnProgramUtil::makeMirrorPosFormationNode(
					SpawnProgramUtil::makePointValueNode(
						Point2{ 
							config::gameWidth / 2.0f + config::gameOffset.x + OFFSET,
							config::topOut
						}
					),
					SpawnProgramUtil::makeFloatValueNode(
						config::gameWidth / 2.0f + config::gameOffset.x
					),
					SpawnProgramUtil::makeSpawnPosNode(s3e9aPrototype)
				),
				SpawnProgramUtil::makeMirrorPosFormationNode(
					SpawnProgramUtil::makePointValueNode(
						Point2{ 
							config::gameWidth / 2.0f + config::gameOffset.x + 2 * OFFSET,
							config::topOut
						}
					),
					SpawnProgramUtil::makeFloatValueNode(
						config::gameWidth / 2.0f + config::gameOffset.x
					),
					SpawnProgramUtil::makeSpawnPosNode(s3e9aPrototype)
				)
			)
		}
		#undef OFFSET
		, s3e9{ s3e9Node, 1, false }

			//wave 10
		, s3d10Node{
			#define SPEED 1.25f
			#define AV -1.4423f
			#define OFFSET 30.0f
			#define ANGLE_INC 20.0f
			#define PROTOTYPE sharpSpringPrototype
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, {22, 17, 13, 10}),
				SpawnProgramUtil::makeListNode(
					SpawnProgramUtil::makeArcFormationNode(
						SpawnProgramUtil::makeVelocityFromPolarNode(
							SpawnProgramUtil::makeFloatValueNode(SPEED),
							SpawnProgramUtil::makeSpiralNode(
								SpawnProgramUtil::makeIntValueNode(2400),
								SpawnProgramUtil::makeFloatValueNode(0.0f),
								SpawnProgramUtil::makeFloatValueNode(AV)
							)
						),
						3,		//symmetry
						ANGLE_INC,
						SpawnProgramUtil::makeSpawnPosVelNode(
							PROTOTYPE,
							SpawnProgramUtil::makeEntityOffsetNode(
								SpawnProgramUtil::makeVelocityFromPolarNode(
									SpawnProgramUtil::makeFloatValueNode(OFFSET),
									SpawnProgramUtil::makeSpiralNode(
										SpawnProgramUtil::makeIntValueNode(2400),
										SpawnProgramUtil::makeFloatValueNode(-90.0f),
										SpawnProgramUtil::makeFloatValueNode(AV)
									)
								)
							)
						)
					),
					SpawnProgramUtil::makeArcFormationNode(
						SpawnProgramUtil::makeVelocityFromPolarNode(
							SpawnProgramUtil::makeFloatValueNode(SPEED),
							SpawnProgramUtil::makeSpiralNode(
								SpawnProgramUtil::makeIntValueNode(2400),
								SpawnProgramUtil::makeFloatValueNode(90.0f),
								SpawnProgramUtil::makeFloatValueNode(AV)
							)
						),
						3,		//symmetry
						ANGLE_INC,
						SpawnProgramUtil::makeSpawnPosVelNode(
							PROTOTYPE,
							SpawnProgramUtil::makeEntityOffsetNode(
								SpawnProgramUtil::makeVelocityFromPolarNode(
									SpawnProgramUtil::makeFloatValueNode(OFFSET),
									SpawnProgramUtil::makeSpiralNode(
										SpawnProgramUtil::makeIntValueNode(2400),
										SpawnProgramUtil::makeFloatValueNode(0.0f),
										SpawnProgramUtil::makeFloatValueNode(AV)
									)
								)
							)
						)
					),
					SpawnProgramUtil::makeArcFormationNode(
						SpawnProgramUtil::makeVelocityFromPolarNode(
							SpawnProgramUtil::makeFloatValueNode(SPEED),
							SpawnProgramUtil::makeSpiralNode(
								SpawnProgramUtil::makeIntValueNode(2400),
								SpawnProgramUtil::makeFloatValueNode(180.0f),
								SpawnProgramUtil::makeFloatValueNode(AV)
							)
						),
						3,		//symmetry
						ANGLE_INC,
						SpawnProgramUtil::makeSpawnPosVelNode(
							PROTOTYPE,
							SpawnProgramUtil::makeEntityOffsetNode(
								SpawnProgramUtil::makeVelocityFromPolarNode(
									SpawnProgramUtil::makeFloatValueNode(OFFSET),
									SpawnProgramUtil::makeSpiralNode(
										SpawnProgramUtil::makeIntValueNode(2400),
										SpawnProgramUtil::makeFloatValueNode(90.0f),
										SpawnProgramUtil::makeFloatValueNode(AV)
									)
								)
							)
						)
					),
					SpawnProgramUtil::makeArcFormationNode(
						SpawnProgramUtil::makeVelocityFromPolarNode(
							SpawnProgramUtil::makeFloatValueNode(SPEED),
							SpawnProgramUtil::makeSpiralNode(
								SpawnProgramUtil::makeIntValueNode(2400),
								SpawnProgramUtil::makeFloatValueNode(270.0f),
								SpawnProgramUtil::makeFloatValueNode(AV)
							)
						),
						3,		//symmetry
						ANGLE_INC,
						SpawnProgramUtil::makeSpawnPosVelNode(
							PROTOTYPE,
							SpawnProgramUtil::makeEntityOffsetNode(
								SpawnProgramUtil::makeVelocityFromPolarNode(
									SpawnProgramUtil::makeFloatValueNode(OFFSET),
									SpawnProgramUtil::makeSpiralNode(
										SpawnProgramUtil::makeIntValueNode(2400),
										SpawnProgramUtil::makeFloatValueNode(180.0f),
										SpawnProgramUtil::makeFloatValueNode(AV)
									)
								)
							)
						)
					)
				)
			)
			#undef SPEED
			#undef AV
			#undef OFFSET
			#undef ANGLE_INC
			#undef PROTOTYPE
		}

		, s3d10{ s3d10Node, 340, false }
		, s3e10Prototype{
			EntityBuilder::makeLinearCollidable(
				{ config::leftOut, 60.0f },
				{ 1.5f, -25.0f },
				WISP_ARGS(2220, pickupProgramsPointer->largePowerSpawnProgram),
				ScriptProgramList{
					{
						ScriptProgramUtil::makeTimerNode(40,
						ScriptProgramUtil::makeSetSpawnNode(s3d10
						))
					},
					{
						ScriptProgramUtil::makeShiftVelocityTurnPeriodNode(
							Velocity{ 0.0f, 0.0f },
							-25.0f,
							150,
							ScriptProgramUtil::makeTimerNode(80,
								ScriptProgramUtil::makeShiftVelocityTurnPeriodNode(
									Velocity{ 1.5f, 25.0f },
									0.0f,
									150
								)
							)
						)
					}
				}
			).heapClone()
		}
		, s3e10Node{
			SpawnProgramUtil::makeSpawnNode(s3e10Prototype)
		}
		, s3e10{ s3e10Node, 1, false }

		, s3d11Node{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(
					SpawnProgramUtil::makeEntityUniformRandomIntNode(
						SpawnProgramUtil::makeIntValueNode(0),
						SpawnProgramUtil::makeIntValueDiffNode({ 130, 100, 70, 40 })
					),
					{ 130, 100, 70, 40 }
				),
				SpawnProgramUtil::makeSpawnPosVelNode(
					sharpMagentaPrototype,
					SpawnProgramUtil::makeEntityPositionNode(),
					SpawnProgramUtil::makeVelocityFromPolarNode(
						SpawnProgramUtil::makeFloatValueDiffNode(
							{1.5f, 1.8f, 2.1f, 2.5f}
						),
						SpawnProgramUtil::makeAngleToPlayerNode()
					)
				)
			)
		}
		, s3d11{ s3d11Node, 1000, true }
		, s3e11Prototype{
			EntityBuilder::makePosVelPrototype(
				BAT_ARGS(60, pickupProgramsPointer->smallPowerThirdSpawnProgram),
				SpawnProgramList{ s3d11 }
			).heapClone()
		}
		, s3e11Node{
			SpawnProgramUtil::makeListNode(
				SpawnProgramUtil::makeIfNode(
					SpawnProgramUtil::makeTickModNode(0, 14),
					SpawnProgramUtil::makeSpawnPosVelNode(
						s3e11Prototype,
						SpawnProgramUtil::makePointValueNode({ config::leftOut, 25.0f }),
						SpawnProgramUtil::makeVelocityValueNode(Velocity{ 2.0f, -15.0f })
					)
				),
				SpawnProgramUtil::makeIfNode(
					SpawnProgramUtil::makeTickModNode(7, 14),
					SpawnProgramUtil::makeSpawnPosVelNode(
						s3e11Prototype,
						SpawnProgramUtil::makePointValueNode({ config::leftOut, 50.0f }),
						SpawnProgramUtil::makeVelocityValueNode(Velocity{ 2.0f, -15.0f })
					)
				)
			)
		}
		, s3e11{ s3e11Node, 290, false }

			//wave 12

		, s3e12Node{
			SpawnProgramUtil::makeListNode(
				SpawnProgramUtil::makeIfNode(
					SpawnProgramUtil::makeTickModNode(0, 14),
					SpawnProgramUtil::makeSpawnPosVelNode(
						s3e11Prototype,
						SpawnProgramUtil::makePointValueNode({ config::rightOut, 25.0f }),
						SpawnProgramUtil::makeVelocityValueNode(
							Velocity{ 2.0f, Angle{ -15.0f }.flipY() }
						)
					)
				),
				SpawnProgramUtil::makeIfNode(
					SpawnProgramUtil::makeTickModNode(7, 14),
					SpawnProgramUtil::makeSpawnPosVelNode(
						s3e11Prototype,
						SpawnProgramUtil::makePointValueNode({ config::rightOut, 50.0f }),
						SpawnProgramUtil::makeVelocityValueNode(
							Velocity{ 2.0f, Angle{ -15.0f }.flipY() }
						)
					)
				)
			)
		}
		, s3e12{ s3e12Node, 290, false }

			//wave 13
		, s3d13Node{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, { 14, 11, 8, 5 }),
				SpawnProgramUtil::makeArcFormationNode(
					SpawnProgramUtil::makeVelocityFromPolarNode(
						SpawnProgramUtil::makeWhipNode(45, 0.8f, 5.0f),
						SpawnProgramUtil::makeAngleToPlayerNode()
					),
					SpawnProgramUtil::makeIntValueDiffNode({ 1, 1, 3, 3 }),
					SpawnProgramUtil::makeFloatValueNode(29.4f),
					SpawnProgramUtil::makeSpawnPosVelNode(
						smallMagentaPrototype,
						SpawnProgramUtil::makeEntityPositionNode()
					)
				)
			)
		}
		, s3d13{ s3d13Node, 45, false }

		, s3e13Prototype{
			EntityBuilder::makePosVelPrototype(
				FLAME_ARGS(350, pickupProgramsPointer->smallPowerThirdSpawnProgram),
				ScriptProgramList{
					ScriptProgramUtil::makeShootOnceAndLeaveProgram(
						20,
						30,
						2,
						s3d13,
						30,
						Velocity{ 1.2f, -90.0f },
						70
					)
				}
			).heapClone()
		}

		, s3e13Node{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, 20),
				SpawnProgramUtil::makeSpawnPosVelNode(
					s3e13Prototype,
					SpawnProgramUtil::makeUniformRandomRectanglePointNode(
						10.0f + config::gameOffset.x, 
						65.0f + config::gameOffset.x, 
						config::topOut, 
						config::topOut + 1.0f
					),
					SpawnProgramUtil::makeVelocityFromPolarNode(
						SpawnProgramUtil::makeUniformRandomFloatNode(0.9f, 2.4f),
						SpawnProgramUtil::makeFloatValueNode(-90.0f)
					)
				)
			)
		}

		, s3e13{ s3e13Node, 250, false }

			//wave 14

		, s3e14Node{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, 20),
				SpawnProgramUtil::makeSpawnPosVelNode(
					s3e13Prototype,
					SpawnProgramUtil::makeUniformRandomRectanglePointNode(
						config::gameWidth - 65.0f + config::gameOffset.x, 
						config::gameWidth - 10.0f + config::gameOffset.x, 
						config::topOut, 
						config::topOut + 1.0f
					),
					SpawnProgramUtil::makeVelocityFromPolarNode(
						SpawnProgramUtil::makeUniformRandomFloatNode(0.9f, 2.4f),
						SpawnProgramUtil::makeFloatValueNode(-90.0f)
					)
				)
			)
		}

		, s3e14{ s3e14Node, 250, false }

			//wave 15
		, s3e15Node{
			#define SPEED 1.8f
			SpawnProgramUtil::makeListNode(
				SpawnProgramUtil::makeIfNode(
					SpawnProgramUtil::makeTickModNode(0, 40),
					SpawnProgramUtil::makeSpawnPosVelNode(
						s3e3Prototype,
						SpawnProgramUtil::makePointFromFloatsNode(
							SpawnProgramUtil::makeFloatValueNode(config::leftOut),
							SpawnProgramUtil::makeUniformRandomFloatNode(30.0f, 70.0f)
						),
						SpawnProgramUtil::makeVelocityFromPolarNode(
							SpawnProgramUtil::makeAddNode(
								SpawnProgramUtil::makeFloatValueNode(SPEED),
								SpawnProgramUtil::makeUniformRandomFloatNode(-0.3f, 0.3f)
							),
							SpawnProgramUtil::makeFloatValueNode(0.0f)
						)
					)
				),

				SpawnProgramUtil::makeIfNode(
					SpawnProgramUtil::makeTickModNode(20, 40),
					SpawnProgramUtil::makeSpawnPosVelNode(
						s3e4Prototype,
						SpawnProgramUtil::makePointFromFloatsNode(
							SpawnProgramUtil::makeFloatValueNode(config::rightOut),
							SpawnProgramUtil::makeUniformRandomFloatNode(30.0f, 70.0f)
						),
						SpawnProgramUtil::makeVelocityFromPolarNode(
							SpawnProgramUtil::makeAddNode(
								SpawnProgramUtil::makeFloatValueNode(SPEED),
								SpawnProgramUtil::makeUniformRandomFloatNode(-0.3f, 0.3f)
							),
							SpawnProgramUtil::makeFloatValueNode(180.0f)
						)
					)
				)
				#undef SPEED
			)
		}

		, s3e15{ s3e15Node, 600, false }

			//wave 16
		, s3d16Node{
			#define SPEED 1.25f
			#define AV 1.2423f
			#define OFFSET 30.0f
			#define ANGLE_INC 25.0f
			#define PROTOTYPE sharpVioletPrototype
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, {27, 22, 17, 13}),
				SpawnProgramUtil::makeListNode(
					SpawnProgramUtil::makeArcFormationNode(
						SpawnProgramUtil::makeVelocityFromPolarNode(
							SpawnProgramUtil::makeFloatValueNode(SPEED),
							SpawnProgramUtil::makeSpiralNode(
								SpawnProgramUtil::makeIntValueNode(2400),
								SpawnProgramUtil::makeFloatValueNode(0.0f),
								SpawnProgramUtil::makeFloatValueNode(AV)
							)
						),
						3,		//symmetry
						ANGLE_INC,
						SpawnProgramUtil::makeSpawnPosVelNode(
							PROTOTYPE,
							SpawnProgramUtil::makeEntityOffsetNode(
								SpawnProgramUtil::makeVelocityFromPolarNode(
									SpawnProgramUtil::makeFloatValueNode(OFFSET),
									SpawnProgramUtil::makeSpiralNode(
										SpawnProgramUtil::makeIntValueNode(2400),
										SpawnProgramUtil::makeFloatValueNode(-90.0f),
										SpawnProgramUtil::makeFloatValueNode(AV)
									)
								)
							)
						)
					),
					SpawnProgramUtil::makeArcFormationNode(
						SpawnProgramUtil::makeVelocityFromPolarNode(
							SpawnProgramUtil::makeFloatValueNode(SPEED),
							SpawnProgramUtil::makeSpiralNode(
								SpawnProgramUtil::makeIntValueNode(2400),
								SpawnProgramUtil::makeFloatValueNode(90.0f),
								SpawnProgramUtil::makeFloatValueNode(AV)
							)
						),
						3,		//symmetry
						ANGLE_INC,
						SpawnProgramUtil::makeSpawnPosVelNode(
							PROTOTYPE,
							SpawnProgramUtil::makeEntityOffsetNode(
								SpawnProgramUtil::makeVelocityFromPolarNode(
									SpawnProgramUtil::makeFloatValueNode(OFFSET),
									SpawnProgramUtil::makeSpiralNode(
										SpawnProgramUtil::makeIntValueNode(2400),
										SpawnProgramUtil::makeFloatValueNode(0.0f),
										SpawnProgramUtil::makeFloatValueNode(AV)
									)
								)
							)
						)
					),
					SpawnProgramUtil::makeArcFormationNode(
						SpawnProgramUtil::makeVelocityFromPolarNode(
							SpawnProgramUtil::makeFloatValueNode(SPEED),
							SpawnProgramUtil::makeSpiralNode(
								SpawnProgramUtil::makeIntValueNode(2400),
								SpawnProgramUtil::makeFloatValueNode(180.0f),
								SpawnProgramUtil::makeFloatValueNode(AV)
							)
						),
						3,		//symmetry
						ANGLE_INC,
						SpawnProgramUtil::makeSpawnPosVelNode(
							PROTOTYPE,
							SpawnProgramUtil::makeEntityOffsetNode(
								SpawnProgramUtil::makeVelocityFromPolarNode(
									SpawnProgramUtil::makeFloatValueNode(OFFSET),
									SpawnProgramUtil::makeSpiralNode(
										SpawnProgramUtil::makeIntValueNode(2400),
										SpawnProgramUtil::makeFloatValueNode(90.0f),
										SpawnProgramUtil::makeFloatValueNode(AV)
									)
								)
							)
						)
					),
					SpawnProgramUtil::makeArcFormationNode(
						SpawnProgramUtil::makeVelocityFromPolarNode(
							SpawnProgramUtil::makeFloatValueNode(SPEED),
							SpawnProgramUtil::makeSpiralNode(
								SpawnProgramUtil::makeIntValueNode(2400),
								SpawnProgramUtil::makeFloatValueNode(270.0f),
								SpawnProgramUtil::makeFloatValueNode(AV)
							)
						),
						3,		//symmetry
						ANGLE_INC,
						SpawnProgramUtil::makeSpawnPosVelNode(
							PROTOTYPE,
							SpawnProgramUtil::makeEntityOffsetNode(
								SpawnProgramUtil::makeVelocityFromPolarNode(
									SpawnProgramUtil::makeFloatValueNode(OFFSET),
									SpawnProgramUtil::makeSpiralNode(
										SpawnProgramUtil::makeIntValueNode(2400),
										SpawnProgramUtil::makeFloatValueNode(180.0f),
										SpawnProgramUtil::makeFloatValueNode(AV)
									)
								)
							)
						)
					)
				)
			)
			#undef SPEED
			#undef AV
			#undef OFFSET
			#undef ANGLE_INC
			#undef PROTOTYPE
		}

		, s3d16{ s3d16Node, 540, false }
		, s3e16Prototype{
			EntityBuilder::makeLinearCollidable(
				{ config::rightOut, 60.0f },
				{ 1.5f, Angle{ -25.0f }.flipY() },
				WISP_ARGS(2520, pickupProgramsPointer->bombSpawnProgram),
				ScriptProgramList{
					{
						ScriptProgramUtil::makeTimerNode(40,
						ScriptProgramUtil::makeSetSpawnNode(s3d16
						))
					},
					{
						ScriptProgramUtil::makeShiftVelocityTurnPeriodNode(
							Velocity{ 0.0f, 180.0f },
							Angle{ -25.0f }.flipY(),
							150,
						ScriptProgramUtil::makeTimerNode(280,
						ScriptProgramUtil::makeShiftVelocityTurnPeriodNode(
							Velocity{ 1.5f, Angle{ 25.0f }.flipY() },
							180.0f,
							150
						)
						)
					)
					}
				}
			).heapClone()
		}
		, s3e16Node{
			SpawnProgramUtil::makeSpawnNode(s3e16Prototype)
		}
		, s3e16{ s3e16Node, 1, false }

		


		// BOSS 3 // BOSS 3 // BOSS 3 // BOSS 3 // BOSS 3 // BOSS 3 // BOSS 3 // BOSS 3 

			//1 spiral with arc walls and medium
		, b3d1Node{
			SpawnProgramUtil::makeListNode(
				SpawnProgramUtil::makeIfNode(
					SpawnProgramUtil::makeTickModNode(0, 20),
					SpawnProgramUtil::makeRingFormationNode(
						SpawnProgramUtil::makeVelocityFromPolarNode(
							SpawnProgramUtil::makeFloatValueNode(0.85f),
							SpawnProgramUtil::makeSpiralNode(
								SpawnProgramUtil::makeIntValueNode(8000),
								SpawnProgramUtil::entityRandomAngleNode,
								SpawnProgramUtil::makeFloatValueNode(0.49827f)
							)
						),
						SpawnProgramUtil::makeIntValueDiffNode({ 6, 7, 8, 9 }),
						SpawnProgramUtil::makeArcFormationNode(
							2,
							12.0f,
							SpawnProgramUtil::makeSpawnPosVelNode(
								mediumRosePrototype,
								SpawnProgramUtil::makeEntityPositionNode()
							)
						)
					)
				),
				SpawnProgramUtil::makeIfNode(
					SpawnProgramUtil::makeTickModNode(0, 45),
					SpawnProgramUtil::makeRingFormationNode(
						SpawnProgramUtil::makeVelocityFromPolarNode(
							SpawnProgramUtil::makeFloatValueNode(1.2f),
							SpawnProgramUtil::makeSpiralNode(
								SpawnProgramUtil::makeIntValueNode(8000),
								SpawnProgramUtil::entityRandomAngleNode,
								SpawnProgramUtil::makeFloatValueNode(-0.19827f)
							)
						),
						SpawnProgramUtil::makeIntValueDiffNode({ 6, 7, 8, 9 }),
						SpawnProgramUtil::makeArcFormationNode(
							SpawnProgramUtil::makeIntValueDiffNode({ 4, 6, 8, 12 }),
							SpawnProgramUtil::makeFloatValueNode(2.0f),
							SpawnProgramUtil::makeSpawnPosVelNode(
								smallRosePrototype,
								SpawnProgramUtil::makeEntityPositionNode()
							)
						)
					)
				)
			)
		}

		, b3d1{ b3d1Node, 8000, true }

			//2 column spirals
		, b3d2aNode{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, { 8, 6, 5, 4 }),
				SpawnProgramUtil::makeRingFormationNode(
					SpawnProgramUtil::makeVelocityFromPolarNode(
						SpawnProgramUtil::makeFloatValueNode(0.5f),
						SpawnProgramUtil::makeSpiralNode(48, 0.0, -2.5f)
					),
					SpawnProgramUtil::makeIntValueDiffNode({ 4, 5, 6, 7 }),
					SpawnProgramUtil::makeColumnFormationNode(
						3,
						0.22f,
						SpawnProgramUtil::makeSpawnPosVelNode(
							smallOrangePrototype,
							SpawnProgramUtil::makeEntityPositionNode()
						)
					)
				)
			)
		}
		, b3d2a{ b3d2aNode, 48, false }

		, b3d2bNode{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, { 8, 6, 5, 4 }),
				SpawnProgramUtil::makeRingFormationNode(
					SpawnProgramUtil::makeVelocityFromPolarNode(
						SpawnProgramUtil::makeFloatValueNode(0.55f),
						SpawnProgramUtil::makeSpiralNode(48, 0.0, 0.7f)
					),
					SpawnProgramUtil::makeIntValueDiffNode({ 10, 11, 11, 12 }),
					SpawnProgramUtil::makeColumnFormationNode(
						2,
						0.23f,
						SpawnProgramUtil::makeSpawnPosVelNode(
							smallCyanPrototype,
							SpawnProgramUtil::makeEntityPositionNode()
						)
					)
				)
			)
		}
		, b3d2b{ b3d2bNode, 48, false }

			//3 large arcs that shoot
		, b3d3aNode{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(20, 40),
				SpawnProgramUtil::makeListNode(
					SpawnProgramUtil::makeSpawnPosVelNode(
						smallBluePrototype,
						SpawnProgramUtil::makeEntityPositionNode(),
						SpawnProgramUtil::makeVelocityFromPolarNode(
							SpawnProgramUtil::makeFloatValueNode(0.6f),
							SpawnProgramUtil::makeAddNode(
								SpawnProgramUtil::makeEntityAngleNode(),
								SpawnProgramUtil::makeFloatValueNode(120.0f)
							)
						)		
					),
					SpawnProgramUtil::makeSpawnPosVelNode(
						smallBluePrototype,
						SpawnProgramUtil::makeEntityPositionNode(),
						SpawnProgramUtil::makeVelocityFromPolarNode(
							SpawnProgramUtil::makeFloatValueNode(0.6f),
							SpawnProgramUtil::makeAddNode(
								SpawnProgramUtil::makeEntityAngleNode(),
								SpawnProgramUtil::makeFloatValueNode(-120.0f)
							)
						)
					)
				)
			)
		}
		, b3d3a{ b3d3aNode, 170, false }

		, b3d3bPrototype{
			EntityBuilder::makePosVelPrototype(
				//for this specific danmaku, large is drawn over small
				BASIC_BULLET_ARGS(largeHitbox, outbound, L"large_rose", -200),
				SpawnProgramList{ b3d3a }
			).heapClone()
		}

		, b3d3bNode{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, {30, 25, 20, 15 }),
				SpawnProgramUtil::makeRingFormationNode(
					SpawnProgramUtil::makeVelocityFromPolarNode(
						SpawnProgramUtil::makeFloatValueNode(0.8f),
						SpawnProgramUtil::makeSpiralNode(125, 0.0f, 1.0f)
					),
					SpawnProgramUtil::makeIntValueDiffNode({ 3, 3, 4, 5 }),
					SpawnProgramUtil::makeSpawnPosVelNode(
						b3d3bPrototype,
						SpawnProgramUtil::makeEntityPositionNode()
					)
				)
			)
		}
		, b3d3b{ b3d3bNode, 125, false }

			//4
		, b3d4aNode{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, {5, 4, 3, 2}),
				SpawnProgramUtil::makeSpawnPosVelNode(
					smallVioletPrototype,
					SpawnProgramUtil::makeEntityPositionNode(),
					SpawnProgramUtil::makeUniformRandomCircleVelocityNode(0.25f, 0.5f)
				)
			)
		}
		, b3d4a{ b3d4aNode, 10000, true }

		, b3d4bPrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(mediumHitbox, outbound, L"medium_violet", 0),
				SpawnProgramList{ b3d4a }
			).heapClone()
		}

		, b3d4bNode{
			SpawnProgramUtil::makeRingFormationNode(
				SpawnProgramUtil::makeVelocityFromPolarNode(
					SpawnProgramUtil::makeFloatValueNode(1.2f),
					SpawnProgramUtil::makeAddNode(
						SpawnProgramUtil::makeAngleToPlayerNode(),
						SpawnProgramUtil::makeFloatValueNode(180.0f)
					)
				),
				SpawnProgramUtil::makeIntValueNode(5),
				SpawnProgramUtil::makeSpawnPosVelNode(
					b3d4bPrototype,
					SpawnProgramUtil::makeEntityPositionNode()
				)
			)
		}

		, b3d4b{ b3d4bNode, 1, false }

			//5 spiral of splitters
		#define ANGLE_OFF -50.0f
		, b3d5aNode{
			SpawnProgramUtil::makeSpawnPosVelNode(
				smallYellowPrototype,
				SpawnProgramUtil::makeEntityPositionNode(),
				SpawnProgramUtil::makeVelocityFromPolarNode(
					SpawnProgramUtil::makeFloatValueNode(0.7f),
					SpawnProgramUtil::makeAddNode(
						SpawnProgramUtil::makeEntityAngleNode(),
						SpawnProgramUtil::makeFloatValueNode(ANGLE_OFF)
					)
				)
			)
		}

		, b3d5a{ b3d5aNode, 1, false }

		, b3d5bPrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(smallHitbox, outbound, L"small_orange", 100),
				ScriptProgramList{
					ScriptProgramUtil::makeTimerNode(
						55,
					ScriptProgramUtil::makeSetSpawnNode(b3d5a
					))
				}
			).heapClone()
		}

		, b3d5bNode{
			SpawnProgramUtil::makeSpawnPosVelNode(
				b3d5bPrototype,
				SpawnProgramUtil::makeEntityPositionNode(),
				SpawnProgramUtil::makeVelocityFromPolarNode(
					SpawnProgramUtil::makeFloatValueNode(0.65f),
					SpawnProgramUtil::makeAddNode(
						SpawnProgramUtil::makeEntityAngleNode(),
						SpawnProgramUtil::makeFloatValueNode(ANGLE_OFF)
					)
				)
			)
		}
		#undef ANGLE_OFF

		, b3d5b{ b3d5bNode, 1, false }

		, b3d5cPrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(smallHitbox, outbound, L"small_rose", 100),
				ScriptProgramList{
					ScriptProgramUtil::makeTimerNode(
						55,
					ScriptProgramUtil::makeSetSpawnNode(b3d5b
					))
				}
			).heapClone()
		}

		, b3d5cNode{
			SpawnProgramUtil::makeListNode(
				SpawnProgramUtil::makeIfNode(
					SpawnProgramUtil::makeTickModNode(0, { 6, 5, 4, 3}),
					SpawnProgramUtil::makeRingFormationNode(
						SpawnProgramUtil::makeVelocityFromPolarNode(
							SpawnProgramUtil::makeFloatValueNode(0.6f),
							SpawnProgramUtil::makeSpiralNode(
								SpawnProgramUtil::makeIntValueNode(8000),
								SpawnProgramUtil::entityRandomAngleNode,
								SpawnProgramUtil::makeFloatValueDiffNode( {
									4.1f, 
									4.1f, 
									4.25f, 
									4.1f
								} )
							)
						),
						2,
						SpawnProgramUtil::makeSpawnPosVelNode(
							b3d5cPrototype,
							SpawnProgramUtil::makeEntityPositionNode()
						)
					)
				),
				SpawnProgramUtil::makeIfNode(
					SpawnProgramUtil::makeTickModNode(35, 80),
					SpawnProgramUtil::makeSpawnPosVelNode(
						mediumRosePrototype,
						SpawnProgramUtil::makeEntityPositionNode(),
						SpawnProgramUtil::makeVelocityFromPolarNode(
							SpawnProgramUtil::makeFloatValueNode(0.85f),
							SpawnProgramUtil::makeAngleToPlayerNode()
						)
					)
				)
			)
		}

		, b3d5c{ b3d5cNode, 8000, true }

			//6 "random" dense arcs
		, b3d6Node{
			SpawnProgramUtil::makeListNode(
				SpawnProgramUtil::makeIfNode(
					SpawnProgramUtil::makeTickModNode(0, { 6, 5, 4, 3 }),
					SpawnProgramUtil::makeArcFormationNode(
						SpawnProgramUtil::makeVelocityFromPolarNode(
							SpawnProgramUtil::makeUniformRandomFloatNode(0.5f, 0.9f),
							SpawnProgramUtil::makeSpiralNode(
								SpawnProgramUtil::makeIntValueNode(4000),
								SpawnProgramUtil::entityRandomAngleNode,
								SpawnProgramUtil::makeSpiralNode(
									SpawnProgramUtil::makeIntValueNode(4000),
									SpawnProgramUtil::entityRandomAngleNode,
									SpawnProgramUtil::makeFloatValueNode(1.0f)
								)
							)
						),
						SpawnProgramUtil::makeIntValueDiffNode({ 4, 4, 5, 5 }),
						SpawnProgramUtil::makeFloatValueNode(2.5f),
						SpawnProgramUtil::makeSpawnPosVelNode(
							smallRedPrototype,
							SpawnProgramUtil::makeEntityOffsetNode(
								SpawnProgramUtil::makeVelocityFromPolarNode(
									SpawnProgramUtil::makeFloatValueNode(20.0f),
									SpawnProgramUtil::makeSpiralNode(4000, 28.9f, 2.0f)
								)
							)
						)
					)
				),
				SpawnProgramUtil::makeIfNode(
					SpawnProgramUtil::makeTickModNode(1, { 12, 10, 8, 6 }),
					SpawnProgramUtil::makeSpawnPosVelNode(
						mediumBluePrototype,
						SpawnProgramUtil::makeEntityOffsetNode(
							SpawnProgramUtil::makeVelocityFromPolarNode(
								SpawnProgramUtil::makeFloatValueNode(20.0f),
								SpawnProgramUtil::makeSpiralNode(
									4000, 
									208.9f,
									2.0f
								)
							)
						),
						SpawnProgramUtil::makeVelocityFromPolarNode(
							SpawnProgramUtil::makeUniformRandomFloatNode(0.4f, 0.8f),
							SpawnProgramUtil::makeSpiralNode(
								SpawnProgramUtil::makeIntValueNode(4000),
								SpawnProgramUtil::entityRandomAngleNode,
								SpawnProgramUtil::makeSpiralNode(
									SpawnProgramUtil::makeIntValueNode(4000),
									SpawnProgramUtil::entityRandomAngleNode,
									SpawnProgramUtil::makeFloatValueNode(1.0f)
								)
							)
						)
					)
				)
			)
		}
		, b3d6{ b3d6Node, 4000, true}

			//the boss
		, b3e1Prototype{
			EntityBuilder::makeLinearUncollidable(
				BOSS_ARGS,
				SpriteInstruction{
					bitmapStoragePointer->get(L"b3_idle_1")->d2dBitmap,
					math::Vector2{ 0.0f, 4.0f }			//offset
				},
				DrawOrder{ config::playerDrawOrder },
				AnimationList{ 
					{
						components::Animation{ {
							L"b3_idle_1", L"b3_idle_2", L"b3_idle_3", L"b3_idle_4"
						} }
					},
					0,	//idle index
					4	//ticks
				},
				ScriptProgramList{
					ScriptProgramUtil::makeBossEntryNode(120, L"3_1",
					ScriptProgramUtil::makeTimerNode(120,
					ScriptProgramUtil::makeSetHealthNode(5000,
					ScriptProgramUtil::makeSetSpawnNode(b3d1,
					ScriptProgramUtil::makeStallingIfNode(
					ScriptProgramUtil::makeIsBossDeadNode(),
					ScriptProgramUtil::makeBossResetNode(90,
					ScriptProgramUtil::makeSetHealthNode(6000,
					ScriptProgramUtil::makeDoubleBossAttackAndMoveNode(
						b3d2a, 
						62, 
						20, 
						b3d2b, 
						62, 
						20,
					ScriptProgramUtil::makeAddSpawnNode(
						pickupProgramsPointer->largePowerSpawnProgram,
					ScriptProgramUtil::makeTimerNode(1,
					ScriptProgramUtil::makeBossResetMidNode(90,
					ScriptProgramUtil::makeSetHealthNode(6200,
					ScriptProgramUtil::makeBossAttackAndMoveNode(
						b3d3b, 
						140, 
						20,
					ScriptProgramUtil::makeBossResetMidNode(90,
					ScriptProgramUtil::makeSetHealthNode(6200,
					ScriptProgramUtil::makeBossAttackAndMoveNode(
						b3d4b,
						90,
						90,
					ScriptProgramUtil::makeAddSpawnNode(
						pickupProgramsPointer->largePowerSpawnProgram,
					ScriptProgramUtil::makeTimerNode(1,
					ScriptProgramUtil::makeBossResetMidNode(90,
					ScriptProgramUtil::makeSetHealthNode(7000,
					ScriptProgramUtil::makeAddSpawnNode(b3d5c,	//<---- 5 is here
					ScriptProgramUtil::makeStallingIfNode(
						ScriptProgramUtil::makeIsBossDeadNode(),
						ScriptProgramUtil::makeBossResetNode(90,
						ScriptProgramUtil::makeSetHealthNode(6500,
						ScriptProgramUtil::makeAddSpawnNode(b3d6,	//<------6 is here
						ScriptProgramUtil::makeBossMoveNode(120, 60,
						ScriptProgramUtil::makeBossEndNode()
					
					))))))))))))))))))))))))))
				}
			).heapClone()
		}
		, b3e1Node{
			SpawnProgramUtil::makeSpawnNode(b3e1Prototype)
		}
		, b3e1{ b3e1Node, 1, false }

		

		, stage3ScriptProgram{
			ScriptProgramUtil::makeTimerNode(690,
			ScriptProgramUtil::makeSetSpawnNode(s3e1a,
			ScriptProgramUtil::makeTimerNode(190,
			ScriptProgramUtil::makeAddSpawnNode(s3e1b,
			ScriptProgramUtil::makeTimerNode(80,
			ScriptProgramUtil::makeAddSpawnNode(s3e1c,
			ScriptProgramUtil::makeTimerNode(130,
			ScriptProgramUtil::makeAddSpawnNode(s3e2,
			ScriptProgramUtil::makeTimerNode(360,
			ScriptProgramUtil::makeAddSpawnNode(s3e3,
			ScriptProgramUtil::makeTimerNode(120,
			ScriptProgramUtil::makeAddSpawnNode(s3e4,
			ScriptProgramUtil::makeTimerNode(200,
			ScriptProgramUtil::makeAddSpawnNode(s3e5,
			ScriptProgramUtil::makeTimerNode(130,
			ScriptProgramUtil::makeAddSpawnNode(s3e1a,	//wave 6, same as wave 1
			ScriptProgramUtil::makeTimerNode(180,
			ScriptProgramUtil::makeAddSpawnNode(s3e1b,
			ScriptProgramUtil::makeTimerNode(80,
			ScriptProgramUtil::makeAddSpawnNode(s3e1c,
			ScriptProgramUtil::makeTimerNode(120,
			ScriptProgramUtil::makeAddSpawnNode(s3e7,
			ScriptProgramUtil::makeTimerNode(190,
			ScriptProgramUtil::makeAddSpawnNode(s3e8a,
			ScriptProgramUtil::makeTimerNode(45,
			ScriptProgramUtil::makeAddSpawnNode(s3e8b,
			ScriptProgramUtil::makeTimerNode(45,
			ScriptProgramUtil::makeAddSpawnNode(s3e8c,
			ScriptProgramUtil::makeTimerNode(140,
			ScriptProgramUtil::makeAddSpawnNode(s3e9,
			ScriptProgramUtil::makeTimerNode(950,
			ScriptProgramUtil::makeAddSpawnNode(s3e10,
			ScriptProgramUtil::makeTimerNode(360,
			ScriptProgramUtil::makeAddSpawnNode(s3e11,
			ScriptProgramUtil::makeTimerNode(360,
			ScriptProgramUtil::makeAddSpawnNode(s3e12,
			ScriptProgramUtil::makeTimerNode(300,
			ScriptProgramUtil::makeAddSpawnNode(s3e13,
			ScriptProgramUtil::makeTimerNode(330,
			ScriptProgramUtil::makeAddSpawnNode(s3e14,
			ScriptProgramUtil::makeTimerNode(360,
			ScriptProgramUtil::makeAddSpawnNode(s3e15,
			ScriptProgramUtil::makeTimerNode(700,
			ScriptProgramUtil::makeAddSpawnNode(s3e16,
			ScriptProgramUtil::makeTimerNode(600,
			ScriptProgramUtil::makeAddSpawnNode(b3e1,
			ScriptProgramUtil::makeStallingIfNode(
				ScriptProgramUtil::makeIsWinNode(),
				ScriptProgramUtil::makeTimerNode(waitEndTime,
				ScriptProgramUtil::makeShowDialogueNode(L"3_2",
				ScriptProgramUtil::makeStallingIfNode(
					ScriptProgramUtil::makeIsDialogueOverNode(),
					ScriptProgramUtil::makeTimerNode(1,
					ScriptProgramUtil::makeEndStageNode()
				)))
			)
			)))))))))))))))))))))))))))))))))))))))))))))))
		}


		// STAGE 4 // STAGE 4 // STAGE 4 // STAGE 4 // STAGE 4 // STAGE 4 // STAGE 4 //

			//wave 1
		, s4d1Node{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(
					SpawnProgramUtil::makeEntityUniformRandomIntNode(
						SpawnProgramUtil::makeIntValueNode(0),
						SpawnProgramUtil::makeIntValueDiffNode({ 95, 80, 65, 50 })
					),
					{ 95, 80, 65, 50 }
				),
				SpawnProgramUtil::makeRingFormationNode(
					SpawnProgramUtil::makeUniformRandomCircleVelocityNode(0.7f, 1.3f),
					SpawnProgramUtil::makeIntValueDiffNode({ 6, 7, 8, 9 }),
					SpawnProgramUtil::makeSpawnPosVelNode(
						smallYellowPrototype,
						SpawnProgramUtil::makeEntityPositionNode()
					)
				)
			)
		}
		, s4d1{ s4d1Node, 10000, false }

		, s4e1Prototype{
			EntityBuilder::makePosVelPrototype(
				CLOUD_ARGS(150, pickupProgramsPointer->smallPowerHalfSpawnProgram),
				SpawnProgramList{ s4d1 }
			).heapClone()
		}

		, s4e1Node{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, 11),
				SpawnProgramUtil::makeIfElseNode(
					SpawnProgramUtil::makeTickModNode(0, 2),
					SpawnProgramUtil::makeSpawnPosVelNode(
						s4e1Prototype,
						SpawnProgramUtil::makePointFromFloatsNode(
							SpawnProgramUtil::makeFloatValueNode(config::leftOut),
							SpawnProgramUtil::makeUniformRandomFloatNode(
								10.0f + config::gameOffset.y,
								70.0f + config::gameOffset.y
							)
						),
						SpawnProgramUtil::makeVelocityValueNode(Vector2{ 1.1f, 0.0f })
					),
					SpawnProgramUtil::makeSpawnPosVelNode(
						s4e1Prototype,
						SpawnProgramUtil::makePointFromFloatsNode(
							SpawnProgramUtil::makeFloatValueNode(config::rightOut),
							SpawnProgramUtil::makeUniformRandomFloatNode(
								10.0f + config::gameOffset.y,
								70.0f + config::gameOffset.y
							)
						),
						SpawnProgramUtil::makeVelocityValueNode(Vector2{ -1.1f, 0.0f })
					)
				)
			)
		}

		, s4e1{ s4e1Node, 530, false }

			//wave 2

		, s4d2Prototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(mediumHitbox, outbound, L"medium_red", 0),
				ScriptProgramList{
					ScriptProgramUtil::makeTimerNode(
						55,
					ScriptProgramUtil::makeShiftSpeedPeriodNode(2.1f, 100
					))
				}
			).heapClone()
		}

		, s4d2Node{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, { 40, 30, 24, 18 }),
				SpawnProgramUtil::makeRingFormationNode(
					SpawnProgramUtil::makeVelocityFromPolarNode(
						SpawnProgramUtil::makeFloatValueNode(0.4f),
						SpawnProgramUtil::makeAngleToPlayerNode()
					),
					SpawnProgramUtil::makeIntValueDiffNode({ 15, 20, 25, 30}),
					SpawnProgramUtil::makeSpawnPosVelNode(
						s4d2Prototype,
						SpawnProgramUtil::makeEntityPositionNode()
					)
				)
			)
		}

		, s4d2{ s4d2Node, 320, false }

		, s4e2Prototype{
			EntityBuilder::makePosVelPrototype(
				WISP_ARGS(2800, pickupProgramsPointer->largePowerSpawnProgram),
				ScriptProgramList{
					ScriptProgramUtil::makeShootOnceAndLeaveProgram(
						20,
						40,
						5,
						s4d2,
						80,
						{ 1.0f, 90.0f},
						100
					)
				}
			).heapClone()
		}

		, s4e2aNode{
			SpawnProgramUtil::makeSpawnPosVelNode(
				s4e2Prototype,
				SpawnProgramUtil::makePointValueNode({
					28.0f + config::gameOffset.x,
					config::topOut
				}),
				SpawnProgramUtil::makeVelocityValueNode(Vector2{ 0.0f, 1.5f })
			)
		}
		, s4e2a{ s4e2aNode, 1, false }

		, s4e2bNode{
			SpawnProgramUtil::makeSpawnPosVelNode(
				s4e2Prototype,
				SpawnProgramUtil::makePointValueNode({
					config::gameWidth - 28.0f + config::gameOffset.x,
					config::topOut
				}),
				SpawnProgramUtil::makeVelocityValueNode(Vector2{ 0.0f, 1.5f })
			)
		}
		, s4e2b{ s4e2bNode, 1, false }

			//wave 3

		, s4d3Node{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(
					SpawnProgramUtil::makeEntityUniformRandomIntNode(
						SpawnProgramUtil::makeIntValueNode(0),
						SpawnProgramUtil::makeIntValueDiffNode({ 40, 30, 20, 15 })
					),
					{ 50, 40, 30, 20 }
				),
				SpawnProgramUtil::makeSpawnPosVelNode(
					smallVioletPrototype,
					SpawnProgramUtil::makeEntityPositionNode(),
					SpawnProgramUtil::makeVelocityFromPolarNode(
						SpawnProgramUtil::makeFloatValueNode(0.8f),
						SpawnProgramUtil::makeAddNode(
							SpawnProgramUtil::makeAngleToPlayerNode(),
							SpawnProgramUtil::makeUniformRandomFloatNode(-20.0f, 20.0f)
						)
					)
				)
			)
		}

		, s4d3{ s4d3Node, 10000, true }

		, s4e3Prototype{
			EntityBuilder::makePosVelPrototype(
				FLAME_ARGS(430, pickupProgramsPointer->smallPowerHalfSpawnProgram),
				ScriptProgramList{
					ScriptProgramUtil::makeTimerNode(30,
					ScriptProgramUtil::makeShiftSpeedPeriodNode(0.0f, 50,
					ScriptProgramUtil::makeSetSpawnNode(s4d3,
					ScriptProgramUtil::makeShiftSpeedPeriodNode(1.0f, 100,
					ScriptProgramUtil::makeStallingIfNode(	//for later waves
						ScriptProgramUtil::makeBoundaryYHighNode(
							config::gameWidth + config::gameOffset.y
						),
						ScriptProgramUtil::makeClearSpawnNode()
					)
					))))
				}
			).heapClone()
		}

		, s4e3Node{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, 18),
				SpawnProgramUtil::makeSpawnPosVelNode(
					s4e3Prototype,
					SpawnProgramUtil::makePointFromFloatsNode(
						SpawnProgramUtil::makeFloatValueNode(config::leftOut),
						SpawnProgramUtil::makeUniformRandomFloatNode(
							50.0f + config::gameOffset.y,
							100.0f + config::gameOffset.y
						)
					),
					SpawnProgramUtil::makeVelocityFromPolarNode(
						SpawnProgramUtil::makeUniformRandomFloatNode(0.8f, 1.0f),
						SpawnProgramUtil::makeUniformRandomFloatNode(0.0f, 20.0f)
					)
				)
			)
		}

		, s4e3{ s4e3Node, 300, false }

			//wave 4
		, s4e4Node{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, 18),
				SpawnProgramUtil::makeSpawnPosVelNode(
					s4e3Prototype,
					SpawnProgramUtil::makePointFromFloatsNode(
						SpawnProgramUtil::makeFloatValueNode(config::rightOut),
						SpawnProgramUtil::makeUniformRandomFloatNode(
							50.0f + config::gameOffset.y,
							100.0f + config::gameOffset.y
						)
					),
					SpawnProgramUtil::makeVelocityFromPolarNode(
						SpawnProgramUtil::makeUniformRandomFloatNode(0.8f, 1.0f),
						SpawnProgramUtil::makeUniformRandomFloatNode(160.0f, 180.0f)
					)
				)
			)
		}

		, s4e4{ s4e4Node, 240, false }

			//wave 5
		, s4d5Node{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, { 27, 22, 18, 15 }),
				SpawnProgramUtil::makeListNode(
					SpawnProgramUtil::makeRingFormationNode(
						SpawnProgramUtil::makeVelocityFromPolarNode(
							SpawnProgramUtil::makeFloatValueNode(1.14f),
							SpawnProgramUtil::makeSpiralNode(
								SpawnProgramUtil::makeIntValueNode(0),
								SpawnProgramUtil::entityRandomAngleNode,
								SpawnProgramUtil::makeFloatValueNode(0.3f)
							)
						),
						SpawnProgramUtil::makeIntValueDiffNode({ 10, 14, 18, 22 }),
						SpawnProgramUtil::makeSpawnPosVelNode(
							smallAzurePrototype,
							SpawnProgramUtil::makeEntityPositionNode()
						)
					),
					SpawnProgramUtil::makeRingFormationNode(
						SpawnProgramUtil::makeVelocityFromPolarNode(
							SpawnProgramUtil::makeFloatValueNode(1.64f),
							SpawnProgramUtil::makeSpiralNode(
								SpawnProgramUtil::makeIntValueNode(0),
								SpawnProgramUtil::makeFloatValueNode(0.0f),
								SpawnProgramUtil::makeFloatValueNode(-0.5f)
							)
						),
						SpawnProgramUtil::makeIntValueDiffNode({ 5, 6, 7, 8 }),
						SpawnProgramUtil::makeSpawnPosVelNode(
							mediumBluePrototype,
							SpawnProgramUtil::makeEntityPositionNode()
						)
					)
				)
			)
		}

		, s4d5{ s4d5Node, 370, false }

		, s4e5Prototype{
			EntityBuilder::makeLinearCollidable(
				{ config::gameWidth / 2.0f + config::gameOffset.x, config::topOut },
				Velocity{ 1.4f, -90.0f },
				CRYSTAL_BLUE_ARGS(2100, pickupProgramsPointer->lifeSpawnProgram),
				ScriptProgramList{
					ScriptProgramUtil::makeShootOnceAndLeaveProgram(
						20,
						35,
						20,
						s4d5,
						20,
						Velocity{ 1.0f, 90.0f },
						30
					)
				}
			).heapClone()
		}

		, s4e5Node{ SpawnProgramUtil::makeSpawnNode(s4e5Prototype) }
		, s4e5{ s4e5Node, 1, false }

			//wave 6

		, s4d6Node{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, 45),
				SpawnProgramUtil::makeListNode(
					SpawnProgramUtil::makeRingFormationNode(
						SpawnProgramUtil::makeVelocityFromPolarNode(
							SpawnProgramUtil::makeFloatValueNode(1.4f),
							SpawnProgramUtil::makeAngleToPlayerNode()
						),
						SpawnProgramUtil::makeIntValueDiffNode({ 12, 14, 16, 18 }),
						SpawnProgramUtil::makeSpawnPosVelNode(
							mediumGreenPrototype,
							SpawnProgramUtil::makeEntityPositionNode()
						)
					),
					SpawnProgramUtil::makeRingFormationNode(
						SpawnProgramUtil::makeVelocityFromPolarNode(
							SpawnProgramUtil::makeFloatValueNode(1.2f),
							SpawnProgramUtil::makeAngleToPlayerNode()
						),
						SpawnProgramUtil::makeIntValueDiffNode({ 16, 20, 24, 28 }),
						SpawnProgramUtil::makeSpawnPosVelNode(
							smallGreenPrototype,
							SpawnProgramUtil::makeEntityPositionNode()
						)
					),
					SpawnProgramUtil::makeRingFormationNode(
						SpawnProgramUtil::makeVelocityFromPolarNode(
							SpawnProgramUtil::makeFloatValueNode(1.0f),
							SpawnProgramUtil::makeAngleToPlayerNode()
						),
						SpawnProgramUtil::makeIntValueDiffNode({ 9, 11, 13, 15 }),
						SpawnProgramUtil::makeSpawnPosVelNode(
							mediumGreenPrototype,
							SpawnProgramUtil::makeEntityPositionNode()
						)
					)
				)
			)
		}

		, s4d6{ s4d6Node, 370, false }

		, s4e6Prototype{
			EntityBuilder::makePosPrototype(
				Velocity{ 1.4f, -90.0f },
				CRYSTAL_GREEN_ARGS(800, pickupProgramsPointer->largePowerSpawnProgram),
				ScriptProgramList{
					ScriptProgramUtil::makeShootOnceAndLeaveProgram(
						20,
						35,
						20,
						s4d6,
						20,
						Velocity{ 1.0f, 90.0f },
						30
					)
				}
			).heapClone()
		}

		, s4e6Node{ 
			SpawnProgramUtil::makeListNode(
				SpawnProgramUtil::makeSpawnPosNode(
					s4e6Prototype,
					SpawnProgramUtil::makePointValueNode(
						{ 35.0f + config::gameOffset.x, config::topOut }
					)
				),
				SpawnProgramUtil::makeSpawnPosNode(
					s4e6Prototype,
					SpawnProgramUtil::makePointValueNode(
						{ config::gameWidth - 35.0f + config::gameOffset.x, 
						config::topOut }
					)
				)
			)
		}
		, s4e6{ s4e6Node, 1, false }

			//wave 7

		, s4d7Node{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, { 27, 22, 18, 15 }),
				SpawnProgramUtil::makeListNode(
					SpawnProgramUtil::makeRingFormationNode(
						SpawnProgramUtil::makeVelocityFromPolarNode(
							SpawnProgramUtil::makeFloatValueNode(1.14f),
							SpawnProgramUtil::makeSpiralNode(
								SpawnProgramUtil::makeIntValueNode(0),
								SpawnProgramUtil::entityRandomAngleNode,
								SpawnProgramUtil::makeFloatValueNode(-0.3f)
							)
						),
						SpawnProgramUtil::makeIntValueDiffNode({ 10, 14, 18, 22 }),
						SpawnProgramUtil::makeSpawnPosVelNode(
							smallOrangePrototype,
							SpawnProgramUtil::makeEntityPositionNode()
						)
					),
					SpawnProgramUtil::makeRingFormationNode(
						SpawnProgramUtil::makeVelocityFromPolarNode(
							SpawnProgramUtil::makeFloatValueNode(1.64f),
							SpawnProgramUtil::makeSpiralNode(
								SpawnProgramUtil::makeIntValueNode(0),
								SpawnProgramUtil::makeFloatValueNode(0.0f),
								SpawnProgramUtil::makeFloatValueNode(0.5f)
							)
						),
						SpawnProgramUtil::makeIntValueDiffNode({ 5, 6, 7, 8 }),
						SpawnProgramUtil::makeSpawnPosVelNode(
							mediumRedPrototype,
							SpawnProgramUtil::makeEntityPositionNode()
						)
					)
				)
			)
		}

		, s4d7{ s4d7Node, 370, false }

		, s4e7Prototype{
			EntityBuilder::makeLinearCollidable(
				{ config::gameWidth / 2.0f + config::gameOffset.x, config::topOut },
				Velocity{ 1.4f, -90.0f },
				CRYSTAL_RED_ARGS(2100, pickupProgramsPointer->bombSpawnProgram),
				ScriptProgramList{
					ScriptProgramUtil::makeShootOnceAndLeaveProgram(
						20,
						35,
						20,
						s4d7,
						20,
						Velocity{ 1.0f, 90.0f },
						30
					)
				}
			).heapClone()
		}

		, s4e7Node{ SpawnProgramUtil::makeSpawnNode(s4e7Prototype) }
		, s4e7{ s4e7Node, 1, false }

			//wave 8: repeat wave 6

			//wave 9
		, s4e9Node{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, 14),
				SpawnProgramUtil::makeSpawnPosVelNode(
					s4e3Prototype,
					SpawnProgramUtil::makePointFromFloatsNode(
						SpawnProgramUtil::makeUniformRandomFloatNode(
							config::gameWidth - 50.0f + config::gameOffset.x,
							config::gameWidth - 10.0f + config::gameOffset.x
						),
						SpawnProgramUtil::makeFloatValueNode(config::topOut)
					),
					SpawnProgramUtil::makeVelocityFromPolarNode(
						SpawnProgramUtil::makeUniformRandomFloatNode(0.8f, 1.0f),
						SpawnProgramUtil::makeUniformRandomFloatNode(-100.0f, -80.0f)
					)
				)
			)
		}

		, s4e9{ s4e9Node, 200, false }

			//wave 10

		, s4e10Node{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, 14),
				SpawnProgramUtil::makeSpawnPosVelNode(
					s4e3Prototype,
					SpawnProgramUtil::makePointFromFloatsNode(
						SpawnProgramUtil::makeUniformRandomFloatNode(
							10.0f + config::gameOffset.x,
							50.0f + config::gameOffset.x
						),
						SpawnProgramUtil::makeFloatValueNode(config::topOut)
					),
					SpawnProgramUtil::makeVelocityFromPolarNode(
						SpawnProgramUtil::makeUniformRandomFloatNode(0.8f, 1.0f),
						SpawnProgramUtil::makeUniformRandomFloatNode(-100.0f, -80.0f)
					)
				)
			)
		}

		, s4e10{ s4e10Node, 170, false }

				//wave 11

		, s4d11Prototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(smallHitbox, outbound, L"small_red", 100),
				ScriptProgramList{
					ScriptProgramUtil::makeTimerNode(
						55,
					ScriptProgramUtil::makeShiftSpeedPeriodNode(3.1f, 100
					))
				}
			).heapClone()
		}

		, s4d11Node{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, { 40, 30, 24, 18 }),
				SpawnProgramUtil::makeRingFormationNode(
					SpawnProgramUtil::makeVelocityFromPolarNode(
						SpawnProgramUtil::makeFloatValueNode(0.4f),
						SpawnProgramUtil::makeAngleToPlayerNode()
					),
					SpawnProgramUtil::makeIntValueDiffNode({ 10, 15, 20, 25}),
					SpawnProgramUtil::makeSpawnPosVelNode(
						s4d11Prototype,
						SpawnProgramUtil::makeEntityPositionNode()
					)
				)
			)
		}

		, s4d11{ s4d11Node, 620, false }

		, s4e11Prototype{
			EntityBuilder::makePosVelPrototype(
				WISP_ARGS(1000, pickupProgramsPointer->largePowerSpawnProgram),
				ScriptProgramList{
					ScriptProgramUtil::makeShootOnceAndLeaveProgram(
						20,
						40,
						5,
						s4d11,
						80,
						{ 1.0f, 90.0f},
						100
					)
				}
			).heapClone()
		}

		, s4e11Node{
			SpawnProgramUtil::makeListNode(
				SpawnProgramUtil::makeSpawnPosVelNode(
					s4e11Prototype,
					SpawnProgramUtil::makePointValueNode({
						28.0f + config::gameOffset.x,
						config::topOut
					}),
					SpawnProgramUtil::makeVelocityValueNode(Vector2{ 0.0f, 1.0f })
				),
				SpawnProgramUtil::makeSpawnPosVelNode(
					s4e11Prototype,
					SpawnProgramUtil::makePointValueNode({
						config::gameWidth - 28.0f + config::gameOffset.x,
						config::topOut
					}),
					SpawnProgramUtil::makeVelocityValueNode(Vector2{ 0.0f, 1.0f })
				),
				SpawnProgramUtil::makeSpawnPosVelNode(
					s4e11Prototype,
					SpawnProgramUtil::makePointValueNode({
						48.0f + config::gameOffset.x,
						config::topOut
					}),
					SpawnProgramUtil::makeVelocityValueNode(Vector2{ 0.0f, 1.5f })
				),
				SpawnProgramUtil::makeSpawnPosVelNode(
					s4e11Prototype,
					SpawnProgramUtil::makePointValueNode({
						config::gameWidth - 48.0f + config::gameOffset.x,
						config::topOut
					}),
					SpawnProgramUtil::makeVelocityValueNode(Vector2{ 0.0f, 1.5f })
				)
			)
			
		}
		, s4e11{ s4e11Node, 1, false }

		// BOSS 4 // BOSS 4 // BOSS 4 // BOSS 4 // BOSS 4 // BOSS 4 // BOSS 4 //

			//1
		, b4d1aNode{
			#define SPEED 0.85f
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(
					SpawnProgramUtil::makeEntityUniformRandomIntNode(
						SpawnProgramUtil::makeIntValueNode(0),
						SpawnProgramUtil::makeIntValueDiffNode({ 150, 130, 110, 90 })
					),
					{ 150, 130, 110, 90 }
				),
				SpawnProgramUtil::makeListNode(
					SpawnProgramUtil::makeSpawnPosVelNode(
						sharpVioletPrototype,
						SpawnProgramUtil::makeEntityPositionNode(),
						SpawnProgramUtil::makeVelocityFromPolarNode(
							SpawnProgramUtil::makeFloatValueNode(SPEED),
							SpawnProgramUtil::makeAddNode(
								SpawnProgramUtil::makeEntityAngleNode(),
								SpawnProgramUtil::makeUniformRandomFloatNode(
									88.0f,
									92.0f
								)
							)
						)
					),
					SpawnProgramUtil::makeSpawnPosVelNode(
						sharpRosePrototype,
						SpawnProgramUtil::makeEntityPositionNode(),
						SpawnProgramUtil::makeVelocityFromPolarNode(
							SpawnProgramUtil::makeFloatValueNode(SPEED),
							SpawnProgramUtil::makeAddNode(
								SpawnProgramUtil::makeEntityAngleNode(),
								SpawnProgramUtil::makeUniformRandomFloatNode(
									-92.0f,
									-88.0f
								)
							)
						)
					)
				)
			)
			#undef SPEED
		}
		, b4d1a{ b4d1aNode, 10000, false }

		, b4d1bPrototype{
			EntityBuilder::makePosVelPrototype(
				SHARP_BULLET_ARGS(sharpHitbox, outbound, L"sharp_orange", 200),
				SpawnProgramList{ b4d1a },
				ScriptProgramList{
					ScriptProgramUtil::makeStallingIfNode(
						ScriptProgramUtil::makeBoundaryYHighNode(
							config::gameHeight + config::gameOffset.y
						),
						ScriptProgramUtil::makeClearSpawnNode()
					)
				}
			).heapClone()
		}

		, b4d1bNode{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, 17),
				SpawnProgramUtil::makeRingFormationNode(
					SpawnProgramUtil::makeVelocityValueNode(Velocity{ 1.6f, 60.0f }),
					6,
					SpawnProgramUtil::makeSpawnPosVelNode(
						b4d1bPrototype,
						SpawnProgramUtil::makeEntityPositionNode()
					)
				)
			)
		}
		, b4d1b{ b4d1bNode, 1700, true }

			//2
		, b4d2aNode{
			SpawnProgramUtil::makeRingFormationNode(
				SpawnProgramUtil::makeVelocityFromPolarNode(
					SpawnProgramUtil::makeFloatValueDiffNode({
						0.9f,
						1.0f,
						1.1f,
						1.2f
					}),
					SpawnProgramUtil::randomAngleNode
				),
				SpawnProgramUtil::makeIntValueDiffNode({ 6, 6, 7, 7 }),
				SpawnProgramUtil::makeColumnFormationNode(
					{ 4, 5, 5, 6 },
					-0.15f,
					SpawnProgramUtil::makeSpawnPosVelNode(
						smallYellowPrototype,
						SpawnProgramUtil::makeEntityPositionNode()
					)
				)
			)
		}

		, b4d2a{ b4d2aNode, 1, false }

		, b4d2bPrototype {
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(mediumHitbox, outbound, L"medium_cyan", 0),
				ScriptProgramList{
					ScriptProgramUtil::makeStallingIfNode(
						ScriptProgramUtil::makeBoundaryXHighNode(
							config::gameWidth + config::gameOffset.x
						),
						ScriptProgramUtil::makeSetSpawnNode(b4d2a,
						ScriptProgramUtil::makeTimerNode(1,
						ScriptProgramUtil::makeRemoveEntityNode(
						)))
					),
					ScriptProgramUtil::makeStallingIfNode(
						ScriptProgramUtil::makeBoundaryXLowNode(
							config::gameOffset.x
						),
						ScriptProgramUtil::makeSetSpawnNode(b4d2a,
						ScriptProgramUtil::makeTimerNode(1,
						ScriptProgramUtil::makeRemoveEntityNode(
						)))
					),
					ScriptProgramUtil::makeStallingIfNode(
						ScriptProgramUtil::makeBoundaryYHighNode(
							config::gameHeight + config::gameOffset.y
						),
						ScriptProgramUtil::makeSetSpawnNode(b4d2a,
						ScriptProgramUtil::makeTimerNode(1,
						ScriptProgramUtil::makeRemoveEntityNode(
						)))
					),
					ScriptProgramUtil::makeStallingIfNode(
						ScriptProgramUtil::makeBoundaryYLowNode(
							config::gameOffset.y
						),
						ScriptProgramUtil::makeSetSpawnNode(b4d2a,
						ScriptProgramUtil::makeTimerNode(1,
						ScriptProgramUtil::makeRemoveEntityNode(
						)))
					)
				}
			).heapClone()
		}

		, b4d2bNode{
			SpawnProgramUtil::makeRingFormationNode(
				SpawnProgramUtil::makeVelocityFromPolarNode(
					SpawnProgramUtil::makeFloatValueNode(1.7f),
					SpawnProgramUtil::makeAddNode(
						SpawnProgramUtil::makeAngleToPlayerNode(),
						SpawnProgramUtil::makeFloatValueNode(180.0f)
					)
				),
				SpawnProgramUtil::makeIntValueDiffNode({ 5, 7, 7, 9 }),
				SpawnProgramUtil::makeSpawnPosVelNode(
					b4d2bPrototype,
					SpawnProgramUtil::makeEntityPositionNode()
				)
			)
		}

		, b4d2b{ b4d2bNode, 1, false }

			//3
		, b4d3aNode{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(
					SpawnProgramUtil::makeEntityUniformRandomIntNode(
						SpawnProgramUtil::makeIntValueNode(0),
						SpawnProgramUtil::makeIntValueDiffNode({ 440, 400, 340, 280 })
					),
					{ 440, 400, 340, 280 }
				),
				SpawnProgramUtil::makeRingFormationNode(
					SpawnProgramUtil::makeVelocityFromPolarNode(
						SpawnProgramUtil::makeFloatValueNode(0.65f),
						SpawnProgramUtil::randomAngleNode
					),
					SpawnProgramUtil::makeIntValueDiffNode({ 4, 5, 6, 7 }),
					SpawnProgramUtil::makeSpawnPosVelNode(
						smallCyanPrototype,
						SpawnProgramUtil::makeEntityPositionNode()
					)
				)
			)
		}
		, b4d3a{ b4d3aNode, 10000, false }

		, b4d3bPrototype{
			EntityBuilder::makePosVelPrototype(
				SHARP_BULLET_ARGS(sharpHitbox, outbound, L"sharp_orange", 200),
				SpawnProgramList{ b4d3a },
				ScriptProgramList{
					ScriptProgramUtil::makeStallingIfNode(
						ScriptProgramUtil::makeBoundaryYHighNode(
							config::gameHeight + config::gameOffset.y
						),
						ScriptProgramUtil::makeClearSpawnNode()
					)
				}
			).heapClone()
		}

		, b4d3bNode{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, 17),
				SpawnProgramUtil::makeRingFormationNode(
					SpawnProgramUtil::makeVelocityValueNode(Velocity{ 1.6f, 60.0f }),
					6,
					SpawnProgramUtil::makeSpawnPosVelNode(
						b4d3bPrototype,
						SpawnProgramUtil::makeEntityPositionNode()
					)
				)
			)
		}
		, b4d3b{ b4d3bNode, 1700, true }

			//4
		, b4d4aNode{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, { 25, 20, 15, 10 }),
				SpawnProgramUtil::makeRepeatNode(
					8,
					SpawnProgramUtil::makeRingFormationNode(
						SpawnProgramUtil::makeVelocityFromPolarNode(
							SpawnProgramUtil::makeUniformRandomFloatNode(0.25f, 1.9f),
							SpawnProgramUtil::makeSpiralNode(
								SpawnProgramUtil::makeIntValueNode(0), 
								SpawnProgramUtil::makeFloatValueNode(0.0f), 
								SpawnProgramUtil::makeFloatValueDiffNode({
									-1.08362f,
									-1.18362f,
									-1.08362f,
									-1.18362f
								} )
							)
						),
						4,
						SpawnProgramUtil::makeSpawnPosVelNode(
							smallRosePrototype,
							SpawnProgramUtil::makeEntityPositionNode()
						)
					)
				)
			)
		}
		, b4d4a{ b4d4aNode, 275, false }

		, b4d4bNode{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, { 25, 20, 15, 10 }),
				SpawnProgramUtil::makeRepeatNode(
					7,
					SpawnProgramUtil::makeRingFormationNode(
						SpawnProgramUtil::makeVelocityFromPolarNode(
							SpawnProgramUtil::makeUniformRandomFloatNode(0.25f, 1.9f),
							SpawnProgramUtil::makeSpiralNode(
								SpawnProgramUtil::makeIntValueNode(0),
								SpawnProgramUtil::makeFloatValueNode(0.0f),
								SpawnProgramUtil::makeFloatValueDiffNode({
									1.08362f,
									1.18362f,
									1.18362f,
									1.18362f
								})
							)
						),
						4,
						SpawnProgramUtil::makeSpawnPosVelNode(
							smallMagentaPrototype,
							SpawnProgramUtil::makeEntityPositionNode()
						)
					)
				)
			)
		}
		, b4d4b{ b4d4bNode, 275, false }

			//5
		, b4d5aNode{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(
					SpawnProgramUtil::makeEntityUniformRandomIntNode(
						SpawnProgramUtil::makeIntValueNode(0),
						SpawnProgramUtil::makeIntValueDiffNode({ 420, 350, 290, 240 })
					),
					{ 420, 350, 290, 240 }
				),
				SpawnProgramUtil::makeRingFormationNode(
					SpawnProgramUtil::makeVelocityFromPolarNode(
						SpawnProgramUtil::makeFloatValueNode(1.2f),
						SpawnProgramUtil::randomAngleNode
					),
					3,
					SpawnProgramUtil::makeSpawnPosVelNode(
						mediumMagentaPrototype,
						SpawnProgramUtil::makeEntityPositionNode()
					)
				)
			)
		}
		, b4d5a{ b4d5aNode, 10000, false }

		, b4d5bPrototype{
			EntityBuilder::makePosVelPrototype(
				SHARP_BULLET_ARGS(sharpHitbox, outbound, L"sharp_orange", 200),
				SpawnProgramList{ b4d5a },
				ScriptProgramList{
					ScriptProgramUtil::makeStallingIfNode(
						ScriptProgramUtil::makeBoundaryYHighNode(
							config::gameHeight + config::gameOffset.y
						),
						ScriptProgramUtil::makeClearSpawnNode()
					)
				}
			).heapClone()
		}

		, b4d5bNode{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, 17),
				SpawnProgramUtil::makeRingFormationNode(
					SpawnProgramUtil::makeVelocityValueNode(Velocity{ 1.6f, 60.0f }),
					6,
					SpawnProgramUtil::makeSpawnPosVelNode(
						b4d5bPrototype,
						SpawnProgramUtil::makeEntityPositionNode()
					)
				)
			)
		}
		, b4d5b{ b4d5bNode, 1700, true }

			//6
		, b4d6aNode{
			#define COLUMN_NODE(angleOff) \
				SpawnProgramUtil::makeColumnFormationNode( \
					SpawnProgramUtil::makeVelocityFromPolarNode( \
						SpawnProgramUtil::makeFloatValueNode(1.5f), \
						SpawnProgramUtil::makeAddNode( \
							SpawnProgramUtil::makeEntityAngleNode(), \
							SpawnProgramUtil::makeFloatValueNode(angleOff) \
						) \
					), \
					SpawnProgramUtil::makeIntValueDiffNode({ 3, 4, 5, 6 }), \
					SpawnProgramUtil::makeFloatValueNode(-0.2f), \
					SpawnProgramUtil::makeSpawnPosVelNode( \
						sharpBluePrototype, \
						SpawnProgramUtil::makeEntityPositionNode() \
					) \
				)

			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, 25),
				SpawnProgramUtil::makeListNode(
					COLUMN_NODE(90.0f),
					COLUMN_NODE(-90.0f)
				)
			)
			#undef COLUMN_NODE
		}

		, b4d6a{ b4d6aNode, 10000, false }

		, b4d6bPrototype{
			EntityBuilder::makePosVelPrototype(
				//for this danmaku, medium drawn above sharp
				BASIC_BULLET_ARGS(mediumHitbox, outbound, L"medium_blue", 200),
				ScriptProgramList{
					ScriptProgramUtil::makeTimerNode(10, 
					ScriptProgramUtil::makeSetSpawnNode(b4d6a,
					ScriptProgramUtil::makeTimerNode(90,
					ScriptProgramUtil::makeClearSpawnNode(
					))))
				}
			).heapClone()
		}

		, b4d6bNode{
			SpawnProgramUtil::makeRingFormationNode(
				SpawnProgramUtil::makeVelocityFromPolarNode(
					SpawnProgramUtil::makeFloatValueNode(1.2f),
					SpawnProgramUtil::randomAngleNode
				),
				SpawnProgramUtil::makeIntValueDiffNode({ 7, 8, 9, 10 }),
				SpawnProgramUtil::makeSpawnPosVelNode(
					b4d6bPrototype,
					SpawnProgramUtil::makeEntityPositionNode()
				)
			)
		}
		, b4d6b{ b4d6bNode, 1, false }

		, b4d6cNode{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, { 13, 11, 9, 7 }),
				SpawnProgramUtil::makeArcFormationNode(
					SpawnProgramUtil::makeVelocityFromPolarNode(
						SpawnProgramUtil::makeFloatValueNode(1.45f),
						SpawnProgramUtil::makeAngleToPlayerNode()
					),
					SpawnProgramUtil::makeIntValueDiffNode({ 5, 9, 13, 17 }),
					SpawnProgramUtil::makeFloatValueDiffNode(
						{ 25.0f, 20.0f, 15.0f, 10.0f }
					),
					SpawnProgramUtil::makeSpawnPosVelNode(
						smallRedPrototype,
						SpawnProgramUtil::makeEntityPositionNode()
					)
				)
			)
		}

		, b4d6c{ b4d6cNode, 50, false }

			//7
		, b4d7aNode{
			SpawnProgramUtil::makeRepeatNode(
				SpawnProgramUtil::makeIntValueDiffNode({ 6, 8, 10, 13 }),
				SpawnProgramUtil::makeSpawnPosVelNode(
					smallCyanPrototype,
					SpawnProgramUtil::makeEntityPositionNode(),
					SpawnProgramUtil::makeVelocityFromPolarNode(
						SpawnProgramUtil::makeUniformRandomFloatNode(0.1f, 1.5f),
						SpawnProgramUtil::makeAddNode(
							SpawnProgramUtil::makeAngleToPlayerNode(),
							SpawnProgramUtil::makeUniformRandomFloatNode(-20.0f, 20.0f)
						)
					)
				)
			)
		}

		, b4d7a{ b4d7aNode, 1, false }

		, b4d7bPrototype {
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(mediumHitbox, outbound, L"medium_blue", 0),
				ScriptProgramList{
					ScriptProgramUtil::makeStallingIfNode(
						ScriptProgramUtil::makeBoundaryXHighNode(
							config::gameWidth + config::gameOffset.x
						),
						ScriptProgramUtil::makeSetSpawnNode(b4d7a,
						ScriptProgramUtil::makeTimerNode(1,
						ScriptProgramUtil::makeRemoveEntityNode(
						)))
					),
					ScriptProgramUtil::makeStallingIfNode(
						ScriptProgramUtil::makeBoundaryXLowNode(
							config::gameOffset.x
						),
						ScriptProgramUtil::makeSetSpawnNode(b4d7a,
						ScriptProgramUtil::makeTimerNode(1,
						ScriptProgramUtil::makeRemoveEntityNode(
						)))
					),
					ScriptProgramUtil::makeStallingIfNode(
						ScriptProgramUtil::makeBoundaryYHighNode(
							config::gameHeight + config::gameOffset.y
						),
						ScriptProgramUtil::makeSetSpawnNode(b4d7a,
						ScriptProgramUtil::makeTimerNode(1,
						ScriptProgramUtil::makeRemoveEntityNode(
						)))
					),
					ScriptProgramUtil::makeStallingIfNode(
						ScriptProgramUtil::makeBoundaryYLowNode(
							config::gameOffset.y
						),
						ScriptProgramUtil::makeSetSpawnNode(b4d7a,
						ScriptProgramUtil::makeTimerNode(1,
						ScriptProgramUtil::makeRemoveEntityNode(
						)))
					)
				}
			).heapClone()
		}

		, b4d7bNode{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, 15),
				SpawnProgramUtil::makeSpawnPosVelNode(
					b4d7bPrototype,
					SpawnProgramUtil::makeEntityPositionNode(),
					SpawnProgramUtil::makeVelocityFromPolarNode(
						SpawnProgramUtil::makeFloatValueNode(2.1f),
						SpawnProgramUtil::makeAngleToPlayerNode()
					)
				)
			)
		}

		, b4d7b{ b4d7bNode, 100, false }

			//8
		, b4d8aNode{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(
					20,
					{ 175, 150, 125, 100 }
				),
				SpawnProgramUtil::makeRingFormationNode(
					SpawnProgramUtil::makeVelocityFromPolarNode(
						SpawnProgramUtil::makeFloatValueNode(0.7f),
						SpawnProgramUtil::makeEntityAngleNode()
					),
					6,
					SpawnProgramUtil::makeSpawnPosVelNode(
						smallRedPrototype,
						SpawnProgramUtil::makeEntityPositionNode()
					)
				)
			)
		}
		, b4d8a{ b4d8aNode, 10000, false }

		, b4d8bPrototype{
			EntityBuilder::makePosVelPrototype(
				//for this danmaku, medium drawn above
				BASIC_BULLET_ARGS(mediumHitbox, outbound, L"medium_red", 202),
				SpawnProgramList{ b4d8a }
			).heapClone()
		}

		#define GHOST_SPEED 0.4f
		, b4d8bNode{
			SpawnProgramUtil::makeListNode(
				SpawnProgramUtil::makeSpawnPosVelNode(
					b4d8bPrototype,
					SpawnProgramUtil::makeEntityPositionNode(),
					SpawnProgramUtil::makeVelocityValueNode(
						Vector2{ 0.0f, GHOST_SPEED }
					)
				),
				SpawnProgramUtil::makeSpawnPosVelNode(
					b4d8bPrototype,
					SpawnProgramUtil::makeEntityPositionNode(),
					SpawnProgramUtil::makeVelocityValueNode(
						Vector2{ 0.0f, -GHOST_SPEED }
					)
				)
			)
		}

		, b4d8b{ b4d8bNode, 1, false }

		, b4d8cNode{
			SpawnProgramUtil::makeListNode(
				SpawnProgramUtil::makeSpawnPosVelNode(
					b4d8bPrototype,
					SpawnProgramUtil::makeEntityPositionNode(),
					SpawnProgramUtil::makeVelocityValueNode(
						Vector2{ GHOST_SPEED, 0.0f }
					)
				),
				SpawnProgramUtil::makeSpawnPosVelNode(
					b4d8bPrototype,
					SpawnProgramUtil::makeEntityPositionNode(),
					SpawnProgramUtil::makeVelocityValueNode(
						Vector2{ -GHOST_SPEED, 0.0f }
					)
				)
			)
		}
		#undef GHOST_SPEED

		, b4d8c{ b4d8cNode, 1, false }

		, b4d8dPrototype{
			EntityBuilder::makePosVelPrototype(
				//for this danmaku, sharp over small
				SHARP_BULLET_ARGS(sharpHitbox, outbound, L"sharp_azure", 200),
				ScriptProgramList{
					ScriptProgramUtil::makeStallingIfNode(
						ScriptProgramUtil::makeBoundaryXHighNode(
							config::gameWidth + config::gameOffset.x
						),
						ScriptProgramUtil::makeSetSpawnNode(b4d8b)
					),
					ScriptProgramUtil::makeStallingIfNode(
						ScriptProgramUtil::makeBoundaryXLowNode(
							config::gameOffset.x
						),
						ScriptProgramUtil::makeSetSpawnNode(b4d8b)
					),
					ScriptProgramUtil::makeStallingIfNode(
						ScriptProgramUtil::makeBoundaryYHighNode(
							config::gameHeight + config::gameOffset.y
						),
						ScriptProgramUtil::makeSetSpawnNode(b4d8c)
					),
					ScriptProgramUtil::makeStallingIfNode(
						ScriptProgramUtil::makeBoundaryYLowNode(
							config::gameOffset.y
						),
						ScriptProgramUtil::makeSetSpawnNode(b4d8c)
					)
				}
			).heapClone()
		}

		, b4d8dNode{
			SpawnProgramUtil::makeRingFormationNode(
				SpawnProgramUtil::makeVelocityValueNode(Velocity{ 1.3f, 60.0f }),
				6,
				SpawnProgramUtil::makeSpawnPosVelNode(
					b4d8dPrototype,
					SpawnProgramUtil::makeEntityPositionNode()
				)
			)
		}

		, b4d8d{ b4d8dNode, 1, false }

			//the boss
		, b4e1Prototype{
			EntityBuilder::makeLinearUncollidable(
				BOSS_ARGS,
				SpriteInstruction{
					bitmapStoragePointer->get(L"b4_idle_1")->d2dBitmap,
					math::Vector2{ 0.0f, 4.0f }			//offset
				},
				DrawOrder{ config::playerDrawOrder },
				AnimationList{ 
					{
						components::Animation{ {
							L"b4_idle_1", L"b4_idle_2", L"b4_idle_3", L"b4_idle_4"
						} }
					},
					0,	//idle index
					4	//ticks
				},
				ScriptProgramList{
					ScriptProgramUtil::makeBossEntryNode(120, L"4_1",
					ScriptProgramUtil::makeTimerNode(120,
					ScriptProgramUtil::makeSetHealthNode(6500,
					ScriptProgramUtil::makeSetSpawnNode(b4d1b,		//1
					ScriptProgramUtil::makeBossMoveNode(
						90,
						90,
					ScriptProgramUtil::makeBossResetNode(90,
					ScriptProgramUtil::makeSetHealthNode(6200,
					ScriptProgramUtil::makeBossAttackAndMoveNode(
						b4d2b,
						140,
						20,
					ScriptProgramUtil::makeAddSpawnNode(
						pickupProgramsPointer->largePowerSpawnProgram,
					ScriptProgramUtil::makeTimerNode(1,
					ScriptProgramUtil::makeBossResetMidNode(90,
					ScriptProgramUtil::makeSetHealthNode(6500,
					ScriptProgramUtil::makeSetSpawnNode(b4d3b,		//3
					ScriptProgramUtil::makeBossMoveNode(
						90,
						90,
					ScriptProgramUtil::makeBossResetMidNode(90,
					ScriptProgramUtil::makeSetHealthNode(8000,
					ScriptProgramUtil::makeDoubleBossAttackAndMoveNode(
						b4d4a,
						300,
						70,
						b4d4b,
						300,
						70,
					ScriptProgramUtil::makeAddSpawnNode(
						pickupProgramsPointer->largePowerSpawnProgram,
					ScriptProgramUtil::makeTimerNode(1,
					ScriptProgramUtil::makeBossResetMidNode(90,
					ScriptProgramUtil::makeSetHealthNode(6500,
					ScriptProgramUtil::makeSetSpawnNode(b4d5b,		//5
					ScriptProgramUtil::makeBossMoveNode(
						90,
						90,
					ScriptProgramUtil::makeBossResetNode(90,
					ScriptProgramUtil::makeSetHealthNode(6000,
					ScriptProgramUtil::makeDoubleBossAttackAndMoveNode(
						b4d6b,
						70,
						20,
						b4d6c,
						40,
						20,
					ScriptProgramUtil::makeAddSpawnNode(
						pickupProgramsPointer->largePowerSpawnProgram,
					ScriptProgramUtil::makeTimerNode(1,
					ScriptProgramUtil::makeBossResetNode(90,
					ScriptProgramUtil::makeSetHealthNode(5000,
					ScriptProgramUtil::makeBossAttackAndMoveNode(
						b4d7b,
						140,
						20,
					ScriptProgramUtil::makeBossResetNode(90,
					ScriptProgramUtil::makeSetHealthNode(6500,
					ScriptProgramUtil::makeBossAttackAndMoveNode(
						b4d8d,
						140,
						20,
					ScriptProgramUtil::makeBossEndNode()
					))))))))))))))))))))))))))))))))))
				}
			).heapClone()
		}
		, b4e1Node{
			SpawnProgramUtil::makeSpawnNode(b4e1Prototype)
		}
		, b4e1{ b4e1Node, 1, false }

		
		, stage4ScriptProgram{
			ScriptProgramUtil::makeTimerNode(160,
			ScriptProgramUtil::makeSetSpawnNode(s4e1,
			ScriptProgramUtil::makeTimerNode(730,
			ScriptProgramUtil::makeAddSpawnNode(s4e2a,
			ScriptProgramUtil::makeTimerNode(480,
			ScriptProgramUtil::makeAddSpawnNode(s4e2b,
			ScriptProgramUtil::makeTimerNode(500,
			ScriptProgramUtil::makeAddSpawnNode(s4e3,
			ScriptProgramUtil::makeTimerNode(430,
			ScriptProgramUtil::makeAddSpawnNode(s4e4,
			ScriptProgramUtil::makeTimerNode(500,
			ScriptProgramUtil::makeAddSpawnNode(s4e5,
			ScriptProgramUtil::makeTimerNode(440,
			ScriptProgramUtil::makeAddSpawnNode(s4e6,
			ScriptProgramUtil::makeTimerNode(440,
			ScriptProgramUtil::makeAddSpawnNode(s4e7,
			ScriptProgramUtil::makeTimerNode(440,
			ScriptProgramUtil::makeAddSpawnNode(s4e6,	//wave 8
			ScriptProgramUtil::makeTimerNode(440,
			ScriptProgramUtil::makeAddSpawnNode(s4e9,
			ScriptProgramUtil::makeTimerNode(440,
			ScriptProgramUtil::makeAddSpawnNode(s4e10,
			ScriptProgramUtil::makeTimerNode(440,
			ScriptProgramUtil::makeAddSpawnNode(s4e11,
			ScriptProgramUtil::makeTimerNode(950,
			ScriptProgramUtil::makeAddSpawnNode(b4e1,
			ScriptProgramUtil::makeStallingIfNode(
				ScriptProgramUtil::makeIsWinNode(),
				ScriptProgramUtil::makeTimerNode(waitEndTime,
				ScriptProgramUtil::makeShowDialogueNode(L"4_2",
				ScriptProgramUtil::makeStallingIfNode(
					ScriptProgramUtil::makeIsDialogueOverNode(),
					ScriptProgramUtil::makeTimerNode(1,
					ScriptProgramUtil::makeEndStageNode()
				))
			)
			))))))))))))))))))))))))))))
			
		}

		// STAGE 5 // STAGE 5 // STAGE 5 // STAGE 5 // STAGE 5 // STAGE 5 // STAGE 5 //

		#define DANMAKU_TICK_MOD { 170, 130, 100, 75 }
		#define ROSE_TICK_MOD { 170/2, 130/2, 100/2, 75/2 }
		#define HEALTH 80
		#define SPAWN_SPEED 2.7f
		#define SPAWN_TICK_MOD 8
		#define WAIT_TIME 55
		#define TURN_TIME 50

			//wave 1
		#define INIT_ANGLE 190.0f	
		#define SPAWN_Y 50.0f

		, s5d1Prototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(smallHitbox, outbound, L"small_spring", 200),
				ScriptProgramList{
					ScriptProgramUtil::makeShiftSpeedPeriodNode(
						0.85f,
						80
					)
				}
			).heapClone()
		}
		, s5d1Node{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(
					SpawnProgramUtil::makeEntityUniformRandomIntNode(
						SpawnProgramUtil::makeIntValueNode(0),
						SpawnProgramUtil::makeIntValueDiffNode(DANMAKU_TICK_MOD)
					),
					DANMAKU_TICK_MOD
				),
				SpawnProgramUtil::makeSpawnPosVelNode(
					s5d1Prototype,
					SpawnProgramUtil::makeEntityPositionNode(),
					SpawnProgramUtil::makeVelocityFromPolarNode(
						SpawnProgramUtil::makeUniformRandomFloatNode(-2.3f, -1.3f),
						SpawnProgramUtil::makeUniformRandomFloatNode(268.0f, 272.0f)
					)
				)
			)
		}

		, s5d1{ s5d1Node, 10000, false }

		, s5e1Prototype{
			EntityBuilder::makePosVelPrototype(
				FLAME_ARGS(HEALTH, pickupProgramsPointer->smallPowerHalfSpawnProgram),
				SpawnProgramList{ s5d1 },
				ScriptProgramList{
					ScriptProgramUtil::makeTimerNode(WAIT_TIME,
					ScriptProgramUtil::makeShiftVelocityTurnLongPeriodNode(
						Velocity{ SPAWN_SPEED, -120.0f },
						INIT_ANGLE,
						TURN_TIME
					))
				}
			).heapClone()
		}

		, s5e1Node{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, SPAWN_TICK_MOD),
				SpawnProgramUtil::makeSpawnPosVelNode(
					s5e1Prototype,
					SpawnProgramUtil::makePointValueNode({
						config::rightOut,
						SPAWN_Y
					}),
					SpawnProgramUtil::makeVelocityValueNode(
						Velocity{ SPAWN_SPEED, INIT_ANGLE }
					)
				)
			)
		}
		, s5e1{ s5e1Node, 90 + 320 + 320 + 320, false }
		#undef INIT_ANGLE
		#undef SPAWN_Y

			//wave 2
		#define INIT_ANGLE -10.0f	
		#define SPAWN_Y 70.0f

		, s5d2Prototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(smallHitbox, outbound, L"small_azure", 200),
				ScriptProgramList{
					ScriptProgramUtil::makeShiftSpeedPeriodNode(
						1.05f,
						80
					)
				}
			).heapClone()
		}
		, s5d2Node{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(
					SpawnProgramUtil::makeEntityUniformRandomIntNode(
						SpawnProgramUtil::makeIntValueNode(0),
						SpawnProgramUtil::makeIntValueDiffNode(DANMAKU_TICK_MOD)
					),
					DANMAKU_TICK_MOD
				),
				SpawnProgramUtil::makeSpawnPosVelNode(
					s5d2Prototype,
					SpawnProgramUtil::makeEntityPositionNode(),
					SpawnProgramUtil::makeVelocityFromPolarNode(
						SpawnProgramUtil::makeUniformRandomFloatNode(-2.3f, -1.3f),
						SpawnProgramUtil::makeUniformRandomFloatNode(268.0f, 272.0f)
					)
				)
			)
		}

		, s5d2{ s5d2Node, 10000, false }

		, s5e2Prototype{
			EntityBuilder::makePosVelPrototype(
				FLAME_ARGS(HEALTH, pickupProgramsPointer->smallPowerHalfSpawnProgram),
				SpawnProgramList{ s5d2 },
				ScriptProgramList{
					ScriptProgramUtil::makeTimerNode(WAIT_TIME,
					ScriptProgramUtil::makeShiftVelocityTurnLongPeriodNode(
						Velocity{ SPAWN_SPEED, Angle{ -120.0f }.flipY() },
						INIT_ANGLE,
						TURN_TIME
					))
				}
			).heapClone()
		}

		, s5e2Node{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, SPAWN_TICK_MOD),
				SpawnProgramUtil::makeSpawnPosVelNode(
					s5e2Prototype,
					SpawnProgramUtil::makePointValueNode({
						config::leftOut,
						SPAWN_Y
					}),
					SpawnProgramUtil::makeVelocityValueNode(
						Velocity{ SPAWN_SPEED, INIT_ANGLE }
					)
				)
			)
		}
		, s5e2{ s5e2Node, 90 + 320 + 320, false }
		#undef INIT_ANGLE
		#undef SPAWN_Y

			//wave 3
		#define INIT_ANGLE 190.0f	
		#define SPAWN_Y 90.0f

		, s5d3Prototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(smallHitbox, outbound, L"small_violet", 200),
				ScriptProgramList{
					ScriptProgramUtil::makeShiftSpeedPeriodNode(
						1.25f,
						80
					)
				}
			).heapClone()
		}
		, s5d3Node{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(
					SpawnProgramUtil::makeEntityUniformRandomIntNode(
						SpawnProgramUtil::makeIntValueNode(0),
						SpawnProgramUtil::makeIntValueDiffNode(DANMAKU_TICK_MOD)
					),
					DANMAKU_TICK_MOD
				),
				SpawnProgramUtil::makeSpawnPosVelNode(
					s5d3Prototype,
					SpawnProgramUtil::makeEntityPositionNode(),
					SpawnProgramUtil::makeVelocityFromPolarNode(
						SpawnProgramUtil::makeUniformRandomFloatNode(-2.3f, -1.3f),
						SpawnProgramUtil::makeUniformRandomFloatNode(268.0f, 272.0f)
					)
				)
			)
		}

		, s5d3{ s5d3Node, 10000, false }

		, s5e3Prototype{
			EntityBuilder::makePosVelPrototype(
				FLAME_ARGS(HEALTH, pickupProgramsPointer->smallPowerHalfSpawnProgram),
				SpawnProgramList{ s5d3 },
				ScriptProgramList{
					ScriptProgramUtil::makeTimerNode(WAIT_TIME,
					ScriptProgramUtil::makeShiftVelocityTurnLongPeriodNode(
						Velocity{ SPAWN_SPEED, -120.0f },
						INIT_ANGLE,
						TURN_TIME
					))
				}
			).heapClone()
		}

		, s5e3Node{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, SPAWN_TICK_MOD),
				SpawnProgramUtil::makeSpawnPosVelNode(
					s5e3Prototype,
					SpawnProgramUtil::makePointValueNode({
						config::rightOut,
						SPAWN_Y
					}),
					SpawnProgramUtil::makeVelocityValueNode(
						Velocity{ SPAWN_SPEED, INIT_ANGLE }
					)
				)
			)
		}
		, s5e3{ s5e3Node, 90 + 320, false }
		#undef INIT_ANGLE
		#undef SPAWN_Y

			//wave 4
		#define INIT_ANGLE -10.0f	
		#define SPAWN_Y 110.0f

		, s5d4Prototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(smallHitbox, outbound, L"small_rose", 200),
				ScriptProgramList{
					ScriptProgramUtil::makeShiftSpeedPeriodNode(
						1.45f,
						80
					)
				}
			).heapClone()
		}
		, s5d4Node{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(
					SpawnProgramUtil::makeEntityUniformRandomIntNode(
						SpawnProgramUtil::makeIntValueNode(0),
						SpawnProgramUtil::makeIntValueDiffNode(ROSE_TICK_MOD)
					),
					ROSE_TICK_MOD
				),
				SpawnProgramUtil::makeSpawnPosVelNode(
					s5d4Prototype,
					SpawnProgramUtil::makeEntityPositionNode(),
					SpawnProgramUtil::makeVelocityFromPolarNode(
						SpawnProgramUtil::makeUniformRandomFloatNode(-2.3f, -1.3f),
						SpawnProgramUtil::makeUniformRandomFloatNode(268.0f, 272.0f)
					)
				)
			)
		}

		, s5d4{ s5d4Node, 10000, false }

		, s5e4Prototype{
			EntityBuilder::makePosVelPrototype(
				FLAME_ARGS(HEALTH, pickupProgramsPointer->smallPowerHalfSpawnProgram),
				SpawnProgramList{ s5d4 },
				ScriptProgramList{
					ScriptProgramUtil::makeTimerNode(WAIT_TIME,
					ScriptProgramUtil::makeShiftVelocityTurnLongPeriodNode(
						Velocity{ SPAWN_SPEED, Angle{ -120.0f }.flipY() },
						INIT_ANGLE,
						TURN_TIME
					))
				}
			).heapClone()
		}

		, s5e4Node{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, SPAWN_TICK_MOD),
				SpawnProgramUtil::makeSpawnPosVelNode(
					s5e4Prototype,
					SpawnProgramUtil::makePointValueNode({
						config::leftOut,
						SPAWN_Y
					}),
					SpawnProgramUtil::makeVelocityValueNode(
						Velocity{ SPAWN_SPEED, INIT_ANGLE }
					)
				)
			)
		}
		, s5e4{ s5e4Node, 90, false }
		#undef INIT_ANGLE
		#undef SPAWN_Y


		#undef DANMAKU_TICK_MOD
		#undef ROSE_TICK_MOD
		#undef HEALTH
		#undef SPAWN_SPEED
		#undef SPAWN_TICK_MOD
		#undef WAIT_TIME
		#undef TURN_TIME

			//wave 5
		, s5d5Prototype{
			EntityBuilder::makePosVelPrototype(
				SHARP_BULLET_ARGS(sharpHitbox, outbound, L"sharp_chartreuse", 100),
				ScriptProgramList{
					ScriptProgramUtil::makeTimerNode(35,
					ScriptProgramUtil::makeShiftSpeedPeriodNode(
						0.25f,
						30
					))
				}
			).heapClone()
		}
		, s5d5Node{
			#define SPAWN_IN_DIRECTION(angle) \
				SpawnProgramUtil::makeSpawnPosVelNode( \
					s5d5Prototype, \
					SpawnProgramUtil::makeEntityPositionNode(), \
					SpawnProgramUtil::makeVelocityFromPolarNode( \
						SpawnProgramUtil::makeUniformRandomFloatNode(2.6f, 3.0f), \
						SpawnProgramUtil::makeAddNode( \
							SpawnProgramUtil::makeFloatValueNode(angle), \
							SpawnProgramUtil::makeUniformRandomFloatNode( \
								-30.0f, \
								30.0f \
							) \
						) \
					) \
				) 
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, { 14, 10, 7, 5 }
				),
				SpawnProgramUtil::makeListNode(
					SPAWN_IN_DIRECTION(0.0f),
					SPAWN_IN_DIRECTION(60.0f),
					SPAWN_IN_DIRECTION(120.0f),
					SPAWN_IN_DIRECTION(180.0f),
					SPAWN_IN_DIRECTION(240.0f),
					SPAWN_IN_DIRECTION(300.0f)
				)
			)
			#undef SPAWN_IN_DIRECTION
		}

		, s5d5{ s5d5Node, 600, false }

		, s5e5Prototype{
			EntityBuilder::makePosVelPrototype(
				WISP_ARGS(4500, pickupProgramsPointer->lifeSpawnProgram),
				ScriptProgramList{
					ScriptProgramUtil::makeShootOnceAndLeaveProgram(
						30,
						45,
						40,
						s5d5,
						60,
						{ 1.3f, -90.0f},
						100
					)
				}
			).heapClone()
		}

		, s5e5Node{
			SpawnProgramUtil::makeSpawnPosVelNode(
				s5e5Prototype,
				SpawnProgramUtil::makePointValueNode({
					config::gameWidth / 2.0f + config::gameOffset.x,
					config::topOut
				}),
				SpawnProgramUtil::makeVelocityValueNode(
					Velocity{ 1.3f, -90.0f }
				)
			)
		}
		, s5e5{ s5e5Node, 1, false }

		// BOSS 5 // BOSS 5 // BOSS 5 // BOSS 5 // BOSS 5 // BOSS 5 // BOSS 5 // 

			//1
		, b5d1Node{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, { 19, 18, 17, 16 }),
				SpawnProgramUtil::makeListNode(
					SpawnProgramUtil::makeRingFormationNode(
						SpawnProgramUtil::makeVelocityFromPolarNode(
							SpawnProgramUtil::makeFloatValueNode(1.6f),
							SpawnProgramUtil::makeSpiralNode(
								SpawnProgramUtil::makeIntValueNode(1), 
								SpawnProgramUtil::entityRandomAngleNode,
								SpawnProgramUtil::makeFloatValueDiffNode(
									{ 1.1f, 1.2f, 1.3f, 1.4f }
								)
							)
						),
						{ 3, 4, 5, 6 },
						SpawnProgramUtil::makeSpawnPosVelNode(
							largeBluePrototype,
							SpawnProgramUtil::makeEntityPositionNode()
						)
					),
					SpawnProgramUtil::makeRingFormationNode(
						SpawnProgramUtil::makeVelocityFromPolarNode(
							SpawnProgramUtil::makeFloatValueNode(0.8f),
							SpawnProgramUtil::makeSpiralNode(
								SpawnProgramUtil::makeIntValueNode(40),
								SpawnProgramUtil::entityRandomAngleNode,
								SpawnProgramUtil::makeFloatValueNode(1.0f)
							)
						),
						3,
						SpawnProgramUtil::makeSpawnPosVelNode(
							largeCyanPrototype,
							SpawnProgramUtil::makeEntityPositionNode()
						)
					),
					SpawnProgramUtil::makeRingFormationNode(
						SpawnProgramUtil::makeVelocityFromPolarNode(
							SpawnProgramUtil::makeFloatValueNode(1.2f),
							SpawnProgramUtil::makeSpiralNode(
								SpawnProgramUtil::makeIntValueNode(80),
								SpawnProgramUtil::entityRandomAngleNode,
								SpawnProgramUtil::makeFloatValueNode(-0.751f)
							)
						),
						3,
						SpawnProgramUtil::makeArcFormationNode(
							SpawnProgramUtil::makeIntValueDiffNode({ 2, 2, 2, 3 }),
							SpawnProgramUtil::makeFloatValueNode(15.0f),
							SpawnProgramUtil::makeSpawnPosVelNode(
								sharpYellowPrototype,
								SpawnProgramUtil::makeEntityPositionNode()
							)
						)
					)
				)
			)
		}

		, b5d1{ b5d1Node, 400, false }

			//2

		, b5d2aPrototype{
			EntityBuilder::makePosVelPrototype(
				SHARP_BULLET_ARGS(sharpHitbox, outbound, L"sharp_yellow", 100),
				ScriptProgramList{
					ScriptProgramUtil::makeTimerNode(50,
					ScriptProgramUtil::makeShiftSpeedPeriodNode(
						0.75f,
						50
					))
				}
			).heapClone()
		}
		, b5d2aNode{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, 3),
				SpawnProgramUtil::makeSpawnPosVelNode(
					b5d2aPrototype,
					SpawnProgramUtil::makeEntityOffsetNode(
						SpawnProgramUtil::makeVelocityFromPolarNode(
							SpawnProgramUtil::makeFloatValueNode(10.0f),
							SpawnProgramUtil::makeSpiralNode(
								SpawnProgramUtil::makeIntValueNode(0),
								SpawnProgramUtil::entityRandomAngleNode,
								SpawnProgramUtil::makeFloatValueNode(9.7f)
							)
						)
					),
					SpawnProgramUtil::makeVelocityFromPolarNode(
						SpawnProgramUtil::makeFloatValueNode(0.0f),
						SpawnProgramUtil::makeSpiralNode(
							SpawnProgramUtil::makeIntValueNode(0),
							SpawnProgramUtil::entityRandomAngleNode,
							SpawnProgramUtil::makeFloatValueNode(-5.7f)
						)
					)
				)
			)
		}

		, b5d2a{ b5d2aNode, 8000, false }

		, b5d2bPrototype{
			EntityBuilder::makePosVelPrototype(
				//for this danmaku, small drawn above sharp
				BASIC_BULLET_ARGS(smallHitbox, outbound, L"small_blue", 200),
				SpawnProgramList{ b5d2a }
			).heapClone()
		}

		, b5d2bNode{
			SpawnProgramUtil::makeArcFormationNode(
				SpawnProgramUtil::makeVelocityValueNode(Vector2{ 1.5f, 0.0f }),
				SpawnProgramUtil::makeIntValueDiffNode({ 2, 3, 4, 5 }),
				SpawnProgramUtil::makeFloatValueDiffNode(
					{ 23.0f, 20.0f, 18.0f, 16.0f }
				),
				SpawnProgramUtil::makeVelConsumerMirrorFormationNode(
					SpawnProgramUtil::makeEntityPositionNode(),
					SpawnProgramUtil::makeSpawnPosVelNode(
						b5d2bPrototype
					)
				)
			)
		}

		, b5d2b{ b5d2bNode, 1, false }

			//3

		#define SPEED 0.8f
		, b5d3aNode{
			SpawnProgramUtil::makeRingFormationNode(
				SpawnProgramUtil::makeVelocityFromPolarNode(
					SpawnProgramUtil::makeFloatValueNode(SPEED),
					SpawnProgramUtil::randomAngleNode
				),
				SpawnProgramUtil::makeIntValueDiffNode({ 5, 6, 7, 8 }),
				SpawnProgramUtil::makeSpawnPosVelNode(
					smallSpringPrototype,
					SpawnProgramUtil::makeEntityPositionNode()
				)
			)
		}

		, b5d3a{ b5d3aNode, 1, false }

		, b5d3bPrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(mediumHitbox, outbound, L"medium_spring", 0),
				ScriptProgramList{
					ScriptProgramUtil::makeTimerNode(50,
					ScriptProgramUtil::makeSetSpawnNode(b5d3a
					))
				}
			).heapClone()
		}

		, b5d3bNode{
			SpawnProgramUtil::makeRingFormationNode(
				SpawnProgramUtil::makeVelocityFromPolarNode(
					SpawnProgramUtil::makeFloatValueNode(SPEED),
					SpawnProgramUtil::randomAngleNode
				),
				SpawnProgramUtil::makeIntValueDiffNode({ 4, 5, 6, 7 }),
				SpawnProgramUtil::makeSpawnPosVelNode(
					b5d3bPrototype,
					SpawnProgramUtil::makeEntityPositionNode()
				)
			)
		}

		, b5d3b{ b5d3bNode, 1, false }

		, b5d3cPrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(largeHitbox, outbound, L"large_spring", -100),
				ScriptProgramList{
					ScriptProgramUtil::makeTimerNode(50,
					ScriptProgramUtil::makeSetSpawnNode(b5d3b
					))
				}
			).heapClone()
		}

		, b5d3cNode{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, 65),
				SpawnProgramUtil::makeSpawnPosVelNode(
					b5d3cPrototype,
					SpawnProgramUtil::makeEntityPositionNode(),
					SpawnProgramUtil::makeVelocityFromPolarNode(
						SpawnProgramUtil::makeFloatValueNode(SPEED),
						SpawnProgramUtil::makeAddNode(
							SpawnProgramUtil::makeAngleToPlayerNode(),
							SpawnProgramUtil::makeUniformRandomFloatNode(-10.0f, 10.0f)
						)
					)
				)
			)
		}

		, b5d3c{ b5d3cNode, 65, true }
		#undef SPEED

			//4
		, b5d4aNode{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(
					SpawnProgramUtil::makeEntityUniformRandomIntNode(
						SpawnProgramUtil::makeIntValueNode(0),
						SpawnProgramUtil::makeIntValueDiffNode({ 140, 115, 90, 70 })
					),
					{ 140, 115, 90, 70 }
				),
				SpawnProgramUtil::makeMirrorFormationNode(
					SpawnProgramUtil::makeEntityPositionNode(),
					SpawnProgramUtil::makeVelocityFromPolarNode(
						SpawnProgramUtil::makeFloatValueNode(0.4f),
						SpawnProgramUtil::makeUniformRandomFloatNode(-2.0f, 2.0f)
					),
					SpawnProgramUtil::makeSpawnPosVelNode(
						smallMagentaPrototype
					)
				)
			)
		}
		, b5d4a{ b5d4aNode, 10000, false }

		, b5d4bNode{
			SpawnProgramUtil::makeSpawnPosVelNode(
				sharpBluePrototype,
				SpawnProgramUtil::makeEntityPositionNode(),
				SpawnProgramUtil::makeVelocityFromPolarNode(
					SpawnProgramUtil::makeFloatValueNode(0.95f),
					SpawnProgramUtil::makeAngleToPlayerNode()
				)
			)
		}

		, b5d4b{ b5d4bNode, 1, false }

		, b5d4cPrototype{
			EntityBuilder::makePosVelPrototype(
				//for this danmaku, medium drawn over small
				BASIC_BULLET_ARGS(mediumHitbox, outbound, L"medium_blue", 200),
				SpawnProgramList{ b5d4a },
				ScriptProgramList{
					ScriptProgramUtil::makeStallingIfNode(
						ScriptProgramUtil::makeBoundaryXHighNode(
							config::gameWidth + config::gameOffset.x
						),
						ScriptProgramUtil::makeSetSpawnNode(b5d4b,
						ScriptProgramUtil::makeTimerNode(1,
						ScriptProgramUtil::makeRemoveEntityNode(
						)))
					),
					ScriptProgramUtil::makeStallingIfNode(
						ScriptProgramUtil::makeBoundaryXLowNode(
							config::gameOffset.x
						),
						ScriptProgramUtil::makeSetSpawnNode(b5d4b,
						ScriptProgramUtil::makeTimerNode(1,
						ScriptProgramUtil::makeRemoveEntityNode(
						)))
					),
					ScriptProgramUtil::makeStallingIfNode(
						ScriptProgramUtil::makeBoundaryYHighNode(
							config::gameHeight + config::gameOffset.y
						),
						ScriptProgramUtil::makeSetSpawnNode(b5d4b,
						ScriptProgramUtil::makeTimerNode(1,
						ScriptProgramUtil::makeRemoveEntityNode(
						)))
					),
					ScriptProgramUtil::makeStallingIfNode(
						ScriptProgramUtil::makeBoundaryYLowNode(
							config::gameOffset.y
						),
						ScriptProgramUtil::makeSetSpawnNode(b5d4b,
						ScriptProgramUtil::makeTimerNode(1,
						ScriptProgramUtil::makeRemoveEntityNode(
						)))
					)
				}
			).heapClone()
		}

		, b5d4cNode{
			#define SPEED 0.6f
			#define SPEED_ADD 0.4f
			SpawnProgramUtil::makeListNode(
				SpawnProgramUtil::makeSpawnPosVelNode(
					b5d4cPrototype,
					SpawnProgramUtil::makeEntityPositionNode(),
					SpawnProgramUtil::makeAddNode(
						SpawnProgramUtil::makeVelocityFromPolarNode(
							SpawnProgramUtil::makeFloatValueNode(SPEED),
							SpawnProgramUtil::makeAngleToPlayerNode()
						),
						SpawnProgramUtil::makeVelocityFromPolarNode(
							SpawnProgramUtil::makeFloatValueNode(SPEED_ADD),
							SpawnProgramUtil::makeAngleToPlayerNode()
						)
					)
					
				),
				SpawnProgramUtil::makeArcFormationNode(
					SpawnProgramUtil::makeVelocityFromPolarNode(
						SpawnProgramUtil::makeFloatValueNode(SPEED),
						SpawnProgramUtil::makeAddNode(
							SpawnProgramUtil::makeAngleToPlayerNode(),
							SpawnProgramUtil::makeFloatValueNode(180.0f)
						)
					),
					SpawnProgramUtil::makeIntValueDiffNode({ 12, 15, 18, 22 }),
					SpawnProgramUtil::makeFloatValueDiffNode({
						290.0f / 12.0f,
						290.0f / 15.0f,
						290.0f / 18.0f,
						290.0f / 22.0f
					} ),
					SpawnProgramUtil::makeAddNode(
						SpawnProgramUtil::makeVelocityFromPolarNode(
							SpawnProgramUtil::makeFloatValueNode(SPEED_ADD),
							SpawnProgramUtil::makeAngleToPlayerNode()
						),
						SpawnProgramUtil::makeSpawnPosVelNode(
							b5d4cPrototype,
							SpawnProgramUtil::makeEntityPositionNode()
						)
					)
				)
			)
			#undef SPEED
			#undef SPEED_ADD
		}

		, b5d4c{ b5d4cNode, 1, false }

			//5
		#define TICK_MOD { 68, 55, 45, 38 }
		#define SPEED 0.35f
		#define ANGULAR_VEL 0.3f
		, b5d5aNode{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, TICK_MOD),
				SpawnProgramUtil::makeRingFormationNode(
					SpawnProgramUtil::makeVelocityFromPolarNode(
						SpawnProgramUtil::makeFloatValueNode(SPEED),
						SpawnProgramUtil::makeSpiralNode(
							SpawnProgramUtil::makeIntValueNode(0),
							SpawnProgramUtil::entityRandomAngleNode,
							SpawnProgramUtil::makeFloatValueNode(ANGULAR_VEL)
						)
					),
					2,
					SpawnProgramUtil::makeSpawnPosVelNode(
						sharpVioletPrototype,
						SpawnProgramUtil::makeEntityPositionNode()
					)
				)
			)
		}

		, b5d5a{ b5d5aNode, 7777, false }

		, b5d5bNode{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, TICK_MOD),
				SpawnProgramUtil::makeRingFormationNode(
					SpawnProgramUtil::makeVelocityFromPolarNode(
						SpawnProgramUtil::makeFloatValueNode(SPEED),
						SpawnProgramUtil::makeSpiralNode(
							SpawnProgramUtil::makeIntValueNode(0),
							SpawnProgramUtil::entityRandomAngleNode,
							SpawnProgramUtil::makeFloatValueNode(-ANGULAR_VEL)
						)
					),
					2,
					SpawnProgramUtil::makeSpawnPosVelNode(
						sharpRosePrototype,
						SpawnProgramUtil::makeEntityPositionNode()
					)
				)
			)
		}

		, b5d5b{ b5d5bNode, 7777, false }

		#undef TICK_MOD
		#undef SPEED
		#undef ANGULAR_VEL

		, b5d5cPrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(mediumHitbox, outbound, L"medium_magenta", -100),
				SpawnProgramList{ b5d5a },
				ScriptProgramList{
					ScriptProgramUtil::makeStallingIfNode(
						ScriptProgramUtil::makeBoundaryYHighNode(
							config::gameHeight + config::gameOffset.y - 30.0f
						),
						ScriptProgramUtil::makeClearSpawnNode()
					)
				}
			).heapClone()
		}
		, b5d5dPrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(mediumHitbox, outbound, L"medium_magenta", -100),
				SpawnProgramList{ b5d5b },
				ScriptProgramList{
					ScriptProgramUtil::makeStallingIfNode(
						ScriptProgramUtil::makeBoundaryYHighNode(
							config::gameHeight + config::gameOffset.y - 30.0f
						),
						ScriptProgramUtil::makeClearSpawnNode()
					)
				}
			).heapClone()
		}
		, b5d5dNode{
			#define SPEED 1.6f
			#define ANGULAR_VEL 0.73f
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, 20),
				SpawnProgramUtil::makeListNode(
					SpawnProgramUtil::makeRingFormationNode(
						SpawnProgramUtil::makeVelocityFromPolarNode(
							SpawnProgramUtil::makeFloatValueNode(SPEED),
							SpawnProgramUtil::makeSpiralNode(500, 45.0f, ANGULAR_VEL)
						),
						2,
						SpawnProgramUtil::makeSpawnPosVelNode(
							b5d5cPrototype,
							SpawnProgramUtil::makeEntityPositionNode()
						)
					),
					SpawnProgramUtil::makeRingFormationNode(
						SpawnProgramUtil::makeVelocityFromPolarNode(
							SpawnProgramUtil::makeFloatValueNode(SPEED),
							SpawnProgramUtil::makeSpiralNode(500, 135.0f, -ANGULAR_VEL)
						),
						2,
						SpawnProgramUtil::makeSpawnPosVelNode(
							b5d5dPrototype,
							SpawnProgramUtil::makeEntityPositionNode()
						)
					)
				)
			)
			#undef SPEED
			#undef ANGULAR_VEL
		}

		, b5d5d{ b5d5dNode, 500, false }

			//6

		, b5d6Node{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, { 10, 8, 6, 5 }),
				SpawnProgramUtil::makeRingFormationNode(
					SpawnProgramUtil::makeVelocityFromPolarNode(
						SpawnProgramUtil::makeFloatValueNode(1.6f),
						SpawnProgramUtil::makeSpiralNode(
							SpawnProgramUtil::makeIntValueNode(0),
							SpawnProgramUtil::entityRandomAngleNode,
							SpawnProgramUtil::makeFloatValueDiffNode({ 
								((360.0f / math::phi) / 10.0f) / 10.0f,
								((360.0f / math::phi) / 8.0f) / 13.0f,
								((360.0f / math::phi) / 6.0f) / 16.0f,
								((360.0f / math::phi) / 5.0f) / 19.0f
							} )
						)
					),
					SpawnProgramUtil::makeIntValueDiffNode({ 10, 13, 16, 19 }),
					SpawnProgramUtil::makeSpawnPosVelNode(
						smallVioletPrototype,
						SpawnProgramUtil::makeEntityPositionNode()
					)
				)
			)
		}

		, b5d6{ b5d6Node, 550, false }

			//7

		, b5d7aPrototype{
			EntityBuilder::makePosVelPrototype(
				SHARP_BULLET_ARGS(sharpHitbox, outbound, L"sharp_cyan", 100),
				ScriptProgramList{
					ScriptProgramUtil::makeTimerNode(50,
					ScriptProgramUtil::makeShiftSpeedPeriodNode(
						0.75f,
						50
					))
				}
			).heapClone()
		}

		, b5d7aNode{
			#define SPEED 0.0f
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, 20),
				SpawnProgramUtil::makeListNode(
					SpawnProgramUtil::makeSpawnPosVelNode(
						b5d7aPrototype,
						SpawnProgramUtil::makeEntityPositionNode(),
						SpawnProgramUtil::makeVelocityFromPolarNode(
							SpawnProgramUtil::makeFloatValueNode(SPEED),
							SpawnProgramUtil::makeAddNode(
								SpawnProgramUtil::makeEntityAngleNode(),
								SpawnProgramUtil::makeFloatValueNode(90.0f)
							)
						)
					),
					SpawnProgramUtil::makeSpawnPosVelNode(
						b5d7aPrototype,
						SpawnProgramUtil::makeEntityPositionNode(),
						SpawnProgramUtil::makeVelocityFromPolarNode(
							SpawnProgramUtil::makeFloatValueNode(SPEED),
							SpawnProgramUtil::makeAddNode(
								SpawnProgramUtil::makeEntityAngleNode(),
								SpawnProgramUtil::makeFloatValueNode(-90.0f)
							)
						)
					)
				)
			)
			#undef SPEED
		}
		, b5d7a{ b5d7aNode, 10000, false }

		, b5d7bPrototype{
			EntityBuilder::makePosVelPrototype(
				//for this danmaku, medium drawn above
				BASIC_BULLET_ARGS(mediumHitbox, outbound, L"medium_blue", 200),
				SpawnProgramList{ b5d7a },
				ScriptProgramList{
					ScriptProgramUtil::makeStallingIfNode(
						ScriptProgramUtil::makeBoundaryXHighNode(
							config::gameWidth + config::gameOffset.x
						),
						ScriptProgramUtil::makeSetSpawnNode(b5d4b,
						ScriptProgramUtil::makeTimerNode(1,
						ScriptProgramUtil::makeRemoveEntityNode(
						)))
					),
					ScriptProgramUtil::makeStallingIfNode(
						ScriptProgramUtil::makeBoundaryXLowNode(
							config::gameOffset.x
						),
						ScriptProgramUtil::makeSetSpawnNode(b5d4b,
						ScriptProgramUtil::makeTimerNode(1,
						ScriptProgramUtil::makeRemoveEntityNode(
						)))
					),
					ScriptProgramUtil::makeStallingIfNode(
						ScriptProgramUtil::makeBoundaryYHighNode(
							config::gameHeight + config::gameOffset.y
						),
						ScriptProgramUtil::makeSetSpawnNode(b5d4b,
						ScriptProgramUtil::makeTimerNode(1,
						ScriptProgramUtil::makeRemoveEntityNode(
						)))
					),
					ScriptProgramUtil::makeStallingIfNode(
						ScriptProgramUtil::makeBoundaryYLowNode(
							config::gameOffset.y
						),
						ScriptProgramUtil::makeSetSpawnNode(b5d4b,
						ScriptProgramUtil::makeTimerNode(1,
						ScriptProgramUtil::makeRemoveEntityNode(
						)))
					)
				}
			).heapClone()
		}

		#define MAX_TICK 120
		#define TICK_MOD { 20, 15, 12, 10 }
		#define ANGULAR_VEL_E 160.0f / -120.0f
		#define ANGULAR_VEL_N 165.0f / -120.0f
		#define ANGULAR_VEL_H 170.0f / -120.0f
		#define ANGULAR_VEL_L 175.0f / -120.0f
		#define SPEED 1.5f

		, b5d7bNode{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, TICK_MOD),
				SpawnProgramUtil::makeRingFormationNode(
					SpawnProgramUtil::makeVelocityFromPolarNode(
						SpawnProgramUtil::makeFloatValueNode(1.5f),
						SpawnProgramUtil::makeSpiralNode(
							SpawnProgramUtil::makeIntValueNode(MAX_TICK),
							SpawnProgramUtil::makeFloatValueNode(90.0f),
							SpawnProgramUtil::makeFloatValueDiffNode({
								ANGULAR_VEL_E,
								ANGULAR_VEL_N,
								ANGULAR_VEL_H,
								ANGULAR_VEL_L
							})
						)
					),
					2,
					SpawnProgramUtil::makeSpawnPosVelNode(
						b5d7bPrototype,
						SpawnProgramUtil::makeEntityPositionNode()
					)
				)
			)
		}

		, b5d7b{ b5d7bNode, MAX_TICK, false }

		, b5d7cNode{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(5, TICK_MOD),
				SpawnProgramUtil::makeRingFormationNode(
					SpawnProgramUtil::makeVelocityFromPolarNode(
						SpawnProgramUtil::makeFloatValueNode(1.5f),
						SpawnProgramUtil::makeSpiralNode(
							SpawnProgramUtil::makeIntValueNode(MAX_TICK),
							SpawnProgramUtil::makeFloatValueNode(90.0f),
							SpawnProgramUtil::makeFloatValueDiffNode({
								-ANGULAR_VEL_E,
								-ANGULAR_VEL_N,
								-ANGULAR_VEL_H,
								-ANGULAR_VEL_L
							})
						)
					),
					2,
					SpawnProgramUtil::makeSpawnPosVelNode(
						b5d7bPrototype,
						SpawnProgramUtil::makeEntityPositionNode()
					)
				)
			)
		}

		, b5d7c{ b5d7cNode, MAX_TICK, false }

		#undef MAX_TICK
		#undef TICK_MOD
		#undef ANGULAR_VEL_E
		#undef ANGULAR_VEL_N
		#undef ANGULAR_VEL_H
		#undef ANGULAR_VEL_L
		#undef SPEED

		#define WAIT_TIME 243
		, b5d7ScriptEnd{
			ScriptProgramUtil::makeTimerNode(WAIT_TIME)
		}

		, b5d7ScriptStart{
			ScriptProgramUtil::makeAddSpawnNode(b5d7b,
			ScriptProgramUtil::makeTimerNode(WAIT_TIME,
			ScriptProgramUtil::makeAddSpawnNode(b5d7c,
			b5d7ScriptEnd
			)))
		}
		#undef WAIT_TIME

			//8

		, b5d8aPrototype{
			EntityBuilder::makePosPrototype(
				Velocity{ 0.0f, 0.0f },
				//for this danmaku, large drawn over
				BASIC_BULLET_ARGS(largeHitbox, outbound, L"large_red", 201),
				ScriptProgramList{
					ScriptProgramUtil::makeTimerNode(4,
					ScriptProgramUtil::makeRemoveEntityNode(
					))
				}
			).heapClone()
		}

		, b5d8aNode{
			SpawnProgramUtil::makeListNode(
				SpawnProgramUtil::makeSpawnAtPosNode(b5d8aPrototype),
				SpawnProgramUtil::makeRepeatNode(
					SpawnProgramUtil::makeIntValueNode(4),
					SpawnProgramUtil::makeSpawnPosVelNode(
						smallCyanPrototype,
						SpawnProgramUtil::makeEntityPositionNode(),
						SpawnProgramUtil::makeUniformRandomCircleVelocityNode(
							0.12f, 
							0.45f
						)
					)
				),
				SpawnProgramUtil::makeSpawnPosVelNode(
					mediumYellowPrototype,
					SpawnProgramUtil::makeEntityPositionNode(),
					SpawnProgramUtil::makeUniformRandomCircleVelocityNode(
						0.12f,
						0.45f
					)
				)
			)
		}
		, b5d8a{ b5d8aNode, 1, false }
		
		, b5d8bPrototype{
			EntityBuilder::makePosVelPrototype(
				SHARP_BULLET_ARGS(sharpHitbox, outbound, L"sharp_red", 100),
				ScriptProgramList{
					ScriptProgramUtil::makeStallingIfNode(
						ScriptProgramUtil::makeBoundaryXHighNode(
							config::gameWidth + config::gameOffset.x
						),
						ScriptProgramUtil::makeSetSpawnNode(b5d8a,
						ScriptProgramUtil::makeTimerNode(1,
						ScriptProgramUtil::makeRemoveEntityNode(
						)))
					),
					ScriptProgramUtil::makeStallingIfNode(
						ScriptProgramUtil::makeBoundaryXLowNode(
							config::gameOffset.x
						),
						ScriptProgramUtil::makeSetSpawnNode(b5d8a,
						ScriptProgramUtil::makeTimerNode(1,
						ScriptProgramUtil::makeRemoveEntityNode(
						)))
					),
					ScriptProgramUtil::makeStallingIfNode(
						ScriptProgramUtil::makeBoundaryYHighNode(
							config::gameHeight + config::gameOffset.y
						),
						ScriptProgramUtil::makeSetSpawnNode(b5d8a,
						ScriptProgramUtil::makeTimerNode(1,
						ScriptProgramUtil::makeRemoveEntityNode(
						)))
					),
					ScriptProgramUtil::makeStallingIfNode(
						ScriptProgramUtil::makeBoundaryYLowNode(
							config::gameOffset.y
						),
						ScriptProgramUtil::makeSetSpawnNode(b5d8a,
						ScriptProgramUtil::makeTimerNode(1,
						ScriptProgramUtil::makeRemoveEntityNode(
						)))
					)
				}
			).heapClone()
		}

		, b5d8bNode{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, { 7, 6, 5, 4 }),
				SpawnProgramUtil::makeListNode(
					SpawnProgramUtil::makeSpawnPosVelNode(
						b5d8bPrototype,
						SpawnProgramUtil::makeEntityPositionNode(),
						SpawnProgramUtil::makeVelocityFromPolarNode(
							SpawnProgramUtil::makeFloatValueNode(4.0f),
							SpawnProgramUtil::makeAngleToPlayerNode()
						)
					),
					SpawnProgramUtil::makeArcFormationNode(
						SpawnProgramUtil::makeVelocityFromPolarNode(
							SpawnProgramUtil::makeFloatValueNode(3.0f),
							SpawnProgramUtil::makeAddNode(
								SpawnProgramUtil::makeAngleToPlayerNode(),
								SpawnProgramUtil::makeFloatValueNode(180.0f)
							)
						),
						3,
						19.0f,
						SpawnProgramUtil::makeSpawnPosVelNode(
							mediumRedPrototype,
							SpawnProgramUtil::makeEntityPositionNode()
						)
					)
				)
			)
		}

		, b5d8b{ b5d8bNode, 90, false }

			//9

		, b5d9aNode{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, 7),
				SpawnProgramUtil::makeSpawnPosVelNode(
					b5d2aPrototype,
					SpawnProgramUtil::makeEntityPositionNode(),
					SpawnProgramUtil::makeVelocityFromPolarNode(
						SpawnProgramUtil::makeFloatValueNode(0.0f),
						SpawnProgramUtil::makeSpiralNode(
							SpawnProgramUtil::makeIntValueNode(0),
							SpawnProgramUtil::entityRandomAngleNode,
							SpawnProgramUtil::makeFloatValueNode(-4.1f)
						)
					)
				)
			)
		}

		, b5d9a{ b5d9aNode, 8000, false }

		, b5d9bPrototype{
			EntityBuilder::makePosVelPrototype(
				//for this danmaku, medium drawn above sharp
				BASIC_BULLET_ARGS(mediumHitbox, outbound, L"medium_yellow", 202),
				SpawnProgramList{ b5d9a },
				ScriptProgramList{
					ScriptProgramUtil::makeStallingIfNode(
						ScriptProgramUtil::makeBoundaryYHighNode(
							config::gameHeight + config::gameOffset.y
						),
						ScriptProgramUtil::makeClearSpawnNode()
					)
				}
			).heapClone()
		}

		, b5d9bNode{
			#define DIST 10.0f
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, { 240, 210, 180, 168 }),
				SpawnProgramUtil::makeListNode(
					SpawnProgramUtil::makeSpawnPosVelNode(
						b5d9bPrototype,
						SpawnProgramUtil::makePointValueNode(
							{ config::gameOffset.x + DIST, config::topOut }
						),
						SpawnProgramUtil::makeVelocityFromPolarNode(
							SpawnProgramUtil::makeFloatValueNode(1.5f),
							SpawnProgramUtil::makeAddNode(
								SpawnProgramUtil::makeFloatValueNode(-90.0f),
								SpawnProgramUtil::makeUniformRandomFloatNode(
									-2.0f, 
									2.0f
								)
							)
						)
					),
					SpawnProgramUtil::makeSpawnPosVelNode(
						b5d9bPrototype,
						SpawnProgramUtil::makePointValueNode(
							{ config::gameOffset.x + config::gameWidth - DIST, 
							config::topOut }
						),
						SpawnProgramUtil::makeVelocityFromPolarNode(
							SpawnProgramUtil::makeFloatValueNode(1.5f),
							SpawnProgramUtil::makeAddNode(
								SpawnProgramUtil::makeFloatValueNode(-90.0f),
								SpawnProgramUtil::makeUniformRandomFloatNode(
									-2.0f,
									2.0f
								)
							)
						)
					)
				)
			)
			#undef DIST
		}

		, b5d9b{ b5d9bNode, 5040, true }

		#define STORM_SPEED 0.45f
		, b5d9cNode{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, { 35, 35, 28, 28 }),
				SpawnProgramUtil::makeRingFormationNode(
					SpawnProgramUtil::makeVelocityFromPolarNode(
						SpawnProgramUtil::makeFloatValueNode(0.58f),
						SpawnProgramUtil::makeSpiralNode(
							SpawnProgramUtil::makeIntValueNode(0),
							SpawnProgramUtil::makeEntityAngleNode(),
							SpawnProgramUtil::makeFloatValueDiffNode({
								#define ANGULAR_VEL_BASE (13.0f * math::phi)
								ANGULAR_VEL_BASE / 34.0f,
								ANGULAR_VEL_BASE / 28.0f,
								ANGULAR_VEL_BASE / 24.0f,
								ANGULAR_VEL_BASE / 20.0f
								#undef ANGULAR_VEL_BASE
							})
						)
					),
					SpawnProgramUtil::makeIntValueDiffNode({3, 4, 4, 5}),
					SpawnProgramUtil::makeAddNode(
						SpawnProgramUtil::makeVelocityFromPolarNode(
							SpawnProgramUtil::makeFloatValueNode(STORM_SPEED),
							SpawnProgramUtil::makeEntityAngleNode()
						),
						SpawnProgramUtil::makeSpawnPosVelNode(
							smallCyanPrototype,
							SpawnProgramUtil::makeEntityPositionNode()
						)
					)
				)
			)
		}

		, b5d9c{ b5d9cNode, 140, false }

		, b5d9dPrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(largeHitbox, outbound, L"large_blue", -200),
				SpawnProgramList{ b5d9c }
			).heapClone()
		}

		, b5d9dNode{
			SpawnProgramUtil::makeRingFormationNode(
				SpawnProgramUtil::makeVelocityFromPolarNode(
					SpawnProgramUtil::makeFloatValueNode(STORM_SPEED),
					SpawnProgramUtil::randomAngleNode
				),
				6,
				SpawnProgramUtil::makeSpawnPosVelNode(
					b5d9dPrototype,
					SpawnProgramUtil::makeEntityPositionNode()
				)
			)
		}

		, b5d9d{ b5d9dNode, 1, false }

		, b5d9eNode{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(35, 70),
				SpawnProgramUtil::makeRingFormationNode(
					SpawnProgramUtil::makeVelocityFromPolarNode(
						SpawnProgramUtil::makeFloatValueNode(0.78f),
						SpawnProgramUtil::makeSpiralNode(0, 122.0f, 3.9f)
					),
					SpawnProgramUtil::makeIntValueDiffNode({ 8, 12, 16, 20 }),
					SpawnProgramUtil::makeAddNode(
						SpawnProgramUtil::makeVelocityFromPolarNode(
							SpawnProgramUtil::makeFloatValueNode(STORM_SPEED),
							SpawnProgramUtil::makeEntityAngleNode()
						),
						SpawnProgramUtil::makeSpawnPosVelNode(
							smallBluePrototype,
							SpawnProgramUtil::makeEntityPositionNode()
						)
					)
				)
			)
		}

		, b5d9e{ b5d9eNode, 140, false }

		, b5d9fPrototype{
			EntityBuilder::makePosVelPrototype(
				BASIC_BULLET_ARGS(largeHitbox, outbound, L"large_cyan", -200),
				SpawnProgramList{ b5d9e }
			).heapClone()
		}

		, b5d9fNode{
			SpawnProgramUtil::makeRingFormationNode(
				SpawnProgramUtil::makeVelocityFromPolarNode(
					SpawnProgramUtil::makeFloatValueNode(STORM_SPEED),
					SpawnProgramUtil::randomAngleNode
				),
				6,
				SpawnProgramUtil::makeSpawnPosVelNode(
					b5d9fPrototype,
					SpawnProgramUtil::makeEntityPositionNode()
				)
			)
		}

		, b5d9f{ b5d9fNode, 1, false }

		#undef STORM_SPEED

		#define WAIT_TIME 150
		, b5d9ActScriptEnd{
			ScriptProgramUtil::makeTimerNode(WAIT_TIME)
		}
		, b5d9ActScriptStart{
			ScriptProgramUtil::makeAddSpawnNode(b5d9d,
			ScriptProgramUtil::makeTimerNode(WAIT_TIME,
			ScriptProgramUtil::makeAddSpawnNode(b5d9f,
			b5d9ActScriptEnd
			)))
		}
		#undef WAIT_TIME
			//the boss

		, b5e1Prototype{
			EntityBuilder::makeLinearUncollidable(
				BOSS_ARGS,
				SpriteInstruction{
					bitmapStoragePointer->get(L"b5_idle_1")->d2dBitmap,
					math::Vector2{ 0.0f, 4.0f }			//offset
				},
				DrawOrder{ config::playerDrawOrder },
				AnimationList{ 
					{
						components::Animation{ {
							L"b5_idle_1", L"b5_idle_2", L"b5_idle_3", L"b5_idle_4"
						} }
					},
					0,	//idle index
					4	//ticks
				},
				ScriptProgramList{
					ScriptProgramUtil::makeBossEntryNode(120, L"5_1",
					ScriptProgramUtil::makeTimerNode(120,
					ScriptProgramUtil::makeSetHealthNode(8500,
					ScriptProgramUtil::makeBossAttackAndMoveNode(
						b5d1,
						410,
						30,
					ScriptProgramUtil::makeBossResetNode(90,
					ScriptProgramUtil::makeSetHealthNode(7600,
					ScriptProgramUtil::makeBossAttackAndMoveNode(
						b5d2b,
						140,
						20,
					ScriptProgramUtil::makeAddSpawnNode(
						pickupProgramsPointer->largePowerSpawnProgram,
					ScriptProgramUtil::makeTimerNode(1,
					ScriptProgramUtil::makeBossResetMidNode(90,
					ScriptProgramUtil::makeSetHealthNode(6700,
					ScriptProgramUtil::makeSetSpawnNode(b5d3c,		//3
					ScriptProgramUtil::makeBossMoveNode(
						90,
						90,
					ScriptProgramUtil::makeBossResetNode(90,
					ScriptProgramUtil::makeSetHealthNode(7000,
					ScriptProgramUtil::makeBossAttackAndMoveNode(
						b5d4c,
						170,
						30,
					ScriptProgramUtil::makeAddSpawnNode(
						pickupProgramsPointer->largePowerSpawnProgram,
					ScriptProgramUtil::makeTimerNode(1,
					ScriptProgramUtil::makeBossResetMidNode(90,
					ScriptProgramUtil::makeSetHealthNode(7500,
					ScriptProgramUtil::makeBossAttackAndMoveNode(
						b5d5d,
						530,
						30,
					ScriptProgramUtil::makeBossResetNode(90,
					ScriptProgramUtil::makeSetHealthNode(3300,
					ScriptProgramUtil::makeBossAttackAndMoveNode(
						b5d6,
						580,
						30,
					ScriptProgramUtil::makeAddSpawnNode(
						pickupProgramsPointer->largePowerSpawnProgram,
					ScriptProgramUtil::makeTimerNode(1,
					ScriptProgramUtil::makeBossResetMidNode(90,
					ScriptProgramUtil::makeSetHealthNode(5500,
					ScriptProgramUtil::makeBossCircular(
						b5d7ScriptStart,
						b5d7ScriptEnd,
					ScriptProgramUtil::makeBossResetNode(90,
					ScriptProgramUtil::makeSetHealthNode(5000,
					ScriptProgramUtil::makeBossAttackAndMoveNode(
						b5d8b,
						130,
						20,
					ScriptProgramUtil::makeAddSpawnNode(
						pickupProgramsPointer->largePowerSpawnProgram,
					ScriptProgramUtil::makeTimerNode(1,
					ScriptProgramUtil::makeBossResetMidNode(90,
					ScriptProgramUtil::makeSetHealthNode(17000,
					ScriptProgramUtil::makeAddSpawnNode(b5d9b,
					ScriptProgramUtil::makeBossActAndMoveNode(
						55,
						20,
						b5d9ActScriptStart,
						b5d9ActScriptEnd,
					ScriptProgramUtil::makeBossEndNode()
					))))))))))))))))))))))))))))))))))))))
				}
			).heapClone()
		}
		, b5e1Node{
			SpawnProgramUtil::makeSpawnNode(b5e1Prototype)
		}
		, b5e1{ b5e1Node, 1, false }

		, stage5ScriptProgram{
			ScriptProgramUtil::makeTimerNode(190,
			ScriptProgramUtil::makeSetSpawnNode(s5e1,
			ScriptProgramUtil::makeTimerNode(340,
			ScriptProgramUtil::makeAddSpawnNode(s5e2,
			ScriptProgramUtil::makeTimerNode(340,
			ScriptProgramUtil::makeAddSpawnNode(s5e3,
			ScriptProgramUtil::makeTimerNode(340,
			ScriptProgramUtil::makeAddSpawnNode(s5e4,
			ScriptProgramUtil::makeTimerNode(240,
			ScriptProgramUtil::makeAddSpawnNode(s5e5,
			ScriptProgramUtil::makeTimerNode(1220,
			ScriptProgramUtil::makeAddSpawnNode(b5e1,
			ScriptProgramUtil::makeStallingIfNode(
				ScriptProgramUtil::makeIsWinNode(),
				ScriptProgramUtil::makeTimerNode(waitEndTime,
				ScriptProgramUtil::makeEndStageNode()
			))
			))))))))))))
			
		}

		{
		}
}