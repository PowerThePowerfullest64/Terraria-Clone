#include "player.hpp"

#include <iostream>

#include "raylib.h"

#include "gameplaySession.hpp"
#include "game.hpp"
#include "textureManager.hpp"
#include "inputManager.hpp"
#include "block.hpp"
#include "settings.hpp"

Player::Player(GameplaySession* gameplaySession, const Vec2f& position) :
    Entity(gameplaySession, position)
    {
        sprite = TextureManager::Get("human"); // Use texture.
        type = EntityType::Player;
        scale = Vec2f::ONE * (2.f / 3.f);
        collider = AABB(-6.f, -14.f, 12.f, 30.f);
        gameplaySession->player = this;
        std::cout << "Constructed Player!\n";
    }

void Player::Update(float dt) {
    MoveAndSlide(dt);

    MiningUpdate(dt);
}

void Player::Render() {
    if (sprite == nullptr) sprite = TextureManager::Get("noTexture");

    Rectangle source = {0.f, 0.f, (float)sprite->width, (float)sprite->height};
    Rectangle dest = {position.x, position.y, source.width * scale.x, source.height * scale.y};
    Vector2 origin = {dest.width / 2.f, dest.height / 2.f};

    DrawTexturePro(*sprite, source, dest, origin, rotation, WHITE);

    if (Settings::showPlayerVelocity) {
        float mag = velocity.length() / 6.f;
        if (mag > 0.f) {
            DrawLineV(position, position + velocity.normalized() * mag, YELLOW);
        }
    }

    if (Settings::showEntityColliders) {
        DrawRectangleLinesEx(
        {
            collider.x + position.x,
            collider.y + position.y,
            collider.w,
            collider.h
        },
            0.5f,
            grounded ? GREEN : RED
        );
    }
}

void Player::MoveAndSlide(float dt) {
    velocity.y += Settings::gravity * dt;

    if (!gameplaySession->console.open) {
        if (InputManager::moveLeftDown)
            velocity.x = std::lerp(velocity.x, -speed, acceleration * dt);
        else if (InputManager::moveRightDown)
            velocity.x = std::lerp(velocity.x, speed, acceleration * dt);
        else if (grounded)
            velocity.x = std::lerp(velocity.x, 0.f, friction * dt);

        if (InputManager::jumpDown && grounded) {
            velocity.y = -jumpPower;
        }
    }

    grounded = false;

    colliders = gameplaySession->world->GetBlockColliders(position);

    position.x += velocity.x * dt;

    for (int i = 0; i < colliders.size(); ++i) {
        if (Intersects(GetRealCollider(collider), colliders[i])) {
            position.x -= velocity.x * dt;
            velocity.x = 0.f;
            break;
        }
    }

    position.y += velocity.y * dt;

    for (int i = 0; i < colliders.size(); ++i) {
        if (Intersects(GetRealCollider(collider), colliders[i])) {
            if (velocity.y > 0.f)
                grounded = true;

            position.y -= velocity.y * dt;
            velocity.y = 0.f;
            break;
        }
    }
}

void Player::MiningUpdate(float dt) {
    if (!InputManager::mineDown) return;

    Vector2 mouseWorld = GetScreenToWorld2D(GetMousePosition(), gameplaySession->cam);

    Vec2i worldPos = World::ToBlock(Vec2f::fromVector2(mouseWorld));
    BlockType type = gameplaySession->world->GetBlock(worldPos);

    if (type == BlockType::AIR) return;

    gameplaySession->world->Mine(worldPos.x, worldPos.y, miningTimeFactor * dt);
}