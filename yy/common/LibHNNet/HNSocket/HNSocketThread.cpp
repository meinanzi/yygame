#include "HNSocketThread.h"
#include "HNSocket.h"
#include "HNSocketMessage.h"
#include "HNSocketProtocolData.h"
#include "HNSocketEventDelegate.h"
#include "HNCommon/HNLog.h"
#include "HNCommon/HNCommonMarco.h"

namespace HN
{
	static const INT TCP_RECV_BUFFER_SIZE = 100*1024;

	static const INT TCP_SEND_BUFFER_SIZE = 16400;

	HNSocketThread::HNSocketThread(ISocketEventDelegate* delegate) 
		: _threadExit(false)
		, _connected(false)
		//, _recvThread(nullptr)
		, _delegate(delegate)
	{
		memset(_readBuffer, 0x0, sizeof(_readBuffer));
		_socket = new HNSocket();
		_socketDataCacheQueue = new HNSocketDataCacheQueue();	
		_socketMessageQueue = new HNSocketMessageQueue();
	}

	HNSocketThread::~HNSocketThread(void)
	{
		_threadExit = true;
		_socket->close();
		_socket->shutdown();
		/*if (_recvThread->joinable())
		{
		_recvThread->join();
		}*/

		for (auto iter = _socketMessageQueue->begin(); iter != _socketMessageQueue->end();)
		{
			HNSocketMessage* message = *iter;
			HNSocketMessage::releaseMessage(message);
			iter = _socketMessageQueue->erase(iter);
		}
		_socketMessageQueue->clear();
		HN_SAFE_DELETE(_socketMessageQueue);

		HN_SAFE_DELETE(_socketDataCacheQueue);
		//HN_SAFE_DELETE(_recvThread);
		HN_SAFE_DELETE(_socket);
	}

	bool HNSocketThread::openWithIp(const CHAR* ip, INT port)
	{
		assert(ip != nullptr);
		if (!_connected)
		{
			_socket->setIP(ip);
			_socket->setPort(port);
			std::thread recvThread(std::bind(&HNSocketThread::onSocketReadThread, this));
			recvThread.detach();
		}
		return true;
	}

	bool HNSocketThread::openWithHost(const CHAR* host, INT port)
	{
		assert(host != nullptr);
		if (!_connected)
		{
			_socket->setHost(host);
			_socket->setPort(port);
			std::thread recvThread(std::bind(&HNSocketThread::onSocketReadThread, this));
			recvThread.detach();
		}
		return true;
	}

	bool HNSocketThread::initSocket()
	{
		bool ret = false;
		do
		{
			//_ips = HNSocket::getIpAddress("www.boss666.com");
			//ips = HNSocket::getIpAddress("localhost");

			// create socket
			if (!_socket->create())
			{
				break;
			}

			// set recv buffer
			_socket->setSoRcvbuf(TCP_RECV_BUFFER_SIZE);

			// set send buffer
			_socket->setSoSendbuf(TCP_SEND_BUFFER_SIZE);

			INT err = _socket->connect();

			if (TCP_CONNECT_OUTTIME == err)
			{
				HNSocketMessage* SocketMessage = HNSocketMessage::getMessage();
				SocketMessage->socketStatus = SocketStatus_OUTTIME;
				{
					std::lock_guard <std::mutex> autoLock(_dataMutex);
					_socketMessageQueue->push_back(SocketMessage);
				}
				_delegate->onReadComplete();
				break;
			}
        
			if (TCP_CONNECT_ERROR == err)
			{
				HNSocketMessage* SocketMessage = HNSocketMessage::getMessage();
				SocketMessage->socketStatus = SocketStatus_ERROR;
				{
					std::lock_guard <std::mutex> autoLock(_dataMutex);
					_socketMessageQueue->push_back(SocketMessage);
				}
				_delegate->onReadComplete();
				break;
			}
			_connected = true;
			_threadExit = false;
			ret = true;
		} while (0);

		return ret;
	}

	void HNSocketThread::clear()
	{
		_connected = false;
		_threadExit = true;
		_socketDataCacheQueue->clear();
	}

	bool HNSocketThread::close()
	{
		if (_connected)
		{
			_connected = false;
			_threadExit = true;	
			_socket->close();
			_socket->shutdown();
			/*if (_recvThread->joinable())
			{
				_recvThread->join();
			}*/
			return true;
		}
		return false;
	}

	INT HNSocketThread::send(char* object, INT objectSize)
	{
		if (_connected)
		{
			return _socket->send(object, objectSize);
		}
		HNLOG_ERROR ("error: network not connected");
		return -1;
	}

	void HNSocketThread::onSocketReadThread()
	{
		if (!initSocket())
		{
			return;
		}

		//std::this_thread::sleep_for(std::chrono::milliseconds(50));

		HNLOG_ERROR ("%s >> onSocketThread >> begin.", _tag.c_str());

		timeval timeOut, timeout_copy;
		timeOut.tv_sec = 0;
		timeOut.tv_usec = 16000;
		
		while (true)
		{
			if (_threadExit) break;

			timeout_copy = timeOut;
        
			INT nready = _socket->select(&timeout_copy);
			// outtime
			if (0 == nready)
			{
				continue;
			}

			// network error
			if (nready < 0)
			{
				HNLOG_ERROR("%s >> %s", _tag.c_str(), strerror(errno));

				HNSocketMessage* SocketMessage = HNSocketMessage::getMessage();
				SocketMessage->socketStatus = SocketStatus_DISCONNECT;
				{
					std::lock_guard <std::mutex> autoLock(_dataMutex);
					_socketMessageQueue->push_back(SocketMessage);
				}
				_delegate->onReadComplete();
				break;
			}
        
			INT recvSize = 0;
			recvSize = _socket->recv(_readBuffer, sizeof(_readBuffer));
        
			// server close
			if (0 == recvSize)	
			{
				HNLOG_ERROR("%s >> server close >> %s",  _tag.c_str(), strerror(errno));
				HNSocketMessage* SocketMessage = HNSocketMessage::getMessage();
				SocketMessage->socketStatus = SocketStatus_DISCONNECT;
				{
					std::lock_guard <std::mutex> autoLock(_dataMutex);
					_socketMessageQueue->push_back(SocketMessage);
				}
				_delegate->onReadComplete();
				break;
			}
        
			if (recvSize > 0)
			{
				onRead(_readBuffer, recvSize);
			} 
		}
	
		clear();
	
		HNLOG_ERROR ("%s >> onSocketThread >> end.", _tag.c_str());
	}

	void HNSocketThread::onRead(const CHAR* buffer, INT recvSize)
	{
		// cache network data
		_socketDataCacheQueue->push(_readBuffer, recvSize);

		const UINT NetMessageHeadSize = sizeof(NetMessageHead);

		NetMessageHead* pMessageHead = nullptr;
	
		UINT messageSize = (UINT)_socketDataCacheQueue->size();
		if (messageSize >= NetMessageHeadSize)
		{
			do
			{
				pMessageHead = (NetMessageHead*) _socketDataCacheQueue->front();
				if (nullptr != pMessageHead && messageSize >= pMessageHead->uMessageSize)
				{
					//HNLOG_INFO("%s >> MainID = [%lu] AssistantID = [%lu] MessageSize = [%lu]", _tag.c_str(), pMessageHead->bMainID, pMessageHead->bAssistantID, pMessageHead->uMessageSize);

					CHAR* pData = _socketDataCacheQueue->front() + NetMessageHeadSize;
					HNSocketMessage* SocketMessage = HNSocketMessage::getMessage();
					if (SocketMessage)
					{
						SocketMessage->setMessage(pMessageHead, pData, pMessageHead->uMessageSize - NetMessageHeadSize);
						SocketMessage->socketStatus = SocketStatus_RECV;
						_socketDataCacheQueue->pop(pMessageHead->uMessageSize);

						if(MDM_CONNECT == SocketMessage->messageHead.bMainID && ASS_NET_TEST_1 == SocketMessage->messageHead.bAssistantID)
						{						
							if(_tag.compare("platform") == 0 && INVALID_VALUE != HNSocketProtocolData::PlatformCheckCode)
							{
								HNSocketProtocolData data;
								data.createPackage(MDM_CONNECT, ASS_NET_TEST_1, HNSocketProtocolData::PlatformCheckCode);
								send(data.getPackage(), data.getPackageSize());
							}
							else if(_tag.compare("game") == 0 && INVALID_VALUE != HNSocketProtocolData::GameCheckCode)
							{
								HNSocketProtocolData data;
								data.createPackage(MDM_CONNECT, ASS_NET_TEST_1, HNSocketProtocolData::GameCheckCode);
								send(data.getPackage(), data.getPackageSize());
							}				
						}

						// dispatch network data
						{
							bool bEnterBackground = Configuration::getInstance()->getValue("bEnterBackground", Value(false)).asBool();
							bool bControl = Configuration::getInstance()->getValue("bControl", Value(false)).asBool();

							std::lock_guard <std::mutex> autoLock(_dataMutex);
							if (bControl && bEnterBackground)
							{
								if (SocketMessage->messageHead.bMainID != MDM_GM_GAME_NOTIFY)
								{
									_socketMessageQueue->push_back(SocketMessage);
								}
								else
								{
									HNSocketMessage::releaseMessage(SocketMessage);
								}
							}
							else
							{
								_socketMessageQueue->push_back(SocketMessage);
							}
							_delegate->onReadComplete();
						}
					}
					else
					{
						int queueSize = 0;
						do 
						{	
							HNSocketMessage* socketMessage = nullptr;
							{
								std::lock_guard < std::mutex > autoLock(_dataMutex);
								queueSize = _socketMessageQueue->size();
								if (queueSize > 0)
								{
									socketMessage = _socketMessageQueue->front();
									_socketMessageQueue->pop_front();
								}
							}
							if (nullptr != socketMessage)
							{
								HNSocketMessage::releaseMessage(socketMessage);
							}
						} while (queueSize > 0);

						HNSocketMessage* SocketMessage = HNSocketMessage::getMessage();
						SocketMessage->socketStatus = SocketStatus_DISCONNECT;
						{
							std::lock_guard <std::mutex> autoLock(_dataMutex);
							_socketMessageQueue->push_back(SocketMessage);
						}
						_delegate->onReadComplete();
						close();
						break;
					}
				}
				else
				{
					HNLOG_WARNING("%s >> not a complete data packet [messageSize = %lu, pMessageHead->uMessageSize = %lu]",
						_tag.c_str(), messageSize, pMessageHead->uMessageSize);
				}
				messageSize = (UINT)_socketDataCacheQueue->size();
				//HNLOG_INFO("%s >> messageSize ---> end = [%lu]", _tag.c_str(), messageSize);
			} while(messageSize >= pMessageHead->uMessageSize);
		}
	}

	void HNSocketThread::transform(std::function<void(HNSocketMessage* socketMessage)> func)
	{
		int queueSize = 0;
		do 
		{	
			HNSocketMessage* socketMessage = nullptr;
			{
				std::lock_guard < std::mutex > autoLock(_dataMutex);
				queueSize = _socketMessageQueue->size();
				if (queueSize > 0)
				{
					socketMessage = _socketMessageQueue->front();
					_socketMessageQueue->pop_front();
				}
			}
			if (nullptr != socketMessage)
			{
				func(socketMessage);
				HNSocketMessage::releaseMessage(socketMessage);
			}
		} while (queueSize > 0);
		Director::getInstance()->getScheduler()->pauseTarget(this);
	}

}
