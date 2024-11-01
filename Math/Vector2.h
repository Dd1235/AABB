#pragma once
#include <cmath>

class Vector2
{
public:
    float x;
    float y;

    Vector2();
    Vector2(float xVal, float yVal);

    Vector2 operator+(const Vector2 &other) const;
    Vector2 operator-(const Vector2 &other) const;
    Vector2 operator*(float scalar) const;
    float dot(const Vector2 &other) const;
    float length() const;
    Vector2 normalize() const;
    Vector2 perpendicular() const;
};