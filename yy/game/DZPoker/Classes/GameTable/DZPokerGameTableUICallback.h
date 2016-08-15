#ifndef __DZPokerGameTableUICallback_h__
#define __DZPokerGameTableUICallback_h__

#include "HNNetExport.h"
#include "HNLogicExport.h"
#include "DZPokerGameClientMessage.h"
/************************************************************************/
/* 类型：回调接口                                                        */
/* 功能：声明牌桌逻辑在收到消息时调用界面的函数接口                        */
/* 说明：带user是针对用户，没有则是针对牌桌                                */
/* -1的参数不用改变                                                      */
/************************************************************************/

namespace DZPoker
{

	class GameTableUICallback  : public HN::IHNGameLogicBase
	{
	public:
		//玩家操作，玩家进行了什么操作(跟住、看牌、加注、弃牌、亮牌、超时、离座)
		virtual void showUserBet(BYTE byDeskStation) = 0;
		virtual void showUserCall(BYTE byDeskStation) = 0;
		virtual void showUserAdd(BYTE byDeskStation) = 0;
		virtual void showUserCheck(BYTE byDeskStation) = 0;
		virtual void showUserFold(BYTE byDeskStation) = 0;
		virtual void showUserAllIn(BYTE byDeskStation) = 0;

		//玩家操作按钮控制
		virtual void showBet(bool bVisible, bool bEnable, LLONG betMoney) = 0;
		virtual void showCall(bool bVisible, bool bEnable) = 0;
		virtual void showRaise(bool bVisible, bool bEnable) = 0;
		virtual void showCheck(bool bVisible, bool bEnable) = 0;
		virtual void showFold(bool bVisible, bool bEnable) = 0;
		virtual void showAllIn(bool bVisible, bool bEnable) = 0;
		virtual void showAdd(bool bVisible, bool bEnable) = 0;
		virtual void showSub(bool bVisible, bool bEnable) = 0;
		virtual void showSlider(bool bVisible, bool bEnable, LLONG max, LLONG min, LLONG current) = 0;
		virtual void setSliderValue(LLONG value) = 0;
		virtual LLONG getSliderValue() = 0;

		//玩家操作按钮数据
		virtual void showBetMoney(bool bVisible, LLONG money) = 0;
		virtual void showCallMoney(bool bVisible, LLONG money) = 0;
		virtual void showRaiseMoney(bool bVisible, LLONG money) = 0;

		//自动操作按钮
		virtual void showAutoCall(bool bVisible, bool bSelected) = 0;
		virtual void showAutoCallAny(bool bVisible, bool bSelected) = 0;
		virtual void showAutoCheck(bool bVisible, bool bSelected) = 0;
		virtual void showAutoCheckFold(bool bVisible, bool bSelected) = 0;

		//更新底池
		virtual void showPot(BYTE index) = 0;
		//发手牌
		virtual void showHandCard(std::vector<THandCard>& handCards) = 0;
		//翻牌
		virtual void showFlopCard(const std::vector<BYTE>& byCards) = 0;
		//转牌
		virtual void showTurnCard(BYTE card) = 0;
		//河牌
		virtual void showRiverCard(BYTE card) = 0;
		//更新用户信息
		virtual void showUser(BYTE byDeskStation, bool bMe, bool sex) = 0;
		virtual void showUserUp(BYTE byDeskStation, bool bMe) = 0;
		virtual void showUserNickName(BYTE byDeskStation, const std::string& nickName) = 0;
		virtual void showUserMoney(BYTE byDeskStation,LLONG MONEY) = 0;
		//显示玩家手上的牌型
		virtual void showUserCardType(BYTE seatNo, INT type) = 0;
		//显示庄家
		virtual void showDealer(BYTE byDeskStation) = 0;
		//显示小盲注
		virtual void showSmallBlind(BYTE byDeskStation) = 0;
		//显示大盲注
		virtual void showBigBlind(BYTE byDeskStation) = 0;
		//显示下注筹码（桌面上的） 
		virtual void showUserBetMoney(BYTE byDeskStation, LLONG money) = 0;
		//派奖
		virtual void showWinPool(const std::vector<std::vector<LLONG>> &winPool) = 0;
		//显示玩家手牌
		virtual void showUserHandCard(BYTE byDeskStaion, const std::vector<BYTE>& byCards) = 0;
		//提高公共牌
		virtual void upCommunityCard(BYTE index) = 0;
		//提高玩家手牌
		virtual void upUserHandCard(BYTE byDeskStation, BYTE index) = 0;
		//显示玩家剩余操作时间
		virtual void showUserLeftTime(BYTE byDeskStation,BYTE byTime, BYTE total, bool isMe) = 0;
		//显示新手教学
		virtual void showHelp(bool bVisible) = 0;
		//玩家桌上钱不够
		virtual void showUserLackMoney(BYTE byDeskStation) = 0;
		//清理牌桌
		virtual void clearDesk() = 0;
		//清空所有数据
		virtual void clearDeskCard() = 0;
		//离开牌桌
		virtual void leaveDesk() = 0;
		//显示牌桌准备
		virtual void showTableReady(bool bVisible) = 0;
		//转动座位
		virtual void rotateStation(int offset) = 0;
		//显示购买对话框
		virtual void showBuyMoneyDialog(BYTE deskStation, LLONG min, LLONG max, LLONG plan, LLONG total) = 0;
		//通知提示
		virtual void noticeMessage(const std::string &message) = 0;
		//更换令牌
		virtual void changeToken(bool isMe,BYTE who) = 0;
		//显示牌桌信息
		virtual void showTableInfo(const std::string& tableName) = 0;
		virtual void showMyMoney(LLONG money) = 0;

		//记录倒计时
		virtual void isWaitTime(bool isWait) = 0;

	public:	//比赛系列
		//比赛淘汰
		virtual void showGameContestKick() =0;       
		//等待比赛结束
		virtual void showGameContestWaitOver() = 0;
		//比赛结束
		virtual void showGameContestOver(MSG_GR_ContestAward* contestAward) = 0;
		//显示比赛局数
		virtual void showConstJuShu(int Index) = 0;
		//显示排名
		virtual void ShowConstRank(int iRankNum, int iRemainPeople) = 0;					
		//更新自己的排名	
		virtual void updateMyRankNum(int iValue) = 0;
	};

}

#endif // __DZPokerGameTableUICallback_h__
