#include <windows.h>
#include <stdbool.h>
#include <math.h>

bool quit = false;
int PointBoldness = 10;
int LineBoldness = 1;
double angle = 0;

struct Point {
    int x = 0;
    int y = 0;
    int z = 0;
};

LRESULT CALLBACK WindowProcessMessage(HWND, UINT, WPARAM, LPARAM);
void DrawBoldPoint(HDC hdc, int x, int y, int boldness);
void DrawLine(HDC hdc, Point p1, Point p2, int boldness);
Point MultiplyMatrixByPoint(double matrix[3][3], Point p);
Point RotatePointAround(Point p, Point pivot, double matrix[3][3]);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow) {
    WNDCLASS window_class = { 0 };
    const wchar_t window_class_name[] = L"MyWindowClass";
    window_class.lpszClassName = window_class_name;
    window_class.lpfnWndProc = WindowProcessMessage;
    window_class.hInstance = hInstance;
    window_class.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // Set background color
    window_class.style = CS_HREDRAW | CS_VREDRAW;

    RegisterClass(&window_class);

    HWND window_handle = CreateWindow(window_class_name, L"My Renderer", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, NULL);
    if (window_handle == NULL) {
        return -1;
    }

    ShowWindow(window_handle, nCmdShow);
    UpdateWindow(window_handle);

    Point Middle = { 400, 300 };
    Point points[4] =
    {
        { Middle.x - 100, Middle.y - 100 },
        { Middle.x + 100, Middle.y - 100 },
        { Middle.x + 100, Middle.y + 100 },
        { Middle.x - 100, Middle.y + 100 }
    };

    // Main loop
    MSG message;
    while (!quit) {
        while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&message);
            DispatchMessage(&message);
        }

        HDC hdc = GetDC(window_handle);

        // Create a compatible memory device context and bitmap for double buffering
        HDC memDC = CreateCompatibleDC(hdc);
        HBITMAP memBitmap = CreateCompatibleBitmap(hdc, 800, 600);
        SelectObject(memDC, memBitmap);

        // Fill the background with black color
        HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
        RECT rect = { 0, 0, 800, 600 };
        FillRect(memDC, &rect, blackBrush);
        DeleteObject(blackBrush);

        // Rotation matrices
        double RotateX[3][3] = {
            {1, 0, 0},
            {0, cos(angle), -sin(angle)},
            {0, sin(angle), cos(angle)}
        };

        double RotateY[3][3] = {
            {cos(angle), 0, sin(angle)},
            {0, 1, 0},
            {-sin(angle), 0, cos(angle)}
        };

        double RotateZ[3][3] = {
            {cos(angle), -sin(angle), 0},
            {sin(angle), cos(angle), 0},
            {0, 0, 1}
        };

        for (int i = 0; i < 4; i++) {
            Point rotatedPoint = RotatePointAround(points[i], Middle, RotateZ);

            DrawBoldPoint(memDC, rotatedPoint.x, rotatedPoint.y, PointBoldness);
            DrawLine(memDC, rotatedPoint, RotatePointAround(points[(i + 1) % 4], Middle, RotateZ), LineBoldness);
        }

        // Copy the off-screen buffer to the screen
        BitBlt(hdc, 0, 0, 800, 600, memDC, 0, 0, SRCCOPY);

        // Clean up
        DeleteObject(memBitmap);
        DeleteDC(memDC);
        ReleaseDC(window_handle, hdc);

        angle += 0.05;
        Sleep(2);
    }

    return 0;
}

LRESULT CALLBACK WindowProcessMessage(HWND window_handle, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_ERASEBKGND: {
        // Set the background color to black
        HDC hdc = (HDC)wParam;
        RECT rect;
        GetClientRect(window_handle, &rect);
        HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0)); // Black background
        FillRect(hdc, &rect, brush);
        DeleteObject(brush); // Clean up the brush
        return 1; // Prevent default background erase
    }
    case WM_CLOSE: // When the window is closed
    case WM_DESTROY:
        quit = true;
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(window_handle, message, wParam, lParam);
    }
}

void DrawBoldPoint(HDC hdc, int x, int y, int boldness) {
    // Loop to draw multiple points around (x, y) for the boldness effect
    for (int i = -boldness; i <= boldness; ++i) {
        for (int j = -boldness; j <= boldness; ++j) {
            // Draw a point at (x + i, y + j)
            SetPixel(hdc, x + i, y + j, RGB(255, 255, 255)); // White color for the point
        }
    }
}

// Function to draw a bold line using Bresenham's Line Algorithm
void DrawLine(HDC hdc, Point p1, Point p2, int boldness) {
    int dx = abs(p2.x - p1.x);
    int dy = abs(p2.y - p1.y);
    int sx = (p1.x < p2.x) ? 1 : -1;
    int sy = (p1.y < p2.y) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        // Draw the main line pixel with boldness
        DrawBoldPoint(hdc, p1.x, p1.y, boldness);

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
    result.x = (int)(matrix[0][0] * p.x + matrix[0][1] * p.y + matrix[0][2] * 1);
    result.y = (int)(matrix[1][0] * p.x + matrix[1][1] * p.y + matrix[1][2] * 1);
    result.z = (int)(matrix[2][0] * p.x + matrix[2][1] * p.y + matrix[2][2] * 1);
    return result;
}

Point RotatePointAround(Point p, Point pivot, double matrix[3][3]) {
    Point result;

    // Translate point to origin
    int x = p.x - pivot.x;
    int y = p.y - pivot.y;
    int z = p.z - pivot.z; // Assuming 2D rotation, Z remains the same

    // Apply rotation matrix
    result.x = (int)(matrix[0][0] * x + matrix[0][1] * y + matrix[0][2] * z);
    result.y = (int)(matrix[1][0] * x + matrix[1][1] * y + matrix[1][2] * z);
    result.z = (int)(matrix[2][0] * x + matrix[2][1] * y + matrix[2][2] * z);

    // Translate point back
    result.x += pivot.x;
    result.y += pivot.y;
    result.z += pivot.z;

    return result;
}
