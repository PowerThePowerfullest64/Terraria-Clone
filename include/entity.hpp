#pragma once

#include <fstream>
#include <vector>

#include "raylib.h"

#include "vec2f.h"
#include "aabb.hpp"

enum class EntityType : uint8_t {
    Player
};

class GameplaySession;

class Entity {
public:
    Entity(GameplaySession* gameplaySession, const Vec2f& position) :
        gameplaySession(gameplaySession),
        position(position) {}
    
    virtual ~Entity() = default;

    virtual void Update(float dt) {}
    virtual void Render() {}

    virtual void Save(std::ofstream& file) const = 0;
    virtual void Load(std::ifstream& file) = 0;

    EntityType GetType() const { return type; }
protected:
    GameplaySession* gameplaySession;
    Vec2f velocity = Vec2f::ZERO;
    Vec2f scale = Vec2f::ONE;
    float rotation = 0.f;
    AABB collider;
    EntityType type;

    // The colliders near the player.
    std::vector<AABB> colliders;

    AABB GetRealCollider(const AABB& collider) { return {collider.x + position.x, collider.y + position.y, collider.w, collider.h}; }
public:
    Vec2f position;
    Texture2D* sprite = nullptr;
};