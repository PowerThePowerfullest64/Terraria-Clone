#include "game.hpp"

#include <iostream>
#include <chrono>
#include <string>
#include <cassert>

#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#define RAYGUI_SUPPORT_ICONS
#include "raygui.h"

#include "entity.hpp"
#include "player.hpp"

#include "gameplaySession.hpp"
#include "textureManager.hpp"
#include "inputManager.hpp"
#include "entityManager.hpp"

Game::Game() {
    // do stuff here
}

Game::~Game() {
    if (gameplaySession != nullptr) {
        delete gameplaySession;
        gameplaySession = nullptr;
    }
}

void Game::Run() {
    running = true;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Terraria Clone Haha");
    SetTargetFPS(60);
    SetExitKey(KEY_P);

    // zoom levels
    {
        zoomLevels[0] = 0.0025f;
        zoomLevels[1] = 0.025f;
        zoomLevels[2] = 0.5f;
        zoomLevels[3] = 1.f;
        zoomLevels[4] = 2.f;
        zoomLevels[5] = 4.f;
        zoomLevels[6] = 6.f;
    }

    TextureManager::LoadTextures();
    TextureManager::SetBlockTextures();

    while (running && !WindowShouldClose()) {
        switch (gs)
        {
        case GameState::GAME:
            assert(gameplaySession != nullptr);

            gameplaySession->Run();

            // Done running, delete it!
            delete gameplaySession;
            gameplaySession = nullptr;

            break;

        case GameState::MENU:
            BeginDrawing();
            RenderMenuUI();
            EndDrawing();

            break;
        
        default:
            running = false;
            std::cout << "Menu has no valid GameState (MENU, GAME); closing game.\n";

            break;
        }
    }

    TextureManager::UnloadTextures();

    CloseWindow();
}

void Game::RenderMenuUI() {
    ClearBackground(GRAY);

    int buttonWidth = 256, buttonHeight = 48;

    float x = SCREEN_WIDTH / 2.f - buttonWidth / 2.f;
    float y = SCREEN_HEIGHT / 2.f - buttonHeight;

    if (GuiButton(Rectangle{x, y, (float)buttonWidth, (float)buttonHeight}, "Play")) {
        gs = GameState::GAME;
        
        gameplaySession = new GameplaySession(this);
    }

    if (GuiButton(Rectangle{x, y + buttonHeight + 16, (float)buttonWidth, (float)buttonHeight}, "Exit")) {
        if (gameplaySession != nullptr) {
            delete gameplaySession;
            gameplaySession = nullptr;
        }

        running = false;
    }
}