#pragma once

#include <iostream>

#include "vec2i.h"

#include "entity.hpp"
#include "collider.hpp"

class GameplaySession;

class Player : public Entity {
public:
    Player(GameplaySession* gameplaySession, const Vec2f& position = Vec2f::ZERO);
    ~Player() { std::cout << "Deconstructed Player!\n"; }
    
    void Update(float dt) override;
    void Render() override;

    void Save(std::ofstream& file) const override {
        file.write(reinterpret_cast<const char*>(&position.x), sizeof(position.x));
        file.write(reinterpret_cast<const char*>(&position.y), sizeof(position.y));
    }

    void Load(std::ifstream& file) override {
        file.read(reinterpret_cast<char*>(&position.x), sizeof(position.x));
        file.read(reinterpret_cast<char*>(&position.y), sizeof(position.y));
    }

private:
    float acceleration = 15.f;
    float friction = 15.f;
    float speed = 250.f;
    float jumpPower = 550.f;

    Vec2i miningPosition = {-1, -1};
    float miningLeft = 0.f;
    float miningTimeFactor = 1.f;

    Collider topRightCollider;
    Collider topLeftCollider;
    Collider bottomRightCollider;
    Collider bottomLeftCollider;
    Collider groundedCollider;

    void MoveAndSlide(float dt);
    void MiningUpdate(float dt);
};