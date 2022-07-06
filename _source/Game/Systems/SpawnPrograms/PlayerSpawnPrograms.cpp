#include "Game/Systems/SpawnPrograms/PlayerSpawnPrograms.h"

namespace wasp::game::systems {

	using namespace math;
	using namespace components;

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
		constexpr int largeDamage{ 20 };
		constexpr float largeOutbound{ -30.0f };

		constexpr Vector2 largeOffset{ 10.0f, 0.0f };

		constexpr Angle unfocusedLargeAngle{ 83.0f };
		constexpr Angle focusedLargeAngle{ 88.0f };

		constexpr int largeMod3{ config::playerShotMaxTick };
		constexpr int largeMod4{ config::playerShotMaxTick / 2 };
		constexpr int largeMod5{ config::playerShotMaxTick / 4 };

		//bomb data
		constexpr float bombHitbox{ 17.0f };
		constexpr int bombDamage{ 1 };
		constexpr float bombOutbound{ -50.0f };
	}

	namespace b {
		constexpr float opacity{ 0.8f };

		//ice shard data
		constexpr float speed{ 8.5f };
		constexpr AABB hitbox{ 11.5f };
		constexpr int damage{ 10 };
		constexpr float outbound{ -20.0f };
		
		constexpr int mod0{ 8 };
		constexpr int mod1{ 5 };

		constexpr float unfocusedAngleIncrement{ 7.0f };
		constexpr float focusedAngleIncrement{ 4.0f };
	}

	PlayerSpawnPrograms::PlayerSpawnPrograms(
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
					SpawnProgramUtil::makeVelocityValueSpawnNode(
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
						SpawnProgramUtil::makeVelocityValueSpawnNode(
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
					SpawnProgramUtil::makeVelocityValueSpawnNode(a::largeOffset)
				),
				//vel
				SpawnProgramUtil::makeIfElseNode(
					SpawnInstructions::isPlayerFocused,
					SpawnProgramUtil::makeVelocityValueSpawnNode(
						Velocity{ a::largeSpeed, a::focusedLargeAngle }
					),
					SpawnProgramUtil::makeVelocityValueSpawnNode(
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
		, shotA{
			shotANode,
			config::playerShotMaxTick,
			false
		}

		//A bomb
		, bombBubblePrototype{
			EntityBuilder::makePosVelPrototype(
				a::bombHitbox,
				EnemyCollisions::Source{},
				Damage{ a::bombDamage },
				Outbound{ a::bombOutbound },
				SpriteInstruction{
					bitmapStoragePointer->get(L"bubble_bomb")->d2dBitmap,
					{},			//offset
					{0.0f},		//rotation
					a::opacity
				},
				DrawOrder{ config::playerBulletDrawOrder - 10 }
			).heapClone()
		}

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
				SpawnProgramUtil::makeVelocityValueSpawnNode(
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
					SpawnProgramUtil::makeVelocityValueSpawnNode(	//baseVel
						Vector2{ 0.0f, -b::speed }
					),
					SpawnProgramUtil::makeIntValueSpawnNode(2),		//symmetry
					SpawnProgramUtil::makeIfElseNode(				//angleIncrement
						SpawnInstructions::isPlayerFocused,
						SpawnProgramUtil::makeFloatValueSpawnNode(
							b::focusedAngleIncrement
						),
						SpawnProgramUtil::makeFloatValueSpawnNode(
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
					SpawnProgramUtil::makeVelocityValueSpawnNode(	//baseVel
						Vector2{ 0.0f, -b::speed }
					),
					SpawnProgramUtil::makeIntValueSpawnNode(3),		//symmetry
					SpawnProgramUtil::makeIfElseNode(				//angleIncrement
						SpawnInstructions::isPlayerFocused,
						SpawnProgramUtil::makeFloatValueSpawnNode(
							b::focusedAngleIncrement
						),
						SpawnProgramUtil::makeFloatValueSpawnNode(
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
					SpawnProgramUtil::makeVelocityValueSpawnNode(	//baseVel
						Vector2{ 0.0f, -b::speed }
					),
					SpawnProgramUtil::makeIntValueSpawnNode(4),		//symmetry
					SpawnProgramUtil::makeIfElseNode(				//angleIncrement
						SpawnInstructions::isPlayerFocused,
						SpawnProgramUtil::makeFloatValueSpawnNode(
							b::focusedAngleIncrement
						),
						SpawnProgramUtil::makeFloatValueSpawnNode(
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
					SpawnProgramUtil::makeVelocityValueSpawnNode(	//baseVel
						Vector2{ 0.0f, -b::speed }
					),
					SpawnProgramUtil::makeIntValueSpawnNode(5),		//symmetry
					SpawnProgramUtil::makeIfElseNode(				//angleIncrement
						SpawnInstructions::isPlayerFocused,
						SpawnProgramUtil::makeFloatValueSpawnNode(
							b::focusedAngleIncrement
						),
						SpawnProgramUtil::makeFloatValueSpawnNode(
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
					SpawnProgramUtil::makeVelocityValueSpawnNode(	//baseVel
						Vector2{ 0.0f, -b::speed }
					),
					SpawnProgramUtil::makeIntValueSpawnNode(6),		//symmetry
					SpawnProgramUtil::makeIfElseNode(				//angleIncrement
						SpawnInstructions::isPlayerFocused,
						SpawnProgramUtil::makeFloatValueSpawnNode(
							b::focusedAngleIncrement
						),
						SpawnProgramUtil::makeFloatValueSpawnNode(
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
		, shotB{
			shotBNode,
			config::playerShotMaxTick,
			false
		}
		{
		}
}