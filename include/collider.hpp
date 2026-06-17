#pragma once

#include "raylib.h"

#include "vec2f.h"

class Entity;

class GameplaySession;

class Collider {
public:
    Collider(GameplaySession* gameplaySession, Entity* entity, const Vec2f& position);

    bool CheckCollision();
    void Render();

private:
    GameplaySession* gameplaySession;
    Entity* entity;
    Vec2f position;
};