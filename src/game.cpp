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
    float moveRate = 7.5f * dt;
    cam.target = {roundf(std::lerp(cam.target.x, target->position.x, moveRate)), roundf(std::lerp(cam.target.y, target->position.y, moveRate))};
}

void Game::Run(bool debug) {
    debugMode = debug;
    running = true;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Terraria Clone Haha");
    SetTargetFPS(60);

    {
        zoomLevels[0] = 0.5f;
        zoomLevels[1] = 0.625f;
        zoomLevels[1] = 0.75f;
        zoomLevels[2] = 1.f;
        zoomLevels[3] = 1.25f;
        zoomLevels[4] = 1.5f;
        zoomLevels[5] = 2.f;
    }

    TextureManager::LoadTextures();
    world.SetBlockTextures();

    SetTPS(240.f);

    Entity* player = EntityManager::CreateEntity<Player>(this, Vec2f{24000.f, 0.f});

    while (running) {
        InputManager::Update();

        if (InputManager::zoomInPressed) { currentZoomLevel++; currentZoomLevel = std::min(currentZoomLevel, 5); cam.zoom = zoomLevels[currentZoomLevel]; };
        if (InputManager::zoomOutPressed) { currentZoomLevel--; currentZoomLevel = std::max(currentZoomLevel, 0); cam.zoom = zoomLevels[currentZoomLevel]; }

        float dt = GetFrameTime();

        auto start = std::chrono::high_resolution_clock::now();

        accumulator += dt;
        while (accumulator >= tickDuration) {
            EntityManager::Update(tickDuration);

            accumulator -= tickDuration;
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto updateTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        UpdateCamera(cam, player, dt);

        start = std::chrono::high_resolution_clock::now();

        BeginDrawing();
        ClearBackground(DARKBLUE);

        BeginMode2D(debugMode ? editorCam : cam);

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

        end = std::chrono::high_resolution_clock::now();
        auto renderTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        DrawFPS(4, 4);

        std::string ut = "Update = " + std::to_string(updateTime.count()) + " ms";
        DrawText(ut.c_str(), 4, 26, 14, WHITE);
        std::string rt = "Render = " + std::to_string(renderTime.count()) + " ms";
        DrawText(rt.c_str(), 4, 48, 14, WHITE);

        EndDrawing();

        if (WindowShouldClose()) running = false;
    }

    TextureManager::UnloadTextures();

    CloseWindow();
}

void Game::SetTPS(float targetTps) {
    tps = targetTps;
    tickDuration = 1.f / tps;
}