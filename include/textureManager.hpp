#pragma once

#include <unordered_map>
#include <string>

#include "raylib.h"

class TextureManager {
public:
    TextureManager();
    ~TextureManager();

    Texture2D* Get(const std::string& name);
    void LoadTextures();
    void UnloadTextures();
private:
    std::unordered_map<std::string, Texture2D> textures;

    void AddTexture(const std::string& name);
};