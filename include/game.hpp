#pragma once

#include <stdint.h>
#include <chrono>

#include "raylib.h"

#include "world.hpp"
#include "player.hpp"

enum class GameState {
    MENU,
    GAME
};

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

    float zoomLevels[9];
    int currentZoomLevel = 3;

    GameState gs = GameState::MENU;

    Player* player;

    std::chrono::high_resolution_clock::time_point timer;
    std::chrono::milliseconds updateTime;
    std::chrono::milliseconds renderTime;

    void Update();
    void RenderGame();
    void RenderUI();
};