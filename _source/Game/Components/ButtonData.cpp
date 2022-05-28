#include "Game/Components/ButtonData.h"

namespace wasp::game::components {

    const std::string ButtonData::getSelImageName() {
        return baseImageName + "_sel";
    }

    const std::string ButtonData::getUnselImageName() {
        return baseImageName + "_unsel";
    }

    const std::string ButtonData::getLockedImageName() {
        return baseImageName + "_locked";
    }
}