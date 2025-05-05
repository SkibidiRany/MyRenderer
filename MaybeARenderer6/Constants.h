#pragma once
#include <windows.h>
#include "MyClasses.h"

// Extern declarations for global variables
extern HWND window_handle;
extern HWND rgb_window_handle;

extern const int screenWidth;
extern const int screenHeight;
extern const float PI;
extern bool quit;
extern const int PointBoldness;
extern const int LineBoldness;

extern double angle;
extern double angleChangeSpeed;
extern const bool AutoAngleChangeSpeed;

extern const bool ToDrawShape;
extern const bool AutoRotate;

extern const int drawingCapacity;
extern char FlushButton;

extern Point Middle;
extern Point pivot;
extern Point LastCursPos;

// Enum for Drawings
enum Drawings {
    Rect,
    Pyramid,
    DoublePyramid,
    PolygonWith2Heads,
    Heart
};
extern Drawings WantedDrawing;