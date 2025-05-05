#pragma once
#include <Windows.h>

// Forward declarations to avoid circular includes
class PointManager;
class LineManager;
struct Point;

void OnLeftMouseDown(HWND hwnd, PointManager* p);
Point OnLeftMouseHold(HWND hwnd, HDC hdc, PointManager* p, int boldness, COLORREF color);
void OnLeftMouseUp(HWND hwnd, Point LastCursPos, PointManager* p, LineManager* l);
