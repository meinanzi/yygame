#ifndef __DZPokerGameTableUI_h__
#define __DZPokerGameTableUI_h__

#include "DZPokerGameTableUICallback.h"

#include "HNNetExport.h"
#include "cocos2d.h"
#include "DZPokerGamePlayer.h"
#include "DZPokerPokerCard.h"
#include "DZPokerGameDealerUI.h"
#include "DZPokerGameCardType.h"
#include <vector>

/************************************************************************/
/* 游戏界面操作                                                          */
/************************************************************************/


namespace DZPoker
{

	typedef struct TableView
	{
		Node*		table;
		Node*		dashboard;
		Node*		pulldownMenu;
		ImageView*	bg;
		ImageView*	dealer;
		ImageView*	card_type;
		Button*		btnMenu;
		Button*		btnPosition[PLAY_COUNT];	// 8个椅子按钮
		int			sitposition;				// 标记当前玩家坐下的座位号，如果没有坐下为-1；
		Button*		btnJetton[4];
		ImageView*	idashbroad;
		Button*		btnControll[3];				// 分别表示 跟牌，加注，弃牌
		int			btnControllTag[3];			// 表示第3个按钮子功能的Tag
		ImageView*	addSliderbg;
		Button*		btnCardType;
		Slider*		addSlider;
		TextAtlas*	addSliderMoney;
		ImageView*  pullDownMume;
		Button*		pullDownItemMume[3];
		int			silderMaxMoney;
		int			sliderMinMoney;
	} TableView;

	class GameTableLogic;

	class GameTableUI : public HNGameUIBase, public GameTableUICallback
	{

		static const int NUM_PUBLIC_CARD	= 5;		// 公共牌数量
		static const int NUM_TABLE_JETTON	= 5;		// 桌子上的筹码

	public:
		//bDeskIndex:桌子的索引
		static HNGameUIBase* create(BYTE bDeskIndex, bool bAutoCreate);

		bool init(BYTE bDeskIndex, bool bAutoCreate);
		GameTableUI();
		virtual ~GameTableUI();
		PlayerUI* getPlayer(int seatNo);

	public:
		virtual void onExit() override;

	private:
		//json文件读取位置
		Vec2 _positionOfPlayer[PLAY_COUNT];
		Vec2 _positionOfJetton[PLAY_COUNT];
		Vec2 _positionOfDealer;
		Vec2 _positionOfPublicCard[5];

		Vec2 _directionOf4[4];				//0-3   下左上右
		Vec2 _directionOf8[8];				//0-7    下，左下，左，左上，上，右上，右，右下
		void initPosition();

	private:
		virtual bool onTouchBegan(Touch *touch, Event *unused_event);

	private:
		//std::vector<GamePlayer*> _playerlist;
		//GamePlayer* _playerlist[PLAY_COUNT];
		PlayerUI* _players[PLAY_COUNT];
		int getSizeOfPlayerList();
		//GamePlayer* _currentPT;
		TableView* _tableView;
		void showCardTypeCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);
		void deskClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);
		void jettonClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);
		void controllBtnClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);
		void sliderCallback(cocos2d::Ref* pSender,ui::Slider::EventType type);

		//void todichiCallBack(cocos2d::Ref* pSender);
		//void leavedichiCallBack(cocos2d::Ref* pSender);
		//void xiazhuCallBack(cocos2d::Ref* pSender);
		//void kaipaiCallBack(cocos2d::Ref* pSender);
		//void checkCallback(cocos2d::Ref* pSender);
		//void qipaiCallBack(cocos2d::Ref* pSender);
		//void callCallBack(cocos2d::Ref* pSender);

		void pullDownCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);
		void pullDownItemCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);
		//按钮点击功能复位
		void menuTouchReset(Button* btn);


		PokerCard*		_publicCard[NUM_PUBLIC_CARD];								//桌子上的公共的5张牌											
		ProgressTimer*	_progressTimer;											//桌子上的计时器
		Sprite*			_loadSprite;													//桌子预加载蒙版
		GameCardType*	_cardTypeLayer;


		//GameAnimation* _gameanimation;

	private:
		Label* _roomIdInFo;
		Label* _lTableName;
		Label* _lPlayerMoney;
		Label* _lConstJuShu;			//比赛局数
		Label* _lConstRank;			//比赛排名
		

	public:
		Sprite* money_dichi_jetton[NUM_TABLE_JETTON];									//桌子上的游戏筹码
	public://动画测试函数；仅用于测试动画效果和调用对应动画的方法  可以删除；
	//	//发牌Test
	//	void startFaPai();
	//	void fapaiTimerFUNC(float dt);

	//	//弃牌Test
	//	void startQiPai();	
	//	//计时器Test
	//	void startTimer();
	//	//下注Test
	//	void startXiaZhu();
	//	//开牌Test
	//	void startKaiPai();
	//private:
		//初始化一局游戏
		void initGameData();
		//开始新的一轮游戏所要将数据初始化
		void initNewturn();


		//////////////////////////////////////////////////////////////////////////
	private:
		void updatePlayerByDeskStation(BYTE bDeskStation);
		void updatePlayerByDeskStation(std::vector<BYTE>& byDeskStations);
	public:
		/**界面回调**/
		//玩家操作(跟住、看牌、加注、弃牌、亮牌、超时、离座)
		virtual void showUserBet(BYTE byDeskStation);			//注意区分showBetMoney	下注
		virtual void showUserCall(BYTE byDeskStation);			//跟注
		virtual void showUserAdd(BYTE byDeskStation);			//滑动条加注
		virtual void showUserCheck(BYTE byDeskStation);			//过牌
		virtual void showUserFold(BYTE byDeskStation);			//弃牌
		virtual void showUserAllIn(BYTE byDeskStation);			//全下

		//玩家操作按钮控制
		virtual void showBet(bool bVisible, bool bEnable, LLONG benMoney);
		virtual void showCall(bool bVisible, bool bEnable);
		virtual void showRaise(bool bVisible, bool bEnable);
		virtual void showCheck(bool bVisible, bool bEnable);
		virtual void showFold(bool bVisible, bool bEnable);
		virtual void showAllIn(bool bVisible, bool bEnable);
		virtual void showAdd(bool bVisible, bool bEnable);
		virtual void showSub(bool bVisible, bool bEnable);
		virtual void showSlider(bool bVisible, bool bEnable, LLONG max, LLONG min, LLONG current);
		virtual void setSliderValue(LLONG value);
		virtual LLONG getSliderValue();

		//玩家操作按钮数据
		virtual void showBetMoney(bool bVisible, LLONG money);
		virtual void showCallMoney(bool bVisible, LLONG money);
		virtual void showRaiseMoney(bool bVisible, LLONG money);

		//更新底池
		virtual void showPot(BYTE index);
		//发手牌
		virtual void showHandCard(std::vector<THandCard>& handCards);
		//翻牌
		virtual void showFlopCard(const std::vector<BYTE>& byCards);
		//转牌
		virtual void showTurnCard(BYTE card);
		//河牌
		virtual void showRiverCard(BYTE card);
		//更新用户信息
		virtual void showUser(BYTE byDeskStation, bool bMe, bool sex);
		virtual void showUserUp(BYTE byDeskStation, bool bMe);
		virtual void showUserNickName(BYTE byDeskStation, const std::string& nickName);
		virtual void showUserMoney(BYTE byDeskStation,LLONG MONEY);
		//显示玩家手上的牌型
		virtual void showUserCardType(BYTE seatNo, INT type);
		//显示庄家
		virtual void showDealer(BYTE byDeskStation);
		//显示小盲注
		virtual void showSmallBlind(BYTE byDeskStation);
		//显示大盲注
		virtual void showBigBlind(BYTE byDeskStation);
		//显示下注筹码（桌面上的） 
		virtual void showUserBetMoney(BYTE byDeskStation,LLONG money);
		//派奖
		virtual void showWinPool(const std::vector<std::vector<LLONG>> &winPool);
		//显示玩家手牌
		virtual void showUserHandCard(BYTE byDeskStaion, const std::vector<BYTE>& byCards);
		//显示公共牌
		virtual void upCommunityCard(BYTE index);
		//提高玩家手牌
		virtual void upUserHandCard(BYTE byDeskStation, BYTE index);
		//显示玩家剩余操作时间
		virtual void showUserLeftTime(BYTE byDeskStation,BYTE byTime, BYTE total, bool isMe);
		//自动操作按钮
		virtual void showAutoCall(bool bShow, bool bSelected);
		virtual void showAutoCallAny(bool bShow, bool bSelected);
		virtual void showAutoCheck(bool bShow, bool bSelected);
		virtual void showAutoCheckFold(bool bShow, bool bSelected);
		//显示新手教学
		virtual void showHelp(bool bVisible);
		//玩家桌上钱不够
		virtual void showUserLackMoney(BYTE byDeskStation);
		//清理牌桌
		virtual void clearDesk();
		//清空桌面的牌
		virtual void clearDeskCard();
		//离开牌桌
		virtual void leaveDesk() ;
		//显示牌桌准备
		virtual void showTableReady(bool bVisible);
		//转动座位
		virtual void rotateStation(int offset);
		//显示购买对话框
		virtual void showBuyMoneyDialog(BYTE deskStation, LLONG min, LLONG max, LLONG plan, LLONG total);
		//通知提示
		virtual void noticeMessage(const std::string &message);
		//更换令牌
		virtual void changeToken(bool isMe,BYTE who);
		//显示牌桌信息
		virtual void showTableInfo(const std::string& tableName);
		virtual void showMyMoney(LLONG money);

	public://比赛系列
		//比赛淘汰
		virtual void showGameContestKick();       
		//等待比赛结束
		virtual void showGameContestWaitOver();
		//比赛结束
		virtual void showGameContestOver(MSG_GR_ContestAward* contestAward);
		//比赛局数
		virtual void showConstJuShu(int Index);
		//显示排名
		virtual void ShowConstRank(int iRankNum, int iRemainPeople) ;		
		//更新自己的排名	
		virtual void updateMyRankNum(int iValue);								
	protected:
		bool				_firstOnEnter;
		GameTableLogic*		_tableLogic;
		GameDelearUI*		_dealer;
		//牌桌界面初始化
		void inflateLayout();
		bool seatNoIsOk(BYTE seatNo);
		friend class GameDelearUI;

	public:
		int _waitTime;
		void isWaitTime(bool isWait);
		void updateTime(float dt);

		int		_iContestNum;							//记录自己排名
	};
}


#endif // __DZPokerGameTableUI_h__