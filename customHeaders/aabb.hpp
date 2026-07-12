#pragma once

struct AABB {
    float x = 0.f, y = 0.f;
    float w = 0.f, h = 0.f;
};

inline bool Intersects(const AABB& a, const AABB& b) {
    return !(a.x + a.w < b.x ||
             a.x > b.x + b.w ||
             a.y + a.h < b.y ||
             a.y > b.y + b.h);
}