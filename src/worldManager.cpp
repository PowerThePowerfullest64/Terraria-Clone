#include "worldManager.hpp"

#include <iostream>

#include "raylib.h"

WorldManager::WorldManager() {
    blocks.resize(WIDTH * HEIGHT, AIR);

    for (size_t y = 0; y < HEIGHT; ++y)
    for (size_t x = 0; x < WIDTH; ++x) {
        if (y == 150) SetBlock(x, y, GRASS);
        if (y > 150) SetBlock(x, y, DIRT);
        if (y > 160) SetBlock(x, y, STONE);
    }

    std::cout << "Constructed WorldManager!\n";
}

WorldManager::~WorldManager() {
    std::cout << "Deconstruced WorldManager!\n";
}

void WorldManager::Render() {
    for (size_t y = 0; y < HEIGHT; ++y)
    for (size_t x = 0; x < WIDTH; ++x) {
        DrawRectangle(x, y, blockSize, blockSize, blockColors[GetBlock(x, y)]);
    }
}