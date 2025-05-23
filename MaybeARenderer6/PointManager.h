#pragma once
#include <windows.h>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <vector>
#include <algorithm>
#include <optional>

#include "MyClasses.h"


// PointManager Class
class PointManager : public IDrawEachFrame, public IFlushable {
private:
    std::unordered_map<Point, std::vector<Point>, PointHash> buckets;
    std::unordered_set<Point, PointHash> points;
    std::queue<Point> insertionOrder;
    int _capacity;
    int _c;

public:
    Point LastDrawn = { 0, 0, 0 };

    // Constructor
    PointManager(int cap, int c);

    Point ToBucket(Point p);
    Point insert(Point p);
    Point remove(Point p);
	std::pair<Point, Point> MovePoint(Point old, Point pos);
    std::optional<Point> CheckIntersection(Point p);

    void DrawPoints(HDC targetHDC);
    void EachFrame(HDC& targetHDC) override;
    void Flush() override;
};
