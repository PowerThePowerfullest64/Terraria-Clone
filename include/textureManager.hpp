#pragma once

#include <unordered_map>
#include <string>

#include "raylib.h"

#include "block.hpp"

class TextureManager {
public:
    // Returns a texture based on the given type, like a map.
    static inline Texture2D* blockTextures[static_cast<size_t>(BlockType::COUNT)] = { nullptr };

    // The font the game uses.
    static inline Font font;

    TextureManager() = delete;

    static Texture2D* Get(const std::string& name);
    static void LoadTextures();
    static void UnloadTextures();
    static void LoadFonts();
    static void UnloadFonts();

    static void SetBlockTextures();
private:
    static std::unordered_map<std::string, Texture2D> textures;

    static inline const std::string texturePath = "Assets/Textures/";
    static inline const std::string fontPath = "Assets/Fonts/";
};