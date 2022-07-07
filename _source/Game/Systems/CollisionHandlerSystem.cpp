#include "Game/Systems/CollisionHandlerSystem.h"

#include "Logging.h"

namespace wasp::game::systems {

	namespace {

		//publish a death message
		void handleDeathCommand(
			Scene& scene,
			const ecs::entity::EntityHandle& entityHandle
		) {
			scene.getChannel(SceneTopics::deaths).addMessage(entityHandle);
		}

		void handleDamageCommand(
			Scene& scene,
			const ecs::entity::EntityHandle& entityHandle,
			const ecs::entity::EntityHandle& collidedHandle
		) {
			//check to see if this entity has health and the other entity has damage
			auto& dataStorage{ scene.getDataStorage() };
			if (dataStorage.containsComponent<Health>(entityHandle)
				&& dataStorage.containsComponent<Damage>(collidedHandle)) 
			{
				//if so, subtract damage from health
				auto& health{ dataStorage.getComponent<Health>(entityHandle) };
				if (health.value > 0) {
					const auto& damage{ 
						dataStorage.getComponent<Damage>(collidedHandle) 
					};
					health.value -= damage.value;
					//if health <= 0, treat it as an entity death
					if (health.value <= 0) {
						handleDeathCommand(scene, entityHandle);
					}
				}
			}
		}

		void handlePlayerCommand(
			Scene& scene,
			const ecs::entity::EntityHandle& playerHandle
		) {
			scene.getChannel(SceneTopics::playerHits).addMessage(playerHandle);
		}

		void handlePickupCommand(
			Scene& scene,
			const ecs::entity::EntityHandle& pickupHandle,
			const ecs::entity::EntityHandle& collidedHandle
		) {
			//todo: handle pickups
		}

		void handleCollisionCommand(
			Scene& scene,
			const ecs::entity::EntityHandle& entityHandle,
			const components::CollisionCommands command,
			const ecs::entity::EntityHandle& collidedHandle
		) {
			switch (command){
				case components::CollisionCommands::death:
					handleDeathCommand(scene, entityHandle);
					break;
				case components::CollisionCommands::damage:
					handleDamageCommand(scene, entityHandle, collidedHandle);
					break;
				case components::CollisionCommands::player:
					handlePlayerCommand(scene, entityHandle);
					break;
				case components::CollisionCommands::pickup:
					handlePickupCommand(scene, entityHandle, collidedHandle);
					break;
				case components::CollisionCommands::none:
					//do nothing
					break;
				default:
					throw std::runtime_error{
						"default case reached in collision handler system"
					};
			}
		}

		template <typename CollisionType>
		void handleCollisions(Scene& scene) {
			const auto& collisionChannel{
				scene.getChannel(CollisionType::collisionTopic)
			};

			if (collisionChannel.hasMessages()) {
				auto& dataStorage{ scene.getDataStorage() };

				//handle the commands for each source/target pair
				for (const auto& [sourceHandle, targetHandle]
					: collisionChannel.getMessages()
				) {
					const auto sourceCommand{
						dataStorage.getComponent<typename CollisionType::Source>(
							sourceHandle
						).command
					};
					handleCollisionCommand(
						scene, 
						sourceHandle, 
						sourceCommand, 
						targetHandle
					);
					const auto targetCommand{
						dataStorage.getComponent<typename CollisionType::Target>(
							targetHandle
						).command
					};
					handleCollisionCommand(
						scene, 
						targetHandle, 
						targetCommand,
						sourceHandle
					);
				}
			}
		}
	}

	void CollisionHandlerSystem::operator()(Scene& scene) {
		//this system is responsible for clearing the playerHits channel
		scene.getChannel(SceneTopics::playerHits).clear();

		handleCollisions<PlayerCollisions>(scene);
		handleCollisions<EnemyCollisions>(scene);
		handleCollisions<BulletCollisions>(scene);
		handleCollisions<PickupCollisions>(scene);
	}
}