#include "game.hpp"

#include <iostream>

#include "raylib.h"

#include "entity.hpp"

Game::Game() :
    wm(this) {}

void UpdateCamera(Camera2D& cam, Entity* target) {
    cam.target = target->position;
}

void Game::Run() {
    running = true;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Terraria Clone Haha");
    SetTargetFPS(60);

    tm.LoadTextures();
    wm.SetBlockTextures();

    SetTPS(60.f);

    Entity* player = em.CreateEntity<Player>(this, Vec2f{100.f, 100.f});
    
    Camera2D cam;
    cam.rotation = 0.f;
    cam.offset = {(float)SCREEN_WIDTH / 2.f, (float)SCREEN_HEIGHT / 2.f};
    cam.zoom = 1.f;
    float zoomSensitivity = 1.f;

    while (running) {
        im.Update();

        if (im.zoomInPressed) cam.zoom *= 1.f + (0.1f * zoomSensitivity);
        if (im.zoomOutPressed) cam.zoom /= 1.f + (0.1f * zoomSensitivity);

        accumulator += GetFrameTime();
        while (accumulator >= tickDuration) {
            em.Update(tickDuration);

            accumulator -= tickDuration;
        }

        UpdateCamera(cam, player);

        BeginDrawing();
        ClearBackground(DARKBLUE);

        BeginMode2D(cam);
        

        wm.Render();
        em.Render();

        EndMode2D();

        DrawFPS(4, 4);

        EndDrawing();

        if (WindowShouldClose()) running = false;
    }

    tm.UnloadTextures();

    CloseWindow();
}

void Game::SetTPS(float targetTps) {
    tps = targetTps;
    tickDuration = 1.f / tps;
}