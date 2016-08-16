#include "HNPlatformLogic.h"
#include "HNPlatformNotifyDelegate.h"
#include "HNSocket/HNSocketExport.h"
#include "HNData/HNUserInfoModule.h"
#include "HNData/HNRoomInfoModule.h"
#include "HNData/HNGamesInfoModule.h"
#include "HNRoom/HNRoomExport.h"
#include "HNCommon/HNConverCode.h"
#include "HNCommon/HNLog.h"
#include "HNGameCreator.h"

namespace HN
{
	static HNPlatformLogic* sHNPlatformLogic = nullptr;

	HNPlatformLogic* HNPlatformLogic::getInstance()
	{
		if (nullptr == sHNPlatformLogic)
		{
			sHNPlatformLogic = new HNPlatformLogic();
			sHNPlatformLogic->init();
		}
		return sHNPlatformLogic;
	}

	HNPlatformLogic::HNPlatformLogic(void) 
		: _logined(false)
		, _heartBeatFrequency(0)
		, _connected(false)
		, _serverPort(INVALID_VALUE)
	{
        memset(&loginResult, 0x00, sizeof(loginResult));
		_socketLogic = new HNSocketLogic(this, "platform");
		_notifyQueue = new HNUIDelegateQueue<IPlatformMessageDelegate*>();
		_addQueue    = new HNUIDelegateQueue<IPlatformMessageDelegate*>();
		_removeQueue = new HNUIDelegateQueue<IPlatformMessageDelegate*>();
	}

	HNPlatformLogic::~HNPlatformLogic(void)
	{
		HN_SAFE_DELETE(_removeQueue);
		HN_SAFE_DELETE(_addQueue);
		HN_SAFE_DELETE(_notifyQueue);
		HN_SAFE_DELETE(_socketLogic);
	}

	bool HNPlatformLogic::init() 
	{
		return true;
	}

	bool HNPlatformLogic::connect()
	{
		RoomInfoModule()->clear();
		GamesInfoModule()->clear();
        _serverAddress = getPlatformServerAddress();
        _serverPort    = getPlatformServerPort();
		return _socketLogic->openWithIp(_serverAddress.c_str(), _serverPort);
	}

	bool HNPlatformLogic::close()
	{
		_connected = false;
		return _socketLogic->close();
	}

	bool HNPlatformLogic::isConnect() const 
	{ 
		return _socketLogic->connected() && _connected; 
	} 

	bool HNPlatformLogic::isLogin() const
	{
		return _logined;
	}

	void HNPlatformLogic::sendData(UINT MainID, UINT AssistantID, void* object, INT objectSize)
	{	
		_socketLogic->send(MainID, AssistantID, HNSocketProtocolData::PlatformCheckCode, object, objectSize);
	}

	void HNPlatformLogic::sendData(UINT MainID, UINT AssistantID, void* object, INT objectSize, SEL_SocketMessage selector)
	{
		_socketLogic->send(MainID, AssistantID, HNSocketProtocolData::PlatformCheckCode, object, objectSize);
		_socketLogic->addEventSelector(MainID, AssistantID, selector);
	}

	void HNPlatformLogic::addEventSelector(UINT MainID, UINT AssistantID, SEL_SocketMessage selector)
	{
		_socketLogic->addEventSelector(MainID, AssistantID, selector);
	}

	void HNPlatformLogic::removeEventSelector(UINT MainID, UINT AssistantID)
	{
		_socketLogic->removeEventSelector(MainID, AssistantID);
	}

	void HNPlatformLogic::addObserver(IPlatformMessageDelegate* delegate)
	{
		_addQueue->addObserver(delegate);
		_removeQueue->removeObserver(delegate);
	}

	void HNPlatformLogic::removeObserver(IPlatformMessageDelegate* delegate)
	{
		_removeQueue->addObserver(delegate);
		_addQueue->removeObserver(delegate);
	}

	void HNPlatformLogic::heartBeat(float dt)
	{
		if (isConnect())
		{
			_socketLogic->heartBeat(MDM_CONNECT, ASS_NET_TEST_2, HNSocketProtocolData::PlatformCheckCode);
			_heartBeatFrequency++;
		}

		if (_heartBeatFrequency > 5)
		{
			_heartBeatFrequency = 0;
			close();
		}
	}

	void HNPlatformLogic::login(const std::string& name, const std::string& password, UINT uNameID)
	{
		MSG_GP_S_LogonByNameStruct data;
		data.uRoomVer = 4;
		data.iUserID = -1;
		data.gsqPs = 5471;
#if (HN_TAREGET_PLATFORM == HN_TARGET_PHONE_ONLY)
		data.uNameID  = uNameID;
#endif
		strcpy(data.TML_SN, "EQ4gG6vEUL06ajaGn4EAuXDa662vaeeqL6UdoOQatxuujAlnqovO6VndvXT4Tv0l4a28XGoDxqde4El6XUAXLXe66lg2o6gQN4tlOgeAoV6gulE2jTNneUulE");
		strcpy(data.szName, name.c_str());
		strcpy(data.szMD5Pass, password.c_str());			
		strcpy(data.szMathineCode, "000266-703028-103FA5-C05024-006BBE-007F50");
		strcpy(data.szCPUID, "612826255");
		strcpy(data.szHardID, "2222222");
		strcpy(data.szIDcardNo, "123456789");
		strcpy(data.szMathineCode, "123456789");			
		strcpy(data.szIDcardNo, "*");
		strcpy(data.szMobileVCode, "*");
		sendData(MDM_GP_LOGON, ASS_GP_LOGON_BY_NAME, &data, sizeof(data));		
	} 

	void HNPlatformLogic::regist(BYTE byFastRegister, UINT uNameID, const std::string& usn, const std::string& name, const std::string& password, const std::string &agency)
	{
		MSG_GP_S_Register data = { 0 };
		data.byFromPhone = 1;
		data.byFastRegister = byFastRegister;
#if (HN_TAREGET_PLATFORM == HN_TARGET_PHONE_ONLY)
		data.uNameID = uNameID;
#endif
		strcpy(data.szName, name.c_str());
		strcpy(data.szPswd, password.c_str());
		strcpy(data.szAgency, agency.c_str());
		strcpy(data.szHardID, usn.c_str());
		PlatformLogic()->sendData(MDM_GP_REGISTER, ASS_GP_REGISTER, &data, sizeof(data));
	}

	void HNPlatformLogic::getRoomList(UINT uKindID, UINT uNameID)
	{
		RoomInfoModule()->clear();
		MSG_GP_SR_GetRoomStruct GetRoomStruct;
		GetRoomStruct.uKindID = uKindID;
		GetRoomStruct.uNameID = uNameID;
		sendData(MDM_GP_LIST, ASS_GP_LIST_ROOM, &GetRoomStruct, sizeof(GetRoomStruct));
	}
	
	void HNPlatformLogic::onConnected(bool connect, emSocketStatus status)
	{
		dispatchMessage([status](IPlatformMessageDelegate* delegate) -> bool
		{
			delegate->I_P_M_Connect(SocketStatus_STCCESS == status);
			return false;
		});	
	}

	void HNPlatformLogic::onDisConnect()
	{
		_connected = false;
		dispatchMessage([](IPlatformMessageDelegate* delegate) -> bool
		{
			delegate->I_P_M_DisConnect();
			return false;
		});
	}

	void HNPlatformLogic::onSocketMessage(HNSocketMessage* socketMessage)
	{
		// send heartBeat to server
		if (MDM_CONNECT == socketMessage->messageHead.bMainID && ASS_NET_TEST_1 == socketMessage->messageHead.bAssistantID)
		{
			//_socketLogic->heartBeat(MDM_CONNECT, ASS_NET_TEST_1, HNSocketProtocolData::PlatformCheckCode);
			return;
		}
		
		if (MDM_CONNECT == socketMessage->messageHead.bMainID && ASS_NET_TEST_2 == socketMessage->messageHead.bAssistantID)
		{
			_heartBeatFrequency--;
			return;
		}
		
		switch (socketMessage->messageHead.bMainID)
		{
		
		// 平台连接
		case MDM_GP_CONNECT: 
			H_P_M_Connect(&socketMessage->messageHead, socketMessage->object, socketMessage->objectSize); 
			break;
		
		// 平台登录
		case MDM_GP_LOGON: 
			H_P_M_Login(&socketMessage->messageHead, socketMessage->object, socketMessage->objectSize); 
			break;
		
		// 列表（类型、名称、房间、房间密码、房间人数、游戏人数）
		case MDM_GP_LIST: 
			H_P_M_List(&socketMessage->messageHead, socketMessage->object, socketMessage->objectSize); 
			break;

		// 登陆玩家数量
		case MDM_GP_LOGONUSERS: 
			H_P_M_LoginUsers(&socketMessage->messageHead, socketMessage->object, socketMessage->objectSize); 
			break;

		// 注册
		case MDM_GP_REGISTER: 
			H_P_M_Regist(&socketMessage->messageHead, socketMessage->object, socketMessage->objectSize); 
			break;

		// 道具
		case  MDM_GP_PROP:
			H_P_M_Prop(&socketMessage->messageHead, socketMessage->object, socketMessage->objectSize);
			break;

		// 消息
		case MDM_GP_MESSAGE:
			H_P_M_Message(&socketMessage->messageHead, socketMessage->object, socketMessage->objectSize);
			break;
		default:
			break;
		}
	}

	void HNPlatformLogic::H_P_M_Connect(const NetMessageHead* messageHead, void* object, INT objectSize)
	{
		CHECK_SOCKET_DATA(MSG_S_ConnectSuccess, objectSize, "size is error.");

		MSG_S_ConnectSuccess* pData = (MSG_S_ConnectSuccess*)object;

		HNSocketProtocolData::PlatformCheckCode = ((UINT)pData->i64CheckCode - getSecrectKey()) / 23;
		_connected = true;

		dispatchMessage([&](IPlatformMessageDelegate* delegate) -> bool
		{
			delegate->I_P_M_Connect(_connected);
			return false;
		});	
	}


	void HNPlatformLogic::H_P_M_Login(const NetMessageHead* messageHead, void* object, INT objectSize)
	{
		if(ASS_GP_LOGON_SUCCESS == messageHead->bAssistantID || ASS_GP_LOGON_ERROR == messageHead->bAssistantID)
		{
			if (ERR_GP_LOGON_SUCCESS == messageHead->bHandleCode)
			{
				CHECK_SOCKET_DATA(MSG_GP_R_LogonResult, objectSize, "size is error.");

				MSG_GP_R_LogonResult* pData = (MSG_GP_R_LogonResult*)object;
				loginResult                 = *pData;
			}

			_logined = (ERR_GP_LOGON_SUCCESS == messageHead->bHandleCode);

			dispatchMessage([&](IPlatformMessageDelegate* delegate) -> bool
			{
				delegate->I_P_M_Login(_logined, messageHead->bHandleCode);
				return false;
			});
		}
	}

	void HNPlatformLogic::H_P_M_ListKind(const NetMessageHead* messageHead, void* object, INT objectSize)
	{
		ComKindInfo* pData = (ComKindInfo*)object;
		INT Count = objectSize / sizeof(ComKindInfo);
		Count = Count < 0 ? 0 : Count;
		while (Count-- > 0)
		{
			ComKindInfo* pInfo = pData++;
			GamesInfoModule()->addGameKind(pInfo);
		}
	}

	void HNPlatformLogic::H_P_M_ListName(const NetMessageHead* messageHead, void* object, INT objectSize)
	{
		ComNameInfo* pData = (ComNameInfo*)object;
		INT Count = objectSize / sizeof(ComNameInfo);
		Count = Count < 0 ? 0 : Count;

		while (Count-- > 0)
		{
			ComNameInfo* pInfo = pData++;
			GamesInfoModule()->addGameName(pInfo);
			GameCreator()->setGameKindId(pInfo->uNameID, pInfo->uKindID);
		}

		if (ERR_GP_LIST_FINISH == messageHead->bHandleCode)
		{
			dispatchMessage([](IPlatformMessageDelegate* delegate) -> bool
			{
				delegate->I_P_M_GameList();
				return false;
			});
		}
	}

	void HNPlatformLogic::H_P_M_ListRoom(const NetMessageHead* messageHead, void* object, INT objectSize)
	{
		if (objectSize >= sizeof(MSG_GP_SR_GetRoomStruct))
		{
			// 跳过房间ComRoomInfo数据前包含MSG_GP_SR_GetRoomStruct结构体，要跳过MSG_GP_SR_GetRoomStruct头。
			CHAR* pData   = ((CHAR*)object) + sizeof(MSG_GP_SR_GetRoomStruct);
			INT userCount = (objectSize - sizeof(MSG_GP_SR_GetRoomStruct)) / sizeof(ComRoomInfo);
			if (userCount > 0)
			{
				ComRoomInfo* pComRoomInfo = (ComRoomInfo*)(pData);
				INT i = 0;
				while (i++ < userCount)
				{
					RoomInfoModule()->addRoom(pComRoomInfo++);
				}
			}

			if (ERR_GP_LIST_FINISH == messageHead->bHandleCode)
			{
				dispatchMessage([](IPlatformMessageDelegate* delegate) -> bool
				{
					delegate->I_P_M_RoomList();
					return false;
				});
			}
		}
	}

	void HNPlatformLogic::H_P_M_ListRoomUserCount(const NetMessageHead* messageHead, void* object, INT objectSize)
	{
		DL_GP_RoomListPeoCountStruct* pData = (DL_GP_RoomListPeoCountStruct*)object;
		INT Count = objectSize / sizeof(DL_GP_RoomListPeoCountStruct);
		Count = Count < 0 ? 0 : Count;
		while (Count-- > 0)
		{
			DL_GP_RoomListPeoCountStruct* pInfo = pData++;
			ComRoomInfo* pRoom = RoomInfoModule()->getByRoomID(pInfo->uID);
			if (nullptr != pRoom)
			{
				pRoom->uPeopleCount = pInfo->uOnLineCount;
				pRoom->uVirtualUser = pInfo->uVirtualUser;
				dispatchMessage([&pInfo](IPlatformMessageDelegate* delegate) -> bool
				{
					delegate->I_P_M_RoomUserCount(pInfo->uID, pInfo->uOnLineCount, pInfo->uVirtualUser);
					return false;
				});
			}
		}
	}

	void HNPlatformLogic::H_P_M_ListGameUserCount(const NetMessageHead* messageHead, void* object, INT objectSize)
	{
		DL_GP_RoomListPeoCountStruct* pData = (DL_GP_RoomListPeoCountStruct*)object;
		INT Count = objectSize / sizeof(DL_GP_RoomListPeoCountStruct);
		Count = Count < 0 ? 0 : Count;
		while (Count-- > 0)
		{
			DL_GP_RoomListPeoCountStruct* pInfo = pData++;
			dispatchMessage([&pInfo](IPlatformMessageDelegate* delegate) -> bool
			{
				delegate->I_P_M_GameUserCount(pInfo);
				return false;
			});
		}
	}


	void HNPlatformLogic::H_P_M_ListRoomPassword(const NetMessageHead* messageHead, void* object, INT objectSize)
	{
		CHECK_SOCKET_DATA(MSG_GP_S_C_CheckRoomPasswd, objectSize, "size error.");
		
		MSG_GP_S_C_CheckRoomPasswd* pData = (MSG_GP_S_C_CheckRoomPasswd*)object;
		dispatchMessage([&pData](IPlatformMessageDelegate* delegate) -> bool
		{
			delegate->I_P_M_RoomPassword(pData);
			return false;
		});
	}

	void HNPlatformLogic::H_P_M_List(const NetMessageHead* messageHead, void* object, INT objectSize)
	{
		// 类型列表
		if(ASS_GP_LIST_KIND == messageHead->bAssistantID)
		{
			H_P_M_ListKind(messageHead, object, objectSize);
		}

		// 名称列表
		else if(ASS_GP_LIST_NAME == messageHead->bAssistantID)
		{
			H_P_M_ListName(messageHead, object, objectSize);
		}

		// 人数列表
		else if(ASS_GP_LIST_COUNT == messageHead->bAssistantID)
		{
			if (objectSize < sizeof(DL_GP_RoomListPeoCountStruct))
			{
				return;
			}

			// 房间人数
			if(0 == messageHead->bHandleCode)
			{
				H_P_M_ListRoomUserCount(messageHead, object, objectSize);
			}

			// 游戏人数
			else
			{
				H_P_M_ListGameUserCount(messageHead, object, objectSize);
			}
		}

		// 房间列表
		else if(ASS_GP_LIST_ROOM == messageHead->bAssistantID)
		{
			H_P_M_ListRoom(messageHead, object, objectSize);
		}

		// 房间密码
		else if(ASS_GP_ROOM_PASSWORD == messageHead->bAssistantID)
		{
			H_P_M_ListRoomPassword(messageHead, object, objectSize);
		}
	}	

	void HNPlatformLogic::H_P_M_LoginUsers(const NetMessageHead* messageHead, void* object, INT objectSize)
	{
		CCAssert(ASS_GP_LOGONUSERS_COUNT == messageHead->bAssistantID, "assistantId error");
		
		CHECK_SOCKET_DATA(ONLINEUSERSCOUNT, objectSize, "size error.");

		ONLINEUSERSCOUNT* pData = (ONLINEUSERSCOUNT*)object;

		RoomInfoModule()->setOnlineAllCount(pData->uiLogonPeopCount);

		dispatchMessage([&pData](IPlatformMessageDelegate* delegate) -> bool
		{
			delegate->I_P_M_LoginUsers(pData->uiLogonPeopCount);
			return false;
		});
	}


	void HNPlatformLogic::H_P_M_Regist(const NetMessageHead* messageHead, void* object, INT objectSize)
	{
		CHECK_SOCKET_DATA(MSG_GP_S_Register, objectSize, "size error.");
		MSG_GP_S_Register* pData = (MSG_GP_S_Register*)object;
		dispatchMessage([&pData, &messageHead](IPlatformMessageDelegate* delegate) -> bool
		{
			delegate->I_P_M_Regist(pData, messageHead->bHandleCode);
			return false;
		});
	}

	void HNPlatformLogic::H_P_M_Prop(const NetMessageHead* messageHead, void* object, INT objectSize)
	{
		CCAssert(ASS_PROP_GETUSERPROP == messageHead->bAssistantID, "size error.");
	}

	void HNPlatformLogic::H_P_M_Message(const NetMessageHead* messageHead, void* object, INT objectSize)
	{
		if(ASS_GP_NEWS_SYSMSG == messageHead->bAssistantID)
		{
			MSG_GR_RS_NormalTalk* pData = (MSG_GR_RS_NormalTalk*)object;
			dispatchMessage([&pData, &messageHead](IPlatformMessageDelegate* delegate) -> bool
			{
				delegate->I_P_M_NewsSys(pData);
				return false;
			});
		}
	}

	bool HNPlatformLogic::dispatchMessage(const PlatformNotifyFun& func)
	{
		//add new observer.
		auto iterA = _addQueue->begin();
		while(iterA != _addQueue->end())
		{
			auto next = iterA;
			next++;
			_notifyQueue->addObserver(iterA->second);
			_addQueue->removeObserver(iterA->second);
			iterA = next;
		}
		
		//remove not active observer.
		auto iterR = _removeQueue->begin();
		while(iterR != _removeQueue->end())
		{
			auto next = iterR;
			next++;
			_notifyQueue->removeObserver(iterR->second);
			_removeQueue->removeObserver(iterR->second);
			iterR = next;
		}

		//dispatch message
		for (auto iter = _notifyQueue->begin(); iter != _notifyQueue->end(); iter++)
		{
			bool removed = false;
			for(auto iterR = _removeQueue->begin(); iterR != _removeQueue->end(); iterR++)
			{
				if(iterR->second == iter->second)
				{
					removed = true;
					break;
				}
			}
			if(!removed)
			{
				func(iter->second);
			}			
		}
		return true;
	}
}
