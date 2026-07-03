#pragma once

class Settings {
public:
    static constexpr int windowWidth = 1440, windowHeight = 810;

    // Debug
    static bool showChunkBorders;
    static bool showMouseBlockType;
    static bool showMouseCoords;
    static bool showFPS;

    // Physics
    static float gravity;
};