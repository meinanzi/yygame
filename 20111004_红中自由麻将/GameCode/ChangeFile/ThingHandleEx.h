#pragma once

#include "../GameMessage/ThingHandle.h"
///事件处理类（重载）
class ThingHandleEx:public ThingHandle
{
public:
	ThingHandleEx(void);
	~ThingHandleEx(void);
	
public:	
	
public:
	
///////接收数据处理//////////////////////////////////////////////////////
	//断线重连事件
	virtual bool	GetGameStaton(BYTE bDeskStation, UINT uSocketID, bool bWatchUser,BYTE GameStation);
	// 游戏数据包处理函数
	virtual int		HandleNotifyMessage(int bDeskStation, UINT code, BYTE* pData, int uSize, bool bWatchUser);
	//玩家换牌
	int		ReceiveUserChangeCard(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser);
	bool	IsSuperUser(BYTE bDeskStation);
	// 收到托管消息
	virtual int		ReceiveUserTuoGuan(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser);
	// 用户出牌
	virtual int		ReceiveUserOutCard(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser);
	// 收到用户pass 消息
	virtual int		ReceiveUserPassCheck(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser,bool bAuto = false);
	//收到用户同意游戏消息
	virtual int		ReceiveUserAgreeGame(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser);
	// 用户吃牌消息
	virtual int		ReceiveUserChiCard(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser);
	// 用户碰牌消息
	virtual int		ReceiveUserPengCard(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser);
	// 用户杠牌消息
	virtual int		ReceiveUserGangCard(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser);
	// 用户听牌消息
	virtual int		ReceiveUserTingCard(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser);
	// 用户糊牌消息
	virtual int		ReceiveUserHuCard(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser);

	
	
	
	

	///描述：检测下一步动作
	virtual void CheckNextAction();
	//获取离糊牌玩家近的玩家:有多个胡牌玩家的时候
	virtual BYTE GetCanHuUser(BYTE outst,bool hu[],BYTE hust);
	///获取下家位置
	virtual BYTE GetNextStation(BYTE station,bool shun=false);
	///清理临时数据
	virtual void ClearTempData();




public:	/*---------游戏流程群函数-----------*/
	//请求事件,type要申请的事件	DWJ
	virtual void	ApplyAction(int type,int time);
	//游戏事件请求处理中心，确定下一个事件是什么，返回事件值,type申请的事件类型	DWJ
	virtual void	GameCPU(int type);
	///开始事件处理	DWJ
	virtual void	ExecuteGameBegin(BYTE &nextID,BYTE &time);
	///以第一个玩家为庄家	DWJ
	virtual void	ExecuteDongNt(BYTE &nextID,BYTE &time);
	///掷2颗色子确定起牌方向（点数和确定方向）	DWJ
	virtual void	ExecuteTwoSeziDir(BYTE &nextID,BYTE &time);	
	///掷2颗色子确定起牌方向和起牌敦数（点数和方向，小点敦数）	DWJ
	virtual void	ExecuteTwoSeziDirGp(BYTE &nextID,BYTE &time);
	///发牌	DWJ
	virtual void	ExecuteSendPai(BYTE &nextID,BYTE &time);
	//发牌函数	DWJ
	virtual void	DisPatchCard();
	//为了买码而打乱的牌
	void	DisPatchCardForMaiMa(BYTE byPai[300]);
	///描述：抓一张牌
	virtual BYTE	GetPai(int station,bool head,BYTE type=0,BYTE *pIndex=NULL);
	///描述：转换门牌数据,将总门牌数组中的数据转换到每个玩家的门牌数组中
	virtual void	ChangeMenPai();
	///所有玩家补花事件	DWJ
	virtual void	ExecuteAllBuHua(BYTE &nextID,BYTE &time);
	///开始出牌通知事件	DWJ
	virtual void	ExecuteBeginOut(BYTE &nextID,BYTE &time);
	///检测玩家station当前能做哪些动作	DWJ
	virtual bool	CheckAction(int station,BYTE pai,BYTE type);
	//玩家能否听牌	DWJ
	bool			CanTingDWJ(BYTE byDeskStation);
	///有玩家有动作发送拦牌信息给玩家	DWJ
	virtual bool	HaveUserCanDoAction(BYTE byIgnoreStation);
	///抓牌事件
	virtual void	ExecuteZhuaPai(BYTE &nextID,BYTE &time);	
	///单个玩家补花事件
	virtual void	ExecuteOneBuHua(BYTE &nextID,BYTE &time);
	//买码事件
	void	ExecuteMaiMa(BYTE &nextID,BYTE &time);
	//买枪事件
	void ExecuteZhongNiao(BYTE &nextID,BYTE &time);
	///算分事件
	virtual void	ExecuteCountFen(BYTE &nextID,BYTE &time);
	//计算番数和花数
	void			CountFanAndHua(BYTE byDeskStation,int &iHuaCount, int &iFanCount);
	///正常游戏结束事件
	virtual void	ExecuteNormalEnd(BYTE &nextID,BYTE &time);



	

	
	
	
	
public:	/*---------计时器响应群函数-----------*/
	/// 处理计时器消息	DWJ
	virtual void	OnGameTimer(UINT uTimerID);
	/// 设置当前正在发生的事件	DWJ
	virtual void	SetDoingThing(BYTE byStation,BYTE thing, BYTE byTime = 255);
	/// 恢复超时数据	DWJ
	virtual void	ReSetOutTime();
	/// 设置超时数据	DWJ
	virtual void	SetOutTime(BYTE station,BYTE time,BYTE thing,bool effect);
	/// 超时处理	DWJ
	virtual void	OutTimeHandle();





public:/*---------事件处理群函数-----------*/
	//	玩家处理超时，系统自动处理	DWJ
	virtual void	AutoHandle();
	//	自动处理出牌或者抓牌	DWJ
	void	AutoOutOrGet();
	//	自动处理动作	DWJ
	void	AutoDoAction();	
	//	自动处理胡牌	DWJ
	void	AutoHuPai();
	//算分函数
	void    CalculateGangFen(void);		

		
	
   	
	
	
	

	///异常游戏结束事件事件
	virtual void ExecuteUnNormalEnd(BYTE &nextID,BYTE &time);

	
	
	
	
	///断线自动处理函数
	virtual void NetCutAutoHandle();
	

	




//////动作结果处理///////////////////////////////////////////////////////////
	///吃牌事件		DWJ
	virtual void	HandleChiPai(BYTE &nextID,BYTE &time);	
	///碰牌事件		DWJ
	virtual void	HandlePengPai(BYTE &nextID,BYTE &time);	
	///杠事件		DWJ
	virtual void	HandleGangPai(BYTE &nextID,BYTE &time);

public:
	CString dwjlog;
};

//全局对象
//extern ThingHandleEx g_ThingHandle;