#include "collider.hpp"

#include "game.hpp"

Collider::Collider(Game* game, const Vec2f& position) :
    game(game),
    position(position) {}

void Collider::Update() {
    
}