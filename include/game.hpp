#pragma once

#include <stdint.h>

#include "entityManager.hpp"
#include "inputManager.hpp"
#include "textureManager.hpp"
#include "worldManager.hpp"

class Game {
public:
    const uint16_t SCREEN_WIDTH = 1440u, SCREEN_HEIGHT = 810u;

    EntityManager em;
    InputManager im;
    TextureManager tm;
    WorldManager wm;

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