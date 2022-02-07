#include "Graphics/Renderer.h"

namespace wasp::graphics {

	Renderer::~Renderer() {
		rendering.store(false);
		if (renderThread.joinable()) {
			renderThread.join();
		}
	}

	void Renderer::render(double dt) {

		static constexpr double smoothing{ 0.9 };

		static int frameCount{ 0 };

		static std::chrono::steady_clock::time_point lastDraw{};
		static std::chrono::steady_clock::time_point thisDraw{};

		static double timeToDraw{ 1.0 };
		static double fps{ 1.0 };

		if (rendering.load()) {
			return;
		}

		if (renderThread.joinable()) {
			renderThread.join();
		}
		rendering.store(true);
		//todo: pass config into constructor, and make getWindowHandle public
		windowPointer->getWindowPainter().beginDraw();
		windowPointer->getWindowPainter().drawSubBitmap(
			{ config::graphicsWidth / 2, config::graphicsHeight / 2 },
					{
						bitmapStoragePointer->get(L"timage")->d2dBitmap,
						45.0f,
						.8f,
						.7f
					},
			{ 100, 100, 600, 400 }
		);
		windowPointer->getWindowPainter().drawText(
			{ 20.0, 30.0 },
			{ std::to_wstring(frameCount++) },
			{ 300.0f, 500.0f }
		);
		windowPointer->getWindowPainter().drawText(
			{ 20.0f, 50.0f },
			{ std::to_wstring(1.0 / fps) },
			{ 400.0f, 300.0f }
		);
		windowPointer->getWindowPainter().endDraw();

		renderThread = std::thread{ [&] {
			windowPointer->getWindowPainter().paint(windowPointer->getWindowHandle());
			lastDraw = thisDraw;
			thisDraw = std::chrono::steady_clock::now();

			auto duration{ thisDraw - lastDraw };
			timeToDraw = duration.count()
				* static_cast<double>(std::chrono::steady_clock::period::num)
				/ std::chrono::steady_clock::period::den;

			fps = (fps * smoothing) + (timeToDraw * (1.0 - smoothing));
			rendering.store(false);
		} };
	}
}