#pragma once

struct AABB {
    float x, y;
    float w, h;
};

inline bool Intersects(const AABB& a, const AABB& b) {
    return !(a.x + a.w < b.x ||
             a.x > b.x + b.w ||
             a.y + a.h < b.y ||
             a.y > b.y + b.h);
}