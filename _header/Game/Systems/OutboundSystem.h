#pragma once

#include "systemInclude.h"

namespace wasp::game::systems {

	class OutboundSystem {
	public:
		void operator()(Scene& scene);
	};
}