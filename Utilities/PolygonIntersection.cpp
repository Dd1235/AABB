#include "PolygonIntersection.h"

static float cross(const Vector2 &a, const Vector2 &b)
{
    return a.x * b.y - a.y * b.x;
}

static bool inside(const Vector2 &p, const Vector2 &a, const Vector2 &b)
{
    return cross(b - a, p - a) >= 0;
}

static Vector2 intersection(const Vector2 &cp1, const Vector2 &cp2, const Vector2 &s, const Vector2 &e)
{
    Vector2 dc = cp1 - cp2;
    Vector2 dp = s - e;
    float n1 = cross(cp1, cp2);
    float n2 = cross(s, e);
    float n3 = cross(dc, dp);
    if (n3 == 0.0f)
        return Vector2(); // Lines are parallel
    return Vector2((n1 * dp.x - n2 * dc.x) / n3, (n1 * dp.y - n2 * dc.y) / n3);
}

std::vector<Vector2> PolygonIntersection::computeIntersection(const std::vector<Vector2> &subjectPolygon, const std::vector<Vector2> &clipPolygon)
{
    std::vector<Vector2> outputList = subjectPolygon;

    size_t cp1Count = clipPolygon.size();
    for (size_t i = 0; i < cp1Count; ++i)
    {
        Vector2 cp1 = clipPolygon[i];
        Vector2 cp2 = clipPolygon[(i + 1) % cp1Count];

        std::vector<Vector2> inputList = outputList;
        outputList.clear();

        if (inputList.empty())
            return outputList;

        Vector2 s = inputList.back();

        for (const Vector2 &e : inputList)
        {
            if (inside(e, cp1, cp2))
            {
                if (!inside(s, cp1, cp2))
                {
                    outputList.push_back(intersection(cp1, cp2, s, e));
                }
                outputList.push_back(e);
            }
            else if (inside(s, cp1, cp2))
            {
                outputList.push_back(intersection(cp1, cp2, s, e));
            }
            s = e;
        }
    }

    return outputList;
}
