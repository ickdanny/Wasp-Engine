#pragma once

namespace wasp::ecs::component {
    struct ComponentTypeBase {
        int index;
    };

    template <typename T>
    struct ComponentType : public ComponentTypeBase {};
}