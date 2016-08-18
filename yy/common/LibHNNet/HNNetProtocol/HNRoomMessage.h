#ifndef _HN_RoomMessage_H__
#define _HN_RoomMessage_H__

#include "HNComStruct.h"

/*********************************************************************************/
#define MAX_TALK_LEN				   500						//最大聊天数据长度
	

///	通信标识定义 

/*********************************************************************************/
//连接消息
#define MDM_GR_CONNECT					1						//连接消息类型
#define	ASS_GR_CONNECT_SUCCESS			3						


//登录游戏房间的相关定义
/*--------------------------------------------------------------------------------*/
#define	MDM_GR_LOGON					100						//登陆游戏房间

#define MDM_GR_QUEUE_MSG				114						///排队消息

//登录返回结果
#define ASS_GR_LOGON_SUCCESS			2						//登陆成功
#define ASS_GR_LOGON_ERROR				3						//登陆失败
#define ASS_GR_SEND_FINISH				4						//登陆完成
#define ASS_GR_LOGON_BY_ID				5						//通过用户ID登陆
#define ASS_GR_IS_VIPROOM               16                      //是否VIP房间
#define ASS_GR_VIP_PW                   17                      //VIP房间需要密码
#define ASS_GR_VIP_NO_PW                18                      //VIP房间不需要密码(第一个进入不需要密码)
#define ASS_GR_NO_VIP                   19                      //不是VIP房间
#define ASS_GR_VIPROOM_PW               20                      //VIP房间密码
#define ASS_GR_VIPROOM_PW_RIGHT         21                      //VIP房间密码正确

/*----------------------------------------------------------------------------------*/
//服务端主动推送
//用户列表主ID
#define MDM_GR_USER_LIST				101						//登录成功后返回个人信息以及该房间玩家信息
//用户列表辅助ID
#define ASS_GR_ONLINE_USER				1						//在线用户
#define ASS_GR_NETCUT_USER				2						//断线用户
#define ASS_GR_DESK_STATION				3						//桌子状态
/*-----------------------------------------------------------------------------------*/

//客户端发送

//玩家动作
#define MDM_GR_USER_ACTION				102						//用户动作，玩家坐桌，起身，断线等都是以该值为主标志
//玩家动作辅助ID
#define ASS_GR_USER_UP					1						//用户起来
#define ASS_GR_USER_SIT					2						//用户坐下
#define ASS_GR_WATCH_UP					3						//旁观起来
#define ASS_GR_WATCH_SIT				4						//旁观坐下
#define ASS_GR_USER_COME				5						//用户进入
#define ASS_GR_USER_LEFT				6						//用户离开
#define ASS_GR_USER_CUT					7						//用户断线
#define ASS_GR_SIT_ERROR				8						//坐下错误
#define ASS_GR_SET_TABLE_BASEPOINT		9						//改变桌子倍数
#define ASS_GR_USER_HIT_BEGIN			10						//用户同意开始
#define ASS_GR_JOIN_QUEUE				11						//加入排队机
#define ASS_GR_QUIT_QUEUE				12						//退出排队机
#define ASS_GR_QUEUE_USER_SIT			13						//排队用户坐下
#define ASS_GR_LEASE_TIMEOVER			14						//排队用户坐下
#define ASS_GR_CONTEST_CANCEL			16						//比赛场退赛
#define ASS_GR_CONTEST_APPLY			24						//比赛场报名
#define ASS_GR_FAST_JOIN_IN			    27		                //快速坐桌


//玩家动作部分操作结果：（专用于比赛房间）
#define ERR_GR_APPLY_SUCCEEDED			1						//参赛或退赛成功
#define ERR_GR_APPLY_ALREADYSIGN		2						//已经报名
#define ERR_GR_APPLY_BEGIN				3						//比赛已经开始
#define ERR_GR_APPLY_NOTENOUPH_MONEY	4						//由于钱包金币不够，导致报名失败


//房间信息
#define MDM_GR_ROOM							103		
//房间信息辅助ID
#define	ASS_GR_NORMAL_TALK				1					//普通聊天
#define	ASS_GR_HIGH_TALK				2					//高级聊天
#define	ASS_GR_USER_AGREE				3					//用户同意
#define	ASS_GR_GAME_BEGIN				4					//游戏开始
#define	ASS_GR_GAME_END					5					//游戏结束
#define	ASS_GR_USER_POINT				6					//用户经验值
#define	ASS_GR_SHORT_MSG				7					//聊短信息
#define	ASS_GR_ROOM_SET					8					//房间设置
#define	ASS_GR_INVITEUSER				9					//邀请用户
#define	ASS_GR_INSTANT_UPDATE			10					//即时更新分数金币
#define	ASS_GR_UPDATE_CHARM				11					//即时更新魅力
#define	ASS_GR_ROOM_PASSWORD_SET		12					//房间设置
#define	ASS_GR_ROOM_QUEUE_READY			13					//排队机准备
#define	ASS_GR_GET_NICKNAME_ONID        14					//根据ID获取昵称
#define	ASS_GR_OWNER_T_ONE_LEFT_ROOM    15					//房主踢玩家离开房间
#define	ASS_GR_GET_NICKNAME_ONID_INGAME 16					//根据ID获取昵称 游戏中
#define	ASS_GR_USER_CONTEST				17					//用户比赛信息，发送NET_ROOM_CONTEST_CHANGE_RESULT，排名有更改
#define	ASS_GR_AVATAR_LOGO_CHANGE		18					//用户形象更改信息
#define	ASS_GR_CAHNGE_ROOM				19					//比赛开始，未报名用户切换房间
#define	ASS_GR_CONTEST_GAMEOVER			20					//比赛结束
#define	ASS_GR_CONTEST_KICK				21					//用户被淘汰
#define	ASS_GR_CONTEST_WAIT_GAMEOVER	22					//比赛结束，但是有用户还在打最后一局，通知已打完的用户等待排名
#define	ASS_GR_INIT_CONTEST				23					//比赛开始，初始化比赛信息，发送NET_ROOM_CONTEST_CHANGE_RESULT，初始化排名信息
#define	ASS_GR_CONTEST_PEOPLE			24					//获取报名数量，登陆完成后服务端主动发送NET_ROOM_CONTEST_PEOPLE_RESULT
#define	ASS_GR_CONTEST_RECORD			25					//个人参赛纪录，登陆完成后服务端主动发送NET_ROOM_CONTEST_RECORD_RESULT
#define	ASS_GR_CONTEST_AWARDS			26					//比赛奖励，一连串的NET_ROOM_CONTEST_AWARD_RESULT
#define	ASS_GR_CONTEST_GETINFO			27					//获取比赛信息（服务端返回24,25,26三个消息包）

#define MDM_GR_PROP							160					//房间和游戏中道具相关的消息
//道具辅助消息
#define ASS_PROP_GETUSERPROP			0x01				//取得服务器上存储的个人道具
#define ASS_PROP_GETPROPINFO			0x02				//取得某个道具的信息
#define ASS_PROP_BUY							0x03				//购买道具
#define ASS_PROP_GIVE							0x04				//赠送道具
#define ASS_PROP_VIP_CHECKTIME			0x05				//检查VIP时间
#define ASS_PROP_KICKUSER					0x06				//踢人
#define ASS_PROP_BIG_BOARDCASE			0x07				//大喇叭消息
#define ASS_PROP_SMALL_BOARDCASE	0x08				//小喇叭消息
/*------------------------------------------------------------------------------------------------------------------------*/

//框架消息
#define MDM_GM_GAME_FRAME				150						//框架消息
//框架消息辅助ID
#define ASS_GM_GAME_INFO				1						//游戏信息
#define ASS_GM_GAME_STATION				2						//游戏状态
#define ASS_GM_FORCE_QUIT				3						//强行退出
#define ASS_GM_NORMAL_TALK				4						//普通聊天
#define ASS_GM_HIGH_TALK				5						//高级聊天
#define ASS_GM_WATCH_SET				6						//旁观设置	
#define ASS_GM_CLEAN_USER               9                       //比赛场清理用户信息
#define ASS_GM_USE_KICK_PROP            7						//使用踢人卡
#define ASS_GM_USE_ANTI_KICK_PROP       8						//使用防踢卡
#define ASS_GM_SET_VIDEOADDR			10						//设置视频服务器地址

/*********************************************************************************/
//通知消息
#define MDM_GM_GAME_NOTIFY				180						//游戏消息
#define ASS_GM_AGREE_GAME				1						//同意游戏

/*********************************************************************************/
///错误代码
#define ERR_GR_ERROR_UNKNOW				0						//未知错误
#define ERR_GR_LOGON_SUCCESS			1						//登陆成功
#define ERR_GR_USER_NO_FIND				2						//用户不存在
#define ERR_GR_USER_PASS_ERROR			3						//用户密码错误
#define ERR_GR_USER_VALIDATA			4						//用户帐号禁用
#define ERR_GR_USER_IP_LIMITED			5						//登陆 IP 禁止
#define ERR_GR_IP_NO_ORDER				6						//不是指定地址
#define ERR_GR_ONLY_MEMBER				7						//会员游戏房间
#define ERR_GR_IN_OTHER_ROOM			8						//正在其他房间
#define ERR_GR_ACCOUNTS_IN_USE			9						//帐号正在使用
#define ERR_GR_PEOPLE_FULL				10						//人数已经满
#define ERR_GR_LIST_PART				11						//部分用户列表
#define ERR_GR_LIST_FINISH				12						//全部用户列表
#define ERR_GR_STOP_LOGON				13						//暂停登陆服务

#define ERR_GR_CONTEST_NOSIGNUP			14
#define ERR_GR_CONTEST_TIMEOUT			15
#define ERR_GR_CONTEST_KICK				16
#define ERR_GR_CONTEST_NOTSTRAT			17
#define ERR_GR_CONTEST_OVER				18
#define ERR_GR_CONTEST_BEGUN			19

#define ERR_GR_MATCH_LOGON				160						//游戏房间
#define ERR_GR_TIME_OVER				161						//时间到期

///wushuqun 2009.6.5
///不在混战场活动时间内
#define ERR_GR_BATTLEROOM_OUTTIME       162

///用户坐下错误码
#define ERR_GR_SIT_SUCCESS				50						//成功坐下
#define ERR_GR_BEGIN_GAME				51						//游戏已经开始
#define ERR_GR_ALREAD_USER				52						//已经有人存在
#define ERR_GR_PASS_ERROR				53						//密码错误
#define ERR_GR_IP_SAME					54						//IP 相同
#define ERR_GR_CUT_HIGH					55						//断线率太高
#define ERR_GR_POINT_LOW				56						//经验值太低
#define ERR_GR_POINT_HIGH				57						//经验值太高
#define ERR_GR_NO_FRIEND				58						//不受欢迎
#define ERR_GR_POINT_LIMIT				59						//经验值不够
#define ERR_GR_CAN_NOT_LEFT				60						//不能离开
#define ERR_GR_NOT_FIX_STATION			61						//不是这位置
#define ERR_GR_MATCH_FINISH				62						//比赛结束
#define ERR_GR_MONEY_LIMIT				63						//金币太低
#define ERR_GR_MATCH_WAIT				64						//比赛场排队提示
#define ERR_GR_IP_SAME_3				65						//IP前3 相同
#define ERR_GR_IP_SAME_4				66						//IP前4 相同
#define ERR_GR_UNENABLE_WATCH			67						//不允许旁观
#define ERR_GR_DESK_FULL				68						// 百家乐 桌子座位满了，无法分配座位给玩家 
#define ERR_GR_FAST_SIT                 69                      // 快速坐下失败

//封桌
#define MDM_GR_MANAGE        115 
#define ASS_GR_ALONE_DESK    15  //封桌
#define	ASS_GR_DEALONE_DESK  16  //解封

#pragma pack(1)

typedef struct MSG_GR_SR_MatchDelete
{
	long int		dwUserID;			///用户 ID
	BYTE			bDeskIndex;			///桌子号码
	BYTE			bDeskStation;		///桌子位置
} MSG_GR_SR_MatchDelete;

///游戏房间登陆
typedef struct tagMSG_GR_S_RoomLogon 
{
#if (HN_TAREGET_PLATFORM == HN_TARGET_PHONE_ONLY)
	BYTE								byFromPhone;			//是否从手机登录（1）
#endif
	UINT								uNameID;				//游戏 ID
	INT									dwUserID;				//用户 ID
	UINT								uRoomVer;				//大厅版本
	UINT								uGameVer;				//游戏版本
	CHAR								szMD5Pass[50];			//加密密码
}  MSG_GR_S_RoomLogon;

//房间管理窗口数据更新
struct MSG_GR_GRM_UpData
{
	//奖池
	bool		bAIWinAndLostAutoCtrl;	//机器人控制输赢开关
	LLONG		iAIWantWinMoney[3];		//机器人输赢控制：机器人赢钱区域1,2,3 4所用断点
	int			iAIWinLuckyAt[4];		//机器人输赢控制：机器人在区域1,2,3,4赢钱的概率
	LLONG		iReSetAIHaveWinMoney;	//机器人输赢控制：重置机器人已经赢钱的数目
	LLONG		iAIHaveWinMoney;		//机器人赢的钱
	//输赢控制
	bool		bWinProbCtrl;			//开关
	INT			dwUserID_win[20];		//可读取20组，多了影响性能,玩家ID
	int			iProb_win[20];			//输赢概率[0,100]
	INT			dwUserID_los[20];		//可读取20组，多了影响性能,玩家ID
	int			iProb_los[20];			//输赢概率[0,100]

	MSG_GR_GRM_UpData()
	{
		memset(this,0,sizeof(MSG_GR_GRM_UpData));
	}
};

///游戏房间登陆返回
typedef struct tagMSG_GR_R_LogonResult 
{
	INT									dwGamePower;						//用户权限
	INT									dwMasterPower;						//管理权限
	INT									dwRoomRule;							//设置规则
	UINT								uLessPoint;							//最少经验值
	UINT								uMaxPoint;							//最多经验值
	UserInfoStruct						pUserInfoStruct;					//用户信息
	RECEIVEMONEY                        strRecMoney;                        //非比赛场玩家金币不足自动赠送

	///登录房间时即时获取虚拟玩家人数
	INT									nVirtualUser;
	INT									nPresentCoinNum;  // 赠送金币数量

	//比赛专用
	INT									iContestID;
	INT									iLowCount;
	LLONG								i64Chip;
	LLONG								i64TimeStart;
	LLONG								i64TimeEnd;
	LLONG								i64LowChip;
	INT									iTimeout;
	INT									iBasePoint;

	/*********************1.1版本新增字段****************************************/
	bool								bGRMUser;//该玩家可以打开房间管理窗口
	bool								bGRMRoom;//该房间可以打开房间管理窗口

	MSG_GR_GRM_UpData					GRM_Updata;//管理窗口更新数据
	/*********************1.1版本新增字段****************************************/

	//new
}  MSG_GR_R_LogonResult;

///游戏房间登陆
typedef struct tagMSG_GR_R_OtherRoom 
{
	INT									iRoomID;
	CHAR								szGameRoomName[61];					///房间名字
}  MSG_GR_R_OtherRoom;

///游戏桌子状态
typedef struct tagMSG_GR_DeskStation 
{
	BYTE								bDeskStation[100];					//桌子状态
	BYTE								bDeskLock[100];						//锁定状态
	INT									iBasePoint[100];					//桌子倍数
	BYTE                                bVirtualDesk[100];                  //虚拟状态
}  MSG_GR_DeskStation;

///用户进入房间
typedef struct tagMSG_GR_R_UserCome 
{
	UserInfoStruct						pUserInfoStruct;					//用户信息
}  MSG_GR_R_UserCome;

///用户离开房间
typedef struct tagMSG_GR_R_UserLeft 
{
	INT								dwUserID;								//用户 ID
}  MSG_GR_R_UserLeft;

///用户坐下
typedef struct tagMSG_GR_S_UserSit 
{
	BYTE								bDeskIndex;							//桌子索引
	BYTE								bDeskStation;						//桌子位置
	CHAR								szPassword[61];						//桌子密码
}  MSG_GR_S_UserSit;

///用户坐下
typedef struct tagMSG_GR_R_UserSit 
{
	INT									dwUserID;							//用户 ID
	BYTE								bLock;								//是否密码
	BYTE								bDeskIndex;							//桌子索引
	BYTE								bDeskStation;						//桌子位置
	BYTE								bUserState;							//用户状态
	BYTE								bIsDeskOwner;						//台主离开
}  MSG_GR_R_UserSit;

///用户坐起来
typedef MSG_GR_R_UserSit MSG_GR_R_UserUp;

///用户断线
typedef struct tagMSG_GR_R_UserCut 
{
	INT									dwUserID;							//用户 ID
	BYTE								bDeskNO;							//游戏桌号
	BYTE								bDeskStation;						//位置号码
}  MSG_GR_R_UserCut;

///用户同意结构
typedef struct tagMSG_GR_R_UserAgree 
{
	BYTE								bDeskNO;							//游戏桌号
	BYTE								bDeskStation;						//位置号码
	BYTE								bAgreeGame;							//同意标志
}  MSG_GR_R_UserAgree;

///用户分数
typedef struct tagMSG_GR_R_InstantUpdate 
{
	INT									dwUserID;							//用户 ID
	INT									dwPoint;							//用户分数
	INT									dwMoney;							//用户金币
}  MSG_GR_R_InstantUpdate;

///用户经验值
typedef struct tagMSG_GR_R_UserPoint 
{	//广播消息
	INT									dwUserID;							//用户 ID
	LLONG								dwPoint;							//用户经验值
	LLONG								dwMoney;							//用户金币
	LLONG								dwSend;								//赠送
	BYTE								bWinCount;							//胜局
	BYTE								bLostCount;							//输局
	BYTE								bMidCount;							//平局
	BYTE								bCutCount;							//逃局
	RECEIVEMONEY                        strAutoSendMoney;                   //添加自动赠送
}  MSG_GR_R_UserPoint;

/********************************************************************************************/

///游戏信息
typedef struct tagMSG_GM_S_GameInfo 
{
	BYTE								bGameStation;						//游戏状态
	BYTE								bWatchOther;						//允许旁观
	BYTE								bWaitTime;							//等待时间
	BYTE								bReserve;							//保留字段
	CHAR								szMessage[1000];					//系统消息
}  MSG_GM_S_GameInfo;


///修改用户经验值
typedef struct tagMSG_GR_S_RefalshMoney 
{
	INT									dwUserID;							//用户 ID
	LLONG								i64Money;							//用户金币
}  MSG_GR_S_RefalshMoney;

///旁观设置
typedef struct tagMSG_GM_WatchSet 
{
	INT									dwUserID;							//设置对象
}  MSG_GM_WatchSet;

///消息数据包
typedef struct tagMSG_GA_S_Message
{
	BYTE								bFontSize;							//字体大小
	BYTE								bCloseFace;							//关闭界面
	BYTE								bShowStation;						//显示位置
	CHAR								szMessage[1000];					//消息内容
}  MSG_GA_S_Message;

//游戏公告消息ATT
typedef struct tagGameNoticeMessage 
{
	BYTE			bDeskIndex;				//桌子号
	BYTE			bAwardCardShape;		//牌型奖励
	CHAR			szMessage[255];			//未进入游戏 在房间 右边框显示公告内容
}  GameNoticeMessage;
/********************************************************************************************/
//比赛消息
typedef struct tagMSG_GR_I_ContestApply
{
	int									iUserID;										//用户ID	
	int									iTypeID;										//操作类型: 0-报名, 1-退赛
}  MSG_GR_I_ContestApply;

typedef struct  tagMSG_GR_ContestApply					//只发给报名者
{
	int									iApplyResult;								//报名结果
	int									iContestBegin;							//比赛是否开始
}  MSG_GR_ContestApply;

#define ASS_GR_CONTEST_APPLYINFO						25			//比赛场报名信息

typedef struct tagMSG_GR_I_ContestInfo					//广播
{
	int									iContestBegin;							//0-比赛还未开始, 1-比赛开始
	int									iContestNum;							//已报名人数
}  MSG_GR_I_ContestInfo;

#define MDM_GP_GET_CONTEST_ROOMID					51			//比赛房间，用户双击列表时向服务器索要房间号
//辅助ID 0

//In:
typedef struct tagMSG_GP_GetContestRoomID
{
	int									iUserID;										//用户ID
	UINT								iGameID;										//游戏ID
	int									iContestID;									//比赛ID
	UINT								iUpPeople;									//比赛达到此人数后才开赛
}  MSG_GP_GetContestRoomID;

//Out:
typedef struct tagDL_GetContestRoomID
{
	UINT								iGameID;							// 游戏ID
	int									iRoomID;							// 比赛房间ID
	int									iContestID;							// 比赛ID
	int									iConstestNum;						// 已报名人数
	int									iChampionCount;						// 夺冠次数
	int									iBestRank;							// 最佳名次
	int									iJoinCount;							// 参赛次数
	int									iEntryFee;							// 入场费用
	int									iRankAward[3];						// 前三名奖励信息
	int									iAwardType[3];						// 前三名奖励类型
}  DL_GetContestRoomID;

//定时赛
#define	MDM_GP_GET_TIMINGMATCH_TIME				52
#define	ASS_GP_GET_TIMINGMATCH_TIME				1
typedef	struct tagMSG_GP_GETMatchBeginTime
{
	UINT								iGameID;										//游戏ID
	int									iContestID;									//比赛ID
	LLONG							I64BeginTime;							//开赛时间
}  MSG_GP_GETMatchBeginTime;

//比赛开始
//Out:   给比赛用发送初始化信息
//ASS_GR_INIT_CONTEST	

typedef struct tagMSG_GR_ContestChange
{
	int									dwUserID;							// 用户 ID	
	int									iContestCount;						// 比赛局数
	LLONG							    i64ContestScore;					// 比赛分数
	int									iRankNum;							// 比赛排名
	int									iRemainPeople;						// 比赛中还剩多少人
	char								szAward[30];						// 比赛获取的奖励
}  MSG_GR_ContestChange;

typedef struct tagMSG_GR_ContestAward
{
	int									dwUserID;
	int									iAward;
	int									iAwardType;
}  MSG_GR_ContestAward;

#pragma pack()

//Out:
//比赛开始后，通知该房间未报名用户退场，无数据，为广播信息
//MDM_GR_ROOM
//ASS_GR_CAHNGE_ROOM//比赛开始，未报名用户切换房间

//Out:
//	广播玩家座位等信息
//MDM_GR_USER_ACTION
//ASS_GR_QUEUE_USER_SIT

//	数据为指向int的指针，大小为（uDeskPeople*3 + 1）(每次传一桌数据)
//	内容为：第一个数据为桌子序号，
//	第二三四个数据为这个桌子第一个玩家的ID，排名及剩余人数
//	第五六七个数据为这个桌子第二个玩家的ID，排名及剩余人数以此类推


//比赛中结算 MDM_GR_ROOM,ASS_GR_USER_CONTEST, &MSG_GR_ContestChange

//单局比赛结束若玩家积分小于下线则发送踢出消息 MDM_GR_ROOM, ASS_GR_CONTEST_KICK, Handlecode为桌号

//若玩家断线，则踢出玩家，广播 MDM_GR_USER_ACTION, ASS_GR_USER_UP, HandleCode  ERR_GR_SIT_SUCCESS
//用户坐下或者起来, MSG_GR_R_UserSit

//比赛结束但是还有用户在进行游戏 MDM_GR_ROOM, ASS_GR_CONTEST_WAIT_GAMEOVER
//比赛结束，但是有用户还在打最后一局，通知已打完的用户等待排名, Handlecode为桌号

//比赛结束：发给所有比赛用户MDM_GR_ROOM, ASS_GR_CONTEST_GAMEOVER

/********************************************************************************************/

#endif	//_HN_RoomMessage_H__
