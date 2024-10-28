#include "Vector2.h"

Vector2::Vector2() : x(0.0f), y(0.0f) {}

Vector2::Vector2(float xVal, float yVal) : x(xVal), y(yVal) {}

Vector2 Vector2::operator+(const Vector2 &other) const
{
    return Vector2(x + other.x, y + other.y);
}

Vector2 Vector2::operator-(const Vector2 &other) const
{
    return Vector2(x - other.x, y - other.y);
}

Vector2 Vector2::operator*(float scalar) const
{
    return Vector2(x * scalar, y * scalar);
}

float Vector2::dot(const Vector2 &other) const
{
    return x * other.x + y * other.y;
}

float Vector2::length() const
{
    return std::sqrt(x * x + y * y);
}

Vector2 Vector2::normalize() const
{
    float len = length();
    if (len == 0.0f)
        return Vector2(0.0f, 0.0f);
    return Vector2(x / len, y / len);
}

Vector2 Vector2::perpendicular() const
{
    return Vector2(-y, x);
}
