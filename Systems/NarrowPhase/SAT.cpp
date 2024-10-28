#include "SAT.h"
#include <cfloat>

bool SAT::checkCollision(const std::vector<Vector2> &shapeA, const std::vector<Vector2> &shapeB)
{
    std::vector<Vector2> axes;

    // The SAT algorithm projects shapes onto aces that are perpendicular to each edge of both shapes. This si because for convex polygons the potential separating aces lie perpendicular to the edges of the shapes.

    // Compute axes for shapeA
    for (size_t i = 0; i < shapeA.size(); ++i)
    {
        Vector2 edge = shapeA[(i + 1) % shapeA.size()] - shapeA[i];
        Vector2 axis = edge.perpendicular().normalize();
        axes.push_back(axis);
    }

    // Compute axes for shapeB
    for (size_t i = 0; i < shapeB.size(); ++i)
    {
        Vector2 edge = shapeB[(i + 1) % shapeB.size()] - shapeB[i];
        Vector2 axis = edge.perpendicular().normalize();
        axes.push_back(axis);
    }

    // Perform SAT on all axes
    for (const auto &axis : axes)
    {
        float minA = FLT_MAX, maxA = -FLT_MAX;
        for (const auto &vert : shapeA)
        {
            float projection = vert.dot(axis);
            minA = std::min(minA, projection);
            maxA = std::max(maxA, projection);
        }

        float minB = FLT_MAX, maxB = -FLT_MAX;
        for (const auto &vert : shapeB)
        {
            float projection = vert.dot(axis);
            minB = std::min(minB, projection);
            maxB = std::max(maxB, projection);
        }

        if (maxA < minB || maxB < minA)
        {
            // Separating axis found, no collision
            return false;
        }
    }

    // No separating axis found, collision detected
    return true;
}
