#include "Game/Systems/RenderSystem.h"

#include <algorithm>

namespace wasp::game::systems {

	//NOTE: if operator() is causing slowdowns, can hook up callbacks to dataStorage
	//i.e. add, remove component topics

	//beginDraw and endDraw are called in the RenderScheduler
	void RenderSystem::operator()(Scene& scene, float deltaTime) {
		static const Topic<ecs::component::Group*> groupPointerStorageTopic{};

		auto groupPointer{
			getGroupPointer<Position, VisibleMarker, SpriteInstruction, DrawOrder>(
				scene, groupPointerStorageTopic
			)
		};

		//first, sort all drawable entityIDs by their drawOrder
		std::vector<IDOrderTuple> entityIDList;
		auto drawOrderIterator{ groupPointer->groupIterator<DrawOrder>() };
		while (drawOrderIterator.isValid()) {
			const auto [drawOrder] = *drawOrderIterator;
			auto entityID{ drawOrderIterator.getEntityID() };
			entityIDList.push_back({ entityID, drawOrder });
			++drawOrderIterator;
		}
		std::sort(
			entityIDList.begin(),
			entityIDList.end(),
			[](const IDOrderTuple& a, const IDOrderTuple& b) {
				return std::get<1>(a).order < std::get<1>(b).order;
			}
		);

		//next, draw all the entities in order
		auto& dataStorage{ scene.getDataStorage() };
		for (const auto& [entityID, unused] : entityIDList) {
			const auto& position{ dataStorage.getComponent<Position>(entityID) };
			const auto& spriteInstruction{
				dataStorage.getComponent<SpriteInstruction>(entityID)
			};
			drawSprite(position, spriteInstruction);
		}

		//todo: delta time via velocity, keep track of pause shit?
	}

	//helper functions

	void RenderSystem::drawSprite(
		const Position& position,
		const SpriteInstruction& drawInstruction
	) {
		windowPainterPointer->drawBitmap(position, drawInstruction);
	}
}