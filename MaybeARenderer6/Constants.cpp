#include "Constants.h"

// Define global variables
HWND window_handle;
HWND rgb_window_handle;


// value constants
const float PI = 3.14159265f;

// Drawing settings
const bool DrawLength = true;
const bool DrawCoords = true;
const bool ToDrawShape = true;
const int drawingCapacity = 60; //currently used but not implemented in PointManager


// Drawing settings
const int LineBoldness = 1;
const int PointBoldness = 10;


// screen settings
const int screenWidth = 800;
const int screenHeight = 600;


// Application settings
bool quit = false;

// input settings
const char FlushButton = 'R';




Point Middle = { screenWidth / 2, screenHeight / 2, 0 };
Point pivot = { Middle.x, Middle.y, Middle.z };
Point LastCursPos = { 0, 0, 0 };


std::vector<std::wstring> ShapeNames = {
    L"Rect",
    L"Pyramid",
    L"DoublePyramid",
    L"PolygonWith2Heads",
    L"Heart"
};

// Define WantedDrawing
Drawings WantedDrawing = Heart;