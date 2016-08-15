#ifndef __DZPokerGameTableLogic_h__
#define __DZPokerGameTableLogic_h__

#include "HNNetExport.h"
#include "HNLogicExport.h"
#include "DZPokerGameClientMessage.h"

namespace DZPoker
{
	
class GameTableUICallback;

/************************************************************************/
/* 游戏逻辑过程处理                                                      */
/************************************************************************/
class GameTableLogic: public HN::HNGameLogicBase
{
public:
	GameTableLogic(GameTableUICallback* uiCallback, BYTE bDeskIndex, bool bAutoCreate);
	virtual ~GameTableLogic();

public:	
	virtual void dealGameMessage(NetMessageHead* messageHead, void* object, INT objectSize);

protected:
	/************************分割线*********************************/
	//游戏消息处理
	void dealUserSetResp(void* object, INT objectSize);
	void dealUserSetChangeResp(void* object, INT objectSize){}
	void dealGameBeginResp(void* object, INT objectSize);//**
	void dealSysDeskCfgResp(void* object, INT objectSize);//**
	void dealCfgDeskResp(void* object, INT objectSize);
	void dealDeskCfgResp(void* object, INT objectSize);
	void dealAgreeDeskCfgResp(void* object, INT objectSize){}//--
	void dealBetResp(void* object, INT objectSize);//**
	void dealHandCardResp(void* object, INT objectSize);
	void dealSend3CardResp(void* object, INT objectSize);
	void dealSend4_5CardResp(void* object, INT objectSize);
	void dealSendCardFinishResp(void* object, INT objectSize);
	void dealBetPoolUpResp(void* object, INT objectSize);
	void dealBetReqResp(void* object, INT objectSize){}
	void dealBetInfoResp(void* object, INT objectSize);
	void dealNewUserResp(void* object, INT objectSize){}
	void dealPassUserResp(void* object, INT objectSize){}
	void dealSubMoneyResp(void* object, INT objectSize); //**
	void dealCompareCardResp(void* object, INT objectSize);
	void dealCallMandateResp(void* object, INT objectSize){}
	void dealCheckMandateResp(void* object, INT objectSize){}
	void dealCallAnyMandateResp(void* object, INT objectSize){}
	void dealPassAbandonMandateResp(void* object, INT objectSize){}
	void dealUserLeftDeskResp(void* object, INT objectSize);
	void dealSortOutResp(void* object, INT objectSize);
	void dealTokenResp(void* object, INT objectSize);
	void dealResultResp(void* object, INT objectSize);
	void dealBuyMoneyResp(void* object, INT objectSize);
	void dealLookCardResp(void* object, INT objectSize);
	void dealCanShowCardResp(void* object, INT objectSize){}
	void dealShowCardResp(void* object, INT objectSize);
	void dealNoPlayerResp(void* object, INT objectSize);
	void dealAgreeResp(void* object, INT objectSize);
	void dealCfgDeskTimeOutResp(void* object, INT objectSize);
	void dealNotEnoughMoneyResp(void* object, INT objectSize);
	void dealMoneyLackResp(void* object, INT objectSize);//**
	void dealNextRoundReqResp(void* object, INT objectSize);//**
	void dealNewRoundBeginResp(void* object, INT objectSize);
	void dealFinishRoundReqResp(void* object, INT objectSize){}
	

	/************************分割线*********************************/
	//框架消息处理
	virtual void dealUserAgreeResp(MSG_GR_R_UserAgree* agree);
	virtual void dealGameStartResp(BYTE bDeskNO);
	virtual void dealGameEndResp(BYTE bDeskNO);
	virtual void dealGamePointResp(void* object, INT objectSize);
	virtual void dealUserSitResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user);
	//玩家排队坐下
	virtual void dealQueueUserSitMessage(BYTE deskNo, const std::vector<QUEUE_USER_SIT_RESULT*>& user);
	virtual void dealUserUpResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user);
	virtual void dealGameInfoResp(MSG_GM_S_GameInfo* pGameInfo);
	virtual void dealGameStationResp(void* object, INT objectSize);
    
    //--------------------------比赛接口---------------------
    //比赛信息广播
    virtual void dealGameContestNotic(MSG_GR_I_ContestInfo* contestInfo) override;
    //用户比赛信息
    virtual void dealGameUserContset(MSG_GR_ContestChange* contestChange) override;
    //比赛淘汰
    virtual void dealGameContestKick() override;
    //等待比赛结束
    virtual void dealGameContestWaitOver() override;
    //比赛结束
    virtual void dealGameContestOver(MSG_GR_ContestAward* contestAward) override;
    
    
public:
	//进入游戏
	void enterGame();
	virtual void sendUserUp();
	void loadDeskUsersUI();
	virtual void clearDesk();

public:
	//发送指令：客户端==>服务端
	//加入排队
	void sendQueue();
	void sendUserRaise(LLONG money);
	void sendUserBet(LLONG money);
	void sendUserFold();
	void sendUserCall();
	void sendUserCheck();
	void sendUserAllIn();
	void sendUserReady();
	LLONG getMinBuy() const;
	LLONG getMaxBuy() const;
	LLONG getPlanBuy() const;
	BYTE getMeStation() const;
protected:
	GameTableUICallback*	_uiCallback;
	TableInfo				_tableInfo;
	UserInfoStruct*         _myInfo;
	void sendUserBet(emType type, LLONG money);
	void setTableOffsetStation(BYTE bDeskStation);
protected:
	void initData();
	/**保存牌桌数据**/
	void saveTableInfo(void* object);
	/**显示玩家信息**/
	void showUserInfo();
	/**显示玩家筹码**/
	void showUserMoney(const TSubMoney* pSubMoney);
	/**显示带入金额对话框**/
	void showBuyMoney(LLONG min, LLONG max, LLONG plan);
	/**获取下一个玩家的位置**/
	BYTE getNextUserStation(BYTE byStation);
	/**获取上一个玩家的位置**/
	BYTE getLastUserStation(BYTE byStation);
	/**逻辑位置<==>视图位置**/
	BYTE viewToLogicStation(BYTE byStation);
	BYTE logicToViewStation(BYTE byStation);
	/**显示操作按钮**/
	void showAction(bool bVisible, void* object);
	//显示教学
	void showHelp(bool bVisible);
	//显示托管
	void showAutoAction(bool bVisible);
	//托管处理
	bool doUserAutoAction(void* object);
	//设置滑动操作调的值
	void setSliderValue();

	//清空桌面信息玩家
	void	ClearDeskUser();

public:
	//记录是否为比赛场准备
	bool _isReadyQueue;
	// 是否比赛场
	bool	_bContestRoom;
	//是否比赛结束 
	bool	_bContestEnd;
};


}

#endif // __DZPokerGameTableLogic_h__