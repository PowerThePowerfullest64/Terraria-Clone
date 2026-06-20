#pragma once

#include <chrono>

#include "vec2i.h"

#include "game.hpp"
#include "world.hpp"
#include "player.hpp"
#include "entityManager.hpp"

class GameplaySession {
public:
    Game* game;
    
    Camera2D cam;

    World* world = nullptr;
    Player* player = nullptr;

    EntityManager* em = nullptr;

    GameplaySession(Game* game);
    ~GameplaySession();

    void Run();

    void SetTPS(float targetTps);
private:
    bool running = false;

    float tps;
    float tickDuration;
    float accumulator = 0.f;

    int currentZoomLevel = 4;

    Vec2i spawnPoint = Vec2i::ZERO;

    std::chrono::high_resolution_clock::time_point timer;
    std::chrono::milliseconds updateTime;
    std::chrono::milliseconds renderTime;

    // Sets the session's spawn point based on the middle of the world and the lowest position from there.
    void SetSpawnPoint();

    void UpdateGame();
    void RenderGame();
    void RenderGameUI();
    void RenderPausedUI();

    // Saves all relevant data from the GameplaySession to a file with the given name.
    void Save(const char*);
};