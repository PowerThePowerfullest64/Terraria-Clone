#include "collider.hpp"

#include <stdint.h>

#include "gameplaySession.hpp"
#include "entity.hpp"

Collider::Collider(GameplaySession* gameplaySession, Entity* entity, const Vec2f& position) :
    gameplaySession(gameplaySession),
    entity(entity),
    position(position) {}

bool Collider::CheckCollision() {
    BlockType type = gameplaySession->world->GetBlockWorld(position + entity->position);

    return type != BlockType::AIR;
}

void Collider::Render() {
    Color c = CheckCollision() ? RED : GREEN;
    DrawCircleV(position+entity->position, 4.f, c);
}