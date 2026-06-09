#pragma once

#include "raylib.h"

#include "vec2f.h"

class Entity;

class Game;

class Collider {
public:
    bool colliding = false;

    Collider(Game* game, Entity* entity, const Vec2f& position);

    void Update();
    void Render();

private:
    Game* game;
    Entity* entity;
    Vec2f position;
};