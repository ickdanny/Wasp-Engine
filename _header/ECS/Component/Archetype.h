#pragma once

#include <vector>
#include <memory>
#include <functional>

#include "ComponentSet.h"
#include "ComponentIndexer.h"
#include "ArchetypeIterator.h"
#include "Container/IntLookupTable.h"
#include "ECS/Entity/EntityID.h"

namespace wasp::ecs::component {

    class Archetype {
    private:
        //typedefs
        template <typename T>
        using IntLookupTable = container::IntLookupTable<T>;
        using IntLookupTableBase = container::IntLookupTableBase;
        using EntityID = entity::EntityID;

        //fields
        const ComponentSet* const componentKeyPointer{};
        const int initEntityCapacity{};
        const int initComponentCapacity{};
        //using unique_ptr to point to base class
        //todo: can probably just use a normal array or something
        IntLookupTable<std::unique_ptr<IntLookupTableBase>> componentStorages;

        static IntLookupTable<std::function<void(const EntityID, Archetype&)>>
            moveComponentVTable;

    public:

        Archetype(
            const ComponentSet* const componentKeyPointer,
            int initEntityCapacity,
            int initComponentCapacity
        ) 
            : componentKeyPointer{ componentKeyPointer }
            , initEntityCapacity{ initEntityCapacity }
            , initComponentCapacity{ initComponentCapacity }
            , componentStorages(
                maxComponents, 
                componentKeyPointer->getPresentTypeIndices().size()
            ) {
        }

        template <typename T>
        T& getComponent(const EntityID entityID) {
            return getComponentStorage<T>().get(entityID);
        }

        template <typename T>
        const T& getComponent(const EntityID entityID) const {
            return getComponentStorage<T>().get(entityID);
        }

        template <typename T>
        bool setComponent(const EntityID entityID, T& component) {
            //this bit of code causes the compiler to generate a moveComponent func
            static bool dummyToInstantiateMoveComponent{
                moveComponentVTable.set(
                    ComponentIndexer::getIndex<T>(),
                    [&](const EntityID entityID, Archetype& newArchetype) {
                        moveComponent<T>(entityID, newArchetype);
                    }
                )
            };

            return getComponentStorage<T>().set(entityID, component);
        }
        
        void moveEntity(const EntityID entityID, Archetype& newArchetype) {
            for (int typeIndex : 
                newArchetype.getComponentKeyPointer()->getPresentTypeIndices()
            ) {
                if (componentStorages.contains(typeIndex)) {
                    std::unique_ptr<IntLookupTableBase>& storagePointer =
                        componentStorages.get(typeIndex);

                    if (storagePointer != nullptr) {
                        //components present in this archetype but not present in
                        //the new archetype will be removed
                        moveComponentVTable.get(typeIndex)(entityID, newArchetype);
                        storagePointer->remove(entityID);
                    }
                }
            }
        }

        bool removeEntity(const EntityID entityID) {
            bool wasAnyComponentRemoved{ false };
            for (int typeIndex : componentKeyPointer->getPresentTypeIndices()) {
                if (componentStorages.contains(typeIndex)) {
                    std::unique_ptr<IntLookupTableBase>& storagePointer =
                        componentStorages.get(typeIndex);

                    if (storagePointer != nullptr 
                        && storagePointer->remove(entityID)) 
                    {
                        wasAnyComponentRemoved = true;
                    }
                }
            }
            return wasAnyComponentRemoved;
        }

        template <typename... Ts>
        ArchetypeIterator<Ts...> begin() {
            return ArchetypeIterator<Ts...>{
                std::tuple{ getComponentStorage<Ts>().begin()... }
            };
        }

        template <typename... Ts>
        ArchetypeIterator<Ts...> end() {
            return ArchetypeIterator<Ts...>{
                std::tuple{ getComponentStorage<Ts>().end()... }
            };
        }

        const ComponentSet* getComponentKeyPointer() const {
            return componentKeyPointer;
        }

    private:
        template <typename T>
        IntLookupTable<T>& getComponentStorage() {
            std::size_t typeIndex{ ComponentIndexer::getIndex<T>() };
            //initialize if necessary
            if (!componentStorages.contains(typeIndex)) {
                componentStorages.emplace(
                    typeIndex,
                    new IntLookupTable<T>{
                        initEntityCapacity,
                        initComponentCapacity
                    }
                );
            }
            IntLookupTableBase& base{ *componentStorages.get(typeIndex) };
            return static_cast<IntLookupTable<T>&>(base);
        }

        template <typename T>
        void moveComponent(const EntityID entityID, Archetype& newArchetype) {
            if (newArchetype.getComponentKeyPointer()->containsComponent<T>()) {
                newArchetype.setComponent(
                    entityID,
                    getComponentStorage<T>().get(entityID)
                );
            }
        }

        friend class ArchetypeFactory;
    };

    container::IntLookupTable<std::function<void(const entity::EntityID, Archetype&)>>
        Archetype::moveComponentVTable{};
}