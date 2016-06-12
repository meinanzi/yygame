#ifndef __HN_SocketLogic_H__
#define __HN_SocketLogic_H__

#include "HNCommon/HNCommonMarco.h"
#include "HNCommon/HNUIDelegateQueue.h"
#include "HNSocketMessage.h"
#include "HNSocketEventDelegate.h"
#include "HNSocketSelector.h"
#include "cocos2d.h"
#include <list>

USING_NS_CC;

namespace HN
{
	class HNSocketThread;
	class ISocketMessageDelegate;
	class HNSocketSelectorItem;

	class HNSocketLogic : public Ref, ISocketEventDelegate 
	{
		typedef HNSocketSelector<std::string> HNSocketMessageSelectorHandler;

	public:
		HNSocketLogic(ISocketMessageDelegate* delegate, const std::string& tag);
		virtual ~HNSocketLogic(void);

	public:
		bool init();

		bool connected() const;

	public:
		// connect server
		bool openWithIp(const CHAR* ip, INT port);

		// connect server
		bool openWithHost(const CHAR* host, INT port);

		// close socket
		bool close();

		// send data 
		INT send(UINT MainID, UINT AssistantID, UINT CheckCode, void* object, INT objectSize);

		// set handler callback event
		void addEventSelector(UINT MainID, UINT AssistantID, SEL_SocketMessage selector);

		// remove handler callback event
		void removeEventSelector(UINT MainID, UINT AssistantID);

		// send a heartbeat packet
		void heartBeat(UINT MainID, UINT AssistantID, UINT CheckCode);
		
		void resetData();

	public:
		//
		virtual void onReadComplete() override;

		// reading network data delegate
		//virtual void onReadSocketData(HNSocketMessage* socketMessage) override;
		
		// the server connection success delegate
		//virtual void onConnected(bool connect, emSocketStatus status) override;
		
		// disconnect the network delegate
		//virtual void onDisConnect() override;

	private:
		/*
		* 网络消息通知处理函数 
		*/
		void socketDataDispatch(float dt);

	private:
		HNSocketThread*					_socketThread;

		HNSocketMessageSelectorHandler*	_selectorQueue;

		ISocketMessageDelegate*			_delegate;
	};
}

#endif	//__HN_SocketLogic_H__

