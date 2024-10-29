#include "PolygonUtils.h"
#include <cmath>

float PolygonUtils::computeArea(const std::vector<Vector2> &polygon)
{
    float area = 0.0f;
    size_t n = polygon.size();

    for (size_t i = 0; i < n; ++i)
    {
        const Vector2 &p1 = polygon[i];
        const Vector2 &p2 = polygon[(i + 1) % n];
        area += (p1.x * p2.y - p2.x * p1.y);
    }

    return std::abs(area) * 0.5f;
}
