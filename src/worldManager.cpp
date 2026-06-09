#include "worldManager.hpp"

#include <iostream>

#include "raylib.h"
#include "FastNoiseLite.h"

#include "random.h"

#include "game.hpp"

WorldManager::WorldManager(Game* game) :
    game(game) {

    blocks.resize(WIDTH * HEIGHT, AIR);

    noise.SetSeed(0);
    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    noise.SetFrequency(0.03f);
    noise.SetFractalOctaves(24);

    for (int x = 0; x < WIDTH; ++x) {
        float n = noise.GetNoise((float)x, 0.f);
        float value = powf(n * 0.5f + 0.5f, 5.f);

        int height = (int)(value * 100.f) + 75;
        heightMap[x] = height;

        for (int y = 0; y < height; ++y) {
            if (y < height - quartzDepth)
                SetBlock(x, HEIGHT - y, QUARTZ);
            else if (y < height - stoneDepth)
                SetBlock(x, HEIGHT - y, STONE);
            else
                SetBlock(x, HEIGHT - y, DIRT);
        }

        SetBlock(x, HEIGHT - height, GRASS);
    }

    std::cout << "Constructed WorldManager!\n";
}

WorldManager::~WorldManager() {
    std::cout << "Deconstruced WorldManager!\n";
}

void WorldManager::SetBlockTextures() {
    blockTextures[0] = game->tm.Get("noTexture"); // air
    blockTextures[1] = game->tm.Get("grass");
    blockTextures[2] = game->tm.Get("dirt");
    blockTextures[3] = game->tm.Get("stone");
    blockTextures[4] = game->tm.Get("sand");
    blockTextures[5] = game->tm.Get("quartz");
}

void WorldManager::Render() {
    for (size_t y = 0; y < HEIGHT; ++y)
    for (size_t x = 0; x < WIDTH; ++x) {
        BlockType type = GetBlock(x, y);

        if (type == AIR) continue;

        DrawTexture(*blockTextures[type], x * blockSize, y * blockSize, WHITE);
    }
}