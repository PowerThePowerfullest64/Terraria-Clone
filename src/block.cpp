#include "block.hpp"

#include <iostream>

void LoadBlockData() {
    miningTimes[static_cast<int>(BlockType::AIR)] = 0.f;
    miningTimes[static_cast<int>(BlockType::DIRT)] = 0.4f;
    miningTimes[static_cast<int>(BlockType::GRASS)] = 0.6f;
    miningTimes[static_cast<int>(BlockType::STONE)] = 1.f;
    miningTimes[static_cast<int>(BlockType::SAND)] = 0.5f;
    miningTimes[static_cast<int>(BlockType::QUARTZ)] = 10.f;

    std::cout << "Loaded Block Data!\n";
}

float GetMiningTime(BlockType blockType) {
    return miningTimes[static_cast<int>(blockType)];
}