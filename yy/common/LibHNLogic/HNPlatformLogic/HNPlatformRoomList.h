#ifndef __HNPlatformRoomList_H__
#define __HNPlatformRoomList_H__

#include "HNPlatformLogic/HNPlatformLogicBase.h"
#include "HNNetExport.h"
#include <string>

namespace HN
{
	/*
	 * redbird platform room interface.
	 */	
	class IHNPlatformRoomList: public IHNPlatformLogicBase
	{
	public:
		virtual void onPlatformRoomListCallback(bool success, const std::string& message){}
		virtual void onPlatformRoomUserCountCallback(UINT roomID, UINT userCount){}
		virtual void onRoomSitCallback(bool success, const std::string& message, INT userID, BYTE deskNo, BYTE seatNo){}
	};

	/*
	 * redbird platform room logic
	 */
	class HNPlatformRoomList: public HNPlatformLogicBase, public IRoomMessageDelegate
	{
	public:
		void requestRoomList();
		void requestRoomLogin();
	public:
		virtual void start() override;
		virtual void stop() override;
		
		// 房间列表
		virtual void I_P_M_RoomList() override;

		// 房间人数
		virtual void I_P_M_RoomUserCount(UINT roomID, UINT peopleCount, UINT virtualCount) override;

		// 用户坐下
		virtual void I_R_M_UserSit(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) override;

		// 用户坐下失败
		virtual void I_R_M_SitError(const std::string& message) override;
	public:
		HNPlatformRoomList(IHNPlatformRoomList* callback);
		virtual ~HNPlatformRoomList();
	protected:
		IHNPlatformRoomList* _callback;
		void platformRequestRoomList();
		void roomRequestLogin();
	};
}

#endif