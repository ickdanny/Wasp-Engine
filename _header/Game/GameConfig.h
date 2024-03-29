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
    constexpr int respawnPeriod{ 20 };
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
    constexpr float pickupInitSpeedBase{ 1.5f };
    constexpr float pickupInitSpeedMulti{ .7f };

    constexpr float pickupFinalSpeed{ -1.7f };

    constexpr float smallPickupHitbox{ 8.0f };
    constexpr float largePickupHitbox{ 12.0f };

    constexpr float pickupOutbound{ -100.0f };

    constexpr int smallPowerGain{ 1 };
    constexpr int largePowerGain{ 5 };

    //enemies
    constexpr float spawnOut{ 20.0f };
    constexpr float topOut{ -spawnOut + config::gameOffset.y };
    constexpr float leftOut{ -spawnOut + config::gameOffset.x };
    constexpr float rightOut{ config::gameWidth + spawnOut + config::gameOffset.x };

    //bosses
    constexpr float bossY{ 60.0f };
    constexpr math::Point2 bossMidpoint{ 
        (config::gameWidth / 2.0f) + config::gameOffset.x, bossY 
    };
    constexpr float bossSpeed{ 1.5f };

    constexpr float bossInbound{ 30.0f };
    constexpr float bossMaxY{ (config::gameHeight * 0.28f) + config::gameOffset.y };
    constexpr math::AABB bossBounds{
        bossInbound + config::gameOffset.x,
        config::gameWidth - bossInbound + config::gameOffset.x,
        bossInbound + config::gameOffset.x,
        bossMaxY
    };
    constexpr float bossGotoRadiusMin{ 30.0f };
    constexpr float bossGotoRadiusMax{ 60.0f };
    constexpr float bossGotoSpeed{ 1.0f };

    constexpr float bossHitbox{ 13.0f };

	//graphics
	constexpr int backgroundDrawOrder{ -10000 };
    constexpr int effectDrawOrder{ -6000 };
    constexpr int playerBulletDrawOrder{ -5000 };
    constexpr int enemyDrawOrder{ -4000 };
    constexpr int pickupDrawOrder{ -3000 };
	constexpr int playerDrawOrder{ 0 };
    constexpr int enemyBulletDrawOrder{ 1000 };
	constexpr int foregroundDrawOrder{ 10000 };
}