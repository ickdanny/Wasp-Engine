#pragma once

#include "ECS/Entity/EntityHandle.h"
#include "Components/TwoFramePosition.h"
#include "Components/MenuCommand.h"
#include "Components/ButtonData.h"
#include "Graphics/BitmapDrawInstruction.h"
#include "Components/PlayerData.h"

namespace wasp::game {

    //ui
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

    //graphics
    struct VisibleMarker {};
    struct SpriteInstruction : graphics::BitmapDrawInstruction {
        using graphics::BitmapDrawInstruction::BitmapDrawInstruction;
    };
    struct DrawOrder {
        int order{};
    };

    struct Position : components::TwoFramePosition {
        using components::TwoFramePosition::TwoFramePosition;

        //Explicitly define copy and move constructors taking Point2
        Position(const math::Point2& toCopy)
            : components::TwoFramePosition(toCopy) {
        }
        Position(math::Point2&& toMove)
            : components::TwoFramePosition(toMove) {
        }
    };
    struct Velocity : math::Vector2 {
        using math::Vector2::Vector2;

        Velocity& operator=(const Velocity& right) = default;
        Velocity& operator=(const math::Vector2& right) {
            math::Vector2::operator=(right);
            return *this;
        }
    };

    //game
    struct CollidableMarker {};
    struct Hitbox : math::AABB {
        using math::AABB::AABB;

        //Explicitly define copy and move constructors taking AABB
        Hitbox(const AABB& toCopy)
            : math::AABB(toCopy) {
        }
        Hitbox(AABB&& toMove)
            : math::AABB(toMove) {
        }
    };

    struct Inbound {
        float bound;
    };
    struct Outbound {
        float bound;
    };

    struct PlayerData : components::PlayerData {
        using components::PlayerData::PlayerData;
    };
}