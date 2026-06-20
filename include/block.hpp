#pragma once

#include <stdint.h>
#include <vector>

enum class BlockType : uint8_t {
    AIR,
    DIRT,
    GRASS,
    STONE,
    SAND,
    QUARTZ,
    COUNT
};

inline float miningTimes[static_cast<int>(BlockType::COUNT)];

struct Block {
    BlockType type;
};

void LoadBlockData();
float GetMiningTime(BlockType blockType);