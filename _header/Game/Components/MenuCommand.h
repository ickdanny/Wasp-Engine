#pragma once

#include <variant>
#include <string>

#include "Game/Scenes.h"
#include "Game/Systems/GameBuilderCommands.h"

namespace wasp::game::components {
    struct MenuCommand {
    private:
        //typedefs
        using GameBuilderCommands = systems::GameBuilderCommands;
        using DataVariant
            = std::variant<std::tuple<SceneNames, GameBuilderCommands>, std::wstring>;

    public:
        //fields
        enum class Commands {
            nav_up,
            nav_down,
            nav_left,
            nav_right,

            nav_far_up,
            nav_far_down,
            nav_far_left,
            nav_far_right,

            enter,
            enter_and_stop_music,

            back,
            back_and_write_settings,
            back_and_set_track_to_menu,

            start_music,

            sound_toggle,
            fullscreen_toggle,

            restart_game,

            game_over,

            exit,

            none
        } command{};
        DataVariant data{};

        //static constant for no command
        static const MenuCommand none;

        //constructors

        //default constructor
        MenuCommand()
            : command{ Commands::none }
            , data{} {
        }
        
        //full arg constructor
        MenuCommand(Commands command, const DataVariant& data)
            : command{ command }
            , data{ data }{
        }

        //constructs a MenuCommand with the given scene name as data and "none" as the
        //game builder command
        MenuCommand(Commands command, SceneNames sceneName)
            : command{ command }
            , data{ std::tuple{ sceneName, GameBuilderCommands::none } }{
        }

        //constructs a MenuCommand with a default-initialized data member
        MenuCommand(Commands command)
            : command{ command }
            , data{}{
        }
    };
}