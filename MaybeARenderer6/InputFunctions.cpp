#include "InputFunctions.h"
#include "PointManager.h"
#include "LineManager.h"
#include "BaseFunctions.h"
#include "MyClasses.h"

void OnLeftMouseDown(HWND hwnd, PointManager* p) {
    Point toAdd = GetCursPos(hwnd);

    if (!PositionIsLegal(toAdd)) return;
    p->insert(toAdd);
}

Point OnLeftMouseHold(HWND hwnd, HDC hdc, PointManager* p, int boldness, COLORREF color) {
    Point LastCursPos = GetCursPos(hwnd);
    if (!PositionIsLegal(LastCursPos)) return { 0, 0, 0 };

    DrawLine(hdc, Line({ LastCursPos, p->LastDrawn }), boldness, color);
    return LastCursPos;
}

void OnLeftMouseUp(HWND hwnd, Point LastCursPos, PointManager* p, LineManager* l) {
    Point intersectionChecker = p->CheckIntersection(LastCursPos);
    Point lastPointDrawn = p->LastDrawn;

    if (!PositionIsLegal(intersectionChecker)) return;

    if (lastPointDrawn != intersectionChecker)
        l->addLine(lastPointDrawn, intersectionChecker);

    p->insert(LastCursPos);
}
