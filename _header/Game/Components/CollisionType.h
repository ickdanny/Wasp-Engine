#pragma once

#include "ECS/Entity/EntityHandle.h"

namespace wasp::game::components {

	enum class CollisionCommands {
		none,
		death,
		damage,
		player,
		pickup
	};

	//When creating a collision type, pass the derived type as the template parameter
	//such that the compiler will instantiate a separate Source and Target type for
	//each collision type.
	template <typename Derived>
	struct CollisionType {
	private:
		using EntityHandle = ecs::entity::EntityHandle;

	public:
		//set and cleared by CollisionDetectorSystem
		//format = sourceEntity, targetEntity
		static const Topic<std::tuple<EntityHandle, EntityHandle>> collisionTopic;

		struct Source {
			CollisionCommands command;
		};
		struct Target {
			CollisionCommands command;
		};
	};

	template <typename Derived>
	const Topic<std::tuple<ecs::entity::EntityHandle, ecs::entity::EntityHandle>> 
		CollisionType<Derived>::collisionTopic{};
}