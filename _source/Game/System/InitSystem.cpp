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
			case SceneNames::options:
				initOptionsMenu(scene);
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

		constexpr math::Point2 initPos{ 200.0f, 130.0f };
		constexpr math::Vector2 offset{ -10.0f, 20.0f };
		constexpr math::Vector2 selOffset{ 2.0f, -2.0f };

		auto buttonHandles{ 
			dataStorage.addEntities(
				makeButton(
					initPos, 
					offset,
					selOffset,
					0,
					L"button_start",
					{ MenuCommandSelect::Commands::enter, SceneNames::start_difficulty },
					{ },	//draw order
					true
				).package(),
				makeButton(
					initPos,
					offset,
					selOffset,
					1,
					L"button_practice",
					{ 
						MenuCommandSelect::Commands::enter, 
						SceneNames::practice_difficulty 
					}
				).package(),
				makeButton(
					initPos,
					offset,
					selOffset,
					2,
					L"button_music",
					{ MenuCommandSelect::Commands::enter, SceneNames::music }
				).package(),
				makeButton(
					initPos,
					offset,
					selOffset,
					3,
					L"button_option",
					{ MenuCommandSelect::Commands::enter, SceneNames::options }
				).package(),
				makeButton(
					initPos,
					offset,
					selOffset,
					4,
					L"button_quit",
					{ MenuCommandSelect::Commands::exit }
				).package()
			)
		};
		
		attachButtonsVertical(dataStorage, buttonHandles);
		setInitSelectedElement(scene, buttonHandles[0]);
	}

	void InitSystem::initOptionsMenu(Scene& scene) const {

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

	InitSystem::BasicButtonComponentTuple InitSystem::makeButton(
		const math::Point2& initPos,
		const math::Vector2& offset,
		const math::Vector2& selOffset,
		int index,
		const std::wstring& name,
		MenuCommandSelect selectCommand,
		DrawOrder drawOrder,
		bool selected
	) const {
		math::Point2 unselPos{ initPos + (offset * static_cast<float>(index)) };
		math::Point2 selPos{ unselPos + selOffset };
		ButtonData buttonData{ unselPos, selPos, name };

		return EntityBuilder::makeVisible(
			selected ? buttonData.getSelPos() : buttonData.getUnselPos(),
			buttonData,
			selectCommand,
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
		const std::vector<EntityHandle>& entityHandles
	) const 
	{
		auto top{ entityHandles.begin() };
		auto bottom{ top + 1 };
		auto end{ entityHandles.end() };

		while (bottom != end) {
			//hook up neighbor elements
			dataStorage.addComponent(
				ecs::AddComponentOrder{ *top, NeighborElementDown{ *bottom } }
			);
			dataStorage.addComponent(
				ecs::AddComponentOrder{ *bottom, NeighborElementUp{ *top } }
			);
			//hook up arrow key menu commands
			dataStorage.addComponent(
				ecs::AddComponentOrder{
					*top,
					MenuCommandDown{
						components::MenuCommand::Commands::nav_down
					}
				}
			);
			dataStorage.addComponent(
				ecs::AddComponentOrder{
					*bottom,
					MenuCommandUp{
						components::MenuCommand::Commands::nav_up
					}
				}
			);
			++top;
			++bottom;
		}
	}

	void InitSystem::attachButtonsHorizontal(
		ecs::DataStorage& dataStorage,
		const std::vector<EntityHandle>& entityHandles
	) const
	{
		auto left{ entityHandles.begin() };
		auto right{ left + 1 };
		auto end{ entityHandles.end() };

		while (right != end) {
			//hook up neighbor elements
			dataStorage.addComponent(
				ecs::AddComponentOrder{ *left, NeighborElementRight{ *right } }
			);
			dataStorage.addComponent(
				ecs::AddComponentOrder{ *right, NeighborElementLeft{ *left } }
			);
			//hook up arrow key menu commands
			dataStorage.addComponent(
				ecs::AddComponentOrder{
					*left,
					MenuCommandRight{
						components::MenuCommand::Commands::nav_right
					}
				}
			);
			dataStorage.addComponent(
				ecs::AddComponentOrder{
					*right,
					MenuCommandLeft{
						components::MenuCommand::Commands::nav_left
					}
				}
			);
			++left;
			++right;
		}
	}

	void InitSystem::setInitSelectedElement(
		Scene& scene,
		const EntityHandle& entityHandle
	) const {
		auto& currentSelectedElementChannel{
			scene.getChannel(SceneTopics::currentSelectedElement)
		};
		if (currentSelectedElementChannel.isEmpty()) {
			currentSelectedElementChannel.addMessage(entityHandle);
		}
		else {
			throw std::runtime_error{ "existing selected element in initSystem!" };
		}
	}
}