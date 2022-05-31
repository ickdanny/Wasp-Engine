#include "Game/Systems/MenuNavigationSystem.h"

namespace wasp::game::systems {

	void MenuNavigationSystem::operator()(Scene& scene) {
		//todo: clear elementSelect channel
		//if this scene has a selected element (in other words, if this scene is a menu)
		auto& currentSelectedElementChannel{
			scene.getChannel(SceneTopics::currentSelectedElement)
		};
		if (currentSelectedElementChannel.hasMessage()) {
			auto& menuNavigationCommandChannel{
				scene.getChannel(SceneTopics::menuNavigationCommands)
			};

			//and if this scene has any menu navigation commands to parse
			if (menuNavigationCommandChannel.hasMessage()) {

				//grab the selected element
				EntityHandle currentSelectedElement{
					currentSelectedElementChannel.getMessages()[0]
				};

				//and start parsing commands
				for (auto menuNavigationCommand 
					: menuNavigationCommandChannel.getMessages()) 
				{
					//todo: read the bool return value!
					parseNavigationCommand(
						scene, 
						menuNavigationCommand, 
						currentSelectedElement
					);
				}
			}
		}
	}

	bool MenuNavigationSystem::parseNavigationCommand(
		Scene& scene,
		MenuNavigationCommands menuNavigationCommand,
		const EntityHandle& currentSelectedElement
	) {
		DataStorage& dataStorage{ scene.getDataStorage() };

		MenuCommand menuCommand;	//uninitialized!
		switch (menuNavigationCommand) {
			case MenuNavigationCommands::back:
				menuCommand = getKeyboardBackMenuCommand(scene);
				break;
			case MenuNavigationCommands::select:
				menuCommand = getMenuCommand<MenuCommandSelect>(
					dataStorage, 
					currentSelectedElement
				);
				break;
			case MenuNavigationCommands::up:
				menuCommand = getMenuCommand<MenuCommandUp>(
					dataStorage,
					currentSelectedElement
				);
				break;
			case MenuNavigationCommands::down:
				menuCommand = getMenuCommand<MenuCommandDown>(
					dataStorage,
					currentSelectedElement
				);
				break;
			case MenuNavigationCommands::left:
				menuCommand = getMenuCommand<MenuCommandLeft>(
					dataStorage,
					currentSelectedElement
				);
				break;
			case MenuNavigationCommands::right:
				menuCommand = getMenuCommand<MenuCommandRight>(
					dataStorage,
					currentSelectedElement
				);
				break;
			default:
				throw std::runtime_error{ "reached default case in MenuNavSystem" };
		}
		return parseMenuCommand(scene, menuCommand, currentSelectedElement);
	}

	const components::MenuCommand& MenuNavigationSystem::getKeyboardBackMenuCommand(
		Scene& scene
	) {
		auto& keyboardBackMenuCommandChannel{
			scene.getChannel(SceneTopics::keyboardBackMenuCommand)
		};
		if (keyboardBackMenuCommandChannel.hasMessage()) {
			return keyboardBackMenuCommandChannel.getMessages()[0];
		}
		return { MenuCommand::none };
	}

	//true if we can continue parsing navigation commands, false if critical
	bool MenuNavigationSystem::parseMenuCommand(
		Scene& scene,
		const MenuCommand& menuCommand,
		const EntityHandle& currentSelectedElement
	) {
		switch (menuCommand.command) {
			//menu navigation
			case MenuCommand::Commands::nav_up:
				handleNavCommand<NeighborElementUp>(scene, currentSelectedElement);
				return true;
			case MenuCommand::Commands::nav_down:
				handleNavCommand<NeighborElementDown>(scene, currentSelectedElement);
				return true;
			case MenuCommand::Commands::nav_left:
				handleNavCommand<NeighborElementLeft>(scene, currentSelectedElement);
				return true;
			case MenuCommand::Commands::nav_right:
				handleNavCommand<NeighborElementRight>(scene, currentSelectedElement);
				return true;
			case MenuCommand::Commands::nav_far_up:
				handleNavFarCommand<NeighborElementUp>(scene, currentSelectedElement);
				return true;
			case MenuCommand::Commands::nav_far_down:
				handleNavFarCommand<NeighborElementDown>(scene, currentSelectedElement);
				return true;
			case MenuCommand::Commands::nav_far_left:
				handleNavFarCommand<NeighborElementLeft>(scene, currentSelectedElement);
				return true;
			case MenuCommand::Commands::nav_far_right:
				handleNavFarCommand<NeighborElementRight>(
					scene, 
					currentSelectedElement
				);
				return true;

			//scene navigation and variants thereof
			case MenuCommand::Commands::enter_and_stop_music:
				handleStopMusic();
			case MenuCommand::Commands::enter:
				handleEnterCommand(menuCommand);
				return false;
			case MenuCommand::Commands::back_and_set_track_to_menu:
				handleStartMusic(L"01");
			case MenuCommand::Commands::back:
				handleBackCommand(menuCommand);
				return false;
			case MenuCommand::Commands::back_and_write_settings:
				handleWriteSettings();
				handleBackCommand(menuCommand);
				return false;

			//miscellaneous functionality
			case MenuCommand::Commands::start_music:
				handleStartMusic(std::get<std::wstring>(menuCommand.data));
				return true;

			case MenuCommand::Commands::sound_toggle:
				handleSoundToggleCommand();
				return true;
			case MenuCommand::Commands::fullscreen_toggle:
				handleFullscreenToggleCommand();
				return true;

			case MenuCommand::Commands::restart_game:
				break;

			case MenuCommand::Commands::game_over:
				handleGameOverCommand();
				return false;

			case MenuCommand::Commands::exit:
				handleExitCommand();
				return false;

			case MenuCommand::Commands::none:
				return true;

			default:
				throw std::runtime_error{ "default case in parseMenuCommand" };
		}	//switch
	}

	void MenuNavigationSystem::handleStopMusic() {
		globalChannelSetPointer->getChannel(GlobalTopics::stopMusicFlag).addMessage();
	}
	void MenuNavigationSystem::handleEnterCommand(const MenuCommand& menuCommand) {
		globalChannelSetPointer->getChannel(GlobalTopics::sceneEntry)
			.addMessage(std::get<SceneNames>(menuCommand.data));
	}

	void MenuNavigationSystem::handleBackCommand(const MenuCommand& menuCommand) {
		globalChannelSetPointer->getChannel(GlobalTopics::sceneExitTo)
			.addMessage(std::get<SceneNames>(menuCommand.data));
	}
	void MenuNavigationSystem::handleStartMusic(const std::wstring& trackName) {
		globalChannelSetPointer->getChannel(GlobalTopics::startMusic)
			.addMessage(trackName);
	}
	void MenuNavigationSystem::handleWriteSettings() {
		globalChannelSetPointer->getChannel(GlobalTopics::writeSettingsFlag)
			.addMessage();
	}
	void MenuNavigationSystem::handleRestartGameCommand() {
		handleStopMusic();
		//todo: need to read the gameConfig and send us back to the correct scene
		//todo: then, need to immediately pop up a new game
	}
	void MenuNavigationSystem::handleSoundToggleCommand() {
		globalChannelSetPointer->getChannel(GlobalTopics::soundToggleFlag).addMessage();
	}
	void MenuNavigationSystem::handleFullscreenToggleCommand() {
		globalChannelSetPointer->getChannel(GlobalTopics::fullscreenToggleFlag)
			.addMessage();
	}

	void MenuNavigationSystem::handleGameOverCommand() {
		handleStopMusic();
		//todo: need to read the gameConfig and send us back to the correct scene
		globalChannelSetPointer->getChannel(GlobalTopics::gameOverFlag).addMessage();
	}
	void MenuNavigationSystem::handleExitCommand() {
		globalChannelSetPointer->getChannel(GlobalTopics::exitFlag).addMessage();
	}

	bool MenuNavigationSystem::isLockedButton(
		DataStorage& dataStorage,
		const EntityHandle& entity
	) {
		if (dataStorage.containsComponent<ButtonData>(entity)) {
			const ButtonData& buttonData{
				dataStorage.getComponent<ButtonData>(entity)
			};
			return buttonData.locked;
		}
		return false;
	}

	void MenuNavigationSystem::setSelectedElement(
		Scene& scene,
		const EntityHandle& previousSelectedElement,
		const EntityHandle& newSelectedElement
	) {
		//bail out if the two elements are the same somehow
		if (newSelectedElement == previousSelectedElement) {
			return;
		}

		DataStorage& dataStorage{ scene.getDataStorage() };

		//update the stored element
		auto& currentSelectedElementChannel{
			scene.getChannel(SceneTopics::currentSelectedElement)
		};
		currentSelectedElementChannel.clear();
		currentSelectedElementChannel.addMessage(newSelectedElement);

		//send out selection messages
		auto& elementSelectionChannel{ 
			scene.getChannel(SceneTopics::elementSelection) 
		};

		elementSelectionChannel.addMessage({ previousSelectedElement, false });
		elementSelectionChannel.addMessage({ newSelectedElement, true });
	}
}