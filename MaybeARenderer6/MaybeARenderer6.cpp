#include <windows.h>
#include <stdbool.h>
#include <math.h>
#include <vector>
#include "Shapes.h"
#include "Utilities.h"
using std::vector;





LRESULT CALLBACK WindowProcessMessage(HWND, UINT, WPARAM, LPARAM);
Shape* GetWantedDrawing() {
	switch (WantedDrawing) {
	case Rect:
		return new Rect3D(200, 200, 200);
		break;
	case Pyramid:
		return new Pyramid3D(200);
		break;
	case DoublePyramid:
		return new DoublePyramid3D(200);
		break;
	case PolygonWith2Heads:
		return new PolygonWith2Heads3D(2, 100, 120);
		break;
	default:
		return NULL;
	}
}

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

 //   Rect3D myRect(200, 200, 200);
 //   Pyramid3D myPyr(200);
	//DoublePyramid3D myDoublePyr(200);
	//PolygonWith2Heads3D myPoly2Heads(7,100,120);

    Shape* myShape = GetWantedDrawing();
    

    if (myShape == NULL) return -1;


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
        RECT rect = { 0, 0, screenWidth, screenHeight };
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
        if (ToDrawShape) {
            myShape->DrawPoints(memDC);
            myShape->DrawLines(memDC);
            myShape->RotatePoints(RotateX, RotateY, RotateZ);
        }


		if ((GetKeyState(VK_LBUTTON) & 0x100) != 0) {
			OnLeftMouseHold(window_handle);
		}
        


        PointsToDraw.DrawPoints(memDC);
		LinesToDraw.DrawLines(memDC);
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
    case WM_LBUTTONDOWN:
        OnLeftMouseDown(window_handle);
        break;
    case WM_LBUTTONUP:
		OnLeftMouseUp(window_handle);
    default:
        return DefWindowProc(window_handle, message, wParam, lParam);
    }
}
