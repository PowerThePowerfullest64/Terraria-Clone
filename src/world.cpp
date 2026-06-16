#include "world.hpp"

#include <iostream>

#include "raylib.h"
#include "FastNoiseLite.h"

#include "random.h"

#include "game.hpp"
#include "textureManager.hpp"

World::World(Game* game) :
    game(game) {

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

void World::Generate(uint16_t seed) {
    for (Chunk& chunk : chunks) { chunk.Reset(); } // Reset old world if any existed.

    if (seed == UINT16_MAX) seed = randomInt(0, UINT16_MAX);

    FastNoiseLite noise;
    noise.SetSeed(seed);
    noise.SetNoiseType(FastNoiseLite::NoiseType_ValueCubic);
    noise.SetFrequency(0.0025f);
    noise.SetFractalOctaves(24);

    FastNoiseLite noise2;
    noise2.SetSeed(seed+1);
    noise2.SetNoiseType(FastNoiseLite::NoiseType_ValueCubic);
    noise2.SetFrequency(0.02f);
    noise2.SetFractalOctaves(24);

    FastNoiseLite noise3;
    noise3.SetSeed(seed+2);
    noise3.SetNoiseType(FastNoiseLite::NoiseType_ValueCubic);
    noise3.SetFrequency(0.04);
    noise3.SetFractalOctaves(24);

    FastNoiseLite noise4;
    noise4.SetSeed(seed+3);
    noise4.SetNoiseType(FastNoiseLite::NoiseType_ValueCubic);
    noise4.SetFrequency(0.08f);
    noise4.SetFractalOctaves(24);

    for (int x = 0; x < WIDTH * Chunk::WIDTH; ++x) {
        float n = noise.GetNoise((float)x, 0.f) * 1.5f +
        noise2.GetNoise((float)x, 0.f) * 0.25f +
        noise3.GetNoise((float)x, 0.f) * 0.15f +
        noise4.GetNoise((float)x, 0.f) * 0.075f;
        float value = n;

        int height = (int)(value * 300.f) + 700;
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
    for (Chunk& chunk : chunks) chunk.Render(game->cam, game->SCREEN_WIDTH, game->SCREEN_HEIGHT);
}