#include "block.hpp"

#include <iostream>

void LoadBlockData() {
    miningTimes[BlockType::AIR] = 0.f;
    miningTimes[BlockType::DIRT] = 0.4f;
    miningTimes[BlockType::GRASS] = 0.6f;
    miningTimes[BlockType::STONE] = 1.f;
    miningTimes[BlockType::SAND] = 0.5f;
    miningTimes[BlockType::QUARTZ] = 10.f;

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
    auto it = miningTimes.find(blockType);

    if (it == miningTimes.end()) {
        std::cout << "Missing mining time for block type '" << (int)blockType << ".\n";
    }

    return it->second;
}