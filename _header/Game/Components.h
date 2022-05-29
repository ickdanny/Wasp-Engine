#pragma once

#include "ECS/Entity/EntityHandle.h"
#include "Components/TwoFramePosition.h"
#include "Components/MenuCommand.h"
#include "Components/ButtonData.h"
#include "Components/LockCondition.h"
#include "Graphics/BitmapDrawInstruction.h"

namespace wasp::game {

    struct MenuCommandUp : components::MenuCommand {};
    struct MenuCommandDown : components::MenuCommand {};
    struct MenuCommandLeft : components::MenuCommand {};
    struct MenuCommandRight : components::MenuCommand {};
    struct MenuCommandSelect : components::MenuCommand {};

    struct NeighborElementUp : ecs::entity::EntityHandle {};
    struct NeighborElementDown : ecs::entity::EntityHandle {};
    struct NeighborElementLeft : ecs::entity::EntityHandle {};
    struct NeighborElementRight : ecs::entity::EntityHandle {};

    struct ButtonData : public components::ButtonData {
        using components::ButtonData::ButtonData;
    };
    struct SpriteInstruction : public graphics::BitmapDrawInstruction {
        using graphics::BitmapDrawInstruction::BitmapDrawInstruction;
    };

    struct VisibleMarker {};

    struct DrawOrder {
        int order{};
    };

    struct Position : components::TwoFramePosition {
        using components::TwoFramePosition::TwoFramePosition;
    };
    struct Velocity : math::Vector2 {
        using math::Vector2::Vector2;
    };
}