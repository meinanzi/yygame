#ifndef _HN_HNUserModule_h__
#define _HN_HNUserModule_h__

#include "HNCommon/HNUIDelegateQueue.h"
#include "HNNetProtocol/HNProtocolExport.h"
#include <vector>
#include <functional>

namespace HN
{
	typedef std::vector<UserInfoStruct*> USERLIST;

	typedef std::function<void (UserInfoStruct*, INT index)> TransformUserFunc;

	class IUserInfoChangedEvent
	{
	public:
		enum COMMAND
		{
			UNKNOW,
			ADD,
			REMOVE,
			UPDATE,
			CLEAR
		};
	public:
		virtual ~IUserInfoChangedEvent() {}
		virtual void onChanged(UserInfoStruct* user, COMMAND command) = 0;
	};
	
	// 维护所有的游戏用户数据
	class HNUserInfoModule
	{
	public:
		static HNUserInfoModule* getInstance();

	// 属性接口
	public:
		INT getUsersCount() const
		{
			return (INT)_onlineUsers->size();
		}

	// 功能接口
	public:
		// 添加数据观察者
		void addObserver(IUserInfoChangedEvent* delegate);
		// 删除数据观察者
		void removeObserver(IUserInfoChangedEvent* delegate);

	// 功能接口
	public:
		// 删除用户信息
		void removeUser(INT userID);
		// 更新用户信息
		void updateUser(UserInfoStruct* user);
		// 添加用户信息
		UserInfoStruct* addUser(UserInfoStruct* user);

	// 查找功能接口
	public:
		// 通过用户ID查找用户
		UserInfoStruct* findUser(INT userID);
		// 通过桌号，座位号查找用户
		UserInfoStruct* findUser(BYTE deskNo, BYTE station);
		// 获取旁观玩家
		void findLooker(BYTE deskNO, std::vector<UserInfoStruct*>& users);
		// 获取桌子玩家
		void findDeskUsers(BYTE deskNO, std::vector<UserInfoStruct*>& users);
		// 获取游戏玩家
		void findGameUsers(BYTE deskNO, std::vector<UserInfoStruct*>& users);

		// 遍历数据功能接口
	public:
		// 遍历用户数据
		void transform(const TransformUserFunc& func);

		// 遍历用户数据
		void transform(BYTE bDeskNO, const TransformUserFunc& func);

	public:
		// 清空数据
		void clear();

	private:
		void onNotify(UserInfoStruct* user, IUserInfoChangedEvent::COMMAND command);

	private:
		// 在线用户列表
		USERLIST*	_onlineUsers;

		// 数据变换通知队列
		std::vector<IUserInfoChangedEvent*>* _dataChangedQueue;

	private:
		HNUserInfoModule();
		~HNUserInfoModule();
	};
}

#define UserInfoModule()	HNUserInfoModule::getInstance()

#endif // _HN_HNUserModule_h__
