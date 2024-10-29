#pragma once

#include <vector>
#include "../Math/Vector2.h"
#include "ShapeType.h"

// provides data for collision detection

struct ColliderComponent
{
    std::vector<Vector2> vertices; // Local space vertices
    Vector2 offset;                // Offset from the transform position
    ShapeType shapeType;           // Type of the shape

    ColliderComponent(const std::vector<Vector2> &verts, ShapeType type = ShapeType::Custom, const Vector2 &off = Vector2())
        : vertices(verts), offset(off), shapeType(type) {}
};
