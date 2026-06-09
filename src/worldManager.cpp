#include "worldManager.hpp"

#include <iostream>

#include "raylib.h"
#include "FastNoiseLite.h"

#include "random.h"

WorldManager::WorldManager() {
    blocks.resize(WIDTH * HEIGHT, AIR);

    noise.SetSeed(0);
    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    noise.SetFrequency(0.005f);
    noise.SetFractalOctaves(24);


    for (size_t x = 0; x < WIDTH; ++x) {
        float value = powf(noise.GetNoise((float)x, 0.f) + 1.f, 4.f) / 2.f;
        int height = (int)(value * 75.f) + 20;

        for (size_t y = 0; y < height; ++y) {
            SetBlock(x, HEIGHT-y, DIRT);
        }
    }

    for (size_t y = 0; y < HEIGHT; ++y)
    for (size_t x = 0; x < WIDTH; ++x) {
        BlockType type = GetBlock(x, y);
        if (type == AIR) continue;

        float depth = 0.f;
        for (int h = y-1; h >= 0; --h) {
            BlockType otherType = GetBlock(x, h);

            if (otherType == AIR) {
                if ((int)depth < 10) SetBlock(x, y, DIRT);
                else SetBlock(x, y, STONE);

                break;
            }

            depth += randomFloat(0.f, 1.5f);
        }

        BlockType newType = GetBlock(x, y);
        if (newType == DIRT) {
            BlockType aboveType = GetBlock(x, y-1);

            if (aboveType == AIR) SetBlock(x, y, GRASS);
        }
    }

    std::cout << "Constructed WorldManager!\n";
}

WorldManager::~WorldManager() {
    std::cout << "Deconstruced WorldManager!\n";
}

void WorldManager::Render() {
    for (size_t y = 0; y < HEIGHT; ++y)
    for (size_t x = 0; x < WIDTH; ++x) {
        BlockType type = GetBlock(x, y);

        if (type == AIR) continue;

        DrawRectangle(x * blockSize, y * blockSize, blockSize, blockSize, blockColors[type]);
    }
}