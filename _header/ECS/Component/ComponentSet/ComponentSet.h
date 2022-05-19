#pragma once

#include <initializer_list>
#include <vector>

#include "ECS\Component\ComponentType.h"

namespace wasp::ecs::component {
    //todo: scene id exists above archetype/group
    //forward declaration of Archetype to handle circular dependency
    class Archetype;

    class ComponentSet {
    public:
        bool containsComponent(const int typeIndex);
        bool doesNotContainComponent(const int typeIndex) {
            return !containsComponent(typeIndex);
        }
        bool containsAllComponents(const std::initializer_list<const int>& typeIndices) {
            for (const int typeIndex : typeIndices) {
                if (doesNotContainComponent(typeIndex)) {
                    return false;
                }
            }
            return true;
        }
        bool containsAnyComponent(const std::initializer_list<const int>& typeIndices) {
            for (const int typeIndex : typeIndices) {
                if (containsComponent(typeIndex)) {
                    return true;
                }
            }
            return false;
        }
        
        bool isContainedIn(const ComponentSet& other);
        int getNumComponents();
        std::vector<int> getPresentTypeIndices();

        void associateArchetype(const Archetype* archetype);
        Archetype* getAssociatedArchetype();
    };
}