#pragma once

#include <vector>
#include <memory>
#include <iostream>

#include "vec2f.h"

#include "entity.hpp"

class EntityManager {
public:
    EntityManager() = delete;

    template<typename T, typename... Args>
    static T* CreateEntity(Args&&... args) {
        auto entity = std::make_unique<T>(std::forward<Args>(args)...);
        T* ptr = entity.get();

        entities.push_back(std::move(entity));

        return ptr;
    }

    static void Update(float dt) { for (auto& entity : entities) entity->Update(dt); } // update all entities
    static void Render() { for (auto& entity : entities) entity->Render(); } // render all entities

private:
    static std::vector<std::unique_ptr<Entity>> entities;
};