#pragma once

#include <chrono>

#include "game.hpp"
#include "world.hpp"
#include "player.hpp"

class GameplaySession {
public:
    Game* game;
    
    Camera2D cam = {{(float)game->SCREEN_WIDTH / 2.f, (float)game->SCREEN_HEIGHT / 2.f}, {0.f, 0.f}, 0.f, 1.f};

    World* world = nullptr;
    Player* player = nullptr;

    GameplaySession(Game* game);
    ~GameplaySession();

    void Run();

    void SetTPS(float targetTps);
private:
    bool running = false;

    float tps;
    float tickDuration;
    float accumulator = 0.f;

    int currentZoomLevel = 3;

    std::chrono::high_resolution_clock::time_point timer;
    std::chrono::milliseconds updateTime;
    std::chrono::milliseconds renderTime;

    void UpdateGame();
    void RenderGame();
    void RenderGameUI();
    void RenderPausedUI();
};