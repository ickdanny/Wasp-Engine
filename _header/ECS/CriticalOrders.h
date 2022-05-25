#pragma once

#include "ECS/Entity/EntityHandle.h"

namespace wasp::ecs {

    //todo: systems store these themselves and then pass these on

    template <typename T>
    struct AddComponentOrder {
    private:
        //typedefs
        using EntityHandle = entity::EntityHandle;

    public:
        //fields
        const EntityHandle entityHandle{};
        const T component; //not initialized!

        AddComponentOrder(EntityHandle entityHandle, const T& component)
            : entityHandle{ entityHandle }
            , component{ component }{
        }
    };

    template <typename T>
    struct SetComponentOrder {
    private:
        //typedefs
        using EntityHandle = entity::EntityHandle;

    public:
        //fields
        const EntityHandle entityHandle{};
        const T component; //not initialized!

        SetComponentOrder(EntityHandle entityHandle, const T& component)
            : entityHandle{ entityHandle }
            , component{ component }{
        }
    };

    template <typename T>
    struct RemoveComponentOrder {
    private:
        //typedefs
        using EntityHandle = entity::EntityHandle;

    public:
        const EntityHandle entityHandle{};

        public RemoveComponentOrder(EntityHandle entityHandle)
            : entityHandle{ entityHandle } {
        }
    };

    template <typename... Ts>
    struct AddEntityOrder {
        //fields
        const std::tuple<Ts...> components{};
        const std::string name{};

        AddEntityOrder(std::tuple<Ts...> components)
            : components{ components } {
        }

        AddEntityOrder(std::tuple<Ts...> components, std::string name)
            : components{ components }
            , name{ name }{
        }
    };

    struct RemoveEntityOrder {
    private:
        //typedefs
        using EntityHandle = entity::EntityHandle;

    public:
        const EntityHandle entityHandle{};

        RemoveEntityOrder(EntityHandle entityHandle)
            : entityHandle{ entityHandle } {
        }
    };
}