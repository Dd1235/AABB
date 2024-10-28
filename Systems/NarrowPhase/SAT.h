#pragma once

#include <vector>
#include "../../Math/Vector2.h"

class SAT
{
public:
    static bool checkCollision(const std::vector<Vector2> &shapeA, const std::vector<Vector2> &shapeB);
};
