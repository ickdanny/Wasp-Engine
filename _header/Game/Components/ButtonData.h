#pragma once

#include <string>

#include "Math/Point2.h"

namespace wasp::game::components {
    struct ButtonData {
    private:
        //typedefs
        using Point2 = math::Point2;

        //fields
        const std::string baseImageName{};
    public:
        const Point2 unselPos{};
        const Point2 selPos{};
        bool locked{};

        ButtonData(Point2 pos, const std::string& baseImageName)
            : unselPos{ pos }
            , selPos{ pos }
            , baseImageName{ baseImageName }
            , locked{ false } {
        }

        ButtonData(Point2 unselPos, Point2 selPos, const std::string& baseImageName)
            : unselPos{ unselPos }
            , selPos{ selPos }
            , baseImageName{ baseImageName } 
            , locked{ false } {
        }

        ButtonData(
            Point2 unselPos,
            Point2 selPos,
            const std::string& baseImage,
            bool locked
        )
            : unselPos{ unselPos }
            , selPos{ selPos }
            , baseImageName{ baseImageName } 
            , locked{ locked } {
        }

        const std::string getSelImageName();
        const std::string getUnselImageName();
        const std::string getLockedImageName();
    };
}