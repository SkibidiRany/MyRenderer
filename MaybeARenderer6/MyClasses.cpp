#include "MyClasses.h"

// Define global Colors instance
struct Colors MyColors;

// Define Line member functions
Line::Line(const Point& a, const Point& b, COLORREF col)
    : p1(a), p2(b), color(col),
    Middle({ (p1.x + p2.x) / 2, (p1.y + p2.y) / 2, (p1.z + p2.z) / 2, col }),
    lengthStr(std::format("{:.2f}", GetDistance(a, b))) {}

Line& Line::operator=(const Line& other) {
    if (this != &other) {
        p1 = other.p1;
        p2 = other.p2;
        Middle = other.Middle;
        lengthStr = other.lengthStr;
        color = other.color;
    }
    return *this;
}


bool Line::operator==(const Line& other) const {
    return (p1 == other.p1 && p2 == other.p2) || (p1 == other.p2 && p2 == other.p1);
}