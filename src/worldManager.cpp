#include "worldManager.hpp"

#include <iostream>

#include "raylib.h"

WorldManager::WorldManager() {
    blocks.resize(WIDTH * HEIGHT, AIR);

    for (size_t y = 0; y < HEIGHT; ++y)
    for (size_t x = 0; x < WIDTH; ++x) {
        if (y == 14 && x == 30) SetBlock(x, y, SAND);
        if (y == 15) SetBlock(x, y, GRASS);
        if (y > 15) SetBlock(x, y, DIRT);
        if (y > 20) SetBlock(x, y, STONE);
    }

    std::cout << "Constructed WorldManager!\n";
}

WorldManager::~WorldManager() {
    std::cout << "Deconstruced WorldManager!\n";
}

void WorldManager::Render() {
    for (size_t y = 0; y < HEIGHT; ++y)
    for (size_t x = 0; x < WIDTH; ++x) {
        uint8_t type = GetBlock(x, y);

        if (type == AIR) continue;

        DrawRectangle(x * blockSize, y * blockSize, blockSize, blockSize, blockColors[type]);
    }
}