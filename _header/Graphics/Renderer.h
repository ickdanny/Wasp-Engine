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

	public:
		Renderer(
			window::MainWindow* windowPointer, 
			game::gameresource::BitmapStorage* bitmapStoragePointer
		)
			: windowPointer{ windowPointer }
			, bitmapStoragePointer{ bitmapStoragePointer }{
		}

		~Renderer();

		void render(double dt);
	};
}