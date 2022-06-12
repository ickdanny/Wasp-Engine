#include "Game/Systems/InboundSystem.h"

namespace wasp::game::systems {

	void InboundSystem::operator()(Scene& scene) {
		//get the group iterator for Position, Velocity, and Inbound
		static const Topic<ecs::component::Group*> groupPointerStorageTopic{};
		auto groupPointer{
			getGroupPointer<Position, Velocity, Inbound>(
				scene,
				groupPointerStorageTopic
			)
		};
		auto groupIterator{ groupPointer->groupIterator<Position, Velocity, Inbound>() };
		
		//change all velocities to be in bound
		while (groupIterator.isValid()) {
			auto [position, velocity, bound] = *groupIterator;
			math::Point2 nextPosition{ position + velocity };
			//todo: rest of these functions
			if (GameUtil.isOutOfBounds(nextPosition, bound)) {
				math::Point2 inboundPos{ GameUtil.inboundPosition(nextPos, bound) };
				math::Vector2 inboundVel = GeometryUtil.vectorFromAToB(pos, inboundPos);
				velocity = inboundVel;
			}
			++groupIterator;
		}
	}
}