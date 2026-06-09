#include "entity.hpp"

#include "game.hpp"

Player::Player(Game* game, const Vec2f& position) :
    Entity(game, position)
    {
        sprite = game->tm.Get("human"); // use texture
    }

void Player::Update(float dt) {
    Vec2f velChange = Vec2f::ZERO;
    float groundY = 700.f; // temp variable for testing purposes

    // do input handling also!
    if (position.y >= groundY) {
        grounded = true;
    } else grounded = false;

    velocity.y += game->GRAVITY * dt;

    if (game->im.moveLeftDown) velocity.x = std::lerp(velocity.x, -speed, acceleration * dt);
    else if (game->im.moveRightDown) velocity.x = std::lerp(velocity.x, speed, acceleration * dt);
    else if (grounded) velocity.x = std::lerp(velocity.x, 0.f, acceleration * dt);

    if (game->im.jumpPressed && grounded) {
        velocity.y = -jumpPower;
    }

    velocity += velChange * dt;
    position += velocity;

    if (position.y > groundY) {
        position.y = groundY;
        velocity.y = std::max(velocity.y, 0.f);
    }
}

void Player::Render() {
    if (sprite == nullptr) sprite = game->tm.Get("noTexture");

    Rectangle source = {0.f, 0.f, (float)sprite->width, (float)sprite->height};
    Rectangle dest = {position.x, position.y, source.width * scale.x, source.height * scale.y};
    Vector2 origin = {dest.width / 2.f, dest.height};

    DrawTexturePro(*sprite, source, dest, origin, rotation, WHITE);

    Color c = grounded ? GREEN : RED;
    DrawCircleV(position, 4.f, c);

    DrawLineV({0.f, 700.f}, {(float)game->SCREEN_WIDTH, 700.f}, YELLOW);
}