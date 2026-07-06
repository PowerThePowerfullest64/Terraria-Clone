#pragma once

#include <stdint.h>
#include <vector>
#include <unordered_map>
#include <string>

enum class BlockType : uint8_t {
    AIR,
    DIRT,
    GRASS,
    STONE,
    SAND,
    QUARTZ,
    COUNT
};

inline std::unordered_map<BlockType, float> miningTimes;
inline std::unordered_map<BlockType, std::string> typeToName;
inline std::unordered_map<std::string, BlockType> nameToType;

struct Block {
    BlockType type;
};

void LoadBlockData();
float GetMiningTime(BlockType blockType);