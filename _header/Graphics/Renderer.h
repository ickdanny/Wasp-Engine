#pragma once

#include <thread>
#include <atomic>

#include "Window/MainWindow.h"
#include "Game/Config.h"
#include "Game/GameResource/BitmapStorage.h"

//encapsulates the threading
//todo: probably need to call the systemChain... whenever that will be lol
namespace wasp::graphics {
	class Renderer {
	private:
		window::MainWindow* windowPointer{};
		game::gameresource::BitmapStorage* bitmapStoragePointer{};

		std::thread renderThread{};
		std::atomic_bool rendering{ false };

		int graphicsWidth{};
		int graphicsHeight{};

	public:
		Renderer(
			window::MainWindow* windowPointer, 
			game::gameresource::BitmapStorage* bitmapStoragePointer,
			int graphicsWidth,
			int graphicsHeight
		)
			: windowPointer{ windowPointer }
			, bitmapStoragePointer{ bitmapStoragePointer }
			, graphicsWidth{ graphicsWidth }
			, graphicsHeight{ graphicsHeight } {
		}

		~Renderer();

		void render(double dt);
	};
}