#ifndef __HN_WaterMargin__RollImage__
#define __HN_WaterMargin__RollImage__

#include "cocos2d.h"
#include "HNUIExport.h"
#include <string>

using namespace std;
using namespace HN;

USING_NS_CC;

namespace WaterMargin
{

	class RollImage : public HNSprite
	{
		char name[30];
	public:
		virtual bool init();
		static RollImage* create(string name);
		//¾«Áé²ÎÊý
		CC_SYNTHESIZE(bool, m_run, Run);

	public:
		//Í¼Æ¬¹ö¶¯
		void imageRun();
		//Í¼Æ¬Í£Ö¹²¢Ìæ»»frame
		void imageStopAndSetFrame(string name);

		void resetReadyImage();
	};
}

#endif