#pragma once
#include "PointManager.h"
#include "LineManager.h"

class PointLineManager : public PointManager, public LineManager, public IDrawEachFrame, public IFlushable
{
private:
	Point oldPoint;
	bool canMovePoint = false;
public:
	PointLineManager(int pointCapacity, int PointGridC);

	void EachFrame(HDC& targetHdc) override;
	void Flush() override;
	Point MovePointLines(Point old, Point pos);


	void OnMovePointDown(const Point& old);
	void OnMovePointUp();
	void OnMovePoint(const Point& oldPoint, const Point& newPoint);

};
