#pragma once

#include "raylib.h"

#include "vec2f.h"

#include "collider.hpp"

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
    Vec2f velocity = Vec2f::ZERO;
    Vec2f scale = Vec2f::ONE;
    float rotation = 0.f;
    bool grounded = false;
public:
    Vec2f position;
    Texture2D* sprite = nullptr;
};

class Player : public Entity {
public:
    Player(Game* game, const Vec2f& position);
    
    void Update(float dt) override;
    void Render() override;

private:
    float acceleration = 15.f;
    float friction = 15.f;
    float speed = 250.f;
    float jumpPower = 500.f;

    Collider bottomCollider;
    Collider rightCollider;
    Collider leftCollider;
    Collider topCollider;

    bool renderDebug = false;
};