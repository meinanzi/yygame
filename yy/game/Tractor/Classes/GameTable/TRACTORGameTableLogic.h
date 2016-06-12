#ifndef __TRACTORGameTableLogic_h__
#define __TRACTORGameTableLogic_h__


#include "HNLogicExport.h"
#include "TRACTORGameClientMessage.h"

namespace TRACTOR
{
	class GameTableUICallback;

	/*
	* GoldenFolower
	* game table logic deal unit.
	*/
	class GameTableLogic: public HNGameLogicBase
	{
	public:
		GameTableLogic(GameTableUICallback* uiCallback, BYTE deskNo, bool bAutoCreate);
		virtual ~GameTableLogic();	
		virtual void dealGameMessage(NetMessageHead* messageHead, void* object, INT objectSize);
	protected:
		/************************line*********************************/
		//game notice message response deal.
		void dealUserSetResp(void* object, INT objectSize);
		void dealBeginUpgradeResp(void* object, INT objectSize);
		void dealSendCardResp(void* object, INT objectSize);
		void dealSendFinishResp(void* object, INT objectSize);
		void dealGamePlayResp(void* object, INT objectSize);
		void dealContinueEndResp(void* object, INT objectSize);
		void dealCutEndResp(void* object, INT objectSize);
		void dealSaleEndResp(void* object, INT objectSize);
		void dealNoteResp(void* object, INT objectSize);
		void dealNoteResultResp(void* object, INT objectSize);
		void dealCollectJetonResp(void* object, INT objectSize);
		void dealCompareResultResp(void* object, INT objectSize);
		void dealFinishCompareResp(void* object, INT objectSize);
		void dealSuperUserResp(void* object, INT objectSize);

		/************************line*********************************/
		//frame message response deal.
		virtual void dealUserAgreeResp(MSG_GR_R_UserAgree* agree);
		virtual void dealGameStartResp(BYTE bDeskNO);
		virtual void dealGameEndResp(BYTE deskNo);
		virtual void dealUserSitResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user);
		virtual void dealUserUpResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user);
		virtual void dealGameInfoResp(MSG_GM_S_GameInfo* pGameInfo);
		virtual void dealGameStationResp(void* object, INT objectSize);
	public:
		void sendStandUp();
		void sendUserSit(BYTE byDeskStation);
		void sendUserReady();
		void loadDeskUsersUI();
		void clearDesk();
		LLONG getMinBuy();
		LLONG getMaxBuy();
		LLONG getPlanBuy();
		BYTE getMeStation();
		/*
		* send message to server, tell which note user do.
		* @param note user note number.
		* @param ming user choose ming or not.
		* @param seatNo user's seat number in view.
		*/
		void sendNote(LLONG note);
		void sendAdd(LLONG note);
		void sendFollow();
		void sendGiveUp();
		void sendLook();
		void compareCardReq();
		void sendCompare(BYTE seatNo);
	protected:
		GameTableUICallback*	_uiCallback;
		TableInfo				_tableInfo;
		void setTableOffsetStation(BYTE bDeskStation);
		bool playingGame(int state);
	protected:
		/* 
		* init member only once.
		*/
		void initParams();

		/*
		* refresh member before every game start.
		*/
		void refreshParams();
		void stopAllWait();
		void initGuoDi();

		void initData();
		/**保存牌桌数据**/
		void saveTableInfo(void* object);
		/**显示玩家信息**/
		void showUserInfo();
		/**显示玩家筹码**/
		//void showUserMoney(const TSubMoney* pSubMoney);
		/**显示带入金额对话框**/
		void showBuyMoney(LLONG min, LLONG max, LLONG plan);
		/**获取下一个玩家的位置**/
		BYTE getNextUserStation(BYTE byStation);
		/**获取上一个玩家的位置**/
		BYTE getLastUserStation(BYTE byStation);
		/**逻辑位置<==>视图位置**/
		BYTE viewToLogicSeatNo(BYTE seatNO);
		BYTE logicToViewSeatNo(BYTE seatNO);
	};
}

#endif // __TRACTORGameTableLogic_h__
