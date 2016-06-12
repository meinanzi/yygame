#ifndef __DriftingCar_Game_Table_UI_Callback_H__
#define __DriftingCar_Game_Table_UI_Callback_H__

#include "HNNetExport.h"
#include "HNLogicExport.h"
#include "DriftingCarMessageHead.h"


namespace DriftingCar
{
	class GameTableUICallback : public HN::IHNGameLogicBase
	{
	public:

		virtual void Test() = 0;

		virtual void OnGameStation() = 0;												//第一次进入时场景

		virtual void startGameFree() = 0;												//空闲消息

		virtual void startGameBeginBet() = 0;											//下注消息

		virtual void startGamePaoMaDeng() = 0;											//跑马灯消息

		virtual void startGameEndResult() = 0;											//结算消息

		virtual void GameBetReuslt() = 0;												//下注结果消息

		virtual void setBetEnabled(bool enabled) = 0;									//下注多少选择按钮是否可用

		virtual void setFreeTime(BYTE time) = 0;										//设置空间时间

		virtual void setBetTime(BYTE time) = 0;											//设置投注时间

		virtual void setSendTime(BYTE time) = 0;										//设置开奖时间

		virtual void setMsg(const std::string &msg) = 0;								//显示提示信息

		virtual void startSend( int end, BYTE time) = 0;								//启动开奖动画

		virtual void addRecord(const std::vector<BYTE>& records, BYTE byResult) = 0;	//添加开奖记录
														
		virtual void addNT(BYTE seatNo, const std::string& name, LLONG money) = 0;		//添加庄家

		virtual void removeAllNT() = 0;													//移除所有庄家
														
		virtual void setBetAreaTotal(bool isMy, BYTE AreaNo, LLONG value) = 0;			//设置下注区域总下注
														
		virtual void setBetAreaUser(bool isMy, BYTE AreaNo, LLONG value) = 0;			//设置下注区域玩家下注
														
		virtual void setBetTotal(LLONG value) = 0;										//设置总下注数量
																														
		virtual void setNTName(const std::string& name) = 0;							//设置庄家名称
														
		virtual void setNTCount(int count) = 0;											//设置连庄盘数
														
		virtual void setNTMoney(LLONG value) = 0;										//设置庄家金币
														
		virtual void setNTGrade(LLONG value) = 0;										//设置庄家成绩
														
		virtual void setUserName(const std::string& name) = 0;							//设置玩家名称
														
		virtual void setUserMoney(LLONG value) = 0;										//设置玩家金币
														
		virtual void setUserGrade(LLONG value) = 0;										//设置玩家成绩

		virtual void setUserBetMoney(LLONG value) = 0;									//设置下注金额
														
		virtual void leaveDesk() = 0;													//离开桌子
																
		virtual void setOpenArea(int index) = 0;										//显示开奖区域
																
		virtual void hideOpenArea() = 0;												//隐藏开奖区域
																
		virtual void setApplyAndLeaveButtonEnabled(bool enabled) = 0;					//设置上庄申请按钮是否可用

		virtual void SetGameEndResult(LLONG userScore, LLONG userCapital,				
			LLONG userNTScore, LLONG userNTCapital) = 0;								//设置结算框显示

		virtual void hideGameEndImage() = 0;											//隐藏结算框

		virtual void ShowWaitNTCount(BYTE mySeatNo) = 0;								//显示自己在庄家列表中的位置

		virtual void ShowApplyButton() = 0;												//显示上庄按钮

		virtual void ShowDownNTButton() = 0;											//显示下庄按钮

		virtual void ShowNoNT() = 0;													//提示没有庄家

		virtual void ShowBetAreaAction() = 0;											//显示下注区域

		virtual void HideBetAreaAction() = 0;											//隐藏下注区域

		virtual void ShowImageMyNT() = 0;												//显示你已上庄
			
		virtual void ShowImageMyNoNT() = 0;												//显示你已下庄
	};
}


#endif