#pragma once
#include <stdexcept>
#include <windows.h>
using std::vector;

COLORREF RED = RGB(255, 0, 0);
COLORREF GREEN = RGB(0, 255, 0);
COLORREF BLUE = RGB(0, 0, 255);
COLORREF WHITE = RGB(255, 255, 255);



int screenWidth = 800;
int screenHeight = 600;

float PI = 3.14159265358979323846;
bool quit = false;
int PointBoldness = 10;
int LineBoldness = 1;
double angle = 0;
double angleChangeSpeed = 0.01;
bool ToDrawShape = false;

int drawingCapacity = 20;



struct Point {
    int x = 0;
    int y = 0;
    int z = 0;
};

enum Drawings {
    Rect,
    Pyramid,
	DoublePyramid,
	PolygonWith2Heads
};


void DrawBoldPoint(HDC hdc, int x, int y, int boldness, COLORREF color);
void DrawLine(HDC hdc, Point p1, Point p2, int boldness, COLORREF color);
Point MultiplyMatrixByPoint(double matrix[3][3], Point p);
Point RotatePointAround(Point p, Point pivot, double matrix[3][3]);
void OnLeftMouseClick();

// PointNode structure
struct PointNode {
    Point data;
    PointNode* next;
    PointNode(Point value) : data(value), next(nullptr) {}
};

// Linked List class
class PointLinkedList {
private:
    PointNode* head;
    PointNode* tail;
    int size;
    int capacity;
    COLORREF drawingColor = WHITE;

public:
    PointLinkedList(int n) : head(nullptr), tail(nullptr), size(0), capacity(n) {}

    // Insert with fixed buffer size
    void insert(Point value) {
        PointNode* newNode = new PointNode(value);
        if (!head) {
            head = tail = newNode;
        }
        else {
            tail->next = newNode;
            tail = newNode;
        }
        size++;

        if (size > capacity) {
            PointNode* temp = head;
            head = head->next;
            delete temp;
            size--;
        }
    }

    void DrawPoints(HDC hdc) {
        PointNode* node = head;
        while (node != nullptr) {
            DrawBoldPoint(hdc, node->data.x, node->data.y, PointBoldness, drawingColor);
            node = node->next;
        }
    }

    // Destructor to free memory
    ~PointLinkedList() {
        PointNode* temp;
        while (head) {
            temp = head;
            head = head->next;
            delete temp;
        }
    }
};





PointLinkedList PointsToDraw(drawingCapacity);


Drawings WantedDrawing = PolygonWith2Heads;





Point Middle = { screenWidth / 2, screenHeight / 2 };
Point pivot = { Middle.x , Middle.y, Middle.z };





void DrawBoldPoint(HDC hdc, int x, int y, int boldness, COLORREF color = WHITE) {
    for (int i = -boldness; i <= boldness; ++i) {
        for (int j = -boldness; j <= boldness; ++j) {
            SetPixel(hdc, x + i, y + j, color);
        }
    }
}

// Function to draw a bold line using Bresenham's Line Algorithm
void DrawLine(HDC hdc, Point p1, Point p2, int boldness, COLORREF color = WHITE) {
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

// Function to multiply a 3x3 matrix by a Point
Point MultiplyMatrixByPoint(double matrix[3][3], Point p) {
    Point result;
    result.x = (int)(matrix[0][0] * p.x + matrix[0][1] * p.y + matrix[0][2] * p.z);
    result.y = (int)(matrix[1][0] * p.x + matrix[1][1] * p.y + matrix[1][2] * p.z);
    result.z = (int)(matrix[2][0] * p.x + matrix[2][1] * p.y + matrix[2][2] * p.z);
    return result;
}

Point RotatePointAround(Point p, Point pivot, double matrix[3][3]) {
    Point result;

    // Translate point to origin
    p.x -= pivot.x;
    p.y -= pivot.y;
    p.z -= pivot.z;

    // Apply rotation matrix
    result = MultiplyMatrixByPoint(matrix, p);

    // Translate point back
    result.x += pivot.x;
    result.y += pivot.y;
    result.z += pivot.z;

    return result;
}


void OnLeftMouseClick() {
    POINT cursPos;
	if (!GetCursorPos(&cursPos)) throw std::runtime_error("Failed to get cursor position");
	Point toAdd = { cursPos.x, cursPos.y };
	PointsToDraw.insert(toAdd);
}


