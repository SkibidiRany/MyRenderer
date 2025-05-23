#pragma once
#include <Windows.h>
#include "PointLineManager.h"

// Forward declarations to avoid circular includes
class PointManager;
class LineManager;
struct Point;

void OnLeftMouseDown(HWND hwnd, PointLineManager* p);

Point OnLeftMouseHold(HWND hwnd, HDC hdc, PointLineManager* p, int boldness, COLORREF color);
void OnLeftMouseUp(HWND hwnd, Point LastCursPos, PointLineManager* p);