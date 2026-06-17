#pragma once

#include "vec2f.h"

#include "entity.hpp"
#include "collider.hpp"

class GameplaySession;

class Player : public Entity {
public:
    Player(GameplaySession* gameplaySession, const Vec2f& position);
    
    void Update(float dt) override;
    void Render() override;

private:
    float acceleration = 15.f;
    float friction = 15.f;
    float speed = 250.f;
    float jumpPower = 550.f;

    Collider topRightCollider;
    Collider topLeftCollider;
    Collider bottomRightCollider;
    Collider bottomLeftCollider;
    Collider groundedCollider;
};