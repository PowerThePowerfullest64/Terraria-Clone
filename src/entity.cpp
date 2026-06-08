#include "entity.h"

void Player::Update(float dt) {
    // do input handling also!
    
    position += velocity * dt;
}

void Player::Render() {
    DrawCircleV(position, 16.f, BLUE);
}