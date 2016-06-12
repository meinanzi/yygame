#ifndef __HN_WaterMargin_GameTableUICallback_H__
#define __HN_WaterMargin_GameTableUICallback_H__

#include "WaterMarginMessageHead.h"

/************************************************************************/
/* 类型：回调接口                                                        */
/* 功能：声明牌桌逻辑在收到消息时调用界面的函数接口                        */
/* 说明：带user是针对用户，没有则是针对牌桌                                */
/* -1的参数不用改变                                                      */
/************************************************************************/

namespace WaterMargin
{

	class GameTableUICallback : public HN::IHNGameLogicBase
	{
	public:
		
		virtual void leaveDesk() = 0;										//离开桌子

		virtual void setMoney(LLONG money) = 0;

		virtual void setDataText(int value, int num) = 0;					//设置5个数据

		virtual void setImageData(int* str) = 0;							//接收滚动结果图片数据

		virtual void setBtnVisible(int Tag, bool visible) = 0;

		virtual void setWinScore(int value) = 0;

		virtual void getWinScore() = 0;										//处理得分按钮

		virtual void setSaiZi(int s1, int s2) = 0;

		virtual void playSecondAnimate(int value, int dianShu) = 0;

		virtual void handleSecondDeFeng() = 0;

		virtual void BiBeiHistory(int* history) = 0;

		virtual void BiBeiAgain() = 0;										//再次比倍

		virtual void MaLiResult(bool begin, int MaLicount, int Index, int* str) = 0;

		virtual void viewWinGold() = 0;                                     //显示小马力赢金币（win和数据）

		virtual void MaLiEndBB() = 0;										//小马力结束，转入比倍

		virtual void MaLIEndDF() = 0;										//小马力以得分结束

		virtual void JudgeMaLi() = 0;

		virtual void JudgeConnect() = 0;
	};
}

#endif //__HN_SlotMachine_GameTableUICallback_H__