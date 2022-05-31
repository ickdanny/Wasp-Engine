#pragma once

#include "Game/Scenes.h"
#include "Game/Systems/RenderSystem.h"
#include "Game/Systems/DebugRenderSystem.h"

namespace wasp::game {

	class SceneRenderer {
	private:
		//fields
		systems::RenderSystem renderSystem;				//not initialized!

		//debug fields
		#ifdef _DEBUG
		systems::DebugRenderSystem debugRenderSystem;	//not initialized!
		#endif

	public:

		SceneRenderer(window::WindowPainter* windowPainterPointer);

		void operator()(Scene& scene, float deltaTime);
	};
}