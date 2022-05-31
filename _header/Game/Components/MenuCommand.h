#pragma once

#include <variant>

namespace wasp::game::components {
    struct MenuCommand {
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
        std::variant<SceneNames, std::wstring> data{};

        //static constant for no command
        static const MenuCommand none;
    };
}