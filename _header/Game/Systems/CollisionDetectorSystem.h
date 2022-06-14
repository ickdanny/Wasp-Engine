#pragma once

#include "systemInclude.h"

namespace wasp::game::systems {
	class CollisionDetectorSystem {
	public:
		void operator()(Scene& scene);
	};
}