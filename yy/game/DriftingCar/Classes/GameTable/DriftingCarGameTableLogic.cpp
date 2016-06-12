#include "DriftingCarGameTableLogic.h"
#include "DriftingCarGameTableUICallback.h"

#include "HNNetExport.h"

using namespace HN;

namespace DriftingCar
{
	GameTableLogic::GameTableLogic(GameTableUICallback* uiCallback, BYTE deskNo, bool bAutoCreate)
		: _uiCallback(uiCallback)
		, HNGameLogicBase(deskNo, DriftingCar::Game_Const::PLAY_COUNT, bAutoCreate, uiCallback)
	{
		initParams();
	}

	GameTableLogic::~GameTableLogic()
	{

	}

	void GameTableLogic::dealGameMessage(NetMessageHead* messageHead, void* object, INT objectSize)
	{
		if(MDM_GM_GAME_NOTIFY != messageHead->bMainID)
		{
			HNLOG("onGameMessage MainID is error.");
			return;
		}

		switch (messageHead->bAssistantID)
		{
		case ASS_PREPARE_END://
			//手机端不用处理。
			//HNLOG("dealPrepareEndResp");
			//dealPrepareEndResp(object, objectSize);
			break;
		case ASS_SUPER_USER:
			HNLOG("dealSuperUserResp");
			dealSuperUserResp(object, objectSize);
			break;
		case ASS_UPDATE_BET:
			HNLOG("dealUpdateBetResp");
			dealUpdateBetResp(object, objectSize);
			break;
		case ASS_GAME_FREE:
			HNLOG("dealGameFreeResp");
			dealGameFreeResp(object, objectSize);
			break;
		case ASS_BEGIN_BET:
			HNLOG("dealBeginBetResp");
			dealBeginBetResp(object, objectSize);
			break;
		case ASS_GAME_SEND:
			HNLOG("dealGameSendResp");
			dealGameSendResp(object, objectSize);
			break;
		case ASS_APPLY_RESULT:
			HNLOG("dealApplyResultResp");
			dealApplyResultResp(object, objectSize);
			break;
		case ASS_LEAVE_RESULT:
			HNLOG("dealLeaveResultResp");
			dealLeaveResultResp(object, objectSize);
			break;
		case ASS_BET_RESULT:
			HNLOG("dealBetResultResp");
			dealBetResultResp(object, objectSize);
			break;
		case ASS_GAME_OPEN:
			HNLOG("dealGameOpenResp");
			dealGameOpenResp(object, objectSize);
			break;
		case ASS_WAITE_NT:
			HNLOG("dealWaitNT");
			dealWaitNT(object, objectSize);
			break;
		default:
			break;
		}
	}

	void GameTableLogic::dealWaitNT( void* object, INT objectSize )
	{
		CCAssert(objectSize == sizeof(S_C_WaiteNtMsg), "size error.");
		S_C_WaiteNtMsg* pData = (S_C_WaiteNtMsg*)object;

		if(pData->bWait)
		{
			_NTSeatNo = INVALID_DESKSTATION;
			_NTCount  = 0;
			_NTGrade  = 0;
		}
		
		//显示一个图片
		_uiCallback->ShowNoNT();
		if(_mySeatNo != _NTSeatNo)
		{
			_uiCallback->ShowImageMyNoNT();
			_uiCallback->ShowApplyButton();
		}
		
		//庄家信息，清除
		updateNTInfo(_NTSeatNo, _NTCount, _NTGrade);

		setApplyButtonEnabled(_NTSeatNo);
	}

	void GameTableLogic::dealGameStationResp(void* object, INT objectSize)
	{
		if (objectSize != sizeof(GameStation_Base))	return;

		GameStation_Base *pData = (GameStation_Base *)object;
		_GameState    = pData->bGameStation;
		_FreeTime     = pData->byFreeTime;
		_BetTime      = pData->byBetTime;
		_SendTime     = pData->bySendTime;
		_RemaindTime  = pData->byRemaindTime;
		_NTSeatNo     = pData->byNtStation;
		_MaxNTPeople  = pData->byMaxNtPeople;
		_NTCount      = pData->iNtPlayCount;
		_NTGrade      = pData->i64NtGrade;

		std::copy(pData->byWaitNtList, pData->byWaitNtList + PLAY_COUNT, _NTWaitList.begin());
		CopyArray(pData->byLuziList, HISTORY_COUNT, _historyArray);

		//删除之前庄家列表
		removeNT(_NTWaitList);

		//添加上庄列表
 		addNT(_NTWaitList); 		

		ShowWaitNTListCount(_NTSeatNo);

// 		//添加历史开奖记录
 		reloadRecord(_historyArray, 72, Result_NON);		
 
 		//庄家信息
 		updateNTInfo(_NTSeatNo, _NTCount, _NTGrade);
 
 		//玩家信息
 		updateUserInfo(_mySeatNo, pData->i64PlayerGrade);

 		//显示上下庄按钮
		if(_mySeatNo != INVALID_DESKSTATION && _mySeatNo == _NTSeatNo)
		{
			_uiCallback->ShowDownNTButton();
		}
 		
 		//是否可以下注
 		setBetEnable();

		_uiCallback->OnGameStation();

		switch(_GameState)
		{
		case GS_WAIT_SETGAME:		//游戏没有开始状态
		case GS_WAIT_ARGEE:			//等待玩家开始状态
		case GS_WAIT_NEXT:
			{
				_uiCallback->setFreeTime(_RemaindTime);
				_uiCallback->startGameFree();
				break; 
			}

		case GS_FREE_STATUS://空闲阶段
			{
				_uiCallback->setFreeTime(_RemaindTime);
				_uiCallback->startGameFree();
				break;
			}
		case GS_BET_STATUS:	//游戏下注阶段
			{
				_uiCallback->setBetTime(_RemaindTime);
				_uiCallback->startGameBeginBet();
				break ;
			}
		case GS_SEND_STATUS://开牌阶段
			{
				_uiCallback->setSendTime(_RemaindTime);
				break;
			}
		}
	}

	void GameTableLogic::dealUserAgreeResp(MSG_GR_R_UserAgree* agree)
	{

	}

	void GameTableLogic::dealGameStartResp(BYTE bDeskNO)
	{

	}

	void GameTableLogic::dealGameEndResp(BYTE bDeskNO)
	{

	}

	void GameTableLogic::dealGamePointResp(void* object, INT objectSize)
	{

		CCAssert(sizeof (MSG_GR_R_UserPoint) == objectSize, "MSG_GR_R_UserPoint is error.");
		MSG_GR_R_UserPoint * pUserPoint = (MSG_GR_R_UserPoint*)object;
		if(_mySeatNo != INVALID_DESKSTATION && pUserPoint->dwUserID == RoomLogic()->loginResult.pUserInfoStruct.dwUserID)
		{
			UserInfoStruct* pUser = _deskUserList->getUserByDeskStation(_mySeatNo);
			_uiCallback->setUserMoney(pUser->i64Money);
		}
	}

	void GameTableLogic::dealUserSitResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user)
	{

	}

	void GameTableLogic::dealUserUpResp(MSG_GR_R_UserUp * userSit, UserInfoStruct* user)
	{
		if(_mySeatNo == INVALID_DESKSTATION)
		{
			_uiCallback->leaveDesk();
		}
	}

	void GameTableLogic::dealGameInfoResp(MSG_GM_S_GameInfo* pGameInfo)
	{

	}

	void GameTableLogic::dealUserCutMessageResp(INT userId, BYTE seatNo)
	{

	}

	/************************************************************************/
	//游戏内消息-开始
	void GameTableLogic::dealPrepareEndResp(void* object, INT objectSize)
	{
		if(objectSize != sizeof(GamePrepareStageQuit))	return;

		GamePrepareStageQuit* pData = (GamePrepareStageQuit*)object;

		_NTSeatNo = pData->byNtStation;
		_NTGrade  = pData->i64NtScore;
		_NTCount  = pData->byNtcount;

		//std::copy(pData->byNtWaitList, pData->byNtWaitList + PLAY_COUNT, _NTWaitList);
		setApplyButtonEnabled(_NTSeatNo);
	}

	void GameTableLogic::dealSuperUserResp(void* object, INT objectSize)
	{

	}

	void GameTableLogic::dealSuperSendResp(void* object, INT objectSize)
	{
		if(objectSize != sizeof(SuperUserControlStart))	return;

		SuperUserControlStart* pData = (SuperUserControlStart*)object;
	}

	void GameTableLogic::dealUpdateBetResp(void* object, INT objectSize)
	{
		if(objectSize != sizeof(TUpdateHistoryBet))	return;

		TUpdateHistoryBet* pData = (TUpdateHistoryBet*)object;

		_SumAllBet = pData->i64SumAllBet;

		CopyArray(pData->i64AreaBetAllSum, BET_ARES, _AreaAllBetArray);
		CopyArray(pData->i64UserBetSum, BET_ARES, _UserBetSumArray);

		//更新下注区域及其下注总额数据
		_uiCallback->setBetTotal(pData->i64SumAllBet);

		for(int i = 0; i < BET_ARES; i++)
		{
			_uiCallback->setBetAreaTotal(false, i, pData->i64AreaBetAllSum[i]);
			_uiCallback->setBetAreaUser(false, i, pData->i64UserBetSum[i]);
		}

		
	}
	
	void GameTableLogic::dealGameFreeResp(void* object, INT objectSize)
	{
		if(objectSize != sizeof(BeginFreeMsg))	return;

		BeginFreeMsg* pData = (BeginFreeMsg*)object;

		if(!pData->bFlag)
			return;

		_FreeTime = pData->byFreeTime;
		_GameState = pData->byGameStation;
		_NTCount = pData->byNtcount;
		_NTSeatNo = pData->byNtStation;
		_NTGrade = pData->i64NtScore;

		_arrayUserBetArea.fill(false);

			//更新庄家列表，有金币更新，和庄家下庄更新
		CopyArray(pData->byWaitNtList, PLAY_COUNT, _NTWaitList);

		removeNT(_NTWaitList);

		addNT(_NTWaitList);

		setApplyButtonEnabled(_NTSeatNo);

		_uiCallback->startGameFree();

		_uiCallback->setFreeTime(_FreeTime);
	
		_uiCallback->hideGameEndImage();

		_uiCallback->hideOpenArea();

		_uiCallback->HideBetAreaAction();
	}

	void GameTableLogic::dealBeginBetResp(void* object, INT objectSize)
	{
		if(objectSize != sizeof(BeginBetMsg))	return;

		BeginBetMsg* pData = (BeginBetMsg*)object;

		_GameState = pData->byGameStation;
		_NTCount = pData->byNtCount;
		_NTSeatNo = pData->byNtStation;
		_NTGrade = pData->i64NtScore;

		// 更新庄家列表
		removeNT(_NTWaitList);
		CopyArray(pData->byNtWaitList, PLAY_COUNT, _NTWaitList);
		addNT(_NTWaitList);

		CopyArray(pData->byCardStyle, SEND_COUNT, _CardStyleArray);

		updateNTInfo(_NTSeatNo, _NTCount, _NTGrade);	

 		setBetEnable();

		setApplyButtonEnabled(_NTSeatNo);

		_uiCallback->setBetTime(_BetTime);

		_uiCallback->startGameBeginBet();

		//显示一个图片
		if(_mySeatNo != _NTSeatNo)
		{
			_uiCallback->ShowImageMyNoNT();
			_uiCallback->ShowApplyButton();
		}
		
	}

	void GameTableLogic::dealGameSendResp(void* object, INT objectSize)
	{
		if(objectSize != sizeof(BeginSendCardMsg))	return;

		BeginSendCardMsg* pData = (BeginSendCardMsg*)object;

		_GameState = pData->byGameStation;

		_WinIndex = pData->byWinPrize;

		_uiCallback->startGamePaoMaDeng();

		_uiCallback->setSendTime(_SendTime);

		_uiCallback->startSend(pData->byWinPrize, _SendTime - 6);

		GetHistroyResult(pData->byWinPrize);

		setBetEnable();

	}

	void GameTableLogic::dealApplyResultResp(void* object, INT objectSize)
	{
		if(objectSize != sizeof(ApplyNtResultMsg))	return;
		ApplyNtResultMsg* pData = (ApplyNtResultMsg*)object;

		_NTCount = pData->byNtcount;
		_NTSeatNo = pData->byNtStation;
		_NTGrade = pData->i64NtScore;

		if(pData->bApplay)
		{
			//钱不够，上庄失败
			_uiCallback->setMsg(GBKToUtf8("钱不够，上庄失败!"));
		}
		else
		{

			removeNT(_NTWaitList);

			_NTWaitList.fill(INVALID_DESKSTATION);

			CopyArray(pData->byWaitNtList,PLAY_COUNT, _NTWaitList );

			addNT(_NTWaitList);

			ShowWaitNTListCount(_mySeatNo);

			updateNTInfo(_NTSeatNo, _NTCount, _NTGrade);

			if (_mySeatNo == pData->byDeskStation)
			{
				_uiCallback->ShowDownNTButton();//显示下庄按钮
				_uiCallback->ShowImageMyNT();
			}
		}

		setApplyButtonEnabled(_NTSeatNo);
	}

	void GameTableLogic::dealLeaveResultResp(void* object, INT objectSize)
	{
		if(objectSize != sizeof(ApplyLiveNtResultMsg))	return;

		ApplyLiveNtResultMsg* pData = (ApplyLiveNtResultMsg*)object;
		
		_NTCount = pData->byNtcount;
		_NTSeatNo = pData->byNtStation;
		_NTGrade = pData->i64NtScore;

		if(_NTSeatNo == 255 && _mySeatNo == pData->byDeskStation)
		{
			_uiCallback->ShowImageMyNoNT();
		}
		removeNT(_NTWaitList);

		_NTWaitList.fill(INVALID_DESKSTATION);

		CopyArray(pData->byWaitNtList, PLAY_COUNT, _NTWaitList);

		addNT(_NTWaitList);

		updateNTInfo(_NTSeatNo, _NTCount, _NTGrade);

		ShowWaitNTListCount(_mySeatNo);

		if(_mySeatNo == pData->byDeskStation)
		{
			_uiCallback->ShowApplyButton();
		}
	}

	void GameTableLogic::dealBetResultResp(void* object, INT objectSize)
	{
		if(objectSize != sizeof(UserBetResultMsg))	return;

		UserBetResultMsg* pData = (UserBetResultMsg*)object;

		// 0表示成功 1表示庄家不够，2表示自己不够，3表示下注已满
		if (pData->byBetDenied)
		{
			return;
		}
		//bool		bDynamicBet;		           //是否动态下注
		//BYTE	    byDeskStation;		           //下注座位号
		//BYTE		byBetArea;			           //下注区域
		//BYTE		byChouMaType;		           //下注筹码类型
		//BYTE	    byBetNumFlag;		           //下注100W，500W，超500W的标志
		//LLONG       i64SumAllBet;                  //玩家总下注
		//LLONG       i64UserBetCount[BET_ARES];     //玩家在每个区域的下注情况
		//LLONG		i64AreaBetAllSum[BET_ARES];	   //各区域的总下注
		_SumAllBet = pData->i64SumAllBet;

		CopyArray(pData->i64AreaBetAllSum, BET_ARES, _AreaAllBetArray);
		CopyArray(pData->i64UserBetCount, BET_ARES, _UserBetSumArray);

		_uiCallback->setBetTotal(pData->i64SumAllBet);

		bool isMy = (_mySeatNo == pData->byDeskStation);

		_uiCallback->setBetAreaTotal(!isMy, pData->byBetArea, pData->i64AreaBetAllSum[pData->byBetArea]);
		_uiCallback->setBetAreaUser(isMy, pData->byBetArea, pData->i64UserBetCount[pData->byBetArea]);
		
		//得到下注区域
		if (_mySeatNo != pData->byDeskStation)
		{
			_uiCallback->GameBetReuslt();

			return;
		}

		LLONG SunOfUserBet = 0;
		for (int i=0; i<BET_ARES; i++)
		{
			SunOfUserBet += _UserBetSumArray[i];
		}

		
		LLONG SunOfPreUserBet = 0;

		for (int i=0; i<BET_ARES; i++)
		{
			SunOfPreUserBet += _PreUserBetArray[i];
		}

		UserInfoStruct* pUser = _deskUserList->getUserByDeskStation(_mySeatNo);
		if(pUser != nullptr)
		{	
			LLONG AfterBetMoney = pUser->i64Money + SunOfPreUserBet - SunOfUserBet;
			_uiCallback->setUserMoney(AfterBetMoney);

			std::copy(_PreUserBetArray.begin(), _PreUserBetArray.end(), _UserBetSumArray.begin());
		}

		_arrayUserBetArea[pData->byBetArea] = true;
		_uiCallback->setUserBetMoney(SunOfUserBet);

		

		_uiCallback->GameBetReuslt();

	}

	void GameTableLogic::ShowGameEndResult()
	{
		reloadRecord(_historyArray, 72, _byHistory);

		updateUserInfo(_mySeatNo,_UserGrade);

		_uiCallback->setBetTotal(0);//清除上盘下注区域及其下注总额数据

		for(int i=0; i < BET_ARES; i++)
		{
			_uiCallback->setBetAreaTotal(false, i, 0);
			_uiCallback->setBetAreaUser(false, i, 0);
		}

		_uiCallback->SetGameEndResult(_lReuslt[0], _lReuslt[1],_lReuslt[2], _lReuslt[3]);
	}

	//注意 这里保存结算信息，然后在跑马灯动画结束时，调用结算界面，这里没有显示结算界面
	void GameTableLogic::dealGameOpenResp(void* object, INT objectSize)
	{
		if(objectSize != sizeof(BeginAward))	return;

		BeginAward* pData = (BeginAward*)object;

		CopyArray(pData->byLuziList, HISTORY_COUNT, _historyArray);

		_UserGrade = pData->i64PlayerGrade[0];//玩家成绩
		_PreUserBetArray.fill(0);
		_lReuslt[0] = pData->i64PlayCoin[0];
		_lReuslt[1] = pData->i64ReturnMoney[0];
		_lReuslt[2] = pData->i64PlayCoin[1];
		_lReuslt[3] = pData->i64ReturnMoney[1];

		_uiCallback->startGameEndResult();

	}
	//游戏内消息-结束
	/************************************************************************/

	void GameTableLogic::initParams()
	{
		
		
		_NTSeatNo = INVALID_DESKSTATION;
		//_PlayerSeatNo = INVALID_DESKSTATION;
		_NTCount = 0;
		_NTGrade = 0.0;
		_UserGrade = 0;
		_byHistory = 0;
		_PreUserBetArray.fill(0);

		_NTWaitList.fill(INVALID_DESKSTATION);

		_historyArray.fill(INVALID_DESKSTATION);

		_arrayUserBetArea.fill(false);
	}

	void GameTableLogic::refreshParams()
	{
		_SumAllBet = 0;
		//_GameState = 0;
		_FreeTime = 0;
		_WinIndex = INVALID_DESKSTATION;

		_AreaAllBetArray.fill(0);
		_UserBetSumArray.fill(0);

		_CardStyleArray.fill(0);
		_RunTimeArray.fill(0);

	}

	void GameTableLogic::clearDesk()
	{
		refreshParams();

		for(int i = 0; i < BET_ARES; i++)
		{
			_uiCallback->setBetAreaTotal(false, i, 0);
			_uiCallback->setBetAreaUser(false, i, 0);
		}
	}

	void GameTableLogic::updateNTInfo(BYTE lSeatNo, int NTCount, LLONG grade)
	{
		
		if(lSeatNo == INVALID_DESKSTATION)
		{
			//表示没有庄家
			_uiCallback->setNTCount(0);
			_uiCallback->setNTName("");
			_uiCallback->setNTGrade(0);
			_uiCallback->setNTMoney(0);
		}
		else
		{
			UserInfoStruct* pUser = _deskUserList->getUserByDeskStation(lSeatNo);
			if(pUser != nullptr)
			{
				_uiCallback->setNTCount(NTCount);
				_uiCallback->setNTName(GBKToUtf8(pUser->nickName));
				_uiCallback->setNTGrade(grade);
				_uiCallback->setNTMoney(pUser->i64Money);
			}
		}
		
	}
		/*
		 * 更新玩家信息
		 * lSeatNo:玩家座位号
		 * grade  :玩家成绩
		 */
	void GameTableLogic::updateUserInfo(BYTE lSeatNo, LLONG grade)
	{
		if(lSeatNo == INVALID_DESKSTATION)	return;

		UserInfoStruct* pUser = _deskUserList->getUserByDeskStation(lSeatNo);
		if(pUser != nullptr)
		{
			_uiCallback->setUserGrade(grade);
			_uiCallback->setUserMoney(pUser->i64Money);
			_uiCallback->setUserName(GBKToUtf8(pUser->nickName));
			_uiCallback->setUserBetMoney(0);
		}
	}

	void GameTableLogic::addNT(BYTE seatNo)
	{
		if(seatNo != INVALID_DESKSTATION)
		{
			UserInfoStruct* pUser = _deskUserList->getUserByDeskStation(seatNo);
			if(pUser != nullptr)
			{
				_uiCallback->addNT(seatNo, GBKToUtf8(pUser->nickName), pUser->i64Money);
			}
		}
	}
	void GameTableLogic::addNT(const std::array<BYTE,PLAY_COUNT>& setNo)
	{
		for (int index = 0; index < setNo.size(); ++index)
		{
			addNT(setNo[index]);
		}
		
	}

	void GameTableLogic::removeNT(const std::array<BYTE,PLAY_COUNT>&)
	{
		_uiCallback->removeAllNT();
	}

	void GameTableLogic::reloadRecord(const std::array<BYTE, HISTORY_COUNT>& records, int size, BYTE byResult)
	{
		if(size > records.size())
		{
			throw "数组越界";
		}
		
		std::vector<BYTE> vtRecord(size);
		std::copy(records.begin(), records.begin() + size, vtRecord.begin());
		

		_uiCallback->addRecord(vtRecord, byResult);

	}
	void GameTableLogic::setBetEnable()
	{
		bool bRet = (_mySeatNo != INVALID_DESKSTATION && _mySeatNo != _NTSeatNo && _GameState == GS_BET_STATUS);
		_uiCallback->setBetEnabled(bRet);
	}
	void GameTableLogic::setApplyButtonEnabled(BYTE NTseatNo)
	{
		if(_mySeatNo == NTseatNo )
		{
			if(_GameState == GS_FREE_STATUS)
			{
				_uiCallback->setApplyAndLeaveButtonEnabled(true);//庄家在空闲时，按钮可用
			}
			else
			{
				_uiCallback->setApplyAndLeaveButtonEnabled(false);
			}
		
		}
		else
		{
			_uiCallback->setApplyAndLeaveButtonEnabled(true);
		}
	}
	void GameTableLogic::requestApplyNT()
	{
		if(_mySeatNo == INVALID_DESKSTATION || _mySeatNo == _NTSeatNo)	return;
		
		ApplyNtMsg msg;
		msg.byDeskStation = _mySeatNo;
		RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, ASS_APPLY_NT, &msg, sizeof(msg));
	}

	void GameTableLogic::requestApplyLeaveNT()
	{
		ApplyLiveNtMsg msg;
		msg.byDeskStation=_mySeatNo;
		RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, ASS_APPLY_LEAVE, &msg, sizeof(msg));
	}

	void GameTableLogic::requestUserBet(BYTE area, BYTE type)
	{

		if(_mySeatNo == INVALID_DESKSTATION)	return;

		if(type < 0 || type >5 || area < 0 || area > 7)	return;

		UserBetMsg msg;
		msg.byDeskStation = _mySeatNo;
		msg.byChouMaType  = type;//[0,5]
		msg.byBetArea     = area;//[0,7]


		RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, ASS_USER_BET, &msg, sizeof(msg));
	}

	void GameTableLogic::requestLeaveTable()
	{
		do 
		{
			if (!RoomLogic()->isConnect())
			{
				_uiCallback->leaveDesk();
				break;
			}

			if (INVALID_DESKNO == _mySeatNo)
			{
				_uiCallback->leaveDesk();
				break;
			}

			if(_GameState == GS_FREE_STATUS || _GameState == GS_WAIT_ARGEE)
			{
				sendUserUp();
				break;
			}
			UserInfoStruct* myInfo = _deskUserList->getUserByDeskStation(_mySeatNo);
			if(myInfo != nullptr && myInfo->bUserState == USER_PLAY_GAME)
			{
				_uiCallback->setMsg(GBKToUtf8("游戏中，不能离开"));
				break;
			}
		} while (0);
	}

	void GameTableLogic::ShowWaitNTListCount( BYTE byseatNo )
	{
		_uiCallback->ShowWaitNTCount(byseatNo);
	}

	void GameTableLogic::GetHistroyResult( int indexLogos )
	{
		//0,大保时捷 0
		//1,小保时捷4
		//2,大宝马 1
		//3,小宝马5
		//4,大奔驰 2
		//5,小奔驰6
		//6,大大众3
		//7 小大众7
		indexLogos = indexLogos % BET_ARES;

		//下注区域
		int iResult = 0;

		switch (indexLogos)
		{
		case 0:
			iResult = 0;
			break;
		case 1:
			iResult = 4;
			break;
		case 2:
			iResult = 1;
			break;
		case 3:
			iResult = 5;
			break;
		case 4:
			iResult = 2;
			break;
		case 5:
			iResult = 6;
			break;
		case 6:
			iResult = 3;
			break;
		case 7:
			iResult = 7;
			break;

		default:
			break;
		}
			
		bool bNoBet = false;
		for (int i = 0; i < _arrayUserBetArea.size(); i++)
		{
			if(_arrayUserBetArea[i])
			{
				bNoBet = true;
				break;
			}
		}
		if(!bNoBet)
		{
			_byHistory = Result_NON;
		}
		else if(_arrayUserBetArea[iResult])
		{
			_byHistory = Result_Win;
		}
		else
		{
			_byHistory = Result_Lose;
		}
		
	}




}
