#include "collider.hpp"

#include <stdint.h>

#include "game.hpp"
#include "entity.hpp"

Collider::Collider(Game* game, Entity* entity, const Vec2f& position) :
    game(game),
    entity(entity),
    position(position) {}

void Collider::Update() {
    Vec2f worldPos = position + entity->position;

    BlockType type = game->wm.GetBlockWorld(worldPos);

    colliding = type != AIR;
}

void Collider::Render() {
    Color c = colliding ? RED : GREEN;
    DrawCircleV(position+entity->position, 4.f, c);
}