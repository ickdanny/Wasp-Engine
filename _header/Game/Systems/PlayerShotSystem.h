#pragma once

#include "systemInclude.h"

#include "Game/Systems/SpawnPrograms/SpawnPrograms.h"

namespace wasp::game::systems {

	class PlayerShotSystem {
	private:
		//fields
		SpawnPrograms* spawnProgramsPointer{};

	public:
		PlayerShotSystem(SpawnPrograms* spawnProgramsPointer)
			: spawnProgramsPointer{ spawnProgramsPointer } {
		}

		void operator()(Scene& scene);

	private:
		//helper functions
		void addPlayerShot(Scene& scene);
	};
}