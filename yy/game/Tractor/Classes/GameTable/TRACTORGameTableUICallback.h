#ifndef __TRACTORGameTableUICallback_h__
#define __TRACTORGameTableUICallback_h__


#include "HNLogicExport.h"
#include "TRACTORGameClientMessage.h"
/************************************************************************/
/* 类型：回调接口                                                        */
/* 功能：声明牌桌逻辑在收到消息时调用界面的函数接口                        */
/* 说明：带user是针对用户，没有则是针对牌桌                                */
/* -1的参数不用改变                                                      */
/************************************************************************/
namespace TRACTOR
{

	class GameTableUICallback : public IHNGameLogicBase
	{
	public:
		//add by yh
		virtual void showResulte(const std::vector<TGameResult>& results , bool isWin_me) = 0;

		virtual void showHandCard(std::vector<THandCard>& cards) = 0;
		virtual void showUser(BYTE seatNo, bool bMe, bool sex) = 0;
		virtual void showUserUp(BYTE seatNo, bool bMe) = 0;
		virtual void showUserName(BYTE seatNo,CHAR* name) = 0;
		virtual void showUserMoney(BYTE seatNo,LLONG MONEY) = 0;
		virtual void showDealer(BYTE seatNo) = 0;
		virtual void showReady(bool visible) = 0;
		virtual void showAddRange(LLONG min, LLONG max) = 0;

		virtual void showUserNoteMoney(BYTE seatNo, LLONG money) = 0;
		virtual void showWin(BYTE seatNo) = 0;

		/*
		* show user's hand card.
		* @param seatNo user's seat number.
		* @param cards user's hand card.
		*/
		virtual void showUserHandCard(BYTE seatNo, const std::vector<BYTE>& cards) = 0;


		virtual void clearDesk() = 0;
		virtual void leaveDesk() = 0;
		virtual void showLoading(bool bVisible) = 0;
		virtual void rotateSeat(int offset) = 0;
		virtual void alertDialog(const std::string& title, const std::string& message) = 0;
		virtual void showNotice(const std::string &message) = 0;
		virtual void showUserProfit(BYTE seatNo, LLONG money) = 0;
		virtual void showTableInfo(const std::string& tableName) = 0;

		/*
		* show wait time for user to ready.
		* @param seatNo user's seat number.
		* @param byTime wait time.
		* @param byTotalTime total time.
		*/
		virtual void showWaitTime(BYTE seatNo, BYTE byTime, BYTE byTotalTime) = 0;
		/*
		* show note info when game start.
		*/
		virtual void showLimitNote(LLONG note) = 0;
		virtual void showBaseNote(LLONG note) = 0;
		virtual void showLimitPerNote(LLONG note) = 0;
		virtual void showGuoDi(LLONG note) = 0;
		virtual void showNT(BYTE seatNo) = 0;
		virtual void showTotalNote(LLONG note) = 0;
		/*
		* show user's aciton
		*/
		virtual void showUserLookCard(BYTE seatNo, bool isMe) = 0;
		virtual void showUserGiveUp(BYTE seatNo) = 0;
		virtual void showUserNote(BYTE seatNo) = 0;
		virtual void showUserCompare(BYTE fromSeatNo, BYTE toSeatNo, BYTE winSeatNo) = 0;
		virtual void showUserFollow(BYTE seatNo) = 0;
		//virtual void showUserAdd(BYTE seatNo) = 0;
		virtual void showUserOpenCard(BYTE seatNo) = 0;
		/*
		* show user action button
		* @param byFlag each bit meaning each action
		*/
		virtual void showUserAction(BYTE seatNo, BYTE byFlag) = 0;
		virtual void showDashboard(bool visible) = 0;
		/*
		*  show compare result
		*/
		virtual void showCompareResult(BYTE winSeatNo, BYTE loseSeatNo) = 0;
		virtual void showCompareOption(const std::vector<bool> seats) = 0;
	};
}

#endif // __TRACTORGameTableUICallback_h__
