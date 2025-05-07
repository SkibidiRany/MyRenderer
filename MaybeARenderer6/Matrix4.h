#pragma once




#include "Vector4.h"

class Matrix4 {

    static const double IdentityMatrix[4][4];

public:
    double m[4][4];
    Matrix4();
    Matrix4(double scalar); // scalar * identity
    Matrix4(const double elements[4][4]); // full init
    Matrix4(const Matrix4& other); // copy constructor

    static Matrix4 eye();

    Matrix4 operator+(const Matrix4& other) const;
    Matrix4 operator*(const Matrix4& other) const;
    Vector4 operator*(const Vector4& v) const;
    bool operator==(const Matrix4& other) const;


};

