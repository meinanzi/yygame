#ifndef _HN_ComStruct_H__
#define _HN_ComStruct_H__

/********************************************************************************************/
#include "HNBaseType.h"
#include "HNCommon/HNCommonMarco.h"
#include "HNMatchMessage.h"
#include <string>

/// 房间规则设置
#define		GRR_MEMBER_ROOM			0x00000001L							//会员房间
#define		GRR_IP_LIMITED			0x00000002L							//地址限制
#define		GRR_ENABLE_WATCH		0x00000004L							//允许旁观
#define		GRR_UNENABLE_WATCH		0x00000008L							//不许旁观
#define		GRR_AUTO_SIT_DESK		0x00000010L							//自动坐下，现在用于防作弊场使用
#define		GRR_LIMIT_DESK			0x00000020L							//限制座位
#define		GRR_LIMIT_SAME_IP		0x00000040L							//限制同IP
#define		GRR_RECORD_GAME			0x00000080L							//记录游戏
#define		GRR_STOP_TIME_CONTROL	0x00000100L							//停止时间控制
#define		GRR_ALL_ON_DESK			0x00000200L							//是否所有人坐下才开始启动
#define		GRR_FORBID_ROOM_TALK	0x00000400L							//禁止房间聊天
#define		GRR_FORBID_GAME_TALK	0x00000800L							//禁止游戏聊天
#define		GRR_MATCH_REG			0x00001000L							//比赛报名
#define		GRR_EXPERCISE_ROOM		0x00002000L							//训练场
#define		GRR_VIDEO_ROOM			0x00004000L							//视频房间

/// 修改防作弊场可看见其他玩家姓名和头像问题！
#define		GRR_NOTFUFENG			0x00008000L							//不允许负积分
#define		GRR_NOTCHEAT			0x00010000L							//防作弊

/// 台费场
#define		GRR_ALL_NEED_TAX		0x00020000L							//收台费场,所有人都需缴纳一定数额台费
#define		GRR_QUEUE_GAME			0x00040000L							//排队机

#define		GRR_NOT_COST_POINT		0x00080000L							//金币场不扣积分

#define		GRR_CONTEST				0x00100000L							//定时淘汰比赛场

#define		GRR_TIMINGCONTEST		0x00800000L							//定时赛		--RoomRule == 8388608



/********************************************************************************************/
///用户状态定义
//#define	USER_NO_STATE		 	0									//没有状态，不可以访问
#define		USER_LOOK_STATE		 	1									//进入了大厅没有坐下
#define		USER_SITTING		 	2									//坐在游戏桌
#define		USER_ARGEE				3									//同意状态
#define		USER_WATCH_GAME		 	4									//旁观游戏
//#define	USER_DESK_AGREE			5								///大厅同意
#define		USER_CUT_GAME		 	20									//断线状态		（游戏中状态）
#define		USER_PLAY_GAME		 	21									//游戏进行中状态	（游戏中状态）
/********************************************************************************************/

#pragma pack(1)

/// 游戏列表辅助结构
typedef struct tagAssistantHead 
{
	UINT						uSize;								//数据大小
	UINT						bDataType;							//类型标识
} AssistantHead;

/// 游戏名称结构
typedef struct tagComNameInfo 
{
	AssistantHead				Head;
	UINT						uKindID;							//游戏类型 ID 号码
	UINT						uNameID;							//游戏名称 ID 号码
	CHAR						szGameName[61];						//游戏名称
	CHAR						szGameProcess[61];					//游戏进程名
	UINT						m_uOnLineCount;						//在线人数
	/**************************1.1版本新增低端*********************************/
	UINT						uVer;								///版本	                                                                    
	/************************************************************************/

}  ComNameInfo;


///游戏类型de结构
typedef struct tagComKindInfo///加入游戏类型AddTreeData
{
	AssistantHead				Head;
	UINT						uKindID;							//游戏类型 ID 号码
	char						szKindName[61];						//游戏类型名字

	//游戏大厅二级目录功能
	UINT                        uParentKindID;                      //父游戏类型ID号码
	// end duanxiaohui
}  ComKindInfo;

/// 游戏房间列表结构
typedef struct tagComRoomInfo 
{
	AssistantHead				Head;
	UINT						uComType;							//游戏类型
	UINT						uKindID;							//游戏类型 ID 号码
	UINT						uNameID;							//游戏名称 ID 号码
	UINT						uRoomID;							//游戏房间 ID 号码
	UINT						uPeopleCount;						//游戏在线人数
	UINT						iUpPeople;							//比赛房间用户达到值后才开赛
	UINT						uDeskPeople;						//每桌游戏人数
	UINT						uDeskCount;							//游戏大厅桌子数目
	UINT						uServicePort;						//房间服务端口
	CHAR						szServiceIP[25];					//房间服务器 IP 地址
	CHAR						szGameRoomName[61];					//游戏房间名称
	INT							uVirtualUser;						//
	INT							uVirtualGameTime;					//
	UINT						uVer;								//版本
	UINT						dwRoomRule;							//游戏房间规则
	bool                        bVIPRoom;                           //金葫芦二代，是否VIP房间（VIP房间设置密码）

   /**************************1.1版本新增字段**************************************/	
	INT							iBasePoint;							// 基础倍数
	UINT						iLessPoint;							// 金币或者积分下限
	UINT						iMaxPoint;							// 金币或者积分上限
	/*************************1.1版本新增字段**************************************/

	INT							iContestID;
	LLONG						i64TimeStart;
	LLONG						i64TimeEnd;

	//租赁房间ID与时间
	INT							iLeaseID;
	LLONG						i64LeaseTimeStart;
	LLONG						i64LeaseTimeEnd;

	///混战场房间ID
	UINT						uBattleRoomID;
	///混战场房间信息表
	CHAR						szBattleGameTable[31];	
	bool						bHasPassword;						// 有无密码	
	UINT						dwTax;								// 房间房费
}  ComRoomInfo;

/// 用户信息结构
typedef struct tagUserInfoStruct 
{
	INT						    dwUserID;							//ID 号码
	INT						    dwExperience;						//经验值
	INT							dwAccID;							//ACC 号码
	INT							dwPoint;							//分数
	LLONG						i64Money;							//金币
	LLONG						i64Bank;							//银行
	UINT						uWinCount;							//胜利数目
	UINT						uLostCount;							//输数目
	UINT						uCutCount;							//强退数目
	UINT						uMidCount;							//和局数目
	CHAR						szName[61];							//登录名
	CHAR						szClassName[61];					//游戏社团
	UINT						bLogoID;							//头像 ID 号码

	BYTE						bDeskNO;							//游戏桌号
	BYTE						bDeskStation;						//桌子位置
	BYTE						bUserState;							//用户状态

	BYTE						bMember;							//会员等级
	BYTE						bGameMaster;						//管理等级
	UINT						dwUserIP;							//登录IP地址
	bool						bBoy;								//性别
	CHAR						nickName[61];						//用户昵称
	UINT						uDeskBasePoint;						//设置的桌子倍数
	INT							dwFascination;						//魅力
	INT							iVipTime;							//会员时间
	INT							iDoublePointTime;					//双倍积分时间
	INT							iProtectTime;						//护身符时间，保留
	INT							isVirtual;							//是否是扩展机器人 
	UINT						dwTax;								//房费   

	///玩家信息结构调整   
	CHAR                        szOccupation[61];                   //玩家职业
	CHAR                        szPhoneNum[61];                     //玩家电话号码
	CHAR                        szProvince[61];                     //玩家所在的省
	CHAR                        szCity[61];                         //玩家所在的市
	CHAR                        szZone[61];                         //玩家所在的地区
	bool                        bHaveVideo;                         //是否具有摄像头
	CHAR						szSignDescr[128];			        //个性签名

	//玩家类型信息
	//0 ,普通玩家
	//1 ,电视比赛玩家
	//2 ,VIP玩家
	//3 ,电视比赛VIP玩家
	INT							userType;

	//作为扩展字段,为方便以后新加功能用
	//此处为以后平台中的新加功能需要修改用户信息结构时，不用重新编译所有游戏
	UINT                        userInfoEx1;		//扩展字段1，用于邮游钻石身份作用时间
	UINT						userInfoEx2;		//扩展字段2，用于GM处理之禁言时效

	INT							bTrader;			//用于判断是不是银商  

	///比赛专用
	INT							iMatchID;			//比赛ID，唯一的标识一场比赛
	LLONG						i64ContestScore;	//比赛分数
	INT							iContestCount;		//比赛次数
	CHAR						timeLeft[8];		//
	INT							iRankNum;			//排行名次
	INT							iRemainPeople;		//比赛中还剩下的人数
	INT							iAward;				//比赛获取的奖励
	INT                         iAwardType;         //比赛获取的奖励类型

	tagUserInfoStruct()
	{
		memset(nickName, 0,sizeof(nickName));
	};
}  UserInfoStruct;

// 自动赠送添加的结构体
typedef struct tagRECEIVEMONEY 
{
	bool      bISOpen;
	//bool      bISAuto;	
	LLONG	  i64Money;
	LLONG	  i64MinMoney;
	INT		  iCount;
	INT		  iTotal;
	INT		  iTime;   
	INT       iResultCode; 
	INT       iLessPoint;
}  RECEIVEMONEY;

#pragma pack()

#endif	//_HN_ComStruct_H__
