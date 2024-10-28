#pragma once
#include "../../Math/Vector2.h"

struct AABB
{
    Vector2 min;
    Vector2 max;

    AABB();
    AABB(const Vector2 &min, const Vector2 &max);

    bool intersects(const AABB &other) const;
};
