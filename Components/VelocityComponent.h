#pragma once

#include "../Math/Vector2.h"

struct VelocityComponent
{
    Vector2 velocity;

    VelocityComponent(const Vector2 &vel = Vector2())
        : velocity(vel) {}
};
