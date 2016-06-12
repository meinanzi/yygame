#include "HNRoomLogic.h"
#include "HNRoomMessageDelegate.h"
#include "HNData/HNRoomInfoModule.h"
#include "HNData/HNUserInfoModule.h"
#include "HNPlatform/HNPlatformExport.h"
#include "HNSocket/HNSocketProtocolData.h"
#include "HNSocket/HNSocketLogic.h"
#include "HNNetProtocol/HNProtocolExport.h"
#include "HNGame/HNGameMessageDelegate.h"
#include "HNCommon/HNConverCode.h"

namespace HN
{
	static HNRoomLogic* sHNRoomLogic = nullptr;

	HNRoomLogic* HNRoomLogic::getInstance()
	{
		if (nullptr == sHNRoomLogic)
		{
			sHNRoomLogic = new HNRoomLogic();
			sHNRoomLogic->init();
		}
		return sHNRoomLogic;
	}
	
	HNRoomLogic::HNRoomLogic(void)
		: _gameRoomRule(0)
		, _serverPort(-1)
		, _connected(false)
		, _logined(false)
		, _selectedRoom(nullptr)
	{
        memset(&loginResult, 0x00, sizeof(loginResult));
		_socketLogic = new HNSocketLogic(this, "game");
		
		_gameNotifyQueue = new HNUIDelegateQueue<IGameMessageDelegate*>();
		_addGameNotifyQueue = new HNUIDelegateQueue<IGameMessageDelegate*>();
		_removeGameNotifyQueue = new HNUIDelegateQueue<IGameMessageDelegate*>();

		_roomNotifyQueue = new HNUIDelegateQueue<IRoomMessageDelegate*>();
		_addRoomNotifyQueue = new HNUIDelegateQueue<IRoomMessageDelegate*>();
		_removeRoomNotifyQueue = new HNUIDelegateQueue<IRoomMessageDelegate*>();
	}

	HNRoomLogic::~HNRoomLogic(void)
	{
		HN_SAFE_DELETE(_addGameNotifyQueue);
		HN_SAFE_DELETE(_gameNotifyQueue);
		HN_SAFE_DELETE(_removeGameNotifyQueue);
		HN_SAFE_DELETE(_roomNotifyQueue);
		HN_SAFE_DELETE(_addRoomNotifyQueue);
		HN_SAFE_DELETE(_removeRoomNotifyQueue);
		HN_SAFE_DELETE(_socketLogic);
	}

	bool HNRoomLogic::init() 
	{
		return true;
	}

	bool HNRoomLogic::connect(const CHAR* ipOrHost, INT port)
	{
		std::string ip(ipOrHost);
		if(!HNSocket::isValidIP(ip))
		{
			ip = HNSocket::getIpAddress(ip.c_str()).at(0);
		}
		_serverAddress = ip;
		_serverPort    = port;
		return _socketLogic->openWithIp(_serverAddress.c_str(), _serverPort);
	}

	bool HNRoomLogic::close()
	{
		_connected =  false;
		return _socketLogic->close();
	}

	bool HNRoomLogic::isConnect() const 
	{ 
		return _connected && _socketLogic->connected(); 
	} 

	bool HNRoomLogic::isLogin() const
	{
		return _logined;
	}

	INT HNRoomLogic::sendData(UINT MainID, UINT AssistantID, void* object, INT objectSize)
	{	
		return _socketLogic->send(MainID, AssistantID, HNSocketProtocolData::GameCheckCode, object, objectSize);
	}

	void HNRoomLogic::sendData(UINT MainID, UINT AssistantID, void* object, INT objectSize, SEL_SocketMessage selector)
	{
		_socketLogic->send(MainID, AssistantID, HNSocketProtocolData::GameCheckCode, object, objectSize);
		_socketLogic->addEventSelector(MainID, AssistantID, selector);
	}

	void HNRoomLogic::addEventSelector(UINT MainID, UINT AssistantID, SEL_SocketMessage selector)
	{
		_socketLogic->addEventSelector(MainID, AssistantID, selector);
	}

	void HNRoomLogic::removeEventSelector(UINT MainID, UINT AssistantID)
	{
		_socketLogic->removeEventSelector(MainID, AssistantID);
	}

	void HNRoomLogic::addObserver(IGameMessageDelegate* delegate)
	{
		_addGameNotifyQueue->addObserver(delegate);
		_removeGameNotifyQueue->removeObserver(delegate);
	}

	void HNRoomLogic::removeObserver(IGameMessageDelegate* delegate)
	{
		_removeGameNotifyQueue->addObserver(delegate);
		_addGameNotifyQueue->removeObserver(delegate);
	}

	void HNRoomLogic::addRoomObserver(IRoomMessageDelegate* delegate)
	{
		_addRoomNotifyQueue->addObserver(delegate);
		_removeRoomNotifyQueue->removeObserver(delegate);
	}

	void HNRoomLogic::removeRoomObserver(IRoomMessageDelegate* delegate)
	{
		_removeRoomNotifyQueue->addObserver(delegate);
		_addRoomNotifyQueue->removeObserver(delegate);
	}

	void HNRoomLogic::login(UINT uGameID)
	{
		MSG_GR_S_RoomLogon data = {0};
#if (HN_TAREGET_PLATFORM == HN_TARGET_PHONE_ONLY)
		data.byFromPhone = 1;
#endif
		data.uNameID = uGameID;
		data.dwUserID = PlatformLogic()->loginResult.dwUserID;
		strcpy(data.szMD5Pass, PlatformLogic()->loginResult.szMD5Pass);
		sendData(MDM_GR_LOGON, ASS_GR_LOGON_BY_ID, &data, sizeof(data));
	}

	void HNRoomLogic::onConnected(bool connect, emSocketStatus status)
	{
		_connected = connect;
		// 分发事件
		dispatchFrameMessage([status](IRoomMessageDelegate* delegate) -> bool
		{
			delegate->I_R_M_Connect(SocketStatus_STCCESS == status);
			return false;
		});
	}

	void HNRoomLogic::onDisConnect()
	{
		_logined = false;

		_connected = false;

		// 分发消息到房间
		dispatchFrameMessage([](IRoomMessageDelegate* delegate) -> bool
		{
			delegate->I_R_M_DisConnect();
			return false;
		});

		// 分发消息到游戏
		dispatchGameMessage([](IGameMessageDelegate* delegate) -> bool
		{
			delegate->I_R_M_DisConnect();
			return false;
		});
	}

	void HNRoomLogic::onSocketMessage(HNSocketMessage* socketMessage)
	{
		// 心跳
		if (MDM_CONNECT == socketMessage->messageHead.bMainID && ASS_NET_TEST_1 == socketMessage->messageHead.bAssistantID)
		{
			//_socketLogic->heartBeat(MDM_CONNECT, ASS_NET_TEST_1, HNSocketProtocolData::GameCheckCode);
			return;
		}

		switch (socketMessage->messageHead.bMainID)
		{
		// 连接
		case MDM_GR_CONNECT: 
			H_R_M_Connect(&socketMessage->messageHead, socketMessage->object, socketMessage->objectSize);
			break;

		// 登录消息
		case MDM_GR_LOGON: 
			H_R_M_Login(&socketMessage->messageHead, socketMessage->object, socketMessage->objectSize);
			break;

		// 用户列表
		case MDM_GR_USER_LIST:
			H_R_M_UserList(&socketMessage->messageHead, socketMessage->object, socketMessage->objectSize);
			break;

		// 用户动作 
		case MDM_GR_USER_ACTION:
			H_R_M_UserAction(&socketMessage->messageHead, socketMessage->object, socketMessage->objectSize);
			break;

		// 房间消息
		case MDM_GR_ROOM:
			H_R_M_Room(&socketMessage->messageHead, socketMessage->object, socketMessage->objectSize);
			break;

		// 框架消息
		case MDM_GM_GAME_FRAME:	
			H_R_M_GameFrame(&socketMessage->messageHead, socketMessage->object, socketMessage->objectSize);
			break;

		// 游戏消息
		case MDM_GM_GAME_NOTIFY:	
			H_R_M_GameNotify(&socketMessage->messageHead, socketMessage->object, socketMessage->objectSize);
			break;

		// 封桌
		case MDM_GR_MANAGE:
			H_R_M_Manage(&socketMessage->messageHead, socketMessage->object, socketMessage->objectSize);
			break;
		default:
			break;
		}
	}

	void HNRoomLogic::H_R_M_GameNotify(const NetMessageHead* messageHead, void* object, INT objectSize)
	{
		dispatchGameMessage([&messageHead, &object, & objectSize](IGameMessageDelegate* delegate) -> bool 
		{
			delegate->onGameMessage((NetMessageHead*)messageHead, object, objectSize);
			return true;
		});
	}

	void HNRoomLogic::H_R_M_GameFrame(const NetMessageHead* messageHead, void* object, INT objectSize)
	{
		// 游戏信息
		if(ASS_GM_GAME_INFO == messageHead->bAssistantID)
		{
			H_R_M_GameInfo(messageHead, object, objectSize);
		}

		// 游戏状态
		else if(ASS_GM_GAME_STATION == messageHead->bAssistantID)
		{
			H_R_M_GameStation(messageHead, object, objectSize);
		}

		// 普通聊天
		else if(ASS_GM_NORMAL_TALK == messageHead->bAssistantID)
		{
			H_R_M_NormalTalk(messageHead, object, objectSize);
		}
	}


	void HNRoomLogic::H_R_M_GameInfo(const NetMessageHead* messageHead, void* object, INT objectSize)
	{
		MSG_GM_S_GameInfo* pGameInfo = (MSG_GM_S_GameInfo*)object;

		// 显示信息
		if (messageHead->uMessageSize > (sizeof(MSG_GM_S_GameInfo) - sizeof(pGameInfo->szMessage)))
		{
			dispatchGameMessage([&pGameInfo](IGameMessageDelegate* delegate) -> bool 
			{
				delegate->I_R_M_GameInfo(pGameInfo);
				return true;
			});
		}
	}

	void HNRoomLogic::H_R_M_GameStation(const NetMessageHead* messageHead, void* object, INT objectSize)
	{
		dispatchGameMessage([&object, objectSize](IGameMessageDelegate* delegate) -> bool 
		{
			delegate->I_R_M_GameStation(object, objectSize);
			return true;
		});
	}

	void HNRoomLogic::H_R_M_NormalTalk(const NetMessageHead* messageHead, void* object, INT objectSize)
	{
		dispatchGameMessage([&object, objectSize](IGameMessageDelegate* delegate) -> bool 
		{
			(dynamic_cast<IGameChartMessageDelegate*>(delegate))->I_R_M_NormalTalk(object, objectSize);
			return true;
		});
	}

	void HNRoomLogic::H_R_M_Room(const NetMessageHead* messageHead, void* object, INT objectSize)
	{
		// 游戏开始
		if(ASS_GR_GAME_BEGIN == messageHead->bAssistantID)
		{
			H_R_M_GameBegin(messageHead, object, objectSize);
		}

		// 游戏结算
		else if(ASS_GR_USER_POINT == messageHead->bAssistantID)
		{
			H_R_M_UserPoint(messageHead, object, objectSize);
		}

		// 游戏结束
		else if(ASS_GR_GAME_END == messageHead->bAssistantID)
		{
			H_R_M_GameEnd(messageHead, object, objectSize);
		}

		// 用户同意
		else if(ASS_GR_USER_AGREE == messageHead->bAssistantID)
		{
			H_R_M_UserAgree(messageHead, object, objectSize);
		}

		// 及时更新金币和积分
		else if(ASS_GR_INSTANT_UPDATE == messageHead->bAssistantID)
		{
			H_R_M_InstantUpdate(messageHead, object, objectSize);
		}

		// 用户比赛信息
		else if(ASS_GR_USER_CONTEST == messageHead->bAssistantID)
		{
			H_R_M_UserContest(messageHead, object, objectSize);
		}

		// 比赛信息广播
		else if (ASS_GR_CONTEST_APPLYINFO == messageHead->bAssistantID)
		{
			H_R_M_ContestNotic(messageHead, object, objectSize);
		}

		// 比赛结束
		else if(ASS_GR_CONTEST_GAMEOVER == messageHead->bAssistantID)
		{
			H_R_M_ContestOver(messageHead, object, objectSize);
		}

		// 用户被淘汰
		else if(ASS_GR_CONTEST_KICK == messageHead->bAssistantID)
		{
			H_R_M_ContestKick(messageHead, object, objectSize);
		}

		// 比赛等待结束
		else if(ASS_GR_CONTEST_WAIT_GAMEOVER == messageHead->bAssistantID)
		{
			H_R_M_ContestWaitOver(messageHead, object, objectSize);
		}

		// 比赛初始化
		else if(ASS_GR_INIT_CONTEST == messageHead->bAssistantID)
		{
			H_R_M_ContestInit(messageHead, object, objectSize);
		}

		// 比赛报名人数
		/*else if(ASS_GR_CONTEST_PEOPLE == messageHead->bAssistantID)
		{
			H_R_M_ContestPeople(messageHead, object, objectSize);
		}*/

		// 个人参赛记录
		/*else if(ASS_GR_CONTEST_RECORD == messageHead->bAssistantID)
		{
			H_R_M_ContestRecord(messageHead, object, objectSize);
		}*/

		// 比赛奖励
		else if(ASS_GR_CONTEST_AWARDS == messageHead->bAssistantID)
		{
			H_R_M_ContestAward(messageHead, object, objectSize);
		}
	}

	void HNRoomLogic::H_R_M_GameBegin(const NetMessageHead* messageHead, void* object, INT objectSize)
	{
		BYTE deskNo = (BYTE)messageHead->bHandleCode;
		// 更新用户状态
		UserInfoModule()->transform(deskNo, [](UserInfoStruct* user, INT index) 
		{
			user->bUserState = USER_PLAY_GAME;
		});

		dispatchGameMessage([&messageHead](IGameMessageDelegate* delegate) -> bool 
		{
			delegate->I_R_M_GameBegin((BYTE)messageHead->bHandleCode);
			return false;
		});
	}

	void HNRoomLogic::H_R_M_UserPoint(const NetMessageHead* messageHead, void* object, INT objectSize)
	{
		// 判断是否是入库类型
		auto isNormal = []()->bool
		{
			UINT flag = (RoomLogic()->getRoomRule() & GRR_EXPERCISE_ROOM) || (RoomLogic()->getRoomRule() & GRR_CONTEST);
			bool ret = !(flag != 0);
			return ret;
		};

		if (10 == messageHead->bHandleCode)			// 同步金币
		{
			// 效验数据
			CHECK_SOCKET_DATA(MSG_GR_S_RefalshMoney, objectSize, "MSG_GR_S_RefalshMoney size is error.");

			MSG_GR_S_RefalshMoney * pReflashMoney = (MSG_GR_S_RefalshMoney*)object;

			// 处理数据
			UserInfoStruct * pUserItem = UserInfoModule()->findUser(pReflashMoney->dwUserID);
			if (pUserItem== nullptr) return;

			pUserItem->i64Money = pReflashMoney->i64Money;

			// 更新自己数据
			if (pReflashMoney->dwUserID == PlatformLogic()->loginResult.dwUserID)
			{
				RoomLogic()->loginResult.pUserInfoStruct = *pUserItem;
				if (isNormal())
				{
					PlatformLogic()->loginResult.i64Money += pReflashMoney->i64Money;
				}				
			}
		}
		else if (11 == messageHead->bHandleCode)	// 同步经验值
		{
			// 效验数据
			CHECK_SOCKET_DATA(MSG_GR_S_RefalshMoney, objectSize, "MSG_GR_S_RefalshMoney size is error.");

			MSG_GR_S_RefalshMoney * pReflashMoney = (MSG_GR_S_RefalshMoney*)object;

			// 处理数据
			UserInfoStruct * pUserItem = UserInfoModule()->findUser(pReflashMoney->dwUserID);
			if (pUserItem== nullptr) return;

			pUserItem->dwPoint = pReflashMoney->i64Money;

			// 更新自己数据
			if (pUserItem->dwUserID == PlatformLogic()->loginResult.dwUserID)
			{
				RoomLogic()->loginResult.pUserInfoStruct = *pUserItem;
			}

		}
		else if (0 == messageHead->bHandleCode)		// 同步经验值
		{
			// 效验数据
			CHECK_SOCKET_DATA(MSG_GR_R_UserPoint, objectSize, "MSG_GR_R_UserPoint size is error.");

			MSG_GR_R_UserPoint * pUserPoint = (MSG_GR_R_UserPoint*)object;

			UserInfoStruct * pUserItem = UserInfoModule()->findUser(pUserPoint->dwUserID);
			if (pUserItem== nullptr) return;

			// 更新用户信息
			pUserItem->dwPoint += pUserPoint->dwPoint;							//用户经验值
			pUserItem->i64Money += pUserPoint->dwMoney;							//用户金币
			//pUserItem->dwSend += pUserPoint->dwSend;							//赠送
			pUserItem->uWinCount += pUserPoint->bWinCount;						//胜局
			pUserItem->uLostCount += pUserPoint->bLostCount;					//输局
			pUserItem->uMidCount += pUserPoint->bMidCount;						//平局
			pUserItem->uCutCount += pUserPoint->bCutCount;						//逃局

			//pUserItem->strAutoSendMoney += pUserPoint->strAutoSendMoney;      //添加自动赠送 2013-01-29 wangzhitu

			// 更新自己数据
			if (pUserItem->dwUserID == PlatformLogic()->loginResult.dwUserID)
			{
				RoomLogic()->loginResult.pUserInfoStruct = *pUserItem;
				if (isNormal())
				{
					PlatformLogic()->loginResult.i64Money += pUserPoint->dwMoney;
				}				
			}
		}
		dispatchFrameMessage([&object, objectSize](IRoomMessageDelegate* delegate) -> bool 
		{
			delegate->I_R_M_GamePoint(object, objectSize);
			return false;
		});

		dispatchGameMessage([&object, objectSize](IGameMessageDelegate* delegate) -> bool 
		{
			delegate->I_R_M_GamePoint(object, objectSize);
			return false;
		});
	}

	void HNRoomLogic::H_R_M_GameEnd(const NetMessageHead* messageHead, void* object, INT objectSize)
	{
		BYTE deskIndex = (BYTE)messageHead->bHandleCode;

		dispatchGameMessage([deskIndex](IGameMessageDelegate* delegate) -> bool 
		{
			delegate->I_R_M_GameEnd(deskIndex);
			return false;
		});

		if ((_gameRoomRule & GRR_CONTEST) || (_gameRoomRule & GRR_QUEUE_GAME))
		{
			std::vector<UserInfoStruct*> users;
			UserInfoModule()->findDeskUsers(deskIndex, users);
			std::for_each(users.begin(), users.end(), [](UserInfoStruct* user)
			{
				user->bDeskStation = INVALID_DESKSTATION;
				user->bDeskNO = INVALID_DESKNO;
				user->bUserState = USER_LOOK_STATE;
			});
		}
	}


	void HNRoomLogic::H_R_M_UserAgree(const NetMessageHead* messageHead, void* object, INT objectSize)
	{
		CHECK_SOCKET_DATA(MSG_GR_R_UserAgree, objectSize, "MSG_GR_R_UserAgree size is error.");

		MSG_GR_R_UserAgree * pUserAgree = (MSG_GR_R_UserAgree*)object;

		UserInfoStruct * pUserItem = UserInfoModule()->findUser(pUserAgree->bDeskNO, pUserAgree->bDeskStation);
		if (nullptr != pUserItem)
		{
			pUserItem->bUserState = USER_ARGEE;
		}

		dispatchGameMessage([&pUserAgree](IGameMessageDelegate* delegate) -> bool 
		{
			delegate->I_R_M_UserAgree(pUserAgree);
			return false;
		});
	}


	void HNRoomLogic::H_R_M_InstantUpdate(const NetMessageHead* messageHead, void* object, INT objectSize)
	{
		CHECK_SOCKET_DATA(MSG_GR_R_InstantUpdate, objectSize, "MSG_GR_R_InstantUpdate size is error.");

		MSG_GR_R_InstantUpdate * pInstantUpdate = (MSG_GR_R_InstantUpdate*)object;

		// 更新数据
		UserInfoStruct * pUserItem = UserInfoModule()->findUser(pInstantUpdate->dwUserID);
		if (nullptr != pUserItem)
		{
			pUserItem->i64Money += pInstantUpdate->dwMoney;
			pUserItem->dwPoint += pInstantUpdate->dwPoint;
		}

		// 更新自己数据
		if (pUserItem->dwUserID == PlatformLogic()->loginResult.dwUserID)
		{
			RoomLogic()->loginResult.pUserInfoStruct = *pUserItem;
			PlatformLogic()->loginResult.i64Money = pInstantUpdate->dwMoney;
		}
	}


	void HNRoomLogic::H_R_M_UserContest(const NetMessageHead* messageHead, void* object, INT objectSize)
	{
		CHECK_SOCKET_DATA(MSG_GR_ContestChange, objectSize, "MSG_GR_ContestChange size of error!");
		MSG_GR_ContestChange * contestChange = (MSG_GR_ContestChange*)object; 
			
		int									dwUserID;							// 用户 ID	
		int									iContestCount;						// 比赛局数
		LLONG							    i64ContestScore;					// 比赛分数
		int									iRankNum;							// 比赛排名
		int									iRemainPeople;						// 比赛中还剩多少人
		char								szAward[30];						// 比赛获取的奖励

		UserInfoStruct* user = UserInfoModule()->findUser(contestChange->dwUserID);
		if (nullptr != user)
		{
			// 排行名次
			user->iRankNum        = contestChange->iRankNum;					

			// 比赛中还剩下的人数
			user->iRemainPeople   = contestChange->iRemainPeople;			

			// 比赛分数
			user->i64ContestScore = contestChange->i64ContestScore;	

			// 比赛局数
			user->iContestCount   = contestChange->iContestCount + 1;
		}

		if (contestChange->dwUserID != PlatformLogic()->loginResult.dwUserID)		
		{
			return;
		}

		dispatchGameMessage([contestChange](IGameMessageDelegate* delegate) -> bool
		{
			delegate->I_R_M_UserContest(contestChange);
			return false;
		});
	}

	void HNRoomLogic::H_R_M_ContestNotic(const NetMessageHead* messageHead, void* object, INT objectSize)
	{
		CHECK_SOCKET_DATA(MSG_GR_I_ContestInfo, objectSize, "MSG_GR_ContestChange size of error!");
		MSG_GR_I_ContestInfo * contestInfo = (MSG_GR_I_ContestInfo*)object; //NET_ROOM_CONTEST_CHANGE_RESULT
			
		dispatchFrameMessage([&](IRoomMessageDelegate* delegate) -> bool 
		{
			delegate->I_R_M_ContestNotic(contestInfo);
			return false;
		});

		dispatchGameMessage([&](IGameMessageDelegate* delegate) -> bool 
		{
			delegate->I_R_M_ContestNotic(contestInfo);
			return false;
		});	
	}

	void HNRoomLogic::H_R_M_ContestOver(const NetMessageHead* messageHead, void* object, INT objectSize)
	{
		CHECK_SOCKET_DATA(MSG_GR_ContestAward, objectSize, "MSG_GR_ContestAward size of error!");
		MSG_GR_ContestAward* contestAward = (MSG_GR_ContestAward*)object;

		dispatchFrameMessage([contestAward](IRoomMessageDelegate* delegate) -> bool 
		{
			delegate->I_R_M_ContestOver(contestAward);
			return false;
		});	

		dispatchGameMessage([contestAward](IGameMessageDelegate* delegate) -> bool 
		{
			delegate->I_R_M_ContestOver(contestAward);
			return false;
		});	
	}

	void HNRoomLogic::H_R_M_ContestKick(const NetMessageHead* messageHead, void* object, INT objectSize)
	{
		dispatchGameMessage([](IGameMessageDelegate* delegate) -> bool 
		{
			delegate->I_R_M_ContestKick();
			return false;
		});	
	}


	void HNRoomLogic::H_R_M_ContestWaitOver(const NetMessageHead* messageHead, void* object, INT objectSize)
	{
		dispatchGameMessage([](IGameMessageDelegate* delegate) -> bool 
		{
			delegate->I_R_M_ContestWaitOver();
			return false;
		});	
	}

	void HNRoomLogic::H_R_M_ContestInit(const NetMessageHead* messageHead, void* object, INT objectSize)
	{
		CHECK_SOCKET_DATA(MSG_GR_ContestChange, objectSize, "MSG_GR_ContestChange size of error!");

		MSG_GR_ContestChange* contestChange = (MSG_GR_ContestChange*)object;

		UserInfoStruct* user = UserInfoModule()->findUser(contestChange->dwUserID);
		if (nullptr != user)
		{
			user->iRankNum = contestChange->iRankNum;						// 排行名次
			user->iRemainPeople = contestChange->iRemainPeople;		// 比赛中还剩下的人数
			user->iContestCount = contestChange->iContestCount + 1;	// 比赛局数
		}	
	}

	void HNRoomLogic::H_R_M_ContestPeople(const NetMessageHead* messageHead, void* object, INT objectSize)
	{
		CHECK_SOCKET_DATA(NET_ROOM_CONTEST_PEOPLE_RESULT, objectSize, "NET_ROOM_CONTEST_PEOPLE_RESULT size of error!");
		NET_ROOM_CONTEST_PEOPLE_RESULT* contestPeople = (NET_ROOM_CONTEST_PEOPLE_RESULT*)object;

		// 分发房间数据
		dispatchFrameMessage([contestPeople](IRoomMessageDelegate* delegate) -> bool 
		{
			delegate->I_R_M_ContestPeople(contestPeople);
			return false;
		});
	}


	void HNRoomLogic::H_R_M_ContestRecord(const NetMessageHead* messageHead, void* object, INT objectSize)
	{
		CHECK_SOCKET_DATA(NET_ROOM_CONTEST_RECORD_RESULT, objectSize, "NET_ROOM_CONTEST_RECORD_RESULT size of error!");
		NET_ROOM_CONTEST_RECORD_RESULT* contestRecord = (NET_ROOM_CONTEST_RECORD_RESULT*)object;

		// 分发房间数据
		dispatchFrameMessage([contestRecord](IRoomMessageDelegate* delegate) -> bool 
		{
			delegate->I_R_M_ContestRecord(contestRecord);
			return false;
		});
	}


	void HNRoomLogic::H_R_M_ContestAward(const NetMessageHead* messageHead, void* object, INT objectSize)
	{
		UINT count = objectSize / sizeof(NET_ROOM_CONTEST_AWARD_RESULT);
		if (count > 0)
		{
			NET_ROOM_CONTEST_AWARD_RESULT* contestRecord = (NET_ROOM_CONTEST_AWARD_RESULT*)object;

			std::vector<NET_ROOM_CONTEST_AWARD_RESULT*> results(count);
			while (count > 0)
			{
				NET_ROOM_CONTEST_AWARD_RESULT* tmp = contestRecord++;
				results[count - 1] = tmp;
				count--;
			}

			// 分发房间数据
			dispatchFrameMessage([&results](IRoomMessageDelegate* delegate) -> bool 
			{
				delegate->I_R_M_ContestAwards(&results);
				return false;
			});
		}
	}

	
	void HNRoomLogic::H_R_M_Manage(const NetMessageHead* messageHead, void* object, INT objectSize)
	{
		//封桌
		if(messageHead->bHandleCode == 1)
		{
			BYTE deskNo = messageHead->bAssistantID;
			RoomLogic()->deskStation.bVirtualDesk[deskNo / 8] |= (1 << (deskNo % 8));

			dispatchFrameMessage([&messageHead](IRoomMessageDelegate* delegate) -> bool 
			{
				delegate->I_R_M_LockDesk(messageHead->bAssistantID);
				return true;
			});
		}

		// 解封桌子
		else
		{
			BYTE deskNo = messageHead->bAssistantID;
			RoomLogic()->deskStation.bVirtualDesk[deskNo / 8] &= (~(1 << (deskNo % 8)));

			dispatchFrameMessage([&messageHead](IRoomMessageDelegate* delegate) -> bool 
			{
				delegate->I_R_M_UnLockDesk(messageHead->bAssistantID);
				return true;
			});
		}
	}

	void HNRoomLogic::H_R_M_Connect(const NetMessageHead* messageHead, void* object, INT objectSize)
	{
		CCAssert(ASS_GR_CONNECT_SUCCESS == messageHead->bAssistantID, "assistId error.");

		CHECK_SOCKET_DATA(MSG_S_ConnectSuccess, objectSize, "size error.");

		int n = sizeof(MSG_S_ConnectSuccess);
		MSG_S_ConnectSuccess* pData = (MSG_S_ConnectSuccess*)object;

		_connected = true;
		HNSocketProtocolData::GameCheckCode = (UINT)(pData->i64CheckCode - getSecrectKey()) / 23;

		// 分发事件
		dispatchFrameMessage([](IRoomMessageDelegate* delegate) -> bool
		{
			delegate->I_R_M_Connect(true);
			return false;
		});
	}	

	void HNRoomLogic::H_R_M_Login(const NetMessageHead* messageHead, void* object, INT objectSize)
	{
		char chs[20] = {0};
		std::string message;

		// 异地登录
		if(ASS_GP_LOGON_ALLO_PART == messageHead->bAssistantID)
		{
			message = GBKToUtf8("异地登录");

			dispatchFrameMessage([&messageHead, &message](IRoomMessageDelegate* delegate) -> bool
			{
				delegate->I_R_M_Login(false, messageHead->bHandleCode, message);
				return false;
			});
		}

		// 设备锁定
		else if(ASS_GP_LOGON_LOCK_VALID == messageHead->bAssistantID)
		{
			message = GBKToUtf8("设备锁定");

			dispatchFrameMessage([&messageHead, &message](IRoomMessageDelegate* delegate) -> bool
			{
				delegate->I_R_M_Login(false, messageHead->bHandleCode, message);
				return false;
			});
		}

		// 登录成功
		else if(ASS_GR_LOGON_SUCCESS == messageHead->bAssistantID)
		{
			message = GBKToUtf8("房间登录成功");

			CHECK_SOCKET_DATA(MSG_GR_R_LogonResult, objectSize, "size error.");			

			MSG_GR_R_LogonResult * pData = (MSG_GR_R_LogonResult*)object;

			loginResult = *pData;
			_logined    = (ERR_GP_LOGON_SUCCESS == messageHead->bHandleCode);

			dispatchFrameMessage([&messageHead, &message](IRoomMessageDelegate* delegate) -> bool
			{
				delegate->I_R_M_Login(true, messageHead->bHandleCode, message);
				return false;
			});
		}

		// 登录错误
		else if(ASS_GR_LOGON_ERROR == messageHead->bAssistantID)
		{
			switch (messageHead->bHandleCode)
			{
			case ERR_GR_USER_PASS_ERROR:
				message = GBKToUtf8("用户密码错误");
				break;
			case ERR_GR_CONTEST_TIMEOUT:
				message = GBKToUtf8("连接超时");
				break;
			case ERR_GR_IN_OTHER_ROOM:
				message = GBKToUtf8("正在其他房间");
				break;
			case ERR_GR_ACCOUNTS_IN_USE:
				message = GBKToUtf8("帐号正在使用");
				break;
			case ERR_GR_STOP_LOGON:
				message = GBKToUtf8("暂停登陆服务");
				break;
			case ERR_GR_PEOPLE_FULL:
				message = GBKToUtf8("房间人数已经满");
				break;
			default:
				message = GBKToUtf8("未知登录错误");
				break;
			}

			dispatchFrameMessage([&messageHead, &message](IRoomMessageDelegate* delegate) -> bool
			{
				delegate->I_R_M_Login(false, messageHead->bHandleCode, message);
				return false;
			});
		}

		// 登录完成
		else if(ASS_GR_SEND_FINISH == messageHead->bAssistantID)
		{
			dispatchFrameMessage([](IRoomMessageDelegate* delegate) -> bool
			{
				delegate->I_R_M_LoginFinish();
				return false;
			});	
		}
	}	

	void HNRoomLogic::H_R_M_UserList(const NetMessageHead* messageHead, void* object, INT objectSize)
	{
		if(ASS_GR_ONLINE_USER == messageHead->bAssistantID || ASS_GR_NETCUT_USER == messageHead->bAssistantID)
		{
			UINT userCount = objectSize / sizeof(UserInfoStruct);
			UserInfoStruct * pUserInfo = (UserInfoStruct *)object;
			while (userCount-- > 0)
			{
				UserInfoModule()->updateUser(pUserInfo++);
			}
		}
		else if(ASS_GR_DESK_STATION == messageHead->bAssistantID)
		{
			CHECK_SOCKET_DATA(MSG_GR_DeskStation, objectSize, "size error.");

			MSG_GR_DeskStation * pDeskStation = (MSG_GR_DeskStation *)object;
			deskStation = *pDeskStation;
		}
		else
		{

		}
	}

	void HNRoomLogic::H_R_M_UserAction(const NetMessageHead* messageHead, void* object, INT objectSize)
	{
		// 排队坐下
		if(ASS_GR_QUEUE_USER_SIT == messageHead->bAssistantID)
		{
			H_R_M_QueueUserSit(messageHead, object, objectSize);
		}

		// 用户进入房间
		else if(ASS_GR_USER_COME == messageHead->bAssistantID)
		{
			H_R_M_UserCome(messageHead, object, objectSize);
		}

		// 用户离开房间
		else if(ASS_GR_USER_LEFT == messageHead->bAssistantID)
		{
			H_R_M_UserLeft(messageHead, object, objectSize);
		}

		// 用户断线
		else if(ASS_GR_USER_CUT == messageHead->bAssistantID)
		{
			H_R_M_UserCut(messageHead, object, objectSize);
		}

		// 用户站起（包含旁观站起）
		else if(ASS_GR_USER_UP == messageHead->bAssistantID || ASS_GR_WATCH_UP == messageHead->bAssistantID)
		{
			H_R_M_UserUp(messageHead, object, objectSize);
		}

		// 坐下错误
		else if(ASS_GR_SIT_ERROR == messageHead->bAssistantID)
		{
			H_R_M_SitError(messageHead, object, objectSize);
		}

		// 用户坐下
		else if(ASS_GR_USER_SIT == messageHead->bAssistantID || ASS_GR_WATCH_SIT == messageHead->bAssistantID)
		{
			H_R_M_UserSit(messageHead, object, objectSize);
		}
	}

	void HNRoomLogic::H_R_M_QueueUserSit(const NetMessageHead* messageHead, void* object, INT objectSize)
	{
		auto queueData = (int*)object;										
		auto deskNo = (BYTE)queueData[0];
		bool isFind = false;
		for (int i = 0; i < 3; i++)
		{
			if (PlatformLogic()->loginResult.dwUserID == queueData[3*i+1])
			{
				isFind = true;
			}
		}
		std::vector<QUEUE_USER_SIT_RESULT *> queueUsers;
		if (isFind)
		{
			for (int i = 0; i < 3; i++)
			{
				QUEUE_USER_SIT_RESULT * tmp = new QUEUE_USER_SIT_RESULT;
				tmp->dwUserID = queueData[3*i+1];
				tmp->bDeskStation = i;
				UserInfoStruct* userInfo = UserInfoModule()->findUser(tmp->dwUserID);

				if (nullptr != userInfo)
				{
					userInfo->bDeskNO = deskNo;
					userInfo->bDeskStation = tmp->bDeskStation;
					userInfo->bUserState = USER_ARGEE;
					userInfo->iRankNum = queueData[3*i+2];
					userInfo->iRemainPeople = queueData[3*i+3];
					//UserInfoModule()->addUser(userInfo);
				}
						
				queueUsers.push_back(tmp);
				//CC_SAFE_DELETE(tmp);
			}
		}

		// 分发游戏消息
		dispatchGameMessage([&deskNo, &queueUsers](IGameMessageDelegate* delegate) -> bool
		{
			delegate->I_R_M_QueueUserSit(deskNo, queueUsers);
			return false;
		});

		// 分发房间消息
		dispatchFrameMessage([&deskNo, &queueUsers](IRoomMessageDelegate* delegate) -> bool
		{
			delegate->I_R_M_QueueUserSit(deskNo, queueUsers);
			return false;
		});

		for (auto sit : queueUsers)
		{
			CC_SAFE_DELETE(sit);
		}
		queueUsers.clear();

	}

	void HNRoomLogic::H_R_M_UserCome(const NetMessageHead* messageHead, void* object, INT objectSize)
	{
		CHECK_SOCKET_DATA(MSG_GR_R_UserCome, objectSize, "size error.");

		MSG_GR_R_UserCome* pUserCome = (MSG_GR_R_UserCome*)object;

		UserInfoModule()->updateUser(&pUserCome->pUserInfoStruct);
		
		// 分发房间消息
		dispatchFrameMessage([&pUserCome](IRoomMessageDelegate* delegate) -> bool
		{
			delegate->I_R_M_UserCome(&pUserCome->pUserInfoStruct);
			return false;
		});	
	}


	void HNRoomLogic::H_R_M_UserLeft(const NetMessageHead* messageHead, void* object, INT objectSize)
	{
		CHECK_SOCKET_DATA(MSG_GR_R_UserLeft, objectSize, "size error.");

		MSG_GR_R_UserLeft * pUserLeft = (MSG_GR_R_UserLeft*)object;

		UserInfoStruct* userInfo = UserInfoModule()->findUser(pUserLeft->dwUserID);

		if(userInfo == nullptr)
		{
			return;
		}

		// 分发房间消息
		dispatchFrameMessage([&userInfo](IRoomMessageDelegate* delegate) -> bool
		{
			delegate->I_R_M_UserLeft(userInfo);
			return false;
		});
		UserInfoModule()->removeUser(pUserLeft->dwUserID);
	}

	void HNRoomLogic::H_R_M_UserCut(const NetMessageHead* messageHead, void* object, INT objectSize)
	{
		CHECK_SOCKET_DATA(MSG_GR_R_UserCut, objectSize, "size error.");

		MSG_GR_R_UserCut* pUserCut = (MSG_GR_R_UserCut*)object;

		UserInfoStruct* userInfo = UserInfoModule()->findUser(pUserCut->dwUserID);
		if (nullptr != userInfo)
		{
			//设置数据
			userInfo->bUserState = USER_CUT_GAME;
		}

		// 分发游戏消息
		dispatchGameMessage([&pUserCut, &userInfo](IGameMessageDelegate* delegate) -> bool
		{
			delegate->I_R_M_UserCut(pUserCut->dwUserID, pUserCut->bDeskNO, pUserCut->bDeskStation);
			return false;
		});

		// 分发房间消息
		dispatchFrameMessage([&pUserCut](IRoomMessageDelegate* delegate) -> bool
		{
			delegate->I_R_M_UserCut(pUserCut->dwUserID, pUserCut->bDeskNO, pUserCut->bDeskStation);
			return false;
		});
	}


	void HNRoomLogic::H_R_M_UserUp(const NetMessageHead* messageHead, void* object, INT objectSize)
	{
		if (ERR_GR_SIT_SUCCESS == messageHead->bHandleCode)
		{
			// 数据校验
			CHECK_SOCKET_DATA(MSG_GR_R_UserUp, objectSize, "size error.");

			MSG_GR_R_UserUp* pUserUp = (MSG_GR_R_UserUp*)object;

			// 锁桌更新
			if(pUserUp->bDeskIndex != INVALID_DESKNO)
			{
				bool isLocked = (pUserUp->bLock != 0);
				BYTE deskNo = pUserUp->bDeskIndex;
				if(isLocked)
				{
					// 锁桌							
					RoomLogic()->deskStation.bDeskLock[deskNo / 8] |= (1 << (deskNo % 8));
				}
				else
				{
					// 解锁
					RoomLogic()->deskStation.bDeskLock[deskNo / 8] &= (~(1 << (deskNo % 8)));
				}
			}

			// 更新自己信息
			if (pUserUp->dwUserID == loginResult.pUserInfoStruct.dwUserID)
			{
				loginResult.pUserInfoStruct.bDeskNO = pUserUp->bDeskIndex;
				loginResult.pUserInfoStruct.bDeskStation = pUserUp->bDeskStation;
				loginResult.pUserInfoStruct.bUserState = pUserUp->bUserState;
			}

			UserInfoStruct* userInfo = UserInfoModule()->findUser(pUserUp->dwUserID);
			// 更新用户数据
			if (nullptr != userInfo)
			{
				userInfo->bDeskNO = INVALID_DESKNO;
				userInfo->bDeskStation = INVALID_DESKSTATION;
				userInfo->bUserState = USER_LOOK_STATE;
			}
			// 分发游戏消息
			dispatchGameMessage([&pUserUp, &userInfo](IGameMessageDelegate* delegate) -> bool
			{
				delegate->I_R_M_UserUp(pUserUp, userInfo);
				return false;
			});

			// 分发房间消息
			dispatchFrameMessage([&pUserUp, &userInfo](IRoomMessageDelegate* delegate) -> bool
			{
				delegate->I_R_M_UserUp(pUserUp, userInfo);
				return false;
			});
		}
		else
		{
			std::string message = getSitErrorReason(messageHead->bHandleCode);
			// 分发游戏消息
			dispatchGameMessage([&message](IGameMessageDelegate* delegate) -> bool
			{
				delegate->I_R_M_SitError(message);
				return false;
			});

			// 分发房间消息
			dispatchFrameMessage([&message](IRoomMessageDelegate* delegate) -> bool
			{
				delegate->I_R_M_SitError(message);
				return false;
			});
		}
	}

	void HNRoomLogic::H_R_M_SitError(const NetMessageHead* messageHead, void* object, INT objectSize)
	{
		std::string message = getSitErrorReason(messageHead->bHandleCode);
		// 分发游戏消息
		dispatchGameMessage([&message](IGameMessageDelegate* delegate) -> bool
		{
			delegate->I_R_M_SitError(message);
			return false;
		});	

		// 分发房间消息
		dispatchFrameMessage([&message](IRoomMessageDelegate* delegate) -> bool
		{
			delegate->I_R_M_SitError(message);
			return false;
		});
	}


	void HNRoomLogic::H_R_M_UserSit(const NetMessageHead* messageHead, void* object, INT objectSize)
	{
		if (ERR_GR_SIT_SUCCESS == messageHead->bHandleCode)
		{
			// 数据校验
			CHECK_SOCKET_DATA(MSG_GR_R_UserSit, objectSize, "MSG_GR_R_UserSit size is error.");

			MSG_GR_R_UserSit * pUserSit = (MSG_GR_R_UserSit*)object;

			// 锁桌更新
			if(pUserSit->bDeskIndex != INVALID_DESKNO)
			{
				bool isLocked = (pUserSit->bLock != 0);
				BYTE deskNo = pUserSit->bDeskIndex;
				if(isLocked)
				{
					// 锁桌							
					RoomLogic()->deskStation.bDeskLock[deskNo / 8] |= (1 << (deskNo % 8));
				}
				else
				{
					// 解锁
					RoomLogic()->deskStation.bDeskLock[deskNo / 8] &= (~(1 << (deskNo % 8)));
				}
			}

			// 更新自己信息
			if (pUserSit->dwUserID == loginResult.pUserInfoStruct.dwUserID)
			{
				loginResult.pUserInfoStruct.bDeskNO = pUserSit->bDeskIndex;
				loginResult.pUserInfoStruct.bDeskStation = pUserSit->bDeskStation;
				loginResult.pUserInfoStruct.bUserState = pUserSit->bUserState;
			}

			UserInfoStruct* userInfo = UserInfoModule()->findUser(pUserSit->dwUserID);
			// 更新用户数据
			if (nullptr != userInfo)
			{
				userInfo->bDeskNO = pUserSit->bDeskIndex;
				userInfo->bDeskStation = pUserSit->bDeskStation;
				userInfo->bUserState = pUserSit->bUserState;
				UserInfoModule()->updateUser(userInfo);
			}

			// 分发游戏消息
			dispatchGameMessage([&pUserSit, &userInfo](IGameMessageDelegate* delegate) -> bool
			{
				delegate->I_R_M_UserSit(pUserSit, userInfo);
				return false;
			});	

			// 分发房间消息
			dispatchFrameMessage([&pUserSit, &userInfo](IRoomMessageDelegate* delegate) -> bool
			{
				delegate->I_R_M_UserSit(pUserSit, userInfo);
				return false;
			});
		}
	}

	std::string HNRoomLogic::getSitErrorReason(int handleCode)
	{
		std::string message;
		switch (handleCode)
		{
		case ERR_GR_BEGIN_GAME:
			message = "坐下此位置失败,游戏已经开始了!";
			break;
		case ERR_GR_ALREAD_USER:
			message = "坐下此位置失败,下次动作快一点喔!";
			break;
		case ERR_GR_PASS_ERROR:
			message = "游戏桌密码错误,请在游戏设置中重新设置您的携带密码!";
			break;
		case ERR_GR_IP_SAME:
			message = "同桌玩家不允许有相同 IP 地址的玩家一起进行游戏!";
			break;
		case ERR_GR_IP_SAME_3:
			message = "同桌玩家不允许有 IP 地址前３位相同的玩家一起进行游戏!";
			break;
		case ERR_GR_IP_SAME_4:
			message = "同桌玩家不允许有IP 地址前４位相同的玩家一起进行游戏!";
			break;
		case ERR_GR_CUT_HIGH:
			message = "同桌玩家认为您的逃跑率太高,不愿意和您游戏!";
			break;
		case ERR_GR_POINT_LOW:
			message = "本桌玩家设置的进入条件，您不符合本桌进入条件!";
			break;
		case ERR_GR_POINT_HIGH:	
			message = "本桌玩家设置的进入条件，您不符合本桌进入条件!";
			break;
		case ERR_GR_NO_FRIEND:
			message = "此桌有您不欢迎的玩家!";
			break;
		case ERR_GR_POINT_LIMIT:
			message = StringUtils::format("此游戏桌需要至少 %u 的游戏积分, 您的积分不够, 不能游戏!", loginResult.uLessPoint);
			break;
		case ERR_GR_CAN_NOT_LEFT:
			message = "您正在游戏中...";
			break;
		case ERR_GR_NOT_FIX_STATION:
			message = "您不能加入此游戏桌游戏!";
			break;
		case ERR_GR_MATCH_FINISH:
			message = "您的比赛已经结束了,不能继续参加比赛!";
			break;
		case ERR_GR_MATCH_WAIT://比赛场排队中
			message = "比赛排队中";
			break;
		case ERR_GR_UNENABLE_WATCH:
			message = "不允许旁观游戏!";
			break;
		case ERR_GR_FAST_SIT:
			message = "快速坐下失败！";
			break;			
		default:
			message = "未知错误";
			break;
		}

		return GBKToUtf8(message.c_str());
	}

	bool HNRoomLogic::dispatchFrameMessage(const RoomFrameMessageFun& func)
	{
		auto iterA = _addRoomNotifyQueue->begin();
		while(iterA != _addRoomNotifyQueue->end())
		{
			auto next = iterA;
			next++;
			_roomNotifyQueue->addObserver(iterA->second);
			_addRoomNotifyQueue->removeObserver(iterA->second);
			iterA = next;
		}

		auto iterR = _removeRoomNotifyQueue->begin();
		while(iterR != _removeRoomNotifyQueue->end())
		{
			auto next = iterR;
			next++;
			_roomNotifyQueue->removeObserver(iterR->second);
			_removeRoomNotifyQueue->removeObserver(iterR->second);
			iterR = next;
		}

		for (auto iter = _roomNotifyQueue->begin(); iter != _roomNotifyQueue->end(); ++iter)
		{
			bool bRemoved = false;
			for(auto iterR = _removeRoomNotifyQueue->begin(); iterR != _removeRoomNotifyQueue->end(); iterR++)
			{
				if(iterR->second == iter->second)
				{
					bRemoved = true;
					break;
				}
			}
			if(!bRemoved)
			{
				func(iter->second);
			}				
		}
		return true;
	}

	bool HNRoomLogic::dispatchGameMessage(const GameMessageFun& func)
	{
		auto iterA = _addGameNotifyQueue->begin();
		while(iterA != _addGameNotifyQueue->end())
		{
			auto next = iterA;
			next++;
			_gameNotifyQueue->addObserver(iterA->second);
			_addGameNotifyQueue->removeObserver(iterA->second);
			iterA = next;
		}
		
		auto iterR = _removeGameNotifyQueue->begin();
		while(iterR != _removeGameNotifyQueue->end())
		{
			auto next = iterR;
			next++;
			_gameNotifyQueue->removeObserver(iterR->second);
			_removeGameNotifyQueue->removeObserver(iterR->second);
			iterR = next;
		}		

		for (auto iter = _gameNotifyQueue->begin(); iter != _gameNotifyQueue->end(); ++iter)
		{
			bool bRemoved = false;
			for(auto iterR = _removeGameNotifyQueue->begin(); iterR != _removeGameNotifyQueue->end(); iterR++)
			{
				if(iterR->second == iter->second)
				{
					bRemoved = true;
					break;
				}
			}
			if(!bRemoved)
			{
				func(iter->second);
			}				
		}
		return true;
	}

}
