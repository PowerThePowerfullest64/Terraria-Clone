#pragma once

#include <fstream>

#include "raylib.h"

#include "vec2f.h"

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
    EntityType type;
public:
    Vec2f position;
    Texture2D* sprite = nullptr;
};