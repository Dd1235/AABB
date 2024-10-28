#include "ShapeFactory.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

std::vector<Vector2> ShapeFactory::createRegularPolygon(int sides, float radius)
{
    std::vector<Vector2> vertices;
    float angleIncrement = 2.0f * M_PI / sides;
    for (int i = 0; i < sides; ++i)
    {
        float angle = i * angleIncrement;
        vertices.emplace_back(radius * std::cos(angle), radius * std::sin(angle));
    }
    return vertices;
}
