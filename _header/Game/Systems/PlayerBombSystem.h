#pragma once

#include "systemInclude.h"

namespace wasp::game::systems {

	class PlayerBombSystem {
	public:
		void operator()(Scene& scene);
	};
}