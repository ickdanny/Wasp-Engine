#pragma once

#include "Game/Systems/ShotType.h"
#include "Game/Systems/PlayerStates.h"

namespace wasp::game::components {

	struct PlayerData {

		//fields
		systems::ShotType shotType{};
		int lives{};
		int bombs{};
		int continues{};
		int power{};
		systems::PlayerStates playerState{};

		//Constructs a player data with the given parameters
		PlayerData(
			systems::ShotType shotType,
			int lives,
			int bombs,
			int continues,
			int power
		)
			: shotType{ shotType }
			, lives{ lives }
			, bombs{ bombs }
			, continues{ continues }
			, power{ power }
			, playerState{ systems::PlayerStates::none } {
		}

		//Default copy and move constructors
		PlayerData(const PlayerData& toCopy) = default;
		PlayerData(PlayerData&& toMove) = default;

		//Default copy and move assignment operators
		PlayerData& operator=(const PlayerData& toCopy) = default;
		PlayerData& operator=(PlayerData&& toMove) = default;
	};
}