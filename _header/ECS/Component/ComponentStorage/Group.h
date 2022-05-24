#pragma once

#include "Container/IntLookupTable.h"
#include "ECS/Component/ComponentStorage/MultiComponentIterator.h"

namespace wasp::ecs::component {
	class Group {
    private:
        //fields
        const ComponentSet* componentKeyPointer{};
	public:

        template <typename... Ts>
        MultiComponentIterator<Ts...> begin() {
            //todo: combine archetypes, group iterator?
        }

        template <typename... Ts>
        MultiComponentIterator<Ts...> end() {
            //todo: combine archetypes
        }
	};
}