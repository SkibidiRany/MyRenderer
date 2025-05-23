#include "PointManager.h"
#include "Constants.h"
#include "InputFields.h"
#include "MyClasses.h"
#include "DrawingFunctions.h"

// Constructor
PointManager::PointManager(int cap, int c) : _capacity(cap), _c(c) {}

Point PointManager::ToBucket(Point p) {
    int bucketX = p.x / _c;
    int bucketY = p.y / _c;
    int bucketZ = p.z / _c;
    return { bucketX, bucketY };
}

Point PointManager::insert(Point p) {
    std::optional<Point> checker = CheckIntersection(p);

    if (checker.has_value()) {
        LastDrawn = checker.value();
        return checker.value(); // Point already exists nearby
    }

    if (points.size() >= _capacity) {
        Point oldest = insertionOrder.front();
        insertionOrder.pop();
        // Note: You may also want to erase `oldest` from `points` and `buckets`
    }

    points.insert(p);
    insertionOrder.push(p);
    buckets[ToBucket(p)].push_back(p);
    LastDrawn = p;
    return p;
}


Point PointManager::remove(Point p) {
	auto intersectionChecker = CheckIntersection(p);

	if (!intersectionChecker.has_value()) return p;
	Point intersection = intersectionChecker.value();

    points.erase(intersection);
    Point key = ToBucket(intersection);
    auto& vec = buckets[key];
    vec.erase(std::remove(vec.begin(), vec.end(), intersection), vec.end());

	return intersection;
}

std::pair<Point, Point> PointManager::MovePoint(Point old, Point pos)
{
	pos.color = old.color;

    old = remove(old);
	Point newPoint = insert(pos);

    return std::pair<Point, Point>(old, newPoint);

}

std::optional<Point> PointManager::CheckIntersection(Point p) {
    Point base = ToBucket(p);
    for (int dx = -1; dx <= 1; ++dx)
        for (int dy = -1; dy <= 1; ++dy)
            for (int dz = -1; dz <= 1; ++dz) {
                Point neighbor = { base.x + dx, base.y + dy, base.z + dz };
                auto it = buckets.find(neighbor);
                if (it != buckets.end()) {
                    for (const Point& q : it->second) {
                        if (std::abs(q.x - p.x) <= _c &&
                            std::abs(q.y - p.y) <= _c &&
                            std::abs(q.z - p.z) <= _c) {
                            return q;
                        }
                    }
                }
            }
    return std::nullopt;
}




void PointManager::DrawPoints(HDC targetHDC) {
    for (const auto& p : points) {
        DrawBoldPoint(targetHDC, p, PointBoldness, p.color);
    }
}

void PointManager::EachFrame(HDC& targetHDC) {
    DrawPoints(targetHDC);
}

void PointManager::Flush() {
    buckets.clear();
    points.clear();
    insertionOrder.empty(); // note: this does nothing; you probably meant insertionOrder = std::queue<Point>();
}
