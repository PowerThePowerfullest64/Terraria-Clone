#pragma once

#include <unordered_map>
#include <string>

#include "raylib.h"

class TextureManager {
public:
    TextureManager() = delete;

    static Texture2D* Get(const std::string& name);
    static void LoadTextures();
    static void UnloadTextures();
private:
    static std::unordered_map<std::string, Texture2D> textures;

    static void AddTexture(const std::string& name);

    static inline const std::string path = "Assets/Textures/";
};