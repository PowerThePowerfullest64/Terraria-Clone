#include "textureManager.hpp"

#include <iostream>
#include <cassert>
#include <filesystem>

namespace fs = std::filesystem;

std::unordered_map<std::string, Texture2D> TextureManager::textures;

Texture2D* TextureManager::Get(const std::string& name) {
    auto it = textures.find(name);

    if (it != textures.end())
        return &it->second;

    std::cout << "Warning: Did not find texture '" << name << "' in TextureManager\n";
    return &textures[path + "noTexture.png"];
}

void TextureManager::LoadTextures() {
    std::cout << "Began loading textures!\n";

    for (const auto& entry : fs::recursive_directory_iterator(path)) {
        if (!entry.is_regular_file())
            continue;
        
        if (entry.path().extension() != ".png")
            continue;
        
        std::string name = entry.path().stem().string();

        textures[name] = LoadTexture(entry.path().string().c_str());
        
        GenTextureMipmaps(&textures[name]);
        SetTextureFilter(textures[name], TEXTURE_FILTER_POINT);
    }

    std::cout << "Finished loading textures!\n";
}

void TextureManager::UnloadTextures() {
    std::cout << "Began unloading textures!\n";

    for (auto it : textures)
        UnloadTexture(it.second);

    std::cout << "Finished unloading textures!\n";
}

void TextureManager::SetBlockTextures() {
    blockTextures[0] = TextureManager::Get("noTexture"); // air
    blockTextures[1] = TextureManager::Get("dirt");
    blockTextures[2] = TextureManager::Get("grass");
    blockTextures[3] = TextureManager::Get("stone");
    blockTextures[4] = TextureManager::Get("sand");
    blockTextures[5] = TextureManager::Get("quartz");
}