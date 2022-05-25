#pragma once

#include "GroupFactory.h"
#include "ECS/CriticalOrders.h"

namespace wasp::ecs::component {
    class ComponentStorage {
    private:
        //fields
        //todo: should componentSetFactory be held in the next level up? (VOSG)
        ComponentSetFactory componentSetFactory{};
        ArchetypeFactory archetypeFactory;  //not initialized!
        GroupFactory groupFactory;          //not initialized!

    public:
        ComponentStorage(int initEntityCapacity, int initComponentCapacity) 
            : componentSetFactory{}
            , archetypeFactory{
                initEntityCapacity,
                initComponentCapacity,
                componentSetFactory
            }
            , groupFactory{ componentSetFactory, archetypeFactory } {
        }

        const Group* createGroup(const ComponentSet& componentKey) {
            return groupFactory.createGroup(componentKey);
        }

        template <typename T>
        T& getComponent(int entityID, const ComponentSet& componentSet) {
            return componentSet.getAssociatedArchetypeWeakPointer().lock()
                ->getComponent<T>(entityID);
        }

        template <typename T>
        void addComponent(
            const AddComponentOrder<T>& addComponentOrder,
            const ComponentSet& oldComponentSet,
            const ComponentSet& newComponentSet
        ) {
            if (oldSet == newSet) {
                throw std::runtime_error{
                    "trying to add component entity "
                    + addComponentOrder.entityHandle 
                    + " already has!"
                };
            }

            auto oldArchetypePointer{
                oldComponentSet.getAssociatedArchetypeWeakPointer().lock()
            };
            auto newArchetypePointer{
                newComponentSet.getAssociatedArchetypeWeakPointer().lock()
            };

            oldArchetypePointer->moveEntity(
                addComponentOrder.entityHandle.entityID,
                *newArchetypePointer
            );
            newArchetypePointer->setComponent(
                addComponentOrder.entityHandle.entityID,
                addComponentOrder.component
            );
        }

        template <typename T>
        void setComponent(
            const SetComponentOrder<T>& setComponentOrder,
            const ComponentSet& oldComponentSet,
            const ComponentSet& newComponentSet
        ) {
            auto newArchetypePointer{
                newComponentSet.getAssociatedArchetypeWeakPointer().lock()
            };
            
            //only move if we are ADDING a component, not setting
            if (oldComponentSet != newComponentSet) {
                auto oldArchetypePointer{
                    oldComponentSet.getAssociatedArchetypeWeakPointer().lock()
                };
                oldArchetypePointer->moveEntity(
                    setComponentOrder.entityHandle.entityID,
                    *newArchetypePointer
                );
            }
            newArchetypePointer->setComponent(
                setComponentOrder.entityHandle.entityID,
                setComponentOrder.component
            );
        }

        template <typename T>
        void removeComponent(
            const RemoveComponentOrder<T>& removeComponentOrder,
            const ComponentSet& oldComponentSet,
            const ComponentSet& newComponentSet
        ) {
            if (oldComponentSet != newComponentSet) {
                auto oldArchetypePointer{
                    oldComponentSet.getAssociatedArchetypeWeakPointer().lock()
                };
                auto newArchetypePointer{
                    newComponentSet.getAssociatedArchetypeWeakPointer().lock()
                };
                //moving cuts off hanging components
                oldArchetypePointer->moveEntity(
                    addComponentOrder.entityHandle.entityID,
                    *newArchetypePointer
                );
            }
        }

        template <typename... Ts>
        void addEntity(
            const AddEntityOrder<Ts...>& addEntityOrder,
            const int entityID,
            const ComponentSet& componentSet
        ) {
            auto archetypePointer{
                componentSet.getAssociatedArchetypeWeakPointer().lock()
            };
            std::apply(
                [&](auto& ...x) {
                    addComponentForEntity(entityID, x, archetypePointer)...;
                },
                addEntityOrder.components
            );
        }

        void removeEntity(
            const RemoveEntityOrder& removeEntityOrder,
            const ComponentSet& componentSet
        ) {
            auto archetypePointer{
                componentSet.getAssociatedArchetypeWeakPointer().lock()
            };
            archetypePointer->removeEntity(removeEntityOrder.entityHandle.entityID);
        }

    private:
        //helper for addEntity
        template <typename T>
        void addComponentForEntity(
            int entityID,
            const T& component,
            std::shared_ptr<Archetype>& archetypePointer
        ) {
            archetypePointer->setComponent(entityID, component);
        }
    };
}