#pragma once
#include <windows.h>
#include "MyClasses.h"

// Extern declarations for global variables
extern HWND window_handle;
extern HWND rgb_window_handle;

// value constants
extern const float PI;

// Drawing settings
extern const bool DrawLength;
extern const bool DrawCoords;
extern const bool ToDrawShape;
extern const int drawingCapacity;

// Drawing settings
extern const int PointBoldness;
extern const int LineBoldness;

// screen settings
extern const int screenWidth;
extern const int screenHeight;

// Application settings
extern bool quit;

// input settings
extern char FlushButton;

// Shape settings
extern double angle;
extern double angleChangeSpeed;
extern const bool AutoAngleChangeSpeed;
extern const bool AutoRotate;

// global variables
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