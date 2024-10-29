#pragma once

#include <vector>
#include "../Math/Vector2.h"

class PolygonUtils
{
public:
    static float computeArea(const std::vector<Vector2> &polygon);
};
