#pragma once
#include <windows.h>
#include <format>
using std::format;

// Colors struct declaration
struct Colors {
    COLORREF RED = RGB(255, 0, 0);
    COLORREF GREEN = RGB(0, 255, 0);
    COLORREF BLUE = RGB(0, 0, 255);
    COLORREF WHITE = RGB(255, 255, 255);
};
extern struct Colors MyColors; // Extern declaration for global variable

// Forward declarations
struct Point;
struct Line;
struct PointHash;
struct LineHash;
interface IDrawEachFrame;
interface IFlushable;

// Function declarations
double GetDistance(const Point& p1, const Point& p2);

// Point struct declaration
struct Point {
    int x = 0;
    int y = 0;
    int z = 0;
    COLORREF color = MyColors.WHITE;

    Point() : x(0), y(0), z(0), color(MyColors.WHITE) {} // Default constructor
    Point(int _d) : x(_d), y(_d), z(_d), color(MyColors.WHITE) {}
    Point(int _x, int _y, int _z = 0, COLORREF col = MyColors.WHITE)
        : x(_x), y(_y), z(_z), color(col) {}

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
    bool operator!=(const Point& other) const {
        return !(*this == other);
    }
    Point& operator+=(const Point& other) {
        x += other.x; y += other.y; z += other.z;
        return *this;
    }
    Point& operator-=(const Point& other) {
        x -= other.x; y -= other.y; z -= other.z;
        return *this;
    }
};

// PointHash struct
struct PointHash {
    size_t operator()(const Point& p) const {
        return std::hash<int>()(p.x) ^ std::hash<int>()(p.y) ^ std::hash<int>()(p.z);
    }
};

// Line struct declaration
struct Line {
    Point p1 = {}, p2 = {}, Middle = {};
    std::string lengthStr = "";
    COLORREF color = MyColors.WHITE;

    Line(const Point& a, const Point& b, COLORREF col = MyColors.WHITE);
    bool operator==(const Line& other) const;
};

// LineHash struct
struct LineHash {
    size_t operator()(const Line& line) const {
        return PointHash{}(line.p1) ^ PointHash {}(line.p2);
    }
};

// Interfaces
interface IDrawEachFrame {
    virtual void EachFrame(HDC& targethdc) = 0;
};

interface IFlushable {
    virtual void Flush() = 0;
};