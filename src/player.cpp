#include "entity.hpp"

#include <iostream>

#include "game.hpp"
#include "textureManager.hpp"
#include "inputManager.hpp"

Player::Player(Game* game, const Vec2f& position) :
    Entity(game, position),
    topRightCollider(game, this, {10.f, -24.f}),
    topLeftCollider(game, this, {-10.f, -24.f}),
    bottomRightCollider(game, this, {10.f, 24.f}),
    bottomLeftCollider(game, this, {-10.f, 24.f}),
    groundedCollider(game, this, {0.f, 34.f})
    {
        sprite = TextureManager::Get("human"); // use texture
    }

void Player::Update(float dt) {
    bool grounded = groundedCollider.CheckCollision();

    velocity.y += game->GRAVITY * dt;

    if (InputManager::moveLeftDown) velocity.x = std::lerp(velocity.x, -speed, acceleration * dt);
    else if (InputManager::moveRightDown) velocity.x = std::lerp(velocity.x, speed, acceleration * dt);
    else if (grounded) velocity.x = std::lerp(velocity.x, 0.f, friction * dt);

    if (InputManager::jumpPressed && grounded) {
        velocity.y = -jumpPower;
    }

    // test position
    position.x += velocity.x * dt;

    // reverse position if it collides
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

    // test position
    position.y += velocity.y * dt;

    // reverse position if it collides
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

void Player::Render() {
    if (sprite == nullptr) sprite = TextureManager::Get("noTexture");

    Rectangle source = {0.f, 0.f, (float)sprite->width, (float)sprite->height};
    Rectangle dest = {position.x, position.y, source.width * scale.x, source.height * scale.y};
    Vector2 origin = {dest.width / 2.f, dest.height / 2.f};

    DrawTexturePro(*sprite, source, dest, origin, rotation, WHITE);

    if (!renderDebug) return;

    float mag = velocity.length() / 6.f;
    if (mag > 0.f) {
        DrawLineV(position, position + velocity.normalized() * mag, YELLOW);
    }

    topRightCollider.Render();
    topLeftCollider.Render();
    bottomRightCollider.Render();
    bottomLeftCollider.Render();
    groundedCollider.Render();
}