#pragma once

#include <windows.h>

#include "MyClasses.h"


struct Point;

// Function declarations
bool PositionIsLegal(Point p);
const char* GetDistanceAsConstChar(const Point& p1, const Point& p2);
float fastSqrt(float number);
double GetDistance(const Point& p1, const Point& p2);

Point GetCursPos(HWND hwnd);
void LogMessage(const char* message);
Point MultiplyMatrixByPoint(const double matrix[3][3], const Point& p);
Point RotatePointAround(Point p, Point piv, double matrix[3][3]);





