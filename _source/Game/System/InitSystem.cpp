#include "Game/Systems/InitSystem.h"

#include <string>

#include "Logging.h"

namespace wasp::game::systems {

	namespace {
		constexpr math::Point2 center{
			config::graphicsWidth / 2,
			config::graphicsHeight / 2
		};
	}

	void InitSystem::operator()(Scene& scene) const {
		static Topic<> initFlag{};

		if (scene.getChannel(initFlag).isEmpty()) {
			scene.getChannel(initFlag).addMessage();
			initScene(scene);
		}
	}

	//helper functions

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
		addBackground(dataStorage, L"background_menu_main");

		constexpr float xOffset{ -10.0f };
		constexpr float yOffset{ 20.0f };
		constexpr float x{ 200.0f };
		constexpr float y{ 130.0f };
		constexpr float selXOffset{ 2.0f };
		constexpr float selYOffset{ -2.0f };

		auto buttonHandles{ 
			dataStorage.addEntities(
				makeButton(
					{ { x, y }, 
					{ x + selXOffset, y + selYOffset }, 
					L"button_start"},
					{ MenuCommandSelect::Commands::ENTER },
					{ SceneNames::difficulty },
					{ },	//draw order
					true
				).package(),
				makeButton(
					{ { x + xOffset, y + yOffset}, 
					{ x + xOffset + selXOffset, y + yOffset + selYOffset }, 
					L"button_practice" },
					{ MenuCommandSelect::Commands::ENTER },
					{ SceneNames::difficulty }
				).package(),
				makeButton(
					{ { x + 2 * xOffset, y + 2 * yOffset}, 
					{ x + 2 * xOffset + selXOffset, y + 2 * yOffset + selYOffset },
					L"button_music" },
					{ MenuCommandSelect::Commands::ENTER },
					{ SceneNames::music }
				).package(),
				makeButton(
					{ { x + 3 * xOffset, y + 3 * yOffset},
					{ x + 3 * xOffset + selXOffset, y + 3 * yOffset + selYOffset },
					L"button_option" },
					{ MenuCommandSelect::Commands::ENTER },
					{ SceneNames::options }
				).package(),
				makeButton(
					{ { x + 4 * xOffset, y + 4 * yOffset},
					{ x + 4 * xOffset + selXOffset, y + 4 * yOffset + selYOffset },
					L"button_quit" },
					{ MenuCommandSelect::Commands::ENTER }
				).package()
			) 
		};
		
		attachButtonsVertical(dataStorage, buttonHandles);
	}

	void InitSystem::addBackground(
		ecs::DataStorage& dataStorage, std::wstring name
	) const {
		dataStorage.addEntity(
			EntityBuilder::makeVisible(
				center,
				SpriteInstruction{
					bitmapStoragePointer->get(name)->d2dBitmap
				},
				DrawOrder{ -1000 }
			).package()
		);
	}

	//todo: should pass in a base point, offset vector, and then button index; p + iv
	ComponentTuple<
		Position,
		VisibleMarker,
		ButtonData,
		MenuCommandSelect,
		ButtonAction,
		SpriteInstruction,
		DrawOrder
	>
	InitSystem::makeButton(
		const ButtonData& buttonData,
		MenuCommandSelect selectCommand,
		const ButtonAction& buttonAction,
		DrawOrder drawOrder,
		bool selected
	) const {
		return EntityBuilder::makeVisible(
			selected ? buttonData.getSelPos() : buttonData.getUnselPos(),
			buttonData,
			selectCommand,
			buttonAction,
			SpriteInstruction{
				bitmapStoragePointer->get(
					selected ? buttonData.getSelImageName() 
					: buttonData.getUnselImageName()
				)->d2dBitmap
			},
			drawOrder
		);
	}

	void InitSystem::attachButtonsVertical(
		ecs::DataStorage& dataStorage,
		const std::vector<ecs::entity::EntityHandle>& entityHandles
	) const 
	{
		auto top{ entityHandles.begin() };
		auto bottom{ top + 1 };
		auto end{ entityHandles.end() };

		while (bottom != end) {
			dataStorage.addComponent(
				ecs::AddComponentOrder{ *top, NeighborElementDown{ *bottom } }
			);
			dataStorage.addComponent(
				ecs::AddComponentOrder{ *bottom, NeighborElementUp{ *top } }
			);
			++top;
			++bottom;
		}
	}

	void InitSystem::attachButtonsHorizontal(
		ecs::DataStorage& dataStorage,
		const std::vector<ecs::entity::EntityHandle>& entityHandles
	) const
	{
		auto left{ entityHandles.begin() };
		auto right{ left + 1 };
		auto end{ entityHandles.end() };

		while (right != end) {
			dataStorage.addComponent(
				ecs::AddComponentOrder{ *left, NeighborElementRight{ *right } }
			);
			dataStorage.addComponent(
				ecs::AddComponentOrder{ *right, NeighborElementLeft{ *left } }
			);
			++left;
			++right;
		}
	}
}