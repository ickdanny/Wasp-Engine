#pragma once

#include "Group.h"
#include "ECS/Component/ComponentSet/ComponentSet.h"

namespace wasp::ecs::component {
	class GroupProvider {
	public:
		const Group* createGroup(const ComponentSet& componentSet);
	};
}