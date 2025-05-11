#pragma once
#define NOMINMAX  // Prevents windows.h from defining min and max macros (we use std::min and std::max)
#include <windows.h>
#include <algorithm>
#include <vector>
#include <string>

#include "Constants.h"

using std::vector;



constexpr int REDi = 0;
constexpr int GREENi = 1;
constexpr int BLUEi = 2;






struct InputField {
    const int id;
    const wchar_t* defaultVal; 
    const wchar_t* text; // Text to display to the side of the input field
    InputField(int id, const wchar_t* val, const wchar_t* txt) : id(id), defaultVal(val), text(txt) {}
};


struct ColorInputField {
    const InputField fields[3];
    const wchar_t* defaultVal; // Default value for the color input field
    const wchar_t* prefix; // Prefix for the color input field

    ColorInputField(int id, const wchar_t* val, const wchar_t* pre) : fields{
        InputField(id, val, L"Red:"),
        InputField(id + 1, val, L"Green"),
        InputField(id + 2, val, L"Blue:")

    }, defaultVal(val), prefix(pre) {}
};



struct DropdownField {
    const int id;
    const wchar_t* label;
    std::vector<std::wstring> items;

    DropdownField(int id, const wchar_t* label, const std::vector<std::wstring>& items)
        : id(id), label(label), items(items) {}
};



// Declare these for global use
extern InputField AngleInputField;
extern InputField AngleChangeSpeedInputField;

extern ColorInputField PointColorInputField;

extern COLORREF lastColorFromInputs;





COLORREF GetColorFromInputs(HWND rgb_window = NULL);

int GetAngleFromInputs(HWND rgb_window = NULL);


double GetAngleChangeSpeedFromInputs(HWND rgb_window = NULL);




class InputFieldsManager {
private:
    static std::vector<InputField> inputFields;
    static std::vector<DropdownField> dropdownFields; 

    static int InputTextWidth;
    static int InputTextHeight;
    static int InputTextX;
    static int InputHeightOffset;
    static int InputWidthOffset;

    static int InputFieldX;
    static int InputFieldWidth;
    static int InputFieldHeight;

    static int currentY;

    
public:
    static bool g_angleChanged;
    static bool g_angleSpeedChanged;

    static HWND g_colorPreviewBox;

    InputFieldsManager() = default;

    void AddInputField(InputField field);
    void AddDropdownField(DropdownField field); 

    std::vector<InputField> GetInputFields();
    std::vector<DropdownField> GetDropdownFields(); 

    void initializeInputFields(HWND rgb_window);
};