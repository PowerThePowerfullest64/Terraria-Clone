#include "chunk.hpp"

#include <iostream>

#include "textureManager.hpp"
#include "settings.hpp"
#include "block.hpp"
#include "world.hpp"

Chunk::Chunk(World* world, const Vec2f& pos) : 
    world(world),
    pos(pos) {
        blocks.resize(WIDTH * HEIGHT, {BlockType::AIR});
    }

void Chunk::BuildColliders() {
    colliders.clear();
    colliders.reserve(WIDTH * HEIGHT);

    float chunkX = GetChunkX();
    float chunkY = GetChunkY();

    for (int y = 0; y < HEIGHT; ++y)
    for (int x = 0; x < WIDTH; ++x) {
        if (GetBlock(x, y) == BlockType::AIR)
            continue;

        colliders.push_back(
            {
                chunkX + x * blockSize,
                chunkY + y * blockSize,
                blockSize,
                blockSize
            }
        );
    }
}

void Chunk::Update() {
    if (blocksChanged) {
        BuildColliders();

        blocksChanged = false;
    }
}

void Chunk::Render() {
    Vec2f chunkPos = GetChunkPos();

    for (int y = 0; y < HEIGHT; ++y)
    for (int x = 0; x < WIDTH; ++x) {
        BlockType type = GetBlock(x, y);
        
        if (type == BlockType::AIR) continue; // comment this line out to see air as missing texture (might be useful).
        
        {
            Texture2D* tex = TextureManager::blockTextures[static_cast<size_t>(type)];

            float texWidth = tex->width;
            float texHeight = tex->height;
            
            DrawTexturePro(
                *tex,
                {0.f, 0.f, texWidth, texHeight},
                {chunkPos.x + x * blockSize, chunkPos.y + y * blockSize, blockSize, blockSize},
                {0.f, 0.f},
                0.f,
                WHITE
            );
        }

        if (Settings::showActivelyMinedBlocks) {
            int index = World::Index(pos.x * WIDTH + x, pos.y * HEIGHT + y);
            auto it = world->activeMiningTimes.find(index);

            if (it != world->activeMiningTimes.end()) {
                float progress = it->second.GetProgress();

                Texture2D* tex = nullptr;

                for (int i = TextureManager::blockBreakingStages; i >= 0; i--) {
                    if (progress >= static_cast<float>(i) / static_cast<float>(TextureManager::blockBreakingStages)) {
                        tex = TextureManager::blockBreakingTextures[i];
                        break;
                    }
                }

                float texWidth = tex->width;
                float texHeight = tex->height;

                DrawTexturePro(
                    *tex,
                    {0.f, 0.f, texWidth, texHeight},
                    {chunkPos.x + x * blockSize, chunkPos.y + y * blockSize, blockSize, blockSize},
                    {0.f, 0.f},
                    0.f,
                    WHITE
                );
            }
        }
    }

    if (Settings::showChunkBorders) {
        Rectangle border = {chunkPos.x, chunkPos.y, blockSize * WIDTH, blockSize * HEIGHT};
        DrawRectangleLinesEx(border, 1.f, YELLOW);
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