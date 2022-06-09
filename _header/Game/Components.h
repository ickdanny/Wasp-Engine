#pragma once

#include "ECS/Entity/EntityHandle.h"
#include "Components/TwoFramePosition.h"
#include "Components/MenuCommand.h"
#include "Components/ButtonData.h"
#include "Components/LockCondition.h"
#include "Graphics/BitmapDrawInstruction.h"
#include "Components/PlayerData.h"

namespace wasp::game {

    struct MenuCommandUp : components::MenuCommand {
        using components::MenuCommand::MenuCommand;
    };
    struct MenuCommandDown : components::MenuCommand {
        using components::MenuCommand::MenuCommand;
    };
    struct MenuCommandLeft : components::MenuCommand {
        using components::MenuCommand::MenuCommand;
    };
    struct MenuCommandRight : components::MenuCommand {
        using components::MenuCommand::MenuCommand;
    };
    struct MenuCommandSelect : components::MenuCommand {
        using components::MenuCommand::MenuCommand;
    };

    struct NeighborElementUp : ecs::entity::EntityHandle {};
    struct NeighborElementDown : ecs::entity::EntityHandle {};
    struct NeighborElementLeft : ecs::entity::EntityHandle {};
    struct NeighborElementRight : ecs::entity::EntityHandle {};

    struct ButtonData : components::ButtonData {
        using components::ButtonData::ButtonData;
    };
    struct SpriteInstruction : graphics::BitmapDrawInstruction {
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

    struct PlayerData : components::PlayerData {};
}