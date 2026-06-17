#pragma once

#include <stdint.h>
#include <chrono>

#include "raylib.h"

class GameplaySession;

enum class GameState {
    MENU,
    GAME,
    PAUSED
};

class Game {
public:
    const uint16_t SCREEN_WIDTH = 1440u, SCREEN_HEIGHT = 810u;


    const float GRAVITY = 2000.f;
    GameState gs = GameState::MENU;

    GameplaySession* gameplaySession = nullptr;

    float zoomLevels[9];

    Game();
    ~Game();

    void Run();
    

private:
    bool running = false;
    
    void RenderMenuUI();
};