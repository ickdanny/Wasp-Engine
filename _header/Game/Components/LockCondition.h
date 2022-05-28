#pragma once

namespace wasp::game::components {
    struct LockCondition {
        enum class Conditions {
            UNLOCK_WHEN_GAME_BEATEN
        } condition{};
    };
}