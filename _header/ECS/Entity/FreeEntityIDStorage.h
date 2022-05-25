#pragma once

#include <vector>
#include <stdexcept>

namespace wasp::ecs::entity {

    namespace {
        constexpr float usageCapacityLimit{ 0.75f };
        constexpr float resizeRatio{ 2.0f };
    }

    class FreeEntityIDStorage {
    private:
        //typedefs
        using Iterator = std::vector<bool>::iterator;

        //fields
        
        //an element at index n is true if that entity ID is in use, false otherwise
        std::vector<bool> entityIDSet{};
        int currentLiveEntities{};
        int currentPos{};

    public:
        FreeEntityIDStorage(int initCapacity)
            : currentLiveEntities{ 0 }
            , currentPos{ 0 }
        {
            if (initCapacity <= 1) {
                throw std::runtime_error{ "init capacity too small!" };
            }
            entityIDSet.resize(initCapacity, false);
        }

        bool isIDUsed(int entityID) const {
            if (entityID >= entityIDSet.size()) {
                return false;
            }
            return entityIDSet[entityID];
        }

        int retrieveID() {
            if (currentPos >= entityIDSet.size()) {
                currentPos = 0;
            }
            //find a dead entity ID
            while (entityIDSet[currentPos]) {
                ++currentPos;
                if (currentPos >= entityIDSet.size()) {
                    currentPos = 0;
                }
            }
            //set that entity ID to alive
            entityIDSet[currentPos] = true;

            //update our live entity count
            ++currentLiveEntities;

            //resize
            resizeIfNecessary();

            //return our entity ID, and step currentPos for efficiency
            return currentPos++;
        }

        void reclaimID(int entityID) {
            //if this entity is currently in use
            if (auto ref{ entityIDSet[entityID] }) {
                //kill that entity
                ref = false;
            }
            //otherwise something went wrong
            else {
                throw std::runtime_error{ "error in reclaimID" };
            }
        }

    private:
        void resizeIfNecessary() {
            float usageCapacity{
                static_cast<float>(currentLiveEntities) / entityIDSet.size()
            };
            if (usageCapacity > usageCapacityLimit) {
                int newSize{ static_cast<int>(entityIDSet.size() * resizeRatio) };
                entityIDSet.resize(newSize, false);
            }
        }
    };
}