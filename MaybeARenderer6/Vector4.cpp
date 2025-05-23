#include "Vector4.h"

Vector4::Vector4(double x, double y, double z, double w) : x(x), y(y), z(z), w(w) {}

Point Vector4::ToPoint() const
{
    return Point(x, y, z);
}

Vector4 Vector4::operator+(const Vector4& other) const {
    return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
}

Vector4 Vector4::operator-(const Vector4& other) const {
    return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
}

Vector4 Vector4::operator*(double scalar) const {
    return Vector4(x * scalar, y * scalar, z * scalar, w * scalar);
}