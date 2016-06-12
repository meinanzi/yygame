#ifndef __WaterMargin_Animation_H__
#define __WaterMargin_Animation_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "HNNetExport.h"

USING_NS_CC;

using namespace cocostudio;
using namespace std;
using namespace ui;
using namespace HN;

namespace WaterMargin
{
	class GameAnimation
	{
	public:
		struct AniSpirteInfo
		{
			std::string name;
			int     	count;
			float	    time;
		};

		AniSpirteInfo _aniNameFirst[11];
		AniSpirteInfo _aniNameSecond[9];
		AniSpirteInfo _aniRight[3];
		AniSpirteInfo _aniLeft[3];
		AniSpirteInfo _aniBoss[5];
	public:
		GameAnimation();
		~GameAnimation();

		//设置动画图片信息
		void onRender();
		//创建帧动画
		void addAniData(AniSpirteInfo ani, BYTE type, BYTE kind);//type-判断是第一种动画还是第二种动画，kind-判断是哪种图片

		//比倍左边玩家动画
		Animate* createBiBeiAni(BYTE type, BYTE kind);
		};
}



#endif