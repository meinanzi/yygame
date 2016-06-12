#include "TRACTORDrawEllipse.h"

namespace TRACTOR
{
	DrawEllipse::DrawEllipse()
	{
		o = Vec2(0, 0);
		x = 2;
		y = 1;
	}


	DrawEllipse::~DrawEllipse()
	{
	}


	DrawEllipse::DrawEllipse(Vec2 oValue, float xValue, float yValue)
	{
		o = oValue;
		x = xValue;
		y = yValue;
	}

	bool DrawEllipse::setEllipse(Vec2 oValue, float xValue, float yValue)
	{
		if (xValue < 0 || yValue < 0)
		{
			return false;
		}

		o = oValue;
		x = xValue;
		y = yValue;

		return true;
	}

	bool DrawEllipse::isOnEllipse(Vec2 point)
	{
		return checkEllipse(point) == 0 ? true : false;
	}
	bool DrawEllipse::isInEllipse(Vec2 point)
	{
		return checkEllipse(point) < 0 ? true : false;
	}
	bool DrawEllipse::isOutEllipse(Vec2 point)
	{
		return checkEllipse(point) > 0 ? true : false;
	}

	float DrawEllipse::checkEllipse(Vec2 point)
	{
		float a = point.x - o.x;
		float b = point.y - o.y;

		return (a*a) / (x*x) + (b*b) / (y*y) - 1;
	}

	Vec2 DrawEllipse::getEllipseOrigin()
	{
		return o;
	}
	float DrawEllipse::getEllipseX()
	{
		return x;
	}
	float DrawEllipse::getEllipseY()
	{
		return y;
	}
}