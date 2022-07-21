#include "Game/Systems/Programs/EnemyPrograms.h"

namespace wasp::game::systems {

	namespace {
		constexpr float smallHitbox{ 3.0f };
		constexpr float mediumHitbox{ 6.25f };
		constexpr float largeHitbox{ 12.0f };
		constexpr float sharpHitbox{ 4.0f };
		constexpr float outbound{ -21.0f };

		constexpr float enemyOut{ -30.0f };

		constexpr int fieldClearLifetime{ 30 };

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
		Hitbox{ 10.0f }, \
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

	//remember to use makeLinearUncollidable()
	#define BOSS_ARGS \
		Position{ (config::gameWidth / 2.0f) + config::gameOffset.x, config::topOut }, \
		Velocity{ math::Vector2{ 0.0f, config::bossSpeed } }, \
		Hitbox{ config::bossHitbox }, \
		PlayerCollisions::Source{}, \
		EnemyCollisions::Target{ components::CollisionCommands::damage }, \
		Health{ 10000 }, \
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
					SpawnProgramUtil::makeIntValueDiffNode({ 1, 2, 3, 4 }),
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
				WISP_ARGS(10000, pickupProgramsPointer->largePowerSpawnProgram),
				ScriptProgramList{
					ScriptProgramUtil::makeTurnEntryAndShootProgram(
						20,
						45,
						{ 0.5f, 192.0f},
						-135.0f,
						s1d8,
						ScriptProgramUtil::makeSetHealthNode(
							400,
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
					SpawnProgramUtil::makeSpawnPosVelNode(
						largeAzurePrototype,
						SpawnProgramUtil::makeEntityPositionNode(),
						SpawnProgramUtil::makeVelocityFromPolarNode(
							SpawnProgramUtil::makeFloatValueNode(0.8f),
							SpawnProgramUtil::makeAngleToPlayerNode()
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
					bitmapStoragePointer->get(L"p_idle_1")->d2dBitmap,
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
					ScriptProgramUtil::makeBossEntryNode(120, L"fake dialogue",
					ScriptProgramUtil::makeSetHealthNode(2000,
					ScriptProgramUtil::makeSetSpawnNode(b1d1,
					ScriptProgramUtil::makeBossMoveNode(90, 90,
					ScriptProgramUtil::makeBossResetNode(90,
					ScriptProgramUtil::makeSetHealthNode(2200,
					ScriptProgramUtil::makeAddSpawnNode(b1d2,
					ScriptProgramUtil::makeBossMoveNode(110, 110,
					ScriptProgramUtil::makeBossResetNode(90,
					ScriptProgramUtil::makeSetHealthNode(2000,
					ScriptProgramUtil::makeAddSpawnNode(b1d3,
					ScriptProgramUtil::makeBossMoveNode(90, 90,
					ScriptProgramUtil::makeBossResetMidNode(90,
					ScriptProgramUtil::makeSetHealthNode(2300,
					ScriptProgramUtil::makeAddSpawnNode(b1d4))
)
					))))))))))))
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
			ScriptProgramUtil::makeTimerNode(1000,
			ScriptProgramUtil::makeRemoveEntityNode(
			))))))))))))))))))))))))))))))))))))))))))
		}

		// STAGE 2 // STAGE 2 // STAGE 2 // STAGE 2 // STAGE 2 // STAGE 2 // STAGE 2 //

			//wave 1
		, s2d1Node{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, { 1, 2, 3, 4 }),
				SpawnProgramUtil::makeRingFormationNode(
					SpawnProgramUtil::makeVelocityFromPolarNode(
						SpawnProgramUtil::makeWhipNode(42, 0.4f, 3.5f),
						SpawnProgramUtil::makeAngleToPlayerNode()
					),
					{ 14, 20, 26, 30 },
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
		, s2d3Node{
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
						SpawnProgramUtil::makeFloatValueNode(0.9f),
						SpawnProgramUtil::makeAngleToPlayerNode()
					),
					SpawnProgramUtil::makeIntValueDiffNode({ 3, 3, 5, 5 }),
					SpawnProgramUtil::makeFloatValueNode(12.0f),
					SpawnProgramUtil::makeSpawnPosVelNode(
						smallBluePrototype,
						SpawnProgramUtil::makeEntityPositionNode()
					)
				)
			)
		}
		, s2d3{ s2d3Node, 1000, true }

		, s2e3Prototype{
			EntityBuilder::makePosVelPrototype(
				FLAME_ARGS(30, pickupProgramsPointer->smallPowerThirdSpawnProgram),
				SpawnProgramList{ s2d3 }
			).heapClone()
		}
		, s2e3Node{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, 14),
				SpawnProgramUtil::makeSpawnPosVelNode(
					s2e3Prototype,
					SpawnProgramUtil::makePointValueNode({ config::rightOut, 35.0f }),
					SpawnProgramUtil::makeVelocityValueNode(Velocity{ 1.8f, 188.0f })
				)
			)
		}
		, s2e3{ s2e3Node, 290, false }



		, stage2ScriptProgram{
			ScriptProgramUtil::makeTimerNode(290,
			ScriptProgramUtil::makeSetSpawnNode(s2e1,
			ScriptProgramUtil::makeTimerNode(220,
			ScriptProgramUtil::makeAddSpawnNode(s2e2,
			ScriptProgramUtil::makeTimerNode(250,
			ScriptProgramUtil::makeAddSpawnNode(s2e3,

			ScriptProgramUtil::makeTimerNode(1000,
			ScriptProgramUtil::makeRemoveEntityNode(
			))))))))
		}



		{
		}
}