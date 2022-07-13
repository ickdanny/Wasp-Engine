#pragma once

#include "Math/Geometry.h"

namespace wasp::game::config {

	//field of play
    constexpr float gameWidth{ 170.0f };
    constexpr float gameHeight{ 214.0f };
    constexpr math::Vector2 gameOffset{ 15.0f, 13.0f };

    constexpr float collisionOutbound{ 100.0f };
    constexpr math::AABB collisionBounds = math::AABB{
        -collisionOutbound,
        gameWidth + collisionOutbound,
        -collisionOutbound,
        gameHeight + collisionOutbound
    } 
    + gameOffset;

	//player
    constexpr math::Point2 playerSpawn = math::Point2{
        gameWidth / 2.0f,
        gameHeight - 25.0f
    }
    + gameOffset;

    constexpr math::AABB playerHitbox{ 2.0f };

	constexpr float playerSpeed{ 3.0f };
    constexpr float focusSpeedMulti{ 0.4f };
	constexpr float focusedSpeed{ playerSpeed * focusSpeedMulti };
    constexpr float playerInbound{ 5.0f };

    constexpr int deathBombPeriod{ 15 };
    constexpr int bombInvulnerabilityPeriod{ 4 * 60 };
    constexpr int deathPeriod{ 30 };
    constexpr int respawnPeriod{ 30 };
    constexpr int respawnInvulnerabilityPeriod{ 3 * 60 };

    constexpr int initLives{ 2 };
    constexpr int initBombs{ 2 };
    constexpr int initContinues{ 2 };

    constexpr int maxLives{ 6 };
    constexpr int maxBombs{ maxLives };

    constexpr int maxPower{ 80 };

    constexpr int continueLives{ 2 };
    constexpr int respawnBombs{ 2 };

    constexpr int playerShotMaxTick{ 40 };

    //pickups
    constexpr float pickupInitSpeedBase{ 3.5f };
    constexpr float pickupInitSpeedMulti{ .7f };

    constexpr float pickupFinalSpeed{ -1.7f };

    constexpr float smallPickupHitbox{ 8.0f };
    constexpr float largePickupHitbox{ 12.0f };

    constexpr float pickupOutbound{ -100.0f };

    constexpr int smallPowerGain{ 1 };
    constexpr int largePowerGain{ 5 };

    /*

//PROJECTILES
    public static final double NORMAL_OUTBOUND = -20;
    public static final double LARGE_OUTBOUND = -50;

//ENEMIES
    public static final double ENEMY_OUTBOUND = -50;
    public static final double ENEMY_SPAWN_INBOUND = 30;

    public static final AABB SMALL_ENEMY_HITBOX = new AABB(10);
    public static final AABB LARGE_ENEMY_HITBOX = new AABB(15);

//BOSSES
    public static final double BOSS_Y = 150;
    public static final DoublePoint BOSS_MIDPOINT = new DoublePoint(WIDTH/2d, BOSS_Y);
    public static final double BOSS_SPEED = 4d;

    public static final double BOSS_INBOUND = 60;
    public static final double BOSS_MAX_Y = HEIGHT * .28;
    public static final AABB BOSS_BOUNDS = new AABB(BOSS_INBOUND, WIDTH - BOSS_INBOUND, BOSS_INBOUND, BOSS_MAX_Y);
    public static final double BOSS_GOTO_RADIUS_MIN = 60;
    public static final double BOSS_GOTO_RADIUS_MAX = 120;
    public static final double BOSS_GOTO_SPEED = 2d;


//ENEMY PROJECTILES
    public static final int ENEMY_BULLET_COLLIDABLE_TIME = 2;

    public static final AABB SMALL_BULLET_HITBOX = new AABB(2.7);
    public static final AABB MEDIUM_BULLET_HITBOX = new AABB(6.3);
    public static final AABB LARGE_BULLET_HITBOX = new AABB(15.7);
    */

	//graphics
	constexpr int backgroundDrawOrder{ -10000 };
    constexpr int playerBulletDrawOrder{ -5000 };
    constexpr int pickupDrawOrder{ -4000 };
	constexpr int playerDrawOrder{ 0 };
	constexpr int foregroundDrawOrder{ 10000 };
}