#pragma once
#include "MyClasses.h"



class Vector4 {
public:
    double x, y, z, w;

    Vector4(double x = 0, double y = 0, double z = 0, double w = 1);

    Point ToPoint() const;

    Vector4 operator+(const Vector4& other) const;
    Vector4 operator-(const Vector4& other) const;
    Vector4 operator*(double scalar) const;

};
