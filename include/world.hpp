#pragma once

#include <vector>
#include <stdint.h>
#include <fstream>
#include <unordered_map>
#include <chrono>
#include <iostream>

#include "raylib.h"

#include "vec2f.h"
#include "vec2i.h"
#include "aabb.hpp"

#include "chunk.hpp"
#include "block.hpp"

class GameplaySession;

struct MiningData {
    float totalTime;
    float timeRemaining;
    float timeSinceMined;

    MiningData(float totalTime) : 
    totalTime(totalTime),
    timeRemaining(totalTime),
    timeSinceMined(0.f) {}

    void Mine(float time) {
        timeRemaining -= time;
        timeSinceMined = 0.f;
    }

    float GetProgress() const {
        return (totalTime - timeRemaining) / totalTime;
    }
};

class World {
public:
    // Width and height of the world in chunks.
    static constexpr int WIDTH = 261, HEIGHT = 75;

    std::unordered_map<int, MiningData> activeMiningTimes;

    World(GameplaySession* gameplaySession);
    ~World();

    // Gets a copy of a block at a given position.
    BlockType GetBlock(int x, int y);
    BlockType GetBlock(const Vec2f& pos) { return GetBlock((int)pos.x, (int)pos.y); }
    BlockType GetBlock(int idx) { int x = idx % (WIDTH * Chunk::WIDTH); int y = idx / (WIDTH * Chunk::WIDTH); return GetBlock(x, y); }
    BlockType GetBlockWorld(const Vec2f& pos) { return GetBlock(pos / (float)Chunk::blockSize); }

    // Sets a block at a given position to a given type.
    void SetBlock(int x, int y, BlockType type);
    void SetBlock(int idx, BlockType type) { int x = idx % (WIDTH * Chunk::WIDTH); int y = idx / (WIDTH * Chunk::WIDTH); SetBlock(x, y, type); }

    void Mine(int x, int y, float time);

    // Given a world coordinate, get all the relevant rectangle colliders.
    std::vector<AABB> GetBlockColliders(const Vec2f& pos);

    // Converts a block position into a screen coordinate.
    static Vec2f ToWorld(const Vec2i& pos) { return pos * Chunk::blockSize; }
    static Vec2f ToWorld(int x, int y) { return ToWorld({x, y}); }
    static Vec2i ToBlock(const Vec2f& pos) { return pos / Chunk::blockSize; }
    static Vec2i ToBlock(int x, int y) { return Vec2f{static_cast<float>(x), static_cast<float>(y)} / Chunk::blockSize; }
    // Given a world coordinate, convert it to chunk coordinates.
    static Vec2i ToChunk(const Vec2f& pos) { return {static_cast<int>(pos.x / static_cast<float>(Chunk::blockSize * Chunk::WIDTH)), static_cast<int>(pos.y / static_cast<float>(Chunk::blockSize * Chunk::HEIGHT))}; }

    // Turns a 2D-coordinate into a 1D-index.
    static inline int Index(int x, int y) { return y * (WIDTH * Chunk::WIDTH) + x; }
    static inline int Index(const Vec2i& pos) { return Index(pos.x, pos.y); }
    
    static inline int IndexChunks(int x, int y) { return y * WIDTH + x; }

    // For now, it only checks for if active mined blocks should be reset.
    void Update(float dt);
    // Renders every block in the world, chunk by chunk.
    void Render();

    void LoadChunk(int idx) { previouslyLoadedChunks.push_back(idx); chunks[idx].ForceLoad(); std::cout << "Forceloaded chunk " << idx << ".\n"; }
    void LoadChunk(const Vec2f& pos) { Vec2i chunkPos = ToChunk(pos); int idx = IndexChunks(chunkPos.x, chunkPos.y); LoadChunk(idx); }

    // Generates a random world, random seed if none is provided or seed == INT32_MAX.
    void Generate(int seed = INT32_MAX);

    void Save(std::ofstream& file) const;
    void Load(std::ifstream& file);

private:
    GameplaySession* gameplaySession;
    std::vector<Chunk> chunks;
    int heightMap[WIDTH * Chunk::WIDTH];

    int stoneDepth = 10;
    int quartzDepth = 1500;

    std::vector<int> previouslyLoadedChunks;

    // Chunks in radius x outside of the view that will be rendered.
    static constexpr int additionalChunkRendering = 1;

    void BuildAllColliders() { for (Chunk& chunk : chunks) { chunk.BuildColliders(); } }
};