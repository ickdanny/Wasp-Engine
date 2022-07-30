#include "Game/Systems/InitSystem.h"

#include <string>

#include "Logging.h"

namespace wasp::game::systems {

	namespace {
		constexpr math::Point2 center{
			config::graphicsWidth / 2,
			config::graphicsHeight / 2
		};

		//middle X for scenes that overlay on top of the game screen
		constexpr float middleX{ 100.0f };

		int getInitPower(const GameState& gameState) {
			if (gameState.gameMode == GameMode::campaign) {
				return 0;
			}
			else {
				switch (gameState.stage) {
					case 1:
						return 0;
					case 2:
						return config::maxPower / 2;
					case 3:
					case 4:
					case 5:
						return config::maxPower;
					default:
						throw std::runtime_error{ "bad stage in getInitPower!" };
				}
			}
		}
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
			case SceneNames::load:
				initLoad(scene);
				break;
			case SceneNames::game:
				initGame(scene);
				break;
			case SceneNames::dialogue:
				initDialogue(scene);
				break;
			case SceneNames::pause:
				initPauseMenu(scene);
				break;
			case SceneNames::continues:
				initContinueMenu(scene);
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

		constexpr math::Point2 initPos{ 275.0f, 150.0f };
		constexpr math::Vector2 offset{ -10.0f, 18.0f };
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

		globalChannelSetPointer->getChannel(GlobalTopics::startMusic).addMessage(L"01");
	}

	void InitSystem::initDifficultyMenu(Scene& scene) const {
		auto& dataStorage{ scene.getDataStorage() };
		addBackground(dataStorage, L"background_menu_difficulty");

		constexpr math::Point2 initPos{ center.x, 85.0f };
		constexpr math::Vector2 offset{ 0.0f, 40.0f };
		constexpr math::Vector2 selOffset{ 0.0f, -1.0f };

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
		addBackground(
			dataStorage, 
			L"background_menu_shot",
			config::playerBulletDrawOrder - config::backgroundDrawOrder + 10
		);

		constexpr float buttonY{ 202.0f };
		constexpr float backY{ 123.0f };
		constexpr float playerY{ 130.0f };
		constexpr float xOffset{ 64.0f };

		addBackground(
			dataStorage, 
			L"background_menu_shot_back",
			-1000,
			math::Point2{ center.x + xOffset - 1, backY }
		);
		addBackground(
			dataStorage,
			L"background_menu_shot_back",
			-1000,
			math::Point2{ center.x - xOffset, backY }
		);

		//adding the player previews
		dataStorage.addEntity(
			EntityBuilder::makeVisible(
				math::Point2{ center.x - xOffset, playerY },
				SpriteInstruction{
					bitmapStoragePointer->get(L"p_idle_1")->d2dBitmap,
					math::Vector2{ 0.0f, 4.0f }			//offset
				},
				DrawOrder{ config::playerDrawOrder },
				SpawnProgramList{
					components::SpawnProgram{ 
						programsPointer->playerPrograms.shotAPreviewProgram
					}
				},
				AnimationList{ 
					components::Animation{ {
						L"p_idle_1", L"p_idle_2", L"p_idle_3", L"p_idle_4"
					} },
					4	//ticks
				}
			).package()
		);

		dataStorage.addEntity(
			EntityBuilder::makeVisible(
				math::Point2{ center.x + xOffset, playerY },
				SpriteInstruction{
					bitmapStoragePointer->get(L"p_idle_1")->d2dBitmap,
					math::Vector2{ 0.0f, 4.0f }			//offset
				},
				DrawOrder{ config::playerDrawOrder },
				SpawnProgramList{
					components::SpawnProgram{ 
						programsPointer->playerPrograms.shotBPreviewProgram
					}
				},
				AnimationList{ 
					components::Animation{ {
						L"p_idle_1", L"p_idle_2", L"p_idle_3", L"p_idle_4"
					} },
					4	//ticks
				}
			).package()
		);

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
					{ center.x - xOffset, buttonY },
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
					{ center.x + xOffset, buttonY },
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

		constexpr math::Point2 initPos{ center.x, 78.0f };
		constexpr math::Vector2 offset{ 0.0f, 31.0f };
		constexpr math::Vector2 selOffset{ 0.0f, -1.0f };

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

	void InitSystem::initLoad(Scene& scene) const {
		auto& dataStorage{ scene.getDataStorage() };

		addBackground(
			dataStorage,
			L"background_load",
			0,
			{ middleX, center.y }
		);
	}

	void InitSystem::initGame(Scene& scene) const {
		const auto& gameStateChannel{ 
			globalChannelSetPointer->getChannel(GlobalTopics::gameState) 
		};
		const auto& gameState{ gameStateChannel.getMessages()[0] };

		//create the prng
		scene.getChannel(SceneTopics::random).addMessage(
			config::PrngType{ gameState.prngSeed }
		);

		auto& dataStorage{ scene.getDataStorage() };
		addForeground(dataStorage, L"overlay_frame");

		//adding the player
		dataStorage.addEntity(
			EntityBuilder::makeStationaryCollidable(
				config::playerSpawn,
				config::playerHitbox,
				Velocity{},
				SpriteInstruction{
					bitmapStoragePointer->get(L"p_idle_1")->d2dBitmap,
					math::Vector2{ 0.0f, 4.0f }			//offset
				},
				DrawOrder{ config::playerDrawOrder },
				PlayerData{
					gameState.shotType,
					config::initLives,
					config::initBombs,
					config::initContinues,
					getInitPower(gameState)
				},
				Inbound{ config::playerInbound },
				PlayerCollisions::Target{ components::CollisionCommands::player },
				PickupCollisions::Target{},
				DeathCommand{ DeathCommand::Commands::playerDeath },
				SpawnProgramList{},
				DeathSpawn{ { programsPointer->playerPrograms.deathSpawnProgram } },
				AnimationList{ 
					{
						components::Animation{ {
							L"p_left_1", L"p_left_2", L"p_left_3", L"p_left_4"
						} },
						components::Animation{ {
							L"p_left_t_3"
						} },
						components::Animation{ {
							L"p_left_t_2"
						} },
						components::Animation{ {
							L"p_left_t_1"
						} },
						components::Animation{ {
							L"p_idle_1", L"p_idle_2", L"p_idle_3", L"p_idle_4"
						} },
						components::Animation{ {
							L"p_right_t_1"
						} },
						components::Animation{ {
							L"p_right_t_2"
						} },
						components::Animation{ {
							L"p_right_t_3"
						} },
						components::Animation{ {
							L"p_right_1", L"p_right_2", L"p_right_3", L"p_right_4"
						} }
					},
					4,	//idle index
					4	//ticks
				}
			).package()
		);

		//adding the spawner
		components::ScriptProgram const* scriptProgramPointer{};
		switch (gameState.stage) {
			case 1:
				scriptProgramPointer
					= &programsPointer->enemyPrograms.stage1ScriptProgram;
				break;
			case 2:
				scriptProgramPointer
					= &programsPointer->enemyPrograms.stage2ScriptProgram;
				break;
			case 3:
				scriptProgramPointer
					= &programsPointer->enemyPrograms.stage3ScriptProgram;
				break;
			case 4:
				scriptProgramPointer
					= &programsPointer->enemyPrograms.stage4ScriptProgram;
				break;
			default:
				throw std::runtime_error{ "no stage script!" };
		}

		dataStorage.addEntity(
			EntityBuilder::makeEntity(
				ScriptProgramList{ 
					{ *scriptProgramPointer }
				},
				SpawnProgramList{ }
			).package()
		);

		//adding the background
		constexpr int screenHeight{ 220 };
		constexpr int screenWidth{ 170 };
		std::wstring backgroundID{};
		int ticks{};
		switch (gameState.stage) {
			case 1:
				backgroundID = L"background_1";
				ticks = 60 * 90;
				break;
			case 2:
				backgroundID = L"background_1";
				ticks = 60 * 90;
				break;
			case 3:
				backgroundID = L"background_1";
				ticks = 60 * 90;
				break;
			case 4:
				backgroundID = L"background_4";
				ticks = 60 * 90;
				break;
			case 5:
				backgroundID = L"background_1";
				ticks = 60 * 90;
				break;
			default:
				throw std::runtime_error{ "unexpected default case " };
		}
		SpriteInstruction backgroundSprite{
			bitmapStoragePointer->get(backgroundID)->d2dBitmap
		};
		std::size_t totalHeight{ backgroundSprite.getBitmap()->GetPixelSize().height };
		int startingY{ static_cast<int>(totalHeight) - screenHeight };
		float yVelocity{ -startingY / static_cast<float>(ticks) };

		dataStorage.addEntity(
			EntityBuilder::makeVisible(
				math::Point2{ middleX, center.y }, 
				backgroundSprite,
				SubImage{ 
					0.0f, 
					static_cast<float>(startingY), 
					static_cast<float>(screenWidth), 
					static_cast<float>(screenHeight)
				},
				SubImageScroll{ 0.0f, yVelocity, 0.0f, 0.0f },
				DrawOrder{ config::backgroundDrawOrder }
			).package()
		);

		//starting stage track
		std::wstring trackName;
		switch (gameState.stage) {
			case 1:
				trackName = L"02";
				break;
			case 2:
				trackName = L"04";
				break;
			case 3:
				trackName = L"06";
				break;
			case 4: 
				trackName = L"08";
				break;
			case 5:
				trackName = L"10";
				break;
			default:
				throw std::runtime_error{ "unexpected default case reached!" };
		}
		globalChannelSetPointer->getChannel(GlobalTopics::startMusic)
			.addMessage(trackName);
	}

	void InitSystem::initDialogue(Scene& scene) const {
		scene.getDataStorage().addEntity(
			EntityBuilder::makeVisible(
				{ config::graphicsWidth / 2.0f, config::graphicsHeight - 40.0f },
				SpriteInstruction{
					bitmapStoragePointer->get(L"background_dialogue")->d2dBitmap,
					{},		//offset
					0.0f,	//rotation
					0.8f	//opacity
				},
				DrawOrder{ config::backgroundDrawOrder }
			).package()
		);
	}

	void InitSystem::initPauseMenu(Scene& scene) const {
		auto& dataStorage{ scene.getDataStorage() };

		addBackground(
			dataStorage,
			L"background_menu_pause", 
			0, 
			{ middleX, center.y }
		);

		constexpr math::Point2 initPos{ middleX, 100.0f };
		constexpr math::Vector2 offset{ 0.0f, 30.0f };
		constexpr math::Vector2 selOffset{ 0.0f, -2.0f };

		auto buttonHandles{
			dataStorage.addEntities(
				makeButton(
					initPos,
					offset,
					selOffset,
					0,
					L"button_resume",
					//the game cannot be paused during dialogue so this is safe
					{ MenuCommandSelect::Commands::backTo, SceneNames::game },
					{ },	//draw order
					true
				).package(),
				makeButton(
					initPos,
					offset,
					selOffset,
					1,
					L"button_retry",
					{ MenuCommandSelect::Commands::restartGame }
				).package(),
				makeButton(
					initPos,
					offset,
					selOffset,
					2,
					L"button_retire",
					{ MenuCommandSelect::Commands::gameOver }
				).package()
			)
		};

		attachButtonsVertical(dataStorage, buttonHandles);
		setInitSelectedElement(scene, buttonHandles[0]);

		scene.getChannel(SceneTopics::keyboardBackMenuCommand).addMessage(
			{ MenuCommandSelect::Commands::backTo, SceneNames::game }
		);
	}

	void InitSystem::initContinueMenu(Scene& scene) const {
		auto& dataStorage{ scene.getDataStorage() };

		addBackground(
			dataStorage,
			L"background_menu_continue",
			0,
			{ middleX, center.y });

		constexpr math::Point2 initPos{ middleX, 115.0f };
		constexpr math::Vector2 offset{ 0.0f, 30.0f };
		constexpr math::Vector2 selOffset{ 0.0f, -2.0f };

		auto buttonHandles{
			dataStorage.addEntities(
				makeButton(
					initPos,
					offset,
					selOffset,
					0,
					L"button_accept",
					{ MenuCommandSelect::Commands::backTo, SceneNames::game },
					{ },	//draw order
					true
				).package(),
				makeButton(
					initPos,
					offset,
					selOffset,
					1,
					L"button_decline",
					{ MenuCommandSelect::Commands::gameOver }
				).package()
			)
		};

		attachButtonsVertical(dataStorage, buttonHandles);
		setInitSelectedElement(scene, buttonHandles[0]);

		auto& playerDataChannel{
			globalChannelSetPointer->getChannel(GlobalTopics::playerData)
		};
		if (!playerDataChannel.hasMessages()) {
			throw std::runtime_error{ "no player data for init continue scene!" };
		}
		constexpr float iconY{ 80.0f };
		constexpr float xShift{ 15.0f };
		switch (playerDataChannel.getMessages()[0].continues) {
			case 2:
				addBackground(
					dataStorage,
					L"ui_continue",
					100,
					{ middleX + xShift, iconY }
				);
			case 1:
				addBackground(
					dataStorage,
					L"ui_continue",
					100,
					{ middleX - xShift, iconY }
				);
		}

		scene.getChannel(SceneTopics::keyboardBackMenuCommand).addMessage(
			{ components::MenuCommand::Commands::navFarDown }
		);
	}

	void InitSystem::addBackground(
		ecs::DataStorage& dataStorage, 
		const std::wstring& name,
		int relativeDrawOrder,
		const math::Point2& position
	) const {
		dataStorage.addEntity(
			EntityBuilder::makeVisible(
				position,
				SpriteInstruction{
					bitmapStoragePointer->get(name)->d2dBitmap
				},
				DrawOrder{ config::backgroundDrawOrder + relativeDrawOrder }
			).package()
		);
	}

	void InitSystem::addForeground(
		ecs::DataStorage& dataStorage, 
		const std::wstring& name,
		int relativeDrawOrder,
		const math::Point2& position
	) const {
		dataStorage.addEntity(
			EntityBuilder::makeVisible(
				position,
				SpriteInstruction{
					bitmapStoragePointer->get(name)->d2dBitmap
				},
				DrawOrder{ config::foregroundDrawOrder + relativeDrawOrder}
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