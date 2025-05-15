#include "DrawingFunctions.h"
#include <string>
#include <format> 
#include <cstring>

#include "Constants.h"
#include "MyClasses.h"
#include "BaseFunctions.h"

void DrawBoldPoint(HDC& hdc, const Point& p, int boldness, COLORREF color) {
    if (!PositionIsLegal(p)) return;

    HBRUSH brush = CreateSolidBrush(color);
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush);

    // Draw a filled square centered at (x, y)
    Rectangle(hdc, p.x - boldness, p.y - boldness, p.x + boldness + 1, p.y + boldness + 1);

    SelectObject(hdc, oldBrush);
    DeleteObject(brush);

	if (!DrawCoords) return;

    int textOffsetX = boldness + 4;
    int textOffsetY = -boldness - 4;

    std::string formatted = std::format("({},{},{})", p.x, p.y, p.z);
    const char* coords = formatted.c_str();

    SetTextColor(hdc, color);
    SetBkMode(hdc, TRANSPARENT);

    TextOutA(hdc, p.x + textOffsetX, p.y + textOffsetY, coords, static_cast<int>(strlen(coords)));
}

void DrawTextAtMiddle(HDC& hdc, const Line& line, const char* text, COLORREF color) {
    SetTextColor(hdc, color);
    SetBkMode(hdc, TRANSPARENT);

    SIZE textSize;
    GetTextExtentPoint32A(hdc, text, static_cast<int>(strlen(text)), &textSize);

    int textX = line.Middle.x - (textSize.cx / 2);
    int textY = line.Middle.y - textSize.cy - 5;

    TextOutA(hdc, textX, textY, text, static_cast<int>(strlen(text)));
}

void DrawLine(HDC& hdc, const Line& line, int boldness, COLORREF color) {
    if (!PositionIsLegal(line.p1) || !PositionIsLegal(line.p2)) return;

    HPEN pen = CreatePen(PS_SOLID, boldness, color);
    HPEN oldPen = (HPEN)SelectObject(hdc, pen);

    MoveToEx(hdc, line.p1.x, line.p1.y, nullptr);
    LineTo(hdc, line.p2.x, line.p2.y);

    SelectObject(hdc, oldPen);
    DeleteObject(pen);

	if (!DrawLength) return;

    DrawTextAtMiddle(hdc, line, line.lengthStr.c_str(), color);
}
