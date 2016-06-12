#ifndef __HNRoomDeskLogic_H__
#define __HNRoomDeskLogic_H__

#include "HNLogic/HNLogicBase.h"
#include "HNNetExport.h"

namespace HN
{
	class IHNRoomDeskLogic
	{
	public:
		virtual void onRoomSitCallback(bool success, const std::string& message, UINT roomID, BYTE deskNo, BYTE seatNo){}
		virtual void onRoomDeskLock(BYTE deskNo) {}
		virtual void onRoomDeskUnLock(BYTE deskNo) {}
		virtual void onRoomUserCome(INT userID){}
		virtual void onRoomUserLeft(INT userID){}
		virtual void onRoomDeskUserCountChanged(){}
	};


	class HNRoomDeskLogic:public HNLogicBase, public IRoomMessageDelegate
	{
	public:
		virtual void start() override;
		virtual void stop() override;
		void requestSit(BYTE deskNo, const std::string& password = "");
		void requestSit(BYTE deskNo, BYTE seatNo, const std::string& password = "");

		// 快速坐下
		void requestQuickSit();

		// 构造函数
		HNRoomDeskLogic(IHNRoomDeskLogic* callback);

		// 析构函数
		virtual ~HNRoomDeskLogic();
	
		// 用户坐下
		virtual void I_R_M_UserSit(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) override;

		// 用户坐下失败
		virtual void I_R_M_SitError(const std::string& message) override;

		// 用户站起
		virtual void I_R_M_UserUp(MSG_GR_R_UserUp * userUp, UserInfoStruct* user) override;

		// 锁桌
		virtual void I_R_M_LockDesk(BYTE deskNo) override;

		// 取消锁桌
		virtual void I_R_M_UnLockDesk(BYTE deskNo) override;

		// 用户进入
		virtual void I_R_M_UserCome(UserInfoStruct* user) override;

		// 用户离开
		virtual void I_R_M_UserLeft(UserInfoStruct* user) override;
	protected:
		IHNRoomDeskLogic* _callback;
		UINT _roomID;
	};
}

#endif