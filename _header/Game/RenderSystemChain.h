#pragma once

#include "Game/Scenes.h"
#include "Game/Systems/RenderSystem.h"
#include "Game/Systems/DebugRenderSystem.h"

namespace wasp::game {

	class RenderSystemChain {
	private:
		//fields
		systems::RenderSystem renderSystem;				//not initialized!

		//debug fields
		#ifdef _DEBUG
		systems::DebugRenderSystem debugRenderSystem;	//not initialized!
		#endif

	public:

		RenderSystemChain(window::WindowPainter* windowPainterPointer)
			: renderSystem{ windowPainterPointer } 

			#ifdef _DEBUG
			, debugRenderSystem{ windowPainterPointer }
			#endif
		{
		}

		void operator()(Scene& scene, float deltaTime) {
			renderSystem(scene, deltaTime);

			#ifdef _DEBUG
			debugRenderSystem(scene, deltaTime);
			#endif
		}
	};
}