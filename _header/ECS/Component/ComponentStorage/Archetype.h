#pragma once

#include <vector>

#include "ECS/Component/ComponentType.h"
#include "ECS/Component/ComponentSet/ComponentSet.h"
#include "Container/IntLookupTable.h"

namespace wasp::ecs::component {

    namespace {
        constexpr int lookupTableInitValueCapacity{ 20 };
    }

    class Archetype {
    private:
        const ComponentSet* componentKey{};
        std::vector<container::IntLookupTableBase*> componentStorages{};
    public:
        Archetype(
            const std::vector<ComponentTypeBase>& types,
            const ComponentSet* componentKey,
            int initEntityCapacity,
            int initComponentCapacity
        ) 
            : componentKey{ componentKey } {
            
            //todo:
        }
        template <typename T>
        T getComponent(const int entityID, const ComponentType<T>& type);

        template <typename T>
        bool setComponent(
            const int entityID, 
            const ComponentType<T>& type, 
            const T& component
        );

        void moveEntity(const int entityID, Archetype newArchetype);
        bool removeEntity(const int entityID);

        ComponentSet* getComponentKey();

        //TODO: component iterating
    }
}