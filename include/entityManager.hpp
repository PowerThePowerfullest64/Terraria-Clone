#pragma once

#include <vector>
#include <memory>
#include <iostream>

#include "vec2f.h"

#include "entity.hpp"

class EntityManager {
public:
    EntityManager() = delete;

    // Create an entity and get a pointer to it.
    template<typename T, typename... Args>
    static T* CreateEntity(Args&&... args) {
        auto entity = std::make_unique<T>(std::forward<Args>(args)...);
        T* ptr = entity.get();

        entities.push_back(std::move(entity));

        return ptr;
    }

    static void DestroyEntity(Entity* entity) {
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
    static void ClearEntities() {
        while (entities.size() > 0) {
            entities.pop_back();
        }
    }

    static void Update(float dt) { for (auto& entity : entities) entity->Update(dt); } // update all entities
    static void Render() { for (auto& entity : entities) entity->Render(); } // render all entities

private:
    static std::vector<std::unique_ptr<Entity>> entities;
};