#include <windows.h>
#include <stdbool.h>
#include <math.h>
#include <vector>
#include <string>
#include <iostream>

#include "Shapes.h"
#include "Utilities.h"
#include "InputFunctions.h"
#include "PointLineManager.h"
#include "Animator.h"
#include "Time.h"
#include "Lerp.h"
#include "Tests.h"
#include "TestFunctions.h"


using std::vector;


Shape* myShape;

void InitializeManagers() {

    
    pointLineManager = new PointLineManager(drawingCapacity, 2 * PointBoldness);
    //PointsToDraw = new PointManager(drawingCapacity, 2 * PointBoldness);
    //LinesToDraw = new LineManager();

	inputFieldsManager = new InputFieldsManager();
}


void FlushScreen() {

	pointLineManager->Flush();
    //LinesToDraw->Flush();
    //PointsToDraw->Flush();

    shapeManager->Flush();
}


LRESULT CALLBACK WindowProcessMessage(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK RGBWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
HWND InitMainWindow(HINSTANCE hInstance, int nCmdShow);
HWND InitRGBControlWindow(HINSTANCE hInstance, int nCmdShow);





int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow) {

    window_handle = InitMainWindow(hInstance, nCmdShow);
    
	rgb_window_handle = InitRGBControlWindow(hInstance, nCmdShow);


    InitializeManagers();
    auto scheduledAnimator = new ScheduledAnimator();
    auto animator = new Animator(*scheduledAnimator);
    
    TickingManager tickingManager;
	

	tickingManager.Subscribe(scheduledAnimator);
	tickingManager.Subscribe(animator);

	auto timeManager = new Time(tickingManager);
    timeManager->Initialize();


    Tests::NewTest([animator]() {
        TestFunctions::LerpPressureTest(animator);
        });

	Tests::NewTest([animator]() {
		TestFunctions::AnimationAfterFunctionalityTest(animator);
		});
//	Tests::Run();



    // Main loop
    MSG message;
    while (!quit) {

        timeManager->Update();


        
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


        if (ToDrawShape){
            shapeManager->EachFrame(memDC);
        }


		if ((GetKeyState(VK_LBUTTON) & 0x100) != 0) {
			LastCursPos = OnLeftMouseHold(window_handle, memDC, pointLineManager, LineBoldness, lastColorFromInputs);
		}
        


        //PointsToDraw->EachFrame(memDC);
		//LinesToDraw->EachFrame(memDC);

		pointLineManager->EachFrame(memDC);

        // Animate and merge animation HDCs 
        HDC animDC = animator->CallAnimations(); // Returns final HDC of all animations

        // Merge animation content into your off-screen buffer (non-black only)
        TransparentBlt(
            memDC, 0, 0, screenWidth, screenHeight,
            animDC, 0, 0, screenWidth, screenHeight,
            RGB(0, 0, 0) // Treat black as transparent
        );

        // Proper cleanup
        HBITMAP oldBitmap = (HBITMAP)SelectObject(animDC, GetStockObject(BLACK_BRUSH));
        DeleteObject(oldBitmap);
        DeleteDC(animDC);

        // Copy the off-screen buffer to the screen
        BitBlt(hdc, 0, 0, screenWidth, screenHeight, memDC, 0, 0, SRCCOPY);
        // Clean up
        DeleteObject(memBitmap);
        DeleteDC(memDC);
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
    case DRAW_POINT_DOWN:
        OnLeftMouseDown(window_handle, pointLineManager);
        break;
    case DRAW_POINT_UP:
		OnLeftMouseUp(window_handle, LastCursPos, pointLineManager);
        break;
    case ROTATE_SHAPE_DOWN:
		OnRightMouseDown(window_handle);
		break;
    case ROTATE_SHAPE_UP:
		OnRightMouseUp();
		break;
    case MOVE_POINT_DOWN:
		pointLineManager ->OnMovePointDown(GetCursPos(window_handle));
        break;
	case MOVE_POINT_UP:
		pointLineManager->OnMovePointUp();
        break;
    case WM_KEYDOWN:
        if (wParam == FlushButton) {
            FlushScreen();
        }
        else if (wParam == 'M') {
            pointLineManager->OnMovePointDown(GetCursPos(window_handle));
        } 
        else if (wParam == 'T') {
            Tests::Run();
        }
        break;
    case WM_KEYUP:
		if (wParam == 'M') {
            pointLineManager->OnMovePointUp();
		}
        break;
    default:
        return DefWindowProc(window_handle, message, wParam, lParam);
    }
	return 0;
}

LRESULT CALLBACK RGBWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_DESTROY:
        return 0;
    case WM_DRAWITEM:
    {
        LPDRAWITEMSTRUCT pDIS = (LPDRAWITEMSTRUCT)lParam;

        // Check if this is our color preview box
        if (pDIS->hwndItem == InputFieldsManager::g_colorPreviewBox) {
            // Get the current RGB using your existing function
            COLORREF color = GetColorFromInputs(rgb_window_handle); // Use your function here

            // Create a brush with the RGB color
            HBRUSH hBrush = CreateSolidBrush(color);

            // Fill the rectangle with the brush
            FillRect(pDIS->hDC, &pDIS->rcItem, hBrush);

            // Draw a border around the color box
            FrameRect(pDIS->hDC, &pDIS->rcItem, (HBRUSH)GetStockObject(BLACK_BRUSH));

            // Delete the brush (important to avoid memory leaks)
            DeleteObject(hBrush);

            return TRUE;
        }
        break;
    }
    case WM_COMMAND:
    {
        int ctrlID = LOWORD(wParam);    // The control that generated the command
        int notifCode = HIWORD(wParam);  // The notification code (e.g., EN_CHANGE)

        if (notifCode == EN_CHANGE) {  // EN_CHANGE is sent when the content of a control changes
            if (ctrlID == AngleInputField.id) {
                InputFieldsManager::g_angleChanged = true;  // Set the dirty flag for angle input
            }
            else if (ctrlID == AngleChangeSpeedInputField.id) {
                InputFieldsManager::g_angleSpeedChanged = true;  // Set the dirty flag for angle change speed input
            }
            else if (ctrlID == PointColorInputField.fields[REDi].id ||
                ctrlID == PointColorInputField.fields[GREENi].id ||
                ctrlID == PointColorInputField.fields[BLUEi].id) {
                // Invalidate the color preview box when any of the color fields change
                InvalidateRect(InputFieldsManager::g_colorPreviewBox, NULL, TRUE);
            }
        }
        else if (notifCode == CBN_SELCHANGE) {
            if (ctrlID == 106) {// TODO #Define magic numbers
                Drawings selected = GetSelectedDrawing(rgb_window_handle);
                shapeManager->SetShape(GetWantedDrawing(selected));
            }
        }
        break;
    }
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

HWND InitMainWindow(HINSTANCE hInstance, int nCmdShow)
{

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
        exit(1);
    }

    ShowWindow(window_handle, nCmdShow);
    UpdateWindow(window_handle);
	return window_handle;
}

HWND InitRGBControlWindow(HINSTANCE hInstance, int nCmdShow)
{
    // Register RGB control window
    WNDCLASS rgb_class = { 0 };
    const wchar_t rgb_class_name[] = L"RGBControlClass";
    rgb_class.lpszClassName = rgb_class_name;
    rgb_class.lpfnWndProc = RGBWindowProc;
    rgb_class.hInstance = hInstance;
    rgb_class.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClass(&rgb_class);
    
    int windowX = 800;
    int windowY = 200;

    HWND rgb_window = CreateWindow(rgb_class_name, L"RGB Controls", WS_OVERLAPPEDWINDOW,
        windowX, windowY, 500, 500, NULL, NULL, hInstance, NULL);
    ShowWindow(rgb_window, nCmdShow);
    UpdateWindow(rgb_window);


	inputFieldsManager->initializeInputFields(rgb_window);
    
    return rgb_window;
}



