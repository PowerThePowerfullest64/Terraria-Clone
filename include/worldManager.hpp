#pragma once

#include <vector>
#include <stdint.h>

#include "raylib.h"

#include "vec2f.h"

enum BlockColor : uint8_t {
    AIR,
    DIRT,
    GRASS,
    STONE,
    SAND,
    ColorCount
};

class WorldManager {
public:
    const uint16_t WIDTH = 60, HEIGHT = 33;
    const uint8_t blockSize = 24;

    WorldManager();
    ~WorldManager();

    // Gets a copy of a block at a given position.
    uint8_t GetBlock(size_t index) { return blocks[index]; }
    uint8_t GetBlock(size_t x, size_t y) { return blocks[Index(x, y)]; }
    uint8_t GetBlock(const Vec2f& pos) { return blocks[Index((int)pos.x, (int)pos.y)]; }
    uint8_t GetBlockWorld(const Vec2f& pos) { return GetBlock(pos / (float)blockSize); }

    // Sets a block at a given position to a given type.
    void SetBlock(size_t index, uint8_t type) { blocks[index] = type; }
    void SetBlock(size_t x, size_t y, uint8_t type) { blocks[Index(x, y)] = type; }

    // Renders every block in the world.
    void Render();

private:
    std::vector<uint8_t> blocks;

    // Turns a 2D-coordinate into a 1D-index.
    size_t Index(size_t x, size_t y) { return y * WIDTH + x; }

    // Returns a color based on the given type, like a map.
    Color blockColors[ColorCount] = {
        {0, 0, 0, 0}, // AIR
        {71, 33, 0, 255}, // DIRT
        {18, 153, 0, 255}, // GRASS
        {77, 77, 77, 255}, // STONE
        {189, 183, 66, 255} // SAND
    };
};