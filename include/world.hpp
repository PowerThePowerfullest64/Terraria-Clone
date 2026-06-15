#pragma once

#include <vector>
#include <stdint.h>

#include "raylib.h"

#include "vec2f.h"

class Game;

enum BlockType : uint8_t {
    AIR,
    DIRT,
    GRASS,
    STONE,
    SAND,
    QUARTZ,
    COUNT
};

class World {
public:
    static constexpr uint16_t WIDTH = 1024, HEIGHT = 200;
    static constexpr uint8_t blockSize = 24;

    World(Game* game);
    ~World();

    void SetBlockTextures();

    // Gets a copy of a block at a given position.
    BlockType GetBlock(size_t index) { if (index >= WIDTH * HEIGHT) return AIR; return (BlockType)blocks[index]; }
    BlockType GetBlock(int x, int y) { if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return AIR; return GetBlock(Index(x, y)); }
    BlockType GetBlock(const Vec2f& pos) { return GetBlock(pos.x, pos.y); }
    BlockType GetBlockWorld(const Vec2f& pos) { return GetBlock(pos / (float)blockSize); }

    // Sets a block at a given position to a given type.
    void SetBlock(size_t index, BlockType type) { if (index >= WIDTH * HEIGHT) return; blocks[index] = type; }
    void SetBlock(int x, int y, BlockType type) { SetBlock(Index(x, y), type); }

    // Renders every block in the world.
    void Render();

private:
    Game* game;
    std::vector<uint8_t> blocks;
    int heightMap[WIDTH];

    int stoneDepth = 10;
    int quartzDepth = 90;

    // Turns a 2D-coordinate into a 1D-index.
    size_t Index(int x, int y) { return static_cast<size_t>(y * WIDTH + x); }

    // Returns a texture based on the given type, like a map.
    Texture2D* blockTextures[COUNT];
};