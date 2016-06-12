#ifndef __DriftingCarGame_Table_UI_H__
#define __DriftingCarGame_Table_UI_H__

#include "DriftingCarGameTableUICallback.h"

#include "HNNetExport.h"
#include "cocos2d.h"
#include <vector>
#include <array>

namespace DriftingCar
{
	class GameTableLogic;
	class GameBoard;
	class GameResultHistory;
	class GameApplyList;

#define Bet_Button_Tag 1000			//筹码按钮
#define betArea_button_tag 2000		//下注区域

	std::string ValueToString(LLONG value);//转成带,的字符串 "123456789"->"123,456,789"

	class GameTableUI : public HN::HNGameUIBase, public GameTableUICallback
	{
	protected:
		GameTableUI();
		virtual ~GameTableUI();

	public:
		static GameTableUI* create(BYTE bDeskIndex, bool bAutoCreate);
		virtual bool init(BYTE bDeskIndex, bool bAutoCreate);
		virtual void onEnter();
		virtual void onExit();
		void areaClickCallback(cocos2d::Node* pNode);
	public:

		virtual void Test()override;

		virtual void OnGameStation()override;										//第一次进入时场景

		virtual void startGameFree()override;										//空闲消息

		virtual void startGameBeginBet()override;									//下注消息

		virtual void startGamePaoMaDeng()override;									//跑马灯消息

		virtual void startGameEndResult()override;									//结算消息

		virtual void GameBetReuslt()override;										//下注结果消息

		virtual void setBetEnabled(bool enabled);									//下注多少选择按钮是否可用
		
		virtual void setFreeTime(BYTE time);										//设置空间时间

		virtual void setBetTime(BYTE time);											//设置投注时间
		
		virtual void setSendTime(BYTE time);										//设置开奖时间
		
		virtual void setMsg(const std::string &msg);								//显示提示信息
		
		virtual void startSend(int end, BYTE time);									//启动开奖动画
		
		virtual void addRecord(const std::vector<BYTE>& records,BYTE byResult);		//添加开奖记录
		
		virtual void addNT(BYTE seatNo, const std::string& name, LLONG money);		//添加庄家
		
		virtual void removeAllNT();													//移除所有庄家

//		virtual void removeNT(BYTE seatNo);											//移除庄家
		
		virtual void setBetAreaTotal(bool isMy, BYTE AreaNo, LLONG value);			//设置下注区域总下注
		
		virtual void setBetAreaUser(bool isMy, BYTE AreaNo, LLONG value);			//设置下注区域玩家下注
		
		virtual void setBetTotal(LLONG value);										//设置总下注数量
	
		virtual void setNTName(const std::string& name);							//设置庄家名称
		
		virtual void setNTCount(int count);											//设置连庄盘数
		
		virtual void setNTMoney(LLONG value);										//设置庄家金币
		
		virtual void setNTGrade(LLONG value);										//设置庄家成绩
		
		virtual void setUserName(const std::string& name);							//设置玩家名称
		
		virtual void setUserMoney(LLONG value);										//设置玩家金币
		
		virtual void setUserGrade(LLONG value);										//设置玩家成绩
		
		virtual void setUserBetMoney(LLONG value)override ;							//设置下注金额

		virtual void leaveDesk();													//离开桌子
		
		virtual void setOpenArea(int index);										//显示开奖区域
		
		virtual void hideOpenArea();												//隐藏开奖区域
		
		virtual void setApplyAndLeaveButtonEnabled(bool enabled);					//设置上庄申请按钮是否可用

		virtual void SetGameEndResult(LLONG userScore,   LLONG userCapital, 
									  LLONG userNTScore, LLONG userNTCapital);		//设置结算框显示

		virtual void hideGameEndImage();											//隐藏结算框

		virtual void ShowWaitNTCount(BYTE mySeatNo);								//显示自己在庄家列表中的位置

		virtual void ShowApplyButton();												//显示上庄按钮

		virtual void ShowDownNTButton() ;											//显示下庄按钮

		virtual void ShowNoNT()override;											//提示没有庄家

		virtual void ShowBetAreaAction()override;									//显示下注区域

		virtual void HideBetAreaAction()override;									//隐藏下注区域

		virtual void ShowImageMyNT()override;										//显示你已上庄

		virtual void ShowImageMyNoNT()override;										//显示你已下庄
	protected:	
		void initParams();															//初始化参数

		void loadLayout();															//加载UI场景

		void startTimer();															//开始计时

		void stopTimer();															//停止计时

		void reduceTime(float delta);												//时间流逝

		void SetShowTime( BYTE time );												//显示时间

		void LoadBetButton(Node*);													//加载下注按钮

		void LoadBetArea(Node*);													//加载下注区域

		void LoadHistory(Node*);													//加载历史记录

		void LoadPaoMaDeng(Node*);													//加载跑马灯

		void LoadGameEndResult(Node*);												//结算面板

		void LoadNTList(Node*);														//加载上庄列表

		void ShowWaitNTList();														//显示等待上庄队列信息
		
		void Playanimation(int index);												//播放开奖动画

		void ShowSmallAwardCar(int index);											//开奖小车图

		void CheckWaitNTList();														//检测下标

		void ShowCarLogoblink(int index);											//设置跑灯闪烁

		void HideAllCarLogos();														//隐藏所有跑灯

		void ShowblinkBetButton(int index);											//指定筹码闪烁 从1开始

		void StopAllBetButton();													//全部筹码停止闪烁

		void ShowGameStatusImage(int iTime);										//显示状态图片

		void PlaytimeWaring(int itime);												//播放警告时间

		void LoadImageGame(int itype);												//加载提示图

		void ShowImageCarBlink();													//设置背景car闪烁

		void HideImageCarBlink();													//隐藏背景car

	private:
		void menuClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);	//上下庄按钮和退出按钮

		void betClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);		//下注筹码

		void betAreaClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);	//下注区域响应

		void WaitNTListClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchType);//申请上庄和申请下庄按钮

	private:
		enum 
		{
			BET_COUNT  = 8,		//筹码下注个数
			AREA_COUNT = 8,		//下注区域
			Wait_NTList = 5,	//等待上庄UI显示数
			CAR_LIGHT = 4,		//车灯光
		};
		struct GameBetText
		{
			ui::TextBMFont* _userText;		//玩家下注金币
			ui::TextBMFont* _userTotalText;//总的下注金币
			GameBetText():_userTotalText(nullptr),_userText(nullptr){}
		};
		struct NTInfo
		{
			BYTE byseatno;			//用户座位号
			std::string UserName;	//用户昵称
			LLONG lmoney;			//用户金币数
		};

		int _betIndex;											//当前下注按钮的索引0-7
		int _iWinAward;											//开奖索引
		int _iGameStatus;										//游戏状态

		std::array<ui::Button*,BET_COUNT> _betButton;			//筹码按钮
		std::array<ui::Button*,AREA_COUNT> _gameBetArea;		//下注区域
		std::array<GameBetText,AREA_COUNT>_gameBetAreaText;		//下注文本显示

		ImageView* _ImageStatus;					//游戏状态图

		ui::Button* _exitButton;					//退出按钮

		ui::Button* _applyButton;					//上庄按钮
		ui::Button* _leaveButton;					//下庄按钮
		ui::ImageView* _ImagenoNT;					//无座等待图片

		//等待上庄玩家列表
		std::array<Text*,Wait_NTList> _arrayWaitNt;	//等待上庄队列
		ui::Text*   _waitNTNum;						//排庄队列中位置
		std::vector<NTInfo>	vtNtInfp;				//等待用户上庄队列信息
		int _iWaitNtIndex;							//用户等待上庄队列索引

		std::array<ImageView*, CAR_LIGHT> _arrayLight;	//车灯
		ImageView* _image_bet_bg;						//背景图			
		ImageView* _image_bet_car;						//车的图片
		cocos2d::Sprite* _spriteAnimation;				//车动画

		Layer*	_layoutBetArea;						//下注区域容器

		ui::Button* _waitUpButton;					//上一页 等待庄家列表
		ui::Button* _waitDwonButton;				//下一页

		ui::TextAtlas* _timeAtlat;					//时间数字

		ui::Text* _stateTimeText;					//时间文字
		ui::Text* _totalBetText;					//下注总数

		ui::Text*		_NTNameText;				//庄家名称
		ui::TextBMFont* _NTCountText;				//连庄盘数
		ui::TextBMFont* _NTMoneyText;				//庄家金币
		ui::TextBMFont* _NTGradeText;				//庄家成绩

		ui::Text*		_userNameText;				//玩家昵称
		ui::TextBMFont* _userMoneyText;				//玩家金币
		ui::TextBMFont* _userGradeText;				//玩家成绩
		ui::TextBMFont* _userBetText;				//玩家下注

		ImageView*		_imageEnd;					//结算图
		ui::Text*		_UserCapital;				//玩家本金
		ui::Text*		_UserWinScore;				//玩家输赢
		ui::Text*		_UserNTScore;				//庄家输赢
		ui::Text*		_UserNTCapital;				//庄家本金

		GameBoard* _gameBoard;						//跑马灯类
		GameResultHistory* _resultHistory;			//开奖的历史
		GameTableLogic* _tableLogic;				//逻辑类 发送消息
		
	};

}


#endif