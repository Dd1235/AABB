#pragma once

#include <vector>
#include "../Math/Vector2.h"

class PolygonIntersection
{
public:
    static std::vector<Vector2> computeIntersection(const std::vector<Vector2> &subjectPolygon, const std::vector<Vector2> &clipPolygon);
};
