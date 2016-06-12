#ifndef _HN_HNRoomInfoModule_h__
#define _HN_HNRoomInfoModule_h__

#include "HNNetProtocol/HNProtocolExport.h"
#include <vector>
#include <functional>

namespace HN
{
	typedef std::function<void (ComRoomInfo*, INT index)> TransformRoomFunc;

	class HNRoomInfoModule
	{
		typedef std::vector<ComRoomInfo*> ROOMLIST;

	public:
		static HNRoomInfoModule* getInstance();

	public:
		INT getOnlineAllCount() const
		{
			return _onlineAllCount;
		}

		void setOnlineAllCount(INT count)
		{
			_onlineAllCount = count;
		}

		INT getRoomCount() const
		{
			return _rooms->size();
		}

		/*
		* 获取房间个数（比赛房间合并）
		*/
		INT	getCountIncludeMatch() const;

		/*
		* 获取房间信息
		*/
		ComRoomInfo* getByRoomID(INT roomID);

		/*
		* 获取房间信息
		*/
		ComRoomInfo* getRoom(INT postion);
		ComRoomInfo* operator [](INT postion);
		const ComRoomInfo* operator [](INT postion) const;

		/*
		* 添加房间数据
		*/
		void addRoom(ComRoomInfo* room);
		/*
		* 删除房间数据
		*/
		void removeRoom(INT roomID);
		/*
		* 更新房间数据
		*/
		void updateRoom(ComRoomInfo* room);
		/*
		* 查找房间数据
		*/
		ComRoomInfo* findRoom(INT roomID);
		/*
		* 清空房间数据
		*/
		void clear();
		/*
		* 遍历房间数据
		*/
		void transform(const TransformRoomFunc& fun);

	private:
		INT				_onlineAllCount;
		// 房间列表
		ROOMLIST*		_rooms;

	private:
		HNRoomInfoModule();
		~HNRoomInfoModule();
	};
}

#define RoomInfoModule()	HNRoomInfoModule::getInstance()

#endif // _HN_HNRoomInfoModule_h__
