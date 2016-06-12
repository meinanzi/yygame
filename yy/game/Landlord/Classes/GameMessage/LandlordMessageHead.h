#ifndef __Landlord_MessageHead_H__
#define __Landlord_MessageHead_H__

#include "HNNetExport.h"

namespace landlord
{
	enum Show_Type
	{
		SHOW_CALL_LORD     = 0x01,
		SHOW_NO_CALL_LORD  = 0x02,
		SHOW_ROB_LORD      = 0x04,
		SHOW_NO_ROB_LORD   = 0x08,
		SHOW_DOUBLE        = 0x10,
		SHOW_NO_DOUBLE     = 0x20,
		SHOW_SHOW_CARD     = 0x40,
		SHOW_START         = 0x80,
	};


	enum Landlord_COMMAND
	{
		//游戏信息
		GAME_NAME_ID         =  10100003, // 名字 ID

		//版本定义	          
		GAME_MAX_VER         =  1,        //现在最高版本
		GAME_LESS_VER        =  1,        //现在最低版本
        GAME_CHANGE_VER		 =	0,		  //修改版本
        
        //游戏内部开发版本号
        DEV_HEIGHT_VERSION   =  2,        //内部开发高版本号(每修正一次LOW_VERSIONG+1)
        DEV_LOW_VERSION	     =	0,	      //内部开发低版本号
        
        //游戏信息
        PLAY_COUNT	         =	3,        //游戏人数
        
        		//游戏状态定义
        GS_WAIT_SETGAME		 =	0,				//等待东家设置状态
        GS_WAIT_ARGEE		 =	1,				//等待同意设置
        GS_SEND_CARD		 =	20,				//发牌状态
        GS_WAIT_BACK		 =	21,				//等待扣压底牌
        GS_PLAY_GAME		 =	22,				//游戏中状态
        GS_WAIT_NEXT		 =	23,				//等待下一盘开始 
        					 
        GS_FLAG_NORMAL		 =	0,				//正常情况
		GS_FLAG_CALL_SCORE	 =	1,				//叫分
		GS_FLAG_ROB_NT		 =	2,				//抢地主
		GS_FLAG_ADD_DOUBLE	 =	3,				//加棒
		GS_FLAG_SHOW_CARD	 =	4,				//亮牌
        GS_FLAG_PLAY_GAME    =  5,
	};



	const  INT  MAX_TASK_TYPE   = 4 ;  ///任务最大种类 
	const  INT  MAX_CARD_SHAPE  = 8 ;  ///牌型最大种类
	const  INT  MAX_CARD_TYPE   = 15 ; ///牌种类

	///底牌类型
	enum BackCardType
	{
		TYPE_NONE        = 0 ,//什么牌型都不是
		TYPE_DOUBLE_CARD = 10 , 
		TYPE_SAME_HUA    = 11,
		TYPE_SMALL_KING  = 12 ,
		TYPE_BIG_KING    = 13 , //大王
		TYPE_TRIPLE_CARD = 14 ,
		TYPE_STRAIT      = 15,
		TYPE_ROCKET      = 16
	};
	///任务类型
	enum LastTaskType
	{
		TYPE_LAST_NONE         = 0 ,    ///无任何牌型 
		TYPE_HAVE_A_CARD       = 100 ,  ///有某张牌
		TYPE_SOME_SHAPE        = 101 ,  ///有某种牌型
		TYPE_SINGLE_SOME_CARD  = 102 ,  ///打的某张牌
		TYPE_DOUBLE_SOME_CARD  = 103   ///打的一对某种牌
	};

	#pragma pack(1)

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
	///游戏中的倍数
	struct  GameMutipleStruct
	{
		INT       sBaseMutiple ;                 ///游戏中的基本底注
		INT       sBackCardMutiple ;             ///底牌倍数
		INT       sBombCount ;                   ///炸弹倍数
		INT       sSprINTMutiple ;               ///春天
		INT       sCardShapeMutiple  ;           ///牌型倍数（一般指任务中的倍数）

		BYTE      sAddGameMutiple[PLAY_COUNT] ;  /// 游戏中加倍
		BYTE      sRobNtMutiple[PLAY_COUNT] ;    ///抢地主倍数
		BYTE      sMingPaiMutiple[PLAY_COUNT] ;  ///每个人的明牌倍数

		GameMutipleStruct()
		{
			sBaseMutiple = 1 ; 
			sBackCardMutiple = 1 ; 
			sBombCount = 0 ; 
			sSprINTMutiple = 1;
			sCardShapeMutiple = 100 ; 

			memset(sAddGameMutiple , 0 , sizeof(sAddGameMutiple)) ; 
			memset(sRobNtMutiple , 0 , sizeof(sRobNtMutiple)) ; 
			memset(sMingPaiMutiple , 0 , sizeof(sMingPaiMutiple)) ; 
		};
		void  IniData(INT  iBaseMutiple)
		{
			sBaseMutiple = iBaseMutiple ; 
			sBombCount = 0 ; 
			sBackCardMutiple = 1 ; 
			sSprINTMutiple = 1;
			sCardShapeMutiple = 100; 

			memset(sAddGameMutiple , 0 , sizeof(sAddGameMutiple)) ; 
			memset(sRobNtMutiple , 0 , sizeof(sRobNtMutiple)) ; 
			memset(sMingPaiMutiple , 0 , sizeof(sMingPaiMutiple)) ; 
		}
		///获取明牌最大倍数
		INT  GetMingMaxMutiple(void)
		{		
			INT iMingMutiple = std::max(std::max(sMingPaiMutiple[0] , sMingPaiMutiple[1]) ,sMingPaiMutiple[2]) ; 

			return (iMingMutiple >0?iMingMutiple :1) ; 
		}
		///获取器抢地主倍数
		INT  GetRobNtMutiple(void)
		{
			INT  iRobMutiple = 0 ; 
			INT  iRobCount   = 0 ; 

			for(INT  i = 0  ; i <PLAY_COUNT ; i++)
			{
				iRobCount += sRobNtMutiple[i] ; 
			}

			iRobMutiple = pow(2.0, iRobCount) ; 

			return iRobMutiple ; 
		}
		///获取公共倍数
		INT  GetPublicMutiple(void)
		{
			INT  iBombMutiple = pow(2.0 , sBombCount) ; 

			INT  iGameMutiple  = sBaseMutiple*sBackCardMutiple*iBombMutiple*sSprINTMutiple*sCardShapeMutiple*GetRobNtMutiple()*GetMingMaxMutiple()/100 ;

			return iGameMutiple ; 

		}
	};


	/********************************************************************************/
	//游戏数据包

	/********************************************************************************/
	//用户出牌数据包 （发向服务器）
	struct OutCardStruct
	{
		INT					iCardCount;						//扑克数目
		BYTE				iCardList[45];					//扑克信息
	};

	//代替玩家出牌
	struct ReplaceOutCardStruct
	{
		BYTE				bDeskStation;					//代替出牌的位置
		INT					iCardCount;						//扑克数目
		BYTE				iCardList[45];					//扑克信息
	};

	//用户出牌数据包 （发向客户端）
	struct OutCardMsg
	{
		INT					iNextDeskStation;				//下一出牌者
		BYTE				iCardCount;						//扑克数目
		BYTE				bDeskStation;					//当前出牌者	
		BYTE				iCardList[45];					//扑克信息
	};
	//新一轮
	struct NewTurnStruct
	{
		BYTE				bDeskStation;					//坐号
		BYTE				bReserve;						//保留
	};
	//游戏状态数据包	（ 等待东家设置状态 ）
	struct GameStation_1
	{
		//游戏版本
		BYTE				iVersion;						//游戏版本号
		BYTE				iVersion2;						//游戏版本号
		//游戏信息
	};

	//游戏状态数据包	（ 等待其他玩家开始 ）
	struct GameStation_2
	{
		BYTE				iVersion;						//游戏版本号
		BYTE				iVersion2;						//游戏版本号
		bool                bUserReady[PLAY_COUNT] ;        ///玩家是否已准备

		BYTE				iBegINTime;						//开始准备时间
		BYTE				iThinkTime;						//出牌思考时间
		BYTE				iCallScoreTime;					//叫分计时
		BYTE                iRobNTTime ;                    //抢地主时间
		BYTE				iAddDoubleTime;					//加棒时间

		INT                 iGameMutiple   ;             ///游戏中的倍数
		INT                 iAddDoubleLimit;             //加倍限制
		INT                 iGameMaxLimit ;              ///游戏最大输赢

		UINT				iCardShape;						//牌型设置
		//游戏倍数
		UINT				iDeskBasePoINT;					//桌面基础分
		UINT				iRoomBasePoINT;					//房间倍数
		LLONG				iRunPublish;					//逃跑扣分

	};


	//游戏状态数据包	（ 等待扣押底牌状态 ）
	struct GameStation_3
	{
		BYTE				iVersion;						//游戏版本号
		BYTE				iVersion2;						//游戏版本号

		BYTE				iBackCount;						//底牌数
		BYTE				iBegINTime;						//开始准备时间
		BYTE				iThinkTime;						//出牌思考时间
		BYTE                iRobNTTime ;                    //抢地主时间
		BYTE				iCallScoreTime;					//叫分计时
		BYTE				iAddDoubleTime;					//加棒时间

		BYTE				iCallScorePeople;				//当前叫分人
		BYTE				iGameFlag;						//叫分标记
		BYTE				iCallScoreResult;				//所叫的分

		INT					iUpGradePeople;					//庄家位置
		INT                 iCurOperator ;                 ///当前操作的人

		UINT				iDeskBasePoINT;					//桌面基础分
		UINT				iRoomBasePoINT;					//房间倍数

		UINT				iCardShape;						//牌型设置
		INT                 iGameMutiple   ;             ///游戏中的倍数
		INT                 iAddDoubleLimit;             //加倍限制
		INT                 iGameMaxLimit ;              ///游戏最大输赢

		bool				bAuto[PLAY_COUNT];				//托管情况
		bool				bCanleave[PLAY_COUNT];			//能否点退出
		BYTE				iCallScore[PLAY_COUNT];			//几家叫分情况
		INT 				iRobNT[PLAY_COUNT];				//抢地主情况
		INT                 iUserDoubleValue[PLAY_COUNT] ;  ///玩家加倍情况
		BYTE				iUserCardCount[PLAY_COUNT];		//用户手上扑克数目
		BYTE				iUserCardList[195];				//用户手上的扑克
		BYTE                iGameBackCard[12] ;                 ///底牌
		BYTE 				iBackCardCount;						//底牌数量
		LLONG				iRunPublish;					//逃跑扣分

		GameMutipleStruct   gameMutiple ;                      ///游戏倍数  
		GameTaskStruct      gameTask ;                         ///游戏任务


	};

	//游戏状态数据包	（ 游戏中状态 ）
	struct GameStation_4	
	{
		bool				bIsLastCard;						//是否有上轮数据

		BYTE				iVersion;							//游戏版本号
		BYTE				iVersion2;							//游戏版本号
		BYTE				iBackCount;							//底牌数

		BYTE				iBegINTime;							//开始准备时间
		BYTE				iThinkTime;							//出牌思考时间
		BYTE                iRobNTTime ;                    //抢地主时间
		BYTE				iCallScoreTime;						//叫分计时
		BYTE				iAddDoubleTime;						//加棒时间

		BYTE				bIsPass;							//是否不出

		INT					iRunPublish;						//逃跑扣分	
		INT					iBase;								//当前炸弹个数
		INT					iUpGradePeople;						//庄家位置

		INT 				iCallScoreResult;					//叫分结果
		INT					iOutCardPeople;						//现在出牌用户
		INT					iFirstOutPeople;					//先出牌的用户
		INT					iBigOutPeople;						//先出牌的用户

		UINT				iDeskBasePoINT;						//桌面基础分
		UINT				iRoomBasePoINT;						//房间倍数

		INT                 iGameMutiple   ;             ///游戏中的倍数
		UINT				iCardShape;							//牌型设置
		INT                 iAddDoubleLimit;             //加倍限制
		INT                 iGameMaxLimit ;              ///游戏最大输赢

		BYTE				iAwardPoINT[PLAY_COUNT];			//奖分
		BYTE				iPeopleBase[PLAY_COUNT];			//加棒
		BYTE				iRobNT[PLAY_COUNT];					//抢地主


		bool				bAuto[PLAY_COUNT];					//托管情况
		bool				bCanleave[PLAY_COUNT];				//能否点退出

		BYTE				iUserCardCount[PLAY_COUNT];			//用户手上扑克数目
		BYTE				iUserCardList[195];					//用户手上的扑克

		BYTE				iBaseOutCount;						//出牌的数目
		BYTE                iBaseCardList[45] ;                //桌面上的牌

		BYTE				iDeskCardCount[PLAY_COUNT];			//桌面扑克的数目
		BYTE                iDeskCardList[PLAY_COUNT][45] ;    ///桌面上的牌

		BYTE				iLastCardCount[PLAY_COUNT];			//上轮扑克的数目
		BYTE				iLastOutCard[PLAY_COUNT][45];		//上轮的扑克

		bool                bPass[PLAY_COUNT] ;                //不出
		bool                bLastTurnPass[PLAY_COUNT] ;         //上一轮不出

		BYTE                iGameBackCard[12] ;                 ///底牌
		BYTE 				iBackCardCount;						//底牌数量

		GameMutipleStruct   gameMutiple ;                      ///游戏倍数  
		GameTaskStruct      gameTask ;                         ///游戏任务

	};

	//游戏状态数据包	（ 等待下盘开始状态  ）
	struct GameStation_5
	{
		BYTE				iVersion;							//游戏版本号
		BYTE				iVersion2;							//游戏版本号
		BYTE				iBegINTime;							//开始准备时间
		BYTE				iThinkTime;							//出牌思考时间
		BYTE                iRobNTTime ;                    //抢地主时间
		BYTE				iCallScoreTime;						//叫分计时
		BYTE				iAddDoubleTime;						//加棒时间

		INT                 iGameMutiple   ;               ///游戏中的倍数
		INT                 iAddDoubleLimit;             //加倍限制
		INT                 iGameMaxLimit ;              ///游戏最大输赢

		bool                bUserReady[PLAY_COUNT] ;     ///玩家准备
		UINT				iCardShape;							//牌型设置

		UINT				iDeskBasePoINT;						//桌面基础分
		UINT				iRoomBasePoINT;						//房间倍数
		LLONG				iRunPublish;						//逃跑扣分
	};

	//用户同意游戏
	struct UserArgeeGame
	{
		BYTE				iPlayCount;							//游戏盘数
		BYTE				iCardCount;							//扑克数目
	};
	///明牌开始
	struct  UserMingStruct
	{
		bool    bStart  ;       ///是否为开始
		bool    bMing ;         ///玩家是否明牌
		BYTE    bDeskStaion ;   ///玩家的位置
		BYTE    byCardCount ;   ///明牌时玩家扑克的数量
	};
	/*----------------------------------------------------------------------*/
	//游戏开始
	struct	GameBeginStruct
	{
		BYTE				iPlayLimit;							//游戏总局数
		BYTE				iBeenPlayGame;						//已经玩了多少局
		BYTE                byUserMingBase[PLAY_COUNT] ;        ///玩家明牌倍数情况
		UINT				iCardShape;							//牌型设置

		GameBeginStruct()
		{
			memset(this, 0, sizeof(GameBeginStruct));
		}
	};
	/*----------------------------------------------------------------------*/
	//游戏准备数据包
	struct	GamePrepareStruct
	{
		BYTE				byCard;									//明牌
		GamePrepareStruct()
		{
			memset(this,255,sizeof(GamePrepareStruct));
		}
	};
	/*----------------------------------------------------------------------*/
	//发牌数据包
	struct	SendCardStruct
	{
		BYTE				bDeskStation;							//玩家
		BYTE				bCard;									//牌标号
	};
	/*----------------------------------------------------------------------*/
	//發送所有牌數據
	struct	SendAllStruct
	{
		BYTE             iUserCardCount[PLAY_COUNT];		//发牌数量
		BYTE             iUserCardList[108];				//发牌队例

		SendAllStruct()
		{
			memset(this,0,sizeof(SendAllStruct));
		}
	};
	/*----------------------------------------------------------------------*/
	//发牌结束
	struct	SendCardFinishStruct
	{
		BYTE		bReserve;	
		BYTE		byBackCardList[12];						//底牌数据
		BYTE		byUserCardCount[PLAY_COUNT];			//用户手上扑克数目
		BYTE		byUserCard[PLAY_COUNT][45];				//用户手上的扑克
		SendCardFinishStruct()
		{
			memset(this,0,sizeof(SendCardFinishStruct));
		}
	};
	/*----------------------------------------------------------------------*/
	//底牌数据包
	struct BackCardExStruct
	{
		BYTE				iGiveBackPeople;				//底牌玩家
		BYTE				iBackCardCount;					//扑克数目
		BYTE				iBackCard[12];					//底牌数据

		GameTaskStruct      gameTask ;                      //游戏任务  

	};
	//游戏开始数据包
	struct BeginPlayStruct
	{
		BYTE				iOutDeskStation;				//出牌的位置
	};


	//用户甩牌失败数据包 （发向客户端）
	struct OutShowCardStruct
	{
		BYTE				iCardCount;						//扑克数目
		BYTE				bDeskStation;					//当前出牌者
		BYTE				iResultCount;					//结果扑克数目
		BYTE				iCardList[78];					//扑克信息
	};

	//代替开始
	struct ReplaceAgreeStruct
	{
		BYTE bDeskStation;									
	};

	//叫分数据包
	struct CallScoreStruct
	{	
		BYTE				bDeskStation;							//当前叫分者
		INT					iValue;									//叫分类型
		bool 				bCallScoreflag;							//叫分标记								
	};
	/*----------------------------------------------------------------------*/
	//抢地主
	struct	RobNTStruct
	{
		BYTE   byDeskStation;		//抢地主坐号
		BYTE   byRobCount ;         //玩家抢地主次数
		INT    iValue;				//抢地主情况(0-叫地主状态 1-抢地主状态)
		RobNTStruct()
		{
			memset(this, 0, sizeof(RobNTStruct));
		}
	};
	/*----------------------------------------------------------------------*/
	//明牌
	struct ShowCardStruct
	{
		BYTE bDeskStation;										//坐号
		BYTE iCardList[54];					                   //扑克信息
		BYTE iCardCount;						              //扑克数目
		INT iValue;											  //保留
		INT iBase;											  //倍数 20081204
	};

	//代替亮牌
	struct ReplaceShowCardStruct
	{
		BYTE bDeskStation;										//坐号
		BYTE bRepDeskStation;									//代替位置
		INT iValue;												//保留
	};
	//加倍
	struct AddDoubleStruct
	{
		BYTE bDeskStation;										//加倍位置
		INT iValue;												//加倍情况
	};

	//代替叫分数据包
	struct ReplaceCallScoreStruct
	{	
		BYTE		bDeskStation;								//代替叫分者
		INT		    NowCallScorePeople;							//当前叫分者
		INT			CallScore;									//叫分类型
		bool 		CallScoreFlag;								//叫分标记								
	};

	//奖分
	struct AwardPoINTStruct
	{
		BYTE	iAwardPoINT;									//奖分
		BYTE	bDeskStation;									//坐号
		INT		iBase;											//倍数
	};

	//托管数据结构
	struct AutoStruct
	{
		BYTE bDeskStation;
		bool bAuto;
	};

	//机器人托管
	struct UseAIStation
	{
		BYTE bDeskStation;					//坐号
		INT bState;						//状态
	};

	//冠军结果
	struct ChampionStruct
	{
		BYTE bDeskStation ;
	};

	//用户出牌数据包 （发向客户端）
	struct BatchOutCardMsg
	{
		BYTE				iCardCount;						//扑克数目
		BYTE				iCardList[156];					//扑克信息
	};

	////道俱
	//struct PropStruct
	//{
	//	BYTE				sendDeskStation;				//发放位置
	//	BYTE				recvDeskStation;				//接收位置
	//	INT					iValue;
	//};
	//游戏结束统计数据包
	struct GameEndStruct
	{
		bool                bFinishTask ;                    //是否完成了任务
		BYTE				iUpGradeStation;				//庄家位置
		BYTE			    iUserCardCount[PLAY_COUNT];	    //用户手上扑克数目
		BYTE			    iUserCard[PLAY_COUNT][45];		//用户手上的扑克

		GameMutipleStruct   gameMutiple ; 

		LLONG  			iTurePoINT[PLAY_COUNT];			//玩家得分
		LLONG				iChangeMoney[8];				//玩家金币

		GameEndStruct()
		{
			memset(this , 0 ,sizeof(GameEndStruct)) ; 
		}

	};

	//游戏结束统计数据包
	struct GameCutStruct
	{
		INT					iRoomBasePoINT;					//倍数
		INT					iDeskBasePoINT;					//桌面倍数
		INT					iHumanBasePoINT;				//人头倍数

		INT					bDeskStation;					//退出位置
		LLONG				iChangeMoney[8];				//玩家金币
		LLONG				iTurePoINT[PLAY_COUNT];			//庄家得分

	};

	struct HaveThingStruct
	{
		BYTE pos;
		char szMessage[61];
	};

	struct LeaveResultStruct
	{
		BYTE bDeskStation;
		INT bArgeeLeave;
	};

	struct MessageStruct	//客户端到服务器
	{
		char Message[200];
	};

	struct StopResultStruct
	{
		bool bArgee;
	};

	//游戏结算
	struct GameFinishNotify
	{
		CHAR name[PLAY_COUNT][61];
		//	INT	iBasePoINT;
		BYTE iStyle;			//游戏类型是否为强退,还是正常结束
		INT	iBasePoINT;			//系统基数
		INT	iDeskBasePoINT;		//桌面基础分
		INT iUpGradePeople;		//莊家
		INT iGrade2;			//莊家盟友
		LLONG	iWardPoINT[PLAY_COUNT];
		LLONG iAwardPoINT[PLAY_COUNT];	//讨赏
		LLONG iTotalScore[PLAY_COUNT];	//总积分
		LLONG iMoney[PLAY_COUNT];			//比赛所留钱数
		BYTE iGameStyle;					//游戏类型
	};

	struct UserleftDesk
	{
		BYTE bDeskStation;                //断线玩家
	};

#pragma pack()
	//-------------------------------------------------------------------------------
	//超端信息结构体
	struct	SuperUserMsg
	{
		BYTE	byDeskStation;	
		bool	bIsSuper;
		SuperUserMsg()
		{
			memset(this,0,sizeof(SuperUserMsg));
		}
	};
	//-------------------------------------------------------------------------------
	/********************************************************************************/

	// 数据包处理辅助标识
	//warning:79号ID预留给超级客户端,永不使用
	/********************************************************************************/

	enum Net_Cmd
	{
		ASS_UG_USER_SET				=	50,				//设置游戏
		ASS_GAME_BEGIN				=	51,				//游戏开始
		ASS_GAME_PREPARE			=	52,				//准备工作
		
		ASS_SEND_CARD				=	53,				//发牌信息
		ASS_SEND_CARD_MSG			=	54,				//发牌过程中处理消息
		ASS_SEND_ALL_CARD			=	55,				//发送所有牌(一下发放全部)
		ASS_SEND_FINISH				=	56,				//发牌完成
		ASS_CALL_SCORE				=	57,				//叫分
		ASS_CALL_SCORE_RESULT		=	58,				//叫分结果
		ASS_CALL_SCORE_FINISH		=	59,				//叫分结束
		ASS_REPLACE_CALL_SCORE		=	60,				//代替叫分
		
		ASS_ROB_NT					=	61,				//抢地主
		ASS_ROB_NT_RESULT			=	62,				//抢地主结果
		ASS_REPLACE_ROB_NT			=	63,				//代替抢地主
		ASS_GAME_MULTIPLE			=	64,				//游戏倍数(抢地主后会加倍)
		ASS_ROB_NT_FINISH			=	65,				//抢地主结果
		
		ASS_BACK_CARD				=	66,				//底牌数据
		ASS_BACK_CARD_EX			=	67,				//扩展底牌数据(其他玩家可见)
		ASS_ADD_DOUBLE				=	68,				//加倍
		ASS_ADD_DOUBLE_RESULT		=	69,				//加倍结果
		ASS_REPLACE_ADD_DOUBLE		=	70,				//代替加棒
		ASS_ADD_DOUBLE_FINISH		=	71,				//加棒结束
		ASS_SHOW_CARD				=	72,				//亮牌
		ASS_SHOW_CARD_RESULT		=	73,				//亮牌结果
		ASS_REPLACE_SHOW_CARD		=	74,				//代替亮牌
		ASS_SHOW_CARD_FINISH		=	75,				//亮牌结束
		
		ASS_GAME_PLAY				=	76,				//开始游戏
		ASS_OUT_CARD				=	77,				//用户出牌
		ASS_OUT_CARD_RESULT			=	78,				//出牌結果
		
		ASS_SUPER_USER				=	79,				//超端消息
		
		
		ASS_REPLACE_OUT_CARD		=	80,				//代替出牌(79留给超级客户端发牌器)
		ASS_ONE_TURN_OVER			=	81,				//一轮完成(使客户端上一轮可用)
		ASS_NEW_TURN				=	82,				//新一轮开始
		ASS_AWARD_POINT				=	83,				//奖分(炸弹火箭)
		
		ASS_CONTINUE_END			=	84,				//游戏结束
		ASS_NO_CONTINUE_END			=	85,				//游戏结束
		ASS_NO_CALL_SCORE_END		=	86,				//无人叫分
		ASS_CUT_END					=	87,				//用户强行离开
		ASS_SAFE_END				=	88,				//游戏安全结束
		ASS_TERMINATE_END			=	89,				//意外结束
		ASS_AHEAD_END				=	90,				//提前结束
		ASS_AUTO					=	91,				//用户托管
		ASS_HAVE_THING				=	92,				//有事
		ASS_LEFT_RESULT				=	93,	     		//有事离开结果
		ASS_MESSAGE					=	94,				//文字消息
		ASS_AI_STATION				=	95,				//机器人托管(断线户用)
		ASS_REPLACE_GM_AGREE_GAME	=	96,				//代替玩家开始
		ASS_USER_LEFTDESK           =   97,               //玩家离开桌子或断线
	};

}


/********************************************************************************/
#endif
