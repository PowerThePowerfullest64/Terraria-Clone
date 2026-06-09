#pragma once

#include <vector>
#include <stdint.h>

#include "raylib.h"

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
    const uint16_t WIDTH = 360, HEIGHT = 202;
    const uint8_t blockSize = 4;

    WorldManager();
    ~WorldManager();

    // get / set
    uint8_t GetBlock(size_t index) { return blocks[index]; }
    uint8_t GetBlock(size_t x, size_t y) { return blocks[Index(x, y)]; }
    void SetBlock(size_t index, uint8_t type) { blocks[index] = type; }
    void SetBlock(size_t x, size_t y, uint8_t type) { blocks[Index(x, y)] = type; }

    void Render();

private:
    std::vector<uint8_t> blocks;

    size_t Index(size_t x, size_t y) { return y * WIDTH + x; }

    Color blockColors[ColorCount] = {
        {0, 0, 0, 0}, // AIR
        {71, 33, 0, 255}, // DIRT
        {18, 153, 0, 255}, // GRASS
        {77, 77, 77, 255}, // STONE
        {189, 183, 66, 255} // SAND
    };
};