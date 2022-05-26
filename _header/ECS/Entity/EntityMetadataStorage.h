#pragma once

#include <vector>

#include "EntityHandle.h"
#include "EntityMetadata.h"
#include "FreeEntityIDStorage.h"

namespace wasp::ecs::entity {

    class EntityMetadataStorage {
    private:

        //fields
        mutable std::vector<EntityMetadata> entityMetadataList{};
        FreeEntityIDStorage freeEntityIDStorage;    //uninitialized!

    public:
        //Constructs an EntityMetadataStorage with the specified number of
        //default initialized entity metadatas
        EntityMetadataStorage(int initCapacity)
            : freeEntityIDStorage{ initCapacity }
        {
            entityMetadataList.resize(initCapacity);
        }

        EntityID createEntity() {
            return freeEntityIDStorage.retrieveID();
        }

        void reclaimEntity(EntityID entityID) {
            entityMetadataList[entityID].newGeneration();
            freeEntityIDStorage.reclaimID(entityID);
        }

        bool isAlive(EntityID entityID) const {
            return freeEntityIDStorage.isIDUsed(entityID);
        }

        bool isDead(EntityID entityID) const {
            return !isAlive(entityID);
        }

        bool isAlive(EntityHandle entityHandle) const {
            //check if this entity ID is alive and the generation matches
            return isAlive(entityHandle.entityID)
                && (getMetadata(entityHandle.entityID).getGeneration() 
                == entityHandle.generation);
        }

        bool isDead(EntityHandle entityHandle) const {
            return !isAlive(entityHandle);
        }

        EntityMetadata& getMetadata(EntityID entityID) {
            if (entityID >= entityMetadataList.size()) {
                entityMetadataList.resize(entityID + 1);
            }
            return entityMetadataList[entityID];
        }

        const EntityMetadata getMetadata(EntityID entityID) const {
            if (entityID >= entityMetadataList.size()) {
                entityMetadataList.resize(entityID + 1);
            }
            return entityMetadataList[entityID];
        }
    };
}