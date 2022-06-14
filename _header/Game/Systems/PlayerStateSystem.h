#pragma once

#include "systemInclude.h"

namespace wasp::game::systems {

	class PlayerStateSystem {
	public:
		void operator()(Scene& scene);
	};
}