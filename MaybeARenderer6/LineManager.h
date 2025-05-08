#pragma once
#include <windows.h>
#include <unordered_set>
#include <unordered_map>

#include "MyClasses.h"
#include "DrawingFunctions.h"

// LineManager Class
class LineManager : public IDrawEachFrame, public IFlushable {
private:
    std::unordered_set<Line, LineHash> lines;
    std::unordered_map<Point, std::unordered_set<Line>, PointHash> pointToLinesMap;

    Point oldPoint;
public:
    LineManager();

    void addLine(const Point& P, const Point& Q);
    void removeLine(Point& P, Point& Q);
    void removeLinesWithPoint(const Point& P);

    std::vector<Line> getLinesWithPoint(const Point& p) const;

    void OnMovePointDown(const Point& old);
    void OnMovePointUp();
	void OnMovePoint(const Point& oldPoint, const Point& newPoint);


    void DrawLines(HDC& targetHDC);
    void EachFrame(HDC& targetHDC) override;
    void Flush() override;
};