#ifndef __HNPlatformGameList_H__
#define __HNPlatformGameList_H__

#include "HNPlatformLogic/HNPlatformLogicBase.h"

namespace HN
{
	/*
	 * platform game list interface.
	 */
	class IHNPlatformGameList: public IHNPlatformLogicBase
	{
	public:
		virtual void onPlatformGameListCallback(bool success, const std::string& message){}
		virtual void onPlatformGameUserCountCallback(UINT Id, UINT count){}
	};

	/*
	 * platform room list logic.
	 */
	class HNPlatformGameList:public HNPlatformLogicBase
	{
	public:
		void requestGameList();
	public:
		// 游戏列表
		virtual void I_P_M_GameList() override;

		// 游戏列表人数更新
		virtual void I_P_M_GameUserCount(DL_GP_RoomListPeoCountStruct* userCount) override;
	public:
		HNPlatformGameList(IHNPlatformGameList* callback);
		virtual ~HNPlatformGameList();
	protected:
		IHNPlatformGameList* _callback;
		void platformRequestGameList();
	};
}

#endif