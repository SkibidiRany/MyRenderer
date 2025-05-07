#include "MatrixManager.h"
#include <cmath>
#include "Constants.h"

Matrix4 MatrixManager::PerspectiveProject;
bool MatrixManager::isPerspectiveInitialized = false;


MatrixManager::MatrixManager() {}

Matrix4 MatrixManager::CreatePerspective(double fovDegrees, double aspect, double nearPlane, double farPlane) {
    if (!isPerspectiveInitialized) {
        double fovRad = fovDegrees * (PI / 180.0);
        double f = 1.0 / std::tan(fovRad / 2.0);
        double nf = 1.0 / (nearPlane - farPlane);

        PerspectiveProject = Matrix4(); // Clear just in case
        PerspectiveProject.m[0][0] = f / aspect;
        PerspectiveProject.m[1][1] = f;
        PerspectiveProject.m[2][2] = (farPlane + nearPlane) * nf;
        PerspectiveProject.m[2][3] = (2 * farPlane * nearPlane) * nf;
        PerspectiveProject.m[3][2] = -1;
        PerspectiveProject.m[3][3] = 0;

        isPerspectiveInitialized = true;
    }
    return PerspectiveProject;
}


Matrix4 MatrixManager::CreateTranslation(double x, double y, double z) {
    Matrix4 mat = Matrix4::eye();
    mat.m[0][3] = x;
    mat.m[1][3] = y;
    mat.m[2][3] = z;
    return mat;
}

Matrix4 MatrixManager::CreateScale(double x, double y, double z) {
    Matrix4 mat = Matrix4::eye();
    mat.m[0][0] = x;
    mat.m[1][1] = y;
    mat.m[2][2] = z;
    return mat;
}

Matrix4 MatrixManager::CreateRotationX(double radians) {
    Matrix4 mat = Matrix4::eye();
    double c = std::cos(radians);
    double s = std::sin(radians);
    mat.m[1][1] = c;
    mat.m[1][2] = -s;
    mat.m[2][1] = s;
    mat.m[2][2] = c;
    return mat;
}

Matrix4 MatrixManager::CreateRotationY(double radians) {
    Matrix4 mat = Matrix4::eye();
    double c = std::cos(radians);
    double s = std::sin(radians);
    mat.m[0][0] = c;
    mat.m[0][2] = s;
    mat.m[2][0] = -s;
    mat.m[2][2] = c;
    return mat;
}

Matrix4 MatrixManager::CreateRotationZ(double radians) {
    Matrix4 mat = Matrix4::eye();
    double c = std::cos(radians);
    double s = std::sin(radians);
    mat.m[0][0] = c;
    mat.m[0][1] = -s;
    mat.m[1][0] = s;
    mat.m[1][1] = c;
    return mat;
}

void MatrixManager::AddMatrix(const Matrix4& matrix) {
    matrices.push_back(matrix);
}

const std::vector<Matrix4>& MatrixManager::GetAllMatrices() const {
    return matrices;
}
