#pragma once

#include <vector>
#include <stdint.h>

#include "raylib.h"

#include "vec2f.h"

enum BlockType : uint8_t {
    AIR,
    DIRT,
    GRASS,
    STONE,
    SAND,
    COUNT
};

class WorldManager {
public:
    static constexpr uint16_t WIDTH = 20, HEIGHT = 30;
    static constexpr uint8_t blockSize = 24;

    WorldManager();
    ~WorldManager();

    // Gets a copy of a block at a given position.
    BlockType GetBlock(size_t index) { if (index >= WIDTH * HEIGHT) return AIR; return (BlockType)blocks[index]; }
    BlockType GetBlock(int x, int y) { if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return AIR; return GetBlock(Index(x, y)); }
    BlockType GetBlock(const Vec2f& pos) { return GetBlock(pos.x, pos.y); }
    BlockType GetBlockWorld(const Vec2f& pos) { return GetBlock(pos / (float)blockSize); }

    // Sets a block at a given position to a given type.
    void SetBlock(size_t index, BlockType type) { if (index >= WIDTH * HEIGHT) return; blocks[index] = type; }
    void SetBlock(int x, int y, BlockType type) { SetBlock(Index(x, y), type); }

    // Renders every block in the world.
    void Render();

private:
    std::vector<uint8_t> blocks;

    // Turns a 2D-coordinate into a 1D-index.
    size_t Index(int x, int y) { return static_cast<size_t>(y * WIDTH + x); }

    // Returns a color based on the given type, like a map.
    Color blockColors[COUNT] = {
        {0, 0, 0, 0}, // AIR
        {71, 33, 0, 255}, // DIRT
        {18, 153, 0, 255}, // GRASS
        {77, 77, 77, 255}, // STONE
        {189, 183, 66, 255} // SAND
    };
};