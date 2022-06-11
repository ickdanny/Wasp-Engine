#include "Game/Systems/PlayerMovementSystem.h"

namespace wasp::game::systems {

	void PlayerMovementSystem::operator()(Scene& scene) {

        //this system only operators on scenes with game commands 
        auto& gameCommandChannel{ scene.getChannel(SceneTopics::gameCommands) };
        if (gameCommandChannel.hasMessages()) {

            //retrieve system data from the scene
            static Topic<SceneData> sceneDataTopic{};
            auto& sceneDataChannel{
                scene.getChannel(sceneDataTopic)
            };
            if (sceneDataChannel.isEmpty()) {
                sceneDataChannel.addMessage({ });
            }
            auto& [twoFramePlayerInputData, active] = sceneDataChannel.getMessages()[0];

            //update the input data
            twoFramePlayerInputData.reset();
            checkActive(scene, active);
            if (active) {
                for (auto& gameCommand : gameCommandChannel.getMessages()) {
                    parseGameCommand(gameCommand, twoFramePlayerInputData);
                }
            }

            //update player velocity if needed
            if (twoFramePlayerInputData != twoFramePlayerInputData.getPast()) {
                Vector2 velocity = calculateVelocity(twoFramePlayerInputData);

                static const Topic<ecs::component::Group*> groupPointerStorageTopic{};

                auto groupPointer{
                    getGroupPointer<PlayerData, Velocity>(
                        scene, 
                        groupPointerStorageTopic
                    )
                };

                auto groupIterator{ groupPointer->groupIterator<Velocity>() };

                while (groupIterator.isValid()) {
                    auto [playerVelocity] = *groupIterator;
                    playerVelocity = velocity;
                    ++groupIterator;
                }
                twoFramePlayerInputData.step();
            }
        }
	}

    //checks if this system should operate on the player based on their state
    void PlayerMovementSystem::checkActive(Scene& scene, bool& active) {
        auto& playerStateEntryChannel{ scene.getChannel(SceneTopics::playerStateEntry) };
        if(playerStateEntryChannel.hasMessages()) {
            for (auto& playerState : playerStateEntryChannel.getMessages()) {
                switch (playerState) {
                    case PlayerStates::normal:
                    case PlayerStates::bombing:
                    case PlayerStates::respawnInvulnerable:
                        active = true;
                        break;
                    case PlayerStates::dead:
                    case PlayerStates::checkContinue:
                    case PlayerStates::respawning:
                    case PlayerStates::gameOver:
                        active = false;
                        break;
                    default:
                        throw std::runtime_error{ "Unexpected player state!" };
                }
            }
        }
    }

    //updates the given PlayerInputData based on the specified game command
    void PlayerMovementSystem::parseGameCommand(
        GameCommands gameCommand,
        PlayerInputData& inputData
    ) {
        switch (gameCommand) {
            case GameCommands::focus:
                inputData.setFocus(true);
                break;
            case GameCommands::up:
                inputData.setUp(true);
                break;
            case GameCommands::down:
                inputData.setDown(true);
                break;
            case GameCommands::left:
                inputData.setLeft(true);
                break;
            case GameCommands::right:
                inputData.setRight(true);
                break;
        }
    }

    //converts a PlayerInputData object into a Vector2 object
    math::Vector2 PlayerMovementSystem::calculateVelocity(
        const PlayerInputData inputData
    ) {
        //return the zero vector if either no direction is pressed or all directions are
        if (inputData.isZero()) {
            return {};
        }

        //use ints to prevent floating point nonsense
        int x{ 0 };
        int y{ 0 };
        if (inputData.isUp()) {
            --y;
        }
        if (inputData.isDown()) {
            ++y;
        }
        if (inputData.isLeft()) {
            --x;
        }
        if (inputData.isRight()) {
            ++x;
        }
        if (x | y) {
            Vector2 velocity{ x, y };
            float magnitude = inputData.isFocused() 
                ? config::focusedSpeed 
                : config::playerSpeed;
            velocity *= magnitude / math::getMagnitude(velocity);
            return velocity;
        }
        else {
            throw std::runtime_error{ "expect either x or y =/= 0!" };
        }
    }
}