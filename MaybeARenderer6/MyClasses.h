#pragma once
#include <windows.h>
#include <format>


using std::format;


struct Point;
struct Line;
struct PointHash;
struct LineHash;
interface IDrawEachFrame;
interface IFlushable;

double GetDistance(const Point& p1, const Point& p2);









struct Colors {
    COLORREF RED = RGB(255, 0, 0);
    COLORREF GREEN = RGB(0, 255, 0);
    COLORREF BLUE = RGB(0, 0, 255);
    COLORREF WHITE = RGB(255, 255, 255);
}MyColors;



// Point Struct
struct Point {
    int x = 0;
    int y = 0;
    int z = 0;



    COLORREF color = MyColors.WHITE;

    Point() : x(0), y(0), z(0), color(MyColors.WHITE) {} // Default constructor
    // A constructor for implicit conversions
    Point(int _d) : x(_d), y(_d), z(_d), color(MyColors.WHITE) {}

    // default / custom constructor
    Point(int _x, int _y, int _z = 0, COLORREF col = MyColors.WHITE)
        : x(_x), y(_y), z(_z), color(col) {}

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    bool operator!=(const Point& other) const {
        return !(*this == other);
    }

    // += operator: adds x/y/z only (keeps first color)
    Point& operator+=(const Point& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    // -= operator: subtracts x/y/z only (keeps first color)
    Point& operator-=(const Point& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }
};

// Hash for Point
struct PointHash {
    size_t operator()(const Point& p) const {
        return std::hash<int>()(p.x) ^ std::hash<int>()(p.y) ^ std::hash<int>()(p.z);
    }
};

// Line Struct
struct Line {
    Point p1 = {}, p2 = {}, Middle = {};
    std::string lengthStr = "";
    COLORREF color = MyColors.WHITE;

    Line(const Point& a, const Point& b, COLORREF col = MyColors.WHITE)
        : p1(a), p2(b), color(col),
        Middle({ (p1.x + p2.x) / 2 ,
                 (p1.y + p2.y) / 2 ,
                 (p1.z + p2.z) / 2 , col }),
        lengthStr(std::format("{:.2f}", GetDistance(a, b))) {}

    bool operator==(const Line& other) const {
        return (p1 == other.p1 && p2 == other.p2) || (p1 == other.p2 && p2 == other.p1);
    }
};

// Hash for Line
struct LineHash {
    size_t operator()(const Line& line) const {
        return PointHash{}(line.p1) ^ PointHash {}(line.p2);
    }
};

interface IDrawEachFrame {
	virtual void EachFrame(HDC& targethdc) = 0;
};

interface IFlushable {
	virtual void Flush() = 0;
};






