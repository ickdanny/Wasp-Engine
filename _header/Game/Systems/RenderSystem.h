#pragma once

#include "systemInclude.h"
#include "Window/WindowPainter.h"

namespace wasp::game::systems {

	class RenderSystem {
	private:
		//typedefs
		using EntityID = ecs::entity::EntityID;
		using IDOrderTuple = std::tuple<EntityID, DrawOrder>;

		//fields
		window::WindowPainter* windowPainterPointer{};

	public:
		RenderSystem(window::WindowPainter* windowPainterPointer)
			: windowPainterPointer{ windowPainterPointer } {
		}

		//beginDraw and endDraw are called in the RenderScheduler
		void operator()(Scene& scene, float deltaTime);

	private:
		//helper functions
		void drawSpritesFromList(
			const std::vector<IDOrderTuple>& entityIDList,
			const ecs::DataStorage& dataStorage,
			float deltaTime
		);
		
		void drawSprite(
			const Position& position,
			const SpriteInstruction& drawInstruction
		);

		void drawSprite(
			const Position& position,
			const SpriteInstruction& drawInstruction,
			const SubImage& subImage
		);
	};
}