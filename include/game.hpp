#pragma once

#include <stdint.h>

#include "raylib.h"

#include "world.hpp"

class Game {
public:
    const uint16_t SCREEN_WIDTH = 1440u, SCREEN_HEIGHT = 810u;

    World world;

    Camera2D cam = {{(float)SCREEN_WIDTH / 2.f, (float)SCREEN_HEIGHT / 2.f}, {0.f, 0.f}, 0.f, 1.f};
    Camera2D editorCam = {{0.f, 0.f}, {0.f, 0.f}, 0.f, 0.1f};

    bool debugMode = false;

    const float GRAVITY = 2000.f;

    Game();

    void Run(bool debug = false);
    void SetTPS(float targetTps);

private:
    bool running = false;

    float tps;
    float tickDuration;
    float accumulator = 0.f;

    float zoomSensitivity = 1.f;
};