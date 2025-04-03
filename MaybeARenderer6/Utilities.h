#pragma once
#include <windows.h>
#include <vector>
#include <stdexcept>
#include <unordered_set>
using std::vector;

COLORREF RED = RGB(255, 0, 0);
COLORREF GREEN = RGB(0, 255, 0);
COLORREF BLUE = RGB(0, 0, 255);
COLORREF WHITE = RGB(255, 255, 255);


const int screenWidth = 800;
const int screenHeight = 600;

const float PI = 3.14159265358979323846;
bool quit = false;
const int PointBoldness = 10;
const int LineBoldness = 1;
double angle = 0;
const double angleChangeSpeed = 0.01;
const bool ToDrawShape = false;

const int drawingCapacity = 20;



// Enum for Drawings
enum Drawings {
    Rect,
    Pyramid,
    DoublePyramid,
    PolygonWith2Heads
}WantedDrawing;


// Point Struct
struct Point {
    int x = 0;
    int y = 0;
    int z = 0;

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
};


// Function Declarations
void DrawBoldPoint(HDC hdc, int x, int y, int boldness, COLORREF color = WHITE);
void DrawLine(HDC hdc, Point p1, Point p2, int boldness, COLORREF color = WHITE);
Point MultiplyMatrixByPoint(double matrix[3][3], Point p);
Point RotatePointAround(Point p, Point pivot, double matrix[3][3]);
void OnLeftMouseClick(HWND hwnd);

// Hash for Point
struct PointHash {
    size_t operator()(const Point& p) const {
        return std::hash<int>()(p.x) ^ std::hash<int>()(p.y) ^ std::hash<int>()(p.z);
    }
};



// Line Struct
struct Line {
    Point p1, p2;
    Line(Point a, Point b) : p1(a), p2(b) {}
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

// PointManager Class
class PointManager {
private:
    std::unordered_set<Point, PointHash> points;
    int capacity;

public:
    PointManager(int cap) : capacity(cap) {}
    void insert(Point p) {
        if (points.size() >= capacity) {
            points.erase(points.begin());
        }
        points.insert(p);
    }
    void remove(Point p) {
        points.erase(p);
    }
    bool checkIntersection(Point p) {
        return points.find(p) != points.end();
    }
	void DrawPoints(HDC hdc) {
		for (const auto& p : points) {
			DrawBoldPoint(hdc, p.x, p.y, PointBoldness);
		}
	}
};

// LineManager Class
class LineManager {
private:
    std::unordered_set<Line, LineHash> lines;
    PointManager& pointManager;

public:
    LineManager(PointManager& pm) : pointManager(pm) {}
    void addLine(Point P, Point Q) {
        if (!(pointManager.checkIntersection(P) && pointManager.checkIntersection(Q))) {
            lines.insert(Line(P, Q));
        }
    }
    void removeLine(Point P, Point Q) {
        lines.erase(Line(P, Q));
    }
    void removeLinesWithPoint(Point P) {
        for (auto it = lines.begin(); it != lines.end();) {
            if (it->p1 == P || it->p2 == P) {
                it = lines.erase(it);
            }
            else {
                ++it;
            }
        }
    }
	void DrawLines(HDC hdc) {
		for (const auto& line : lines) {
			DrawLine(hdc, line.p1, line.p2, LineBoldness);
		}
	}
};


PointManager PointsToDraw(drawingCapacity);
LineManager LinesToDraw(PointsToDraw);

// Function Definitions
void DrawBoldPoint(HDC hdc, int x, int y, int boldness, COLORREF color) {
    for (int i = -boldness; i <= boldness; ++i) {
        for (int j = -boldness; j <= boldness; ++j) {
            SetPixel(hdc, x + i, y + j, color);
        }
    }
}

void DrawLine(HDC hdc, Point p1, Point p2, int boldness, COLORREF color) {
    int dx = abs(p2.x - p1.x);
    int dy = abs(p2.y - p1.y);
    int sx = (p1.x < p2.x) ? 1 : -1;
    int sy = (p1.y < p2.y) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        DrawBoldPoint(hdc, p1.x, p1.y, boldness, color);
        if (p1.x == p2.x && p1.y == p2.y) break;
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            p1.x += sx;
        }
        if (e2 < dx) {
            err += dx;
            p1.y += sy;
        }
    }
}

Point MultiplyMatrixByPoint(double matrix[3][3], Point p) {
    Point result;
    result.x = (int)(matrix[0][0] * p.x + matrix[0][1] * p.y + matrix[0][2] * p.z);
    result.y = (int)(matrix[1][0] * p.x + matrix[1][1] * p.y + matrix[1][2] * p.z);
    result.z = (int)(matrix[2][0] * p.x + matrix[2][1] * p.y + matrix[2][2] * p.z);
    return result;
}

Point RotatePointAround(Point p, Point pivot, double matrix[3][3]) {
    p.x -= pivot.x;
    p.y -= pivot.y;
    p.z -= pivot.z;
    Point result = MultiplyMatrixByPoint(matrix, p);
    result.x += pivot.x;
    result.y += pivot.y;
    result.z += pivot.z;
    return result;
}

void OnLeftMouseClick(HWND hwnd) {
    POINT cursPos;
    GetCursorPos(&cursPos);
    ScreenToClient(hwnd, &cursPos);
    Point toAdd = { cursPos.x, cursPos.y };
	PointsToDraw.insert(toAdd);
    LinesToDraw.addLine(toAdd, { screenWidth / 2, screenHeight / 2 });
}



Point Middle = { screenWidth / 2, screenHeight / 2 };
Point pivot = { Middle.x , Middle.y, Middle.z };


// TODO : FIX CODE STRUCTURE