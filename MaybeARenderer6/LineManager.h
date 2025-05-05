#pragma once
#include <windows.h>
#include <unordered_set>

#include "MyClasses.h"
#include "DrawingFunctions.h"

// LineManager Class
class LineManager : public IDrawEachFrame, public IFlushable {
private:
    std::unordered_set<Line, LineHash> lines;

public:
    LineManager();

    void addLine(Point& P, Point& Q);
    void removeLine(Point& P, Point& Q);
    void removeLinesWithPoint(Point& P);
    void DrawLines(HDC& targetHDC);
    void EachFrame(HDC& targetHDC) override;
    void Flush() override;
};