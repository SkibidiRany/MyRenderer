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

    elapsedTime += deltaTime;

    // Clamp t between 0 and 1
    float t = std::min(elapsedTime / duration, 1.0f);


    // Linearly interpolate between a and b
    Point current;
    current.x = a.x + t * (b.x - a.x);
    current.y = a.y + t * (b.y - a.y);

    // Draw the current point
    DrawBoldPoint(hdc, current, PointBoldness, a.color);

    // Stop animation if it's done
    if (t >= 1.0f)
        Stop();
}



