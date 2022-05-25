#pragma once

#include "Archetype.h"
#include "ComponentSetFactory.h"

namespace wasp::ecs::component {

    class ArchetypeFactory {

    private:
        int initEntityCapacity{};
        int initComponentCapacity{};

        //throwing around raw pointers to elements in a vector is a HORRIBLE idea,
        //therefore we use shared_ptr
        std::vector<std::shared_ptr<Archetype>> archetypePointers{};

        std::function<void(std::shared_ptr<Archetype>)> newArchetypeCallback{};

    public:
        ArchetypeFactory(
            int initEntityCapacity, 
            int initComponentCapacity,
            ComponentSetFactory& componentSetFactory
        )
            : initEntityCapacity{ initEntityCapacity }
            , initComponentCapacity{ initComponentCapacity }
        {
            componentSetFactory.setNewComponentSetCallback(
                [&](const ComponentSet& componentSet) {
                    makeArchetype(componentSet);
                }
            );
        }

        //setting callbacks
        void setNewArchetypeCallback(
            const std::function<void(std::shared_ptr<Archetype>)> newArchetypeCallback
        ) {
            this->newArchetypeCallback = newArchetypeCallback;
        }

    private:
        void makeArchetype(const ComponentSet& componentSet) {
            archetypePointers.push_back(std::make_shared<Archetype>(
                &componentSet, initEntityCapacity, initComponentCapacity
            ));
            componentSet.associateArchetype(archetypePointers.back());
            newArchetypeCallback(archetypePointers.back());
        }
    };
}