#pragma once

#include <unordered_set>

#include "ComponentSet.h"

namespace wasp::ecs::component {
    class ComponentSetFactory {
    private:
        //fields
        std::unordered_set<ComponentSet> canonicalComponentSets{};
        std::function<void(const ComponentSet&)> newComponentSetCallback{};

    public:
        //default constructor
        ComponentSetFactory() = default;

        //component set creation
        const ComponentSet& makeSet() {
            return getCanonicalSetAndBroadcastIfNew(ComponentSet{});
        }

        template <typename... Ts>
        const ComponentSet& makeSet() {
            return getCanonicalSetAndBroadcastIfNew(ComponentSet<Ts...>{});
        }

        template <typename T>
        const ComponentSet& addComponent(const ComponentSet& base) {
            return getCanonicalSetAndBroadcastIfNew(base.addComponent<T>());
        }

        template <typename... Ts>
        const ComponentSet& addComponents(const ComponentSet& base) {
            return getCanonicalSetAndBroadcastIfNew(base.addComponents<Ts...>());
        }

        template <typename T>
        const ComponentSet& removeComponent(const ComponentSet& base) {
            return getCanonicalSetAndBroadcastIfNew(base.removeComponent<T>());
        }

        template <typename... Ts>
        const ComponentSet& removeComponents(const ComponentSet& base) {
            return getCanonicalSetAndBroadcastIfNew(base.removeComponents<Ts...>());
        }

        //setting callbacks
        void setNewComponentSetCallback(
            const std::function<void(const ComponentSet&)>& newComponentSetCallback
        ) {
            this->newComponentSetCallback = newComponentSetCallback;
        }

    private:
        const ComponentSet& getCanonicalSetAndBroadcastIfNew(
            const ComponentSet& componentSet
        ) {
            const auto [iterator, isNew] = canonicalComponentSets.insert(componentSet);
            const ComponentSet& toRet{ *iterator };
            if (isNew) {
                newComponentSetCallback(toRet);
            }
            return toRet;
        }
    };
}