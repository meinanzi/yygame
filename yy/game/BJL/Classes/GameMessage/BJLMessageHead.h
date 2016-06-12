#ifndef __BJLMessageHead_h__
#define __BJLMessageHead_h__

#include <memory.h>
#include "HNNetExport.h"

namespace BJL 
{
	#pragma pack(1)
	
	enum BJL_COMMAND
	{
		//游戏信息
		GAME_NAME_ID			    =	11100200,   // 名字 ID
		PLAY_COUNT					=	180,	    // 游戏人数

		//游戏内部开发版本号
		DEV_HIGH_VERSION			=	2,			// 内部开发高版本号(每修正一次LOW_VERSIONG+1)
		DEV_LOW_VERSION				=	0,			// 内部开发低版本号

		// 游戏状态定义
		GS_WAIT_SETGAME				=	0,			// 无庄等待
		GS_WAIT_AGREE				=	1,			// 等待同意设置
		GS_NOTE_STATE				=	20,			// 下注状态
		GS_SEND_CARD				=	21,			// 发牌状态
		GS_SHOW_WIN					=	22,			// 显示中奖区域
		GS_WAIT_NEXT				=	23,			// 等待下一盘开始


		NO_WAITING                  =   0,          // 不处于等待状态
		WAITING_USER_BEGIN          =   1,          // 等待其他玩家开始

		ID_VIEW_CLOCKWISE           =   1,           // 顺时针
		ID_VIEW_ANTICLOCKWISE       =   0,           // 逆时针

		USER_HAND_CARD_COUNT        =   27,         // 用户手中的牌
		HAVE_THING_LENGHT           =   61,         // 有事要走的消息长度
		USER_AGREE_LEAVE            =    1,         // 同意玩家离开
		USER_DISAGREE_LEAVE         =    0,         // 不同意玩家离开

		ONE_SECOND                  =    1000,      // 1 秒时间
		MAXCOUNT				    =    50, 

		ASS_SUPER_SET_CARD          =    79,       // 提牌器消息id，平台决定的游戏不能修改

		/// 消息ID
		ASS_UPDATE_CLIENT_VIEW   =   100,     // 通知更新客户端界面  
		ASS_SEND_CARD            =   101,     // 发牌
		ASS_GAME_BEGIN           =   102,     // 发完牌游戏开始
		ASS_CUT_END			     =	 103,	  // 用户强行离开
		ASS_SAFE_END			 =   104,	  // 用户安全结束

		ASS_TRUSTEE              =   110,     // 托管
		ASS_HAVE_THING           =   120,     // 有事要走消息
		ASS_USER_AGREE_LEAVE     =   121,     // 有事要走消息答复结果
		ASS_ALL_USER_AGREEED     =   122,     // 有事要走消息所有人答复完
		ASS_WATCH_SET_CHANGE     =   123,     // 玩家修改旁观属性		
		ASS_XIA_ZHU              =   130,     // 下注消息		
		ASS_KAI_PAI			     =	 131,     // 开牌消息
		ASS_DENG_DAI             =   132,     // 等待消息
		ASS_SHANG_ZHUANG         =   133,     // 上庄消息
		ASS_BJL_BEGIN            =   134,     // 百家乐开始消息
		ASS_WU_ZHUANG            =   135,     // 游戏无庄闲置消息消息
		ASS_SHOW_JS			     =	 136,     // 通知显示结算信息消息
		ASS_APPLY_GAME_DATA	     =   137,     // 断线重连时申请游戏数据消息
		ASS_USER_SCORE           =   138,     // 玩家得分信息
		ASS_BJL_TEST             =   140,     // 测试消息

		ASS_SUPER_SETTING        =   150,     // 由客户端发送消息控制本局输赢
		ASS_SUPER_STATE          =   151,     // 通知客户端是否为超级状态
		ASS_SUPER_AI_INFO        =   152,     // 机器人输赢控制 ： 通知客户端机器人当前的信息

		ASS_XIA_ZHU_FULL		 =   153,     // 下注已满消息 
		ASS_CHANGE_NT			 =   154,     // 换庄消息 

		C_S_APPLY_ZHUANG		=	 90,      // 申请上下庄
		S_C_APPLY_ZHUANG_RESULT	=	 91,      // 申请上下庄结果
		C_S_XIA_ZHU				=	 92,      // 玩家下注
		S_C_XIA_ZHU_RESULT		=	 93,      // 玩家下注结果
		
		S_C_NO_NT_WAITE			=	 100,	  // 通知无庄等待
		S_C_NOTICE_XIA_ZHU		=	 101,	  // 通知下注消息
		S_C_SEND_CARD			=	 102,	  // 发牌消息
		S_C_SHOW_WINAREA		=	 104,	  // 显示中奖区域消息
		S_C_GAME_END			=	 105,	  // 游戏结束

	};
	
	//8个下注区域
	const	int	BET_ARES = 8;	
	
	//7个筹码值
	const	LLONG	G_i64ChouMaValues[7] = {100,1000,10000,100000,1000000,5000000,10000000};
	
	struct LuziData
	{
		BYTE		byZPoint;
		BYTE		byXpoint;
		BYTE		byWinResult;	//赢的区域 0=闲 1-庄 2-和
		LuziData()
		{
			memset(this,255,sizeof(LuziData));
		}
	};
	
	//申请上庄数据包
	struct C_S_ApplyShangZhuang
	{
		bool	bShangZhuang;		//是否上庄 true -上庄  false - 下庄
		C_S_ApplyShangZhuang()
		{
			memset(this,0,sizeof(C_S_ApplyShangZhuang));
		}
	};

	//申请上庄数据包
	struct S_C_ApplyZhuangResult
	{	
		bool		bXiaZhuang;					// 是否庄家申请下庄(true表示下庄false表示上庄)
		BYTE		byDeskStation;				// 申请的玩家
		BYTE		byZhuangList[PLAY_COUNT];	// 上庄列表
		int			iNowNtStation;				// 庄家位置
		int			iZhuangBaShu;				// 庄家把数
		LLONG		i64NtMoney;					// 玩家金币数量
		LLONG		i64NtWin;					// 当前庄家赢的金币 

		S_C_ApplyZhuangResult()
		{
			memset(	byZhuangList,255,sizeof(byZhuangList));
			iNowNtStation = -1;
			iZhuangBaShu = 0;
			i64NtMoney = 0;
			i64NtWin = 0;
			bXiaZhuang = false;	
			byDeskStation = 255;
		}
	};
	
	
	/*--------------------------------------------------------------------*/
	//无庄等待消息
	struct	S_C_NoNtStation
	{
		int			iNowNtStation;	// 当前庄家的位置
		int			iNtPlayCount;	// 庄家坐庄次数
		LLONG		i64NtMoney;		// 庄家金币
		LLONG		i64NtWinMoney;	// 庄家输赢情况
		S_C_NoNtStation()
		{
			iNowNtStation = -1;	    // 当前庄家的位置
			iNtPlayCount = 0;	    // 庄家坐庄次数
			i64NtMoney	= 0;	    // 庄家金币
			i64NtWinMoney = 0;	    // 庄家输赢情况
		}
	};
	/*--------------------------------------------------------------------------*/

	///游戏开始信息
	struct S_C_GameBegin
	{
		BYTE		byZhuangList[PLAY_COUNT];	  // 上庄列表
		int			iGameCount;		              // 当前已经进行了几把
		int			iGameBeen;		              // 当前局数
		int			iNowNtStation;	              // 当前庄家的位置
		int			iNtPlayCount;	              // 庄家坐庄次数
		LLONG		i64NtMoney;		              // 庄家金币
		LLONG		i64NtWinMoney;	              // 庄家输赢情况
		LLONG		i64UserMoney;	              // 玩家自身的金币数量
		LLONG		i64MaxXiaZhu;	              // 最大下注总注
		LLONG	    i64AreaMaxZhu[BET_ARES];      // 每个区域能下的最大注	
		S_C_GameBegin()
		{
			memset(this, 0, sizeof(S_C_GameBegin));
			iNowNtStation=-1;
			memset(byZhuangList,255,sizeof(byZhuangList));
		}
	};
	/*--------------------------------------------------------------------*/


	struct	C_S_UserNote
	{
		BYTE	byDeskStation;
		int		iChouMaType;		//筹码类型
		int		iNoteArea;			//下注区域
		C_S_UserNote()
		{
			byDeskStation = -1;
			iChouMaType   = -1;
			iNoteArea     = -1;
		}
	};


	/*--------------------------------------------------------------------*/
	struct	C_S_UserNoteResult
	{
		BYTE		byDeskStation;					// 下注玩家
		int			iArea;
		LLONG		i64Money;
		LLONG		i64ZhongZhu;					// 本把当前总注额
		LLONG		i64QuYuZhuTrue[BET_ARES];		// 真实玩家的下注值
		LLONG		i64QuYuZhu[BET_ARES];			// 本把每个区域下的注额(包含真人和机器人)
		LLONG	    i64UserXiaZhuData[BET_ARES];	// 玩家下注情况
		LLONG		i64AreaMaxZhu[BET_ARES];		// 每个区域还能下多少注
		C_S_UserNoteResult()
		{
			memset(this,0,sizeof(C_S_UserNoteResult));
		}
	};
	/*--------------------------------------------------------------------*/
	
	//开牌数据包
	struct S_C_SendCard
	{
		BYTE	byUserCard[2][3];			//庄闲的牌，0为闲，1为庄
		int  	iZPaiXing[5];				//庄家牌型,元素0前两张牌的值，元素1总牌值，元素2天王，元素3对子，元素4和
		int     iXPaiXing[5];				//闲家牌型
		int     iWinQuYu[BET_ARES];			//游戏的赢钱区域 值=0 表示没中奖  大于0的时候 表示中奖了  0闲，1闲天王，2闲对子，3庄，4庄天王，5庄对子，6和，7同点和	

		S_C_SendCard()
		{
			memset(this, 0, sizeof(S_C_SendCard));
		}
	};
	/*--------------------------------------------------------------------------*/
	
	//中奖区域数据包
	struct S_C_ShowWinAreas
	{	
		int		iWinQuYu[BET_ARES];			/**< 游戏的赢钱区域 0闲，1闲天王，2闲对子，3庄，4庄天王，5庄对子，6和，7同点和*/
		S_C_ShowWinAreas()
		{
			memset(this,0,sizeof(S_C_ShowWinAreas));
		}
	};
	
	/*--------------------------------------------------------------------*/
	struct	S_C_GameResult
	{
		BYTE		byZhuangList[PLAY_COUNT];	 // 上庄列表
		float		fUserPercent;				 // 玩家命中率
		LLONG		i64UserAreaScore[BET_ARES];	 // 玩家各区域得分情况
		LLONG		i64NtScoreSum;				 // 庄家本局总得分
		LLONG		i64UserScoreSum;			 // 闲家本局得分
		LLONG		i64NtWin;			         // 庄家输赢成绩（累计统计）
		LLONG		i64NtMoney;			         // 庄家的金币数量
		LLONG		i64UserWin;			         // 玩家输赢成绩（累计统计）
		LLONG		i64UserMoney;		         // 玩家自身的金币
		LuziData	TLuziData[MAXCOUNT];	     // 路子信息
		S_C_GameResult()
		{
			memset(this,0,sizeof(S_C_GameResult));
			memset(TLuziData,255,sizeof(TLuziData));
		}
	};

	/*--------------------------------------------------------------------------*/
	
	//游戏基础数据
	struct	GameStationBase
	{
		BYTE		byZhuangList[PLAY_COUNT];  // 上庄列表	
		LuziData	TLuziData[MAXCOUNT];	   // 路子信息

		int			iXiaZhuTime;		       // 下注时间					
		int			iKaiPaiTime;     	       // 开牌时间    
		int			iFreeTime;			       // 空闲时间	
		int			iShowWinTime;		       // 显示中奖时间

		int			iNtStation;			       // 庄家位置
		int			iNtPlayCount;		       // 庄家坐庄次数
		LLONG		i64NtMoney;			       // 庄家金币
		LLONG		i64NtWinMoney;		       // 庄家输赢情况

		LLONG		i64UserWin;			       // 个人输赢
		LLONG		i64MyMoney;			       // 个人金币数 -从服务端发送过去

		LLONG		i64UserMaxNote;			   // 玩家最大下注数
		LLONG		i64ShangZhuangLimit;	   // 上庄需要的最少金币	
		GameStationBase()
		{
			memset(this, 0, sizeof(GameStationBase));
			memset(byZhuangList,255,sizeof(byZhuangList));
			memset(TLuziData,255,sizeof(TLuziData));
		}
	};

	/*--------------------------------------------------------------------------*/
	//下注状态数据
	struct	GameStation_Bet
	{
		BYTE		byZhuangList[PLAY_COUNT];	    // 上庄列表	
		LuziData	TLuziData[MAXCOUNT];	        // 路子信息
												    
		int			iXiaZhuTime;		            // 下注时间					
		int			iKaiPaiTime;     	            // 开牌时间    
		int			iFreeTime;			            // 空闲时间	
		int			iShowWinTime;		            // 显示中奖时间
												    
		int			iNtStation;			            // 庄家位置
		int			iNtPlayCount;		            // 庄家坐庄次数
		LLONG		i64NtMoney;			            // 庄家金币
		LLONG		i64NtWinMoney;		            // 庄家输赢情况
												    
		LLONG		i64UserWin;			            // 个人输赢
		LLONG		i64MyMoney;			            // 个人金币数 -从服务端发送过去
												    
		LLONG		i64UserMaxNote;			        // 玩家最大下注数
		LLONG		i64ShangZhuangLimit;	        // 上庄需要的最少金币	

		LLONG		i64ZhongZhu;   					// 本把当前总注额
		LLONG		i64QuYuZhu[BET_ARES]; 			// 本把每个区域下的注额
		LLONG		i64QuYuZhuTrue[BET_ARES];		// 真实玩家的下注值
		LLONG		i64UserXiaZhuData[BET_ARES];	// 玩家区域下注信息
		LLONG	    i64AreaMaxZhu[BET_ARES];		// 每个区域能下的最大注	
	
		GameStation_Bet()
		{
			memset(this,0,sizeof(GameStation_Bet));
			memset(byZhuangList,255,sizeof(byZhuangList));
			memset(TLuziData,255,sizeof(TLuziData));
		}
	};

	/*--------------------------------------------------------------------------*/
	//开牌状态
	struct	GameStation_SendCard
	{
		BYTE		byZhuangList[PLAY_COUNT];	    // 上庄列表	
		LuziData	TLuziData[MAXCOUNT];	        // 路子信息		

		int			iXiaZhuTime;		            // 下注时间					
		int			iKaiPaiTime;     	            // 开牌时间    
		int			iFreeTime;			            // 空闲时间	
		int			iShowWinTime;		            // 显示中奖时间

		int			iNtStation;			            // 庄家位置
		int			iNtPlayCount;		            // 庄家坐庄次数
		LLONG		i64NtMoney;			            // 庄家金币
		LLONG		i64NtWinMoney;		            // 庄家输赢情况

		LLONG		i64UserWin;			            // 个人输赢
		LLONG		i64MyMoney;			            // 个人金币数 -从服务端发送过去

		LLONG		i64UserMaxNote;			        // 玩家最大下注数
		LLONG		i64ShangZhuangLimit;	        // 上庄需要的最少金币	

		LLONG		i64ZhongZhu;   					// 本把当前总注额
		LLONG		i64QuYuZhu[BET_ARES]; 			// 本把每个区域下的注额
		LLONG		i64QuYuZhuTrue[BET_ARES];		// 真实玩家的下注值
		LLONG		i64UserXiaZhuData[BET_ARES];	// 玩家区域下注信息
		LLONG	    i64AreaMaxZhu[BET_ARES];		// 每个区域能下的最大注

		BYTE		byUserCard[2][3];               // 庄闲的牌，0为闲，1为庄
		int  		iZPaiXing[5];			        // 庄家牌型,元素0前两张牌的值，元素1总牌值，元素2天王，元素3对子，元素4和
		int			iXPaiXing[5];			        // 闲家牌型

		GameStation_SendCard()
		{
			memset(this,0,sizeof(GameStation_SendCard));
			memset(byZhuangList,255,sizeof(byZhuangList));
			memset(TLuziData,255,sizeof(TLuziData));
		}
	};
	/*--------------------------------------------------------------------------*/
	//结算状态数据
	struct	GameStation_ShowWin
	{
		BYTE		byZhuangList[PLAY_COUNT];	   // 上庄列表	
		LuziData	TLuziData[MAXCOUNT];	       // 路子信息	

		int			iXiaZhuTime;		           // 下注时间					
		int			iKaiPaiTime;     	           // 开牌时间    
		int			iFreeTime;			           // 空闲时间	
		int			iShowWinTime;		           // 显示中奖时间

		int			iNtStation;			           // 庄家位置
		int			iNtPlayCount;		           // 庄家坐庄次数
		LLONG		i64NtMoney;			           // 庄家金币
		LLONG		i64NtWinMoney;		           // 庄家输赢情况

		LLONG		i64UserWin;			           // 个人输赢
		LLONG		i64MyMoney;			           // 个人金币数 -从服务端发送过去

		LLONG		i64UserMaxNote;			       // 玩家最大下注数
		LLONG		i64ShangZhuangLimit;	       // 上庄需要的最少金币	

		LLONG		i64ZhongZhu;   				   // 本把当前总注额
		LLONG		i64QuYuZhu[BET_ARES]; 		   // 本把每个区域下的注额
		LLONG		i64QuYuZhuTrue[BET_ARES];	   // 真实玩家的下注值
		LLONG		i64UserXiaZhuData[BET_ARES];   // 玩家区域下注信息
		LLONG	    i64AreaMaxZhu[BET_ARES];	   // 每个区域能下的最大注	

		int			iWinQuYu[BET_ARES];		       // 游戏的赢钱区域 0闲，1闲天王，2闲对子，3庄，4庄天王，5庄对子，6和，7同点和

		GameStation_ShowWin()
		{
			memset(this,0,sizeof(GameStation_ShowWin));
			memset(byZhuangList,255,sizeof(byZhuangList));
			memset(TLuziData,255,sizeof(TLuziData));
		}
	};

	/*------------------------------------------------------------------------------*/
	/// 通知客户端是否为超级客户端状态消息结构
	struct S_C_IsSuperUser
	{
		BYTE byDeskStation;      // 玩家位置
		bool bEnable;            // 是否开通
		S_C_IsSuperUser()
		{
			memset(this,0,sizeof(S_C_IsSuperUser));
		}
	};
	/*------------------------------------------------------------------------------*/
	struct SuperUserSetData
	{
		bool	bSetSuccese;			// 是否设置成功了
		int		iSetResult[BET_ARES];	// 设置结果

		SuperUserSetData()
		{
			memset(this,0,sizeof(SuperUserSetData));
		}
	};
}

#pragma pack()
/*--------------------------------------------------------------------------*/

#endif // __BJLMessageHead_h__

