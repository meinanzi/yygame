#include "HNGameUIBase.h"
#include "HNRoom/HNRoomLogic.h"

namespace HN
{
	HNGameUIBase::HNGameUIBase()
	{

	}

	HNGameUIBase::~HNGameUIBase()
	{

	}

	void HNGameUIBase::sendData(UINT MainID, UINT AssistantID, void* object, INT objectSize)
	{
		RoomLogic()->sendData(MainID, AssistantID, object, objectSize);
	}
}