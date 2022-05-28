#pragma once

#include "ECS/Entity/EntityHandle.h"
#include "Math/Point2.h"
#include "Components/MenuCommand.h"
#include "Components/ButtonData.h"
#include "Components/LockCondition.h"
#include "Graphics/BitmapDrawInstruction.h"

namespace wasp::game {

    struct MenuCommandUp : public components::MenuCommand {};
    struct MenuCommandDown : public components::MenuCommand {};
    struct MenuCommandLeft : public components::MenuCommand {};
    struct MenuCommandRight : public components::MenuCommand {};
    struct MenuCommandSelect : public components::MenuCommand {};

    struct NeighborElementUp : public ecs::entity::EntityHandle {};
    struct NeighborElementDown : public ecs::entity::EntityHandle {};
    struct NeighborElementLeft : public ecs::entity::EntityHandle {};
    struct NeighborElementRight : public ecs::entity::EntityHandle {};

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

    struct Position : public math::Point2 {};
    struct Velocity : public math::Vector2 {};
}