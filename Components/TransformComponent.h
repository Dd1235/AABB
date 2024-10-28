#pragma once

#include "../Math/Vector2.h"

struct TransformComponent
{
    Vector2 position;
    float rotation;
    float scale;

    TransformComponent(const Vector2 &pos = Vector2(), float rot = 0.0f, const float scale = 1.0f)
        : position(pos), rotation(rot), scale(scale) {}
};
