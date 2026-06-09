#pragma once

#include "raylib.h"

#include "vec2f.h"

class Game;

class Entity {
public:
    Entity(Game* game, const Vec2f& position) :
        game(game),
        position(position) {}
    
    virtual ~Entity() = default;

    virtual void Update(float dt) {}
    virtual void Render() {}
protected:
    Game* game;
    Vec2f position;
    Vec2f velocity = Vec2f::ZERO;
    Vec2f scale = Vec2f::ONE;
    float rotation = 0.f;
    bool grounded = false;
public:
    Texture2D* sprite = nullptr;
};

class Player : public Entity {
public:
    Player(Game* game, const Vec2f& position);
    
    void Update(float dt) override;
    void Render() override;

private:
    float acceleration = 5.0f;
    float speed = 7.5f;
    float jumpPower = 10.f;

    Vec2f scale = {1.5f, 1.5f};
};