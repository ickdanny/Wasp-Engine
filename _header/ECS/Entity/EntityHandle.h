#pragma once

#include <functional>
#include <string>

#include "EntityID.h"

namespace wasp::ecs::entity {
    struct EntityHandle {
        const EntityID entityID{};
        const int generation{};

        friend bool operator==(const EntityHandle& a, const EntityHandle& b);

        friend bool operator!= (const EntityHandle& a, const EntityHandle& b);

        explicit operator std::string() const;
    };
}