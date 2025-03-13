#include <windows.h>
#include <stdbool.h>


bool quit = false;
int PointBoldness = 10;
int LineBoldness = 1;

struct Point {
    int x=0;
    int y=0;
};



LRESULT CALLBACK WindowProcessMessage(HWND, UINT, WPARAM, LPARAM);
void DrawBoldPoint(HDC hdc, int x, int y, int boldness);
void DrawLine(HDC hdc, Point p1, Point p2, int boldness);

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


    Point Middle = {400,300};
    Point points[4] =
    {
        { Middle.x -100, Middle.y -100 },
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

        
        for (int i = 0; i < 4; i++) {
            DrawBoldPoint(hdc, points[i].x, points[i].y, PointBoldness);
        }
    
        DrawLine(hdc, points[0], points[1], LineBoldness);
        DrawLine(hdc, points[1], points[2], LineBoldness);
        DrawLine(hdc, points[2], points[3], LineBoldness);
        DrawLine(hdc, points[3], points[0], LineBoldness);

        //DrawBoldPoint(hdc, 400, 300, 10); // 5 is the boldness
        ReleaseDC(window_handle, hdc);
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