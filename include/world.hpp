#pragma once

#include <vector>
#include <stdint.h>

#include "raylib.h"

#include "vec2f.h"

#include "chunk.hpp"
#include "block.hpp"

class GameplaySession;

class World {
public:
    static constexpr int WIDTH = 522, HEIGHT = 150;

    World(GameplaySession* gameplaySession);
    ~World();

    // Gets a copy of a block at a given position.
    BlockType GetBlock(int x, int y);
    BlockType GetBlock(const Vec2f& pos) { return GetBlock((int)pos.x, (int)pos.y); }
    BlockType GetBlockWorld(const Vec2f& pos) { return GetBlock(pos / (float)Chunk::blockSize); }

    // Sets a block at a given position to a given type.
    void SetBlock(int x, int y, BlockType type);

    // Renders every block in the world.
    void Render();

    // Generates a random world, random seed if none is provided or seed == UINT16_MAX.
    void Generate(int seed = INT32_MAX);

private:
    GameplaySession* gameplaySession;
    std::vector<Chunk> chunks;
    int heightMap[WIDTH * Chunk::WIDTH];

    int stoneDepth = 10;
    int quartzDepth = 1500;

    // Turns a 2D-coordinate into a 1D-index.
    size_t Index(int x, int y) { return static_cast<size_t>(y * WIDTH + x); }
};