#pragma once

#include <functional>
#include <string>

namespace wasp::ecs::entity {
    struct EntityHandle {
        const int entityID{};
        const int generation{};

        friend bool operator==(const EntityHandle& a, const EntityHandle& b) {
            return (a.entityID == b.entityID) && (a.generation == b.generation);
        }

        friend bool operator!= (const EntityHandle& a, const EntityHandle& b) {
            return (a.entityID != b.entityID) || (a.generation != b.generation);
        }

        explicit operator std::string() const {
            return "E_" + std::to_string(entityID) + "G_" + std::to_string(generation);
        }
    };
}