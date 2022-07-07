#pragma once

#include "systemInclude.h"

#include "Game/Systems/SpawnPrograms/SpawnPrograms.h"

namespace wasp::game::systems {

	class PlayerBombSystem {
	private:
		//typedefs
		using EntityHandle = ecs::entity::EntityHandle;

		//fields
		SpawnPrograms* spawnProgramsPointer{};

	public:
		PlayerBombSystem(SpawnPrograms* spawnProgramsPointer)
			: spawnProgramsPointer{ spawnProgramsPointer } {
		}

		void operator()(Scene& scene);

	private:
		//helper functions
		void bomb(Scene& scene, const EntityHandle& playerHandle);
	};
}