#pragma once

#include "systemInclude.h"

namespace wasp::game::systems {

	class SpawnSystem {

	public:
		void operator()(Scene& scene);
	};
}