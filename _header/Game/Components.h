#pragma once

#include "ECS/Entity/EntityHandle.h"
#include "Components/TwoFramePosition.h"
#include "Components/MenuCommand.h"
#include "Components/ButtonData.h"
#include "Graphics/BitmapDrawInstruction.h"
#include "Components/PlayerData.h"
#include "Components/CollisionType.h"
#include "Components/DeathCommand.h"
#include "Components/SpawnProgramList.h"
#include "Components/ScriptProgramList.h"

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

    struct RotateSpriteForwardMarker {};

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
    struct Velocity : math::PolarVector {
    private:
        using PolarVector = math::PolarVector;
        using Vector2 = math::Vector2;

    public:
        using PolarVector::PolarVector;

        //Explicitly define copy and move constructors taking PolarVector
        Velocity(const PolarVector& toCopy)
            : PolarVector(toCopy) {
        }
        Velocity(PolarVector&& toMove)
            : PolarVector(toMove) {
        }

        //Explicitly define copy and move constructors taking Vector2
        Velocity(const Vector2& toCopy)
            : PolarVector(toCopy) {
        }
        Velocity(Vector2&& toMove)
            : PolarVector(toMove) {
        }

        //Explicitly define assignment operators
        Velocity& operator=(const Velocity& right) = default;
        Velocity& operator=(const PolarVector& right) {
            PolarVector::operator=(right);
            return *this;
        }
    };

    //game
    struct CollidableMarker {};

    struct PlayerCollisions : components::CollisionType<PlayerCollisions> {};
    struct EnemyCollisions : components::CollisionType<EnemyCollisions> {};
    struct PickupCollisions : components::CollisionType<PickupCollisions> {};

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

    struct Health {
        int value{};
    };
    struct Damage {
        int value{};
    };
    struct DeathCommand : components::DeathCommand {
        using components::DeathCommand::DeathCommand;

        //Explicitly defined constructor taking the command
        DeathCommand(components::DeathCommand::Commands command)
            : components::DeathCommand{ command } {
        }
    };
    struct ScriptProgramList : components::ScriptProgramList {
        using components::ScriptProgramList::ScriptProgramList;
    };

    struct SpawnProgramList : components::SpawnProgramList {
        using components::SpawnProgramList::SpawnProgramList;
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