#pragma once

#include <vector>
#include <stdint.h>
#include <fstream>
#include <unordered_map>
#include <chrono>

#include "raylib.h"

#include "vec2f.h"
#include "vec2i.h"

#include "chunk.hpp"
#include "block.hpp"

class GameplaySession;

struct MiningData {
    float timeRemaining;
    float timeSinceMined;

    MiningData(float totalTime) {
        timeRemaining = totalTime;
        timeSinceMined = 0.f;
    }

    void Mine(float time) {
        timeRemaining -= time;
        timeSinceMined = 0.f;
    }
};

class World {
public:
    // Width and height of the world in chunks.
    static constexpr int WIDTH = 522, HEIGHT = 150;

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

    // Converts a block position into a screen coordinate.
    static Vec2f FromWorld(const Vec2i& pos) { return pos * Chunk::blockSize; }
    static Vec2f FromWorld(int x, int y) { return FromWorld({x, y}); }
    static Vec2i ToWorld(const Vec2f& pos) { return pos / Chunk::blockSize; }
    static Vec2i ToWorld(int x, int y) { return Vec2f{static_cast<float>(x), static_cast<float>(y)} / Chunk::blockSize; }

    // Turns a 2D-coordinate into a 1D-index.
    static inline int Index(int x, int y) { return y * (WIDTH * Chunk::WIDTH) + x; }
    static inline int Index(const Vec2i& pos) { return Index(pos.x, pos.y); }
    
    static inline int IndexChunks(int x, int y) { return y * WIDTH + x; }

    // For now, it only checks for if active mined blocks should be reset.
    void Update(float dt);

    // Renders every block in the world, chunk by chunk.
    void Render();

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

    static constexpr int additionalChunkRendering = 1;
};