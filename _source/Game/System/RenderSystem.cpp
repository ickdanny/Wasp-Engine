#include "Game/Systems/RenderSystem.h"

#include <algorithm>

namespace wasp::game::systems {

	//NOTE: if operator() is causing slowdowns, can hook up callbacks to dataStorage
	//i.e. add, remove component topics

	//beginDraw and endDraw are called in the RenderScheduler
	void RenderSystem::operator()(Scene& scene, float deltaTime) {

		auto& lastDeltaTimeChannel{ scene.getChannel(SceneTopics::lastDeltaTime) };

		static const Topic<ecs::component::Group*> groupPointerStorageTopic{};

		auto groupPointer{
			getGroupPointer<Position, VisibleMarker, SpriteInstruction, DrawOrder>(
				scene, 
				groupPointerStorageTopic
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

		if (scene.hasChannel(SceneTopics::pauseFlag)) {
			if (!scene.getChannel(SceneTopics::pauseFlag).hasMessages()) {
				drawSpritesFromList(entityIDList, dataStorage, deltaTime);
			}
			//if there is a pause flag raised, draw the last stored deltaTime
			else {
				const float lastDeltaTime{ lastDeltaTimeChannel.getMessages()[0] };
				drawSpritesFromList(entityIDList, dataStorage, lastDeltaTime);
			}
		}
		else {
			drawSpritesFromList(entityIDList, dataStorage, deltaTime);
		}

		lastDeltaTimeChannel.clear();
		lastDeltaTimeChannel.addMessage(deltaTime);
	}

	//helper functions

	void RenderSystem::drawSpritesFromList(
		const std::vector<IDOrderTuple>& entityIDList, 
		const ecs::DataStorage& dataStorage,
		float deltaTime
	) {
		for (const auto& [entityID, unused] : entityIDList) {
			const auto& position{ dataStorage.getComponent<Position>(entityID) };
			const auto& spriteInstruction{
				dataStorage.getComponent<SpriteInstruction>(entityID)
			};

			if (dataStorage.containsComponent<Velocity>(entityID)) {
				const auto& velocity{
					dataStorage.getComponent<Velocity>(entityID)
				};
				const auto& lerpPosition{ position + velocity * deltaTime };
				drawSprite(lerpPosition, spriteInstruction);
			}
			else {
				drawSprite(position, spriteInstruction);
			}
		}
	}

	void RenderSystem::drawSprite(
		const Position& position,
		const SpriteInstruction& drawInstruction
	) {
		windowPainterPointer->drawBitmap(position, drawInstruction);
	}
}