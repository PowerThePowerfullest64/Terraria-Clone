#include "gameplaySession.hpp"

#include <iostream>
#include <cassert>

#include "raylib.h"
#include "raygui.h"

#include "game.hpp"
#include "inputManager.hpp"
#include "entityManager.hpp"
#include "player.hpp"
#include "block.hpp"

void UpdateCamera(Camera2D& cam, Entity* target, float dt) {
    assert(target != nullptr);

    float baseSpeed = 7.5f * sqrtf(cam.zoom);

    float blendFactor = 1.f - std::expf(-baseSpeed * dt);

    cam.target = {std::lerp(cam.target.x, target->position.x, blendFactor), std::lerp(cam.target.y, target->position.y, blendFactor)};
}

GameplaySession::GameplaySession(Game* game) :
    game(game) {

    em = new EntityManager();
    
    world = new World(this);
    world->Generate();

    SetSpawnPoint();
    player = em->CreateEntity<Player>(this, spawnPoint);

    std::cout << "Opened GameplaySession.\n";
}

GameplaySession::~GameplaySession() {
    if (world != nullptr) {
        delete world;
        world = nullptr;
    }

    if (em != nullptr) {
        delete em;
        em = nullptr;
    }

    std::cout << "Closed GameplaySession.\n";
}

void GameplaySession::Run() {
    running = true;

    SetTPS(240.f);

    while (running && !WindowShouldClose()) {
        InputManager::Update();

        if (InputManager::togglePausedReleased) {
            if (game->gs == GameState::GAME) game->gs = GameState::PAUSED;
            else if (game->gs == GameState::PAUSED) game->gs = GameState::GAME;
        }

        BeginDrawing();

        switch (game->gs)
        {
        case GameState::MENU:
            running = false;

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
}

void GameplaySession::SetTPS(float targetTps) {
    tps = targetTps;
    tickDuration = 1.f / tps;
}

void GameplaySession::SetSpawnPoint() {
    spawnPoint.x = world->WIDTH * Chunk::WIDTH / 2;
    
    for (int y = 0; y < world->HEIGHT * Chunk::HEIGHT; ++y) {
        if (world->GetBlock(spawnPoint.x, y) != BlockType::AIR) {
            y -= 2; // Reverse 2 position back. (1 block above ground)

            spawnPoint.y = y;
            break;
        }
    }

    spawnPoint = World::FromWorld(spawnPoint);

    std::cout << "Found spawn point!\n";
}

void GameplaySession::UpdateGame() {
    timer = std::chrono::high_resolution_clock::now();

    if (InputManager::zoomInPressed) { currentZoomLevel++; currentZoomLevel = std::min(currentZoomLevel, 6); cam.zoom = game->zoomLevels[currentZoomLevel]; };
    if (InputManager::zoomOutPressed) { currentZoomLevel--; currentZoomLevel = std::max(currentZoomLevel, 0); cam.zoom = game->zoomLevels[currentZoomLevel]; }

    float dt = GetFrameTime();

    accumulator += dt;
    while (accumulator >= tickDuration) {
        em->Update(tickDuration);

        accumulator -= tickDuration;
    }

    UpdateCamera(cam, player, dt);

    // Get update time
    auto now = std::chrono::high_resolution_clock::now();
    updateTime = std::chrono::duration_cast<std::chrono::milliseconds>(now - timer);
}

void GameplaySession::RenderGame() {
    timer = std::chrono::high_resolution_clock::now();

    ClearBackground(DARKBLUE);

    Camera2D renderCam = cam;
    renderCam.target = {roundf(cam.target.x), roundf(cam.target.y)};

    BeginMode2D(renderCam);

    world->Render();
    em->Render();

    EndMode2D();
}

void GameplaySession::RenderGameUI() {
    // Get render time
    auto now = std::chrono::high_resolution_clock::now();
    renderTime = std::chrono::duration_cast<std::chrono::milliseconds>(now - timer);
    
    DrawFPS(4, 4);

    std::string ut = "Update = " + std::to_string(updateTime.count()) + " ms";
    DrawText(ut.c_str(), 4, 26, 14, WHITE);
    std::string rt = "Render = " + std::to_string(renderTime.count()) + " ms";
    DrawText(rt.c_str(), 4, 48, 14, WHITE);
}

void GameplaySession::RenderPausedUI() {
    int buttonWidth = 256, buttonHeight = 48;

    float x = game->SCREEN_WIDTH / 2.f - buttonWidth / 2.f;
    float y = game->SCREEN_HEIGHT / 2.f - buttonHeight;

    if (GuiButton(Rectangle{x, y, (float)buttonWidth, (float)buttonHeight}, "Resume")) {
        game->gs = GameState::GAME;
    }

    if (GuiButton(Rectangle{x, y + buttonHeight + 16, (float)buttonWidth, (float)buttonHeight}, "Exit to Menu")) {
        game->gs = GameState::MENU;
        running = false;
    }
}