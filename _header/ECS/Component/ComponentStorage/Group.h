#pragma once

#include "Container/IntLookupTable.h"
#include "ECS/Component/ComponentType.h"

namespace wasp::ecs::component {
	class Group {
	public:
		template<typename T>
		using ComponentIterator = container::IntLookupTable<T>::Iterator;

		template <typename T>
		ComponentIterator<T> getComponentIterator() {
			//todo:
		}
	};
}