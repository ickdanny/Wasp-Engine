#pragma once

#include <vector>
#include <memory>
#include <functional>

#include "ComponentSet.h"
#include "ComponentIndexer.h"
#include "ArchetypeIterator.h"
#include "Container/IntLookupTable.h"

namespace wasp::ecs::component {

    class Archetype {
    private:
        //typedefs
        template <typename T>
        using IntLookupTable = container::IntLookupTable<T>;
        using IntLookupTableBase = container::IntLookupTableBase;

        //fields
        const ComponentSet const* componentKeyPointer{};
        const int initEntityCapacity{};
        const int initComponentCapacity{};
        IntLookupTable<std::unique_ptr<IntLookupTableBase>> componentStorages;

        static IntLookupTable<std::function<void(const int, Archetype&)>> 
            moveComponentVTable;

        Archetype(
            const ComponentSet const* componentKeyPointer,
            int initEntityCapacity,
            int initComponentCapacity
        ) 
            : componentKeyPointer{ componentKeyPointer }
            , initEntityCapacity{ initEntityCapacity }
            , initComponentCapacity{ initComponentCapacity }
            , componentStorages(
                ComponentIndexer::getMaxIndex(), 
                componentKeyPointer->getPresentTypeIndices().size()
            ) {
        }

    public:
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
            static bool dummyToInstantiateMoveComponent{ 
                moveComponentVTable.set(
                    ComponentIndexer::retrieveIndex<T>(),
                    [&](const int entityID, Archetype& newArchetype) {
                        moveComponent<T>(entityID, newArchetype);
                    }
                ) 
            };

            return getComponentStorage<T>().set(entityID, component);
        }
        
        void moveEntity(const int entityID, Archetype& newArchetype) {
            for (int i : 
                newArchetype.getComponentKeyPointer()->getPresentTypeIndices()
            ) {
                std::unique_ptr<IntLookupTableBase>& storagePointer = 
                    componentStorages.get(i);

                if (storagePointer != nullptr) {
                    //components present in this archetype but not present in
                    //the new archetype will be removed
                    moveComponentVTable.get(i)(entityID, newArchetype);
                    storagePointer->remove(entityID);
                }
            }
        }

        bool removeEntity(const int entityID) {
            bool wasAnyComponentRemoved{ false };
            for (int i : componentKeyPointer->getPresentTypeIndices()) {
                std::unique_ptr<IntLookupTableBase>& storagePointer = 
                    componentStorages.get(i);

                if (storagePointer != nullptr && storagePointer->remove(entityID)) {
                    wasAnyComponentRemoved = true;
                }
            }
            return wasAnyComponentRemoved;
        }

        template <typename... Ts>
        ArchetypeIterator<Ts...> begin() {
            return MultiComponentIterator<Ts...>{
                std::tuple{ getComponentStorage<Ts>().begin()... }
            };
        }

        template <typename... Ts>
        ArchetypeIterator<Ts...> end() {
            return MultiComponentIterator<Ts...>{
                std::tuple{ getComponentStorage<Ts>().end()... }
            };
        }

        const ComponentSet* getComponentKeyPointer() const {
            return componentKeyPointer;
        }

    private:
        template <typename T>
        IntLookupTable<T>& getComponentStorage() {
            int typeIndex{ ComponentIndexRetriever::retrieveIndex<K>() };
            IntLookupTableBase& base{ *componentStorages.get(typeIndex) };
            return static_cast<IntLookupTable<T>&>(base);
        }

        template <typename T>
        void moveComponent(const int entityID, Archetype& newArchetype) {
            if (newArchetype.getComponentKeyPointer()->containsComponent<T>()) {
                newArchetype.setComponent(
                    entityID,
                    getComponentStorage<T>().get(entityID)
                );
            }
        }

        friend class ArchetypeFactory;
    };

    container::IntLookupTable<std::function<void(const int, Archetype&)>> 
        Archetype::moveComponentVTable{};
}