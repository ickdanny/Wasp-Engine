#pragma once

#include <thread>
#include <atomic>

#include "Window/MainWindow.h"
#include "Game/Config.h"
#include "Game/Resources/BitmapStorage.h"

//encapsulates the threading
namespace wasp::graphics {
	class RendererScheduler {
	public:
		//typedefs
		using RenderCallback = std::function<void(float)>;

	private:
		//fields
		window::MainWindow* windowPointer{};
		game::resources::BitmapStorage* bitmapStoragePointer{};

		std::thread renderThread{};
		std::atomic_bool rendering{ false };

		int graphicsWidth{};
		int graphicsHeight{};

	public:
		RendererScheduler(
			window::MainWindow* windowPointer, 
			game::resources::BitmapStorage* bitmapStoragePointer,
			int graphicsWidth,
			int graphicsHeight
		)
			: windowPointer{ windowPointer }
			, bitmapStoragePointer{ bitmapStoragePointer }
			, graphicsWidth{ graphicsWidth }
			, graphicsHeight{ graphicsHeight } {
		}

		~RendererScheduler();

		void render(float dt, RenderCallback& renderCallback);
	};
}