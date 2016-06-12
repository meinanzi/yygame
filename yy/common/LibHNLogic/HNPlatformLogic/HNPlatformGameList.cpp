#include "HNPlatformLogic/HNPlatformGameList.h"

namespace HN
{
	void HNPlatformGameList::requestGameList()
	{
		platformRequestGameList();
	}

	void HNPlatformGameList::I_P_M_GameList()
	{
		_callback->onPlatformGameListCallback(true, GBKToUtf8("获取游戏列表成功"));
	}

	void HNPlatformGameList::I_P_M_GameUserCount(DL_GP_RoomListPeoCountStruct* userCount)
	{
		UINT count = userCount->uOnLineCount + userCount->uVirtualUser;
		_callback->onPlatformGameUserCountCallback(userCount->uID, count);
	}

	HNPlatformGameList::HNPlatformGameList(IHNPlatformGameList* callback)
		: HNPlatformLogicBase(callback)
	{
		CCAssert(callback != nullptr, "callback is null.");
		_callback = callback;
	}

	HNPlatformGameList::~HNPlatformGameList()
	{

	}

	void HNPlatformGameList::platformRequestGameList()
	{
		PlatformLogic()->sendData(MDM_GP_LIST, ASS_GP_LIST_KIND);
	}
}