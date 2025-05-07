#pragma once









#include "Matrix4.h"
#include <vector>

class MatrixManager {
private:
    std::vector<Matrix4> matrices;

    static Matrix4 PerspectiveProject;
    static bool isPerspectiveInitialized; 
public:
    MatrixManager();

    static Matrix4 CreatePerspective(double fovDegrees, double aspect, double nearPlane, double farPlane);
    Matrix4 CreateTranslation(double x, double y, double z);
    Matrix4 CreateScale(double x, double y, double z);
    Matrix4 CreateRotationX(double radians);
    Matrix4 CreateRotationY(double radians);
    Matrix4 CreateRotationZ(double radians);

    void AddMatrix(const Matrix4& matrix);
    const std::vector<Matrix4>& GetAllMatrices() const;


};