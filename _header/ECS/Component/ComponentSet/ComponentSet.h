#pragma once

#include <initializer_list>
#include <vector>
#include <bitset>

#include "ECS/Component/ComponentStorage/Archetype.h"
#include "ECS/Component/ComponentIndexer.h"

namespace wasp::ecs::component {
    //todo: scene id exists above archetype/group
    
    namespace {
        constexpr std::size_t bitsetSize{ 96 };
    }

    //forward declaration of Archetype to handle circular dependency
    class Archetype;

    class ComponentSet {
    private:
        using Bitset = std::bitset<bitsetSize>;

        //fields
        Bitset bitset{};
        int numComponents{};
        std::vector<int> presentTypeIndices{};

        const Archetype* archetypePointer{};

    public:

        //constructs an empty component set
        ComponentSet() 
            : numComponents{ 0 } {
            testBitsetOutOfBounds();
        }
        
        //constructs a component set based on the provided component types
        template <typename... Ts>
        ComponentSet() 
            : numComponents{ sizeof...(Ts) } 
        {
            testBitsetOutOfBounds();
            bitset[ComponentIndexer::getIndex<Ts>()] = true...;
            presentTypeIndices.push_back(ComponentIndexer::getIndex<Ts>())...;
        }

        //copy constructor does NOT copy the archetypePointer
        ComponentSet(const ComponentSet& toCopy)
            : bitset{ toCopy.bitset }
            , numComponents{ toCopy.numComponents }
            , presentTypeIndices{ toCopy.presentTypeIndices } {
        }

        template <typename T>
        bool containsComponent() const {
            return bitSet[ComponentIndexer::getIndex<T>()];
        }

        template <typename T>
        bool doesNotContainComponent() const {
            return !containsComponent<T>();
        }

        template <typename... Ts>
        bool containsAllComponents() const {
            if (doesNotContainComponent<Ts>()...) {
                return false;
            }
            return true;
        }

        template <typename... Ts>
        bool containsAnyComponent() const {
            if (containsComponent<Ts>()...) {
                return true;
            }
            return false;
        }
        
        bool isContainedIn(const ComponentSet& other) const {
            Bitset temp{bitset};    //copy our bitset into our temp
            temp &= other.bitset;   //our temp is now A&B
            return bitset == temp;  
        }

        int getNumComponents() const {
            return numComponents;
        }
        const std::vector<int>& getPresentTypeIndices() const {
            return presentTypeIndices;
        }

        void associateArchetype(const Archetype* archetypePointer) {
            this->archetypePointer = archetypePointer ;
        }
        const Archetype* getAssociatedArchetype() const {
            return archetypePointer;
        }

    private:

        template <typename T>
        ComponentSet& addComponent() {
            const int index{ ComponentIndexer::getIndex<T>() };
            //if we need to add a component
            if (!bitset[index]) {
                ComponentSet toRet{ *this };
                toRet.bitSet.set(index);
                toRet.presentTypeIndices.push_back(index);
                return toRet;
            }
            //otherwise return ourselves
            return this;
        }

        //todo: other mutators

        void testBitsetOutOfBounds() {
            bitset.test(ComponentIndexer::getMaxIndex());
        }
    };
}