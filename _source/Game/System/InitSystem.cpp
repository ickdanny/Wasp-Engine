#include "Game/Systems/InitSystem.h"

#include <string>

#include "Game/Systems/EntityBuilder.h"

#include "Logging.h"

namespace wasp::game::systems {

	void InitSystem::operator()(Scene& scene) const {
		static Topic<> initFlag{};

		if (scene.getChannel(initFlag).isEmpty()) {
			scene.getChannel(initFlag).addMessage();
			initScene(scene);
		}
	}

	//selects correct init function from the scene name
	void InitSystem::initScene(Scene& scene) const {
		switch (scene.getName()) {
			case SceneNames::main:
				initMainMenu(scene);
				break;
			default:
				int nameID{ static_cast<int>(scene.getName()) };
				debug::log("No init function for: " + std::to_string(nameID));
				break;
		}
	}

	void InitSystem::initMainMenu(Scene& scene) const {
		auto& dataStorage{ scene.getDataStorage() };
		dataStorage.addEntity(
			EntityBuilder::makeVisible(
				{ 0, 0 },
				SpriteInstruction{ bitmapStoragePointer->get(L"timage")->d2dBitmap }
			).package()
		);
	}
}