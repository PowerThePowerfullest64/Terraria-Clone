#include "game.h"

#include "raylib.h"

void Game::Run() {
    running = true;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Terraria Clone Haha");
    SetTargetFPS(60);

    SetTPS(20.f);

    em.CreateEntity<Player>(this, Vec2f{(float)SCREEN_WIDTH/2.f, (float)SCREEN_HEIGHT/2.f});

    while (running) {
        im.Update();

        accumulator += GetFrameTime();
        while (accumulator >= tickDuration) {
            em.Update(tickDuration);

            accumulator -= tickDuration;
        }

        BeginDrawing();
        ClearBackground(BLACK);

        em.Render();

        DrawFPS(4, 4);

        EndDrawing();

        if (WindowShouldClose()) running = false;
    }

    CloseWindow();
}

void Game::SetTPS(float targetTps) {
    tps = targetTps;
    tickDuration = 1.f / tps;
}