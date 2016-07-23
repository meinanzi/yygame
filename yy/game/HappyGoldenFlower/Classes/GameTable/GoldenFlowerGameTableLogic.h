#ifndef __GoldenFlowerGame_Table_Logic_H__
#define __GoldenFlowerGame_Table_Logic_H__

#include "GoldenFlowerMessageHead.h"

#include "HNNetExport.h"
#include "HNLogicExport.h"

namespace goldenflower
{
	class GameTableUICallback;

	/*
	 * GoldenFolower
	 * game table logic deal unit.
	 */
	class GameTableLogic: public HN::HNGameLogicBase
	{
	public:
		// 构造函数
		GameTableLogic(GameTableUICallback* uiCallback, BYTE deskNo, bool bAutoCreate);
	
		// 析构函数
		virtual ~GameTableLogic();

	protected:
		// 处理游戏消息
		virtual void dealGameMessage(NetMessageHead* messageHead, void* object, INT objectSize);

		// 游戏开始
		void dealGameBeginResp(void* object, INT objectSize);

		// 发牌消息
		void dealSendCardResp(void* object, INT objectSize);

		// 开始游戏
		void dealGamePlayResp(void* object, INT objectSize);

		// 通知玩家动作
		void dealCallActionResp(void* object, INT objectSize);

		// 游戏结束（游戏内消息）
		void dealGameEndResp(void* object, INT objectSize);

		// 超端校验
		void dealSuperProofResp(void* object, INT objectSize);

		// 超端设置结果
		void dealSuperSetResultResp(void* object, INT objectSize);

		// 玩家看牌结果
		void dealLookCardResultResp(void* object, INT objectSize);

		// 玩家跟注结果
		void dealGenZhuResultResp(void* object, INT objectSize);

		// 加注结果
		void dealJiaZhuResultResp(void* object, INT objectSize);

		// 玩家比牌结果
		void dealBiPaiResultResp(void* object, INT objectSize);

		// 玩家弃牌结果
		void dealGiveUpResultResp(void* object, INT objectSize);

		// 玩家同意游戏（平台消息）
		virtual void dealUserAgreeResp(MSG_GR_R_UserAgree* agree);

		// 游戏开始（平台消息）
		virtual void dealGameStartResp(BYTE bDeskNO);

		// 游戏结束（平台消息）
		virtual void dealGameEndResp(BYTE deskNo);

		// 玩家坐下（平台消息）
		virtual void dealUserSitResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user);
		
		//玩家排队坐下
		virtual void dealQueueUserSitMessage(BYTE deskNo, const std::vector<QUEUE_USER_SIT_RESULT*>& user);

		// 玩家站起（平台消息）
		virtual void dealUserUpResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user);

		// 游戏消息（平台消息）
		virtual void dealGameInfoResp(MSG_GM_S_GameInfo* pGameInfo);

		// 游戏状态消息（平台消息）
		virtual void dealGameStationResp(void* object, INT objectSize);

		// 游戏状态信息（等待同意）
		void dealWaitAgreeResp(S_C_GameStation_WaiteAgree* pData);

		// 游戏状态信息（发牌）
		void dealSendCardResp(S_C_GameStation_SendCard* pData);

		// 游戏状态信息（游戏中）
		void dealPlayGameResp(S_C_GameStation_PlayGame* pData);

		//--------------------------比赛接口---------------------
		//比赛信息广播
		virtual void dealGameContestNotic(MSG_GR_I_ContestInfo* contestInfo) override;
		//用户比赛信息
		virtual void dealGameUserContset(MSG_GR_ContestChange* contestChange) override;
		 //比赛淘汰
		virtual void dealGameContestKick() override;
		 //等待比赛结束
		virtual void dealGameContestWaitOver() override;
		 //比赛结束
		virtual void dealGameContestOver(MSG_GR_ContestAward* contestAward) override;

	

	public:
		//加入排队
		void sendQueue();
		// 站起
		void sendStandUp();
	
		// 坐下
		void sendUserSit(BYTE byDeskStation);

		// 准备
		void sendUserReady();

		//进入游戏
		void enterGame();

		// 加载用户
		void loadUsers();

		// 清理牌桌
		void clearDesk();

		// 清理非法用户
		void clearInvalidUser();

		// 获取最小购买
		LLONG getMinBuy();

		// 获取最大购买
		LLONG getMaxBuy();

		// 获取购买值
		LLONG getPlanBuy();

		// 获取我的座位号
		BYTE getMeStation();
	
		// 下注
		void sendNote(LLONG note);

		// 加注
		void sendAdd(LLONG note);

		//加注的倍数
		void sendAddBet(int multiple);

		// 跟住
		void sendFollow();

		// 弃牌
		void sendGiveUp();

		// 看牌
		void sendLook();

		// 显示比牌选择
		void compareCardReq();

		// 比牌
		void sendCompare(BYTE seatNo);

		// 显示操作按钮
	protected:
		// 界面回掉
		GameTableUICallback*	_uiCallback;

		// 牌桌信息
		//TableInfo				_tableInfo;

		// 当前下注数
		LLONG                   _curNote;


		// 玩家是否在玩
		bool playingGame(int state);

		// 变量初始化
		void initParams();

		// 变量重置
		void refreshParams();

		// 停止所有等待
		void stopAllWait();

		// 初始化锅底
		void initGuoDi();

		// 显示玩家信息
		void showUserInfo();

		// 显示购买对话框
		void showBuyMoney(LLONG min, LLONG max, LLONG plan);

		// 获取下一个玩家的位置
		BYTE getNextUserStation(BYTE byStation);

		// 获取上一个玩家的位置
		BYTE getLastUserStation(BYTE byStation);

	private:
		
		// 庄家座位号
		BYTE                _NTSeatNo;                     

		// 本局锅底值
		int                 _GuoDi;

		// 本局底注
		int					_DiZhu;							

		// 玩家状态
		int					_UserState[PLAY_COUNT];			

		//本局下注上限 (即 最大每轮最大下注数不能超过这个值) 暗注的，但显示为明注的
		//LLONG				_ShangXian;					

		//本局顶注 -总下注上限	达到上限 就要开牌	
		LLONG				_DingZhu;						

		//当前总下注;
		LLONG				_ZongXiaZhu;	

		//当前个人总下注;
		LLONG				_MyZongXiaZhu;

		//玩家下注数据
		LLONG				_XiaZhuData[PLAY_COUNT];

		// 游戏状态
		BYTE                _gameStation;

		// 思考时间（准备）
		BYTE                _thinkTime;

		// 开始时间
		BYTE                _beginTime;

		// 同意状态
		bool                _agree[PLAY_COUNT];

		// 用户牌张数
		BYTE                _userCardCount[PLAY_COUNT];

		// 用户牌数据
		BYTE                _userCard[PLAY_COUNT][MAX_CARD_COUNT];
		
		// 当前操作的玩家
		BYTE		        _CurrHandleSeatNo;

		// 可否看牌
		bool		        _CanLook;

		// 可否跟注
		bool		        _CanFollow;		

		// 可否加注
		bool		        _CanAdd[E_CHOUMA_COUNT];

		// 可否比牌
		bool		        _CanOpen;	

		// 可否弃牌
		bool		        _CanGiveUp;			

		public:
			std::string _userName[PLAY_COUNT];                                     //玩家昵称
		public:
			//记录是否为比赛场准备
			bool _isReadyQueue;
			// 是否比赛场
			bool	_bContestRoom;
			//是否比赛结束 
			bool	_bContestEnd;

	};
}



#endif