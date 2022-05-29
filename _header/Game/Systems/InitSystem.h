#pragma once

#include "systemInclude.h"
#include "Game/Resources/BitmapStorage.h"

namespace wasp::game::systems {

	class InitSystem {
	private:
		resources::BitmapStorage* bitmapStoragePointer{};

	public:
		InitSystem(resources::BitmapStorage* bitmapStoragePointer)
			: bitmapStoragePointer{ bitmapStoragePointer } {
		}

		void operator()(Scene& scene) const;

		//helper functions
	private:
		void initScene(Scene& scene) const;
		void initMainMenu(Scene& scene) const;
	};
}