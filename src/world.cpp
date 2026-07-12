#include "world.hpp"

#include <iostream>

#include "raylib.h"
#include "FastNoiseLite.h"

#include "random.h"

#include "gameplaySession.hpp"
#include "game.hpp"
#include "textureManager.hpp"
#include "settings.hpp"

World::World(GameplaySession* gameplaySession) :
    gameplaySession(gameplaySession) {

    chunks.resize(WIDTH * HEIGHT);

    // Initialize chunks properly
    for (int i = 0; i < WIDTH * HEIGHT; ++i) {
        chunks[i] = Chunk(this, Vec2f{(float)(i % WIDTH), (float)(i / WIDTH)});
    }
}

World::~World() {
    std::cout << "Deconstructed World!\n";
}

void World::Generate(int seed) {
    for (Chunk& chunk : chunks) { chunk.Reset(); } // Reset old world if any existed.

    if (seed == INT32_MAX) seed = randomInt(INT32_MIN, INT32_MAX);

    FastNoiseLite noise;
    noise.SetSeed(seed);
    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin); // better than ValueCubic for terrain
    noise.SetFractalType(FastNoiseLite::FractalType_FBm);
    noise.SetFractalOctaves(12);

    for (int x = 0; x < WIDTH * Chunk::WIDTH; ++x) {
        float fx = (float)x;

        float base   = noise.GetNoise(fx * 0.001f, 0.0f);
        float hills  = noise.GetNoise(fx * 0.005f, 500.0f);
        float detail = noise.GetNoise(fx * 0.02f, 1000.0f);

        float value =
            base   * 1.4f +
            hills  * 0.9f +
            detail * 0.25f;

        int height = (int)(value * 2200.0f) + 1500;

        heightMap[x] = height;

        for (int y = 0; y < height; ++y) {
            if (y < height - quartzDepth)
                SetBlock(x, HEIGHT * Chunk::HEIGHT - y, BlockType::QUARTZ);
            else if (y < height - stoneDepth)
                SetBlock(x, HEIGHT * Chunk::HEIGHT - y, BlockType::STONE);
            else
                SetBlock(x, HEIGHT * Chunk::HEIGHT - y, BlockType::DIRT);
        }

        SetBlock(x, HEIGHT * Chunk::HEIGHT - height, BlockType::GRASS);
    }

    std::cout << "Generated World! (" << chunks.size() << " chunks, " << chunks.size() * Chunk::WIDTH * Chunk::HEIGHT << " blocks)\n";
}

BlockType World::GetBlock(int x, int y) {
    if (x < 0 || x >= WIDTH * Chunk::WIDTH || y < 0 || y >= HEIGHT * Chunk::HEIGHT)
        return BlockType::AIR;
    
    int chunk =
        (y / Chunk::HEIGHT) * WIDTH +
        (x / Chunk::WIDTH);

    uint16_t block =
        (y % Chunk::HEIGHT) * Chunk::WIDTH +
        (x % Chunk::WIDTH);

    return chunks[chunk].GetBlock(block);
}

void World::SetBlock(int x, int y, BlockType type) {
    if (x < 0 || x >= WIDTH * Chunk::WIDTH || y < 0 || y >= HEIGHT * Chunk::HEIGHT)
        return;
    
    int chunk =
        (y / Chunk::HEIGHT) * WIDTH +
        (x / Chunk::WIDTH);

    uint16_t block =
        (y % Chunk::HEIGHT) * Chunk::WIDTH +
        (x % Chunk::WIDTH);
    
    chunks[chunk].SetBlock(block, type);
}

void World::Mine(int x, int y, float time) {
    int id = Index(x, y);

    auto it = activeMiningTimes.find(id);

    if (it != activeMiningTimes.end()) {
        it->second.Mine(time);
    } else {
        BlockType type = GetBlock(id);

        if (type == BlockType::AIR) return;

        // Add it to the hash-map and set mining time remaining.
        it = activeMiningTimes.emplace(id, MiningData(GetMiningTime(type))).first;
        it->second.Mine(time);
    }

    // If block is fully mined.
    if (it->second.timeRemaining <= 0.f) {
        // Mine the block.
        SetBlock(id, BlockType::AIR);
        // Remove it from the active mined blocks hash-map.
        activeMiningTimes.erase(it);
    }
}

std::vector<AABB> World::GetBlockColliders(const Vec2f& pos) {
    std::vector<AABB> colliders;
    colliders.reserve(9 * Chunk::WIDTH * Chunk::HEIGHT);

    Vec2i chunkPos = ToChunk(pos);

    for (int y = -1; y <= 1; ++y)
    for (int x = -1; x <= 1; ++x) {
        int chunkX = chunkPos.x + x;
        int chunkY = chunkPos.y + y;

        // Outside of bounds, ignore it.
        if (chunkX < 0 || chunkX >= WIDTH || chunkY < 0 || chunkY >= HEIGHT)
            continue;

        int index = IndexChunks(chunkX, chunkY);
        const std::vector<AABB>& chunkColliders = chunks[index].colliders;
        colliders.insert(colliders.end(), chunkColliders.begin(), chunkColliders.end());
    }

    return colliders;
}

void World::Update(float dt) {
    std::vector<int> markedIndices;
    for (auto& it : activeMiningTimes) {
        it.second.timeSinceMined += dt;

        if (it.second.timeSinceMined >= Settings::resetMineCooldown)
            markedIndices.push_back(it.first);
    }

    for (int idx : markedIndices)
        activeMiningTimes.erase(idx);
    
    for (Chunk& chunk : chunks) {
        chunk.Update();
    }
}

void World::Render() {
    const Camera2D* cam = &gameplaySession->cam;

    float viewW = Settings::windowWidth / cam->zoom;
    float viewH = Settings::windowHeight / cam->zoom;

    float viewX = cam->target.x - viewW * 0.5f;
    float viewY = cam->target.y - viewH * 0.5f;

    int firstChunkX = floorf(viewX / Chunk::pixelWidth) - additionalChunkRendering;
    int lastChunkX = floorf((viewX + viewW) / Chunk::pixelWidth) + additionalChunkRendering;

    int firstChunkY = floorf(viewY / Chunk::pixelHeight) - additionalChunkRendering;
    int lastChunkY = floorf((viewY + viewH) / Chunk::pixelHeight) + additionalChunkRendering;

    for (int y = firstChunkY; y <= lastChunkY; ++y)
    for (int x = firstChunkX; x <= lastChunkX; ++x)
        chunks[IndexChunks(x, y)].Render();
}

void World::Save(std::ofstream& file) const {
    size_t chunkCount = chunks.size();
    file.write(reinterpret_cast<char*>(&chunkCount), sizeof(chunkCount));

    for (const Chunk& chunk : chunks) chunk.Save(file);
}

void World::Load(std::ifstream& file) {
    size_t chunkCount = 0;

    file.read(reinterpret_cast<char*>(&chunkCount), sizeof(chunkCount));

    chunks.resize(chunkCount);

    for (Chunk& chunk : chunks) {
        chunk.Load(file);
    }
}