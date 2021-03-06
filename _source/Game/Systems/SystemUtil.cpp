#include "Game/Systems/SystemUtil.h"

namespace wasp::game::systems {

	//Returns true if the given position is outside of the bounds specified,
	//false otherwise
	bool isOutOfBounds(math::Point2 pos, float bound) {
		float lowXBound = bound + config::gameOffset.x;
		float lowYBound = bound + config::gameOffset.y;
		float highXBound = config::gameWidth - bound + config::gameOffset.x;
		float highYBound = config::gameHeight - bound + config::gameOffset.y;
		return pos.x < lowXBound
			|| pos.x > highXBound
			|| pos.y < lowYBound
			|| pos.y > highYBound;
	}
}