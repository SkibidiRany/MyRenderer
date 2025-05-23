#pragma once

#include "Matrix4.h"
#include "Vector4.h"
#include <vector>

class MatrixManager {
private:
    std::vector<Matrix4> matrices;

    static Matrix4 perspectiveMatrix;
    static bool perspectiveInitialized;

public:
    MatrixManager();

    // Projection
    static Matrix4 CreatePerspective(double fovDegrees, double aspect, double nearPlane, double farPlane);

    // View
    static Matrix4 CreateView(const Vector4& eye, const Vector4& target, const Vector4& up);

    // Transforms
    static Matrix4 CreateTranslation(double x, double y, double z);
    static Matrix4 CreateScale(double x, double y, double z);
    static Matrix4 CreateRotationX(double radians);
    static Matrix4 CreateRotationY(double radians);
    static Matrix4 CreateRotationZ(double radians);

    static Point TransformPoint(const Point& p, MatrixManager& manager, int screenWidth, int screenHeight);

    // Matrix stack
    void AddMatrix(const Matrix4& matrix);
    const std::vector<Matrix4>& GetAllMatrices() const;

    // Apply transformations
    Vector4 ApplyAll(const Vector4& point) const;
};
