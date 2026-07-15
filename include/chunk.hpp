#pragma once

#include <stdint.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <memory>

#include "raylib.h"

#include "vec2f.h"
#include "aabb.hpp"

#include "block.hpp"

class World;

class Chunk {
public:

    // Width and height of the chunk in blocks.
    static constexpr int WIDTH = 32, HEIGHT = 32;
    // Pixel width and height of a singular block.
    static constexpr int blockSize = 16;
    // The width of a chunk in pixels.
    static constexpr int pixelWidth = WIDTH * blockSize;
    // The height of a chunk in pixels.
    static constexpr int pixelHeight = HEIGHT * blockSize;

    std::vector<Vec2f> colliders;

    Chunk(World* world = nullptr, const Vec2f& pos = Vec2f::ZERO);

    // Gets a copy of a block at a given position.
    BlockType GetBlock(size_t index) { if (index >= WIDTH * HEIGHT) return BlockType::AIR; return (BlockType)blocks[index].type; }
    BlockType GetBlock(int x, int y) { if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return BlockType::AIR; return GetBlock(Index(x, y)); }
    BlockType GetBlock(const Vec2f& pos) { return GetBlock(Index(pos)); }

    // Sets a block at a given position to a given type.
    void SetBlock(size_t index, BlockType type) { if (index >= WIDTH * HEIGHT) return; blocks[index].type = type; colliderUpdateFlag = true; textureUpdateFlag = true; }
    void SetBlock(int x, int y, BlockType type) { SetBlock(Index(x, y), type); }

    Vec2f GetChunkPos() const {
        return {
            pos.x * pixelWidth,
            pos.y * pixelHeight
        };
    }

    float GetChunkX() const { return pos.x * pixelWidth; }
    float GetChunkY() const { return pos.y * pixelHeight; }

    void Reset() { for (int i = 0; i < blocks.size(); ++i) SetBlock(i, BlockType::AIR); }

    void Update();
    // Renders every block in the chunk.
    void Render();

    void Save(std::ostream& file) const;
    void Load(std::ifstream& file);

    void Load() { texture = LoadRenderTexture(pixelWidth, pixelHeight); textureUpdateFlag = true;  }
    void Unload() { UnloadRenderTexture(texture); }

private:
    World* world;
    // Chunk position in chunk coordinates.
    Vec2f pos;
    std::vector<Block> blocks;

    bool colliderUpdateFlag = true;
    bool textureUpdateFlag = false;

    RenderTexture2D texture{};

    void BuildColliders();
    void BuildTexture();

    // Turns a 2D-coordinate into a 1D-index.
    size_t Index(int x, int y) { return static_cast<size_t>(y * WIDTH + x); }
    size_t Index(const Vec2f& pos) { return static_cast<size_t>((int)pos.y * WIDTH + (int)pos.x); }
};