#include "Game/Systems/RenderSystem.h"

namespace wasp::game::systems {

	//beginDraw and endDraw are called in the RenderScheduler
	void RenderSystem::operator()(Scene& scene, float deltaTime) {
		static const Topic<ecs::component::Group*> groupPointerStorageTopic{};

		auto groupPointer{
			getGroupPointer<Position, VisibleMarker, SpriteInstruction>(
				scene, groupPointerStorageTopic
			)
		};

		auto groupIterator{
			groupPointer->groupIterator<Position, SpriteInstruction>()
		};

		while (groupIterator.isValid()) {
			const auto& [position, spriteInstruction] = *groupIterator;
			drawSprite(position, spriteInstruction);
			++groupIterator;
		}
		//todo: delta time via velocity, keep track of pause shit?
		//todo: drawOrder
	}

	//helper functions

	void RenderSystem::drawSprite(
		const Position& position,
		const SpriteInstruction& drawInstruction
	) {
		windowPainterPointer->drawBitmap(position, drawInstruction);
	}
}