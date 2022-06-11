#pragma once

namespace wasp::game::systems {

	enum class PlayerStates {
		normal,
		bombing,
		dead,
		checkContinue,
		respawning,
		respawnInvulnerable,
		gameOver
	};
}