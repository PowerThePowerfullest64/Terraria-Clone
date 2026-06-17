#pragma once

#include <unordered_map>
#include <string>

#include "raylib.h"

#include "block.hpp"

class TextureManager {
public:
    // Returns a texture based on the given type, like a map.
    static inline Texture2D* blockTextures[static_cast<size_t>(BlockType::COUNT)] = { nullptr };

    TextureManager() = delete;

    static Texture2D* Get(const std::string& name);
    static void LoadTextures();
    static void UnloadTextures();

    static void SetBlockTextures();
private:
    static std::unordered_map<std::string, Texture2D> textures;

    static inline const std::string path = "Assets/Textures/";
};