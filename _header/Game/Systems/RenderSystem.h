#pragma once

#include "systemInclude.h"
#include "Window/WindowPainter.h"

namespace wasp::game::systems {

	class RenderSystem {
	private:
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
		
		void drawSprite(
			const Position& position,
			const SpriteInstruction& drawInstruction
		);
	};
}