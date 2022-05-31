#include "Game/GameRenderer.h"

namespace wasp::game {

	SceneRenderer::SceneRenderer(window::WindowPainter* windowPainterPointer)
		: renderSystem{ windowPainterPointer }

		#ifdef _DEBUG
		, debugRenderSystem{ windowPainterPointer }
		#endif
	{
	}

	void SceneRenderer::operator()(Scene& scene, float deltaTime) {
		renderSystem(scene, deltaTime);

		#ifdef _DEBUG
		debugRenderSystem(scene, deltaTime);
		#endif
	}
}