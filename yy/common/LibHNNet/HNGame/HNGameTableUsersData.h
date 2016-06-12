#ifndef __HNGameTableUsersData_h__
#define __HNGameTableUsersData_h__

#include "HNNetExport.h"
#include "HNData/HNUserInfoModule.h"

namespace HN
{

	class HNGameTableUsersData : public IUserInfoChangedEvent
	{
	public:
		HNGameTableUsersData(BYTE deskNo);
		virtual ~HNGameTableUsersData(void);

	public:
		void update();
		void update(BYTE deskNo);

	public:
		// 获取玩家信息
		UserInfoStruct* getUserByDeskStation(BYTE bDeskStation);
		// 获取玩家信息
		UserInfoStruct* getUserByUserID(INT userID);
		// 获取玩家信息
		UserInfoStruct* getUserByIndex(BYTE index);
		// 获取旁观玩家
		void findLooker(std::vector<UserInfoStruct*>& users);
		// 获取坐桌玩家
		void findSitUsers(std::vector<UserInfoStruct*>& users);
		// 获取正在游戏玩家
		void findGameUsers(std::vector<UserInfoStruct*>& users);

		void clear();

	public:
		// 遍历用户数据
		void transform(const TransformUserFunc& func);

	public:
		virtual void onChanged(UserInfoStruct* user, COMMAND command);

	private:
		BYTE							_deskNo;
		std::vector<UserInfoStruct*>	_users;
	};

}

#endif // GameTableUsersData_h__


