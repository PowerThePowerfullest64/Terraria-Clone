#include "chunk.hpp"

#include <iostream>

#include "aabb.hpp"

#include "world.hpp"

Chunk::Chunk(World* world, const Vec2f& pos) : 
    world(world),
    pos(pos) {
        blocks.resize(WIDTH * HEIGHT, AIR);
    }

void Chunk::Render(const Camera2D& cam, int sWidth, int sHeight) {
    // The position of the chunk in world space
    Vec2f worldPos;
    worldPos.x = pos.x * (float)blockSize * WIDTH;
    worldPos.y = pos.y * (float)blockSize * HEIGHT;

    if (dirty) BuildTexture();
    
    if (!IsVisible(cam, sWidth, sHeight)) return;

    DrawTexturePro(
        texture.texture,
        {0.f, 0.f, (float)texture.texture.width, -(float)texture.texture.height},
        {worldPos.x, worldPos.y, (float)texture.texture.width, (float)texture.texture.height},
        {0.f, 0.f},
        0.f,
        WHITE
    );
}

bool Chunk::IsVisible(const Camera2D& cam, int sWidth, int sHeight) {
    float chunkW = WIDTH * blockSize;
    float chunkH = HEIGHT * blockSize;

    AABB chunk = {
        pos.x * chunkW,
        pos.y * chunkH,
        chunkW,
        chunkH
    };

    float viewW = sWidth / cam.zoom;
    float viewH = sHeight / cam.zoom;

    float viewX = cam.target.x - viewW * 0.5f;
    float viewY = cam.target.y - viewH * 0.5f;

    AABB camera = {
        viewX,
        viewY,
        viewW,
        viewH
    };

    return Intersects(chunk, camera);
}

void Chunk::BuildTexture() {
    std::cout << "Building Chunk Texture.\n";
    
    BeginTextureMode(texture);
    ClearBackground(BLANK);

    for (int y = 0; y < HEIGHT; ++y)
    for (int x = 0; x < WIDTH; ++x) {
        BlockType type = GetBlock(x, y);
        if (type == AIR) continue;

        DrawTexture(
            *world->blockTextures[type],
            x * blockSize,
            y * blockSize,
            WHITE
        );
    }

    EndTextureMode();
    dirty = false;

    std::cout << "Finished Building Chunk Texture.\n";
}