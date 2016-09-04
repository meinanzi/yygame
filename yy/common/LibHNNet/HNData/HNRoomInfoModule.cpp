#include "HNRoomInfoModule.h"
#include "HNCommon/HNCommonMarco.h"

namespace HN
{
	HNRoomInfoModule* HNRoomInfoModule::getInstance()
	{
		static HNRoomInfoModule sRoomInfoModule;
		return &sRoomInfoModule;
	}

	HNRoomInfoModule::HNRoomInfoModule(void) : _onlineAllCount(0)
	{
		_rooms = new ROOMLIST();
	}

	HNRoomInfoModule::~HNRoomInfoModule(void)
	{
		clear();
		HN_SAFE_DELETE(_rooms);
	}

	void HNRoomInfoModule::clear()
	{
		for (auto iter = _rooms->begin(); iter != _rooms->end(); ++iter)
		{
			HN_SAFE_DELETE(*iter);
		}
		_rooms->clear();
	}

	INT	HNRoomInfoModule::getCountIncludeMatch() const
	{
		INT	roomCount = 0;
		std::vector<UINT> vecMatchRoom;
		for (auto iter = _rooms->begin(); iter != _rooms->end(); ++iter)
		{

			ComRoomInfo* pInfo = (ComRoomInfo*)*iter;
			// 不是比赛
			if(((pInfo->dwRoomRule & GRR_TIMINGCONTEST) == 0) && ((pInfo->dwRoomRule & GRR_CONTEST) == 0))
			{
				roomCount++;
				continue;
			}
			
			if ((*iter)->dwRoomRule & GRR_TIMINGCONTEST)
			{
				roomCount++;
				continue;
			}

			auto it = vecMatchRoom.begin();
			for (; it != vecMatchRoom.end(); ++it)
			{
				if ((*iter)->iContestID == *it && (*iter)->dwRoomRule == *(++it))
				{
					break;
				}
			}

			if (it == vecMatchRoom.end() )	
			{
				vecMatchRoom.push_back((*iter)->iContestID);
				vecMatchRoom.push_back((*iter)->dwRoomRule);
				roomCount++;
			}
		}

		vecMatchRoom.clear();

		return roomCount;
	}

	ComRoomInfo* HNRoomInfoModule::getByRoomID(INT roomID)
	{
		ComRoomInfo* room = nullptr;
		do 
		{
			for (auto iter =_rooms->begin(); iter != _rooms->end(); ++iter)
			{
				if ((*iter)->uRoomID == roomID)
				{
					room = *iter; 
					break;
				}
			}
		} while (0);
		return room;
	}

	ComRoomInfo* HNRoomInfoModule::getRoom(INT Postion)
	{
		if (Postion >= _rooms->size())
		{
			return nullptr;
		}

		if (!_rooms->empty())
		{
			return _rooms->at(Postion);
		}

		return nullptr;
	}

	ComRoomInfo* HNRoomInfoModule::operator [](INT postion)
	{
		if (postion >= _rooms->size())
		{
			return nullptr;
		}

		if (!_rooms->empty())
		{
			return (*_rooms)[postion];
		}

		return nullptr;
	}

	const ComRoomInfo* HNRoomInfoModule::operator [](INT postion) const
	{
		if (postion >= _rooms->size())
		{
			return nullptr;
		}

		if (!_rooms->empty())
		{
			return (*_rooms)[postion];
		}

		return nullptr;
	}

	void HNRoomInfoModule::addRoom(ComRoomInfo* room)
	{
		assert(room != nullptr);
		ComRoomInfo* rm = new ComRoomInfo;
		*rm = *room;
		_rooms->push_back(rm);
	}

	void HNRoomInfoModule::removeRoom(INT roomID)
	{
		for (auto iter = _rooms->begin(); iter != _rooms->end(); ++iter)
		{
			if ((*iter)->uRoomID == roomID) 
			{
				HN_SAFE_DELETE(*iter);
				_rooms->erase(iter);
				break;
			}
		}
	}

	ComRoomInfo* HNRoomInfoModule::findRoom(INT roomID)
	{
		ComRoomInfo* room = nullptr;
		for (auto iter = _rooms->begin(); iter != _rooms->end(); ++iter)
		{
			if ((*iter)->uRoomID == roomID) 
			{
				room = *iter;
				break;
			}
		}
		return room;
	}

	void HNRoomInfoModule::updateRoom(ComRoomInfo* room)
	{
		for (auto iter = _rooms->begin(); iter != _rooms->end(); ++iter)
		{
			if (*iter == room) 
			{
				*(*iter) = *room;
				break;
			}
		}
	}

	void HNRoomInfoModule::transform(const TransformRoomFunc& fun)
	{
		INT index = 0;
		for (auto iter = _rooms->begin(); iter != _rooms->end(); ++iter, ++index)
		{
			fun(*iter, index);
		}
	}
}
