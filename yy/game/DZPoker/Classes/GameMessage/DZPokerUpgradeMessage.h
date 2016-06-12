// ******************************************************************************************************
//  UpgradeMessage   version:  1.0   ·  date: 03/29/2011
//  --------------------------------------------------------------------------------------------------

//	德克萨斯扑克，游戏服务器-客户端协议\
//  NameID：10100008

//  --------------------------------------------------------------------------------------------------
//  Copyright (C) 2011 - All Rights Reserved
// ******************************************************************************************************
// 
// ******************************************************************************************************
#ifndef __DZPokerUpgradeMessage_h__
#define __DZPokerUpgradeMessage_h__

#include <cstring>
#include "HNBaseType.h"

namespace DZPoker
{

	enum DZPoker_COMMAND
	{
		UD_VF_CALL				=	0x01,  //可以跟注
		UD_VF_RAISE				=	0x02,  //可以加注
		UD_VF_BET				=	0x04,  //可以下注
		UD_VF_FOLD				=	0x08,  //可以弃牌
		UD_VF_CHECK				=	0x10,  //可以过牌
		UD_VF_ALLIN				=	0x20,  //可以全下
		UD_VF_CHECKMANDATE		=	0x40,  //可以过牌托管
		UD_VF_CALLMANDATE		=	0x80 , //可以跟注托管
		UD_VF_CALLANYMANDATE	=	0x100, //可以跟任何注托管
		UD_VF_PASSABANDONMANDATE =	0x200, //可以过牌/弃牌托管

		MAX_ZORDER				=	500,
	};

	enum DZPoker_COMMAND1
	{
		//游戏信息配置
		//////////////////////////////////////////////////////////////////////////
		//文件名字定义
		GAME_NAME_ID			=		10100008	,	// 名字 ID

		PLAY_COUNT				=		8			,	// 游戏人数

		PUBLIC_CARD_COUNT		=		5			,	// 公共牌数
	
		MAX_NAME_INFO          =         256 ,

		//游戏数据长度相关
		MAX_CARDS_NUM			=		216		,		// 四副牌的总牌数，也是所支持最大的牌数
		MAX_HANDCARDS_NUM		=		52		,		// 手上持有的最大的牌张数
		MAX_510K_CARDS_NUM		=		48		,		// 四副牌5/10/K最大的牌张数
		MAX_FIRST_CARDS			=		1		,		// 发牌数量
		MAX_DEAL_CARDS			=		2		,		// 发牌数量
		MAX_THREE_CARDS			=		3		,		// 发牌数量
		MAX_BACKCARDS_NUM		=		16		,		// 最大的底牌张数

		TYPE_PLAY_GAME			=		10000	,		// 是玩家
		SUPER_PLAYER            =       10      ,		// 超级玩家
		//******************************************************************************************
		//数据包处理辅助标识
		//******************************************************************************************

		ASS_UG_USER_SET				=	50		,		//用户设置游戏
		ASS_UG_USER_SET_CHANGE		=	51	,			//玩家设置更改

		ASS_GAME_BEGIN				=	120		,		//升级游戏开始
		ASS_SYS_DESK_CFG			=	121		,		//确定定注结果消息
		ASS_CFG_DESK				=	122		,		//玩家设置底注
		ASS_DESK_CFG				=	123		,		//确定定注结果消息
		ASS_AGREE_DESK_CFG			=	124    ,         //同意底注
		ASS_BET						=	125    ,         //玩家下注信息
		ASS_SEND_A_CARD				=	130		,		//发1张牌信息
		ASS_SEND_3_CARD				=	131		,		//发3张牌信息
		ASS_SEND_4_5_CARD			=	132		,		//发4,5张牌信息
		ASS_SEND_CARD_FINISH		=	133		,		//发牌结束
		ASS_BETPOOL_UP				=	134		,		//边池更新
		ASS_BET_REQ					=	135		,		//下注请求
		ASS_BET_INFO				=	136		,		//下注消息
		ASS_NEW_USER				=	137		,		//下一个玩家操作
		ASS_PASS_USER				=	138		,		//弃牌的玩家操作
		ASS_SUB_MONEY                =   139   ,          //代入金币限制
		ASS_COMPARE_CARD              =  140  ,           //比牌信息
	
		ASS_CALLMANDATE				 =   141	,			//跟注托管
		ASS_CHECKMANDATE             =   142    ,         //过牌托管
		ASS_CALLANYMANDATE           =   143	,			//跟任何注托管
		ASS_PASSABANDONMANDATE      =    144    ,         //过牌或弃牌托管

		ASS_USER_LEFT_DESK         =     145    ,         //玩家离开牌桌

		ASS_SORT_OUT				=	170		,		//清理图片

		ASS_TOKEN					=	186		,		//令牌信息
		ASS_RESULT					=	191		,		//回合算分
		ASS_SHOW_IN_MONEY			=	192		,		//显示代入金额
		ASS_LOOK_CARD				=	193		,		//看牌
		ASS_CAN_SHOW_CARD			=	194		,		//能展示牌的消息
		ASS_SHOW_CARD				=	195		,		//展示牌的消息
	

		//异常类消息
		ASS_NO_PLAYER				=	155		,		//没有玩家进行游戏
		ASS_AGREE					=	156		,		//玩家是否同意的消息
		ASS_CFG_DESK_TIMEOUT		=	157		,		//设置底注超时
		ASS_NOT_ENOUGH_MONEY		=	158		,		//玩家金币不足的清除玩家信息
		ASS_MONEY_LACK				=	159		,		//玩家金币不足的提示
	
		ASS_NEXT_ROUND_REQ			=	150		,		//玩家下一回合请求
		ASS_NEW_ROUND_BEGIN   		=	151		,		//下一回合开始

		//调试类消息
		ASS_FINISH_ROUND_REQ		=	180		,		//结束本轮游戏
		S_C_IS_SUPER_SIG            =   181     ,		//是否是超端
	};
	
	#define TAG_INIT(tagName) \
		tagName() { Clear();} \
		void Clear()

	//******************************************************************************************
	// 游戏状态定义
	//******************************************************************************************
	enum
	{
		GS_WAIT_SETGAME			=		0,	//等待东家设置状态
		GS_WAIT_ARGEE			=		1,	//等待同意设置
		GS_CONFIG_NOTE			=		20, //设置底注状态
		GS_AGREE_NOTE			=		21,	//同意底注状态
		GS_PLAY_GAME			=		25,	//游戏过程状态
		GS_WAIT_NEXT_ROUND		=		28,	//等待下一回合结束状态
	};


	/******************************************************************************************
	*									德克萨斯扑克状态定义									  *
	*******************************************************************************************/

	#define GS_USER_STAKE				30	// 玩家下注额状态

	//******************************************************************************************
	// 下注类型
	enum emType
	{
		ET_UNKNOWN 		= 0,	//未知类型
		ET_BET			= 1,	//下注
		ET_CALL			= 2,	//跟注
		ET_RAISE 		= 3,	//加注
		ET_CHECK		= 4,	//过牌
		ET_FOLD    		= 5,    //弃牌
		ET_ALLIN		= 6,	//全下
		ET_AUTO			= 7,    //自动下注
	};

	// 发牌类型
	enum emTypeCard
	{
		UNKNOWN			= 0,	//未知类型
		SEND_A_CAND		= 1,	//下发一张牌牌
		SEND_3_CAND		= 2,	//下发 3 张牌牌
		SEND_4_5_CAND   = 3,	//下发 4, 5 张牌牌
	};

	// 代入金额类型
	enum emTypeMoney
	{
		NT_UNKNOWN	= 0,	//未知类型
		NT_LOAD		= 1,	//确定代入金额
		NT_EXIT		= 2,    //退出游戏
		NT_ADD		= 3,    //增加金额
		NT_SUB		= 4,    //减少金额
	};

	// 托管状态
	enum emToken
	{
		TO_UNKNOWN					= 0,	// 未知类型
		TO_CALLMANDATE				= 1,	// 跟注托管按钮
		TO_CHECKMANDATE				= 2,    // 过牌托管按钮
		TO_CALLANYMANDATE			= 3,    // 跟任何注按钮
		TO_PASSABANDONMANDAT		= 4,    // 过牌/弃牌托管按钮
	};

	#pragma pack(1)

	//通知设置游戏包
	typedef struct tagTCfgDesk
	{
		//底注相关
		struct TDizhu
		{
			bool	bCanSet;		//玩家可以是否可以设置，下发时用
			INT		iLowers[5];		//存放5个可选的底注值，下发时用
			bool    bSelects[5];	//这5个值的可选择性
			BYTE	bySelect;		//玩家从iValues选择了哪一个底注值，上发时用

			void Clear()
			{
				bCanSet = false;	//玩家可以是否可以设置
				memset(iLowers,0,sizeof(iLowers));		 //存放三个可选的底注值
				memset(bSelects,false,sizeof(bSelects)); //这三个值的可选择性
				bySelect = 255;	//玩家从iValues选择了哪一个底注值
			}

		} dz;

		TAG_INIT(tagTCfgDesk)
		{
			dz.Clear();
		}
	} TCfgDesk;

	//有关桌面配置
	typedef struct tagTDeskCfg
	{
		//底注相关
		struct TDizhu
		{
			LLONG	iLowers[5];						// 存放5个可选的底注值
			LLONG	iUppers[5];						// 存放5个顶注值
			LLONG	iRoomMultiple;                  // 房间倍数
			LLONG	iLower;							// 系统自动设置的底注值
			LLONG	iUpper;							// 系统自动设置的顶注值
			LLONG	iSmallBlindNote;				// 小盲注设置的底注值
			LLONG	iBigBlindNote;					// 大盲注设置的底注值

			// 代入金币设置
			bool    bIsRoomSubMoney;                // 此房间是否需要代入金币才可玩
			LLONG	iSubMinMoney;                   // 本房间最低代入金币
			LLONG	iSubMaxMoney;                   // 本房间最高代入金币 
			LLONG	iSubPlanMoney;					// 当前准备代入金币
			LLONG	iMinusMoney;					// 修改注额加减金币
			LLONG	iMinMoney;						// 最小下注金币
			LLONG	iUntenMoney;					// 少于多少金额时带入 

			bool iUserClose;						// 一局后10秒内将玩家强退
			INT iUserCloseTime;						// 关闭客户端时间
			INT iUserInMoney;						// 游戏金额带入时间

			emTypeMoney nType;						// 请求包类型

			void Clear()
			{
				memset(iLowers, 0, sizeof(iLowers));	// 存放5个可选的底注值
				memset(iUppers, 0, sizeof(iUppers));	// 存放5个顶注值
				iLower = 0;							// 系统自动设置的底注值
				iUpper = 0;							// 系统自动设置的顶注值
				iSmallBlindNote = 0;				// 小盲注设置的底注值
				iBigBlindNote = 0;					// 大盲注设置的底注值
				nType = NT_UNKNOWN;					// 未知类型
			}

		} dz;

		//时间相关
		struct TTime
		{
			BYTE   byOperate;    	 // 玩家操作的时间

			void Clear()
			{
				byOperate = 0;	 	 // 玩家操作的时间
			}
		} Time;

		//规则相关
		 struct TRule
		{
			INT  nPokeNum;			//使用多少幅扑克
			void Clear()
			{
				nPokeNum = 1;		//使用多少幅扑克
			}

		} Rule;

		//其它共性
		bool bShowUserInfo;			// 是否显示玩家和身份，多用于比赛场

		TAG_INIT(tagTDeskCfg)
		{
			dz.Clear();
			Time.Clear();
			Rule.Clear();
		}
	} TDeskCfg;

	//某玩家是否意底注的消息包
	typedef struct tagTAgree 
	{
		BYTE byUser; 				// 消息所属的玩家
		bool bAgree; 				// 是否同意
		bool bAllAgree; 			// 当前是否所有玩家都同意游戏

		TAG_INIT(tagTAgree)
		{
			byUser = 255;			// 消息所属的玩家
			bAgree = true;			// 是否同意
			bAllAgree = false;		// 当前是否所有玩家都同意游戏
		}
	} TAgree;

	//请求下注包
	typedef struct tagTBet
	{
		emType	nType;				// 下注类型
		BYTE	byUser;				// 下注的玩家
		BYTE	bUserman[8];			// 弃牌玩家
		LLONG	nMoney;				// 金币
		LLONG	nBetMoney;			// 下注最大的玩家

		TAG_INIT(tagTBet)
		{ 
			nType = ET_UNKNOWN;		// 下注类型
			byUser = 255;			// 下注的玩家
			//bUserman = 255;		// 弃牌玩家
			memset(bUserman, 0, sizeof(bUserman));
			nMoney = 0;				// 玩家的选择
			nBetMoney = 0;			// 下注最大的玩家
		}
	} TBet;

	// 代入金币
	typedef struct tagTSubMoney
	{
		BYTE	bDeskStation;
		LLONG	nMoney[PLAY_COUNT];
		bool	bReady[PLAY_COUNT];       // 准备状态

		tagTSubMoney()
		{
			memset(nMoney, 0, sizeof(nMoney));
			memset(bReady, 0, sizeof(bReady));
			bDeskStation = 255;
		}
	} TSubMoney;


	// 比牌数据包
	typedef struct tagTCompareCard
	{
		INT  nCardKind[PLAY_COUNT];		 // 玩家牌型
		BYTE bCards[PLAY_COUNT][5];		 // 所有玩家组成的牌数据
		BYTE bCardsEx[PLAY_COUNT][2];    // 剩下2张没用的牌
		BYTE bHandCards[PLAY_COUNT][2];  // 玩家手牌
		INT nCardCount;                  // 组合牌张数
		INT nHandCardCount[PLAY_COUNT];  // 玩家手牌张数

		tagTCompareCard()
		{
			memset(this, 0, sizeof(tagTCompareCard));
			memset(bHandCards, 255, sizeof(bHandCards));
		}
	} TCompareCard;

	//用于下发牌数据给客户端的数据包
	typedef struct tagTCards
	{
		emTypeCard		nTypeCard;					//发牌类型
		BYTE   			byUser;						//该牌发给的玩家
		BYTE			byCards[PLAY_COUNT][MAX_DEAL_CARDS];	//牌数据
		BYTE			byPubCards[5];				//公共牌牌数据
		INT				iCardsNum;					//各玩家手上牌的数量

		TAG_INIT(tagTCards)
		{
			nTypeCard	= UNKNOWN;					//发牌类型
			byUser		= 255;						//该牌发给的玩家
			memset(byCards,0,sizeof(byCards));		//牌数据
			iCardsNum = 0;							//玩家手上牌的数量
		}
	} TCards;

	//令牌包，用来激活用户进行活动，如出牌等等
	typedef struct tagTToken
	{
		BYTE	  byUser;									// 所属玩家
		BYTE	  byVerbFlag;								// 允许动作标志
		INT		  iSelPoints[5];							// 加注值选择表
		emToken	  nemToken;									// 托管状态类型
		bool	  bNewTurn;                                 // 是否为新的一轮开始
		bool	  bNewTurns;                                // 是否为新的一轮开始
		INT		  nTurnNums;								// 当前游戏活动的圈数
		LLONG   nCallMoney;                               // 当前可以跟注的金币
		INT       iPassedTime;                               //已经过了的时间（操作）

		TAG_INIT(tagTToken)
		{
			nemToken = TO_UNKNOWN;						// 未知类型
			byUser = 255;								// 所属玩家
			byVerbFlag = 0;								// 允许动作标志
			bNewTurn = false;
			bNewTurns = false;
			nCallMoney = 0;
			nTurnNums = 0;
			::memset(iSelPoints,0,sizeof(iSelPoints));	// 加注值选择表
			iPassedTime = 0;
		}
	} TToken;

	// 玩家下注边池更新包
	typedef struct tagTBetPool
	{
		LLONG iBetPools[8];			//下注边池

		TAG_INIT(tagTBetPool)
		{
			::memset(iBetPools, 0, sizeof(iBetPools));  //边池初始化
		}
	} TBetPool;

	//结算结构包
	typedef struct tagTResult
	{
		INT		nbyUserID[PLAY_COUNT];				//赢家ID
		CHAR	szName[8][61];						//赢家昵称
		LLONG	nBetPools[8];					//所有下注池
		LLONG	nUserBet[PLAY_COUNT][8];		//每个玩家下注数据
		bool	bWinBetPools[PLAY_COUNT][8];		//赢的下注池
		LLONG	nWinPoolsMoney[PLAY_COUNT][8];	//赢的下注池金币
		LLONG	nScore[PLAY_COUNT];     		//输赢积分
		LLONG	nMoney[PLAY_COUNT];     		//实际输赢金币
		LLONG	nMoneyEx[PLAY_COUNT];          //输赢金币 
		LLONG	nSubMoney[PLAY_COUNT];  		//当前每个玩家代入金币
		LLONG	nSelfMoney[PLAY_COUNT]; 		//自己的金币更新: 客户端获取的金币有可能还没有刷新, 所以在这里发下去
		bool	bWin[PLAY_COUNT];       			//记录赢家
		bool	bExit;                  			//true:玩家需要退出, false:不需要退出
		BYTE	bOrder[PLAY_COUNT];                //玩家排名, 按金币最多的排

		tagTResult()
		{
			::memset(this, 0, sizeof(tagTResult));
			::memset(nUserBet, 0, sizeof(nUserBet));
			::memset(nMoneyEx, 0, sizeof(nMoneyEx));
			::memset(bOrder, 255, sizeof(bOrder)); 
		}
	} TResult;

	//新的一回合请求包
	typedef struct tagTNextRoundReq
	{
		enum emType
		{
			UNKNOWN		= 0,	//未知类型
			READY		= 1,	//准备好
			EXIT		= 2,    //想退出游戏
		};

		emType nType;			//请求包类型

		TAG_INIT(tagTNextRoundReq)
		{
			nType = UNKNOWN;	//未知类型
		}
	} TNextRoundReq;

	//新的一回合消息包
	typedef struct tagTNextRoundBeginInfo
	{
		BYTE byNTUser;		//庄家位置
		BYTE bSmallBlind;	//小盲注
		BYTE bBigBlind;		//大盲注

		TAG_INIT(tagTNextRoundBeginInfo)
		{
			byNTUser = 255;
			bSmallBlind = 255;
			bBigBlind = 255;
		}
	} TNextRoundBeginInfo;

	//没有玩家在桌的通知
	typedef struct tagTNoPlayer
	{
		bool bGameFinished;	//桌子已散掉

		TAG_INIT(tagTNoPlayer)
		{
			bGameFinished = false;	//桌子已散掉
		}
	} TNoPlayer;

	// 玩家金额不足
	typedef struct tagTMoneyLack
	{
		INT nbyUser;			// 金额不足玩家

		TAG_INIT(tagTMoneyLack)
		{
			nbyUser = 255;
		}
	} TMoneyLack;

	/******************************************************************************************
	*																						  *
	*******************************************************************************************/
	//游戏状态包的基本
	typedef struct tagTGSBase 
	{
		/*-GSBase基本属性-*/
		INT  iEvPassTime;			//当前事件已消耗的时间（秒）
		INT	 iVerbTime;				//游戏中各动作的思考时间
		bool bBoy[PLAY_COUNT];  	//玩家性别
		bool bHaveUser[PLAY_COUNT]; //桌上各位置有没有玩家
		BYTE bGameStatus;			//游戏状态

		/*-桌面基本配置-*/
		tagTDeskCfg tagDeskCfg;

		TAG_INIT(tagTGSBase)
		{
			::memset(this, 0, sizeof(tagTGSBase));
		}
	} TGSBase;

	//游戏状态包：等待同意/未开始时
	typedef struct tagTGSWaitAgree : public TGSBase 
	{
		bool	bReady[PLAY_COUNT];               //是否准备
		LLONG	nSubMoney[PLAY_COUNT];            //每个玩家代入的金币

		TAG_INIT(tagTGSWaitAgree)
		{
			::memset(this, 0, sizeof(tagTGSWaitAgree));
		}
	} TGSWaitAgree;

	//游戏状态包：游戏中
	typedef struct tagTGSPlaying : public TGSBase 
	{
		BYTE  byHandCard[PLAY_COUNT][2];	    //玩家手上的扑克
		INT   nHandCardNums;		            //玩家手上扑克数目

		BYTE  byCards[5];                       //桌面上公共牌
		INT   nCardsCount;                      //桌面公共牌数
		INT   nID[5];							//五个金额按钮
		LLONG   nBetMoney[PLAY_COUNT][8];     //下注金币
		bool  bNoBet[PLAY_COUNT];               //记录不可下注的玩家位置
		bool  bGiveUp[PLAY_COUNT];              //记录放弃的玩家位置

		BYTE  byTokenUser;		                //得到令牌的用户
		BYTE  byNTUser;					        //庄家
		BYTE  bySmallBlind;                     //小盲家
		BYTE  byBigBlind;                       //大肓家
		LLONG	  nCallMoney;					//跟注金币

		BYTE  byVerbFlag;                       //操作属性
		bool  bNewTurn;                         //新的一轮操作
		bool  bNewTurns;                        //新的一轮操作

		LLONG   nBetPools[8];			        //下注边池金币
		LLONG   nSubMoney[PLAY_COUNT];        //每个玩家代入的金币

		INT   iUserStation ;

		TAG_INIT(tagTGSPlaying)
		{
			::memset(this,0,sizeof(tagTGSPlaying));
		}
	} TGSPlaying;


	//游戏状态包：等待下一回合
	typedef struct tagTGSWaitNextRound : public TGSBase 
	{
		LLONG   nSubMoney[PLAY_COUNT];            //每个玩家代入的金币

		TAG_INIT(tagTGSWaitNextRound)
		{
			::memset(this, 0, sizeof(tagTGSWaitNextRound));
		}
	} TGSWaitNextRound;


	typedef struct tagUserLeftDeskStruct
	{
		BYTE    bDeskStation; 
		bool    bClearLogo ;
	} UserLeftDeskStruct;


	//看牌
	typedef struct tagUserLookCardStruct
	{
		BYTE    bDeskStation; 

		tagUserLookCardStruct()
		{
			bDeskStation = 255;
		}
	} UserLookCardStruct;

	//展示牌
	typedef struct tagShowCardStruct
	{
		BYTE	byDeskStation;
		BYTE    byCardList[2]; 
		INT		iCardCount;

		tagShowCardStruct()
		{
			::memset(this,0,sizeof(tagShowCardStruct));
			byDeskStation = 255;
		}
	} ShowCardStruct;

	#pragma pack()
}

#endif // __DZPokerUpgradeMessage_h__
