#include "game.hpp"

#include <iostream>
#include <chrono>
#include <string>
#include <cassert>
#include <fstream>

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
#include "settings.hpp"

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

    InitWindow(Settings::windowWidth, Settings::windowHeight, "Terraria Clone Haha");
    SetTargetFPS(165);
    SetExitKey(KEY_NULL);

    TextureManager::LoadTextures();
    TextureManager::LoadFonts();
    TextureManager::SetBlockTextures();

    LoadBlockData();

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
    TextureManager::UnloadFonts();

    CloseWindow();
}

void Game::RenderMenuUI() {
    ClearBackground(GRAY);

    int buttonWidth = 256, buttonHeight = 48;

    float x = Settings::windowWidth / 2.f - buttonWidth / 2.f;
    float y = Settings::windowHeight / 2.f - buttonHeight;

    if (GuiButton(Rectangle{x, y, (float)buttonWidth, (float)buttonHeight}, "Play")) {
        gs = GameState::GAME;
        
        gameplaySession = new GameplaySession(this);
    }

    if (GuiButton(Rectangle{x, y + (buttonHeight + 16) * 1, (float)buttonWidth, (float)buttonHeight}, "Load")) {
        std::string texturePath = worldsPath + "world1.bin";
        std::ifstream worldTest(texturePath, std::ios::binary);

        if (worldTest.is_open()) {
            gs = GameState::GAME;
            gameplaySession = new GameplaySession(this, true);
        } else {
            std::cout << "Failed to open save file! Make sure you have a save file in '" << worldsPath << "/' of '.bin' type.\n";
        }
    }

    if (GuiButton(Rectangle{x, y + (buttonHeight + 16) * 2, (float)buttonWidth, (float)buttonHeight}, "Exit")) {
        if (gameplaySession != nullptr) {
            delete gameplaySession;
            gameplaySession = nullptr;
        }

        running = false;
    }
}