#pragma once

#include <stdint.h>

#include "entityManager.h"
#include "inputManager.h"

class Game {
public:
    const uint16_t SCREEN_WIDTH = 1440u, SCREEN_HEIGHT = 810u;

    EntityManager em;
    InputManager im;

    void Run();
    void SetTPS(float targetTps);

private:
    bool running = false;

    float tps;
    float tickDuration;
    float accumulator;
};