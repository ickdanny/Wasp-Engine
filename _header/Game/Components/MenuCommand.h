#pragma once

namespace wasp::game::components {
    struct MenuCommand {
        enum class Commands {
            NAV_UP,
            NAV_DOWN,
            NAV_LEFT,
            NAV_RIGHT,

            NAV_FAR_UP,
            NAV_FAR_DOWN,
            NAV_FAR_LEFT,
            NAV_FAR_RIGHT,

            ENTER,
            ENTER_AND_STOP_MUSIC,

            BACK,
            BACK_TO_MENU,
            BACK_TO_MAIN,
            BACK_AND_WRITE_PROPERTIES,
            BACK_AND_SET_TRACK_TO_MENU,

            START_TRACK,

            SOUND_TOGGLE,
            FULLSCREEN_TOGGLE,

            RESTART_GAME,

            GAME_OVER,

            EXIT
        } command{};
    };
}