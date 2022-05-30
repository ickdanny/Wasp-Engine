#pragma once

#include "ECS/Component/ComponentStorage.h"
#include "ECS/Entity/EntityMetadataStorage.h"
#include "ECS/Entity/EntityID.h"

namespace wasp::ecs {
    class DataStorage {
    private:
        //typedefs
        using EntityMetadataStorage = entity::EntityMetadataStorage;
        using EntityID = entity::EntityID;
        using EntityHandle = entity::EntityHandle;
        using EntityMetadata = entity::EntityMetadata;
        using ComponentStorage = component::ComponentStorage;
        using ComponentSet = component::ComponentSet;
        using Group = component::Group;

        //fields (not initialized!)
        EntityMetadataStorage entityMetadataStorage;
        ComponentStorage componentStorage;

    public:

        //constructs a DataStorage with the specified initial entity capacity
        //and initial component capacity
        DataStorage(std::size_t initEntityCapacity, std::size_t initComponentCapacity)
            : entityMetadataStorage{ initEntityCapacity }
            , componentStorage{ initEntityCapacity, initComponentCapacity } {
        }

        //data query functions

        template <typename... Ts>
        Group* getGroupPointer() {
            return componentStorage.getGroupPointer<Ts...>();
        }

        bool isAlive(EntityHandle entityHandle) const {
            return entityMetadataStorage.isAlive(entityHandle);
        }

        bool isDead(EntityHandle entityHandle) const {
            return entityMetadataStorage.isDead(entityHandle);
        }

        template <typename T>
        bool containsComponent(EntityHandle entityHandle) const {
            if (isAlive(entityHandle)) {
                return getComponentSetPointer(entityHandle.entityID)
                    ->containsComponent<T>();
            }
            return false;
        }

        template <typename... Ts>
        bool containsAllComponents(EntityHandle entityHandle) const {
            if (isAlive(entityHandle)) {
                return getComponentSetPointer(entityHandle.entityID)
                    ->containsAllComponents<Ts...>();
            }
            return false;
        }

        template <typename... Ts>
        bool containsAnyComponent(EntityHandle entityHandle) const {
            if (isAlive(entityHandle)) {
                return getComponentSetPointer(entityHandle.entityID)
                    ->containsAnyComponent<Ts...>();
            }
            return false;
        }

        template <typename T>
        T& getComponent(EntityHandle entityHandle) {
            if (isAlive(entityHandle)) {
                if (containsComponent<T>(entityHandle)) {
                    return componentStorage.getComponent<T>(
                        entityHandle.entityID,
                        *getComponentSetPointer(entityHandle.entityID)
                    );
                }
                else {
                    throw std::runtime_error{ "entity doesn't contain component!" };
                }
            }
            throw std::runtime_error{ "tried to get component of dead entity!" };
        }

        template <typename T>
        const T& getComponent(EntityHandle entityHandle) const {
            if (isAlive(entityHandle)) {
                if (containsComponent<T>(entityHandle)) {
                    return componentStorage.getComponent<T>(
                        entityHandle.entityID,
                        *getComponentSetPointer(entityHandle.entityID)
                    );
                }
            }
            throw std::runtime_error{ "tried to get component of dead entity!" };
        }

        //Returns an entity handle for the entity with the specified entityID of the
        //current generation. Throws runtime_error if there is no such alive entity.
        EntityHandle makeHandle(EntityID entityID) const;

        //todo: named entities not implemented

        //data modification functions

        //returns true if successfully added component, false otherwise
        template <typename T>
        bool addComponent(AddComponentOrder<T> addComponentOrder) {
            if (isAlive(addComponentOrder.entityHandle)) {
                EntityID entityID{ addComponentOrder.entityHandle.entityID };

                const ComponentSet* oldComponentSetPointer{
                    getComponentSetPointer(entityID)
                };

                const ComponentSet* newComponentSetPointer{ 
                    componentStorage.addComponent(
                        addComponentOrder,
                        *oldComponentSetPointer
                    )
                };

                if (newComponentSetPointer) {
                    //successfully added component
                    setComponentSetPointer(entityID, newComponentSetPointer);
                    return true;
                }
            }
            return false;
        }

        //returns true if successfully set component, false otherwise
        template <typename T>
        bool setComponent(SetComponentOrder<T> setComponentOrder) {
            if (isAlive(setComponentOrder.entityHandle)) {
                EntityID entityID{ setComponentOrder.entityHandle.entityID };

                const ComponentSet* oldComponentSetPointer{
                    getComponentSetPointer(entityID)
                };

                const ComponentSet* newComponentSetPointer{ 
                    componentStorage.setComponent(
                        setComponentOrder,
                        *oldComponentSetPointer
                    ) 
                };

                if (newComponentSetPointer != oldComponentSetPointer) {
                    setComponentSetPointer(entityID, newComponentSetPointer);
                }
                return true;
            }
            return false;
        }

        //returns true if successfully removed component, false otherwise
        //entities with no components will still be "alive"
        template <typename T>
        bool removeComponent(RemoveComponentOrder<T> removeComponentOrder) {
            if (isAlive(removeComponentOrder.entityHandle)) {
                EntityID entityID{ removeComponentOrder.entityHandle.entityID() };

                const ComponentSet* oldComponentSetPointer{
                    getComponentSetPointer(entityID)
                };

                const ComponentSet* newComponentSetPointer{
                    componentStorage.removeComponent(
                        removeComponentOrder,
                        *oldComponentSetPointer
                    )
                };

                if (newComponentSetPointer != oldComponentSetPointer) {
                    //successfully removed component
                    setComponentSetPointer(entityID, newComponentSetPointer);
                    return true;
                }
            }
            return false;
        }

        //returns entityID
        template <typename... Ts>
        EntityHandle addEntity(const AddEntityOrder<Ts...>& addEntityOrder) {
            EntityHandle entityHandle{ entityMetadataStorage.createEntity() };

            const ComponentSet* componentSetPointer{
                componentStorage.addEntity(addEntityOrder, entityHandle.entityID)
            };

            setComponentSetPointer(entityHandle.entityID, componentSetPointer);
            return entityHandle;
        }

        //returns entityIDs in order
        template <typename... Ts, typename... Us>
        std::vector<EntityHandle> addEntities(
            const AddEntityOrder<Ts...>& addEntityOrder, 
            const Us&... args
        ) {
            std::vector<EntityHandle> entityHandleVector{};
            entityHandleVector.push_back(addEntity(addEntityOrder));
            if constexpr (sizeof...(args) > 0) {
                addEntities(entityHandleVector, args...);
            }
            return std::move(entityHandleVector);
        }

        //returns true if successfully removed entity, false otherwise
        bool removeEntity(RemoveEntityOrder removeEntityOrder);

    private:
        //helper functions
        const ComponentSet* getComponentSetPointer(EntityID entityID) const {
            return getMetadata(entityID).getComponentSetPointer();
        }

        //the non-const version of getMetadata() returns by reference
        EntityMetadata& getMetadata(EntityID entityID) {
            return entityMetadataStorage.getMetadata(entityID);
        }

        //the const version of getMetadata() returns by value
        EntityMetadata getMetadata(EntityID entityID) const {
            return entityMetadataStorage.getMetadata(entityID);
        }

        void setComponentSetPointer(
            EntityID entityID,
            const ComponentSet* componentSetPointer
        );

        template <typename... Ts, typename... Us>
        void addEntities(
            std::vector<EntityHandle>& entityHandleVector,
            const AddEntityOrder<Ts...>& addEntityOrder,
            const Us&... args
        ) {
            entityHandleVector.push_back(addEntity(addEntityOrder));
            if constexpr (sizeof...(args) > 0) {
                addEntities(entityHandleVector, args...);
            }
        }
    };
}