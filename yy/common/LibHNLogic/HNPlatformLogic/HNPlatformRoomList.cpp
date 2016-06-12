#include "HNPlatformLogic/HNPlatformRoomList.h"

namespace HN
{
	void HNPlatformRoomList::requestRoomList()
	{
		platformRequestRoomList();
	}

	void HNPlatformRoomList::requestRoomLogin()
	{
		roomRequestLogin();
	}

	void HNPlatformRoomList::start()
	{
		HNPlatformLogicBase::start();
		RoomLogic()->addRoomObserver(this);
	}

	void HNPlatformRoomList::stop()
	{
		RoomLogic()->removeRoomObserver(this);
		HNPlatformLogicBase::stop();
	}

	void HNPlatformRoomList::I_P_M_RoomList()
	{
		_callback->onPlatformRoomListCallback(true, GBKToUtf8("获取成功"));
	}

	void HNPlatformRoomList::I_P_M_RoomUserCount(UINT roomID, UINT peopleCount, UINT virtualCount)
	{
		_callback->onPlatformRoomUserCountCallback(roomID, peopleCount + virtualCount);
	}

	void HNPlatformRoomList::I_R_M_UserSit(MSG_GR_R_UserSit * userSit, UserInfoStruct* user)
	{
		if(nullptr == userSit || nullptr == user)
		{
			return;
		}

		if(INVALID_DESKNO != userSit->bDeskIndex && INVALID_DESKSTATION != userSit->bDeskStation)
		{
			_callback->onRoomSitCallback(true, GBKToUtf8("坐下成功"), userSit->dwUserID, userSit->bDeskIndex, userSit->bDeskStation);
			return;
		}
	}

	void HNPlatformRoomList::I_R_M_SitError(const std::string& message)
	{
		_callback->onRoomSitCallback(false, message, INVALID_USER_ID, INVALID_DESKNO, INVALID_DESKSTATION);
	}

	HNPlatformRoomList::HNPlatformRoomList(IHNPlatformRoomList* callback)
		: HNPlatformLogicBase(callback)
	{
		CCAssert(callback != nullptr, "callback is null.");
		_callback = callback;
	}

	HNPlatformRoomList::~HNPlatformRoomList()
	{
		_callback = nullptr;
	}

	void HNPlatformRoomList::platformRequestRoomList()
	{
		UINT gameNameID = GameCreator()->getCurrentGameNameID();
		UINT gameKindID = GameCreator()->getCurrentGameKindID();

		if(0 == gameKindID || -1 == gameKindID)
		{
			_callback->onPlatformRoomListCallback(false, GBKToUtf8("游戏id或kindid错误"));
			return;
		}

		PlatformLogic()->getRoomList(gameKindID, gameNameID);
	}

	void HNPlatformRoomList::roomRequestLogin()
	{
		ComRoomInfo* pRoomInfo = RoomLogic()->getSelectedRoom();
		RoomLogic()->connect(pRoomInfo->szServiceIP, pRoomInfo->uServicePort);
	}
}