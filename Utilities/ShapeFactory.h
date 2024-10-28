#pragma once

#include <vector>
#include "../Math/Vector2.h"
#include "../Components/ShapeType.h"

class ShapeFactory
{
public:
    static std::vector<Vector2> createRegularPolygon(int sides, float radius);
};
