#include "entityManager.hpp"

#include <iostream>

#include "player.hpp"
#include "gameplaySession.hpp"

EntityManager::EntityManager() {
    std::cout << "Constructed EntityManager!\n";
}

EntityManager::~EntityManager() {
    std::cout << "Deconstructed EntityManager!\n";
}

void EntityManager::Save(std::ofstream& file) {
    size_t entityCount = entities.size();
    file.write(reinterpret_cast<char*>(&entityCount), sizeof(entityCount));

    for (const auto& entity : entities) {
        EntityType type = entity->GetType();
        file.write(reinterpret_cast<char*>(&type), sizeof(type));

        entity->Save(file);
    }
}

void EntityManager::Load(std::ifstream& file, GameplaySession* gameplaySession) {
    size_t entityCount = 0;
    file.read(reinterpret_cast<char*>(&entityCount), sizeof(entityCount));

    entities.clear();
    
    for (size_t i = 0; i < entityCount; ++i) {
        EntityType type;
        file.read(reinterpret_cast<char*>(&type), sizeof(type));

        Entity* entity;

        switch (type) {
            case EntityType::Player:
                entity = CreateEntity<Player>(gameplaySession);
                std::cout << "EntityType found on an entity.\n";
                break;
            default:
                std::cout << "EntityType not found on an entity.\n";
                continue;
        }

        entity->Load(file);
    }
}