#pragma once
#include <windows.h>

struct Point;
struct Line;



void DrawBoldPoint(HDC& hdc, const Point& p, int boldness, COLORREF color = RGB(255,255,255));
void DrawTextAtMiddle(HDC hdc, Line line, const char* text, COLORREF color = RGB(255, 255, 255));
void DrawLine(HDC& hdc, const Line& line, int boldness, COLORREF color = RGB(255, 255, 255));
