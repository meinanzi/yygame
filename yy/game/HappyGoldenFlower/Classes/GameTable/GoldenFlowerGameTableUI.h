#ifndef __GoldenFlowerGame_Table_UI_H__
#define __GoldenFlowerGame_Table_UI_H__

#include "GoldenFlowerGameTableUICallback.h"
#include "GoldenFlowerGamePlayer.h"
#include "GoldenFlowerPokerCard.h"
#include "GoldenFlowerGameDealerUI.h"
#include "GoldenFlowerGameCardType.h"

#include "HNNetExport.h"
#include "cocos2d.h"
#include <vector>

namespace goldenflower
{
	/*
 * game ui unit.
 */

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
		ui::Layout* lOutline;
		ui::ImageView* bCompare[PLAY_COUNT];
		ui::ImageView* ReadySign[PLAY_COUNT];
		ImageView* WatchSign[PLAY_COUNT];
		ImageView* GiveUpSign[PLAY_COUNT];
		ImageView* next_game;
		ImageView* head_image[2];                //比牌时头像框
		ui::Button* bSeat[PLAY_COUNT];
		ImageView* iDealer[PLAY_COUNT];	
		ImageView* iCard[PLAY_COUNT];
		Node* Nbet[PLAY_COUNT];
		ui::Button* bCardType;
		ui::Button* bJumpMenu[4];
		ui::Button* bBackMenu;
		ui::Layout* nPullDownMenu;
		ImageView*  iTable;
		ui::Button* bReady;
		ui::Text* lGuoDi;
		ui::Text* lLimitNote;
		ui::Text* lNote;
		ui::Text* lTopNote;
		ui::Text* lLastNote;
		ui::Text* lMyNote;
		ui::Text* lTotalNote;
		ui::Text* lMyTotalNote;
		// 计时器背景
		ui::ImageView* ImageView_TimerBg;
		// 计时器数字
		ui::TextAtlas* TextAtlas_TimerText;
		TTableUI()
			: bCardType(nullptr)
			, nPullDownMenu(nullptr)
			, bBackMenu(nullptr)
			, iTable(nullptr)
			, bReady(nullptr)
			, lGuoDi(nullptr)
			, lLimitNote(nullptr)
			, lNote(nullptr)
			, lTopNote(nullptr)
			, lTotalNote(nullptr)
			, lMyTotalNote(nullptr)
			, next_game(nullptr)
			, ImageView_TimerBg(nullptr)
			, TextAtlas_TimerText(nullptr)
		{	
			memset(ReadySign, 0, sizeof(ReadySign));
			memset(bCompare, 0, sizeof(bCompare));
			memset(bSeat, 0, sizeof(bSeat));
			memset(bJumpMenu, 0 , sizeof(bJumpMenu));
			memset(Nbet, 0, sizeof(Nbet));
			memset(WatchSign, 0, sizeof(WatchSign)); 
			memset(GiveUpSign, 0, sizeof(GiveUpSign));
			memset(head_image, 0, sizeof(head_image));
		}
	}TTableUI;

	//json文件读取位置
	cocos2d::Vec2 _seatPosition[PLAY_COUNT];
	cocos2d::Vec2 _jettonPosition[PLAY_COUNT];
	BYTE _viewSeat[PLAY_COUNT];
	S_C_GameEnd _pendData;
	BYTE _viewOfWiner;
	cocos2d::Vec2 _dealerPostion;
	cocos2d::Vec2 _headImagePostion[2];                                     //记录比牌时的头像位置
	Sprite* _loadSprite;													//桌子预加载蒙版
	
	PlayerUI* _players[PLAY_COUNT];
	Label* _lRoomInfo;
	GameTableLogic* _tableLogic;
	GameDelearUI* _dealer;
	Dashboard* _dashboard;
	Layout *_endLayer;                                                     //结算层
	TTableUI _tableUI;
	std::vector<PokerCard *> _vecCard;
	int _visibleWidth, _visibleHeight;
private:
	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
private:
	int getPlayerCount();
	//menu click callback.
	void menuClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);
	void EndLayerClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);
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
	void addNote(LLONG money);
	void addBet(int multiple);
	void getReady();
public:
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
	virtual void showUserFlipCard(BYTE seatNo, const std::vector<BYTE>& cards);


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
	virtual void showUserNoteMoney(BYTE seatNo, LLONG money, int DiZhu);
	virtual void showWatchCard(BYTE seatNo, bool visible);
	virtual void showGiveUpCard(BYTE seatNo, bool visible);
	virtual void showReadySign(BYTE seatNo, bool visible);
	virtual void showAddRange(LLONG min, LLONG max);

	//send prize to winer.
	virtual void showWin(BYTE winnerSeatNo);

	//显示结算框
	virtual void showEndBox(S_C_GameEnd *pdata, BYTE *viewSeat, BYTE viewWiner);
	void startEndBox(S_C_GameEnd *pdata, BYTE *viewSeat, BYTE viewWiner);
	virtual void clearDesk();
	virtual void leaveDesk() ;
	virtual void clearInvalidUser();
	virtual void showLoading(bool bVisible);
	virtual void rotateSeat(int offset);
	virtual void alertDialog(const std::string& title, const std::string& message);
	virtual void showNotice(const std::string &message);
	virtual void showUserProfit(BYTE seatNo, LLONG money);
	virtual void showTableInfo(const std::string& tableName);
	// 启动倒计时
	virtual void IStartTimer(int second);
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
	virtual void showTotalNote(LLONG note);
	virtual void showMyTotalNote(LLONG note);
	virtual void showReady(bool visible);
	virtual void showNextGame(bool visible);
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
	virtual void showDashboard(bool visible);

	virtual void setLookVisible(bool visible) override;
	virtual void setFollowVisible(bool visible) override;
	virtual void setAddVisible(bool visible, bool *CanAdd) override;
	virtual void setOpenVisible(bool visible) override;
	virtual void setGiveUpVisible(bool visible) override;
	/*
	 *  show compare result
	 */
	virtual void showCompareResult(BYTE winSeatNo, BYTE loseSeatNo);
	virtual void showCompareOption(const std::vector<bool> seats);

	void playThunderAnimate(float duration);
	void playBombAnimate(float duration, bool leftWin); 
	void moveBomb(bool leftWiner);
	//比牌完后移除头像
	void moveHead(bool winnerIsLeft, BYTE winSeatNo);
	void boom(cocos2d::Ref* pRef);
	Vec2 getCardPosition(BYTE seatNo);
	Vec2 getSeatPosition(BYTE seatNo);
protected:
	//牌桌界面初始化
	void inflateLayout();
	bool seatNoIsOk(BYTE seatNo);
	void initGameData();
	void initPosition();
	void timerUpdate(float delat);
	void releaseGameEndBox(float dt);
	void disapearEndBox(float dt);
	

	PlayerUI* getPlayer(int seatNo);
	PlayerUI* getPlayer(LLONG userId);
	void showCardType();
	GameTableUI();
	virtual ~GameTableUI();
	friend class GameDelearUI;
	// 计时器跑秒时间
	int _remainTime;
};
}


#endif	//__GoldenFlowerGame_Table_UI_H__