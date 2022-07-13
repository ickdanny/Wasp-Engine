#include "Game/Systems/SpawnPrograms/SpawnProgramUtil.h"

namespace wasp::game::systems {
	const std::shared_ptr<components::SpawnNode> SpawnProgramUtil::randomAngleNode{
		makeUniformRandomFloatNode(
				makeFloatValueNode(0.0f),
				makeFloatValueNode(360.0f)
		)
	};
}