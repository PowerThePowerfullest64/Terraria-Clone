#pragma once

#include <vector>
#include <memory>

#include "vec2f.h"

#include "entity.hpp"

class EntityManager {
public:
    template<typename T, typename... Args>
    T* CreateEntity(Args&&... args) {
        auto entity = std::make_unique<T>(std::forward<Args>(args)...);
        T* ptr = entity.get();

        entities.push_back(std::move(entity));

        return ptr;
    }

    void Update(float dt) { for (auto& entity : entities) entity->Update(dt); } // update all entities
    void Render() { for (auto& entity : entities) entity->Render(); } // render all entities

private:
    std::vector<std::unique_ptr<Entity>> entities;
};