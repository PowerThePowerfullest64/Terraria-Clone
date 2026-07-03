#include "block.hpp"

#include <iostream>

void LoadBlockData() {
    miningTimes[static_cast<int>(BlockType::AIR)] = 0.f;
    miningTimes[static_cast<int>(BlockType::DIRT)] = 0.4f;
    miningTimes[static_cast<int>(BlockType::GRASS)] = 0.6f;
    miningTimes[static_cast<int>(BlockType::STONE)] = 1.f;
    miningTimes[static_cast<int>(BlockType::SAND)] = 0.5f;
    miningTimes[static_cast<int>(BlockType::QUARTZ)] = 10.f;

    typeToName[BlockType::AIR] = "air";
    typeToName[BlockType::DIRT] = "dirt";
    typeToName[BlockType::GRASS] = "grass";
    typeToName[BlockType::STONE] = "stone";
    typeToName[BlockType::SAND] = "sand";
    typeToName[BlockType::QUARTZ] = "quartz";

    nameToType["air"] = BlockType::AIR;
    nameToType["dirt"] = BlockType::DIRT;
    nameToType["grass"] = BlockType::GRASS;
    nameToType["stone"] = BlockType::STONE;
    nameToType["sand"] = BlockType::SAND;
    nameToType["quartz"] = BlockType::QUARTZ;

    std::cout << "Loaded Block Data!\n";
}

float GetMiningTime(BlockType blockType) {
    return miningTimes[static_cast<int>(blockType)];
}