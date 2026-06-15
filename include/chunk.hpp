#pragma once

#include <stdint.h>
#include <vector>

#include "raylib.h"

#include "vec2f.h"

class World;

enum BlockType : uint8_t {
    AIR,
    DIRT,
    GRASS,
    STONE,
    SAND,
    QUARTZ,
    COUNT
};

class Chunk {
public:
    // Width and height of the chunk in blocks
    static constexpr int WIDTH = 16, HEIGHT = 16;
    static constexpr int blockSize = 24;

    Chunk(World* world = nullptr, const Vec2f& pos = Vec2f::ZERO);

    // Gets a copy of a block at a given position.
    BlockType GetBlock(size_t index) { if (index >= WIDTH * HEIGHT) return AIR; return (BlockType)blocks[index]; }
    BlockType GetBlock(int x, int y) { if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return AIR; return GetBlock(Index(x, y)); }
    BlockType GetBlock(const Vec2f& pos) { return GetBlock(Index(pos)); }

    // Sets a block at a given position to a given type.
    void SetBlock(size_t index, BlockType type) { if (index >= WIDTH * HEIGHT) return; blocks[index] = type; }
    void SetBlock(int x, int y, BlockType type) { SetBlock(Index(x, y), type); }

    // Renders every block in the chunk.
    void Render();

private:
    World* world;
    // Chunk position in chunk coordinates
    Vec2f pos;

    std::vector<BlockType> blocks;

    // Turns a 2D-coordinate into a 1D-index.
    size_t Index(int x, int y) { return static_cast<size_t>(y * WIDTH + x); }
    size_t Index(const Vec2f& pos) { return static_cast<size_t>((int)pos.y * WIDTH + (int)pos.x); }
};