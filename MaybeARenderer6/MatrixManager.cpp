#include "MatrixManager.h"
#include <cmath>
#include "Constants.h"

Matrix4 MatrixManager::perspectiveMatrix;
bool MatrixManager::perspectiveInitialized = false;

MatrixManager::MatrixManager() {}

Matrix4 MatrixManager::CreatePerspective(double fovDegrees, double aspect, double nearPlane, double farPlane) {
    if (!perspectiveInitialized) {
        double fovRad = fovDegrees * (PI / 180.0);
        double f = 1.0 / std::tan(fovRad / 2.0);
        double nf = 1.0 / (nearPlane - farPlane);

        perspectiveMatrix = Matrix4();
        perspectiveMatrix.m[0][0] = f / aspect;
        perspectiveMatrix.m[1][1] = f;
        perspectiveMatrix.m[2][2] = (farPlane + nearPlane) * nf;
        perspectiveMatrix.m[2][3] = (2 * farPlane * nearPlane) * nf;
        perspectiveMatrix.m[3][2] = -1.0;
        perspectiveMatrix.m[3][3] = 0.0;

        perspectiveInitialized = true;
    }
    return perspectiveMatrix;
}

Matrix4 MatrixManager::CreateView(const Vector4& eye, const Vector4& target, const Vector4& up) {
    Vector4 z = (eye - target); // camera direction
    double lenZ = std::sqrt(z.x * z.x + z.y * z.y + z.z * z.z);
    z = Vector4(z.x / lenZ, z.y / lenZ, z.z / lenZ);

    Vector4 x = Vector4(
        up.y * z.z - up.z * z.y,
        up.z * z.x - up.x * z.z,
        up.x * z.y - up.y * z.x,
        0
    );
    double lenX = std::sqrt(x.x * x.x + x.y * x.y + x.z * x.z);
    x = Vector4(x.x / lenX, x.y / lenX, x.z / lenX);

    Vector4 y = Vector4(
        z.y * x.z - z.z * x.y,
        z.z * x.x - z.x * x.z,
        z.x * x.y - z.y * x.x,
        0
    );

    Matrix4 view = Matrix4::eye();
    view.m[0][0] = x.x; view.m[0][1] = x.y; view.m[0][2] = x.z; view.m[0][3] = -(x.x * eye.x + x.y * eye.y + x.z * eye.z);
    view.m[1][0] = y.x; view.m[1][1] = y.y; view.m[1][2] = y.z; view.m[1][3] = -(y.x * eye.x + y.y * eye.y + y.z * eye.z);
    view.m[2][0] = z.x; view.m[2][1] = z.y; view.m[2][2] = z.z; view.m[2][3] = -(z.x * eye.x + z.y * eye.y + z.z * eye.z);
    return view;
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
    double c = std::cos(radians), s = std::sin(radians);
    mat.m[1][1] = c;
    mat.m[1][2] = -s;
    mat.m[2][1] = s;
    mat.m[2][2] = c;
    return mat;
}

Matrix4 MatrixManager::CreateRotationY(double radians) {
    Matrix4 mat = Matrix4::eye();
    double c = std::cos(radians), s = std::sin(radians);
    mat.m[0][0] = c;
    mat.m[0][2] = s;
    mat.m[2][0] = -s;
    mat.m[2][2] = c;
    return mat;
}

Matrix4 MatrixManager::CreateRotationZ(double radians) {
    Matrix4 mat = Matrix4::eye();
    double c = std::cos(radians), s = std::sin(radians);
    mat.m[0][0] = c;
    mat.m[0][1] = -s;
    mat.m[1][0] = s;
    mat.m[1][1] = c;
    return mat;
}

Point MatrixManager::TransformPoint(const Point& p, MatrixManager& manager, int screenWidth, int screenHeight) {
    // Convert Point to Vector4
    Vector4 vec(static_cast<double>(p.x), static_cast<double>(p.y), static_cast<double>(p.z), 1.0);

    // Apply all matrices stored in the manager
    Vector4 transformed = manager.ApplyAll(vec);

    // Perspective divide
    if (transformed.w != 0.0) {
        transformed.x /= transformed.w;
        transformed.y /= transformed.w;
        transformed.z /= transformed.w;
    }

    // Convert from normalized device coordinates to screen space
    int screenX = static_cast<int>((transformed.x + 1.0) * 0.5 * screenWidth);
    int screenY = static_cast<int>((1.0 - transformed.y) * 0.5 * screenHeight); // Y is flipped in screen space

    // Return the screen-space point
    return Point(screenX, screenY, transformed.z, p.color);
}


void MatrixManager::AddMatrix(const Matrix4& matrix) {
    matrices.push_back(matrix);
}

const std::vector<Matrix4>& MatrixManager::GetAllMatrices() const {
    return matrices;
}

Vector4 MatrixManager::ApplyAll(const Vector4& point) const {
    Vector4 result = point;
    for (const auto& mat : matrices) {
        result = mat * result;
    }
    return result;
}
