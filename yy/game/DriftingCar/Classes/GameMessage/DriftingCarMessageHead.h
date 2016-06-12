#ifndef __DriftingCar_MessageHead_H__
#define __DriftingCar_MessageHead_H__

#include "HNNetExport.h"

namespace DriftingCar
{
	//游戏ID
	enum Game_ID
	{
		NAME_ID = 31010100
	};

	//游戏指令ID
	enum CMD_ID
	{
		ASS_GAME_FREE			=	53,	  //空闲消息
		ASS_BEGIN_BET			=	54,	  //开始下注
		ASS_GAME_SEND			=	55,	  //开始开牌
		ASS_UPDATE_BET			=	56,	  //更新下注
		ASS_APPLY_NT			=	57,	  //申请上庄消息
		ASS_APPLY_RESULT		=	58,	  //申请上庄结果消息
		ASS_APPLY_LEAVE			=	59,	  //申请下庄消息
		ASS_LEAVE_RESULT		=	60,	  //申请下庄结果消息
		ASS_USER_BET			=	61,	  //玩家下注消息
		ASS_BET_RESULT			=	62,	  //玩家下注结果消息
		ASS_GAME_OPEN           =   63,   //开奖
		ASS_SUPER_RESULT		=	64,	  //超端用户控制结果消息包
	
		ASS_SUPER_USER			=	65,	  //超端用户消息
		ASS_PREPARE_END			=	66,	  //游戏准备阶段用户退出
		ASS_WAITE_NT		    =   67,   //无庄等待消息
		ASS_SHOW_ROBOTBET       =   68,   //显示机器人下注消息
	};

	//游戏状态
	enum Game_State
	{
		GS_WAIT_SETGAME			=	0,	 //等待东家设置状态
		GS_WAIT_ARGEE			=	1,	 //等待同意设置
		GS_FREE_STATUS			=	20,	 //游戏空闲阶段
		GS_BET_STATUS			=	21,	 //游戏下注阶段
		GS_SEND_STATUS			=	22,	 //发牌阶段
		GS_WAIT_NEXT			=	23,	 //等待下一盘开始阶段
	};

	enum Game_Const
	{
		PLAY_COUNT				=	180, //游戏人数
		SEND_COUNT				=	32,	 //发牌张数
		BET_ARES				=	8,	 //下注区域数
		BET_STYLE				=	6,	 //筹码类型
		HISTORY_COUNT			=   72,  //历史记录
		TIME_COUNT				=   100, //跑马灯时间
		CAR_LOGO_COUNT			=   32,  //车标数量
		NON_NT					=   255, //没有申请上庄
	};


//	//下注区域赔率
//	static	const	int		G_iArePeiLv[BET_ARES] = {40,30,20,10,5,5,5,5};
//	//筹码类型值
//	static	const	int		G_iChouMaValue[BETSTYLE] = {100,1000,10000,100000,1000000,10000000};

	#pragma pack(1)

	//申请上庄消息
	struct	ApplyNtMsg
	{
		BYTE	byDeskStation;		//申请座位号
		ApplyNtMsg()
		{
			memset(this,-1,sizeof(ApplyNtMsg));

		}
	};
	//超端用户控制的结果
	struct	SuperUserControlReslut
	{
		BYTE	bSuperDeskStation;	//超端用户座位号
		BYTE    bySuperReslut;		//超端用户控制的结果
		
		SuperUserControlReslut()
		{
			memset(this,-1,sizeof(SuperUserControlReslut));

		}
	};
	//超端用户向客服端发送结果
	struct	SuperUserControlStart
	{
		BYTE	byDeskStation;		//超端用户座位号
		BYTE    bySuperStart;		//超端用户控制的起点
		SuperUserControlStart()
		{
			memset(this,-1,sizeof(SuperUserControlStart));

		}
	};

	//准备阶段退出结构
	struct GamePrepareStageQuit
	{
		BYTE				byNtStation;				//庄家位置
		BYTE				byNtWaitList[PLAY_COUNT];	//庄家列表
		BYTE                byNtcount;					//坐庄次数
		LLONG             i64NtScore;					//庄家成绩
		GamePrepareStageQuit()
		{
			memset(this,0,sizeof(GamePrepareStageQuit));
			memset(byNtWaitList,255,sizeof(byNtWaitList));
			byNtStation=255;
		}
	};
//-------------------------------------------------------------------------------
//无庄等待
struct S_C_WaiteNtMsg
{
	bool	bWait;
	S_C_WaiteNtMsg()
	{
		memset(this,0,sizeof(S_C_WaiteNtMsg));
	}
};
	//-------------------------------------------------------------------------------
	//开始下注消息包
	struct BeginBetMsg
	{
		BYTE	byNtStation;	            //庄家位置
		BYTE    byNtCount;                  //庄家坐庄次数
		BYTE    byGameStation;              //游戏状态
		BYTE    byNtWaitList[PLAY_COUNT];   //庄家列表
		BYTE	byCardStyle[SEND_COUNT];    //牌类型

		LLONG   i64NtScore;                 //庄家成绩
		BeginBetMsg()
		{
			memset(this,0,sizeof(BeginBetMsg));
			memset(byNtWaitList,255,sizeof(byNtWaitList));
			memset(byCardStyle,-1,sizeof(byCardStyle));
			byNtStation=255;
		
		}
	};
	//开奖消息包(二维数组，0表示玩家的，1表示庄家的)
	struct BeginAward
	{
		BYTE	  bDeskStation;       //座位号
		BYTE	  byLuziList[72];     //路子
		LLONG     i64PlayerGrade[2];  //玩家成绩
		LLONG     i64PlayCoin[2];     //玩家输赢金币
		LLONG     i64ReturnMoney[2];  //返还的金币数
		BeginAward()
		{
			memset(this,0,sizeof(BeginAward));
			memset(byLuziList,255,sizeof(byLuziList));
		}
	};
	//发牌消息包
	struct BeginSendCardMsg
	{
		BYTE byGameStation;					//游戏状态
		BYTE byWinPrize;					//开奖位置 大保时捷 小，大宝马，小，大奔驰，小，大大众，小
		BeginSendCardMsg()
		{
			memset(this,0,sizeof(BeginSendCardMsg));
			byWinPrize=-1;

		}
	};
	//更新历史下注
	struct TUpdateHistoryBet
	{
		LLONG   i64AreaBetAllSum[BET_ARES];//各区域的总下注
		LLONG	i64UserBetSum[BET_ARES];   //玩家在各区域的总下注
		LLONG	i64SumAllBet;//总下注数
		TUpdateHistoryBet()
		{
			memset(this,0,sizeof(i64AreaBetAllSum));
		}
	};
	//空闲消息包
	struct BeginFreeMsg
	{
		bool	bFlag;						//空闲标识
		BYTE	byGameStation ;				//当前的游戏状态
		BYTE	byFreeTime;					//空闲时间
		BYTE    byNtStation;				//庄家位置
		BYTE    byPlayStation;				//玩家位置
		BYTE    byNtcount;					//庄家坐庄次数
		BYTE    byWaitNtList[PLAY_COUNT];	//上庄列表
		LLONG   i64NtScore;					//庄家成绩
		BeginFreeMsg()
		{
			memset(this,0,sizeof(BeginFreeMsg));
			byNtStation = 255;
			memset(byWaitNtList, 255, sizeof(byWaitNtList));
		}
	};
	//申请上庄结果消息
	struct	ApplyNtResultMsg
	{
		bool    bNtFlag;							//在没有庄家的情况下上庄的标志
		bool	bApplay;							//是否因为金币没有达到上庄金币而上庄不成功
		BYTE	byDeskStation;						//申请座位号
		BYTE	byWaitNtList[PLAY_COUNT];			//申请上庄列表
		BYTE	byNtStation;						//庄家的位置
		BYTE    byNtcount;							//庄家坐庄次数
		BYTE    bGameStation;
		LLONG   i64NtScore;							//庄家成绩
		ApplyNtResultMsg()
		{
			memset(this,-1,sizeof(ApplyNtResultMsg));
			memset(byWaitNtList,255,sizeof(byWaitNtList));
			byNtStation=255;
			bApplay=false;
			bNtFlag=false;
		}
	};
	//申请下庄消息包
	struct	ApplyLiveNtMsg
	{
		BYTE	byDeskStation;						//申请座位号
		ApplyLiveNtMsg()
		{
			memset(this,-1,sizeof(ApplyLiveNtMsg));
		}
	};
	//申请下庄结果消息包
	struct	ApplyLiveNtResultMsg
	{
		BYTE    byNtStation;                //庄家位置
		BYTE	byDeskStation;		        //申请座位号
		BYTE    byWaitNtList[PLAY_COUNT];	//上庄列表
		BYTE    byNtcount;                  //庄家坐庄次数
		LLONG   i64NtScore;                 //庄家成绩
		ApplyLiveNtResultMsg()
		{
			memset(this,-1,sizeof(ApplyLiveNtResultMsg));
			memset(byWaitNtList,255,sizeof(byWaitNtList));
			byNtStation=255;
			byDeskStation=255;
			byNtcount=0;
			i64NtScore=0;
		}
	};

	//玩家下注
	struct	UserBetMsg
	{
		BYTE	byDeskStation;		//下注座位号
		BYTE	byBetArea;			//下注区域
		BYTE	byChouMaType;		//下注筹码类型
		UserBetMsg()
		{
			memset(this,0,sizeof(UserBetMsg));
		}
	};
	//游戏状态基础数据包
	struct	GameStation_Base
	{
		BYTE		bGameStation;		           //游戏状态
		BYTE		byFreeTime;					   //空闲时间
		BYTE		byBetTime;			           //下注时间
		BYTE		bySendTime;	                   //开牌时间
		BYTE		byRemaindTime;		           //剩余时间
		BYTE		byNtStation;			       //庄家位置
		BYTE		byMaxNtPeople;		           //最大上庄人数

		BYTE		byLuziList[72];		           //路子（开奖记录）
		BYTE		byWaitNtList[PLAY_COUNT];	   //上庄列表
		int			iNtPlayCount;		           //庄家坐庄次数
		int			iApplyNtLimite;		           //上庄条件
		LLONG		i64NtGrade;		               //庄家成绩
		LLONG       i64PlayerGrade;			       //閑家成績
		LLONG       i64AreaBetLimite;       	   //最大下注限制
		GameStation_Base()
		{
			memset(this,0, sizeof(GameStation_Base));
		}
	};
	//-------------------------------------------------------------------------------
	//下注结果(总下注)
	struct	UserBetResultMsg
	{
		bool		bDynamicBet;		           //是否动态下注
		BYTE		byBetDenied;				   //下注状态 0表示成功 1表示庄家不够，2表示自己不够，3表示下注已满
		BYTE	    byDeskStation;		           //下注座位号
		BYTE		byBetArea;			           //下注区域
		BYTE		byChouMaType;		           //下注筹码类型
		BYTE	    byBetNumFlag;		           //下注100W，500W，超500W的标志
		LLONG       i64SumAllBet;                  //总下注
		LLONG       i64UserBetCount[BET_ARES];     //玩家在每个区域的下注情况
		LLONG		i64AreaBetAllSum[BET_ARES];	   //各区域的总下注
		UserBetResultMsg()
		{
			memset(this,0,sizeof(UserBetResultMsg));
		}
	};
	///游戏任务结构
	struct GameTaskStruct
	{
		BYTE     byBackCardType ; 
		BYTE     byTaskType ; 
		BYTE     bySpecifyShape ; 
		BYTE     bySpecifyCard ;
		BYTE     byBackCardMutiple ;  ///底牌倍数

		GameTaskStruct()
		{
			memset(this , 0 , sizeof(GameTaskStruct)) ; 
		}
		void IniData(void)
		{
			memset(this , 0 ,sizeof(GameTaskStruct)) ; 
		}

	};

	#pragma	pack()
}


/********************************************************************************/
#endif
