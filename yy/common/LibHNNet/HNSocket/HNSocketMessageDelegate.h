#ifndef __HN_HNGameMessageDelegate_H__
#define __HN_HNGameMessageDelegate_H__

#include "HNBaseType.h"
#include "HNNetProtocol/HNProtocolExport.h"
#include "HNSocketMessage.h"

namespace HN {

	class ISocketMessageDelegate
	{
	public:
		virtual ~ISocketMessageDelegate(void) {}
		virtual void onSocketMessage(HNSocketMessage* socketMessage) = 0;
		virtual void onConnected(bool connect, emSocketStatus status) = 0;
		virtual void onDisConnect() = 0;
	};

};

#endif	//__HN_HNGameMessageDelegate_H__

