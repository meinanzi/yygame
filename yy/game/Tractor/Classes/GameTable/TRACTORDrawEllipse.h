#ifndef __TRACTORDrawEllipse_h__
#define __TRACTORDrawEllipse_h__

#include "cocos2d.h"
USING_NS_CC;

namespace TRACTOR
{
	class DrawEllipse
	{
	public:
		DrawEllipse(Vec2 oValue, float xValue, float yValue);
		bool setEllipse(Vec2 oValue, float xValue, float yValue);

		bool isOnEllipse(Vec2 point);
		bool isInEllipse(Vec2 point);
		bool isOutEllipse(Vec2 point);

		float checkEllipse(Vec2 point);

		Vec2 getEllipseOrigin();
		float getEllipseX();
		float getEllipseY();

		DrawEllipse();
		~DrawEllipse();


	private:
		// the radius in x-axis
		float x;

		// the radius in y-axis
		float y;

		// the origin of the ellipse
		Vec2 o;
	};

}
#endif // __TRACTORDrawEllipse_h__


