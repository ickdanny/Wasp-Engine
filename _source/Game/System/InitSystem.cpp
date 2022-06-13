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
			case SceneNames::difficulty:
				initDifficultyMenu(scene);
				break;
			case SceneNames::shot:
				initShotMenu(scene);
				break;
			case SceneNames::stage:
				initStageMenu(scene);
				break;
			case SceneNames::music:
				throw "no music menu yet";
				break;
			case SceneNames::options:
				initOptionsMenu(scene);
				break;
			case SceneNames::game:
				initGame(scene);
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
					{ 
						MenuCommandSelect::Commands::enter, 
						std::tuple{ SceneNames::difficulty, GameBuilderCommands::start }
					},
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
						std::tuple{ 
							SceneNames::difficulty, 
							GameBuilderCommands::practice 
						}
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

		scene.getChannel(SceneTopics::keyboardBackMenuCommand).addMessage(
			{ components::MenuCommand::Commands::navFarDown }
		);
	}

	void InitSystem::initDifficultyMenu(Scene& scene) const {
		auto& dataStorage{ scene.getDataStorage() };
		addBackground(dataStorage, L"background_menu_difficulty");

		constexpr math::Point2 initPos{ center.x, 80.0f };
		constexpr math::Vector2 offset{ 0.0f, 40.0f };
		constexpr math::Vector2 selOffset{ 0.0f, -2.0f };

		auto buttonHandles{
			dataStorage.addEntities(
				makeButton(
					initPos,
					offset,
					selOffset,
					0,
					L"button_easy",
					{
						MenuCommandSelect::Commands::enter,
						std::tuple{ SceneNames::shot, GameBuilderCommands::easy }
					},
					{ },	//draw order
					true
				).package(),
				makeButton(
					initPos,
					offset,
					selOffset,
					1,
					L"button_normal",
					{
						MenuCommandSelect::Commands::enter,
						std::tuple{ SceneNames::shot, GameBuilderCommands::normal }
					}
				).package(),
				makeButton(
					initPos,
					offset,
					selOffset,
					2,
					L"button_hard",
					{ 
						MenuCommandSelect::Commands::enter, 
						std::tuple{ SceneNames::shot, GameBuilderCommands::hard}
					}
				).package(),
				makeButton(
					initPos,
					offset,
					selOffset,
					3,
					L"button_lunatic",
					{ 
						MenuCommandSelect::Commands::enter, 
						std::tuple{ SceneNames::shot, GameBuilderCommands::lunatic }
					}
				).package()
			)
		};

		attachButtonsVertical(dataStorage, buttonHandles);
		setInitSelectedElement(scene, buttonHandles[0]);

		scene.getChannel(SceneTopics::keyboardBackMenuCommand).addMessage(
			{ 
				components::MenuCommand::Commands::backTo, 
				SceneNames::main
			}
		);
	}

	void InitSystem::initShotMenu(Scene& scene) const {
		auto& dataStorage{ scene.getDataStorage() };
		addBackground(dataStorage, L"background_menu_shot");

		constexpr float y{ 130.0f };
		constexpr float xOffset{ 120.0f };

		SceneNames nextScene;

		const auto& gameStateChannel{
			globalChannelSetPointer->getChannel(GlobalTopics::gameState)
		};
		if (gameStateChannel.hasMessages()) {
			if (gameStateChannel.getMessages()[0].gameMode == GameMode::campaign) {
				nextScene = SceneNames::game;
			}
			else {
				nextScene = SceneNames::stage;
			}
		}
		else {
			throw std::runtime_error{ "no game state for init shot menu!" };
		}

		auto buttonHandles{
			dataStorage.addEntities(
				makeButton(
					{ center.x - xOffset, y },
					{ },	//selOffset
					L"button_a",
					{
						MenuCommandSelect::Commands::enter,
						std::tuple{ nextScene, GameBuilderCommands::shotA }
					},
					{ },	//draw order
					true
				).package(),
				makeButton(
					{ center.x + xOffset, y },
					{ },	//selOffset
					L"button_b",
					{
						MenuCommandSelect::Commands::enter,
						std::tuple{ nextScene, GameBuilderCommands::shotB }
					}
				).package()
			)
		};

		attachButtonsHorizontal(dataStorage, buttonHandles);
		setInitSelectedElement(scene, buttonHandles[0]);

		scene.getChannel(SceneTopics::keyboardBackMenuCommand).addMessage(
			{ 
				components::MenuCommand::Commands::backTo,
				SceneNames::difficulty
			}
		);
	}

	void InitSystem::initStageMenu(Scene& scene) const {
		auto& dataStorage{ scene.getDataStorage() };
		addBackground(dataStorage, L"background_menu_stage");

		constexpr math::Point2 initPos{ center.x, 70.0f };
		constexpr math::Vector2 offset{ 0.0f, 35.0f };
		constexpr math::Vector2 selOffset{ 0.0f, -2.0f };

		auto buttonHandles{
			dataStorage.addEntities(
				makeButton(
					initPos,
					offset,
					selOffset,
					0,
					L"button_stage1",
					{
						MenuCommandSelect::Commands::enter,
						std::tuple{ SceneNames::game, GameBuilderCommands::stage1 }
					},
					{ },	//draw order
					true
				).package(),
				makeButton(
					initPos,
					offset,
					selOffset,
					1,
					L"button_stage2",
					{
						MenuCommandSelect::Commands::enter,
						std::tuple{ SceneNames::game, GameBuilderCommands::stage2 }
					}
				).package(),
				makeButton(
					initPos,
					offset,
					selOffset,
					2,
					L"button_stage3",
					{
						MenuCommandSelect::Commands::enter,
						std::tuple{ SceneNames::game, GameBuilderCommands::stage3}
					}
				).package(),
				makeButton(
					initPos,
					offset,
					selOffset,
					3,
					L"button_stage4",
					{
						MenuCommandSelect::Commands::enter,
						std::tuple{ SceneNames::game, GameBuilderCommands::stage4 }
					}
				).package(),
				makeButton(
					initPos,
					offset,
					selOffset,
					4,
					L"button_stage5",
					{
						MenuCommandSelect::Commands::enter,
						std::tuple{ SceneNames::game, GameBuilderCommands::stage5 }
					}
				).package()
			)
		};

		attachButtonsVertical(dataStorage, buttonHandles);
		setInitSelectedElement(scene, buttonHandles[0]);

		scene.getChannel(SceneTopics::keyboardBackMenuCommand).addMessage(
			{
				components::MenuCommand::Commands::backTo,
				SceneNames::shot
			}
		);
	}

	void InitSystem::initOptionsMenu(Scene& scene) const {
		auto& dataStorage{ scene.getDataStorage() };
		addBackground(dataStorage, L"background_menu_option");

		constexpr math::Point2 initPos{ center.x, 85.0f };
		constexpr math::Vector2 offset{ 0.0f, 55.0f };
		constexpr math::Vector2 selOffset{ 0.0f, -2.0f };

		auto buttonHandles{
			dataStorage.addEntities(
				makeButton(
					initPos,
					offset,
					selOffset,
					0,
					L"button_sound",
					{ MenuCommandSelect::Commands::toggleSound },
					{ },	//draw order
					true
				).package(),
				makeButton(
					initPos,
					offset,
					selOffset,
					1,
					L"button_fullscreen",
					{ MenuCommandSelect::Commands::toggleFullscreen }
				).package(),
				makeButton(
					initPos,
					offset,
					selOffset,
					2,
					L"button_option_exit",
					{ MenuCommandSelect::Commands::backAndWriteSettings }
				).package()
			)
		};

		attachButtonsVertical(dataStorage, buttonHandles);
		setInitSelectedElement(scene, buttonHandles[0]);

		scene.getChannel(SceneTopics::keyboardBackMenuCommand).addMessage(
			{ components::MenuCommand::Commands::navFarDown }
		);
	}

	void InitSystem::initGame(Scene& scene) const {
		const auto& gameStateChannel{ 
			globalChannelSetPointer->getChannel(GlobalTopics::gameState) 
		};
		const auto& gameState{ gameStateChannel.getMessages()[0] };

		auto& dataStorage{ scene.getDataStorage() };
		addBackground(dataStorage, L"temp_background_game");

		dataStorage.addEntity(
			EntityBuilder::makeStationaryCollidable(
				config::playerSpawn,
				config::playerHitbox,
				Velocity{},
				SpriteInstruction{
					bitmapStoragePointer->get(L"temp_player")->d2dBitmap
				},
				DrawOrder{ config::playerDrawOrder },
				PlayerData{
					gameState.shotType,
					config::initLives,
					config::initBombs,
					config::initContinues,
					0	//todo: init power
				},
				Inbound{ config::playerInbound }
				//todo:spawn, damage, death, animation
			).package()
		);
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
				DrawOrder{ config::backgroundDrawOrder }
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
		return makeButton(unselPos, selOffset, name, selectCommand, drawOrder, selected);
	}

	InitSystem::BasicButtonComponentTuple InitSystem::makeButton(
		const math::Point2& unselPos,
		const math::Vector2& selOffset,
		const std::wstring& name,
		MenuCommandSelect selectCommand,
		DrawOrder drawOrder,
		bool selected
	) const {
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
						components::MenuCommand::Commands::navDown
					}
				}
			);
			dataStorage.addComponent(
				ecs::AddComponentOrder{
					*bottom,
					MenuCommandUp{
						components::MenuCommand::Commands::navUp
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
						components::MenuCommand::Commands::navRight
					}
				}
			);
			dataStorage.addComponent(
				ecs::AddComponentOrder{
					*right,
					MenuCommandLeft{
						components::MenuCommand::Commands::navLeft
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