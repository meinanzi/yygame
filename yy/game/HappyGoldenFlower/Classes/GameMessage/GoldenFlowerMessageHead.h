#ifndef _GoldenFlower_Upgrade_Message_H__
#define _GoldenFlower_Upgrade_Message_H__

#include "HNBaseType.h"
#include <string>

namespace goldenflower
{
	//游戏信息
	enum GoldenFlower_COMMAND
	{
		GAME_NAME_ID  =  12101106,	    // 名字 ID
		GAME_KIND_ID  =  2,				// 名字 ID
	};

	

	enum 
	{
		//游戏信息
		PLAY_COUNT					=	6,								//游戏人数
		MAX_CARD_COUNT              =   3,
		E_CHOUMA_COUNT              =   4,

		//游戏状态定义
		GS_WAIT_SETGAME				=	0,				//等待东家设置状态
		GS_WAIT_ARGEE				=	1,				//等待同意设置
		GS_SEND_CARD				=	20,				//发牌状态
		GS_PLAY_GAME				=	21,				//游戏中状态
		GS_WAIT_NEXT				=	23,				//等待下一盘开始 		
		
		
		TYPE_OPEN_LOSE              =    0x06,            //开牌失败
		TYPE_OPEN_WIN               =    0x07,            //开牌失败
		TYPE_REACH_LIMITE_NEXT      =    0x08,            //下一家下注后达到上限
		TYPE_REACH_LIMITE_SELF      =    0x09,            //自己下注后达到上限
		TYPE_COMPARE_CARD           =    0x09,           //比牌操作

		//按注类型
		STATE_ERR					=	0x00,			//错误状态                                                      
		STATE_NORMAL				=	0x01,			//正常状态
		STATE_LOOK					=	0x02,			//看牌了状态
		STATE_WAITE_OPEN			=	0x03,			//下注满了 等待开牌
		STATE_GIVE_UP				=	0x04,			//弃牌状态
		INVALID_SEAT_NO				=   255,		    //无效的椅子号


		ACTION_LOOK					=	0x00,			//看牌
		ACTION_FOLLOW				=	0x01,			//跟注
		ACTION_ADD					=	0x02,			//加注
		ACTION_OPEN					=	0x03,			//比牌
		ACTION_GIVEUP				=	0x04,			//弃牌
		ACTION_WIN					=	0x05,			//赢牌
		ACTION_LOST					=	0x06,			//输牌
		ACTION_NO					=	0x07,			//无

		
		SH_THREE                    = 7, //三条
		SH_SAME_HUA_CONTINUE        = 6, //同花顺
		SH_SAME_HUA                 = 5, //同花
		SH_CONTIUE                  = 4, //顺子
		SH_DOUBLE                   = 3, //对子
		SH_SPECIAL                  = 1, //特殊235
		SH_OTHER                    = 2, //单牌
		SH_ERROR                    = 0, //错误
		
		UD_VF_CALL				    =	 0x01,  //可以跟注
		UD_VF_RAISE				    =	 0x02,  //可以加注
		UD_VF_BET				    =	 0x04,  //可以下注
		UD_VF_FOLD				    =	 0x08,  //可以弃牌
		UD_VF_CHECK				    =	 0x10,  //可以过牌
		UD_VF_ALLIN				    =	 0x20,  //可以全下
		UD_VF_CHECKMANDATE		    =	 0x40,  //可以过牌托管
		UD_VF_CALLMANDATE		    =	 0x80,  //可以跟注托管
		UD_VF_CALLANYMANDATE	    =	 0x100, //可以跟任何注托管
		UD_VF_PASSABANDONMANDATE    =	 0x200, //可以过牌/弃牌托管
		
		C_VF_FOLLOW   = 0x01,  //can follow
		C_VF_LOOK     = 0x02,  //can look
		C_VF_ADD      = 0x04,  //can add
		C_VF_COMPARE  = 0x08,  //can compare
		C_VF_GIVEUP   = 0x10,  //can give up
	};
	
	// Audio effect
	static const char*	GOLD_ADD_LADY			=	"goldenflower/sound/add_lady.wav";
	static const char*	GOLD_ADD_MAN			=	"goldenflower/sound/add_man.wav";
	
	static const char*	GOLD_FOLLOW_LADY		=	"goldenflower/sound/follow_lady.wav";
	static const char*	GOLD_FOLLOW_MAN			=	"goldenflower/sound/follow_man.wav";
	
	static const char*	GOLD_GIVEUP_LADY		=	"goldenflower/sound/giveup_lady.wav";
	static const char*	GOLD_GIVEUP_MAN			=	"goldenflower/sound/giveup_man.wav";
	
	static const char*	GOLD_LOOK_LADY			=	"goldenflower/sound/look_lady.wav";
	static const char*	GOLD_LOOK_MAN			=	"goldenflower/sound/look_man.wav";
	
	static const char*	GOLD_PK_LADY			=	"goldenflower/sound/pk_lady.wav";
	static const char*	GOLD_PK_MAN				=	"goldenflower/sound/pk_man.wav";
	
	static const char*	GOLD_PK_THUNDER			=	"goldenflower/sound/pk_thunder.wav";
	static const char*	GAME_FAPAI				=	"goldenflower/sound/fapai.mp3";
	static const char*	GAME_SHOUCHOUMA			=    "goldenflower/sound/shouchouma.mp3";
	static const char*	GAME_XIAZHU				=	"goldenflower/sound/xiazhu.mp3";


	
	/********************************************************************************/

	//游戏数据包

	/********************************************************************************/

	//发送用户结果
	#pragma pack(1)
	
	/*------------------------S和C都需要用到的数据------------------------------------*/
	struct	TGameBaseData
	{
		/*服务端配置文件配置参数*/
		BYTE				m_byThinkTime;			//游戏思考时间
		BYTE				m_byBeginTime;			//游戏开始时间	

		int					m_iGuoDi;				//锅底值	
		int					m_iDiZhu;				//底注     
		LLONG				m_i64DingZhu;			//顶注--总下注数达到这个值 就强制开牌		
//		LLONG				m_i64ShangXian;			//个人下注上限(即 最大每轮最大下注数不能超过这个值)  暗注的，但显示为明注的
		int					m_iBaseRatio;			//房间倍率

		/*每局游戏结束/开始需要重置的数据*/
		BYTE				m_byNtPeople;           //庄家位置
		BYTE				m_byCurrHandleDesk;		//当前操作的用户
		bool				m_bCanLook;				//是否可以看牌	
		bool				m_bCanFollow;			//是否可以跟注	
		bool				m_bCanAdd[E_CHOUMA_COUNT];				//是否可以加注	
		bool				m_bCanOpen;				//是否可以比牌	
		bool				m_bCanGiveUp;			//是否可以弃牌	

		int					m_iUserState[PLAY_COUNT];	//记录用户状态	

		LLONG				m_i64ZongXiaZhu;				//总注	
		LLONG				m_i64XiaZhuData[PLAY_COUNT];	//玩家下注数据
	//	LLONG				m_i64GuessXiaZhu[PLAY_COUNT];	//玩家的蒙注

		BYTE				m_byUserCard[PLAY_COUNT][MAX_CARD_COUNT];	//用户手上的扑克
		BYTE				m_byUserCardCount[PLAY_COUNT];				//用户手上扑克数目

		bool				m_bFirstNote;                       //是否为第一次下注
		int					m_iAddNoteTime;							//下注次数，用来给客户端判断是否可以比牌
		LLONG				m_i64CurrZhuBase;					//当前下注的基数


		TGameBaseData()
		{
			InitAllData();
		}

		void	InitAllData()
		{
			m_byThinkTime	= 15;			
			m_byBeginTime	= 15;		

			m_iGuoDi	= 100;				
			m_iDiZhu	= 100;			
			m_i64DingZhu= 1000;					
			//m_i64ShangXian=10000;	

			m_byNtPeople = 255;

			InitSomeData();
		}

		/*每局游戏结束/开始需要重置的数据*/
		void	InitSomeData()
		{
			m_byCurrHandleDesk	= 255;		
			m_bCanLook	= false;				
			m_bCanFollow= false;			
		//	m_bCanAdd	= false;				
			m_bCanOpen	= false;				
			m_bCanGiveUp= false;

			memset(m_bCanAdd,0,sizeof(m_bCanAdd));

			memset(m_iUserState,STATE_ERR,sizeof(m_iUserState));
			m_i64ZongXiaZhu = 0;	
			memset(m_i64XiaZhuData,0,sizeof(m_i64XiaZhuData));
			//memset(m_i64GuessXiaZhu,0,sizeof(m_i64GuessXiaZhu));

			memset(m_byUserCard,0,sizeof(m_byUserCard));
			memset(m_byUserCardCount,0,sizeof(m_byUserCardCount));

			m_bFirstNote	= true;
			m_iAddNoteTime	= 0;							
			m_i64CurrZhuBase= 0;					
		}
	};
	/*--------------------------------------------------------------------------------*/
	/********************************************************************************/
	//游戏数据包
	/********************************************************************************/
	/*-----------------------------------------------------------------------------*/
	/*-----------------------------------------------------------------------------*/

	/*-----------------------------------------------------------------------------*/
	//断线重连游戏状态数据包（ 等待其他玩家开始 ）
	struct S_C_GameStation_WaiteAgree
	{
		BYTE		byThinkTime;			//游戏思考时间
		BYTE		byBeginTime;			//游戏开始时间

		bool		bAgree[PLAY_COUNT];		//玩家准备状态

		int			iGuoDi;					//锅底值	
		int			iDiZhu;					//底注      

		LLONG		i64ShangXian;			//个人下注上限	明注
		LLONG		i64DingZhu;				//顶注

		S_C_GameStation_WaiteAgree()
		{
			memset(this,0,sizeof(S_C_GameStation_WaiteAgree));
		}
	};
	/*-----------------------------------------------------------------------------*/
	//断线重连游戏状态数据包	（ 游戏正在发牌状态 ）
	struct S_C_GameStation_SendCard
	{
		BYTE		byThinkTime;			//游戏思考时间
		BYTE		byBeginTime;			//游戏开始时间

		BYTE		byNtStation;					//庄家位置
		BYTE		byUserCardCount[PLAY_COUNT];	//用户手上的牌数

		int			iUserState[PLAY_COUNT];		//玩家状态

		int			iGuoDi;						//锅底值	
		int			iDiZhu;						//底注      
		//LLONG		i64ShangXian;				//个人下注上限	明注
		LLONG		i64DingZhu;					//顶注

		LLONG		i64AllZongZhu;				//总注
		LLONG		i64XiaZhuData[PLAY_COUNT];	//用户当前下注信息
		S_C_GameStation_SendCard()
		{
			memset(this,0,sizeof(S_C_GameStation_SendCard));
		}
	};
	/*-----------------------------------------------------------------------------*/
	//断线重连游戏状态数据包	（ 游戏中状态 ）
	struct S_C_GameStation_PlayGame 
	{
		bool		bCanLook;				//可否看牌
		bool		bCanFollow;				//可否跟注
		bool		bCanAdd[E_CHOUMA_COUNT];//可否加注
		bool		bCanOpen;				//可否比牌
		bool		bCanGiveUp;				//可否弃牌


		BYTE		byThinkTime;			//游戏思考时间
		BYTE		byBeginTime;			//游戏开始时间

		BYTE		byNtStation;					//庄家位置
		BYTE		byCurrHandleDesk;				//当前操作的玩家
		BYTE		byUserCardCount[PLAY_COUNT];				//用户手上的牌数
		BYTE		byUserCard[PLAY_COUNT][MAX_CARD_COUNT];		//用户手上的牌

		int			iUserState[PLAY_COUNT];		//玩家状态
		int         iRemainderTime;             //剩余时间

		int			iGuoDi;						//锅底值	
		int			iDiZhu;						//底注      
		LLONG		i64ShangXian;				//个人下注上限	明注
		LLONG		i64DingZhu;					//顶注

		LLONG		i64AllZongZhu;				//总注
		LLONG		i64XiaZhuData[PLAY_COUNT];	//用户当前下注信息

		S_C_GameStation_PlayGame()
		{
			memset(this,0,sizeof(S_C_GameStation_PlayGame));
		}
	};


	/*-----------------------------------------------------------------------------*/
	//游戏开始
	struct	S_C_GameBegin
	{
		BYTE                byNtStation;                      //庄家 

		int                 iGuoDi;                         //本局锅底值
		int					iDiZhu;							//本局底注

		int					iUserState[PLAY_COUNT];			//玩家状态

		//LLONG				i64ShangXian;					//本局下注上限 (即 最大每轮最大下注数不能超过这个值) 暗注的，但显示为明注的
		LLONG				i64DingZhu;						//本局顶注 -总下注上限	达到上限 就要开牌	

		LLONG				i64ZongXiaZhu;					//当前总下注;
		LLONG				i64XiaZhuData[PLAY_COUNT];		//玩家下注数据

		S_C_GameBegin()
		{
			memset(this,0,sizeof(S_C_GameBegin));
		}
	};
	/*-----------------------------------------------------------------------------*/
	//发牌数据包
	struct S_C_SendCard
	{
		BYTE		byCard[PLAY_COUNT][MAX_CARD_COUNT];				//牌数据
		BYTE		byCardCount[PLAY_COUNT];                        //牌张数
		BYTE		bySendCardTurn[PLAY_COUNT*MAX_CARD_COUNT];		//发牌顺序--记录的是发牌的位置  255表示不需要发牌

		S_C_SendCard()
		{
			memset(this,0,sizeof(S_C_SendCard));
			memset(bySendCardTurn,255,sizeof(bySendCardTurn));
		}

	};
	/*-----------------------------------------------------------------------------*/
	//游戏开始数据包
	struct S_C_BeginPlay
	{
		BYTE		byCurrHandleStation;			//出牌的位置
		bool		bLook;		//是否可看牌
		bool		bFollow;	//是否可跟注
		bool		bAdd[E_CHOUMA_COUNT];		//是否可加注
		bool		bOpen;		//是否可比牌
		bool		bGiveUp;	//是否可弃牌

		S_C_BeginPlay()
		{
			memset(this,0,sizeof(S_C_BeginPlay));
		}
	};
	/*-----------------------------------------------------------------------------*/
	//通知操作
	struct S_C_NoticeAction
	{
		BYTE				byCurrHandleStation;	//出牌的位置
		bool				bCanLook;				//是否可以看牌	
		bool				bCanFollow;				//是否可以跟注	
		bool				bCanAdd[E_CHOUMA_COUNT];				//是否可以加注	
		bool				bCanOpen;				//是否可以比牌	
		bool				bCanGiveUp;				//是否可以弃牌	

		S_C_NoticeAction()
		{
			memset(this,0,sizeof(S_C_NoticeAction));
		}
	};
	/*-----------------------------------------------------------------------------*/
	//用户看牌
	struct C_S_LookCard
	{
		BYTE	byDeskStation;	
		C_S_LookCard()
		{
			memset(this,255,sizeof(C_S_LookCard));
		}
	};

	//看牌数据
	struct S_C_LookCardResult
	{	
		BYTE            byDeskStation;				//玩家位置
		BYTE			byUserCardCount;			//用户手上的牌数
		BYTE			byUserCard[MAX_CARD_COUNT];	//用户手上的牌
		int				iUserState;					//玩家状态
		S_C_LookCardResult()
		{
			memset(this,-1,sizeof(S_C_LookCardResult));
			byUserCardCount = 0;
		}
	};
	/*-----------------------------------------------------------------------------*/
	//用户跟注
	struct C_S_UserGenZhu
	{
		BYTE	byDeskStation;		//玩家位置
	};
	/*-----------------------------------------------------------------------------*/
	//用户跟注结果
	struct S_C_UserGenZhuResult
	{

		BYTE		byDeskStation;		//玩家位置
		int			iUserState;			//该位置玩家的状态
		LLONG		i64FollowNum;		//跟注的大小
		LLONG		i64UserZongZhu;		//玩家的总下注
		LLONG		i64AllZongZhu;		//所有总下注

		S_C_UserGenZhuResult()
		{
			memset(this,0,sizeof(S_C_UserGenZhuResult));
		}
	};
	/*-----------------------------------------------------------------------------*/
	//用户下注
	struct C_S_JiaZhu
	{

		BYTE		byDeskStation;	
		int			iAddType;			//加注类型

		C_S_JiaZhu()
		{
			memset(this,0,sizeof(C_S_JiaZhu));
		}
	};
	/*-----------------------------------------------------------------------------*/
	//用户下注结果
	struct S_C_JiaZhuResult
	{
		BYTE		byDeskStation;	
		int			iUserState;			//该位置玩家的状态
		LLONG		i64AddNum;			//跟注的大小
		LLONG		i64UserZongZhu;		//玩家的总下注
		LLONG		i64AllZongZhu;		//所有总下注

		S_C_JiaZhuResult()
		{
			memset(this,0,sizeof(S_C_JiaZhuResult));
		}
	};
	/*-----------------------------------------------------------------------------*/
	//用户弃牌、弃牌结果
	struct C_S_UserGiveUp
	{
		BYTE	byDeskStation;	
	};

	struct S_C_UserGiveUpResult
	{
		BYTE	byDeskStation;	
		int		iUserState;					//玩家状态
	};
	/*-----------------------------------------------------------------------------*/
	//用户比牌
	struct C_S_UserBiPai
	{
		BYTE	byDeskStation;		//主动比牌的玩家
		BYTE	byBeDeskStation;	//被动比牌玩家

		C_S_UserBiPai()
		{
			memset(this, 255, sizeof(C_S_UserBiPai));
		}
	};
	/*-----------------------------------------------------------------------------*/
	//用户比牌
	struct S_C_UserBiPaiResult
	{
		BYTE	byDeskStation;		//主动比牌的玩家
		BYTE	byBeDeskStation;	//被动比牌玩家

		BYTE	byWinDesk;			//比牌赢的玩家
		BYTE	byLostDesk;			//比牌输的玩家

		int		iUserStation[PLAY_COUNT];			//记录用户状态

		LLONG		i64AddNum;			//比牌需要下注的数
		LLONG		i64UserZongZhu;		//玩家的总下注
		LLONG		i64AllZongZhu;		//所有总下注



		S_C_UserBiPaiResult()
		{
			memset(this,0,sizeof(S_C_UserBiPaiResult));
		}
	};
	/*-----------------------------------------------------------------------------*/
	//游戏结束统计数据包
	struct S_C_GameEnd
	{
		BYTE				byWiner;							//赢家

		BYTE				byCard[PLAY_COUNT][MAX_CARD_COUNT];	//牌数据
		BYTE				byCardShape[PLAY_COUNT];			//牌型

		int					iOperateCount[PLAY_COUNT];			//操作次数
		LLONG				i64UserScore[PLAY_COUNT];			//各玩家得分-扣税前
		LLONG				i64ChangeMoney[PLAY_COUNT];			//各玩家金币变化-扣税后

		S_C_GameEnd()
		{
			memset(this,0,sizeof(S_C_GameEnd));
		}
	};
	/*-----------------------------------------------------------------------------*/
	//游戏结束统计数据包
	struct GameCutStruct
	{
		BYTE					byDeskStation;					//退出位置
		LLONG					iTurePoint[PLAY_COUNT];					//庄家得分
		LLONG					iChangeMoney[PLAY_COUNT];
		GameCutStruct()
		{
			memset(this,0,sizeof(GameCutStruct));
		}
	};
	//-------------------------------------------------------------------------------
	//超端信息结构体
	struct	S_C_SuperUserProof
	{
		BYTE	byDeskStation;	
		bool	bIsSuper;
		S_C_SuperUserProof()
		{
			memset(this,0,sizeof(S_C_SuperUserProof));
			byDeskStation = 255;
		}
	};
	//超端信息结构体
	struct	C_S_SuperUserSet
	{
		BYTE	byDeskStation;	
		BYTE	byMaxDesk;		//设定最大的玩家
		BYTE	byMinDesk;		//设定最小的玩家
		C_S_SuperUserSet()
		{
			memset(this,255,sizeof(C_S_SuperUserSet));
		}
	};
	//超端设置结果
	struct	S_C_SuperUserSetResult
	{
		bool	bSuccess;		//是否设定成功
		BYTE	byDeskStation;	
		BYTE	byMaxDesk;		//设定最大的玩家
		BYTE	byMinDesk;		//设定最小的玩家
		S_C_SuperUserSetResult()
		{
			memset(this,255,sizeof(S_C_SuperUserSetResult));
			bSuccess = false;
		}
	};
	/*-----------------------------------------------------------------------------*/
	#pragma pack()

		/********************************************************************************/

		// 数据包处理辅助标识

		/********************************************************************************/
	enum
	{
		S_C_GAME_BEGIN = 50,			//游戏开始		
		S_C_SEND_CARD = 51,			//开始发牌
		S_C_GAME_PLAY = 52,			//开始游戏		
		S_C_CALL_ACTION = 53,			//通知玩家操作	
		S_C_GAME_END = 54,			//游戏结束

		S_C_SUPER_PROOF = 70,			//超端校验
		C_S_SUPER_SET = 71,			//超端设置
		S_C_SUPER_SET_RESULT = 72,			//超端设置结果

		C_S_LOOK_CARD = 80,			//玩家看牌
		S_C_LOOK_CARD_RESULT = 81,			//玩家看牌结果
		C_S_GEN_ZHU = 82,			//玩家跟注
		S_C_GEN_ZHU_RESULT = 83,			//玩家跟注结果
		C_S_JIA_ZHU = 84,			//加注
		S_C_JIA_ZHU_RESULT = 85,			//加注结果
		C_S_BI_PAI = 86,			//玩家比牌
		S_C_BI_PAI_RESULT = 87,			//玩家比牌结果
		C_S_GIVE_UP = 88,			//玩家弃牌
		S_C_GIVE_UP_RESULT = 89,			//玩家弃牌结果
	};
                            
	//自定义消息结构        
	//game result data node.
	struct TGameResult
	{
		std::string szName;
		LLONG llMoney;
		std::string szType;
		TGameResult()
		{
			llMoney = 0;
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

}


#endif
