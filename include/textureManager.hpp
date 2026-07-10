#pragma once

#include <unordered_map>
#include <string>
#include <array>

#include "raylib.h"

#include "block.hpp"

class TextureManager {
public:
    // Returns a texture based on the given type, like a map.
    static inline Texture2D* blockTextures[static_cast<size_t>(BlockType::COUNT)] = { nullptr };

    static inline std::array<Texture2D*, 4> blockBreakingTextures = { nullptr };
    static constexpr int blockBreakingStages = blockBreakingTextures.size();

    // The font the game uses.
    static inline Font font;

    TextureManager() = delete;

    static Texture2D* Get(const std::string& name);
    static void LoadTextures();
    static void UnloadTextures();
private:
    static std::unordered_map<std::string, Texture2D> textures;

    static inline const std::string texturePath = "Assets/Textures/";
    static inline const std::string fontPath = "Assets/Fonts/";

    static void LoadFonts();
    static void UnloadFonts();

    static void SetBlockTextures();
    static void SetBlockBreakingTextures();
};