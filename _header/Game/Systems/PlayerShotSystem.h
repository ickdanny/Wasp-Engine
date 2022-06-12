#pragma once

#include "systemInclude.h"

namespace wasp::game::systems {

	class PlayerShotSystem {

	public:
		void operator()(Scene& scene);

	private:
		//helper functions
		void addPlayerShot(Scene& scene);
	};
}