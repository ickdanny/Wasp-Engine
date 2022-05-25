#pragma once

#include <initializer_list>
#include <vector>
#include <bitset>

#include "Archetype.h"
#include "ComponentIndexer.h"

namespace wasp::ecs::component {

    namespace {
        constexpr std::size_t bitsetSize{ 96 };
    }

    //todo: scene id exists above archetype/group

    //forward declaration of Archetype to handle circular dependency
    class Archetype;

    class ComponentSet {
    private:
        //typedefs
        using Bitset = std::bitset<bitsetSize>;

        //fields
        Bitset bitset{};
        int numComponents{};
        mutable std::vector<int> presentTypeIndices{};

        mutable std::weak_ptr<Archetype> archetypeWeakPointer{};

        //constructs an empty component set
        ComponentSet()
            : numComponents{ 0 } 
        {
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

        //constructs a component set based on the provided type index
        ComponentSet(int typeIndex) 
            : numComponents{ 1 } 
        {
            testBitsetOutOfBounds();
            bitset.test(typeIndex);
            bitset[typeIndex] = true;
            presentTypeIndices.push_back(typeIndex);
        }

        //constructs a component set based on the provided type indices
        ComponentSet(const std::vector<int>& typeIndices)
            : numComponents{ typeIndices.size() }
        {
            testBitsetOutOfBounds();
            for (int typeIndex : typeIndices) {
                bitset.test(typeIndex);
                bitset[typeIndex] = true;
            }
            presentTypeIndices = typeIndices;
        }

        //copy constructor does NOT copy the archetypeWeakPointer
        ComponentSet(const ComponentSet& toCopy)
            : bitset{ toCopy.bitset }
            , numComponents{ toCopy.numComponents }
            , presentTypeIndices{ toCopy.presentTypeIndices } 
        {
            testBitsetOutOfBounds();
        }

    public:

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
            Bitset temp{ bitset };    //copy our bitset into our temp
            temp &= other.bitset;   //our temp is now A&B
            return bitset == temp;
        }

        int getNumComponents() const {
            return numComponents;
        }
        const std::vector<int>& getPresentTypeIndices() const {
            //assuming the empty ComponentSet will not get called very much
            if (presentTypeIndices.empty()) {
                makePresentTypeIndices();
            }
            return presentTypeIndices;
        }

        //archetype stuff
        void associateArchetype(
            const std::weak_ptr<Archetype> archetypeWeakPointer
        ) const {
            this->archetypeWeakPointer = archetypeWeakPointer;
        }
        std::weak_ptr<Archetype> getAssociatedArchetypeWeakPointer() const {
            return archetypeWeakPointer;
        }

    private:
        //modifiers
        template <typename T>
        ComponentSet addComponent() const {
            const int index{ ComponentIndexer::getIndex<T>() };
            //if we need to add a component
            if (!bitset[index]) {
                ComponentSet toRet{ *this };
                toRet.bitset.set(index);
                toRet.numComponents = numComponents + 1;
                return toRet;
            }
            //otherwise return ourselves
            return *this;
        }

        template <typename T>
        ComponentSet removeComponent() const {
            const int index{ ComponentIndexer::getIndex<T>() };
            //if we need to remove a component
            if (bitset[index]) {
                ComponentSet toRet{};
                toRet.bitset = bitset;
                toRet.bitset.reset(index);
                //type indices gets lazy initialized
                return toRet;
            }
            //otherwise return ourselves
            return *this;
        }

        template <typename... Ts>
        ComponentSet addComponents() const {
            if (sizeof...(Ts) <= 0) {
                throw std::runtime_error{ "zero type parameters!" };
            }
            std::vector<int> indicesToAdd{};
            indicesToAdd.push_back(ComponentIndexer.getIndex<Ts>)...;
            ComponentSet toRet{ *this };
            for (int index : indicesToAdd) {
                if (!bitset[index]) {
                    toRet.bitset.set(index);
                    ++(toRet.numComponents);
                }
            }
            return toRet;
        }

        template <typename... Ts>
        ComponentSet removeComponents() const {
            if (sizeof...(Ts) <= 0) {
                throw std::runtime_error{ "zero type parameters!" };
            }
            std::vector<int> indicesToRemove{};
            indicesToRemove.push_back(ComponentIndexer.getIndex<Ts>)...;
            ComponentSet toRet{};
            toRet.bitset = bitset;
            for (int index : indicesToRemove) {
                if (bitset[index]) {
                    toRet.bitset.reset(index);
                }
            }
            //type indices gets lazy initialized
            return toRet;
        }

        //helper functions
        void makePresentTypeIndices() const {
            presentTypeIndices = std::vector<int>{};
            for (int i = 0; i < bitsetSize; ++i) {
                if (bitset[i]) {
                    presentTypeIndices.push_back(i);
                }
            }
        }

        void testBitsetOutOfBounds() const {
            bitset.test(ComponentIndexer::getMaxIndex());
        }

    //operators
    public:
        friend bool operator==(const ComponentSet& a, const ComponentSet& b) {
            return a.bitset == b.bitset;
        }

        friend bool operator!= (const ComponentSet& a, const ComponentSet& b) {
            return a.bitset != b.bitset;
        }

        friend std::hash<ComponentSet>;
        friend class ComponentSetFactory;
    };
}

//specialization for hash
namespace std {
    template <> 
    class hash<wasp::ecs::component::ComponentSet> {
    private:
        using ComponentSet = wasp::ecs::component::ComponentSet;

    public:
        size_t operator()(const ComponentSet& componentSet) const {
            return hash<ComponentSet::Bitset>()(componentSet.bitset);
        }
    };
}