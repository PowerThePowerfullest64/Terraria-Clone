#include "game.hpp"

#include <iostream>
#include <chrono>
#include <string>

#include "raylib.h"

#include "entity.hpp"
#include "player.hpp"

#include "textureManager.hpp"
#include "inputManager.hpp"
#include "entityManager.hpp"

Game::Game() :
    world(this) {}

void UpdateCamera(Camera2D& cam, Entity* target, float dt) {
    float baseSpeed = 7.5f * sqrtf(cam.zoom);

    float blendFactor = 1.f - std::expf(-baseSpeed * dt);

    cam.target = {std::lerp(cam.target.x, target->position.x, blendFactor), std::lerp(cam.target.y, target->position.y, blendFactor)};
}

void Game::Run(bool debug) {
    debugMode = debug;
    running = true;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Terraria Clone Haha");
    SetTargetFPS(60);

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
    world.SetBlockTextures();

    SetTPS(240.f);

    player = EntityManager::CreateEntity<Player>(this, Vec2f{96000.f, 0.f});

    while (running) {
        Update();

        RenderGame();
        RenderUI();

        if (WindowShouldClose()) running = false;
    }

    TextureManager::UnloadTextures();

    CloseWindow();
}

void Game::SetTPS(float targetTps) {
    tps = targetTps;
    tickDuration = 1.f / tps;
}

void Game::Update() {
    timer = std::chrono::high_resolution_clock::now();

    InputManager::Update();

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

    BeginDrawing();
    ClearBackground(DARKBLUE);

    Camera2D renderCam = cam;
    renderCam.target = {roundf(cam.target.x), roundf(cam.target.y)};

    BeginMode2D(debugMode ? editorCam : renderCam);

    world.Render();
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

void Game::RenderUI() {
    // Get render time
    auto now = std::chrono::high_resolution_clock::now();
    renderTime = std::chrono::duration_cast<std::chrono::milliseconds>(now - timer);
    
    DrawFPS(4, 4);

    std::string ut = "Update = " + std::to_string(updateTime.count()) + " ms";
    DrawText(ut.c_str(), 4, 26, 14, WHITE);
    std::string rt = "Render = " + std::to_string(renderTime.count()) + " ms";
    DrawText(rt.c_str(), 4, 48, 14, WHITE);

    EndDrawing();
}