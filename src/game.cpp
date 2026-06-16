#include "game.hpp"

#include <iostream>
#include <chrono>
#include <string>

#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#define RAYGUI_SUPPORT_ICONS
#include "raygui.h"

#include "entity.hpp"
#include "player.hpp"

#include "textureManager.hpp"
#include "inputManager.hpp"
#include "entityManager.hpp"

Game::Game() {
    // do stuff here
}

void UpdateCamera(Camera2D& cam, Entity* target, float dt) {
    if (target == nullptr) return;

    float baseSpeed = 7.5f * sqrtf(cam.zoom);

    float blendFactor = 1.f - std::expf(-baseSpeed * dt);

    cam.target = {std::lerp(cam.target.x, target->position.x, blendFactor), std::lerp(cam.target.y, target->position.y, blendFactor)};
}

void Game::Run(bool debug) {
    debugMode = debug;
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

    SetTPS(240.f);

    while (running && !WindowShouldClose()) {
        InputManager::Update();

        if (InputManager::togglePausedReleased) {
            if (gs == GameState::GAME) gs = GameState::PAUSED;
            else if (gs == GameState::PAUSED) gs = GameState::GAME;
        }

        BeginDrawing();

        switch (gs)
        {
        case GameState::MENU:
            RenderMenuUI();

            break;
        case GameState::GAME:
            UpdateGame();

            RenderGame();
            RenderGameUI();

            break;
        case GameState::PAUSED:
            RenderGame();
            RenderPausedUI();

            break;
        
        default:
            running = false; // No gamestate set, exit because wth?
        }

        EndDrawing();
    }

    TextureManager::UnloadTextures();

    CloseWindow();
}

void Game::SetTPS(float targetTps) {
    tps = targetTps;
    tickDuration = 1.f / tps;
}

void Game::UpdateGame() {
    timer = std::chrono::high_resolution_clock::now();

    if (InputManager::zoomInPressed) { currentZoomLevel++; currentZoomLevel = std::min(currentZoomLevel, 6); cam.zoom = zoomLevels[currentZoomLevel]; };
    if (InputManager::zoomOutPressed) { currentZoomLevel--; currentZoomLevel = std::max(currentZoomLevel, 0); cam.zoom = zoomLevels[currentZoomLevel]; }

    float dt = GetFrameTime();

    accumulator += dt;
    while (accumulator >= tickDuration) {
        EntityManager::Update(tickDuration);

        accumulator -= tickDuration;
    }

    UpdateCamera(cam, player, dt);

    // Get update time
    auto now = std::chrono::high_resolution_clock::now();
    updateTime = std::chrono::duration_cast<std::chrono::milliseconds>(now - timer);
}

void Game::RenderGame() {
    timer = std::chrono::high_resolution_clock::now();

    ClearBackground(DARKBLUE);

    Camera2D renderCam = cam;
    renderCam.target = {roundf(cam.target.x), roundf(cam.target.y)};

    BeginMode2D(debugMode ? editorCam : renderCam);

    world->Render();
    EntityManager::Render();

    if (debugMode) {
        float viewW = SCREEN_WIDTH / cam.zoom;
        float viewH = SCREEN_HEIGHT / cam.zoom;

        float viewX = cam.target.x - viewW * 0.5f;
        float viewY = cam.target.y - viewH * 0.5f;

        DrawRectangleLines(viewX, viewY, viewW, viewH, RED);
    }

    EndMode2D();
}

void Game::RenderGameUI() {
    // Get render time
    auto now = std::chrono::high_resolution_clock::now();
    renderTime = std::chrono::duration_cast<std::chrono::milliseconds>(now - timer);
    
    DrawFPS(4, 4);

    std::string ut = "Update = " + std::to_string(updateTime.count()) + " ms";
    DrawText(ut.c_str(), 4, 26, 14, WHITE);
    std::string rt = "Render = " + std::to_string(renderTime.count()) + " ms";
    DrawText(rt.c_str(), 4, 48, 14, WHITE);
}

void Game::RenderMenuUI() {
    ClearBackground(GRAY);

    int buttonWidth = 256, buttonHeight = 48;

    float x = SCREEN_WIDTH / 2.f - buttonWidth / 2.f;
    float y = SCREEN_HEIGHT / 2.f - buttonHeight;

    if (GuiButton(Rectangle{x, y, (float)buttonWidth, (float)buttonHeight}, "Play")) {
        gs = GameState::GAME;
        
        world = new World(this);
        world->Generate();

        player = EntityManager::CreateEntity<Player>(this, Vec2f{96000.f, 0.f});
    }

    if (GuiButton(Rectangle{x, y + buttonHeight + 16, (float)buttonWidth, (float)buttonHeight}, "Exit")) {
        if (world != nullptr) {
            delete world;
            world = nullptr;
        }

        if (player != nullptr) {
            player = nullptr;
        }

        running = false;
    }
}

void Game::RenderPausedUI() {
    int buttonWidth = 256, buttonHeight = 48;

    float x = SCREEN_WIDTH / 2.f - buttonWidth / 2.f;
    float y = SCREEN_HEIGHT / 2.f - buttonHeight;

    if (GuiButton(Rectangle{x, y, (float)buttonWidth, (float)buttonHeight}, "Resume")) {
        gs = GameState::GAME;
    }

    if (GuiButton(Rectangle{x, y + buttonHeight + 16, (float)buttonWidth, (float)buttonHeight}, "Exit to Menu")) {
        gs = GameState::MENU;

        if (world != nullptr) {
            delete world;
            world = nullptr;
        }

        if (player != nullptr) {
            EntityManager::ClearEntities();
            player = nullptr;
        }
    }
}