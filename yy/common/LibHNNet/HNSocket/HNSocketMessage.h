#ifndef __NH_HNSocketMessage_H__
#define __NH_HNSocketMessage_H__

#include "HNBaseType.h"
#include "HNNetProtocol/HNProtocolExport.h"
#include <functional>
#include <string>

namespace HN 
{

	#define SELECTER_KEY_FORMAT	"%u_%u"

	class HNSocketMessage;

	typedef std::function<bool(HNSocketMessage* socketMessage)> SEL_SocketMessage;

	#define HN_SOCKET_CALLBACK(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, ##__VA_ARGS__)


	class HNSocketMessage 
	{
		public:
			static HNSocketMessage* getMessage();
			static void releaseMessage(HNSocketMessage* message);

		public:
			void setMessage(const NetMessageHead* head, CHAR* obj, INT Size);

		private:
			HNSocketMessage();
			~HNSocketMessage();

 			void *operator new(std::size_t ObjectSize);
 			void operator delete(void *ptrObject);

		public:
			NetMessageHead messageHead;
			UINT objectSize;			// message size
			CHAR object[4096];
			emSocketStatus socketStatus;
			std::string strKey;
			
	};

	class HNSocketSelectorItem
	{
	public:
		HNSocketSelectorItem(SEL_SocketMessage selector) : _selector(selector)
		{
		}

	public:
		bool doCallSelector(HNSocketMessage* socketMessage)
		{ 
			return (_selector)(socketMessage);
		}

	private:
		SEL_SocketMessage	_selector;
	};
};

#endif	//__NH_HNSocketMessage_H__
