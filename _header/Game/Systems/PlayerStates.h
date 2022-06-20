#pragma once

namespace wasp::game::systems {
	enum class PlayerStates {
		none,
		normal,
		bombing,
		dead,
		respawning,
		respawnInvulnerable,
		gameOver
	};
}