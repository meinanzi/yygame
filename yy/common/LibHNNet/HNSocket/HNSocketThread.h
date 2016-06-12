#ifndef __NH_SocketThread_H__
#define __NH_SocketThread_H__

#include "HNBaseType.h"
#include "HNCommon/HNCommonMarco.h"
#include <string>
#include <vector>
#include <deque>
#include <functional>
#include <mutex>
#include <thread>

#define TCP_BUFSIZE_READ	16400

namespace HN 
{
	class ISocketEventDelegate;
	class HNSocketMessage;
	class HNSocket;

	template<typename _Type>
	class HNSocketDataCacheQueueT
	{
	public:
		HNSocketDataCacheQueueT(void) { _datas = new std::vector<_Type>(); }
		~HNSocketDataCacheQueueT(void) { HN_SAFE_DELETE(_datas); }

	public:
		void push(_Type* _value, INT size)
		{
			INT i = 0;
			CHAR* p = _value;
			while (i++ < size)
			{
				_datas->push_back(*p++);
			}
		}

		void pop(INT pos)
		{
			_datas->erase(_datas->begin(), _datas->begin() + pos);
		}

		size_t size()
		{
			return _datas->size();
		}

		_Type* front()
		{		
			return _datas->data();
		}

		void clear()
		{
			_datas->clear();
		}

	private:
		std::vector<_Type>* _datas;
	};

	class HNSocketThread
	{
		typedef std::deque<HNSocketMessage*>		HNSocketMessageQueue;
		typedef HNSocketDataCacheQueueT<CHAR>		HNSocketDataCacheQueue;

	public:
		HNSocketThread(ISocketEventDelegate* delegate);
		virtual ~HNSocketThread(void);

	public:
		HN_PROPERTY_PASS_BY_REF(std::string, Tag, tag)

		/*
		* 打开服务端连接
		*/
		bool openWithIp(const CHAR* ip, INT port);

		/*
		* 打开服务端连接
		*/
		bool openWithHost(const CHAR* host, INT port);

		/*
		* 关闭服务端连接
		*/
		bool close();

		/*
		* 发送服务端数据
		*/
		INT send(char* object, INT objectSize);
		
		/*
		*
		*/
		bool connected() const { return _connected; } 
		
		void transform(std::function<void(HNSocketMessage* socketMessage)> func);

	private:
		/*
		* 线程执行函数
		*/
		void onSocketReadThread();

		void onRead(const CHAR* buffer, INT recvSize);

	private:
		/*
		* 网络消息通知处理函数 
		*/
		//void socketDataDispatch(float dt);

		// 初始化socket
		bool initSocket();

		void clear();

	private:
		bool							_threadExit;

		std::mutex						_dataMutex;

		HNSocketDataCacheQueue*			_socketDataCacheQueue;

		HNSocket*						_socket;

		bool							_connected;

		HNSocketMessageQueue*			_socketMessageQueue;

		ISocketEventDelegate*			_delegate;

		CHAR							_readBuffer[TCP_BUFSIZE_READ];

	private:
		//std::thread*					_recvThread;
	};

};

#endif	//__NH_HNSocketThread_H__
