#pragma once

#define NOMINMAX
#include "Animation.h"
#include "myCLasses.h"

class Lerp : public Animation
{
private:
	Point a = 0, b = 0;

public:
	Lerp(Point a, Point b, float dur);

	virtual void DoAnimation(HDC hdc, float deltaTime) override;



};

