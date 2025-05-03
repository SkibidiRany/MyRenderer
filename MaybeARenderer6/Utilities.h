#pragma once
#define NOMINMAX
#include <windows.h>
#include <algorithm>
#include <vector>
#include <stdexcept>
#include <unordered_set>
#include <unordered_map>    
#include <queue>
#include "InputFields.h"
#include <thread>
#include <condition_variable>
#include <locale>
#include <memory>
#include <format>
#include <string>

using std::vector;
using std::string;

class PointManager;
class LineManager;

#define X 0
#define Y 1
#define Z 2

const bool ToRotate[] = { false, true, false };

PointManager* PointsToDraw;
LineManager* LinesToDraw;



COLORREF RED = RGB(255, 0, 0);
COLORREF GREEN = RGB(0, 255, 0);
COLORREF BLUE = RGB(0, 0, 255);
COLORREF WHITE = RGB(255, 255, 255);

HWND window_handle;
HWND rgb_window_handle;

const int screenWidth = 800;
const int screenHeight = 600;

const float PI = 3.14159265358979323846f;
bool quit = false;
const int PointBoldness = 10;
const int LineBoldness = 1;


double angle = 0;
double angleChangeSpeed = 0.001;
const bool AutoAngleChangeSpeed = false; // if false, the angle change speed will be set to the value in the input field

const bool ToDrawShape = true;
const bool AutoRotate = true;

const int drawingCapacity = 60;





// Enum for Drawings
enum Drawings {
    Rect,
    Pyramid,
    DoublePyramid,
    PolygonWith2Heads,
    Heart
};

Drawings WantedDrawing = Heart;




// Point Struct
struct Point {
    int x = 0;
    int y = 0;
    int z = 0;
    
    std::string CoordStr = "";


    COLORREF color = WHITE;

	Point() : x(0), y(0), z(0), color(WHITE), CoordStr(std::format("{},{},{}",x,y,z)) {} // Default constructor
    // A constructor for implicit conversions
    Point(int _d) : x(_d), y(_d), z(_d), color(WHITE), CoordStr(std::format("{},{},{}", x, y, z)) {}

	// default / custom constructor
    Point(int _x, int _y, int _z = 0, COLORREF col = WHITE)
        : x(_x), y(_y), z(_z), color(col), CoordStr(std::format("{},{},{}", x, y, z)) {}

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


double GetDistance(const Point& p1, const Point& p2);
const char* GetDistanceAsConstChar(const Point& p1, const Point& p2);
float fastSqrt(float number);




// Line Struct
struct Line {
    Point p1 = {}, p2 = {}, Middle = {};
    std::string lengthStr = ""; 
    COLORREF color = WHITE;

    Line(const Point& a, const Point& b, COLORREF col = WHITE)
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

Point Middle = { screenWidth / 2, screenHeight / 2, 0 };
Point pivot = { Middle.x , Middle.y, Middle.z };
Point LastCursPos = { 0, 0, 0 };

// Function Declarations

void LogMessage(const char* message);
void DrawBoldPoint(HDC& hdc, const Point& p, int boldness, COLORREF color = WHITE);
void DrawLine(HDC& hdc, const Line& line, const int boldness, const COLORREF& color = WHITE);
void DrawTextAtMiddle(HDC hdc, Line line, const char* text, COLORREF color = WHITE);
Point MultiplyMatrixByPoint(const double matrix[3][3], const Point& p);
Point RotatePointAround(Point p, Point pivot, double matrix[3][3]);
bool PositionIsLegal(Point p);
void RemovePoint(Point p);
void OnLeftMouseDown(HWND hwnd);
void OnLeftMouseHold(HWND hwnd, HDC hdc);
void OnLeftMouseUp(HWND hwnd);




interface IDrawEachFrame {
    virtual void EachFrame(HDC& targethdc) = 0;
};


// PointManager Class
class PointManager : public IDrawEachFrame {
private:
    std::unordered_map<Point, std::vector<Point>, PointHash> buckets;
    std::unordered_set<Point, PointHash> points;
    std::queue<Point> insertionOrder;
    int _capacity;
    int _c;


public:
    Point LastDrawn = { 0, 0, 0 };

    // Constructor
    PointManager(int cap, int c) : _capacity(cap), _c(c) {}

    Point ToBucket(Point p) {
        int bucketX = p.x / _c;
        int bucketY = p.y / _c;
        int bucketZ = p.z / _c;
        return { bucketX, bucketY };
    }

    Point insert(Point p) {

        Point checker = CheckIntersection(p);
        LastDrawn = checker;
        if (checker != p) return checker; // if we found a point checker intersecting with p, return it 

        // if we didn't find such point, add p
        if (points.size() >= _capacity) {
            Point oldest = insertionOrder.front();
            insertionOrder.pop();
            RemovePoint(oldest);
        }

        p.color = lastColorFromInputs;
        points.insert(p);
        insertionOrder.push(p);
        buckets[ToBucket(p)].push_back(p);
        return p;
    }

    void remove(Point p) {
        points.erase(p);
        Point key = ToBucket(p);
        auto& vec = buckets[key];
        vec.erase(std::remove(vec.begin(), vec.end(), p), vec.end());
    }

    Point CheckIntersection(Point p) {
        Point base = ToBucket(p);
        for (int dx = -1; dx <= 1; ++dx)
            for (int dy = -1; dy <= 1; ++dy)
                for (int dz = -1; dz <= 1; ++dz) {
                    Point neighbor = { base.x + dx, base.y + dy, base.z + dz };
                    auto it = buckets.find(neighbor);
                    if (it != buckets.end()) {
                        for (const Point& q : it->second) {
                            if (std::abs(q.x - p.x) <= _c &&
                                std::abs(q.y - p.y) <= _c &&
                                std::abs(q.z - p.z) <= _c) {
                                return q;  // Found nearby match
                            }
                        }
                    }
                }
        return p; // no match
    }

    void DrawPoints(HDC targetHDC) {
        for (const auto& p : points) {
            DrawBoldPoint(targetHDC, p, PointBoldness, p.color);
        }
    }

	void EachFrame(HDC& targetHDC) override {
		DrawPoints(targetHDC);
	}

};



// LineManager Class
class LineManager : public IDrawEachFrame{
private:
    std::unordered_set<Line, LineHash> lines;

public:

    LineManager() = default;

    void addLine(Point& P, Point& Q) {
        lines.insert({ P, Q, lastColorFromInputs });
    }

    void removeLine(Point& P, Point& Q) {
        lines.erase(Line{ P, Q });
    }

    void removeLinesWithPoint(Point& P) {
        for (auto it = lines.begin(); it != lines.end();) {
            if (it->p1 == P || it->p2 == P) {
                it = lines.erase(it);
            }
            else {
                ++it;
            }
        }
    }

    void DrawLines(HDC& targetHDC) {
        for (const auto& l : lines) {
            DrawLine(targetHDC, l, LineBoldness, l.color);
        }
    }

    void EachFrame(HDC& targetHDC) override {
        DrawLines(targetHDC);
    }

};


// Function Definitions
double GetDistance(const Point& p1, const Point& p2) {
    double dx = std::abs(p1.x - p2.x);
    double dy = std::abs(p1.y - p2.y);
    double dz = std::abs(p1.z - p2.z);

    // Manual sort of dx, dy, dz to find max, mid, min
    double maxD = dx, midD = dy, minD = dz;

    if (maxD < midD) std::swap(maxD, midD);
    if (midD < minD) std::swap(midD, minD);
    if (maxD < midD) std::swap(maxD, midD);

    return 0.939 * maxD + 0.389 * midD + 0.298 * minD;
}

const char* GetDistanceAsConstChar(const Point& p1, const Point& p2)
{
	string distanceStr = std::format("{:.2f}", GetDistance(p1, p2));
	const char* result = distanceStr.c_str();
    return result;
}

float fastSqrt(float number) // copied off of wikipedia, based on Quake 3 fast inverse square root
{
    long i;
    float x2, y;
    const float threehalfs = 1.5F;

    x2 = number * 0.5F;
    y = number;
    i = *(long*)&y;                       // evil floating point bit level hacking
    i = 0x5f3759df - (i >> 1);               // what the fuck?
    y = *(float*)&i;
    y = y * (threehalfs - (x2 * y * y));   // 1st iteration
    //	y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

    return y;
}

void LogMessage(const char* message) {
    OutputDebugStringA(message);
}

void DrawBoldPoint(HDC& hdc, const Point& p, int boldness, COLORREF color) {
    if (!PositionIsLegal(p)) return;

    HBRUSH brush = CreateSolidBrush(color);
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush);

    // Draw a filled square centered at (x, y)
    Rectangle(hdc, p.x - boldness, p.y - boldness, p.x + boldness + 1, p.y + boldness + 1);

    SelectObject(hdc, oldBrush);
    DeleteObject(brush);


    int textOffsetX = boldness + 4;
    int textOffsetY = -boldness - 4;


    const char* coords = p.CoordStr.c_str();
    TextOutA(hdc, p.x + textOffsetX, p.y + textOffsetY, coords, strlen(coords));
}

void DrawTextAtMiddle(HDC hdc, Line line, const char* text, COLORREF color) {
    SetTextColor(hdc, color);
    SetBkMode(hdc, TRANSPARENT); 

    SIZE textSize;
    GetTextExtentPoint32A(hdc, text, strlen(text), &textSize);

    int textX = line.Middle.x - (textSize.cx / 2);
    int textY = line.Middle.y - textSize.cy - 5;

    TextOutA(hdc, textX, textY, text, strlen(text));
}


void DrawLine(HDC& hdc, const Line& line, const int boldness, const COLORREF& color) {
    if (!PositionIsLegal(line.p1) || !PositionIsLegal(line.p2)) return;

    HPEN pen = CreatePen(PS_SOLID, boldness, color);
    HPEN oldPen = (HPEN)SelectObject(hdc, pen);

    MoveToEx(hdc, line.p1.x, line.p1.y, nullptr);
    LineTo(hdc, line.p2.x, line.p2.y);

    SelectObject(hdc, oldPen);
    DeleteObject(pen);

    DrawTextAtMiddle(hdc, line, line.lengthStr.c_str(), color);
}

Point MultiplyMatrixByPoint(const double matrix[3][3], const Point& p) {
	if (!matrix) return p; // if the matrix is null, return the point as is
    Point result;
    result.x = (int)(matrix[0][0] * p.x + matrix[0][1] * p.y + matrix[0][2] * p.z);
    result.y = (int)(matrix[1][0] * p.x + matrix[1][1] * p.y + matrix[1][2] * p.z);
    result.z = (int)(matrix[2][0] * p.x + matrix[2][1] * p.y + matrix[2][2] * p.z);
	result.color = p.color; // Keep the color of the original point
    return result;
}

Point RotatePointAround(Point p, Point pivot, double matrix[3][3]) {
	p -= pivot; // Translate point to origin
    Point result = MultiplyMatrixByPoint(matrix, p);
	result += pivot; // Translate point back
    return result;
}


bool PositionIsLegal(Point p) {
	return (p.x >= 0 && p.x < screenWidth && p.y >= 0 && p.y < screenHeight);
}



void OnLeftMouseDown(HWND hwnd) {
    POINT cursPos;
    GetCursorPos(&cursPos);
    ScreenToClient(hwnd, &cursPos);
    Point toAdd = { cursPos.x, cursPos.y };
    if(!PositionIsLegal(toAdd)) return;
	PointsToDraw->insert(toAdd);
}




void OnLeftMouseHold(HWND hwnd, HDC hdc) {
	POINT cursPos;
	GetCursorPos(&cursPos);
	ScreenToClient(hwnd, &cursPos);
	LastCursPos = { cursPos.x, cursPos.y };
	if (!PositionIsLegal(LastCursPos)) return;
    DrawLine(hdc, Line({ LastCursPos, PointsToDraw->LastDrawn }), LineBoldness, lastColorFromInputs);

}

             
void OnLeftMouseUp(HWND hwnd) {
	Point intersectionChecker = PointsToDraw->CheckIntersection(LastCursPos);
    Point lastPointDrawn = PointsToDraw->LastDrawn;
    if (!PositionIsLegal(intersectionChecker)) return;
    if(lastPointDrawn != intersectionChecker) LinesToDraw->addLine(lastPointDrawn, intersectionChecker);
    PointsToDraw->insert(LastCursPos);
}

void RemovePoint(Point p) {
    PointsToDraw->remove(p);
	LinesToDraw->removeLinesWithPoint(p);
}

// Initialize the PointManager and LineManager objects
void InitializeManagers() {
    PointsToDraw = new PointManager(drawingCapacity, 2* PointBoldness);
    LinesToDraw = new LineManager();
}


