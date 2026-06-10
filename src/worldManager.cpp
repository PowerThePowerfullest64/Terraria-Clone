#include "worldManager.hpp"

#include <iostream>

#include "raylib.h"
#include "FastNoiseLite.h"

#include "random.h"

#include "game.hpp"

WorldManager::WorldManager(Game* game) :
    game(game) {

    blocks.resize(WIDTH * HEIGHT, AIR);

    FastNoiseLite noise;
    noise.SetSeed(0);
    noise.SetNoiseType(FastNoiseLite::NoiseType_ValueCubic);
    noise.SetFrequency(0.005f);
    noise.SetFractalOctaves(24);

    FastNoiseLite noise2;
    noise2.SetSeed(1);
    noise2.SetNoiseType(FastNoiseLite::NoiseType_ValueCubic);
    noise2.SetFrequency(0.04f);
    noise2.SetFractalOctaves(24);

    FastNoiseLite noise3;
    noise3.SetSeed(2);
    noise3.SetNoiseType(FastNoiseLite::NoiseType_ValueCubic);
    noise3.SetFrequency(0.08);
    noise3.SetFractalOctaves(24);

    FastNoiseLite noise4;
    noise4.SetSeed(3);
    noise4.SetNoiseType(FastNoiseLite::NoiseType_ValueCubic);
    noise4.SetFrequency(0.16f);
    noise4.SetFractalOctaves(24);

    for (int x = 0; x < WIDTH; ++x) {
        float n = noise.GetNoise((float)x, 0.f) * 1.5f +
        noise2.GetNoise((float)x, 0.f) * 0.25f +
        noise3.GetNoise((float)x, 0.f) * 0.15f +
        noise4.GetNoise((float)x, 0.f) * 0.075f;
        float value = n;

        int height = (int)(value * 160.f) + 180;
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
    blockTextures[1] = game->tm.Get("dirt");
    blockTextures[2] = game->tm.Get("grass");
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