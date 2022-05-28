#pragma once

#include "Group.h"
#include "ArchetypeFactory.h"
#include "ComponentSet.h"
#include "ComponentSetFactory.h"

namespace wasp::ecs::component {
	class GroupFactory {

    private:
        std::unordered_map<ComponentSet, Group> keyToGroupMap{};
        Group* zeroGroupPointer{};

    public:
        GroupFactory(
            ComponentSetFactory& componentSetFactory,
            ArchetypeFactory& archetypeFactory
        );

        Group* getGroup(const ComponentSet& componentKey);

    private:
        void initSingleComponentGroups(
            ComponentSetFactory& componentSetFactory
        );
	};
}