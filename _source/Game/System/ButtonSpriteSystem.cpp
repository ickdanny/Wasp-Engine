#include "Game/Systems/ButtonSpriteSystem.h"

namespace wasp::game::systems {

	ButtonSpriteSystem::ButtonSpriteSystem(
		resources::BitmapStorage* bitmapStoragePointer
	)
		: bitmapStoragePointer{ bitmapStoragePointer } {
	}

	void ButtonSpriteSystem::operator()(Scene& scene) {
		auto& elementSelectionChannel{ scene.getChannel(SceneTopics::elementSelection) };
		if (elementSelectionChannel.hasMessages()) {
			for (auto& [entityHandle, select] : elementSelectionChannel.getMessages()) {
				DataStorage& dataStorage{ scene.getDataStorage() };
				if (dataStorage.containsAllComponents<
					ButtonData,
					SpriteInstruction,
					Position
				>(entityHandle)) 
				{
					if (select) {
						selectButton(dataStorage, entityHandle);
					}
					else {
						unselectButton(dataStorage, entityHandle);
					}
				}
			}
		}
	}

	void ButtonSpriteSystem::selectButton(
		DataStorage& dataStorage, 
		const EntityHandle& buttonHandle
	) {
		ButtonData& buttonData{ dataStorage.getComponent<ButtonData>(buttonHandle) };
		changeSpriteAndPosition(
			dataStorage,
			buttonHandle,
			buttonData.getSelImageName(),
			buttonData.getSelPos()
		);
	}

	void ButtonSpriteSystem::unselectButton(
		DataStorage& dataStorage,
		const EntityHandle& buttonHandle
	) {
		ButtonData& buttonData{ dataStorage.getComponent<ButtonData>(buttonHandle) };
		changeSpriteAndPosition(
			dataStorage,
			buttonHandle,
			buttonData.getUnselImageName(),
			buttonData.getUnselPos()
		);
	}

	void ButtonSpriteSystem::changeSpriteAndPosition(
		DataStorage& dataStorage,
		const EntityHandle& buttonHandle,
		const std::wstring& spriteName,
		const math::Point2& position
	) {
		dataStorage.getComponent<SpriteInstruction>(buttonHandle).setBitmap(
			bitmapStoragePointer->get(spriteName)->d2dBitmap
		);
		dataStorage.getComponent<Position>(buttonHandle) = position;
	}
}