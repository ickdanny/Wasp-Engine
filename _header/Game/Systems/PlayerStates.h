#pragma once

namespace wasp::game::systems {

	//todo: check continue system reads dead or respawning and starts it's own timer
	enum class PlayerStates {
		normal,
		bombing,
		dead,
		respawning,
		respawnInvulnerable,
		gameOver
	};
}