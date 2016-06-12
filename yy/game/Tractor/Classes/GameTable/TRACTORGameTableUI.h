#ifndef __TRACTORGameTableUI_h__
#define __TRACTORGameTableUI_h__


#include "TRACTORGameTableUICallback.h"

#include "HNNetExport.h"
#include "HNUIExport.h"
#include "cocos2d.h"
#include "TRACTORGamePlayer.h"
#include "TRACTORPokerCard.h"
#include "TRACTORGameDealerUI.h"
#include "TRACTORGameCardType.h"
#include <vector>


/*
* game ui unit.
*/
namespace TRACTOR
{
	class GameTableLogic;
	class Dashboard;

	class GameTableUI : public HNGameUIBase, public GameTableUICallback
	{
	public:
		static HNGameUIBase* create(BYTE bDeskIndex, bool bAutoCreate);
		virtual bool init(BYTE bDeskIndex, bool bAutoCreate);
		virtual void onExit() override;

	private:
		typedef struct TTableUI
		{
			ui::Widget* wOutline;
			ui::Button* bCompare[PLAY_COUNT];
			ui::Button* bSeat[PLAY_COUNT];
			ui::Button* bCardType;
			ui::Button* bJumpMenu[4];
			ui::Button* bBackMenu;
			ui::Widget* wPullDownMenu;
			ImageView*  iTable;
			ui::Button* bReady;
			ui::TextAtlas* lGuoDi;
			ui::TextAtlas* lLimitNote;
			ui::TextAtlas* lNote;
			ui::TextAtlas* lTopNote;
			ui::Text* lLastNote;
			ui::Text* lMyNote;
			ui::TextAtlas* lTotalNote;
			TTableUI()
				: bCardType(nullptr)
				, wPullDownMenu(nullptr)
				, bBackMenu(nullptr)
				, iTable(nullptr)
				, bReady(nullptr)
				, lGuoDi(nullptr)
				, lLimitNote(nullptr)
				, lNote(nullptr)
				, lTopNote(nullptr)
				, lTotalNote(nullptr)
			{
				memset(bCompare, 0, sizeof(bCompare));
				memset(bSeat, 0, sizeof(bSeat));
				memset(bJumpMenu, 0 , sizeof(bJumpMenu));
			}
		}TTableUI;

		//json文件读取位置
		cocos2d::Vec2 _seatPosition[PLAY_COUNT];
		cocos2d::Vec2 _jettonPosition[PLAY_COUNT];
		cocos2d::Vec2 _dealerPostion;
		Sprite* _loadSprite;													//桌子预加载蒙版

		PlayerUI* _players[PLAY_COUNT];
		Label* _lRoomInfo;
		GameTableLogic* _tableLogic;
		GameDelearUI* _dealer;
		Dashboard* _dashboard;
		TTableUI _tableUI;
	private:
		virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	private:
		int getPlayerCount();
		//menu click callback.
		void menuClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);
		//seat menu click callback.
		void seatClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);
		//compare menu click callbakc.
		void compareClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);
		void pullDownItemCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);
		void menuTouchReset(Button* btn);
	public:
		/*
		* user note aciton from dashboard.
		*/
		void compareCardReq();
		void lookCard();
		void compareCard();
		void followNote();
		void giveUpNote();
		void hideVSButton();
		void addNote(LLONG money);
		void getReady();
	public:
		//add yh
		virtual void showResulte(const std::vector<TGameResult>& results , bool isWin_me) override;
		
		/*
		* dispatch user hand card.
		@ param cards users hand card
		*/
		virtual void showHandCard(std::vector<THandCard>& cards);
		/*
		* show user's hand card.
		* @param seatNo user's seat number.
		* @param cards user's hand card.
		*/
		virtual void showUserHandCard(BYTE seatNo, const std::vector<BYTE>& cards);


		/*
		* show user when user sit in table.
		* @param seatNo user's seat number.
		* @param bMe if it is login user.
		*/
		virtual void showUser(BYTE seatNo, bool bMe, bool sex);
		/*
		* clear user when user leave table.
		* @param seatNo user's seat number.
		* @param bMe if it is login user.
		*/
		virtual void showUserUp(BYTE seatNo, bool bMe);
		virtual void showUserName(BYTE seatNo,CHAR* name);
		virtual void showUserMoney(BYTE seatNo,LLONG MONEY);
		virtual void showDealer(BYTE seatNo);
		virtual void showUserNoteMoney(BYTE seatNo, LLONG money);
		virtual void showReady(bool visible);
		virtual void showAddRange(LLONG min, LLONG max);

		//send prize to winer.
		virtual void showWin(BYTE winnerSeatNo);

		virtual void clearDesk();
		virtual void leaveDesk() ;
		virtual void showLoading(bool bVisible);
		virtual void rotateSeat(int offset);
		virtual void alertDialog(const std::string& title, const std::string& message);
		virtual void showNotice(const std::string &message);
		virtual void showUserProfit(BYTE seatNo, LLONG money);
		virtual void showTableInfo(const std::string& tableName);
		/*
		* show wait time for user to ready.
		* @param seatNo user's seat number.
		* @param byTime wait time.
		*/
		virtual void showWaitTime(BYTE seatNo, BYTE byTime, BYTE byTotalTime);
		/*
		* show note info when game start.
		*/
		virtual void showLimitNote(LLONG note);
		virtual void showBaseNote(LLONG note);
		virtual void showLimitPerNote(LLONG note);
		virtual void showGuoDi(LLONG note);
		virtual void showNT(BYTE seatNo);
		virtual void showTotalNote(LLONG note);
		/*
		* show user's aciton
		*/
		virtual void showUserLookCard(BYTE seatNo, bool isMe);
		virtual void showUserGiveUp(BYTE seatNo);
		virtual void showUserNote(BYTE seatNo);
		virtual void showUserCompare(BYTE fromSeatNo, BYTE toSeatNo, BYTE winSeatNo);
		virtual void showUserFollow(BYTE seatNo);
		//virtual void showUserAdd(BYTE seatNo);
		virtual void showUserOpenCard(BYTE seatNo);
		/*
		* show user action button
		* @param byFlag each bit meaning each action
		*/
		virtual void showUserAction(BYTE seatNo, BYTE byFlag);
		virtual void showDashboard(bool visible);
		/*
		*  show compare result
		*/
		virtual void showCompareResult(BYTE winSeatNo, BYTE loseSeatNo);
		virtual void showCompareOption(const std::vector<bool> seats);

		void playThunderAnimate(float duration);
		void boom(cocos2d::Ref* pRef);

	protected:
		//牌桌界面初始化
		void inflateLayout();
		bool seatNoIsOk(BYTE seatNo);
		void initGameData();
		void initPosition();
		PlayerUI* getPlayer(int seatNo);
		void showCardType();
		GameTableUI();
		virtual ~GameTableUI();
		friend class GameDelearUI;
	};
}


#endif // __TRACTORGameTableUI_h__