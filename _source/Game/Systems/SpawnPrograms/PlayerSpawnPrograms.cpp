#include "Game/Systems/SpawnPrograms/PlayerSpawnPrograms.h"

namespace wasp::game::systems {

	using namespace math;
	using namespace components;

	namespace a {
		constexpr float opacity{ 0.8f };

		//small bubble data
		constexpr float smallSpeed{ 6.0f };
		constexpr AABB smallHitbox{ 8.0f };
		constexpr int smallDamage{ 10 };
		constexpr float smallOutbound{ -20.0f };

		constexpr float smallSpacing{ 7.0f };

		constexpr int smallMod0{ 8 };
		constexpr int smallMod1{ 5 };

		//large bubble data
		constexpr float largeSpeed{ smallSpeed * 0.8f };
		constexpr float largeHitbox{ 14.0f };
		constexpr Angle unfocusedLargeAngle{ 70.0f };
		constexpr Angle focusedLargeAngle{ 80.0f };
	}

	PlayerSpawnPrograms::PlayerSpawnPrograms(
		resources::BitmapStorage* bitmapStoragePointer
	)
		: bitmapStoragePointer{ bitmapStoragePointer }
		, smallBubblePrototype{
			EntityBuilder::makePosPrototype(
				Velocity{ Vector2{ 0.0f, -a::smallSpeed } },
				a::smallHitbox,
				EnemyCollisions::Source{ components::CollisionCommands::death },
				Damage{ a::smallDamage },
				Outbound{ a::smallOutbound },
				SpriteInstruction{
					bitmapStoragePointer->get(L"bubble_small")->d2dBitmap,
					{},		//offset
					{0.0f},	//rotation
					a::opacity
				},
				DrawOrder{ config::playerBulletDrawOrder }
			).heapClone()
	}
		, largeBubblePrototype{
			EntityBuilder::makePosVelPrototype(
				a::largeHitbox
				//todo: large bullet prototype
			).heapClone()
	}
		, spawnSingleSmallBubbleNode{
		/*
			&(new SpawnNodeData<std::shared_ptr<ComponentTupleBase>>{
				SpawnInstructions::spawnPos,
				smallBubblePrototype
			})->link(
				std::make_shared<SpawnNode>(SpawnInstructions::entityPosition)
			)
			*/
			SpawnProgramUtil::makeSpawnPosNode(
				smallBubblePrototype,
				SpawnProgramUtil::makeEntityPositionNode()
			)
	}
		, spawnDoubleSmallBubbleNode{
			SpawnProgramUtil::makeMirrorPosFormationNode(
				SpawnProgramUtil::makeEntityOffsetNode(
					SpawnProgramUtil::makeVelocityValueSpawnNode(
						math::Vector2{ a::smallSpacing / 2.0f, 0.0f }
					)
				),
				SpawnProgramUtil::makeEntityXNode(),
				//pass this since linked node is a non-factor
				spawnSingleSmallBubbleNode	
			)
		},
		spawnTripleSmallBubbleNode{
			SpawnProgramUtil::makeListNode(
				spawnSingleSmallBubbleNode,
				SpawnProgramUtil::makeMirrorPosFormationNode(
					SpawnProgramUtil::makeEntityOffsetNode(
						SpawnProgramUtil::makeVelocityValueSpawnNode(
							math::Vector2{ a::smallSpacing, 0.0f }
						)
					),
					SpawnProgramUtil::makeEntityXNode(),
					//pass this since linked node is a non-factor
					spawnSingleSmallBubbleNode
				)
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
		, shotANode{
			&(new SpawnNode{SpawnInstructions::playerPowerSplit})
			->link(
				shotAPowerBucket0Node,
				shotAPowerBucket1Node,
				shotAPowerBucket2Node
			)
		}
		, shotA{
			shotANode, 
			config::playerShotMaxTick, 
			false
		} {
		}
}