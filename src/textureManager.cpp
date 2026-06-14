#include "textureManager.hpp"

#include <iostream>
#include <cassert>

std::unordered_map<std::string, Texture2D> TextureManager::textures;

Texture2D* TextureManager::Get(const std::string& name) {
    auto it = textures.find(name);

    if (it != textures.end()) {
        return &it->second;
    }

    std::cout << "Warning: Did not find texture '" << name << "' in TextureManager\n";
    return &textures["Assets/Textures/noTexture.png"];
}

void TextureManager::AddTexture(const std::string& name) {
    assert(textures.find(name) == textures.end() && "Could not add texture to TextureManager; texture is already stored.");

    textures[name]; // add key to map
}

void TextureManager::LoadTextures() {
    std::cout << "Began adding textures!\n";

    std::string extension = ".png";

    // Add all my textures, probably added automatically from within a folder in the future.
    AddTexture("noTexture");
    AddTexture("human");
    AddTexture("grass");
    AddTexture("dirt");
    AddTexture("stone");
    AddTexture("quartz");
    AddTexture("sand");

    std::cout << "Finished adding textures, texture loading begins!\n";

    for (auto& it : textures) {
        it.second = LoadTexture((path + it.first + extension).c_str()); // load and assign textures
    }

    std::cout << "Finished loading textures!\n";
}

void TextureManager::UnloadTextures() {
    std::cout << "Began unloading textures!\n";

    for (auto it : textures) {
        UnloadTexture(it.second);
    }

    std::cout << "Finished unloading textures!\n";
}