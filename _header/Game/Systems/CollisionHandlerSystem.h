#pragma once

#include "systemInclude.h"

namespace wasp::game::systems {

	class CollisionHandlerSystem {
	public:
		void operator()(Scene& scene);
	};
}