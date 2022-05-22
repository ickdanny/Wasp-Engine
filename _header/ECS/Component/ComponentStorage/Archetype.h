#pragma once

#include <vector>
#include <memory>
#include <functional>

#include "ECS/Component/ComponentSet/ComponentSet.h"
#include "ECS/Component/ComponentIndexer.h"
#include "MultiComponentIterator.h"
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
                ComponentIndexer::getMaxIndex(), 
                componentKeyPointer->getPresentTypeIndices().size()
            ){
        }

        template <typename T>
        T& getComponent(const int entityID) {
            return getComponentStorage<T>().get(entityID);
        }

        template <typename T>
        const T& getComponent(const int entityID) const {
            return getComponentStorage<T>().get(entityID);
        }

        template <typename T>
        bool setComponent(const int entityID, const T& component) {
            //this bit of code causes the compiler to generate a 
            static bool dummyToInstantiateMoveComponent{ moveComponentVTable.set(
                ComponentIndexRetriever::retrieveIndex<T>(),
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

        template <typename... Ts>
        MultiComponentIterator<Ts...> begin() {
            return MultiComponentIterator<Ts...>{
                std::tuple{ getComponentStorage<Ts>().begin()... }
            };
        }

        template <typename... Ts>
        MultiComponentIterator<Ts...> end() {
            return MultiComponentIterator<Ts...>{
                std::tuple{ getComponentStorage<Ts>().end()... }
            };
        }

        const ComponentSet* getComponentKey() const {
            return componentKeyPointer;
        }

    private:
        template <typename T>
        IntLookupTable<T>& getComponentStorage() {
            int typeIndex{ ComponentIndexRetriever::retrieveIndex<T>() };
            IntLookupTableBase& base{ *componentStorages.get(typeIndex) };
            return static_cast<IntLookupTable<T>&>(base);
        }

        template <typename T>
        void moveComponent(const int entityID, Archetype& newArchetype) {
            newArchetype.setComponent(entityID, getComponentStorage<T>().get(entityID));
        }
    };

    container::IntLookupTable<std::function<void(const int, Archetype&)>> 
        Archetype::moveComponentVTable{};
}