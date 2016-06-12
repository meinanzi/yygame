#ifndef __TRACTORGameClientMessage_h__
#define __TRACTORGameClientMessage_h__


#include "TRACTORMessageHead.h"
#include <string>
#include "HNBaseType.h"

namespace TRACTOR
{
	/************************************************************************/
	/* 客户端定义的消息结构，仅提供客户端使用                                  */
	/************************************************************************/
	enum 
	{
		UD_VF_CALL			=		0x01,  //可以跟注
		UD_VF_RAISE			=		0x02,  //可以加注
		UD_VF_BET			=		0x04,  //可以下注
		UD_VF_FOLD			=		0x08,  //可以弃牌
		UD_VF_CHECK			=		0x10,  //可以过牌
		UD_VF_ALLIN			=		0x20,  //可以全下
		UD_VF_CHECKMANDATE	=		0x40,  //可以过牌托管
		UD_VF_CALLMANDATE		=	0x80,  //可以跟注托管
		UD_VF_CALLANYMANDATE	=	0x100, //可以跟任何注托管
		UD_VF_PASSABANDONMANDATE=	0x200, //可以过牌/弃牌托管

		C_VF_FOLLOW  =  0x01,  //can follow
		C_VF_LOOK    =  0x02,  //can look
		C_VF_ADD     =  0x04,  //can add
		C_VF_COMPARE =  0x08,  //can compare
		C_VF_GIVEUP  =  0x10,  //can give up
	};


	// Audio effect
	static const  char*  GOLD_BACKGROUND_MUSIC		=	"tractor/sound/bgm.wav";
	static const  char*  GOLD_SEND_POKER			=	"tractor/sound/sendpoker.wav";
	static const  char*  GOLD_RECYCLE_CHIP			=	"tractor/sound/recyclechips.wav";		
	static const  char*  GOLD_ADD_LADY				=	"tractor/sound/add_lady.wav";
	static const  char*  GOLD_ADD_MAN				=	"tractor/sound/add_man.wav";		
	static const  char*  GOLD_FOLLOW_LADY			=	"tractor/sound/follow_lady.wav";
	static const  char*  GOLD_FOLLOW_MAN			=	"tractor/sound/follow_man.wav";		
	static const  char*  GOLD_GIVEUP_LADY			=	"tractor/sound/giveup_lady.wav";
	static const  char*  GOLD_GIVEUP_MAN			=	"tractor/sound/giveup_man.wav";		
	static const  char*  GOLD_LOOK_LADY				=	"tractor/sound/look_lady.wav";
	static const  char*  GOLD_LOOK_MAN				=	"tractor/sound/look_man.wav";		
	static const  char*  GOLD_PK_LADY				=	"tractor/sound/pk_lady.wav";
	static const  char*  GOLD_PK_MAN				=	"tractor/sound/pk_man.wav";
	static const  char*  GOLD_PK_THUNDER			=	"tractor/sound/pk_thunder.wav";
	static const  char*  GOLD_PK_WIN				=	"tractor/sound/pk_win.wav";
	static const  char*  GOLD_PK_BGM				=	"tractor/sound/pk_bgm.wav";
	static const  char*  GOLD_OVER					=	"tractor/sound/gameover.wav";
	static const  char*  GOLD_WIN					=	"tractor/sound/gamewin.wav";
	static const  char*  GOLD_WARNING				=	"tractor/sound/warning.wav";


	struct PlayerInfo
	{

	};

	//game result data node.
	struct TGameResult
	{
		std::string szName;
		LLONG llMoney;			//输赢的金币
		LLONG i64Money;			//结算前玩家当前身上的金币
		BYTE szType;
		BYTE viewSeatNo;
		TGameResult()
			:llMoney(0)
		{

		}
	};

	//发牌玩家信息
	struct THandCard
	{
		BYTE bySeatNo;
		BYTE byCards[3];

		THandCard()
		{
			bySeatNo = INVALID_DESKNO;
			memset(byCards,0,sizeof(byCards));
		}
	};

	struct TableInfo
	{
		INT                 iOffset;
		BYTE                byDeskNo;                       //桌子编号
		BYTE                byMeSeatNo;                     //我的座位号
		bool                bAutoCreate;                    //自动创建的牌桌
		BYTE                byGameStation;                  //游戏状态
		BYTE                byWaitTime;                     //准备等待时间
		BYTE				iThinkTime;	       		        //游戏思考时间
		BYTE				iBeginTime;				        //游戏开始时间
		BYTE				iAllCardCount;			        //扑克数目
		INT					iRoomBasePoint;			        //房间倍数
		INT					iRunPublish;				        //逃跑扣分
		BYTE                bEXFlag1[12];                   //zht 2011-4-12, 不使用的字段，占位用
		LLONG				iLimitNote;				        //下注封顶值   明注 / 暗注
		LLONG				iThisTurnLimit;				    //本轮限注   明注 / 暗注
		INT					iBaseNote;				        //底注	       暗注
		INT                 iGuoDi;                         //锅底
		LLONG               iLimtePerNote;                  //暗注上限，明注需要 * 2
		INT                bShowIdOrPlay;                  //显示玩家还是id
		INT                 iFrontNote;                     //前位玩家有效下注数
		bool                bPlayer[PLAY_COUNT];            //是否玩家或中途进入观战者(是否参与游戏，还需要通过玩家状态判断)
		BYTE				iUserCardCount[PLAY_COUNT];		//用户手上的牌数
		BYTE				iUserCard[PLAY_COUNT][5];		//用户手上的牌
		bool                bSuperUser[PLAY_COUNT];         //是否超级玩家
		LLONG               iPlayerNote[PLAY_COUNT];         //总共下注数量
		LLONG               iLastNote[PLAY_COUNT];		        //上一轮下注数量
		LLONG               iTotalNote;                     //总下注
		bool                bOpenLose[PLAY_COUNT];          //玩家比牌是否输了
		bool                bMing[PLAY_COUNT];              //明牌状态
		bool                bIsGiveUp[PLAY_COUNT];          //玩家是否放弃
		BYTE                byWin[PLAY_COUNT];
		CHAR                strCity[PLAY_COUNT][50];        //所在城市
		CHAR                strUserID[PLAY_COUNT][50];      //玩家ID
		bool                bFirstGame;                     //是否第一次下注
		BYTE                byNtPeople;                     //庄家
		BYTE                byOutCardPeople;                //现在出牌用户
		BYTE                byFirstOutPeople;               //第一个出牌的玩家
		tagUserResult       tag;
		BYTE                byTimeOutCount;                 //超时次数
		BYTE                byComLoseStation;               //比牌负者
		INT                 iNoteTimes;                     //下注次数（下注超过一圈才能比牌）
	};

}
#endif // __TRACTORGameClientMessage_h__