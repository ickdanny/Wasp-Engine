#include "Game/Systems/Programs/SpawnProgramUtil.h"

namespace wasp::game::systems {
	const std::shared_ptr<components::SpawnNode> SpawnProgramUtil::randomAngleNode{
		makeUniformRandomFloatNode(
				makeFloatValueNode(0.0f),
				makeFloatValueNode(360.0f)
		)
	};

	const std::shared_ptr<components::SpawnNode> SpawnProgramUtil::entityRandomAngleNode{
		makeEntityUniformRandomFloatNode(
				makeFloatValueNode(0.0f),
				makeFloatValueNode(360.0f)
		)
	};
}