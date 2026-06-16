#pragma once

#include <stdint.h>

enum class BlockType : uint8_t {
    AIR,
    DIRT,
    GRASS,
    STONE,
    SAND,
    QUARTZ,
    COUNT
};