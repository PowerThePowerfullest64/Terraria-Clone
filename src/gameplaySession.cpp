#include "gameplaySession.hpp"

#include <iostream>
#include <cassert>
#include <fstream>

#include "raylib.h"
#include "raygui.h"

#include "game.hpp"
#include "inputManager.hpp"
#include "entityManager.hpp"
#include "player.hpp"
#include "block.hpp"
#include "settings.hpp"
#include "textureManager.hpp"

void RenderCamera(const Camera2D& cam) {
    float viewW = Settings::windowWidth / cam.zoom;
    float viewH = Settings::windowHeight / cam.zoom;

    float viewX = cam.target.x - viewW * 0.5f;
    float viewY = cam.target.y - viewH * 0.5f;

    Rectangle camRec = {viewX, viewY, viewW, viewH};

    DrawRectangleLinesEx(camRec, 8.f, YELLOW);
}

void UpdateCamera(Camera2D& cam, Entity* target, float dt) {
    assert(target != nullptr);

    float baseSpeed = 7.5f * sqrtf(cam.zoom);

    float blendFactor = 1.f - std::expf(-baseSpeed * dt);

    cam.target = {std::lerp(cam.target.x, target->position.x, blendFactor), std::lerp(cam.target.y, target->position.y, blendFactor)};
}

GameplaySession::GameplaySession(Game* game, bool load) :
    game(game), console(this) {

    em = new EntityManager();
    
    world = new World(this);

    if (load) {
        Load("world1");
    }
    else {
        world->Generate();
        ComputeSpawnpoint();
        em->CreateEntity<Player>(this, spawnPoint);
    }

    cam = {{(float)Settings::windowWidth / 2.f, (float)Settings::windowHeight / 2.f}, {0.f, 0.f}, 0.f, 1.f};
    testCam = {{(float)Settings::windowWidth / 2.f, (float)Settings::windowHeight / 2.f}, player->position, 0.f, 0.15f};

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

    SetTickRate(240.f);

    while (running && !WindowShouldClose()) {
        InputManager::Update();

        if (InputManager::togglePausedReleased) {
            // If operating the console don't check the next code.
            if (!console.open) {
                if (game->gs == GameState::GAME) game->gs = GameState::PAUSED;
                else if (game->gs == GameState::PAUSED) game->gs = GameState::GAME;
            }
        }

        BeginDrawing();
        ClearBackground(DARKBLUE);

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
        case GameState::SAVING:
            RenderGame();

            // For now, just finish saving immediately, since we aren't waiting for naming.
            Save("world1");
            game->gs = GameState::GAME;
            
            break;
        
        default:
            running = false; // No gamestate set, exit because wth?
        }

        EndDrawing();
    }
}

void GameplaySession::SetTickRate(float targetTickRate) {
    tickRate = targetTickRate;
    tickDuration = 1.f / tickRate;
}

void GameplaySession::ComputeSpawnpoint() {
    spawnPoint.x = world->WIDTH * Chunk::WIDTH / 2;
    
    for (int y = 0; y < world->HEIGHT * Chunk::HEIGHT; ++y) {
        if (world->GetBlock(spawnPoint.x, y) != BlockType::AIR) {
            y -= 2; // Reverse 2 position back. (1 block above ground)

            spawnPoint.y = y;
            break;
        }
    }

    spawnPoint = World::ToWorld(spawnPoint);

    std::cout << "Found spawn point!\n";
}

void GameplaySession::UpdateGame() {
    timer = std::chrono::high_resolution_clock::now();

    if (InputManager::zoomInPressed)
        currentZoomLevel++;
    if (InputManager::zoomOutPressed)
        currentZoomLevel--;

    currentZoomLevel = std::clamp(currentZoomLevel, 0, game->maxZoomIndex);
    cam.zoom = game->zoomLevels[currentZoomLevel];

    console.Update();

    float dt = GetFrameTime();

    accumulator += dt;
    while (accumulator >= tickDuration) {
        em->Update(tickDuration);
        world->Update(tickDuration);

        accumulator -= tickDuration;
    }

    UpdateCamera(cam, player, dt);

    // Get update time
    auto now = std::chrono::high_resolution_clock::now();
    updateTime = std::chrono::duration_cast<std::chrono::milliseconds>(now - timer);
}

void GameplaySession::RenderGame() {
    timer = std::chrono::high_resolution_clock::now();

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
    
    if (Settings::showFPS) {
        DrawTextEx(TextureManager::font, ("FPS: " + std::to_string(GetFPS())).c_str(), {4.f, 4.f}, 20.f, 0.f, WHITE);
    }

    Vector2 mousePos = GetMousePosition();
    Vector2 mouseWorld = GetScreenToWorld2D(mousePos, cam);
    Vec2i worldPos = World::ToBlock(Vec2f::fromVector2(mouseWorld));

    if (Settings::showMouseCoords) {
        std::string text = "(" + std::to_string(worldPos.x) + ", " + std::to_string(worldPos.y) + ")";

        Vector2 textPos = {mousePos.x + 24, mousePos.y - 24};
        
        DrawTextEx(TextureManager::font, text.c_str(), textPos, 20, 0.f, WHITE);
    }

    if (Settings::showMouseBlockType) {
        BlockType type = world->GetBlock(worldPos);

        std::string text = "block: " + typeToName[type];

        Vector2 textPos = {mousePos.x + 24, mousePos.y};
        
        DrawTextEx(TextureManager::font, text.c_str(), textPos, 20.f, 0.f, WHITE);
    }

    std::string ut = "Update = " + std::to_string(updateTime.count()) + " ms";
    DrawTextEx(TextureManager::font, ut.c_str(), {4.f, 26.f}, 20.f, 0.f, WHITE);
    std::string rt = "Render = " + std::to_string(renderTime.count()) + " ms";
    DrawTextEx(TextureManager::font, rt.c_str(), {4.f, 48.f}, 20.f, 0.f, WHITE);
    console.Render();
}

void GameplaySession::RenderPausedUI() {
    int buttonWidth = 256, buttonHeight = 48;

    float x = Settings::windowWidth / 2.f - buttonWidth / 2.f;
    float y = Settings::windowHeight / 2.f - buttonHeight;

    if (GuiButton(Rectangle{x, y, (float)buttonWidth, (float)buttonHeight}, "Resume")) {
        game->gs = GameState::GAME;
    }

    if (GuiButton(Rectangle{x, y + (buttonHeight + 16) * 1, (float)buttonWidth, (float)buttonHeight}, "Save")) {
        game->gs = GameState::SAVING;
    }

    if (GuiButton(Rectangle{x, y + (buttonHeight + 16) * 2, (float)buttonWidth, (float)buttonHeight}, "Exit to Menu")) {
        game->gs = GameState::MENU;
        running = false;
    }
}

void GameplaySession::Save(const std::string& name) {
    std::string texturePath = game->worldsPath + name + ".bin";
    std::ofstream save(texturePath, std::ios::binary);

    if (!save.is_open()) {
        std::cout << "Failed to create save file!\n";
        return;
    }

    save.write(reinterpret_cast<char*>(&currentZoomLevel), sizeof(currentZoomLevel));
    save.write(reinterpret_cast<char*>(&spawnPoint.x), sizeof(spawnPoint.x));
    save.write(reinterpret_cast<char*>(&spawnPoint.y), sizeof(spawnPoint.y));

    world->Save(save);

    em->Save(save);

    if (!save) {
        std::cout << "Failed to write save file!\n";
        running = false;
        return;
    }

    // Do save stuff!
    std::cout << "Saved World!\n";
}

void GameplaySession::Load(const std::string& name) {
    std::string texturePath = game->worldsPath + name + ".bin";
    std::ifstream save(texturePath, std::ios::binary);

    if (!save.is_open()) {
        std::cout << "Failed to open save file! Make sure you have a save file in '" << game->worldsPath << "/' of '.bin' type.\n";
        return;
    }

    save.read(reinterpret_cast<char*>(&currentZoomLevel), sizeof(currentZoomLevel));
    save.read(reinterpret_cast<char*>(&spawnPoint.x), sizeof(spawnPoint.x));
    save.read(reinterpret_cast<char*>(&spawnPoint.y), sizeof(spawnPoint.y));

    world->Load(save);

    em->Load(save, this);

    if (!save) {
        std::cout << "Failed to load save file!\n";
        return;
    }

    std::cout << "Loaded World!\n";
}