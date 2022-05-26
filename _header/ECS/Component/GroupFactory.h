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
        ) {
            const auto& zeroSet{ componentSetFactory.makeSet() };
            keyToGroupMap.emplace(zeroSet, &zeroSet);
            zeroGroupPointer = &keyToGroupMap[zeroSet];
            initSingleComponentGroups(componentSetFactory);
            archetypeFactory.setNewArchetypeCallback(
                [&](std::shared_ptr<Archetype> archetypePointer) {
                    zeroGroupPointer->receiveNewArchetype(archetypePointer);
                }
            );
        }

        Group* makeGroup(const ComponentSet& componentKey) {
            auto found{ keyToGroupMap.find(componentKey) };
            if (found == keyToGroupMap.end()) {
                keyToGroupMap.emplace(componentKey, &componentKey);
                Group* newGroupPointer{ &keyToGroupMap[componentKey] };
                if (!zeroGroupPointer->addNewGroup(newGroupPointer)) {
                    throw std::runtime_error{ "error in makeGroup" };
                }
                return newGroupPointer;
            }
            return &(found->second);
        }

    private:
        void initSingleComponentGroups(
            ComponentSetFactory& componentSetFactory
        ) {
            for (
                std::size_t typeIndex = 0;
                typeIndex < maxComponents;
                ++typeIndex
            ) {
                const auto& singleSet = componentSetFactory.makeSet(typeIndex);
                keyToGroupMap.emplace(singleSet, &singleSet);
                if (!zeroGroupPointer->addNewGroup(&keyToGroupMap[singleSet])) {
                    throw std::runtime_error{ "error in initSingleComponentGroups" };
                }
            }
        }
	};
}