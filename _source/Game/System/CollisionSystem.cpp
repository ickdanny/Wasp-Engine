#include "Game/Systems/CollisionSystem.h"

#include "Game//Systems//QuadTree.h"

namespace wasp::game::systems {

	namespace {
		template <typename CollisionType>
		void checkCollisions(Scene& scene) {
			//get the group iterator for Position, Hitbox, CollidableMarker, and our
			//source type
			static const Topic<ecs::component::Group*> sourceGroupPointerStorageTopic{};
			auto sourceGroupPointer{
				getGroupPointer<
					Position, 
					Hitbox, 
					CollidableMarker, 
					typename CollisionType::Source
				>(
					scene,
					sourceGroupPointerStorageTopic
				)
			};
			auto sourceGroupIterator{ 
				sourceGroupPointer->groupIterator<
					Position, 
					Hitbox
				>() 
			};

			//insert all source entities into a quadTree
			QuadTree<ecs::entity::EntityID> quadTree{ config::collisionBounds };
			while (sourceGroupIterator.isValid()) {
				const auto [position, hitbox] = *sourceGroupIterator;
				ecs::entity::EntityID id{ sourceGroupIterator.getEntityID() };
				quadTree.insert(id, hitbox, position);
				++sourceGroupIterator;
			}

			//get the group iterator for Position, Hitbox, CollidableMarker, and our
			//target type
			static const Topic<ecs::component::Group*> targetGroupPointerStorageTopic{};
			auto targetGroupPointer{
				getGroupPointer<
					Position,
					Hitbox,
					CollidableMarker,
					typename CollisionType::Target
				>(
					scene,
					targetGroupPointerStorageTopic
				)
			};
			auto targetGroupIterator{
				targetGroupPointer->groupIterator<
					Position,
					Hitbox
				>()
			};

			//check every target entity against our quadTree
			while (targetGroupIterator.isValid()) {
				const auto [position, hitbox] = *targetGroupIterator;
				const auto& collidedEntities{ 
					quadTree.checkCollisions(hitbox, position) 
				};

				if (!collidedEntities.empty()) {
					ecs::entity::EntityID targetID{ sourceGroupIterator.getEntityID() };
					for (ecs::entity::EntityID sourceID : collidedEntities) {
						if (sourceID != targetID) {
							//todo: handle collisions
						}
					}
				}
			}
		}
	}

	void CollisionSystem::operator()(Scene& scene) {
		checkCollisions<PlayerCollisions>(scene);
	}
}