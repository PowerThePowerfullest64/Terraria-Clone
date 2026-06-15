#pragma once

#include <stdint.h>

#include "raylib.h"

#include "world.hpp"

class Game {
public:
    const uint16_t SCREEN_WIDTH = 1440u, SCREEN_HEIGHT = 810u;

    World world;
    
    Camera2D cam;

    const float GRAVITY = 2000.f;

    Game();

    void Run();
    void SetTPS(float targetTps);

private:
    bool running = false;

    float tps;
    float tickDuration;
    float accumulator = 0.f;
};