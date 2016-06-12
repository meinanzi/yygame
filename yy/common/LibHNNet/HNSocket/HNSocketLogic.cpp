#include "HNSocketLogic.h"
#include "HNSocketThread.h"
#include "HNSocketMessageDelegate.h"
#include "HNSocketProtocolData.h"
#include "HNNetProtocol/HNProtocolExport.h"
#include "HNCommon/HNLog.h"
#include "HNCommon/HNCommonMarco.h"

namespace HN
{
	HNSocketLogic::HNSocketLogic(ISocketMessageDelegate* delegate, const std::string& tag) : _delegate(delegate)
	{
		_socketThread = new HNSocketThread(this);
		_socketThread->setTag(tag);
		_selectorQueue = new HNSocketMessageSelectorHandler();
		auto scheduler = Director::getInstance()->getScheduler();
		scheduler->schedule(schedule_selector(HNSocketLogic::socketDataDispatch), this, 0.0f, true);
	}

	HNSocketLogic::~HNSocketLogic(void)
	{
		auto scheduler = Director::getInstance()->getScheduler();
		scheduler->unschedule(schedule_selector(HNSocketLogic::socketDataDispatch), this);

		HN_SAFE_DELETE(_selectorQueue);

		HN_SAFE_DELETE(_socketThread);
	}

	bool HNSocketLogic::init() 
	{
		return true;
	}

	void HNSocketLogic::resetData()
	{
		_selectorQueue->clear();
	}

	bool HNSocketLogic::openWithIp(const CHAR* ip, INT port)
	{
		return _socketThread->openWithIp(ip, port);
	}

	bool HNSocketLogic::openWithHost(const CHAR* host, INT port)
	{
		return _socketThread->openWithHost(host, port);
	}

	bool HNSocketLogic::close()
	{		
		resetData();
		return _socketThread->close();
	}

	bool HNSocketLogic::connected() const
	{ 
		return _socketThread->connected(); 
	}

	INT HNSocketLogic::send(UINT MainID, UINT AssistantID, UINT CheckCode, void* object, INT objectSize)
	{	
		HNSocketProtocolData protocolData;
		protocolData.createPackage(MainID, AssistantID, CheckCode, object, objectSize);
		return _socketThread->send(protocolData.getPackage(), protocolData.getPackageSize());
	}

	void HNSocketLogic::addEventSelector(UINT MainID, UINT AssistantID, SEL_SocketMessage selector)
	{
		CCAssert(nullptr != selector, "selector is nullptr");
		if (nullptr != selector)
		{
			CHAR messageKey[16] = {0};
			sprintf(messageKey, SELECTER_KEY_FORMAT, MainID, AssistantID);
			_selectorQueue->addSelector(messageKey, selector);
		}
	}

	void HNSocketLogic::removeEventSelector(UINT MainID, UINT AssistantID)
	{
		CHAR messageKey[16] = {0};
		sprintf(messageKey, SELECTER_KEY_FORMAT, MainID, AssistantID);
		_selectorQueue->removeSelector(messageKey);
	}

	void HNSocketLogic::heartBeat(UINT MainID, UINT AssistantID, UINT CheckCode)
	{
		HNSocketProtocolData protocolData;
		protocolData.createPackage(MainID, AssistantID, CheckCode);
		int ret = _socketThread->send(protocolData.getPackage(), protocolData.getPackageSize());
		if (ret >= 0)
		{	
			//HNLOG_WARNING("%s >> heartBeat", _socketThread->getTag().c_str());
		}
	}

	void HNSocketLogic::onReadComplete()
	{
		Director::getInstance()->getScheduler()->resumeTarget(this);
	}

	void HNSocketLogic::socketDataDispatch(float dt)
	{
		_socketThread->transform([this](HNSocketMessage* socketMessage)
		{
			assert(nullptr != _delegate);
			switch (socketMessage->socketStatus)
			{
			case SocketStatus_UNKNOW:
				break;
			case SocketStatus_CONNECT:
				break;
			case SocketStatus_OUTTIME:
				_delegate->onConnected(false, SocketStatus_OUTTIME);
				break;
			case SocketStatus_STCCESS:
				{

				}
				break;
			case SocketStatus_RECV:
				{
					bool ret = _selectorQueue->executeSelector(socketMessage->strKey, socketMessage);
					if (!ret)
					{
						_delegate->onSocketMessage(socketMessage);
					}
				}
				break;
			case SocketStatus_DISCONNECT:
				_selectorQueue->clear();
				_delegate->onDisConnect();
				break;
			case SocketStatus_ERROR:
				_selectorQueue->clear();
				_delegate->onConnected(false, SocketStatus_ERROR);
				break;
			default:
				break;
			}
		});
		Director::getInstance()->getScheduler()->pauseTarget(this);
	}
}
