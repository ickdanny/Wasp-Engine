#pragma once

#include "systemInclude.h"

namespace wasp::game::systems {

	template <typename ProviderFlag, typename ReceiverFlag>
	class CollisionSystem {

	private:

	public:
		void operator()(Scene& scene) {
			//todo: create spatial partition for every entity w/ provider flag
			//then, check all entities with receiver flag against that
			//finally, make sure to filter out self-collisions
		}
	};
}