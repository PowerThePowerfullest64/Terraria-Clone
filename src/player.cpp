#include "player.hpp"

#include <iostream>

#include "gameplaySession.hpp"
#include "game.hpp"
#include "textureManager.hpp"
#include "inputManager.hpp"
#include "block.hpp"
#include "settings.hpp"

Player::Player(GameplaySession* gameplaySession, const Vec2f& position) :
    Entity(gameplaySession, position),
    topRightCollider(gameplaySession, this, {7.f, -14.f}),
    topLeftCollider(gameplaySession, this, {-7.f, -14.f}),
    bottomRightCollider(gameplaySession, this, {7.f, 16.f}),
    bottomLeftCollider(gameplaySession, this, {-7.f, 16.f}),
    groundedCollider(gameplaySession, this, {0.f, 24.f})
    {
        sprite = TextureManager::Get("human"); // Use texture.
        type = EntityType::Player;
        scale = Vec2f::ONE * (2.f / 3.f);
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

    if (Settings::showPlayerColliders) {
        topRightCollider.Render();
        topLeftCollider.Render();
        bottomRightCollider.Render();
        bottomLeftCollider.Render();
        groundedCollider.Render();
    }
}

void Player::MoveAndSlide(float dt) {
    bool grounded = groundedCollider.CheckCollision();

    velocity.y += Settings::gravity * dt;

    if (!gameplaySession->console.open) {
        if (InputManager::moveLeftDown) velocity.x = std::lerp(velocity.x, -speed, acceleration * dt);
        else if (InputManager::moveRightDown) velocity.x = std::lerp(velocity.x, speed, acceleration * dt);
        else if (grounded) velocity.x = std::lerp(velocity.x, 0.f, friction * dt);

        if (InputManager::jumpDown && grounded) {
            velocity.y = -jumpPower;
        }
    }

    // Test position.
    position.x += velocity.x * dt;

    // Undo position if it collides.
    if (velocity.x > 0.f) {
        if (bottomRightCollider.CheckCollision() || topRightCollider.CheckCollision()) {
            position.x -= velocity.x * dt;
            velocity.x = 0.f;
        }
    } else if (velocity.x < 0.f) {
        if (bottomLeftCollider.CheckCollision() || topLeftCollider.CheckCollision()) {
            position.x -= velocity.x * dt;
            velocity.x = 0.f;
        }
    }

    // Test position.
    position.y += velocity.y * dt;

    // Undo position if it collides.
    if (velocity.y > 0.f) {
        if (bottomRightCollider.CheckCollision() || bottomLeftCollider.CheckCollision()) {
            position.y -= velocity.y * dt;
            velocity.y = 0.f;
        }
    } else if (velocity.y < 0.f) {
        if (topRightCollider.CheckCollision() || topLeftCollider.CheckCollision()) {
            position.y -= velocity.y * dt;
            velocity.y = 0.f;
        }
    }
}

void Player::MiningUpdate(float dt) {
    if (!InputManager::mineDown) return;

    Vector2 mouseWorld = GetScreenToWorld2D(GetMousePosition(), gameplaySession->cam);

    Vec2i worldPos = World::ToWorld(Vec2f::fromVector2(mouseWorld));
    BlockType type = gameplaySession->world->GetBlock(worldPos);

    if (type == BlockType::AIR) return;

    gameplaySession->world->Mine(worldPos.x, worldPos.y, miningTimeFactor * dt);
}