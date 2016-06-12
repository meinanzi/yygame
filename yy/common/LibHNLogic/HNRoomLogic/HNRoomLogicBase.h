#ifndef __HNRoomLogicBase_H__
#define __HNRoomLogicBase_H__

#include "HNLogic/HNLogicBase.h"

namespace HN
{
	class IHNRoomLogicBase
	{
	public:
		virtual void onRoomLoginCallback(bool success, const std::string& message, UINT roomID){}
		virtual void onRoomSitCallback(bool success, const std::string& message, UINT roomID, BYTE deskNo, BYTE seatNo){}
		virtual void onRoomQueueSitCallback(bool success, const std::string& message, UINT roomID, BYTE deskNo){}
		virtual void onRoomSitUpCallback(bool success, const std::string& message, BYTE destNo, BYTE seatNo){}
		virtual void onRoomUserCome(INT userID){}
		virtual void onRoomUserLeft(INT userID){}
		virtual void onRoomDisConnect(const std::string &message){}
		// 房间密码
		virtual void onPlatformRoomPassEnter(bool success, UINT roomId){}
	};

	class IRoomMessageDelegate;
	/*
	 * redbird room logic base.
	 */
	class HNRoomLogicBase: public HNLogicBase, public IRoomMessageDelegate, public IPlatformMessageDelegate
	{
	public:
		virtual void start() override;
		virtual void stop() override;
		void requestLogin(UINT roomID);
		void requestSit(BYTE deskNo);
		void requestSit(BYTE deskNo, BYTE seatNo);
		void requestJoinQueue();

		// 快速坐下
		void requestQuickSit();

		void requestRoomPasword(UINT roomId, const std::string& password);
	public:
		// 比赛初始化
		virtual void I_R_M_ContestInit(NET_ROOM_CONTEST_CHANGE_RESULT* contestChange){}

		// 用户比赛信息
		virtual void I_R_M_UserContest(NET_ROOM_CONTEST_CHANGE_RESULT* contestChange){}

		// 比赛结束
		virtual void I_R_M_ContestOver(NET_ROOM_CONTEST_AWARD_RESULT* contestAward){}

		// 比赛淘汰
		virtual void I_R_M_ContestKick(){}

		// 比赛等待结束
		virtual void I_R_M_ContestWaitOver(){}

		// 比赛奖励
		virtual void I_R_M_ContestAwards(std::vector<NET_ROOM_CONTEST_AWARD_RESULT*>* awards){}

		// 比赛报名人数
		virtual void I_R_M_ContestPeople(NET_ROOM_CONTEST_PEOPLE_RESULT* contestPeople){}

		// 比赛记录
		virtual void I_R_M_ContestRecord(NET_ROOM_CONTEST_RECORD_RESULT* contestRecord){}

		// 普通聊天
		virtual void I_R_M_NormalTalk(void* object, INT objectSize){}

		// 用户坐下
		virtual void I_R_M_UserSit(MSG_GR_R_UserSit * userSit, UserInfoStruct* user);

		// 用户坐下失败
		virtual void I_R_M_SitError(const std::string& message) override;

		// 用户排队坐下
		virtual void I_R_M_QueueUserSit(BYTE deskNo, const std::vector<QUEUE_USER_SIT_RESULT*>& users);

		// 用户站起
		virtual void I_R_M_UserUp(MSG_GR_R_UserUp * userUp, UserInfoStruct* user);

		// 用户断线
		virtual void I_R_M_UserCut(INT dwUserID, BYTE bDeskNO, BYTE	bDeskStation) {}

		// 断开连接
		virtual void I_R_M_DisConnect() override;

		// 网络连接
		virtual void I_R_M_Connect(bool result) override;

		// 房间登录
		virtual void I_R_M_Login(bool success, UINT handleCode, const std::string& message) override;

		// 登录完成
		virtual void I_R_M_LoginFinish() override;

		// 用户进入
		virtual void I_R_M_UserCome(UserInfoStruct* user) override;

		// 用户离开
		virtual void I_R_M_UserLeft(UserInfoStruct* user) override;

		// 结算消息
		virtual void I_R_M_GamePoint(void * object, INT objectSize) override;

		// 房间密码
		virtual void I_P_M_RoomPassword(MSG_GP_S_C_CheckRoomPasswd* pData) override;
	public:
		HNRoomLogicBase(IHNRoomLogicBase* callback);
		virtual ~HNRoomLogicBase();
	protected:
		IHNRoomLogicBase* _callback;
		UINT _roomID;
	};
}

#endif