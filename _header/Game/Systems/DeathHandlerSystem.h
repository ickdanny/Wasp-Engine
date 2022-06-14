#pragma once

#include "systemInclude.h"

namespace wasp::game::systems {

	class DeathHandlerSystem {
	public:
		void operator()(Scene& scene);
	};
}