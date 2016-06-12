#ifndef __HN_ProtocolExport_h__
#define __HN_ProtocolExport_h__

#include "HNNetProtocol/HNBaseCommand.h"
#include "HNNetProtocol/HNComStruct.h"
#include "HNNetProtocol/HNNetMessageHead.h"
#include "HNNetProtocol/HNPlatformMessage.h"
#include "HNNetProtocol/HNRoomMessage.h"
#include "HNNetProtocol/HNBankMessage.h"
#include "HNNetProtocol/HNMatchMessage.h"
#include <vector>

// 比赛接口
class IGameMatchMessageDelegate
{
public:
	// 比赛初始化
	virtual void I_R_M_ContestInit(NET_ROOM_CONTEST_CHANGE_RESULT* contestChange){}
	
	//比赛信息广播
	virtual void I_R_M_ContestNotic(MSG_GR_I_ContestInfo* contestInfo){}
	
	// 用户比赛信息
	virtual void I_R_M_UserContest(MSG_GR_ContestChange* contestChange){}

	// 比赛结束
	virtual void I_R_M_ContestOver(MSG_GR_ContestAward* contestAward){}

	// 比赛淘汰
	virtual void I_R_M_ContestKick(){}

	// 等待比赛结束
	virtual void I_R_M_ContestWaitOver(){}

	// 比赛奖励
	virtual void I_R_M_ContestAwards(std::vector<NET_ROOM_CONTEST_AWARD_RESULT*>* awards){}

	// 报名数量
	virtual void I_R_M_ContestPeople(NET_ROOM_CONTEST_PEOPLE_RESULT* contestPeople){}

	// 个人参赛纪录
	virtual void I_R_M_ContestRecord(NET_ROOM_CONTEST_RECORD_RESULT* contestRecord){}
};

// 聊天接口
class IGameChartMessageDelegate
{
public:
	// 聊天消息
	virtual void I_R_M_NormalTalk(void* object, INT objectSize){}
};


// 用户行为接口
class IUserActionMessageDelegate
{
public:
	// 用户坐下消息
	virtual void I_R_M_UserSit(MSG_GR_R_UserSit * userSit, UserInfoStruct* user){}

	// 排队用户坐下
	virtual void I_R_M_QueueUserSit(BYTE deskNo, const std::vector<QUEUE_USER_SIT_RESULT*>& users){}

	// 用户站起消息
	virtual void I_R_M_UserUp(MSG_GR_R_UserUp * userUp, UserInfoStruct* user){}

	// 用户断线
	virtual void I_R_M_UserCut(INT dwUserID, BYTE bDeskNO, BYTE	bDeskStation){}

	// 用户坐下失败
	virtual void I_R_M_SitError(const std::string& message){}
};



#endif // __HN_ProtocolExport_h__
