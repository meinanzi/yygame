#include "WaterMarginGameAnimation.h"
#include <string>

namespace WaterMargin
{
	GameAnimation::GameAnimation()
	{
		memset(_aniNameFirst, NULL, sizeof(_aniNameFirst));
		memset(_aniNameSecond, NULL, sizeof(_aniNameSecond));
		memset(_aniRight, NULL, sizeof(_aniRight));
		memset(_aniLeft, NULL, sizeof(_aniLeft));
		memset(_aniBoss, NULL, sizeof(_aniBoss));
	}
	GameAnimation::~GameAnimation()
	{

	}

	//设置动画图片信息
	void GameAnimation::onRender()
	{
		_aniNameSecond[0].name = "SH_2_";					//水浒1
		_aniNameSecond[0].count = 54;
		_aniNameSecond[0].time = 0.05f;

		_aniNameSecond[1].name = "ZYT_2_";					//忠义堂1
		_aniNameSecond[1].count = 49;
		_aniNameSecond[1].time = 0.05f;

		_aniNameSecond[2].name = "TT_2_";					//道行天替1
		_aniNameSecond[2].count = 47;
		_aniNameSecond[2].time = 0.057f;

		_aniNameSecond[3].name = "S_2_";					//宋1
		_aniNameSecond[3].count = 35;
		_aniNameSecond[3].time = 0.077f;

		_aniNameSecond[4].name = "LC_2_";					//林冲1
		_aniNameSecond[4].count = 35;
		_aniNameSecond[4].time = 0.077f;

		_aniNameSecond[5].name = "LZS_2_";					//鲁智深1
		_aniNameSecond[5].count = 54;
		_aniNameSecond[5].time = 0.05f;

		_aniNameSecond[6].name = "D_2_";					//刀1
		_aniNameSecond[6].count = 30;
		_aniNameSecond[6].time = 0.09f;

		_aniNameSecond[7].name = "M_2_";					//矛1
		_aniNameSecond[7].count = 44;
		_aniNameSecond[7].time = 0.061f;

		_aniNameSecond[8].name = "F_2_";					//斧1
		_aniNameSecond[8].count = 42;
		_aniNameSecond[8].time = 0.064f;

		_aniNameFirst[0].name = "SH_1_";					//水浒2
		_aniNameFirst[0].count = 12;
		_aniNameFirst[0].time = 0.067f;

		_aniNameFirst[1].name = "ZYT_1_";					//忠义堂2
		_aniNameFirst[1].count = 12;
		_aniNameFirst[1].time = 0.067f;

		_aniNameFirst[2].name = "TT_1_";					//道行天替2
		_aniNameFirst[2].count = 12;
		_aniNameFirst[2].time = 0.067f;

		_aniNameFirst[3].name = "S_1_";						//宋2
		_aniNameFirst[3].count = 12;
		_aniNameFirst[3].time = 0.067f;

		_aniNameFirst[4].name = "LC_1_";					//林冲2
		_aniNameFirst[4].count = 12;
		_aniNameFirst[4].time = 0.067f;

		_aniNameFirst[5].name = "LZS_1_";					//鲁智深2
		_aniNameFirst[5].count = 12;
		_aniNameFirst[5].time = 0.067f;

		_aniNameFirst[6].name = "D_1_";						//刀2
		_aniNameFirst[6].count = 12;
		_aniNameFirst[6].time = 0.067f;

		_aniNameFirst[7].name = "M_1_";						//矛2
		_aniNameFirst[7].count = 12;
		_aniNameFirst[7].time = 0.067f;

		_aniNameFirst[8].name = "F_1_";						//斧2
		_aniNameFirst[8].count = 11;
		_aniNameFirst[8].time = 0.072f;

		_aniNameFirst[9].name = "yaoqi_";					 //摇旗
		_aniNameFirst[9].count = 42;
		_aniNameFirst[9].time = 0.064f;

		_aniNameFirst[10].name = "frame_";					 //框
		_aniNameFirst[10].count = 5;
		_aniNameFirst[10].time = 0.54f;

		_aniRight[0].name = "Rightlost_";                    //右输
		_aniRight[0].count = 25;
		_aniRight[0].time = 0.15f;

		_aniRight[1].name = "Rightwait_";                    //右等待
		_aniRight[1].count = 31;
		_aniRight[1].time = 0.15f;

		_aniRight[2].name = "Rightwin_";                     //右赢
		_aniRight[2].count = 18;
		_aniRight[2].time = 0.15f;

		_aniLeft[0].name = "Leftlost_";                      //左输
		_aniLeft[0].count = 30; 
		_aniLeft[0].time = 0.15f;

		_aniLeft[1].name = "Leftwait_";                      //左等待
		_aniLeft[1].count = 27;
		_aniLeft[1].time = 0.15f;

		_aniLeft[2].name = "Leftwin_";                       //左赢
		_aniLeft[2].count = 30;
		_aniLeft[2].time = 0.15f;

		_aniBoss[0].name = "Bosslost_";                      //boss输
		_aniBoss[0].count = 25;
		_aniBoss[0].time = 0.15f;

		_aniBoss[1].name = "Bossopen_";                      //boss开奖
		_aniBoss[1].count = 14;
		_aniBoss[1].time = 0.15f;

		_aniBoss[2].name = "Bossthrow_";                     //boss摇色子
		_aniBoss[2].count = 25;
		_aniBoss[2].time = 0.15f;

		_aniBoss[3].name = "Bosswait_";                      //boss等待
		_aniBoss[3].count = 8;
		_aniBoss[3].time = 0.15f;

		_aniBoss[4].name = "BossWin_";                       //boss赢
		_aniBoss[4].count = 15;
		_aniBoss[4].time = 0.15f;
	}

	//将帧动画加到缓存池
	void GameAnimation::addAniData(AniSpirteInfo ani, BYTE type, BYTE kind)
	{
		char str[32];
		//创建动画
		Animation *bMove = Animation::create();
		bMove->setDelayPerUnit(ani.time);
		for (int i = 1; i <= ani.count; i++)
		{
			std::string name(ani.name);
			sprintf(str, "%d.png", i);
			name.append(str);
			bMove->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(name));
		}
		
		sprintf(str, "ani_%d_%d", type, kind);//1-第一动画，2-第二动画，3-比倍右边人的动画，4-左边人的动画，5-boss动画
		AnimationCache::getInstance()->addAnimation(bMove, str);
	}

	Animate* GameAnimation::createBiBeiAni(BYTE type, BYTE kind)
	{
		char name[30] = { 0 };
		sprintf(name, "ani_%d_%d", type, kind);
		auto animation = dynamic_cast<Animation*>(AnimationCache::getInstance()->getAnimation(name));
		auto animate = Animate::create(animation);
		return animate;
	}
}