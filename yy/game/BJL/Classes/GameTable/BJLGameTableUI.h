#ifndef __BJLGameTableUI_h__
#define __BJLGameTableUI_h__

#include "HNNetExport.h"
#include "platform/CCCommon.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "BJLGameLogic.h"
#include "BJLMessageHead.h"
#include "BJLGameTableUICallback.h"
#include "BJLResultCard.h"
#include "BJLResultDialog.h"
#include "BJLHistoryDialog.h"
#include "BJLGameTableUiData.h"

#include <string>

using namespace cocostudio;
using namespace ui;
using namespace std;

#define CHOUMANUM			 7									//筹码的按钮的个数
#define NUMOFBETAREA		 8									//多少个下注区域

namespace BJL
{

	class GameTableLogic;

	//游戏界面UI管理
	struct GameView
	{
		Layout*      myLayout;
		Layout*      dashbroad_layout;
		Layout*      gametable_layout;
		ImageView*   table;

		ImageView* dash_bg_imgview;

		//庄家列表(准备删除  不用了)
		ui::ScrollView* scrollView;
		Text* svDealerName[4];

		// 庄家列表
		ListView* ListView_NTList;

		// 庄家列表内容
		ImageView* ImageView_ListItem;

		// 8个下注区域
		Button* Button_BetArea[8];

		// 8个下注提示区域
		ImageView* ImageView_BetWarning[8];

		// 8个赢牌提示区域 闪烁赢的区域
		ImageView* ImageView_BetWin[8];									

		// 请下注
		ImageView* ImageView_betHint;

		// 等待时间
		ImageView* ImageView_WaitHint;		
		
		// 7个筹码按钮
		Button* Button_Bet[7];

		// 7个筹码按钮背景
		ImageView* ImageView_BetBg[7];				

		// 申请上庄
		Button* Button_applyNT;

		// 申请下庄
		Button* Button_applyNoNT;

		// 退出
		Button* Button_Exit;

		// 路子信息
		Button* Button_Records;

		// 8个下注按钮 筹码摆放按钮区，0-7下标分别表示8个区别
		NoteViewInfo pnoteview[8];    

		// 总下注筹码，0-7下标分别表示8个区别
		TextAtlas* TextAtlas_TotalBet[8];

		// 玩家下注筹码，0-7下标分别表示8个区别
		TextAtlas* TextAtlas_UserBet[8];

		// 庄家信息视图
		ImageView* ImageView_NTContainer;

		// 庄家昵称位置
		ImageView* ImageView_NTNickName;

		// 庄家金币位置
		ImageView* ImageView_NTMoney;

		// 计时器背景
		ImageView* ImageView_TimerBg;

		// 计时器数字
		TextAtlas* TextAtlas_TimerText;
	};

	typedef struct tagBetAndType 
	{
		int whichJetton;	//哪一个筹码
		int whichType;		//哪一个区域
	} BET_TYPE;

	class GameTableUI : public HNGameUIBase , public GameTableUICallback
	{
	public:
		// 获取游戏界面
		static HNGameUIBase* create(BYTE bDeskIndex, bool bAutoCreate);

		// 初始化
		bool init(BYTE bDeskIndex, bool bAutoCreate);

		// 退出
		virtual void onExit() override;

		// 进入
		virtual void onEnter() override;

		// 构造函数
		GameTableUI();

		// 析构函数
		virtual ~GameTableUI();
	private:
		bool initUI();
		void initSprite();
		void loadRes();
		void initTheNewGameData();
	private:
		// 下注区域
		void onBetAreaClickCallback(cocos2d::Ref* pSender,Widget::TouchEventType touchtype);

		// 下注筹码
		void onBetClickCallback(cocos2d::Ref* pSender,Widget::TouchEventType touchtype);

		// 公用按钮
		void onCommonClickCallback(cocos2d::Ref* pSender,Widget::TouchEventType touchtype);

		// 申请上庄、下庄
		void onApplyNTClickCallback(cocos2d::Ref* pSender,Widget::TouchEventType touchtype);

		// 下注动画回调
		void betAnimationCallback(cocos2d::Node* pSender);

		// 发牌
		void dispatchCard(float delta);
		
		//路子回调
		void luziscrollview_scroll(cocos2d::Ref* pobj, ui::ScrollView::EventType type);

	private:
		void stopBetAnimation();
		void hideXiaZhuBTN();
		void showSettlementDialog(S_C_GameResult* pData);
		void showMyBetAnimation(BYTE whichBTN , BYTE whichArea);
		void resetDealerList(BYTE dealerStation[], SHORT count);

	private:
		ResultDialog*      _gameResult;
//		GameHistoryDialog* _Dialog;
		GameTableLogic*    _logic;
		ResultCard*        _resultCard;
		
		//游戏界面UI管理
		GameView*          _gameView;
		
		//上庄列表模块
		T_DEALER_LIST*     _dealerList;
	public:
		virtual void leaveTheGame() override;
		
		//第一个上庄者 游戏直接进入下注
		virtual void OnHandleBegin(S_C_GameBegin * pGameBeginData);
		
		//结算
		virtual void showSettlement(S_C_GameResult* pGameResult); 
		
		// 显示提示信息
		virtual void IShowNoticeMessage(const std::string& msg) override;

		// 申请上、下庄按钮
		virtual void IApplyNT(bool isNT) override;

		// 更新上庄列表
		virtual void IUpdateNTList() override;

		// 显示庄家信息
		virtual void IUpdateNTInfo(const std::string name, LLONG money) override;

		// 显示自己下注额
		virtual void IDisplayMyBetMoney(int noteArea, LLONG money) override;

		// 更新玩家金币
		virtual void IUpdateUserMoney(LLONG money) override;

		// 显示玩家下注动画
		virtual void IPlayNoteAnimation(BYTE noteType, int noteArea) override;

		// 设置筹码按钮是否可用
		virtual void ISetBetEnable(bool enabled[], int count) override;

		// 启动倒计时
		virtual void IStartTimer(int second) override;

		// 在指定区域下注筹码
		virtual void IAreaBet(int area, LLONG betMoney) override;

		// 初始化牌
		virtual void IInitCard(BYTE cards[][3], int playerCount, int NTCount, int playerCardType[], int NTCardType[]) override;

		// 显示牌
		virtual void IShowCards(bool animate) override;

		// 发牌
		virtual void ISendCard(bool animate) override;

		// 提示下注
		virtual void IShowBetHint(bool visible) override;

		// 提示等待时间
		virtual void IShowWaitHint(bool visible) override;

		// 游戏开始
		virtual void IGameStart() override;

		// 显示中间区域
		virtual void IShowWinArea(bool visible) override;

		// 开奖面板
		virtual void IShowOpenBoard(bool visible) override;

		//倒计时图移动新位置
		virtual void SetImageTimeMoveNewPostion()override;

		//倒计时图移动到老位置
		virtual void SetImageTimeMoveOldPostion()override;
		
	private:
		//动态创建的一些精灵
		Label* _dealerName;
		Label* _dealerMoney;
		Text* _ownName;
		Text* _ownMoney;
		LLONG _currentMaxZhu[NUMOFBETAREA + 1];         // 每个区域还能下多少注
		LLONG _currentMyZhu[NUMOFBETAREA];				// 玩家下注情况
		LLONG _currentDealerLimit;						   
		Vector<Sprite*> spriteArr;
	private:
		// 计时器跑秒时间
		int _remainTime;

		// 选中的下注按钮
		int _betIndex;

		void timerUpdate(float delat);
		float _fxScale ;
		float _fyScale ;
	};

}

#endif // __BJLGameTableUI_h__