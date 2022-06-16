#pragma once

#include "systemInclude.h"

namespace wasp::game::systems {

	class PlayerBombSystem {
	private:
		//typedefs
		using EntityHandle = ecs::entity::EntityHandle;
	public:
		void operator()(Scene& scene);

	private:
		//helper functions
		void bomb(Scene& scene, const EntityHandle& playerHandle);
	};
}