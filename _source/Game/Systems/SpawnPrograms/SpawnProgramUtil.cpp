#include "Game/Systems/SpawnPrograms/SpawnProgramUtil.h"

namespace wasp::game::systems {
	const std::shared_ptr<components::SpawnNode> SpawnProgramUtil::randomAngleNode{
		makeUniformRandomFloatSpawnNode(
				makeFloatValueSpawnNode(0.0f),
				makeFloatValueSpawnNode(360.0f)
		)
	};
}