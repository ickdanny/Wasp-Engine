#pragma once

#include "systemInclude.h"

namespace wasp::game::systems {

	class InboundSystem {
	public:
		void operator()(Scene& scene);
	};
}