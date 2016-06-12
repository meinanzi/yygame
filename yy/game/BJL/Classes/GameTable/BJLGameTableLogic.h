#ifndef __BJLGameTableLogic_h__
#define __BJLGameTableLogic_h__

#include "BJLMessageHead.h"
#include "BJLGameTableUICallback.h"
#include "HNNetExport.h"
#include "HNLogicExport.h"

using namespace std;

namespace BJL
{
	class GameTableLogic : public HNGameLogicBase
	{
	public:
		// 构造函数
		GameTableLogic(GameTableUICallback* uiCallback, BYTE deskNo, bool autoCreate);

		// 析构函数
		virtual ~GameTableLogic();

	public:
		// 请求站起
		void requestStandUp();
		
		// 请求下注
		void requestBet(int betType, int betArea);

		// 请求上庄、下庄
		void requestBeNT(bool isNT);

		// 申请断线重连，或者刚刚进入房间时申请恢复桌子数据包
		void requestGameData();

		// 请求庄家列表
		vector<string> requestNTList();

		// 更新路子
		std::vector<LuziData> getRecords();

		// 获取区域赢
		int* getWinArea();

		// 获取空闲时间
		int getFreeTime();

		// 获取游戏状态
		BYTE getGameStatus();

		// 是否是庄家或者在申请庄家列表里
		bool isInNTList(BYTE seatNo);

	public:
		// 发送上、下庄消息
		void sendBeNT(bool isNT);

		// 发送下注消息
		void sendBet(BYTE deskNo, int betType, int betArea);

	public:
		// 同意开始
		virtual void dealUserAgreeResp(MSG_GR_R_UserAgree* agree) override;

		// 游戏开始
		virtual void dealGameStartResp(BYTE bDeskNO) override;

		// 游戏结束
		virtual void dealGameEndResp(BYTE bDeskNO) override;

		// 玩家坐下
		virtual void dealUserSitResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) override;

		// 玩家站起
		virtual void dealUserUpResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) override;

		// 游戏信息
		virtual void dealGameInfoResp(MSG_GM_S_GameInfo* pGameInfo) override;

		// 游戏状态
		virtual void dealGameStationResp(void* object, INT objectSize) override;

		// 游戏消息
		virtual void dealGameMessage(NetMessageHead* messageHead, void* object, INT objectSize) override;

	public:
		// 申请上庄
		void dealApplyNTResp(void* object, INT objectSize);

		// 无庄等待
		void dealNoNTWaitResp(void* object, INT objectSize);

		// 游戏开始下注
		void dealNoticeBetResp(void* object, INT objectSize);

		// 游戏下注结果
		void dealBetResultResp(void* object, INT objectSize);

		// 开始发牌
		void dealSendCardResp(void* object, INT objectSize);

		// 显示中奖区域
		void dealShowWinAreaResp(void* object, INT objectSize);

		// 游戏结束
		void dealGameEndResp(void* object, INT objectSize);
	
	private:
		// 界面回调接口
		GameTableUICallback* _uiCallback;

		// 庄家座位号
		BYTE _NTSeatNo;

		// 庄家金币
		LLONG _NTMoney;

		// 庄家赢的金币
		LLONG _NTWinMoney;

		// 庄家把数
		LLONG _NTPlayCount;

		// 上庄列表
		BYTE  _NTList[PLAY_COUNT];	            

		// 玩家金币
		LLONG _userMoney;

		// 玩家输赢
		LLONG _userWin;

		// 玩家总得分
		LLONG _userTotalScore;

		// 玩家区域得分
		LLONG _userAreaScore[BET_ARES];

		// 玩家命中率
		float _userPercent;

		// 庄家最少金币
		LLONG _NTMinLimit;

		// 最大下注数量
		LLONG _maxBet;

		// 游戏状态
		BYTE _gameStatus;

		// 下注玩家
		BYTE _betSeatNo;
		
		// 下注区域
		int	_betArea;

		// 下注数量
		LLONG _betMoney;

		// 本把当前总注额
		LLONG _roundTotaBetMoney;
		
		// 真实玩家的下注值
		LLONG _areaBet[BET_ARES];

		// 本把每个区域下的注额
		LLONG _roundAreaBet[BET_ARES];

		// 玩家下注情况
		LLONG _userAreaBet[BET_ARES];
		
		// 每个区域还能下多少注
		LLONG _areaMaxBet[BET_ARES];

		// 玩家的牌,0为闲，1为庄
		BYTE _userCard[2][3];

		// 庄家牌型,元素0前两张牌的值，元素1总牌值，元素2天王，元素3对子，元素4和
		int _NTCardType[5];				

		// 闲家牌型
		int _playerCardType[5];				
		
		// 游戏的赢钱区域 值=0 表示没中奖  大于0的时候 表示中奖了  0闲，1闲天王，2闲对子，3庄，4庄天王，5庄对子，6和，7同点和
		int _winArea[BET_ARES];

		// 路子信息
		LuziData _records[MAXCOUNT];

		// 下注时间
		int _betTime;

		// 开牌时间
		int _openTime;

		// 空闲时间
		int _freeTime;

		// 显示开奖时间
		int _showWinTime;
	};
}

#endif // __BJLGameTableLogic_h__
