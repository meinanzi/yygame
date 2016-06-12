#include "HNGameTableUsersData.h"

namespace HN
{

	HNGameTableUsersData::HNGameTableUsersData(BYTE deskNo) : _deskNo(deskNo)
	{
		UserInfoModule()->addObserver(this);
		update();
	}

	void HNGameTableUsersData::update()
	{
		UserInfoModule()->findDeskUsers(_deskNo, _users);
	}

	void HNGameTableUsersData::update(BYTE deskNo)
	{
		_deskNo = deskNo;
		UserInfoModule()->findDeskUsers(_deskNo, _users);
	}

	HNGameTableUsersData::~HNGameTableUsersData(void)
	{
		UserInfoModule()->removeObserver(this);
	}

	UserInfoStruct* HNGameTableUsersData::getUserByDeskStation(BYTE bDeskStation)
	{
		UserInfoStruct* user = nullptr;
		std::vector<UserInfoStruct*>::iterator iter = std::find_if(_users.begin(), _users.end(), 
			[&bDeskStation](UserInfoStruct* user)
			{
				return user->bDeskStation == bDeskStation;
			});
	
		if (iter != _users.end())
		{
			user = (*iter);
		}

		return user;
	}

	// 获取玩家信息
	UserInfoStruct* HNGameTableUsersData::getUserByUserID(INT userID)
	{
		UserInfoStruct* user = nullptr;
		std::vector<UserInfoStruct*>::iterator iter = std::find_if(_users.begin(), _users.end(), 
			[&userID](UserInfoStruct* user) 
			{
				return user->dwUserID == userID;
			});
	
		if (iter != _users.end())
		{
			user = (*iter);
		}

		return user;
	}

	UserInfoStruct* HNGameTableUsersData::getUserByIndex(BYTE index)
	{
		if (index >= _users.size())
		{
			return nullptr;
		}
		return _users[index];
	}

	void HNGameTableUsersData::findLooker(std::vector<UserInfoStruct*>& users)
	{
		{
			std::vector<UserInfoStruct*>().swap(users);
		}

		std::copy_if(_users.begin(), _users.end(), std::back_inserter(users), [](UserInfoStruct* user) -> bool
			{
				return user->bUserState == USER_WATCH_GAME;
			});
	}

	// 获取坐桌玩家
	void HNGameTableUsersData::findSitUsers(std::vector<UserInfoStruct*>& users)
	{
		{
			std::vector<UserInfoStruct*>().swap(users);
		}

		std::copy_if(_users.begin(), _users.end(), std::back_inserter(users), [](UserInfoStruct* user) -> bool
			{
				return user->bUserState == USER_SITTING;
			});
	}

	// 获取正在游戏玩家
	void HNGameTableUsersData::findGameUsers(std::vector<UserInfoStruct*>& users)
	{
		{
			std::vector<UserInfoStruct*>().swap(users);
		}

		std::copy_if(_users.begin(), _users.end(), std::back_inserter(users), [](UserInfoStruct* user) -> bool
			{
				return user->bUserState == USER_PLAY_GAME;
			});
	}

	void HNGameTableUsersData::clear()
	{
		_users.clear();
	}

	// 遍历用户数据
	void HNGameTableUsersData::transform(const TransformUserFunc& func)
	{
		INT index = 0;
		for (auto iter = _users.begin(); iter != _users.end(); ++iter, ++index)
		{
			func(*iter, index);
		}
	}

	void HNGameTableUsersData::onChanged(UserInfoStruct* user, COMMAND command)
	{
		switch (command)
		{
		case HN::IUserInfoChangedEvent::UNKNOW:
			break;
		case HN::IUserInfoChangedEvent::ADD:
			if(INVALID_DESKNO != _deskNo && _deskNo == user->bDeskNO)
			{
				_users.push_back(user);
			}
			break;
		case HN::IUserInfoChangedEvent::REMOVE:
		{
			auto ptr = std::find(_users.begin(), _users.end(), user);
			if (ptr != _users.end())
			{
				_users.erase(std::remove(_users.begin(), _users.end(), user));
			}
		}
			break;
		case HN::IUserInfoChangedEvent::UPDATE:
			{
				auto iter = _users.begin();
				for (; iter != _users.end(); ++iter)
				{
					if (*iter == user)
					{
						break;
					}
				}

				if(user->bDeskNO == _deskNo)
				{
					if(iter == _users.end())
					{
						_users.push_back(user);
					}
					else
					{
						**iter = *user;
					}
				}
				else
				{
					if(iter != _users.end())
					{
						_users.erase(iter);
					}
				}
			}
			break;
		case HN::IUserInfoChangedEvent::CLEAR:
			_users.clear();
			break;
		default:
			break;
		}
	}
}
