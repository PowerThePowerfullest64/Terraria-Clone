#include "game.hpp"

#include <iostream>

#include "raylib.h"

#include "entity.hpp"

void Game::Run() {
    running = true;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Terraria Clone Haha");
    SetTargetFPS(60);

    tm.LoadTextures();

    SetTPS(60.f);

    Entity* player = em.CreateEntity<Player>(this, Vec2f{(float)SCREEN_WIDTH/2.f, (float)SCREEN_HEIGHT/2.f});

    while (running) {
        im.Update();

        accumulator += GetFrameTime();
        while (accumulator >= tickDuration) {
            em.Update(tickDuration);

            accumulator -= tickDuration;
        }

        BeginDrawing();
        ClearBackground(DARKBLUE);

        em.Render();
        //wm.Render();

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