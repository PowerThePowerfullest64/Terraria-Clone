#include "collider.hpp"

#include <stdint.h>

#include "game.hpp"
#include "entity.hpp"

Collider::Collider(Game* game, Entity* entity, const Vec2f& position) :
    game(game),
    entity(entity),
    position(position) {}

bool Collider::CheckCollision() {
    BlockType type = game->world.GetBlockWorld(position + entity->position);

    return type != AIR;
}

void Collider::Render() {
    Color c = CheckCollision() ? RED : GREEN;
    DrawCircleV(position+entity->position, 4.f, c);
}