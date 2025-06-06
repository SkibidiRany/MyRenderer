#include "Lerp.h"
#include "DrawingFunctions.h"
#include "Constants.h"
#include <algorithm>

Lerp::Lerp(Point a, Point b, float dur) : a(a), b(b), Animation(dur)
{

}

void Lerp::DoAnimation(HDC hdc, float deltaTime)
{
    if (!IsRunning()) return;

    // Linearly interpolate between a and b
    Point current;
    current.x = a.x + progress * (b.x - a.x);
    current.y = a.y + progress * (b.y - a.y);

	DrawBoldPoint(hdc, a, PointBoldness, a.color); // Draw the start point
	DrawBoldPoint(hdc, b, PointBoldness, b.color); // Draw the end point

    DrawLine(hdc, { a, current }, LineBoldness, a.color);
    // Draw the current point
    DrawBoldPoint(hdc, current, PointBoldness, a.color);

}