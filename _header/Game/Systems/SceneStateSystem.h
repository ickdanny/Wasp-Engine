#pragma once

#include "systemInclude.h"

namespace wasp::game::systems {

	class SceneStateSystem {
	public:
		void operator()(Scene& scene);
	};
}