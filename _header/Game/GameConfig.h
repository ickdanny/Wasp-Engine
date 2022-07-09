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
    constexpr int initContinues{ 3 };

    constexpr int maxLives{ 6 };
    constexpr int maxBombs{ maxLives };

    constexpr int maxPower{ 80 };

    constexpr int continueLives{ 2 };
    constexpr int respawnBombs{ 2 };

    //player shot
    constexpr int playerShotMaxTick{ 40 };

    /*
    //PLAYER SHOT
    public static final double BASIC_PLAYER_BULLET_SPEED = 16.5;
    public static final AABB BASIC_PLAYER_BULLET_HITBOX = new AABB(25);
    public static final int BASIC_PLAYER_BULLET_DAMAGE = 100;

    public static final double BASIC_PLAYER_BULLET_ANGLE_ADD = -5;

    public static final AABB SPECIAL_PLAYER_BULLET_HITBOX = new AABB(35);
    public static final double SPECIAL_PLAYER_BULLET_SPEED_NORMAL = 15;
    public static final double SPECIAL_PLAYER_BULLET_SPEED_MEDIUM = 18;
    public static final double SPECIAL_PLAYER_BULLET_SPEED_HIGH = 22;

    public static final int SPECIAL_PLAYER_BULLET_DAMAGE_NORMAL = 150;
    public static final int SPECIAL_PLAYER_BULLET_DAMAGE_MEDIUM = 165;
    public static final int SPECIAL_PLAYER_BULLET_DAMAGE_HIGH = 185;

//PICKUPS
    */
    constexpr float pickupInitSpeedBase{ 3.5f };
    constexpr float pickupInitSpeedMulti{ .7f };

    /*
    
    public static final double PICKUP_FINAL_SPEED = -4;
    public static final double PICKUP_DECELERATION = .08;
    public static final double PICKUP_INBOUND = 15;

    public static final int NUM_SMALL_PLAYER_PICKUPS = 4;
    public static final double PLAYER_PICKUP_INBOUND = 30;
    public static final double PLAYER_PICKUP_Y_HIGH = 100;
    public static final int PLAYER_PICKUP_AIR_TIME = 60;

    private static final double LARGE_PICKUP_HITBOX_RADIUS = 25;
    public static final AABB LARGE_PICKUP_HITBOX = new AABB(LARGE_PICKUP_HITBOX_RADIUS);
    public static final AABB SMALL_PICKUP_HITBOX = new AABB(LARGE_PICKUP_HITBOX_RADIUS * 4/5);

    public static final int LARGE_POWER_GAIN = 8;
    public static final int SMALL_POWER_GAIN = 1;

    public static final double PICKUP_OUTBOUND = -100;

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