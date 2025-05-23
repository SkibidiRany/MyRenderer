#include "PointLineManager.h"
#include "BaseFunctions.h"
#include "Constants.h"

PointLineManager::PointLineManager(int pointCapacity, int PointGridC)
	: PointManager(pointCapacity, PointGridC), LineManager() {}

void PointLineManager::EachFrame(HDC& targetHdc) {
	PointManager::EachFrame(targetHdc);
	LineManager::EachFrame(targetHdc);
}


void PointLineManager::Flush() {
	PointManager::Flush();
	LineManager::Flush();
}

Point PointLineManager::MovePointLines(Point old, Point pos) {
	auto [oldPoint, inserted] = PointManager::MovePoint(old, pos);
	LineManager::MoveLinesWithPoint(oldPoint, inserted);
	return inserted;
}






void PointLineManager::OnMovePointDown(const Point& old) {
	auto found = CheckIntersection(old);
	if (found.has_value()) {
		oldPoint = found.value();
		canMovePoint = true;
	}
	else {
		canMovePoint = false;
	}
}

void PointLineManager::OnMovePointUp()
{
	if (!canMovePoint) return; // if not allowed to move, exit

    Point newP = GetCursPos(window_handle);
    OnMovePoint(oldPoint, newP);
}

void PointLineManager::OnMovePoint(const Point& oldP, const Point& newP) {

	MovePointLines(oldP, newP);

}