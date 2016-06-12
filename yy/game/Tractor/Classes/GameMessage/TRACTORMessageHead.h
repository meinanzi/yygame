#ifndef TRACTORUpgradeMessage_h__
#define TRACTORUpgradeMessage_h__
#include "HNBaseType.h"

namespace TRACTOR
{

	//同楼 普通版/视频版 编译  release/视频版  ，提牌器编译提牌器版

	//游戏信息
	enum 
	{
		GAME_NAME_ID			=			12111006,		// 名字 ID
	};


	//版本定义
	enum
	{
		//GAME_MAX_VER		=			1,								//现在最高版本
		//GAME_LESS_VER		=			1,								//现在最低版本
		//GAME_CHANGE_VER		=			0,								//修改版本

	//游戏信息

		PLAY_COUNT			=			6,							//游戏人数
		MAX_CARD_COUNT      =            3,

	//游戏状态定义
		GS_WAIT_SETGAME		=			0,				//等待东家设置状态
		GS_WAIT_ARGEE		=			1,				//等待同意设置
		GS_SEND_CARD		=			20,				//发牌状态
		GS_WAIT_BACK		=			21,				//等待扣压底牌
		GS_PLAY_GAME		=			22,				//游戏中状态
		GS_WAIT_NEXT		=			23,				//等待下一盘开始 

	//按注类型

		TYPE_GIVE_UP		=			0x00,			//放弃
		TYPE_NOTE			=			0x01,			//下注
		TYPE_ADD			=			0x02,			//加注
		TYPE_FOLLOW			=			0x03,			//跟注
		TYPE_OPENCARD		=			0x04,			//开牌
		TYPE_LOOKCARD		=			0x05,			//看牌
		TYPE_NORMAL         =            0x06,            //正常状态
		
		TYPE_OPEN_LOSE      =            0x06 ,           //开牌失败
		TYPE_OPEN_WIN       =            0x07,            //开牌胜利
		TYPE_REACH_LIMITE_NEXT     =     0x08 ,           //下一家下注后达到上限
		TYPE_REACH_LIMITE_SELF     =     0x09 ,           //自己下注后达到上限
		TYPE_COMPARE_CARD          =     0x10  ,          //比牌操作
		
		SH_THREE              =  7, //三条
		SH_SAME_HUA_CONTINUE  =  6, //同花顺
		SH_SAME_HUA           =  5, //同花
		SH_CONTIUE            =  4 ,//顺子
		SH_DOUBLE             =  3, //对子
		SH_SPECIAL            =  1 ,//特殊235
		SH_OTHER              =  2, //单牌
		SH_ERROR              =  0, //错误
	};
	/********************************************************************************/

	//游戏数据包

	/********************************************************************************/
#pragma pack(push ,1)
	//发送用户结果
	struct tagUserResult
	{
		BYTE bCountNotePeople;		     //当前未放弃玩家数
		INT bAddTime;				     //押注次数
		BYTE iNowBigNoteStyle;		     //下注类型
		BYTE iNowBigPeople;
		BYTE iCutOutLastpeople;
		BYTE iCutOutLastType;
		BYTE iOutPeople;			     //下注者
		INT iNowBigNote;			     //当前下注
		INT iTotalNote;				     //总下注
		INT iUserStation[PLAY_COUNT];    //玩家下注类型
		INT iFirstOutPeople;             //第一下注玩家
		LLONG iUserNote[PLAY_COUNT];     //用户本轮下注
		bool bIsFirstNote;
		tagUserResult()
		{
			memset(this,0,sizeof(tagUserResult));
		}
	};
	//看牌数据

	struct lookCardResult
	{
		BYTE             bDeskStation;
		BYTE				iUserCardCount;				//用户手上的牌数
		BYTE				iUserCard[5];				//用户手上的牌
		lookCardResult()
		{
			memset(this,0,sizeof(lookCardResult));
		}
	};


	struct GameStationBase
	{
		BYTE bGameStation;
		GameStationBase()
		{
			memset(this,0,sizeof(GameStationBase));
		}
	};
	//游戏状态数据包	（ 等待东家设置状态 ）
	struct GameStation_1 : GameStationBase
	{
		//游戏版本
		BYTE				iVersion;						//游戏版本号
		BYTE				iVersion2;						//游戏版本号
		GameStation_1()
		{
			memset(this,0,sizeof(GameStation_1));
		}
	};

	//游戏状态数据包	（ 等待其他玩家开始 ）
	struct GameStation_2 : GameStationBase
	{
		BYTE                bEXFlag[12];                    //zht 2011-4-12, 不使用的字段，占位用
		BYTE				iVersion;						//游戏版本号
		BYTE				iVersion2;						//游戏版本号
		BYTE				iThinkTime;						//游戏思考时间
		BYTE				iBeginTime;						//游戏开始时间
		BYTE				iAllCardCount;				    //扑克数目
		INT					iRoomBasePoint;					//房间倍数
		INT					iRunPublish;					//逃跑扣分
		BYTE                bEXFlag1[12];                    // 不使用的字段，占位用
		//   房间相关信息
		LLONG				iLimitNote;						//下注封顶值   明注 / 暗注
		INT					iBaseNote;						//底注	       暗注
		INT                 iGuoDi;                         //锅底
		LLONG               iLimtePerNote;                  //暗注上限，明注需要 * 2
		INT                bShowIdOrPlay;                  //显示玩家还是显示id
		GameStation_2()
		{
			memset(this,0,sizeof(GameStation_2));
		}
	};

	//游戏状态数据包	（ 游戏正在发牌状态 ）
	struct GameStation_3 : GameStationBase
	{
		BYTE                bEXFlag[12];                    //zht 2011-4-12, 不使用的字段，占位用
		BYTE				iVersion;						//游戏版本号
		BYTE				iVersion2;						//游戏版本号
		BYTE				iThinkTime;						//游戏盘数
		BYTE				iBeginTime;						//游戏盘数
		BYTE				iAllCardCount;					//扑克数目
		BYTE				iBeenPlayGame;					//已经游戏的局数
		BYTE				iSysCardCount;					//还剩下牌数量
		BYTE                bEXFlag1[12];                   //zht 2011-4-12, 不使用的字段，占位用
		BYTE				iUserCardCount[PLAY_COUNT];		//用户手上的牌数
		BYTE				iUserCard[PLAY_COUNT][5];		//用户手上的牌	
		INT					iUpGradePeople;					//庄家位置
		INT					iRoomBasePoint;					//房间倍数
		INT					iRunPublish;					//逃跑扣分

		LLONG			    iLimitNote;						//下注封顶值   明注 / 暗注
		INT					iBaseNote;						//底注	       暗注
		INT                 iGuoDi;                         //锅底
		LLONG               iLimtePerNote;                  //暗注上限，明注需要 * 2
		LLONG				iTotalNote[PLAY_COUNT];			//用户当前下注
		INT                bShowIdOrPlay;
		CHAR                strCityText[PLAY_COUNT][50];    //超级玩家随机地址
		CHAR                strUserIDText[PLAY_COUNT][50];  //超级玩家随机ID
		bool                bSuperUser[PLAY_COUNT];         //该玩家是否是超级玩家
		INT                bPlayer[PLAY_COUNT];            //是否玩家或中途进入观战者
		GameStation_3()
		{
			memset(this,0,sizeof(GameStation_3));
		}
	};

	//游戏状态数据包	（ 游戏中状态 ）
	struct GameStation_4 : GameStationBase
	{
		BYTE                bEXFlag[12];                    //zht 2011-4-12, 不使用的字段，占位用
		BYTE				iVersion;						//游戏版本号
		BYTE				iVersion2;						//游戏版本号
		BYTE				iThinkTime;						//游戏盘数
		BYTE				iBeginTime;						//游戏盘数
		BYTE				iAllCardCount;					//扑克数目
		BYTE				iBeenPlayGame;					//已经游戏的局数
		BYTE				SysCard[28];					//还剩下的牌
		BYTE				iSysCardCount;					//还剩下牌数量
		BYTE                bEXFlag2[12];                   //zht 2011-4-12, 不使用的字段，占位用
		LLONG				iLimitNote;						//下注封顶值   明注 / 暗注
		INT					iBaseNote;						//底注	       暗注
		INT                 iGuoDi;                         //锅底
		LLONG               iLimtePerNote;                  //暗注上限，明注需要 * 2
		INT                 iCurNote;                       //上一位玩家下注值，已暗注计算
		INT					iRoomBasePoint;					//房间倍数
		INT					iRunPublish;					//逃跑扣分
		BYTE                bNtPeople;                      //庄家位置
		BYTE				iUpGradePeople;					//下注位置
		BYTE				iOutCardPeople;					//现在出牌用户
		BYTE				iFirstOutPeople;				//先出牌的用户
		LLONG				iTotalNote[PLAY_COUNT];			//用户当前下注
		LLONG               iThisGameNote[PLAY_COUNT];		//本局游戏下注数(明注)
		LLONG				iTotalGameNote[PLAY_COUNT];
		INT                iMing[PLAY_COUNT];              //明牌状态
		bool                bOpenLose[PLAY_COUNT];          //玩家比牌是否输了
		bool                bIsGiveUp[PLAY_COUNT];                      //玩家是否放弃
		bool                bIsFirstNote;                   //是否为第一次下注
		INT				    iTimeRest;						// 定时器实际剩下的时间，用于表示断线重连时剩余时间值
		CHAR                strCityText[PLAY_COUNT][50];    //超级玩家随机地址
		CHAR                strUserIDText[PLAY_COUNT][50];  //超级玩家随机ID
		bool                bSuperUser[PLAY_COUNT];         //该玩家是否是超级玩家
		tagUserResult		tag;
		BYTE				iUserCardCount[PLAY_COUNT];	    //用户手上的牌数
		BYTE				iUserCard[PLAY_COUNT][5];	    //用户手上的牌
		bool                bOpenLoser[PLAY_COUNT];
		INT                bPlayer[PLAY_COUNT];
		BYTE                byWin[PLAY_COUNT];
		INT                bShowIdOrPlay;
		GameStation_4()
		{
			memset(this,0,sizeof(GameStation_4));
		}
	};

	//游戏状态数据包	（ 等待下盘开始状态  ）
	struct GameStation_5 : GameStationBase
	{
		INT                 bShowIdOrPlay;
		bool                bSuperUser[PLAY_COUNT];             //该玩家是否是超级玩家
		//游戏版本
		BYTE				iVersion;						//游戏版本号
		BYTE				iVersion2;						//游戏版本号
		BYTE				iBeenPlayGame;					//已经游戏的局数
		BYTE                bEXFlag[12];                    //不使用的字段，占位用
		//游戏信息
		BYTE				iThinkTime;						//游戏盘数
		BYTE				iBeginTime;						//游戏盘数
		BYTE				iAllCardCount;				//扑克数目
		BYTE                bEXFlag2[12];                    //不使用的字段，占位用
		INT					iRoomBasePoint;					//房间倍数
		INT					iRunPublish;					//逃跑扣分
		INT					iBaseNote;						//底注	暗注
		INT                 iGuoDi;                         //锅底
		LLONG				iLimitNote;						//下注封顶值   明注 / 暗注
		LLONG               iLimtePerNote;                   //暗注上限，明注需要 * 2
		//运行状态变量
		INT					iUpGradePeople;					//庄家位置
		GameStation_5()
		{
			memset(this,0,sizeof(GameStation_5));
		}
	};

	//用户同意游戏
	struct UserArgeeGame
	{
		BYTE				iBeginStation;					//开始位置
		BYTE				iEndStation;					//结束位置
		BYTE				iPlayCount;						//游戏盘数
		BYTE				iCardCount;						//扑克数目
		UserArgeeGame()
		{
			memset(this,0,sizeof(UserArgeeGame));
		}
	};

	//游戏开始
	struct BeginUpgradeStruct
	{
		INT                 bIsShowIdOrPlay;                  //显示id还是玩家
		bool                bSuperUser[PLAY_COUNT];           //该玩家是否是超级玩家
		CHAR                strCityText[PLAY_COUNT][50];
		CHAR                strUserIDText[PLAY_COUNT][50];
		BYTE                bNtStation;                      //庄家 
		BYTE				AllCardCount;					 //一共使用的牌数
		BYTE				SysCard[54];					 //当前牌
		INT					iBaseNote;						 //底注	
		INT                 iGuoDi;                          //锅底
		LLONG				iLimitNote;						 //最大注数
		LLONG               iLimtePerNote;                   //暗注上限，明注需要 * 2   
		BeginUpgradeStruct()
		{
			memset(this,0,sizeof(BeginUpgradeStruct));
		}
	};
	//收集筹码动画
	struct CollectJetonStruct
	{
		BYTE bDeskStation;
		INT JetonTotal;
		CollectJetonStruct()
		{
			memset(this,0,sizeof(CollectJetonStruct));
		}
	};
	//发牌数据包
	struct SendCardStruct
	{
		BYTE bCard[PLAY_COUNT][MAX_CARD_COUNT];	//牌标号
		BYTE bCardCount[PLAY_COUNT];     //牌张数
		SendCardStruct()
		{
			memset(this,0,sizeof(SendCardStruct));
		}
	};
	//游戏开始数据包
	struct BeginPlayStruct
	{
		BYTE				iOutDeskStation;				//出牌的位置
		BYTE                bNtPeople;                      //庄家位置
		BYTE				byUserData[PLAY_COUNT][2];		//确定玩家是否机器人和玩家的牌大小排名（只发给机器人)
		BeginPlayStruct()
		{
			memset(this,0,sizeof(BeginPlayStruct));
		}
	};

	//用户出牌数据包 （发向服务器）
	struct OutCardStruct
	{
		BYTE				iCardList[39];					//扑克信息
		INT					iCardCount;						//扑克数目
		OutCardStruct()
		{
			memset(this,0,sizeof(OutCardStruct));
		}
	};



	//用户出牌数据包 （发向客户端）
	struct OutCardMsg
	{
		BYTE				iCardCount;						//扑克数目
		BYTE				bDeskStation;					//当前出牌者
		BYTE				iCardList[39];					//扑克信息
		INT					iNextDeskStation;				//下一出牌者
		OutCardMsg()
		{
			memset(this,0,sizeof(OutCardMsg));
		}
	};

	//游戏结束统计数据包
	struct GameEndStruct
	{
		BYTE				iUpGradeStation;						//庄家位置
		BYTE				bCard[PLAY_COUNT][MAX_CARD_COUNT];		//牌标号
		BYTE				iCardShape[PLAY_COUNT];
		INT					iUserState[PLAY_COUNT];					//玩家状态(提前放弃,还是梭)
		LLONG				iTurePoint[PLAY_COUNT];					//庄家得分
		LLONG				iChangeMoney[8];
		GameEndStruct()
		{
			memset(this,0,sizeof(GameEndStruct));
		}
	};

	//游戏结束统计数据包
	struct GameCutStruct
	{
		INT					bDeskStation;					//退出位置
		LLONG					iTurePoint[PLAY_COUNT];					//庄家得分
		LLONG					iChangeMoney[8];
	};

	struct HaveThingStruct
	{
		BYTE pos;
		CHAR szMessage[61];
	};

	struct LeaveResultStruct
	{
		BYTE bDeskStation;
		INT bArgeeLeave;
	};

	struct MessageStruct	//客户端到服务器
	{
		CHAR Message[200];
	};

	struct StopResultStruct
	{
		bool bArgee;
	};
	//配牌数据包
	struct SetCardStruct
	{
		BYTE bCardCount;//牌总数
		BYTE bPlayCount;//玩家数
		INT  nPlayCardCount[8]; //每个玩家配牌数
		BYTE bPlayCard[8][60]; // 每个玩家的牌
		SetCardStruct()
		{
			memset(this,0,sizeof(SetCardStruct));
		}
	};
	/*
	//按钮类型
	enum BUTTON_TYPE
	{
	BUTTON_TYPE_NONE = -1,
	BUTTON_TYPE_GIVEUP,//放弃
	BUTTON_TYPE_100NOTE,//100注
	BUTTON_TYPE_200NOTE,//200注
	BUTTON_TYPE_400NOTE,//400注
	BUTTON_TYPE_ADD,	//加注
	BUTTON_TYPE_FOLLOW,//跟了
	BUTTON_TYPE_SHUTTLE,//梭了
	};*/

	//用户处理信息
	struct tagUserProcess
	{
		BYTE iVrebType;//所处理的按钮
		INT  iNote;		//下注数
		INT  bMing;     //明牌否?
		tagUserProcess()
		{
			memset(this,0,sizeof(tagUserProcess));
		}
	};

	struct tagCompareCard :  public tagUserProcess 
	{
		BYTE byComparedStation ;  //被对比牌的玩家   
	};

	struct tagCompareCardResult
	{
		BYTE iNt;		//比牌者
		BYTE iNoNt;     //被比者
		BYTE iLoster;	//败者
		BYTE bWinner;		// Add By ZXD 20090820 胜利者，若比牌结束后，下一家马上达到下注上限，则此时客户端需要知道胜利者是谁
		INT iCurNote;		//当前玩家下注数
		INT iNote;          //当前有效注数
		BYTE bAddStyle;   //类型
		bool bGameFinish;
		tagCompareCardResult()
		{
			memset(this,0,sizeof(tagCompareCardResult));
		}
	};

	//代替断线用户处理信息
	struct SupersedeUserNote
	{
		BYTE bDeskStation;//代替的位置
		BYTE iVrebType;//所处理的按钮
		INT  iNote;		//下注数
		SupersedeUserNote()
		{
			memset(this,0,sizeof(SupersedeUserNote));
		}
	};
	//換底牌數據結果
	struct ChangeBackStruct
	{
		BYTE bDeskStation;//位置
		BYTE bSourceCard;//源牌
		BYTE bResultCard;//結果牌
		INT  bExtVal;	//是否替換成功
		ChangeBackStruct()
		{
			memset(this,0,sizeof(ChangeBackStruct));
		}
	};

	//发送用户押注
	struct NoteResult
	{
		BYTE bAddStyle;		          //下注类型
		BYTE iOutPeople;	          //下注者
		INT iCurNote;		          //当前玩家下注数
		INT iNote;                    //当前有效注数
		BYTE bNextDeskStation;
		LLONG  iLimiteNote;           //玩家限注
		NoteResult()
		{
			memset(this,0,sizeof(NoteResult));
		}
	};
	struct GameFinishNotify
	{
		CHAR name[PLAY_COUNT][61];
		//int dwUserID[PLAY_COUNT];         //打印userid
		INT	iBasePoint;
		BYTE iStyle;			//游戏类型是否为强退,还是正常结束
		INT	iWardPoint[PLAY_COUNT];
		//LLONG iTotalPoint[4];
		BYTE iCardShape[PLAY_COUNT];
		LLONG iMoney[PLAY_COUNT];
		GameFinishNotify()
		{
			memset(this,0,sizeof(GameFinishNotify));
		}
	};
	//超端用户控制的结果
	struct	SuperUserControlReslut
	{
		BYTE    bWinStation;		//输位置
		BYTE    bLoseStation;		//赢位置
		SuperUserControlReslut()
		{
			memset(this,-1,sizeof(SuperUserControlReslut));
		}
	};
	//-------------------------------------------------------------------------------
	/// 通知客户端是否为超级客户端状态消息结构
	typedef struct SuperUserState
	{
		BYTE byDeskStation;      /**< 玩家位置 */
		bool bEnable;           /**< 是否开通 */
		SuperUserState()
		{
			memset(this,0,sizeof(SuperUserState));
		}
	}SUPERSTATE;


	/*
	//放弃结果数据
	struct tagUserGiveUp
	{
	bool bShowOnly;		//仅显示
	bool bAddFlag;	//加倍标记	
	BYTE bAddTime;	//押注次数
	BYTE bAddStyle;	//下注类型	
	BYTE iGiveUpPeo;//放弃者
	BYTE iOutPeople;//下注者
	//	tagButtonStatus ButtonPara;//按钮参数
	};
	*/
	/********************************************************************************/

	// 数据包处理辅助标识

	/********************************************************************************/
	enum{
		ASS_UG_USER_SET			=		50,				//用户设置游戏
		ASS_BEGIN_UPGRADE		=		51,				//升级游戏开始
		ASS_SEND_CARD			=		52,				//发牌信息
		ASS_SEND_FINISH			=		54,				//发牌完成
		ASS_GAME_PLAY			=		59	,			//开始游戏
		ASS_NEW_TURN			=		64	,			//新一轮开始
		ASS_CONTINUE_END		=		65	,			//游戏结束
		ASS_NO_CONTINUE_END		=		66	,			//游戏结束
		ASS_CUT_END				=		67	,			//用户强行离开
		ASS_SALE_END			=		68	,			//游戏安全结束
		
		
		ASS_VREB_CHECK			=		179	,			//用户处理
		
		ASS_SHUTTLE				=		80	,			//梭了
		ASS_ADD_NOTE			=		82	,			//加注
		ASS_GIVE_UP				=		86	,			//放弃
		ASS_MODIFY_PLAY_NOTE	=		90	,			//更新用户所下注
		ASS_NOTE				=		91	,			//下注
		ASS_NOTE_RESULT			=		92	,			//下注结果
		ASS_COLLECT_JETON		=		94	,			//收集筹码
		ASS_BIPAI_RESULT        =        95  ,            //比牌结果
		ASS_GM_SUPERCLIENT		=		100	,			//超級客戶端
		ASS_FINISH_COMPARE      =        101 ,            
		ASS_AI_WIN              =        102 ,            //机器人必赢
		ASS_SET_TEST_CARD       =        120 ,            //发牌器设定牌
		ASS_SUPER_RESULT        =        121 ,            //超端控制结果消息
		ASS_SUPER_USER          =        122  ,           //超端验证消息
	};
}

#pragma pack(pop)
#endif // TRACTORUpgradeMessage_h__
/********************************************************************************/