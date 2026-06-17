#include "world.hpp"

#include <iostream>

#include "raylib.h"
#include "FastNoiseLite.h"

#include "random.h"

#include "gameplaySession.hpp"
#include "game.hpp"
#include "textureManager.hpp"

World::World(GameplaySession* gameplaySession) :
    gameplaySession(gameplaySession) {

    chunks.resize(WIDTH * HEIGHT);

    // Initialize chunks properly
    for (int i = 0; i < WIDTH * HEIGHT; ++i) {
        chunks[i] = Chunk(this, {(float)(i % WIDTH), (float)(i / WIDTH)});
    }
}

World::~World() {
    std::cout << "Deconstructing " << chunks.size() << " chunks! (" << chunks.size() * Chunk::WIDTH * Chunk::HEIGHT << " blocks)\n";
    std::cout << "Deconstructing World!\n";
}

void World::Generate(int seed) {
    for (Chunk& chunk : chunks) { chunk.Reset(); } // Reset old world if any existed.

    if (seed == INT32_MAX) seed = randomInt(INT32_MIN, INT32_MAX);

    FastNoiseLite noise;
    noise.SetSeed(seed);
    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin); // better than ValueCubic for terrain
    noise.SetFractalType(FastNoiseLite::FractalType_FBm);
    noise.SetFractalOctaves(12);

    for (int x = 0; x < WIDTH * Chunk::WIDTH; ++x) {
        float fx = (float)x;

        float base   = noise.GetNoise(fx * 0.001f, 0.0f);
        float hills  = noise.GetNoise(fx * 0.005f, 500.0f);
        float detail = noise.GetNoise(fx * 0.02f, 1000.0f);

        float value =
            base   * 1.4f +
            hills  * 0.9f +
            detail * 0.25f;

        int height = (int)(value * 2000.0f) + 1500;

        heightMap[x] = height;

        for (int y = 0; y < height; ++y) {
            if (y < height - quartzDepth)
                SetBlock(x, HEIGHT * Chunk::HEIGHT - y, BlockType::QUARTZ);
            else if (y < height - stoneDepth)
                SetBlock(x, HEIGHT * Chunk::HEIGHT - y, BlockType::STONE);
            else
                SetBlock(x, HEIGHT * Chunk::HEIGHT - y, BlockType::DIRT);
        }

        SetBlock(x, HEIGHT * Chunk::HEIGHT - height, BlockType::GRASS);
    }

    std::cout << "Generated World!\n";
}

BlockType World::GetBlock(int x, int y) {
    if (x < 0 || x >= WIDTH * Chunk::WIDTH || y < 0 || y >= HEIGHT * Chunk::HEIGHT)
        return BlockType::AIR;
    
    int chunk =
        (y / Chunk::HEIGHT) * WIDTH +
        (x / Chunk::WIDTH);

    uint16_t block =
        (y % Chunk::HEIGHT) * Chunk::WIDTH +
        (x % Chunk::WIDTH);

    return chunks[chunk].GetBlock(block);
}

void World::SetBlock(int x, int y, BlockType type) {
    if (x < 0 || x >= WIDTH * Chunk::WIDTH || y < 0 || y >= HEIGHT * Chunk::HEIGHT)
        return;
    
    int chunk =
        (y / Chunk::HEIGHT) * WIDTH +
        (x / Chunk::WIDTH);

    uint16_t block =
        (y % Chunk::HEIGHT) * Chunk::WIDTH +
        (x % Chunk::WIDTH);
    
    chunks[chunk].SetBlock(block, type);
}

void World::Render() {
    for (Chunk& chunk : chunks) chunk.Render(gameplaySession->cam, gameplaySession->game->SCREEN_WIDTH, gameplaySession->game->SCREEN_HEIGHT);
}