#pragma once

/**
* 山东麻将服务器
*/

#include "StdAfx.h"
#include "..\GameMessage\CLogicBase.h"

#include "..\ChangeFile\GameDataEx.h"
#include "..\ChangeFile\UserDataEx.h"
#include "..\ChangeFile\CheckHuPaiEx.h"
#include "..\ChangeFile\CheckCPGActionEx.h"
#include "..\ChangeFile\ThingHandleEx.h"


// 定时器 ID
#define TIME_GAME_FINISH			91				// 游戏结束定时器
#define TIME_CHECK_CONNECT          92              // 检测玩家
#define DEFAULT_CHECK_TIME          10000           //如果一份总内收不到消息

//游戏结束标志定义
#define GF_NORMAL					10				// 游戏正常结束
#define GF_SALE						11				// 游戏安全结束
#define GF_NET_CAUSE                12              //网络原因，游戏结束   yjj 081204


#define GFF_FORCE_CONTINUE          2               //add by yjj

#define  DEFAULT_AIOUT_TIMER        600
#define  DEFAULT_AUTO_HU_TIME       600
#define  DEFAULT_AIOUT_HANDLE_TIMER 601          //默认自动处理计时器


#define TIME_CHECK_GAME_MSG          30              // 检测游戏消息数据包，超时没有收到数据包就认为是卡住了，系统自动处理
#define TIME_DEFAULT_HANDLE          60000			 // 默认时间60秒

const int TIME_STEP = 31;           /// 时间
const int TIME_STEP_TIME = 500;     /// 

const int TIME_NETWORK = 1000;      /// 网络延迟时间

/** 游戏正常结束 */
#define GF_NORMAL					10
/** 游戏安全结束 */
#define GF_SALE						11	

/// 游戏桌子类
class CServerGameDesk : public CGameDesk
{
public:

protected:

public:

public:
	/// 构造函数
	CServerGameDesk(); 
	/// 析构函数
	virtual ~CServerGameDesk();

public:
	/// 初始游戏
	virtual bool	InitDeskGameStation();
	/// 游戏开始
	virtual bool	GameBegin(BYTE bBeginFlag);
	/// 游戏结束
	virtual bool	GameFinish(BYTE bDeskStation, BYTE bCloseFlag);
	/// 判断是否正在游戏
	virtual bool	IsPlayGame(BYTE bDeskStation);
	/// 框架消息处理函数
	virtual bool	HandleFrameMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser);
	/// 游戏数据包处理函数
	virtual bool	HandleNotifyMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser);
	/// 用户离开游戏桌
	virtual BYTE	UserLeftDesk(BYTE bDeskStation, CGameUserInfo * pUserInfo);	
	//用来改变用户断线条件的函数
	virtual bool	CanNetCut(BYTE bDeskStation);
	//玩家断线处理
	virtual bool UserNetCut(BYTE bDeskStation, CGameUserInfo * pLostUserInfo);
	/// 玩家坐下
	virtual BYTE	UserSitDesk(MSG_GR_S_UserSit * pUserSit, CGameUserInfo * pUserInfo);
	/// 获取游戏状态信息
	virtual bool	OnGetGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser);
	/// 重置游戏状态
	virtual bool	ReSetGameState(BYTE bLastStation);
	/// 定时器消息
	virtual bool	OnTimer(UINT uTimerID);





public:
	////////////基本数据定义/////////////////////////////////////////////////////////
	///同意结束游戏的玩家数量
	BYTE			m_byAgreeLeaveNum;	

    BYTE			m_byGameStation;	//游戏状态

    int				m_RoomId;			//房间id

	GameDataEx		sGameData;			//游戏数据	DWJ

	UserDataEx		sUserData;			//用户数据	DWJ

	CheckCPGActionEx sCPGAction;		//吃碰杠检测方法

	CheckHuPaiEx	sCheckHuPai;		//糊牌检测方法

	ThingHandleEx	m_sThingHandle;		//事件处理

	vector <long int> m_vlSuperUserID;	//超端玩家ID容器
	static int m_iBaseMoney;            //底分

	int				m_iMaiMaCount;		//买码个数
	int				m_i23ZhongCount;	//买码2、3、中至少出现的个数
	BYTE           m_byAutoTime[PLAY_COUNT];        //超时次数

public:

	UINT m_uSocketID;

	///设置游戏定时器
	void SetGameTimer(UINT iId,int iTime);
	//删除指定计时器
	void KillGameTimer(UINT iId);
	void SetParamaterStation(BYTE byStation);
    ///发送游戏状态
    void SendGameStationEx(BYTE bDeskStation, bool bWatchUser, void * pData, UINT uSize);
    ///发送数据函数 （发送消息给游戏者）
    void SendGameDataEx(BYTE bDeskStation, BYTE bMainID, BYTE bAssID, BYTE bHandleCode);
    ///发送旁观数据 （发送消息给旁观者）
    void SendWatchDataEx(BYTE bDeskStation, BYTE bMainID, BYTE bAssID, BYTE bHandleCode);
    ///发送数据函数 （发送消息给游戏者）
    void SendGameDataEx(BYTE bDeskStation, void * pData, UINT uSize, BYTE bMainID, BYTE bAssID, BYTE bHandleCode);
    ///发送旁观数据 （发送消息给旁观者）
    void SendWatchDataEx(BYTE bDeskStation, void * pData, UINT uSize, BYTE bMainID, BYTE bAssID, BYTE bHandleCode);
	//加载房间设置
	void	LoadiniByRoom(int iRoomId);
	//修改奖池数据1(平台->游戏)
	/// @param bAIWinAndLostAutoCtrl 机器人控制输赢开关
	/// @return 是否成功
	virtual bool SetRoomPond(bool	bAIWinAndLostAutoCtrl);

	//修改奖池数据2(平台->游戏)
	/// @param m_iAIWantWinMoney 机器人输赢控制：机器人赢钱区域1,2,3 4所用断点
	/// @param m_iAIWinLuckyAt 机器人输赢控制：机器人在区域1,2,3,4赢钱的概率
	/// @return 是否成功
	virtual bool SetRoomPondEx(__int64	iAIWantWinMoney[], int	iAIWinLuckyAt[]);

	//修改奖池数据3(平台->游戏)
	/// @param iReSetAIHaveWinMoney 机器人输赢控制：重置机器人已经赢钱的数目
	/// @return 是否成功
	virtual bool SetRoomPondEx(__int64	iReSetAIHaveWinMoney);

	//控制胜负
	/// 判断
	virtual BOOL Judge();
	/// 胜
	virtual BOOL JudgeWiner();
	/// 输
	virtual BOOL JudgeLoser();
	//GRM函数
	//更新奖池数据(游戏->平台)
	/// @return 机器人赢钱数
	virtual void UpDataRoomPond(__int64 iAIHaveWinMoney);
};


/******************************************************************************************************/
