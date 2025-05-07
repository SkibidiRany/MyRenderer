#include "BaseFunctions.h"
#include <string>
#include <format>

#include "Constants.h"
#include "MyClasses.h"

bool PositionIsLegal(Point p) {
    return (p.x >= 0 && p.x < screenWidth && p.y >= 0 && p.y < screenHeight);
}

const char* GetDistanceAsConstChar(const Point& p1, const Point& p2) {
    static std::string distanceStr; 
    distanceStr = std::format("{:.2f}", GetDistance(p1, p2));
    return distanceStr.c_str();
}

float fastSqrt(float number) {
    long i;
    float x2, y;
    const float threehalfs = 1.5F;

    x2 = number * 0.5F;
    y = number;
    i = *(long*)&y;
    i = 0x5f3759df - (i >> 1);
    y = *(float*)&i;
    y = y * (threehalfs - (x2 * y * y));  // 1st iteration

    return y;
}

double GetDistance(const Point& p1, const Point& p2) {
    double dx = std::abs(p1.x - p2.x);
    double dy = std::abs(p1.y - p2.y);
    double dz = std::abs(p1.z - p2.z);

    double maxD = dx, midD = dy, minD = dz;

    if (maxD < midD) std::swap(maxD, midD);
    if (midD < minD) std::swap(midD, minD);
    if (maxD < midD) std::swap(maxD, midD);

    return 0.939 * maxD + 0.389 * midD + 0.298 * minD;
}

Point GetCursPos(HWND hwnd) {
    POINT cursPos;
    GetCursorPos(&cursPos);
    ScreenToClient(hwnd, &cursPos);
    return { cursPos.x, cursPos.y };
}

void LogMessage(const char* message) {
    OutputDebugStringA(message);
}

Point MultiplyMatrixByPoint(const double matrix[3][3], const Point& p) {
    if (!matrix) return p;

    Point result;
    result.x = static_cast<int>(matrix[0][0] * p.x + matrix[0][1] * p.y + matrix[0][2] * p.z);
    result.y = static_cast<int>(matrix[1][0] * p.x + matrix[1][1] * p.y + matrix[1][2] * p.z);
    result.z = static_cast<int>(matrix[2][0] * p.x + matrix[2][1] * p.y + matrix[2][2] * p.z);
    result.color = p.color;

    return result;
}

Point RotatePointAround(Point p, Point piv, double matrix[3][3]) {
    p -= piv;
    Point result = MultiplyMatrixByPoint(matrix, p);
    result += piv;
    return result;
}


