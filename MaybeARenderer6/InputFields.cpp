#include "InputFields.h"
#include <algorithm>



bool InputFieldsManager::g_angleChanged = false;
bool InputFieldsManager::g_angleSpeedChanged = false;

HWND InputFieldsManager::g_colorPreviewBox = NULL;


InputField  AngleInput(104, L"0", L"Angle:"),
			AngleChangeSpeedInput(105, L"0", L"Angle Change Speed:");

ColorInputField PointColorInput(101, L"255", L"Point: ");


template <typename T>
T Clamp(T val, T minVal, T maxVal) {
    return std::max(minVal, std::min(val, maxVal));
}

COLORREF GetColorFromInputs(HWND rgb_window ) {
    
    if (!rgb_window) {
        return RGB(255, 255, 255);
    }
    BOOL success;
    int r = GetDlgItemInt(rgb_window, PointColorInput.fields[REDi].id, &success, FALSE);
    if (!success) r = 255;

    int g = GetDlgItemInt(rgb_window, PointColorInput.fields[GREENi].id, &success, FALSE);
    if (!success) g = 255;

    int b = GetDlgItemInt(rgb_window, PointColorInput.fields[BLUEi].id, &success, FALSE);
    if (!success) b = 255;

    // Clamp values between 0 and 255
    r = Clamp(r,0,255);
    g = Clamp(g, 0, 255);
    b = Clamp(b, 0, 255);

    return RGB(r, g, b);
}


int GetAngleFromInputs(HWND rgb_window ) {
    if (!rgb_window) {
        return 0;
    }
    BOOL success;
    int angle = GetDlgItemInt(rgb_window, AngleInput.id, &success, FALSE);
    if (!success) angle = 0;

    // Clamp values between 0 and 360
    angle = Clamp(angle, 0, 360);

    return angle;
}



double GetAngleChangeSpeedFromInputs(HWND rgb_window ) {
    if (!rgb_window) {
        return 0;
    }
    BOOL success;
    double MaxChangeSpeed = 0.2;
    int rawValue = GetDlgItemInt(rgb_window, AngleChangeSpeedInput.id, &success, FALSE);
    if (!success) rawValue = 0;

    double angleChangeSpeed = rawValue / 100.0; // Convert to percentage

    // Clamp it between 0 and MaxChangeSpeed
    angleChangeSpeed = Clamp(angleChangeSpeed, 0.0, MaxChangeSpeed);

    return angleChangeSpeed;
}










std::vector<InputField> InputFieldsManager::inputFields = {
    PointColorInput.fields[REDi], PointColorInput.fields[GREENi], PointColorInput.fields[BLUEi], AngleInput, AngleChangeSpeedInput
};

int InputFieldsManager::InputTextWidth = 150;
int InputFieldsManager::InputTextHeight = 45;
int InputFieldsManager::InputTextX = 10;
int InputFieldsManager::InputHeightOffset = 10;
int InputFieldsManager::InputWidthOffset = 10;

int InputFieldsManager::InputFieldX = InputFieldsManager::InputTextX +
                                    InputFieldsManager::InputTextWidth +
                                    InputFieldsManager::InputWidthOffset;
int InputFieldsManager::InputFieldWidth = 60;
int InputFieldsManager::InputFieldHeight = InputFieldsManager::InputTextHeight;

int InputFieldsManager::currentY = InputFieldsManager::InputHeightOffset;

void InputFieldsManager::AddInputField(InputField field) {
    inputFields.push_back(field);
}

std::vector<InputField> InputFieldsManager::GetInputFields() {
    return inputFields;
}

void InputFieldsManager::initializeInputFields(HWND rgb_window) {
    for (const auto& field : inputFields) {
        CreateWindowW(L"STATIC", field.text, WS_VISIBLE | WS_CHILD,
            InputTextX, currentY, InputTextWidth, InputTextHeight,
            rgb_window, NULL, NULL, NULL);

        HWND editField = CreateWindowW(L"EDIT", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER,
            InputFieldX, currentY, InputFieldWidth, InputFieldHeight,
            rgb_window, (HMENU)field.id, NULL, NULL);

        SetWindowTextW(editField, field.defaultVal);

        //  If this is the Green field, place the preview box next to it
        if (field.id == PointColorInput.fields[GREENi].id) {
            int previewBoxX = InputFieldX + InputFieldWidth + 10;
            int boxSize = InputTextHeight;

            InputFieldsManager::g_colorPreviewBox = CreateWindowW(L"STATIC", NULL,
                WS_VISIBLE | WS_CHILD | SS_OWNERDRAW,
                previewBoxX, currentY, boxSize, boxSize,
                rgb_window, NULL, NULL, NULL);
            if (!InputFieldsManager::g_colorPreviewBox) {
                // Handle error - maybe log it
                OutputDebugString(L"Failed to create color preview box\n");
            }
        }

        currentY += InputTextHeight + InputHeightOffset;
    }
}

