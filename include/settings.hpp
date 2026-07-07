#pragma once

class Settings {
public:
    static constexpr int windowWidth = 1440, windowHeight = 810;

    // Debug
    static bool showChunkBorders;
    static bool showMouseBlockType;
    static bool showMouseCoords;
    static bool showFPS;
    static bool showPlayerVelocity;
    static bool showPlayerColliders;
    static bool showActivelyMinedBlocks;

    // Physics
    static float gravity;

    // How many seconds pass before a somewhat mined block resets to fully unmined.
    static float resetMineCooldown;
};