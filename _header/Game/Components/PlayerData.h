#pragma once

#include "Game/Systems/ShotType.h"

namespace wasp::game::components {

	struct PlayerData {
		const systems::ShotType shotType{};
		int lives{};
		int bombs{};
		int continues{};
		int power{};
	};
}