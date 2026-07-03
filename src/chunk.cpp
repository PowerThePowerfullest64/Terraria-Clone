#include "chunk.hpp"

#include <iostream>

#include "aabb.hpp"

#include "textureManager.hpp"
#include "debugManager.hpp"

Chunk::Chunk(World* world, const Vec2f& pos) : 
    world(world),
    pos(pos) {
        blocks.resize(WIDTH * HEIGHT, {BlockType::AIR});
    }

void Chunk::Render(const Camera2D& cam, int sWidth, int sHeight) {
    // The position of the chunk in world space
    Vec2f worldPos;
    worldPos.x = pos.x * (float)blockSize * WIDTH;
    worldPos.y = pos.y * (float)blockSize * HEIGHT;
    
    if (!IsVisible(cam, sWidth, sHeight)) return;

    for (int y = 0; y < HEIGHT; ++y)
    for (int x = 0; x < WIDTH; ++x) {
        BlockType type = GetBlock(x, y);
        
        if (type == BlockType::AIR) continue; // comment this line out to see air as missing texture (might be useful)

        Texture2D* tex = TextureManager::blockTextures[static_cast<size_t>(type)];

        DrawTexturePro(
            *tex,
            {0.f, 0.f, (float)tex->width, -(float)tex->height},
            {worldPos.x + x * blockSize, worldPos.y + y * blockSize, (float)tex->width, (float)tex->height},
            {0.f, 0.f},
            0.f,
            WHITE
        );

        if (DebugManager::showChunkBorders) {
            Rectangle border = {worldPos.x, worldPos.y, blockSize * WIDTH, blockSize * HEIGHT};
            DrawRectangleLinesEx(border, 1.f, YELLOW);
        }
    }
}

void Chunk::Save(std::ostream& file) const {
    size_t blockCount = blocks.size();
    file.write(reinterpret_cast<char*>(&blockCount), sizeof(blockCount));

    file.write(reinterpret_cast<const char*>(blocks.data()),
           blockCount * sizeof(Block));
}

void Chunk::Load(std::ifstream& file) {
    size_t blockCount = 0;
    file.read(reinterpret_cast<char*>(&blockCount), sizeof(blockCount));

    blocks.resize(blockCount);

    file.read(reinterpret_cast<char*>(blocks.data()),
        blockCount * sizeof(Block));
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