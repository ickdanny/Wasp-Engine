#pragma once

#include <initializer_list>
#include <vector>

namespace wasp::ecs::component {
    //todo: scene id exists above archetype/group
    //forward declaration of Archetype to handle circular dependency
    class Archetype;

    class ComponentSet {
    public:
        bool containsComponent(const int typeIndex) const;
        bool doesNotContainComponent(const int typeIndex) const {
            return !containsComponent(typeIndex);
        }
        bool containsAllComponents(const std::initializer_list<const int>& typeIndices) const {
            for (const int typeIndex : typeIndices) {
                if (doesNotContainComponent(typeIndex)) {
                    return false;
                }
            }
            return true;
        }
        bool containsAnyComponent(const std::initializer_list<const int>& typeIndices) const {
            for (const int typeIndex : typeIndices) {
                if (containsComponent(typeIndex)) {
                    return true;
                }
            }
            return false;
        }
        
        bool isContainedIn(const ComponentSet& other) const;
        int getNumComponents() const;
        const std::vector<int>& getPresentTypeIndices() const;

        void associateArchetype(const Archetype* archetype);
        Archetype* getAssociatedArchetype() const;
    };
}