#pragma once

#include "systemInclude.h"
#include "Programs/Programs.h"

namespace wasp::game::systems {

	class CollisionHandlerSystem {
	private:
		//typedefs
		using EntityHandle = ecs::entity::EntityHandle;
		using CollisionCommands = components::CollisionCommands;

		//fields
		Programs* programsPointer{};

	public:
		CollisionHandlerSystem(Programs* programsPointer);
		void operator()(Scene& scene);

	private:
		//helper functions

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

		void handleCollisionCommand(
			Scene& scene,
			const EntityHandle& entityHandle,
			const components::CollisionCommands command,
			const EntityHandle& collidedHandle
		);

		void handlePickupCommand(
			Scene& scene,
			const EntityHandle& pickupHandle,
			const EntityHandle& collidedHandle
		);

		void handlePlayerCommand(Scene& scene, const EntityHandle& playerHandle);

		void handleDamageCommand(
			Scene& scene,
			const EntityHandle& entityHandle,
			const EntityHandle& collidedHandle
		);

		void handleDeathCommand(
			Scene& scene,
			const EntityHandle& entityHandle
		);
	};
}