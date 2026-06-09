#include "entity.hpp"

#include <iostream>

#include "game.hpp"

Player::Player(Game* game, const Vec2f& position) :
    Entity(game, position),
    bottomCollider(game, this, {0.f, 24.f}),
    rightCollider(game, this, {10.f, 0.f}),
    leftCollider(game, this, {-10.f, 0.f}),
    topCollider(game, this, {0.f, -22.f})
    {
        sprite = game->tm.Get("human"); // use texture
    }

void Player::Update(float dt) {
    bottomCollider.Update();
    rightCollider.Update();
    leftCollider.Update();

    // do input handling also!
    grounded = bottomCollider.colliding;
    if (grounded) velocity.y = std::min(velocity.y, 0.f);

    if (!grounded)
        velocity.y += game->GRAVITY * dt;

    if (game->im.moveLeftDown) velocity.x = std::lerp(velocity.x, -speed, acceleration * dt);
    else if (game->im.moveRightDown) velocity.x = std::lerp(velocity.x, speed, acceleration * dt);
    else if (grounded) velocity.x = std::lerp(velocity.x, 0.f, friction * dt);

    if (game->im.jumpPressed && grounded) {
        velocity.y = -jumpPower;
    }

    if (rightCollider.colliding) velocity.x = std::min(velocity.x, 0.f);
    if (leftCollider.colliding) velocity.x = std::max(velocity.x, 0.f);

    if (topCollider.colliding) velocity.y = std::max(velocity.y, 0.f);

    position += velocity * dt;
}

void Player::Render() {
    if (sprite == nullptr) sprite = game->tm.Get("noTexture");

    Rectangle source = {0.f, 0.f, (float)sprite->width, (float)sprite->height};
    Rectangle dest = {position.x, position.y, source.width * scale.x, source.height * scale.y};
    Vector2 origin = {dest.width / 2.f, dest.height / 2.f};

    DrawTexturePro(*sprite, source, dest, origin, rotation, WHITE);

    if (!renderDebug) return;

    float mag = velocity.length() / 6.f;
    if (mag > 0.f) {
        DrawLineV(position, position + velocity.normalized() * mag, YELLOW);
    }

    Color c = grounded ? GREEN : RED;

    bottomCollider.Render();
    rightCollider.Render();
    leftCollider.Render();
    topCollider.Render();
}