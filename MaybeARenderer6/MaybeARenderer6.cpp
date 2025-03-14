#include <windows.h>
#include <stdbool.h>
#include <math.h>

int screenWidth = 1400;
int screenHeight = 800;

bool quit = false;
int PointBoldness = 10;
int LineBoldness = 1;
double angle = 0;
double angleChangeSpeed = 0.01;




struct Rect3D{
    int width;
	int height;
	int depth;
};


struct Point {
    int x = 0;
    int y = 0;
    int z = 0;
};


Point Middle = { screenWidth / 2, screenHeight / 2 };
Point pivot = { Middle.x, Middle.y, Middle.z };

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
        CW_USEDEFAULT, CW_USEDEFAULT, screenWidth, screenHeight, NULL, NULL, hInstance, NULL);
    if (window_handle == NULL) {
        return -1;
    }

    ShowWindow(window_handle, nCmdShow);
    UpdateWindow(window_handle);

    Point Middle = { screenWidth / 2, screenHeight / 2 };

	Rect3D myRect = { 200, 200, 200 };
    Point points[8] =
    {
        { Middle.x - myRect.width / 2, Middle.y - myRect.height / 2, Middle.z + myRect.depth / 2 },
        { Middle.x + myRect.width / 2, Middle.y - myRect.height / 2, Middle.z + myRect.depth / 2 },
        { Middle.x + myRect.width / 2, Middle.y + myRect.height / 2, Middle.z + myRect.depth / 2 },
        { Middle.x - myRect.width / 2, Middle.y + myRect.height / 2, Middle.z + myRect.depth / 2 },
        { Middle.x - myRect.width / 2, Middle.y - myRect.height / 2, Middle.z - myRect.depth / 2 },
        { Middle.x + myRect.width / 2, Middle.y - myRect.height / 2, Middle.z - myRect.depth / 2  },
        { Middle.x + myRect.width / 2, Middle.y + myRect.height / 2, Middle.z - myRect.depth / 2  },
        { Middle.x - myRect.width / 2, Middle.y + myRect.height / 2, Middle.z - myRect.depth / 2  }
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
        HBITMAP memBitmap = CreateCompatibleBitmap(hdc, screenWidth, screenHeight);
        SelectObject(memDC, memBitmap);

        // Fill the background with black color
        HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
        RECT rect = { 0, 0, screenWidth, screenHeight};
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

        // Rotate all points
        Point rotatedPoints[8];
        for (int i = 0; i < 8; i++) {
            rotatedPoints[i] = RotatePointAround(points[i], pivot, RotateY);
            rotatedPoints[i] = RotatePointAround(rotatedPoints[i], pivot, RotateX);
            rotatedPoints[i] = RotatePointAround(rotatedPoints[i], pivot, RotateZ);
        }

        // Draw bold points
        for (int i = 0; i < 8; i++) {
            DrawBoldPoint(memDC, rotatedPoints[i].x, rotatedPoints[i].y, PointBoldness);
        }

        // Draw lines for the top face
        DrawLine(memDC, rotatedPoints[0], rotatedPoints[1], LineBoldness);
        DrawLine(memDC, rotatedPoints[1], rotatedPoints[2], LineBoldness);
        DrawLine(memDC, rotatedPoints[2], rotatedPoints[3], LineBoldness);
        DrawLine(memDC, rotatedPoints[3], rotatedPoints[0], LineBoldness);

        // Draw lines for the bottom face
        DrawLine(memDC, rotatedPoints[4], rotatedPoints[5], LineBoldness);
        DrawLine(memDC, rotatedPoints[5], rotatedPoints[6], LineBoldness);
        DrawLine(memDC, rotatedPoints[6], rotatedPoints[7], LineBoldness);
        DrawLine(memDC, rotatedPoints[7], rotatedPoints[4], LineBoldness);

        // Draw vertical lines connecting top and bottom faces
        DrawLine(memDC, rotatedPoints[0], rotatedPoints[4], LineBoldness);
        DrawLine(memDC, rotatedPoints[1], rotatedPoints[5], LineBoldness);
        DrawLine(memDC, rotatedPoints[2], rotatedPoints[6], LineBoldness);
        DrawLine(memDC, rotatedPoints[3], rotatedPoints[7], LineBoldness);

        // Copy the off-screen buffer to the screen
        BitBlt(hdc, 0, 0, screenWidth, screenHeight, memDC, 0, 0, SRCCOPY);

        // Clean up
        DeleteObject(memBitmap);
        DeleteDC(memDC);
        ReleaseDC(window_handle, hdc);

        angle += angleChangeSpeed;
        //Sleep(2);
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
    for (int i = -boldness; i <= boldness; ++i) {
        for (int j = -boldness; j <= boldness; ++j) {
            SetPixel(hdc, x + i, y + j, RGB(255, 255, 255)); 
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
    result.x = (int)(matrix[0][0] * p.x + matrix[0][1] * p.y + matrix[0][2] * p.z);
    result.y = (int)(matrix[1][0] * p.x + matrix[1][1] * p.y + matrix[1][2] * p.z);
    result.z = (int)(matrix[2][0] * p.x + matrix[2][1] * p.y + matrix[2][2] * p.z);
    return result;
}

Point RotatePointAround(Point p, Point pivot, double matrix[3][3]) {
    Point result;

    // Translate point to origin
    p.x -= pivot.x;
    p.y -= pivot.y;
    p.z -= pivot.z;

    // Apply rotation matrix
	result = MultiplyMatrixByPoint(matrix, p);

    // Translate point back
    result.x += pivot.x;
    result.y += pivot.y;
    result.z += pivot.z;

    return result;
}
