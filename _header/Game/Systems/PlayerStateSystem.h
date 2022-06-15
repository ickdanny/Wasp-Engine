#pragma once

#include "systemInclude.h"

namespace wasp::game::systems {

	class PlayerStateSystem {
	private:
		//typedefs
		using EntityHandle = ecs::entity::EntityHandle;

	public:
		void operator()(Scene& scene);
	};
}