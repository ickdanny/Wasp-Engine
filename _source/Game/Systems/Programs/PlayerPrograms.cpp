#include "Game/Systems/Programs/PlayerPrograms.h"

namespace wasp::game::systems {

	using namespace math;
	using namespace components;

	using ScriptProgramList = game::ScriptProgramList;

	namespace a {
		constexpr float opacity{ 0.8f };

		//small bubble data
		constexpr float smallSpeed{ 7.0f };
		constexpr AABB smallHitbox{ 8.0f };
		constexpr int smallDamage{ 10 };
		constexpr float smallOutbound{ -15.0f };

		constexpr float smallSpacing{ 7.0f };

		constexpr int smallMod0{ 8 };
		constexpr int smallMod1{ 5 };

		//large bubble data
		constexpr float largeSpeed{ smallSpeed * 0.8f };
		constexpr float largeHitbox{ 14.0f };
		constexpr int largeDamage{ 25 };
		constexpr float largeOutbound{ -30.0f };

		constexpr Vector2 largeOffset{ 10.0f, 0.0f };

		constexpr Angle unfocusedLargeAngle{ 83.0f };
		constexpr Angle focusedLargeAngle{ 88.0f };

		constexpr int largeMod3{ config::playerShotMaxTick };
		constexpr int largeMod4{ config::playerShotMaxTick / 2 };
		constexpr int largeMod5{ config::playerShotMaxTick / 4 };

		//bomb data
		constexpr float bombHitbox{ 17.0f };
		constexpr int bombDamage{ 100 };
		constexpr float bombOutbound{ -50.0f };

		constexpr int bombMod{ 5 };
		constexpr int bombModAdd{ 2 };

		constexpr float bombMaxSpawnOffsetRadius{ 2.0f };

		constexpr int bombWaitPeriod{ 20 };
		constexpr int bombSpeedPeriod{ 120 };
		constexpr float bombMaxSpeed{ 4.0f };
		constexpr float bombAngleSpread{ 3.0f };

		constexpr float bombGhostSpeed{ 3.0f };
	}

	namespace b {
		constexpr float opacity{ 0.8f };

		//ice shard data
		constexpr float speed{ 8.5f };
		constexpr AABB hitbox{ 11.5f };
		constexpr int damage{ 10 };
		constexpr float outbound{ -60.0f };		//reason this is high is for preview rofl
		
		constexpr int mod0{ 8 };
		constexpr int mod1{ 5 };

		constexpr float unfocusedAngleIncrement{ 7.0f };
		constexpr float focusedAngleIncrement{ 4.0f };

		//bomb data
		constexpr float bombHitbox{ 17.0f };
		constexpr int bombDamage{ 2 };
		constexpr float bombOutbound{ -50.0f };

		constexpr float bombSpeed{ 1.0f };

		constexpr float bombSpin{ 1.0f };
		
		constexpr int bombSymmetry{ 18 };
	}

	namespace death {
		constexpr int clearLifetime{ 60 };

		constexpr int numSmallPickups{ 2 };
		constexpr float pickupInbound{ 15.0f };
		constexpr float pickupYHigh{ 50.0f };
		constexpr int pickupAirTime{ 50 };

		constexpr float velocityScale{ 1.0f / pickupAirTime };
	}

	PlayerPrograms::PlayerPrograms(
		resources::BitmapStorage* bitmapStoragePointer
	)
		: bitmapStoragePointer{ bitmapStoragePointer }

		//A shot
		, smallBubblePrototype{
			EntityBuilder::makePosPrototype(
				Velocity{ Vector2{ 0.0f, -a::smallSpeed } },
				a::smallHitbox,
				EnemyCollisions::Source{ components::CollisionCommands::death },
				Damage{ a::smallDamage },
				Outbound{ a::smallOutbound },
				SpriteInstruction{
					bitmapStoragePointer->get(L"bubble_small")->d2dBitmap,
					{},			//offset
					{0.0f},		//rotation
					a::opacity
				},
				DrawOrder{ config::playerBulletDrawOrder }
			).heapClone()
		}
		, largeBubblePrototype{
			EntityBuilder::makePosVelPrototype(
				a::largeHitbox,
				EnemyCollisions::Source{ components::CollisionCommands::death },
				Damage{ a::largeDamage },
				Outbound{ a::largeOutbound },
				SpriteInstruction{
					bitmapStoragePointer->get(L"bubble_large")->d2dBitmap,
					{},			//offset
					{0.0f},		//rotation
					a::opacity
				},
				DrawOrder{ config::playerBulletDrawOrder + 1 }
			).heapClone()
		}
		, spawnSingleSmallBubbleNode{
			SpawnProgramUtil::makeSpawnAtPosNode(smallBubblePrototype)
		}
		, spawnDoubleSmallBubbleNode{
			SpawnProgramUtil::makeMirrorPosFormationNode(
				SpawnProgramUtil::makeEntityOffsetNode(
					SpawnProgramUtil::makeVelocityValueNode(
						math::Vector2{ a::smallSpacing / 2.0f, 0.0f }
					)
				),
				//pass this since linked node is a non-factor
				spawnSingleSmallBubbleNode	
			)
		}
		, spawnTripleSmallBubbleNode{
			SpawnProgramUtil::makeListNode(
				spawnSingleSmallBubbleNode,
				SpawnProgramUtil::makeMirrorPosFormationNode(
					SpawnProgramUtil::makeEntityOffsetNode(
						SpawnProgramUtil::makeVelocityValueNode(
							math::Vector2{ a::smallSpacing, 0.0f }
						)
					),
					//pass this since linked node is a non-factor
					spawnSingleSmallBubbleNode
				)
			)
		}
		, spawnLargeBubbleNode{
			SpawnProgramUtil::makeMirrorFormationNode(
				//pos
				SpawnProgramUtil::makeEntityOffsetNode(
					SpawnProgramUtil::makeVelocityValueNode(a::largeOffset)
				),
				//vel
				SpawnProgramUtil::makeIfElseNode(
					SpawnInstructions::isPlayerFocused,
					SpawnProgramUtil::makeVelocityValueNode(
						Velocity{ a::largeSpeed, a::focusedLargeAngle }
					),
					SpawnProgramUtil::makeVelocityValueNode(
						Velocity{ a::largeSpeed, a::unfocusedLargeAngle }
					)
				),
				//consumer
				SpawnProgramUtil::makeSpawnPosVelNode(largeBubblePrototype)
			)
		}
		, shotAPowerBucket0Node{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, a::smallMod0),
				spawnSingleSmallBubbleNode
			)
		}
		, shotAPowerBucket1Node{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, a::smallMod1),
				spawnSingleSmallBubbleNode
			)
		}
		, shotAPowerBucket2Node{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, a::smallMod1),
				spawnDoubleSmallBubbleNode
			)
		}
		, shotAPowerBucket3Node{
			SpawnProgramUtil::makeListNode(
				shotAPowerBucket2Node,
				SpawnProgramUtil::makeIfNode(
					SpawnProgramUtil::makeTickModNode(0, a::largeMod3),
					spawnLargeBubbleNode
				)
			)
		}
		, shotAPowerBucket4Node{
			SpawnProgramUtil::makeListNode(
				shotAPowerBucket2Node,
				SpawnProgramUtil::makeIfNode(
					SpawnProgramUtil::makeTickModNode(0, a::largeMod4),
					spawnLargeBubbleNode
				)
			)
		}
		, shotAPowerBucket5Node{
			SpawnProgramUtil::makeListNode(
				shotAPowerBucket2Node,
				SpawnProgramUtil::makeIfNode(
					SpawnProgramUtil::makeTickModNode(0, a::largeMod5),
					spawnLargeBubbleNode
				)
			)
		}
		, shotAPowerBucket6Node{
			SpawnProgramUtil::makeListNode(
				SpawnProgramUtil::makeIfNode(
					SpawnProgramUtil::makeTickModNode(0, a::smallMod1),
					spawnTripleSmallBubbleNode
				),
				SpawnProgramUtil::makeIfNode(
					SpawnProgramUtil::makeTickModNode(0, a::largeMod5),
					spawnLargeBubbleNode
				)
			)
		}
		, shotANode{
			&(new SpawnNode{SpawnInstructions::playerPowerSplit})
			->link(
				shotAPowerBucket0Node,
				shotAPowerBucket1Node,
				shotAPowerBucket2Node,
				shotAPowerBucket3Node,
				shotAPowerBucket4Node,
				shotAPowerBucket5Node,
				shotAPowerBucket6Node
			)
		}
		, shotASpawnProgram{ shotANode, config::playerShotMaxTick, false }

		//A bomb
		, bombBubbleScriptNode{
			ScriptProgramUtil::makeTimerNode(
				a::bombWaitPeriod,
				ScriptProgramUtil::makeShiftSpeedPeriodNode(
					a::bombMaxSpeed,
					a::bombSpeedPeriod
				)
			)
		}
		, bombBubblePrototype{
			EntityBuilder::makePosVelPrototype(
				a::bombHitbox,
				EnemyCollisions::Source{ components::CollisionCommands::death },
				BulletCollisions::Source{},
				Damage{ a::bombDamage },
				Outbound{ a::bombOutbound },
				SpriteInstruction{
					bitmapStoragePointer->get(L"bubble_bomb")->d2dBitmap,
					{},			//offset
					{0.0f},		//rotation
					a::opacity
				},
				ScriptProgramList{ 
					ScriptProgram{ bombBubbleScriptNode} 
				},
				DrawOrder{ config::playerBulletDrawOrder - 10 }
			).heapClone()
		}
		, spawnSingleBombBubbleNode{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(a::bombModAdd, a::bombMod),
				SpawnProgramUtil::makeSpawnPosVelNode(
					bombBubblePrototype,
					SpawnProgramUtil::makeEntityOffsetNode(
						SpawnProgramUtil::makeUniformRandomCircleVelocityNode(
							0,
							a::bombMaxSpawnOffsetRadius
						)
					),
					SpawnProgramUtil::makeVelocityFromPolarNode(
						SpawnProgramUtil::makeFloatValueNode(0.0f),
						SpawnProgramUtil::makeUniformRandomFloatNode(
							SpawnProgramUtil::makeFloatValueNode(
								90.0f - a::bombAngleSpread
							),
							SpawnProgramUtil::makeFloatValueNode(
								90.0f + a::bombAngleSpread
							)
						)
					)
				)
			)
		}
		, bombGhostSpawnProgram{
			spawnSingleBombBubbleNode,
			a::bombMod,
			true
		}
		, bombGhostPrototype{
			EntityBuilder::makeEntity(
				Outbound{ a::bombOutbound },
				game::SpawnProgramList{
					bombGhostSpawnProgram
				}
			).heapClone()
		}
		, bombANode{
			SpawnProgramUtil::makeMirrorFormationNode(
				SpawnProgramUtil::makeEntityPositionNode(),
				SpawnProgramUtil::makeVelocityValueNode(
					Velocity{a::bombGhostSpeed, Angle(0)}
				),
				SpawnProgramUtil::makeSpawnPosVelNode(bombGhostPrototype)
			)
		}
		, bombASpawnProgram{ bombANode, 1, false }

		//B shot
		, iceShardPrototype{
			EntityBuilder::makePosVelPrototype(
				b::hitbox,
				EnemyCollisions::Source{ components::CollisionCommands::death },
				Damage{ b::damage },
				Outbound{ b::outbound },
				SpriteInstruction{
					bitmapStoragePointer->get(L"ice")->d2dBitmap,
					{},			//offset
					{0.0f},		//rotation
					b::opacity
				},
				DrawOrder{ config::playerBulletDrawOrder },
				RotateSpriteForwardMarker{}
			).heapClone()
		}
		, spawnSingleIceShardNode{
			SpawnProgramUtil::makeSpawnPosVelNode(
				iceShardPrototype,
				SpawnProgramUtil::makeEntityPositionNode(),
				SpawnProgramUtil::makeVelocityValueNode(
					Vector2{ 0.0f, -b::speed }
				)
			)
		}
		, shotBPowerBucket0Node{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, b::mod0),
				spawnSingleIceShardNode
			)
		}
		, shotBPowerBucket1Node{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, b::mod1),
				spawnSingleIceShardNode
			)
		}
		, shotBPowerBucket2Node{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, b::mod1),
				SpawnProgramUtil::makeArcFormationNode(
					SpawnProgramUtil::makeVelocityValueNode(	//baseVel
						Vector2{ 0.0f, -b::speed }
					),
					SpawnProgramUtil::makeIntValueNode(2),		//symmetry
					SpawnProgramUtil::makeIfElseNode(				//angleIncrement
						SpawnInstructions::isPlayerFocused,
						SpawnProgramUtil::makeFloatValueNode(
							b::focusedAngleIncrement
						),
						SpawnProgramUtil::makeFloatValueNode(
							b::unfocusedAngleIncrement
						)
					),
					//passing a vel will still use our position node
					spawnSingleIceShardNode							//velConsumer
				)
			)
		}
		, shotBPowerBucket3Node{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, b::mod1),
				SpawnProgramUtil::makeArcFormationNode(
					SpawnProgramUtil::makeVelocityValueNode(	//baseVel
						Vector2{ 0.0f, -b::speed }
					),
					SpawnProgramUtil::makeIntValueNode(3),		//symmetry
					SpawnProgramUtil::makeIfElseNode(				//angleIncrement
						SpawnInstructions::isPlayerFocused,
						SpawnProgramUtil::makeFloatValueNode(
							b::focusedAngleIncrement
						),
						SpawnProgramUtil::makeFloatValueNode(
							b::unfocusedAngleIncrement
						)
					),
					//passing a vel will still use our position node
					spawnSingleIceShardNode							//velConsumer
				)
			)
		}
		, shotBPowerBucket4Node{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, b::mod1),
				SpawnProgramUtil::makeArcFormationNode(
					SpawnProgramUtil::makeVelocityValueNode(	//baseVel
						Vector2{ 0.0f, -b::speed }
					),
					SpawnProgramUtil::makeIntValueNode(4),		//symmetry
					SpawnProgramUtil::makeIfElseNode(				//angleIncrement
						SpawnInstructions::isPlayerFocused,
						SpawnProgramUtil::makeFloatValueNode(
							b::focusedAngleIncrement
						),
						SpawnProgramUtil::makeFloatValueNode(
							b::unfocusedAngleIncrement
						)
					),
					//passing a vel will still use our position node
					spawnSingleIceShardNode							//velConsumer
				)
			)
		}
		, shotBPowerBucket5Node{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, b::mod1),
				SpawnProgramUtil::makeArcFormationNode(
					SpawnProgramUtil::makeVelocityValueNode(	//baseVel
						Vector2{ 0.0f, -b::speed }
					),
					SpawnProgramUtil::makeIntValueNode(5),		//symmetry
					SpawnProgramUtil::makeIfElseNode(				//angleIncrement
						SpawnInstructions::isPlayerFocused,
						SpawnProgramUtil::makeFloatValueNode(
							b::focusedAngleIncrement
						),
						SpawnProgramUtil::makeFloatValueNode(
							b::unfocusedAngleIncrement
						)
					),
					//passing a vel will still use our position node
					spawnSingleIceShardNode							//velConsumer
				)
			)
		}
		, shotBPowerBucket6Node{
			SpawnProgramUtil::makeIfNode(
				SpawnProgramUtil::makeTickModNode(0, b::mod1),
				SpawnProgramUtil::makeArcFormationNode(
					SpawnProgramUtil::makeVelocityValueNode(	//baseVel
						Vector2{ 0.0f, -b::speed }
					),
					SpawnProgramUtil::makeIntValueNode(6),		//symmetry
					SpawnProgramUtil::makeIfElseNode(				//angleIncrement
						SpawnInstructions::isPlayerFocused,
						SpawnProgramUtil::makeFloatValueNode(
							b::focusedAngleIncrement
						),
						SpawnProgramUtil::makeFloatValueNode(
							b::unfocusedAngleIncrement
						)
					),
					//passing a vel will still use our position node
					spawnSingleIceShardNode							//velConsumer
				)
			)
		}
		, shotBNode{
			&(new SpawnNode{ SpawnInstructions::playerPowerSplit })
			->link(
				shotBPowerBucket0Node,
				shotBPowerBucket1Node,
				shotBPowerBucket2Node,
				shotBPowerBucket3Node,
				shotBPowerBucket4Node,
				shotBPowerBucket5Node,
				shotBPowerBucket6Node
			)
		}
		, shotBSpawnProgram{ shotBNode, config::playerShotMaxTick, false }

		//B bomb
		, snowflakePrototype{
			EntityBuilder::makePosVelPrototype(
				b::bombHitbox,
				EnemyCollisions::Source{},
				BulletCollisions::Source{},
				Damage{ b::bombDamage },
				Outbound{ b::bombOutbound },
				SpriteInstruction{
					bitmapStoragePointer->get(L"snowflake")->d2dBitmap,
					{},			//offset
					{0.0f},		//rotation
					b::opacity
				},
				SpriteSpin{ b::bombSpin },
				DrawOrder{ config::playerBulletDrawOrder - 10 }
			).heapClone()
		}
		, bombBNode{
			SpawnProgramUtil::makeRingFormationNode(
				Vector2{ 0.0f, b::bombSpeed },
				b::bombSymmetry,
				SpawnProgramUtil::makeSpawnPosVelNode(
					snowflakePrototype,
					SpawnProgramUtil::makeEntityPositionNode()
				)
			)
		}
		, bombBSpawnProgram{ bombBNode, 1, false }
		
		//death
		, clearScriptNode{
			ScriptProgramUtil::makeTimerNode(
				death::clearLifetime,
				ScriptProgramUtil::makeRemoveEntityNode()
			)
		}
		, clearPrototype{
			EntityBuilder::makeStationaryCollidable(
				Point2{ 
					config::gameWidth / 2, 
					config::gameHeight / 2 
				} + config::gameOffset,
				AABB{ config::gameWidth / 2, config::gameHeight / 2},
				PlayerCollisions::Target{ components::CollisionCommands::player },
				ScriptProgramList{ clearScriptNode }
			).heapClone()
		}
		, spawnClearNode{
			SpawnProgramUtil::makeSpawnNode(clearPrototype)
		}
		, powerScriptNode{
			ScriptProgramUtil::makeTimerNode(
				death::pickupAirTime,
				ScriptProgramUtil::makeSetVelocityNode(
					Velocity{ config::pickupFinalSpeed, Angle{ 90 } }
				)
			)
		}
		, smallPowerPrototype{ 
			EntityBuilder::makePosVelPrototype(
				config::smallPickupHitbox,
				PickupCollisions::Source{ components::CollisionCommands::pickup },
				game::PickupType{ components::PickupType::Types::powerSmall },
				Outbound{ config::pickupOutbound },
				SpriteInstruction{
					bitmapStoragePointer->get(L"power_small")->d2dBitmap,
				},
				ScriptProgramList{ powerScriptNode },
				DrawOrder{ config::pickupDrawOrder }
			).heapClone()
		}
		, largePowerPrototype{ 
			EntityBuilder::makePosVelPrototype(
				config::largePickupHitbox,
				PickupCollisions::Source{ components::CollisionCommands::pickup },
				game::PickupType{ components::PickupType::Types::powerLarge },
				Outbound{ config::pickupOutbound },
				SpriteInstruction{
					bitmapStoragePointer->get(L"power_large")->d2dBitmap,
				},
				ScriptProgramList{ powerScriptNode },
				DrawOrder{ config::pickupDrawOrder + 1 }
			).heapClone() 
		}
		, spawnPowerNode{
			SpawnProgramUtil::makeListNode(
				SpawnProgramUtil::makeSpawnPosVelNode(
					largePowerPrototype,
					SpawnProgramUtil::makeEntityPositionNode(),
					SpawnProgramUtil::makeMultiplyNode(
						SpawnProgramUtil::makeVelocityToPointNode(
							SpawnProgramUtil::makeEntityPositionNode(),
							SpawnProgramUtil::makeUniformRandomRectanglePointNode(
								death::pickupInbound + config::gameOffset.x,
								config::gameWidth - death::pickupInbound 
									+ config::gameOffset.x,
								death::pickupInbound + config::gameOffset.y,
								death::pickupYHigh + config::gameOffset.y
							)
						),
						SpawnProgramUtil::makeFloatValueNode(death::velocityScale)
					)
				),
				SpawnProgramUtil::makeRepeatNode(
					death::numSmallPickups,
					SpawnProgramUtil::makeSpawnPosVelNode(
						smallPowerPrototype,
						SpawnProgramUtil::makeEntityPositionNode(),
						SpawnProgramUtil::makeMultiplyNode(
							SpawnProgramUtil::makeVelocityToPointNode(
								SpawnProgramUtil::makeEntityPositionNode(),
								SpawnProgramUtil::makeUniformRandomRectanglePointNode(
									death::pickupInbound + config::gameOffset.x,
									config::gameWidth - death::pickupInbound
										+ config::gameOffset.x,
									death::pickupInbound + config::gameOffset.y,
									death::pickupYHigh + config::gameOffset.y
								)
							),
							SpawnProgramUtil::makeFloatValueNode(death::velocityScale)
						)
					)
				)
			)
		}
		, deathSpawnNode{
			SpawnProgramUtil::makeListNode(spawnClearNode, spawnPowerNode)
		}
		, deathSpawnProgram{ deathSpawnNode, 1, false }

		//shot menu preview
		, shotAPreviewProgram{
			shotAPowerBucket6Node, config::playerShotMaxTick, true
		}

		, shotBPreviewProgram{
			shotBPowerBucket6Node, config::playerShotMaxTick, true
		}

		{
		}
}