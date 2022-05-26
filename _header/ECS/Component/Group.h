#pragma once

#include <stdexcept>
#include <vector>

#include "Container/IntLookupTable.h"
#include "ComponentSet.h"
#include "Archetype.h"
#include "GroupIterator.h"

namespace wasp::ecs::component {
    
	class Group {
    private:
        //fields
        const ComponentSet* const componentKeyPointer{};
        std::vector<std::shared_ptr<Archetype>> archetypePointers{};
        std::vector<Group*> childGroupPointers{};

	public:
        Group() {
            throw std::runtime_error{ "should never be called; group ctor" };
        }

        Group(const ComponentSet* const componentKeyPointer)
            : componentKeyPointer{ componentKeyPointer } 
        {
            receiveNewArchetype(
                componentKeyPointer->getAssociatedArchetypeWeakPointer().lock()
            );
        }

        void receiveNewArchetype(std::shared_ptr<Archetype> archetypePointer) {
            if (componentSetFitsIntoGroup(
                archetypePointer->getComponentKeyPointer()
            )) {
                archetypePointers.push_back(archetypePointer);
                for (Group* childPointer : childGroupPointers) {
                    childPointer->receiveNewArchetype(archetypePointer);
                }
            }
        }

        bool addNewGroup(Group* groupPointer) {
            if (groupPointer == this) {
                throw std::runtime_error{
                    "Tried to add group to itself!"
                };
            }
            auto otherComponentKeyPointer{ groupPointer->getComponentKeyPointer() };
            if (otherComponentKeyPointer == componentKeyPointer) {
                throw new std::runtime_error{ 
                    "Tried to add a group with the same key!" 
                };
            }

            if (componentSetFitsIntoGroup(otherComponentKeyPointer)) {
                for (Group* childPointer : childGroupPointers) {
                    if (childPointer->addNewGroup(groupPointer)) {
                        return true;
                    }
                }
                //if fits this group but no children, add as a direct child
                addChildGroup(groupPointer);
                return true;
            }
            return false;
        }

        const ComponentSet* getComponentKeyPointer() const {
            return componentKeyPointer;
        }

        //the reason this is templated and not the whole class is becuase there is an
        //actual use case for acquiring a group iterator without every component,
        //namely treating certain components as markers
        template <typename... Ts>
        GroupIterator<Ts...> groupIterator() {
            std::vector<std::pair<ArchetypeIterator<Ts...>, ArchetypeIterator<Ts...>>> 
                archetypeIterators{};
            for (std::shared_ptr<Archetype>& archetypePointer : archetypePointers) {
                archetypeIterators.push_back(
                    { 
                        archetypePointer->begin<Ts...>(), 
                        archetypePointer->end<Ts...>() 
                    }
                );
            }
            return GroupIterator{ archetypeIterators };
        }

    private:
        void addChildGroup(Group* childGroupPointer) {
            childGroupPointers.push_back(childGroupPointer);
            for (std::shared_ptr<Archetype> archetypePointer : archetypePointers) {
                childGroupPointer->receiveNewArchetype(archetypePointer);
            }
        }

        bool componentSetFitsIntoGroup(
            const ComponentSet* const componentSetPointer
        ) const {
            return componentKeyPointer->isContainedIn(*componentSetPointer);
        }

        template <typename... Ts>
        void throwIfInvalidTypes() {
            if (!componentKeyPointer->containsAllComponents<Ts...>) {
                throw std::runtime_error{ "Group doesn't contain component!" };
            }
        }
	};
}