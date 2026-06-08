#pragma once

#include "vec2f.h"

class Game;

class Entity {
public:
    Entity(Game* game, const Vec2f& position) :
        game(game),
        position(position) {}
    
    virtual ~Entity() = default;

    virtual void Update(float dt) {}
    virtual void Render() {}
protected:
    Game* game;
    Vec2f position;
    Vec2f velocity = Vec2f::ZERO;
    Vec2f scale = Vec2f::ONE;
    float rotation = 0.f;
};

class Player : public Entity {
public:
    Player(Game* game, const Vec2f& position) :
        Entity(game, position) {}
    
    void Update(float dt) override;
    void Render() override;
};