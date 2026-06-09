#pragma once

#include "raylib.h"

#include "vec2f.h"

class Game;

class Collider {
public:
    bool colliding = false;

    Collider(Game* game, const Vec2f& position);

    void Update();
    void Render(const Vec2f& rel) { Color c = colliding ? RED : GREEN; DrawCircleV(position+rel, 4.f, c); }
    
private:
    Game* game;
    Vec2f position;
};