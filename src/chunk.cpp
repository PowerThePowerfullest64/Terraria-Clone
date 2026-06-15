#include "chunk.hpp"

#include <iostream>

#include "world.hpp"

Chunk::Chunk(World* world, const Vec2f& pos) : 
    world(world),
    pos(pos) {
        blocks.resize(WIDTH * HEIGHT, AIR);
    }

void Chunk::Render() {
    // The position of the chunk in world space
    Vec2f worldPos;
    worldPos.x = pos.x * (float)blockSize * WIDTH;
    worldPos.y = pos.y * (float)blockSize * HEIGHT;
    
    for (size_t y = 0; y < HEIGHT; ++y)
    for (size_t x = 0; x < WIDTH; ++x) {
        BlockType type = GetBlock(x, y);

        if (type == AIR) continue;

        DrawTexture(
            *(world->blockTextures[type]),
            worldPos.x + x * blockSize,
            worldPos.y + y * blockSize,
            WHITE
        );
    }

    std::cout << worldPos.x << ", " << worldPos.y << "\n";
}