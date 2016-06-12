#include "HNRoomDeskLogic.h"

namespace HN
{
	void HNRoomDeskLogic::start()
	{
		RoomLogic()->addRoomObserver(this);
	}

	void HNRoomDeskLogic::stop()
	{
		RoomLogic()->removeRoomObserver(this);
	}

	void HNRoomDeskLogic::requestSit(BYTE deskNo, const std::string& password)
	{
		ComRoomInfo* pRoomInfo = RoomLogic()->getSelectedRoom();
		if(nullptr == pRoomInfo)
		{
			_callback->onRoomSitCallback(false, GBKToUtf8("房间信息为空"), _roomID, deskNo, INVALID_DESKSTATION);
			return;
		}

		std::vector<UserInfoStruct*> deskUsers;
		UserInfoModule()->findDeskUsers(deskNo, deskUsers);

		std::vector<bool> empty(pRoomInfo->uDeskPeople, true);

		// 查找坐下的用户座位号 
		for (auto user : deskUsers)
		{
			if (USER_SITTING == user->bUserState		    // 坐下
				|| USER_PLAY_GAME == user->bUserState		// 游戏中
				|| USER_CUT_GAME == user->bUserState		// 断线
				|| USER_ARGEE == user->bUserState)			// 同意游戏
			{
				empty[user->bDeskStation] = false;
			}
		}

		BYTE seatNo = INVALID_DESKSTATION;
		for(size_t i = 0; i < empty.size(); i++)
		{
			if(empty[i])
			{
				seatNo = i; 
				break;
			}
		}

		if(INVALID_DESKSTATION != seatNo)
		{
			requestSit(deskNo, seatNo, password);
		}	
		else
		{
			_callback->onRoomSitCallback(false, GBKToUtf8("房间没有空座"), _roomID, deskNo, seatNo);
		}
	}

	void HNRoomDeskLogic::requestSit(BYTE deskNo, BYTE seatNo, const std::string& password)
	{
		MSG_GR_S_UserSit data;
		data.bDeskIndex   = deskNo;
		data.bDeskStation = seatNo;
		strcpy(data.szPassword, password.c_str());
		RoomLogic()->sendData(MDM_GR_USER_ACTION, ASS_GR_USER_SIT, &data ,sizeof(data));
	}

	void HNRoomDeskLogic::requestQuickSit()
	{
		RoomLogic()->sendData(MDM_GR_USER_ACTION, ASS_GR_FAST_JOIN_IN, nullptr, 0);
	}

	HNRoomDeskLogic::HNRoomDeskLogic(IHNRoomDeskLogic* callback)
	{
		CCAssert(callback != nullptr, "callback is null.");
		_callback = callback;
		ComRoomInfo* pRoomInfo = RoomLogic()->getSelectedRoom();
		CCAssert(pRoomInfo != nullptr, "room info is null.");
		_roomID = pRoomInfo->uRoomID;
	}

	HNRoomDeskLogic::~HNRoomDeskLogic()
	{
		_callback = nullptr;
	}

	void HNRoomDeskLogic::I_R_M_UserSit(MSG_GR_R_UserSit * userSit, UserInfoStruct* user)
	{
		if (nullptr == userSit ||  nullptr == user) 
		{			
			return;
		}

		// 断线重连进来
		if (userSit->dwUserID == PlatformLogic()->loginResult.dwUserID)
		{
			if (INVALID_DESKNO != userSit->bDeskIndex && INVALID_DESKSTATION != userSit->bDeskStation)
			{
				_callback->onRoomSitCallback(true, GBKToUtf8("坐下成功"), _roomID, userSit->bDeskIndex, userSit->bDeskStation);
			}
		}
		else
		{
			_callback->onRoomDeskUserCountChanged();
		}
	}

	// 用户坐下失败
	void HNRoomDeskLogic::I_R_M_SitError(const std::string& message)
	{
		_callback->onRoomSitCallback(false, message, _roomID, INVALID_DESKNO, INVALID_DESKSTATION);
	}

	void HNRoomDeskLogic::I_R_M_UserUp(MSG_GR_R_UserUp * userUp, UserInfoStruct* user)
	{
		_callback->onRoomDeskUserCountChanged();
	}

	void HNRoomDeskLogic::I_R_M_LockDesk(BYTE deskNo)
	{
		_callback->onRoomDeskLock(deskNo);
	}

	void HNRoomDeskLogic::I_R_M_UnLockDesk(BYTE deskNo)
	{
		_callback->onRoomDeskUnLock(deskNo);
	}

	void HNRoomDeskLogic::I_R_M_UserCome(UserInfoStruct* user)
	{
		_callback->onRoomUserCome(user->dwUserID);
	}
	
	void HNRoomDeskLogic::I_R_M_UserLeft(UserInfoStruct* user)
	{
		_callback->onRoomUserLeft(user->dwUserID);
	}
}