#ifndef __BJLGameTableUICallback_h__
#define __BJLGameTableUICallback_h__

#include "HNBaseType.h"
#include "BJLMessageHead.h"
#include "HNNetExport.h"
#include "HNLogicExport.h"

/************************************************************************/
/* 类型：回调接口                                                        */
/* 功能：声明牌桌逻辑在收到消息时调用界面的函数接口                        */
/* 说明：带user是针对用户，没有则是针对牌桌                                */
/* -1的参数不用改变                                                      */
/************************************************************************/
namespace BJL
{
	class GameTableUICallback : public HN::IHNGameLogicBase
	{
	public:
		virtual void leaveTheGame() = 0;
		
		// 第一个上庄者 游戏直接进入下注
		virtual void OnHandleBegin(S_C_GameBegin * pGameBeginData) = 0;
		
		// 结算
		virtual void showSettlement(S_C_GameResult* pGameResult) = 0; 

		// 显示提示信息
		virtual void IShowNoticeMessage(const std::string& msg) = 0;

		// 申请上、下庄按钮
		virtual void IApplyNT(bool isNT) = 0;

		// 更新上庄列表
		virtual void IUpdateNTList() = 0;

		// 显示庄家信息
		virtual void IUpdateNTInfo(const std::string name, LLONG money) = 0;

		// 显示自己下注额
		virtual void IDisplayMyBetMoney(int noteArea, LLONG money) = 0;

		// 更新玩家金币
		virtual void IUpdateUserMoney(LLONG money) = 0;

		// 显示玩家下注动画
		virtual void IPlayNoteAnimation(BYTE noteType, int noteArea) = 0;

		// 设置筹码按钮是否可用
		virtual void ISetBetEnable(bool enabled[], int count) = 0;

		// 启动倒计时
		virtual void IStartTimer(int second) = 0;

		// 在指定区域下注筹码
		virtual void IAreaBet(int area, LLONG betMoney) = 0;

		// 初始化牌
		virtual void IInitCard(BYTE cards[][3], int playerCount, int NTCount, int playerCardType[], int NTCardType[]) = 0;

		// 显示牌
		virtual void IShowCards(bool animate) = 0;

		// 发牌
		virtual void ISendCard(bool animate) = 0;

		// 提示下注
		virtual void IShowBetHint(bool visible) = 0;

		// 提示等待时间
		virtual void IShowWaitHint(bool visible) = 0;

		// 游戏开始
		virtual void IGameStart() = 0;

		// 显示中奖区域
		virtual void IShowWinArea(bool visible) = 0;

		// 开奖面板
		virtual void IShowOpenBoard(bool visible) = 0;

		//倒计时图移动新位置
		virtual void SetImageTimeMoveNewPostion() = 0;

		//倒计时图移动到老位置
		virtual void SetImageTimeMoveOldPostion() = 0;
	};
}


#endif // __BJLGameTableUICallback_h__