#pragma once

#include "raylib.h"

#include "vec2f.h"

class GameplaySession;

class Entity {
public:
    Entity(GameplaySession* gameplaySession, const Vec2f& position) :
        gameplaySession(gameplaySession),
        position(position) {}
    
    virtual ~Entity() = default;

    virtual void Update(float dt) {}
    virtual void Render() {}
protected:
    GameplaySession* gameplaySession;
    Vec2f velocity = Vec2f::ZERO;
    Vec2f scale = Vec2f::ONE;
    float rotation = 0.f;
public:
    Vec2f position;
    Texture2D* sprite = nullptr;
};