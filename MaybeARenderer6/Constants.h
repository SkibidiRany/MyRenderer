#pragma once
#include <windows.h>
#include <vector>
#include <string>

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
extern const char FlushButton;
inline extern constexpr UINT DRAW_POINT_DOWN = WM_LBUTTONDOWN;
inline extern constexpr UINT DRAW_POINT_UP = WM_LBUTTONUP;
inline extern constexpr UINT ROTATE_SHAPE_DOWN = WM_RBUTTONDOWN;
inline extern constexpr UINT ROTATE_SHAPE_UP = WM_RBUTTONUP;
inline extern constexpr UINT MOVE_POINT_DOWN = WM_MBUTTONDOWN;
inline extern constexpr UINT MOVE_POINT_UP = WM_MBUTTONUP;

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

extern std::vector<std::wstring> ShapeNames;

extern Drawings WantedDrawing;