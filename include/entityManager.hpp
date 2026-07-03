#pragma once

#include <vector>
#include <memory>
#include <fstream>
#include <stdint.h>

#include "vec2f.h"

#include "entity.hpp"

class GameplaySession;

class EntityManager {
public:
    EntityManager();
    ~EntityManager();

    // Create an entity and get a pointer to it.
    template<typename T, typename... Args>
    T* CreateEntity(Args&&... args) {
        auto entity = std::make_unique<T>(std::forward<Args>(args)...);
        T* ptr = entity.get();

        entities.push_back(std::move(entity));

        return ptr;
    }

    void DestroyEntity(Entity* entity) {
        entities.erase(
            std::remove_if(
                entities.begin(),
                entities.end(),
                [entity](const std::unique_ptr<Entity>& ptr)
                {
                    return ptr.get() == entity;
                }),

            entities.end());
    }

    // Destroys all entities in existence.
    void ClearEntities() {
        while (entities.size() > 0) {
            entities.pop_back();
        }
    }

    // Returns a vector containing all entities as pointers.
    std::vector<Entity*> GetAllEntities() {
        std::vector<Entity*> ptrs;
        ptrs.reserve(entities.size());

        for (auto& entity : entities) {
            ptrs.push_back(entity.get()); // Gets raw pointer.
        }

        return ptrs;
    }

    void Update(float dt) { for (auto& entity : entities) entity->Update(dt); } // Updates all entities.
    void Render() { for (auto& entity : entities) entity->Render(); } // Renders all entities.

    void Save(std::ofstream& file);
    void Load(std::ifstream& file, GameplaySession* gameplaySession);

private:
    std::vector<std::unique_ptr<Entity>> entities = {};
};