#ifndef __NH_HNSocketMessageDelegate_H__
#define __NH_HNSocketMessageDelegate_H__

#include "HNBaseType.h"
#include "HNNetProtocol/HNProtocolExport.h"
#include "HNSocketMessage.h"

namespace HN 
{

	class ISocketEventDelegate
	{
	public:
		virtual ~ISocketEventDelegate() {}
		//virtual void onReadSocketData(HNSocketMessage* socketMessage) = 0;
		virtual void onReadComplete() = 0;
		//virtual void onConnected(bool connect, emSocketStatus status) = 0;
		//virtual void onDisConnect() = 0;
	};

}

#endif	//__NH_HNSocketMessageDelegate_H__
