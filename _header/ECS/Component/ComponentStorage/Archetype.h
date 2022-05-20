#pragma once

#include <vector>
#include <memory>
#include <functional>

#include "ECS/Component/ComponentSet/ComponentSet.h"
#include "ECS/Component/TypeIndexRetriever.h"
#include "Container/IntLookupTable.h"

namespace wasp::ecs::component {

    class Archetype {
    private:
        //typedefs
        template <typename T>
        using IntLookupTable = container::IntLookupTable<T>;
        using IntLookupTableBase = container::IntLookupTableBase;

        //fields
        const ComponentSet* componentKeyPointer{};
        const int initEntityCapacity{};
        const int initComponentCapacity{};
        IntLookupTable<std::unique_ptr<IntLookupTableBase>> componentStorages;

        static IntLookupTable<std::function<void(const int, Archetype&)>> moveComponentVTable;

    public:
        Archetype(
            const ComponentSet* componentKeyPointer,
            int initEntityCapacity,
            int initComponentCapacity
        ) 
            : componentKeyPointer{ componentKeyPointer }
            , initEntityCapacity{ initEntityCapacity }
            , initComponentCapacity{ initComponentCapacity }
            , componentStorages(
                TypeIndexRetriever::maxIndex(), 
                componentKeyPointer->getPresentTypeIndices().size()
            ){
        }

        template <typename T>
        T& getComponent(const int entityID) {
            return getComponentStorage<T>().get(entityID);
        }

        template <typename T>
        bool setComponent(const int entityID, const T& component) {
            //this bit of code causes the compiler to generate a 
            static bool dummyToInstantiateMoveComponent{ moveComponentVTable.set(
                TypeIndexRetriever::retrieveIndex<T>(),
                [&](const int entityID, Archetype& newArchetype) {
                    moveComponent<T>(entityID, newArchetype);
                }
            ) };

            return getComponentStorage<T>().set(entityID, component);
        }
        
        void moveEntity(const int entityID, Archetype& newArchetype) {
            for (int i : newArchetype.getComponentKey()->getPresentTypeIndices()) {
                std::unique_ptr<IntLookupTableBase>& storagePointer = componentStorages.get(i);
                if (storagePointer != nullptr) {
                    moveComponentVTable.get(i)(entityID, newArchetype);
                }
            }
            removeEntity(entityID);
        }

        bool removeEntity(const int entityID) {
            bool wasAnyComponentRemoved{ false };
            for (int i : componentKeyPointer->getPresentTypeIndices()) {
                std::unique_ptr<IntLookupTableBase>& storagePointer = componentStorages.get(i);
                if (storagePointer != nullptr && storagePointer->remove(entityID)) {
                    wasAnyComponentRemoved = true;
                }
            }
            return wasAnyComponentRemoved;
        }

        const ComponentSet* getComponentKey() const;

    private:
        template <typename T>
        IntLookupTable<T>& getComponentStorage() {
            int typeIndex{ TypeIndexRetriever::retrieveIndex<T>() };
            IntLookupTableBase& base{ *componentStorages.get(typeIndex) };
            return static_cast<IntLookupTable<T>&>(base);
        }

        template <typename T>
        void moveComponent(const int entityID, Archetype& newArchetype) {
            newArchetype.setComponent(entityID, getComponentStorage<T>().get(entityID));
        }

        //TODO: component iterating with tuples?
    };

    container::IntLookupTable<std::function<void(const int, Archetype&)>> 
        Archetype::moveComponentVTable{};
}