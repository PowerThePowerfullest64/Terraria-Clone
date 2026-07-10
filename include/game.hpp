#pragma once

#include <stdint.h>
#include <chrono>
#include <string>

#include "raylib.h"

class GameplaySession;

enum class GameState {
    MENU,
    GAME,
    PAUSED,
    SAVING
};

class Game {
public:
    const std::string worldsPath = "Worlds/";

    GameState gs = GameState::MENU;

    GameplaySession* gameplaySession = nullptr;

    std::vector<float> zoomLevels = {
        //0.005f, debug levels, very far
        //0.1f,
        //0.25f,
        0.5f,
        0.625f,
        0.875f,
        1.f,
        2.f,
        3.f,
        4.f,
        6.f
    };
    int maxZoomIndex = zoomLevels.size()-1;

    Game();
    ~Game();

    void Run();
    

private:
    bool running = false;
    
    void RenderMenuUI();
};