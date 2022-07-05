#pragma once

#include "systemInclude.h"

namespace wasp::game::systems {

	class RotateSpriteForwardSystem {
	public:
		void operator()(Scene& scene);
	};
}