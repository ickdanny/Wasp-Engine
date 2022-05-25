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
        EntityMetadataStorage(int initCapacity)
            : freeEntityIDStorage{ initCapacity }
        {
            entityMetadataList.resize(initCapacity);
        }

        int createEntity() {
            return freeEntityIDStorage.retrieveID();
        }

        void reclaimEntity(int entityID) {
            entityMetadataList[entityID].newGeneration();
            freeEntityIDStorage.reclaimID(entityID);
        }

        bool isAlive(int entityID) const {
            return freeEntityIDStorage.isIDUsed(entityID);
        }

        bool isDead(int entityID) const {
            return !isAlive(entityID);
        }

        bool isAlive(EntityHandle entityHandle) const {
            //check if this entity ID is alive and the generation matches
            return isAlive(entityHandle.entityID)
                && getMetadata(entityHandle.entityID).getGeneration() 
                == entityHandle.generation;
        }

        bool isDead(EntityHandle entityHandle) const {
            return !isAlive(entityHandle);
        }

        EntityMetadata getMetadata(int entityID) {
            if (entityID >= entityMetadataList.size()) {
                entityMetadataList.resize(entityID + 1);
            }
            return entityMetadataList[entityID];
        }

        const EntityMetadata getMetadata(int entityID) const {
            if (entityID >= entityMetadataList.size()) {
                entityMetadataList.resize(entityID + 1);
            }
            return entityMetadataList[entityID];
        }
    };
}