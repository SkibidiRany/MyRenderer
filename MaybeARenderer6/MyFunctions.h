#pragma once
#include <string>

// Forward declarations to avoid circular dependencies
extern class PointManager;
extern class LineManager;
extern struct Point;

void RemovePoint(Point p);
void FlushScreen(PointManager* pM, LineManager* lM);
