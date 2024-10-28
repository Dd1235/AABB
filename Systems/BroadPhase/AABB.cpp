#include "AABB.h"
// represesnts an axis-aligned bounding box for broad phase collision detection
AABB::AABB() : min(Vector2()), max(Vector2()) {}

AABB::AABB(const Vector2 &minVal, const Vector2 &maxVal) : min(minVal), max(maxVal) {}

bool AABB::intersects(const AABB &other) const
{
    return (min.x <= other.max.x && max.x >= other.min.x) &&
           (min.y <= other.max.y && max.y >= other.min.y);
}
