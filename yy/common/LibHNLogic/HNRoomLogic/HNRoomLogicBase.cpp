#include "HNRoomLogic/HNRoomLogicBase.h"
#include "HNNetExport.h"

namespace HN
{
	void HNRoomLogicBase::start()
	{
		RoomLogic()->addRoomObserver(this);
		PlatformLogic()->addObserver(this);
	}

	void HNRoomLogicBase::stop()
	{
		RoomLogic()->removeRoomObserver(this);
		PlatformLogic()->removeObserver(this);
	}

	void HNRoomLogicBase::requestLogin(UINT roomID)
	{
		ComRoomInfo* pRoomInfo = RoomInfoModule()->getByRoomID(roomID);
		if(nullptr == pRoomInfo)
		{
			_callback->onRoomLoginCallback(false, GBKToUtf8("房间Id错误"), _roomID);
		}
		else
		{
			if (PlatformLogic()->loginResult.i64Money < pRoomInfo->iLessPoint)
			{
				char buffer[128];
				sprintf(buffer, "您的金币小于%u, 不能进入房间, 请到商店充值。", pRoomInfo->iLessPoint);
				_callback->onRoomLoginCallback(false, GBKToUtf8(buffer), _roomID);
			}
			else if(pRoomInfo->iMaxPoint != 0 && (PlatformLogic()->loginResult.i64Money > pRoomInfo->iMaxPoint))
			{
				char buffer[128];
				sprintf(buffer, "您的金币大于%u, 不能进入房间,超过最大买入。", pRoomInfo->iMaxPoint);
				_callback->onRoomLoginCallback(false, GBKToUtf8(buffer), _roomID);
			}
			else
			{
				RoomLogic()->setSelectedRoom(pRoomInfo);
				RoomLogic()->setRoomRule(pRoomInfo->dwRoomRule);
				_roomID = roomID;

				if (RoomLogic()->getRoomRule() & GRR_QUEUE_GAME)		
				{
					// 排队机
					// 连接房间服务器
					UserInfoModule()->clear();
					RoomLogic()->connect(pRoomInfo->szServiceIP, pRoomInfo->uServicePort);
				}
				else												
				{
					// 金币场不扣积分
					// 连接房间服务器
					UserInfoModule()->clear();
					RoomLogic()->connect(pRoomInfo->szServiceIP, pRoomInfo->uServicePort);
				}
			}
		}
	}

	void HNRoomLogicBase::requestSit(BYTE deskNo)
	{
		
		ComRoomInfo* pRoomInfo = RoomLogic()->getSelectedRoom();
		if(nullptr == pRoomInfo)
		{
			_callback->onRoomSitCallback(false, GBKToUtf8("房间信息为空"), _roomID, deskNo, INVALID_DESKSTATION);
			return;
		}

		std::vector<UserInfoStruct*> deskUsers;
		UserInfoModule()->findDeskUsers(deskNo, deskUsers);

		std::vector<bool> empty(pRoomInfo->uDeskPeople, true);
		
		// 查找坐下的用户座位号
		for (auto user : deskUsers)
		{
			if (USER_SITTING == user->bUserState		    // 坐下
				|| USER_PLAY_GAME == user->bUserState		// 游戏中
				|| USER_CUT_GAME == user->bUserState		// 断线
				|| USER_ARGEE == user->bUserState)			// 同意游戏
			{
				empty[user->bDeskStation] = false;
			}
		}

		BYTE seatNo = INVALID_DESKSTATION;
		for(size_t i = 0; i < empty.size(); i++)
		{
			if(empty[i])
			{
				seatNo = i; 
				break;
			}
		}

		if(INVALID_DESKSTATION != seatNo)
		{
			requestSit(deskNo, seatNo);
		}	
		else
		{
			_callback->onRoomSitCallback(false, GBKToUtf8("房间没有空座"), _roomID, deskNo, seatNo);
		}
	}

	void HNRoomLogicBase::requestSit(BYTE deskNo, BYTE seatNo)
	{
		MSG_GR_S_UserSit data;
		data.bDeskIndex   = deskNo;
		data.bDeskStation = seatNo;
		RoomLogic()->sendData(MDM_GR_USER_ACTION, ASS_GR_USER_SIT, &data ,sizeof(data));
	}

	void HNRoomLogicBase::requestJoinQueue()
	{
		RoomLogic()->sendData(MDM_GR_USER_ACTION, ASS_GR_JOIN_QUEUE);
	}

	void HNRoomLogicBase::requestQuickSit()
	{
		RoomLogic()->sendData(MDM_GR_USER_ACTION, ASS_GR_FAST_JOIN_IN, nullptr, 0);
	}

	void HNRoomLogicBase::requestRoomPasword(UINT roomId, const std::string& password)
	{
		MSG_GP_CheckRoomPasswd data;
		data.uRoomID = roomId;
		strcpy(data.szMD5PassWord, MD5_CTX::MD5String(password).c_str());
		PlatformLogic()->sendData(MDM_GP_LIST, ASS_GP_ROOM_PASSWORD, &data, sizeof(data));
	}

	void HNRoomLogicBase::I_R_M_SitError(const std::string& message)
	{
		_callback->onRoomSitCallback(false, message, INVALID_USER_ID, INVALID_DESKNO, INVALID_DESKSTATION);
	}

	void HNRoomLogicBase::I_R_M_UserSit(MSG_GR_R_UserSit * userSit, UserInfoStruct* user)
	{
		if (userSit->dwUserID == PlatformLogic()->loginResult.dwUserID && (nullptr == userSit ||  nullptr == user)) 
		{
			_callback->onRoomSitCallback(false, GBKToUtf8("坐下失败"), _roomID, INVALID_DESKNO, INVALID_DESKSTATION);
			RoomLogic()->close();
			return;
		}

		// 断线重连进来
		if (userSit->dwUserID == PlatformLogic()->loginResult.dwUserID)
		{
			if (INVALID_DESKNO != userSit->bDeskIndex && INVALID_DESKSTATION != userSit->bDeskStation)
			{
				_callback->onRoomSitCallback(true, GBKToUtf8("坐下成功"), _roomID, userSit->bDeskIndex, userSit->bDeskStation);
			}
		}
	}

	void HNRoomLogicBase::I_R_M_QueueUserSit(BYTE deskNo, const std::vector<QUEUE_USER_SIT_RESULT*>& users)
	{
		if(INVALID_DESKNO == deskNo)
		{
			_callback->onRoomQueueSitCallback(false, GBKToUtf8("座位号错误"), _roomID, deskNo);
			return;
		}

		bool find = false;
		for (auto iter = users.begin(); iter != users.end(); ++iter)
		{
			if((*iter)->dwUserID == PlatformLogic()->loginResult.dwUserID)
			{
				find = true;
				break;
			}
		}
		if (find)
		{
			_callback->onRoomQueueSitCallback(true, GBKToUtf8("成功进入游戏"), _roomID, deskNo);
		}
		else
		{
			_callback->onRoomQueueSitCallback(false, GBKToUtf8("未发现玩家"), _roomID, deskNo);
		}
	}

	void HNRoomLogicBase::I_R_M_UserUp(MSG_GR_R_UserUp * userUp, UserInfoStruct* user)
	{
		if(INVALID_DESKNO == userUp->bDeskIndex)
		{
			_callback->onRoomSitUpCallback(false, GBKToUtf8("站起失败"), userUp->bDeskIndex, userUp->bDeskStation);
		}
		else
		{
			_callback->onRoomSitUpCallback(true, GBKToUtf8("站起成功"), userUp->bDeskIndex, userUp->bDeskStation);
		}
	}

	void HNRoomLogicBase::I_R_M_DisConnect()
	{
		RoomLogic()->close();
		_callback->onRoomDisConnect(GBKToUtf8("网络断开连接"));
	}

	void HNRoomLogicBase::I_R_M_Connect(bool result)
	{
		if(!result)
		{
			_callback->onRoomLoginCallback(false, GBKToUtf8("网络连接失败"), _roomID);
		}
		else
		{
			ComRoomInfo* pRoomInfo = RoomLogic()->getSelectedRoom();
			if(pRoomInfo == nullptr)
			{
				_callback->onRoomLoginCallback(false, GBKToUtf8("房间信息为空"), _roomID);
			}
			else
			{
				RoomLogic()->login(pRoomInfo->uNameID);
			}			
		}
	}

	void HNRoomLogicBase::I_R_M_Login(bool success, UINT handleCode, const std::string& message)
	{
		if(!success)
		{			
			_callback->onRoomLoginCallback(false, message, _roomID);
		}
		else
		{
			;
		}
	}

	void HNRoomLogicBase::I_R_M_LoginFinish()
	{
		UserInfoStruct info = RoomLogic()->loginResult.pUserInfoStruct;

		if (INVALID_DESKNO != info.bDeskNO && INVALID_DESKSTATION != info.bDeskStation)
		{
			_callback->onRoomSitCallback(true, GBKToUtf8("坐下成功"), _roomID, info.bDeskNO, info.bDeskStation);
		}
		else
		{
			_callback->onRoomLoginCallback(true, GBKToUtf8("连接成功"), _roomID);
		}
	}

	void HNRoomLogicBase::I_R_M_UserCome(UserInfoStruct* user) 
	{
		_callback->onRoomUserCome(user->dwUserID);
	}

	void HNRoomLogicBase::I_R_M_UserLeft(UserInfoStruct* user)
	{
		_callback->onRoomUserLeft(user->dwUserID);
	}

	void HNRoomLogicBase::I_R_M_GamePoint(void * object, INT objectSize)
	{

	}

	void HNRoomLogicBase::I_P_M_RoomPassword(MSG_GP_S_C_CheckRoomPasswd* pData)
	{
		CCAssert(pData != nullptr, "size error.");
		_callback->onPlatformRoomPassEnter(pData->bRet, pData->uRoomID);
	}

	HNRoomLogicBase::HNRoomLogicBase(IHNRoomLogicBase* callback)
	{
		CCAssert(callback != nullptr, "callback is null.");
		_callback = callback;
	}

	HNRoomLogicBase::~HNRoomLogicBase()
	{
		_callback = nullptr;
	}
}