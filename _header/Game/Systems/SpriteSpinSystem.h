#pragma once

#include "systemInclude.h"

namespace wasp::game::systems {

	class SpriteSpinSystem {
	public:
		void operator()(Scene& scene);
	};
}