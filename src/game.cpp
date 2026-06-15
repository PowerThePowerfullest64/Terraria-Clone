#include "game.hpp"

#include <iostream>
#include <chrono>
#include <string>

#include "raylib.h"

#include "entity.hpp"
#include "textureManager.hpp"
#include "inputManager.hpp"
#include "entityManager.hpp"

Game::Game() :
    world(this) {}

void UpdateCamera(Camera2D& cam, Entity* target, float dt) {
    float moveRate = 7.5f * dt;
    cam.target = {std::lerp(cam.target.x, target->position.x, moveRate), std::lerp(cam.target.y, target->position.y, moveRate)};
}

void Game::Run() {
    running = true;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Terraria Clone Haha");
    SetTargetFPS(60);

    TextureManager::LoadTextures();
    world.SetBlockTextures();

    SetTPS(180.f);

    Entity* player = EntityManager::CreateEntity<Player>(this, Vec2f{32.f, 0.f});
    
    cam.target = player->position;
    cam.rotation = 0.f;
    cam.offset = {(float)SCREEN_WIDTH / 2.f, (float)SCREEN_HEIGHT / 2.f};
    cam.zoom = 1.f;
    float zoomSensitivity = 1.f;

    while (running) {
        InputManager::Update();

        if (InputManager::zoomInPressed) cam.zoom *= 1.f + (0.1f * zoomSensitivity);
        if (InputManager::zoomOutPressed) cam.zoom /= 1.f + (0.1f * zoomSensitivity);

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

        BeginMode2D(cam);
        

        world.Render();
        EntityManager::Render();

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