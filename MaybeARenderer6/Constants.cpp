#include "Constants.h"

// Define global variables
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
const bool AutoAngleChangeSpeed = false;

const bool ToDrawShape = true;
const bool AutoRotate = true;

const int drawingCapacity = 60;
char FlushButton = 'R';

Point Middle = { screenWidth / 2, screenHeight / 2, 0 };
Point pivot = { Middle.x, Middle.y, Middle.z };
Point LastCursPos = { 0, 0, 0 };

// Define WantedDrawing
Drawings WantedDrawing = Heart;