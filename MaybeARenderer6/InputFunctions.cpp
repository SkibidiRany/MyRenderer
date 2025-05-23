#include "InputFunctions.h"
#include "PointManager.h"
#include "LineManager.h"
#include "PointLineManager.h"
#include "BaseFunctions.h"
#include "MyClasses.h"
#include "InputFields.h"

void OnLeftMouseDown(HWND hwnd, PointLineManager* p) {
    Point toAdd = GetCursPos(hwnd);
    toAdd.color = lastColorFromInputs;

    if (!PositionIsLegal(toAdd)) return;
    p->insert(toAdd);
}

Point OnLeftMouseHold(HWND hwnd, HDC hdc, PointLineManager* p, int boldness, COLORREF color) {
    Point LastCursPos = GetCursPos(hwnd);
    if (!PositionIsLegal(LastCursPos)) return { 0, 0, 0 };

    DrawLine(hdc, Line({ LastCursPos, p->LastDrawn }), boldness, color);
    return LastCursPos;
}

void OnLeftMouseUp(HWND hwnd, Point LastCursPos, PointLineManager* p) {
    std::optional<Point> intersectionChecker = p->CheckIntersection(LastCursPos);
    Point intersection = intersectionChecker.value_or(LastCursPos);
    Point lastPointDrawn = p->LastDrawn;

    if (!PositionIsLegal(intersection)) return;

    if (lastPointDrawn != intersection)
        p->addLine(Line{ lastPointDrawn, intersection, lastColorFromInputs });

    LastCursPos.color = lastColorFromInputs;
    p->insert(LastCursPos);
}
