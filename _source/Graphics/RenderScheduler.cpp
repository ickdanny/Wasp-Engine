#include "Graphics/RenderScheduler.h"

namespace wasp::graphics {

	RendererScheduler::~RendererScheduler() {
		rendering.store(false);
		if (renderThread.joinable()) {
			renderThread.join();
		}
	}

	void RendererScheduler::render(float dt, RenderCallback& renderCallback) {
		//return if the render thread is currently rendering
		if (rendering.load()) {
			return;
		}
		//join the render thread if its still running despite rendering being false
		if (renderThread.joinable()) {
			renderThread.join();
		}
		//set rendering to true
		rendering.store(true);

		//draw the screen using the supplied callback
		windowPointer->getWindowPainter().beginDraw();
		renderCallback(dt);
		windowPointer->getWindowPainter().endDraw();

		//schedule a new render
		renderThread = std::thread{ 
			[&] {
				//at some point, paint the window and set rendering to false
				windowPointer->getWindowPainter().paint(
					windowPointer->getWindowHandle()
				);
				rendering.store(false);
			} 
		};
	}
}