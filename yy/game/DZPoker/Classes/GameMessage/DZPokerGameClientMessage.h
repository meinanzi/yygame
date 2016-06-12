#ifndef __DZPokerGameClientMessage_h__
#define __DZPokerGameClientMessage_h__

#include "DZPokerUpgradeMessage.h"
#include "HNBaseType.h"

namespace DZPoker
{
	/************************************************************************/
	/* 客户端定义的消息结构，仅提供客户端使用                                  */
	/************************************************************************/

	
	// 发牌玩家信息
	struct THandCard
	{
		BYTE byUser;
		BYTE byCards[2];

		THandCard()
		{
			byUser = INVALID_DESKNO;
			memset(byCards, 0x00, sizeof(byCards));
		}
	};

	struct TableInfo
	{
		INT					iOffsetStation;						// 位置偏移量
		BYTE				bWaitTime;							// 等待时间
		BYTE				bGameStatus;						// 游戏状态
		BYTE				byMeStation;						// 自己的位置
		BYTE				byNTUser;							// 庄家位置
		BYTE				bySmallBlind;						// 小盲注位置
		BYTE				byBigBlind;							// 大盲注位置
		emTypeCard			nTypeCard;							// 发牌类型
		BYTE				byHandCard[PLAY_COUNT][2];			// 底牌数据
		INT					iHandCardCounts;					// 底牌数
		BYTE				byCommunityCard[MAX_HANDCARDS_NUM];	// 保存底牌
		INT					iCommunityCardCounts;
		TBetPool			tagBetPool;							// 下注边池
		BYTE				bEableWatch;						// 是否允许旁观
		bool				bWatch;								// 玩家是不是旁观者
		TDeskCfg			tagDeskCfg;							// 牌桌配置信息
		bool				bHaveUser[PLAY_COUNT];				// 玩家为真
		LLONG				iMoneys[PLAY_COUNT];				// 记录每个玩家代入金额
		bool				bAutoStartTimer;					// 自动开始时间锁
		bool				bIsOffline;							// 是否为断线重回玩家
		BYTE				byTokenUser;						// 得到令牌的玩家
		INT					iUserStation;						// 是否是玩家(用法不定)
		bool				bAllIn[PLAY_COUNT];					// 全下玩家
		bool				bFold[PLAY_COUNT];					// 弃牌玩家
		LLONG				nBetMoney[PLAY_COUNT];				// 每个玩家下注金币 
		bool				bSoundPlay;							// 音乐开关
		emType				emLastOperation[PLAY_COUNT];		// 记录每个玩家操作类型
		std::stack<BYTE>	stackLastStation;					// 记录上一个操作玩家
		LLONG				nCurrentMoney;						// 当前下注金币
		bool				bTrusteeship[4];					// 托管标志 0: 自动跟注(只限一轮)  1: 自动跟任何注(每轮都跟注)  2: 自动过牌  3: 自动弃牌(上家有人过牌就过牌，没人过牌就弃牌)
		LLONG				nCallMoney;							// 跟注金币
		TResult				tagResult;							// 结算信息
		bool				bTuoGuan;							// 是否处理托管状态
		BYTE				byBackCard[MAX_BACKCARDS_NUM];		// 底牌数据
		INT					iBackCardNums;						// 底牌数量
		BYTE				bDeskIndex;							// 桌子的编号
		bool				bAutoCreate;						// 是否自动创建桌子
	};

	// 用户信息
	struct UserInfoForGame
	{
		CHAR			szNickName[61];							// 昵称
		LLONG			i64Money;								// 用户金币
		UINT     		dwUserID;								// ID 号码
		UINT			dwUserIP;								// 登录IP地址
		UINT			bLogoID;								// 头像 ID 号码
		bool			bBoy;									// 性别
		bool            bIsRobot;								// 是否机器人
		BYTE			bGameMaster;							// 管理等级
		BYTE			bDeskNO;								// 游戏桌号
		BYTE			bDeskStation;							// 桌子位置
		CHAR			szCity[61];								// 所在城市
	};

}


#endif // __GameClientMessage_h__
