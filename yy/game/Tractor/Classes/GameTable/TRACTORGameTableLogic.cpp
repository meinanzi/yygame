#include "TRACTORGameTableLogic.h"
#include "TRACTORGameTableUI.h"
#include "TRACTORMessageHead.h"
#include "HNLogicExport.h"

using namespace HN;
namespace TRACTOR
{
	/************************分割线*********************************/
	GameTableLogic::GameTableLogic(GameTableUICallback* uiCallback, BYTE deskNo, bool bAutoCreate):
		_uiCallback(uiCallback), HNGameLogicBase(deskNo, PLAY_COUNT, bAutoCreate, uiCallback)
	{
		_tableInfo.byDeskNo = deskNo;
		_tableInfo.bAutoCreate = bAutoCreate;
		initParams();
	}

	GameTableLogic::~GameTableLogic()
	{

	}

	/************************line*********************************/
	void GameTableLogic::dealGameMessage(NetMessageHead* messageHead, void* object, INT objectSize)
	{
		switch(messageHead->bAssistantID)//通知消息
		{
		case ASS_UG_USER_SET:
			HNLOG("ASS_UG_USER_SET");
			//CCAssert( objectSize == sizeof() , "");
			dealUserSetResp(object,objectSize);
			break;
		case ASS_BEGIN_UPGRADE:
			HNLOG("ASS_BEGIN_UPGRADE");
			dealBeginUpgradeResp(object, objectSize);
			break;
		case ASS_SEND_CARD:
			HNLOG("ASS_SEND_CARD");
			dealSendCardResp(object, objectSize);
			break;
		case ASS_SEND_FINISH:
			HNLOG("ASS_SEND_FINISH");
			dealSendFinishResp(object, objectSize);
			break;
		case ASS_GAME_PLAY:
			HNLOG("ASS_GAME_PLAY");
			dealGamePlayResp(object, objectSize);
			break;
		case ASS_NEW_TURN:
			HNLOG("ASS_NEW_TURN");
			break;
		case ASS_CONTINUE_END:
			HNLOG("ASS_CONTINUE_END");
			dealContinueEndResp(object, objectSize);
			break;
		case ASS_NO_CONTINUE_END:
			HNLOG("ASS_NO_CONTINUE_END");
			dealContinueEndResp(object, objectSize);
			break;
		case ASS_CUT_END:
			HNLOG("ASS_CUT_END");
			dealCutEndResp(object, objectSize);
			break;
		case ASS_SALE_END:
			HNLOG("ASS_SALE_END");
			dealSaleEndResp(object, objectSize);
			break;
		case ASS_VREB_CHECK:
			HNLOG("ASS_VREB_CHECK");
			break;
		case ASS_SHUTTLE:
			HNLOG("ASS_SHUTTLE");
			break;
		case ASS_ADD_NOTE:
			HNLOG("ASS_ADD_NOTE");
			break;
		case ASS_GIVE_UP:
			HNLOG("ASS_GIVE_UP");
			break;
		case ASS_MODIFY_PLAY_NOTE:
			HNLOG("ASS_MODIFY_PLAY_NOTE");
			break;
		case ASS_NOTE:
			HNLOG("ASS_NOTE");
			dealNoteResp(object, objectSize);
			break;
		case ASS_NOTE_RESULT:
			HNLOG("ASS_NOTE_RESULT");
			dealNoteResultResp(object, objectSize);
			break;
		case ASS_COLLECT_JETON:
			HNLOG("ASS_COLLECT_JETON");
			dealCollectJetonResp(object, objectSize);
			break;
		case ASS_BIPAI_RESULT:
			HNLOG("ASS_BIPAI_RESULT");
			dealCompareResultResp(object, objectSize);
			break;
		case ASS_GM_SUPERCLIENT:
			HNLOG("ASS_GM_SUPERCLIENT");
			break;
		case ASS_FINISH_COMPARE:
			HNLOG("ASS_FINISH_COMPARE");
			dealFinishCompareResp(object, objectSize);
			break;
		case ASS_AI_WIN:
			HNLOG("ASS_AI_WIN");
			break;
		case ASS_SET_TEST_CARD:
			HNLOG("ASS_SET_TEST_CARD");
			break;
		case ASS_SUPER_RESULT:
			HNLOG("ASS_SUPER_RESULT");
			break;
		case ASS_SUPER_USER:
			HNLOG("ASS_SUPER_USER");
			dealSuperUserResp(object, objectSize);
			break;
		default:
			;
		}
	}

	/************************分割线*********************************/

	/************************************************************************/
	/* 消息处理函数                                                         */
	/************************************************************************/
	void GameTableLogic::dealUserSetResp(void* object, INT objectSize)
	{

	}

	void GameTableLogic::dealBeginUpgradeResp(void* object, INT objectSize)
	{
		if(objectSize != sizeof(BeginUpgradeStruct))
		{
			return;
		}
		refreshParams();

		BeginUpgradeStruct* pBegin = (BeginUpgradeStruct*)object;
		_tableInfo.byGameStation   = GS_SEND_CARD;
		_tableInfo.iLimitNote      = pBegin->iLimitNote;
		_tableInfo.iBaseNote       = pBegin->iBaseNote;
		_tableInfo.iLimtePerNote   = pBegin->iLimtePerNote;
		_tableInfo.iGuoDi          = pBegin->iGuoDi;
		_tableInfo.byNtPeople      = pBegin->bNtStation;
		_tableInfo.bShowIdOrPlay   = pBegin->bIsShowIdOrPlay;

		for(int i = 0; i < PLAY_COUNT; i++)
		{
			memcpy(_tableInfo.strCity[i], pBegin->strCityText[i], sizeof(pBegin->strCityText[i]));
			memcpy(_tableInfo.strUserID[i], pBegin->strUserIDText[i], sizeof(pBegin->strUserIDText[i]));
		}
		memcpy(_tableInfo.bSuperUser, pBegin->bSuperUser, sizeof(pBegin->bSuperUser));

		int iGamePlayer = 0;
		for(int i = 0; i < PLAY_COUNT; i++)
		{
			UserInfoStruct* userInfo = _deskUserList->getUserByDeskStation(i);
			if(userInfo != nullptr)
			{
				_tableInfo.bPlayer[i] = true;
				if(playingGame(userInfo->bUserState))
				{
					iGamePlayer++;
				}
				_uiCallback->showUserMoney(logicToViewSeatNo(i), userInfo->i64Money);
			}
			else
			{
				_tableInfo.bPlayer[i] = false;
			}
		}

		//initGuoDi();
		showUserInfo();

		_uiCallback->showLimitNote(pBegin->iLimitNote);
		_uiCallback->showBaseNote(pBegin->iBaseNote);
		_uiCallback->showLimitPerNote(pBegin->iLimtePerNote * 2);
		_uiCallback->showGuoDi(pBegin->iGuoDi);
		_uiCallback->showNT(logicToViewSeatNo(pBegin->bNtStation));
		_uiCallback->showTotalNote(_tableInfo.iTotalNote);
	}

	void GameTableLogic::dealSendCardResp(void* object, INT objectSize)
	{
		if(objectSize != sizeof(SendCardStruct))
		{
			return;
		}
		//add by yh - 20150112
		initGuoDi();
		_uiCallback->showTotalNote(_tableInfo.iTotalNote);

		_tableInfo.byGameStation = GS_SEND_CARD;

		SendCardStruct* pSendCard = (SendCardStruct*)object;
		memcpy(_tableInfo.iUserCardCount, pSendCard->bCardCount, sizeof(pSendCard->bCardCount));
		for(int i = 0; i < PLAY_COUNT; i++)
		{
			memcpy(_tableInfo.iUserCard[i], pSendCard->bCard[i], sizeof(pSendCard->bCard[i]));
		}

		std::vector<THandCard> cards;
		for(int i = 0; i < PLAY_COUNT; i++)
		{
			if(pSendCard->bCardCount[i] > 0)
			{
				THandCard card;
				card.bySeatNo = logicToViewSeatNo(i);
				memcpy(card.byCards, pSendCard->bCard[i], sizeof(card.byCards));
				cards.push_back(card);
			}
		}
		if(!cards.empty())
		{
			CCLOG("BUGBUG DearCards begin:");
			_uiCallback->showHandCard(cards);
		}
	}

	void GameTableLogic::dealSendFinishResp(void* object, INT objectSize)
	{

	}

	void GameTableLogic::dealGamePlayResp(void* object, INT objectSize)
	{
		if(objectSize != sizeof(BeginPlayStruct))
		{
			return;
		}
		_tableInfo.byGameStation  = GS_PLAY_GAME;

		BeginPlayStruct* pGamePlay = (BeginPlayStruct*)object;
		_tableInfo.byTimeOutCount = 0;
		_tableInfo.byOutCardPeople = _tableInfo.byFirstOutPeople = pGamePlay->iOutDeskStation;
		_tableInfo.iThisTurnLimit = _tableInfo.iLimitNote;
		_tableInfo.byNtPeople = pGamePlay->bNtPeople;

		BYTE vSeatNo = logicToViewSeatNo(pGamePlay->iOutDeskStation);
		_uiCallback->showDealer(logicToViewSeatNo(pGamePlay->bNtPeople));
		bool isMe = pGamePlay->iOutDeskStation == _tableInfo.byMeSeatNo;
		_uiCallback->showDashboard(isMe);
		if(isMe)
		{
			_uiCallback->showAddRange(_tableInfo.iBaseNote, _tableInfo.iLimtePerNote);
			_uiCallback->showUserAction(vSeatNo, C_VF_ADD | C_VF_GIVEUP | C_VF_LOOK);
		}

		stopAllWait();
		_uiCallback->showWaitTime(vSeatNo, _tableInfo.iThinkTime, _tableInfo.iThinkTime);
	}

	void GameTableLogic::dealContinueEndResp(void* object, INT objectSize)
	{
		if(objectSize != sizeof(GameEndStruct))
		{
			return;
		}
		BYTE winViewSeeNo = -1;
		_tableInfo.byGameStation = GS_WAIT_ARGEE;

		GameEndStruct* pData = (GameEndStruct*)object;

		//stopAllWait();

		//std::string type[8] = 
		//{
		//	GBKToUtf8("错误"),GBKToUtf8("特殊牌型"),
		//	GBKToUtf8("单牌"),GBKToUtf8("对子"),
		//	GBKToUtf8("顺子"),GBKToUtf8("同花"),
		//	GBKToUtf8("同花顺"),GBKToUtf8("三条")
		//};
		bool isWin_Me = pData->iTurePoint[_mySeatNo] > 0 ? true : false;
		std::vector<TGameResult> results;
		for(int i = 0; i < PLAY_COUNT; i++)
		{
			BYTE vSeatNo = logicToViewSeatNo(i);
			TGameResult rt;
			rt.llMoney = pData->iTurePoint[i];
			rt.szType = pData->iCardShape[i];
			rt.viewSeatNo = vSeatNo;
			UserInfoStruct* userInfo = _deskUserList->getUserByDeskStation(i);
			if(userInfo != nullptr)
			{
				rt.szName = userInfo->nickName;
				rt.i64Money = userInfo->i64Money;
				results.push_back(rt);
			}
			//show all user's hand card.
			if(pData->bCard[i][0] != 0 && pData->bCard[i][1] != 0 && pData->bCard[i][2] != 0)
			{
				std::vector<BYTE> cards(3);
				std::copy(pData->bCard[i], pData->bCard[i] + 3, &cards.front());
				std::copy(pData->bCard[i], pData->bCard[i] + 3, _tableInfo.iUserCard[i]);
				_uiCallback->showUserHandCard(vSeatNo, cards);
			}
			if(pData->iTurePoint[i] > 0)
			{
				_uiCallback->showWin(vSeatNo);
			}
			if(pData->iTurePoint[i] != 0)
			{
				_uiCallback->showUserProfit(vSeatNo, pData->iTurePoint[i]);
			}
			//showUserInfo();
		}
		if(_tableInfo.byMeSeatNo != INVALID_DESKNO && _tableInfo.byWaitTime > 0)
		{
			_uiCallback->showWaitTime(logicToViewSeatNo(_tableInfo.byMeSeatNo), _tableInfo.byWaitTime, _tableInfo.byWaitTime);
		}

		_uiCallback->showResulte(results , isWin_Me);
		sendUserReady();
	}

	void GameTableLogic::dealCutEndResp(void* object, INT objectSize)
	{
		GameCutStruct* pData = (GameCutStruct*)object;
	}

	void GameTableLogic::dealSaleEndResp(void* object, INT objectSize)
	{
		GameCutStruct* pData = (GameCutStruct*)object;
	}

	void GameTableLogic::dealNoteResp(void* object, INT objectSize)
	{
		if(objectSize == sizeof(lookCardResult))
		{
			lookCardResult* pResult = (lookCardResult*)object;
			BYTE vSeatNo = logicToViewSeatNo(pResult->bDeskStation);
			_tableInfo.bMing[pResult->bDeskStation] = true;

			bool isMe = (pResult->bDeskStation == _tableInfo.byMeSeatNo);
			_uiCallback->showUserLookCard(vSeatNo, isMe);
			if(isMe)
			{
				std::vector<BYTE> cards(pResult->iUserCardCount);
				std::copy(pResult->iUserCard, pResult->iUserCard + pResult->iUserCardCount, &cards.front());

				_uiCallback->showUserHandCard(vSeatNo, cards);
				_uiCallback->showAddRange(_tableInfo.iBaseNote * 2, _tableInfo.iLimtePerNote);
			}
		}
		else if(objectSize == sizeof(tagUserResult))
		{
			tagUserResult* pResult     = (tagUserResult*)object;
			_tableInfo.byOutCardPeople = pResult->iOutPeople;
			_tableInfo.iThisTurnLimit  = _tableInfo.iLimitNote;
			_tableInfo.bFirstGame      = pResult->bIsFirstNote;
			_tableInfo.iTotalNote      = pResult->iTotalNote;

			_uiCallback->showTotalNote(pResult->iTotalNote);
			BYTE vNextSeatNo = logicToViewSeatNo(pResult->iOutPeople);
			_uiCallback->showWaitTime(vNextSeatNo, _tableInfo.iThinkTime, _tableInfo.iThinkTime);
			bool nextIsMe = (pResult->iOutPeople == _tableInfo.byMeSeatNo);
			_uiCallback->showDashboard(nextIsMe);
			if(nextIsMe)
			{
				if(_tableInfo.bMing[_tableInfo.byMeSeatNo])
				{
					_uiCallback->showAddRange(_tableInfo.iBaseNote * 2, _tableInfo.iLimtePerNote);
				}
				else
				{
					_uiCallback->showAddRange(_tableInfo.iBaseNote, _tableInfo.iLimtePerNote);
				}

				int players = 0;
				for(int i = 0; i < PLAY_COUNT; i++)
				{
					UserInfoStruct* pUser = _deskUserList->getUserByDeskStation(i);
					if(pUser != nullptr && playingGame(pUser->bUserState))
					{
						players++;
					}
				}
				if(_tableInfo.iNoteTimes >= players)
				{
					_uiCallback->showUserAction(vNextSeatNo, C_VF_FOLLOW | C_VF_LOOK | C_VF_ADD | C_VF_COMPARE | C_VF_GIVEUP);
				}
				else if(_tableInfo.iPlayerNote[_tableInfo.byMeSeatNo] < _tableInfo.iLimtePerNote)
				{
					_uiCallback->showUserAction(vNextSeatNo, C_VF_FOLLOW | C_VF_LOOK | C_VF_ADD | C_VF_GIVEUP);
				}
				else
				{
					_uiCallback->showUserAction(vNextSeatNo, C_VF_FOLLOW | C_VF_LOOK | C_VF_GIVEUP);
				}
			}
		}
		else
		{

		}
	}

	void GameTableLogic::dealNoteResultResp(void* object, INT objectSize)
	{
		if(objectSize != sizeof(NoteResult))
		{
			return;
		}
		stopAllWait();

		NoteResult* pResult = (NoteResult*)object;
		_tableInfo.iLimitNote = pResult->iLimiteNote;
		_tableInfo.iLastNote[pResult->iOutPeople] = pResult->iCurNote;
		_tableInfo.iPlayerNote[pResult->iOutPeople] += pResult->iCurNote;
		_tableInfo.iTotalNote += pResult->iCurNote;
		_tableInfo.iNoteTimes++;

		//current note people.
		bool isMeNoted = (pResult->iOutPeople == _tableInfo.byMeSeatNo);
		BYTE vSeatNo = logicToViewSeatNo(pResult->iOutPeople);
		switch (pResult->bAddStyle)
		{
		case TYPE_GIVE_UP:
			{
				_tableInfo.bMing[pResult->iOutPeople] = false;
				_tableInfo.bIsGiveUp[pResult->iOutPeople] = true;
				_uiCallback->showUserGiveUp(vSeatNo);

				int count = 0;
				for(int i = 0; i < PLAY_COUNT; i++)
				{
					if(_tableInfo.bPlayer[i] && _tableInfo.iUserCardCount[i] > 0 && !_tableInfo.bIsGiveUp[i])
					{
						count++;
						if(count >= 2)
						{
							break;
						}
					}
				}
				if(count < 2)
				{
					pResult->bNextDeskStation = INVALID_DESKNO;
				}
			}
			break;
		case TYPE_NOTE:
			{
				_uiCallback->showUserNote(vSeatNo);
			}
			break;
			//case TYPE_ADD:
			//	{
			//		_uiCallback->showUserAdd(vSeatNo);
			//	}
			//	break;
		case TYPE_FOLLOW:
			{
				_uiCallback->showUserFollow(vSeatNo);
			}
			break;
		case TYPE_LOOKCARD:
			{
				_uiCallback->showUserLookCard(vSeatNo, isMeNoted);
			}
			break;
		case TYPE_OPENCARD:
			{
				_uiCallback->showUserOpenCard(vSeatNo);
			}
			break;
		case TYPE_COMPARE_CARD:
			{
				//_uiCallback->showUserCompare(vSeatNo,)
			}
			break;
		default:
			break;
		}

		if(pResult->iNote > 0)
		{
			_uiCallback->showUserNoteMoney(vSeatNo, pResult->iCurNote);
		}

		//next note people.
		if(pResult->bNextDeskStation == INVALID_DESKNO)
		{
			return;
		}

		BYTE vNextSeatNo = logicToViewSeatNo(pResult->bNextDeskStation);
		_uiCallback->showWaitTime(vNextSeatNo, _tableInfo.iThinkTime, _tableInfo.iThinkTime);
		bool nextIsMe = (pResult->bNextDeskStation == _tableInfo.byMeSeatNo);
		_uiCallback->showDashboard(nextIsMe);
		if(nextIsMe)
		{
			if(_tableInfo.bMing[_tableInfo.byMeSeatNo])
			{
				_uiCallback->showAddRange(_tableInfo.iBaseNote * 2, _tableInfo.iLimtePerNote);
			}
			else
			{
				_uiCallback->showAddRange(_tableInfo.iBaseNote, _tableInfo.iLimtePerNote);
			}

			int players = 0;
			for(int i = 0; i < PLAY_COUNT; i++)
			{
				UserInfoStruct* pUser = _deskUserList->getUserByDeskStation(i);
				if(pUser != nullptr && playingGame(pUser->bUserState))
				{
					players++;
				}
			}
			if(_tableInfo.iNoteTimes >= players)
			{
				_uiCallback->showUserAction(vNextSeatNo, C_VF_FOLLOW | C_VF_LOOK | C_VF_ADD | C_VF_COMPARE | C_VF_GIVEUP);
			}
			else if(_tableInfo.iPlayerNote[_tableInfo.byMeSeatNo] < _tableInfo.iLimtePerNote)
			{
				_uiCallback->showUserAction(vNextSeatNo, C_VF_FOLLOW | C_VF_LOOK | C_VF_ADD | C_VF_GIVEUP);
			}
			else
			{
				_uiCallback->showUserAction(vNextSeatNo, C_VF_FOLLOW | C_VF_LOOK | C_VF_GIVEUP);
			}
		}
	}

	void GameTableLogic::dealCollectJetonResp(void* object, INT objectSize)
	{

	}

	void GameTableLogic::dealCompareResultResp(void* object, INT objectSize)
	{
		if(objectSize != sizeof(tagCompareCardResult))
		{
			return;
		}
		stopAllWait();

		tagCompareCardResult* pResult = (tagCompareCardResult*)object;
		_tableInfo.iLastNote[pResult->iNt] += pResult->iCurNote;
		_tableInfo.iPlayerNote[pResult->iNt] += pResult->iCurNote;
		_tableInfo.bOpenLose[pResult->iLoster] = true;
		_tableInfo.iTotalNote += pResult->iCurNote;

		if(pResult->bGameFinish)
		{

		}

		//reach limit note
		if(pResult->bAddStyle == TYPE_REACH_LIMITE_SELF)
		{

		}
		else if(pResult->bAddStyle == TYPE_REACH_LIMITE_NEXT)
		{

		}
		else
		{

		}

		_uiCallback->showDashboard(false);
		_uiCallback->showCompareResult(logicToViewSeatNo(pResult->bWinner), logicToViewSeatNo(pResult->iLoster));

		//add by yh - 20150112
		_uiCallback->showUserNoteMoney(logicToViewSeatNo(pResult->iNt), pResult->iCurNote);
		_uiCallback->showTotalNote(_tableInfo.iTotalNote);
	}

	/*
	* compare before game over.
	*/
	void GameTableLogic::dealFinishCompareResp(void* object, INT objectSize)
	{
		if(objectSize != sizeof(GameEndStruct))
		{
			return;
		}
		stopAllWait();

		GameEndStruct* pData = (GameEndStruct*)object;
		BYTE bWinSeatNo = INVALID_DESKNO;
		for(int i = 0; i < PLAY_COUNT; i++)
		{
			if(pData->iTurePoint[i] > 0)
			{
				bWinSeatNo = i;
			}
			if(pData->iUserState[_tableInfo.byMeSeatNo] && pData->iUserState[i])
			{
				std::vector<BYTE> cards(3);
				std::copy(pData->bCard[i], pData->bCard[i] + 3, &cards.front());
				_uiCallback->showUserHandCard(logicToViewSeatNo(i), cards);
			}
		}
		if(bWinSeatNo < PLAY_COUNT)
		{
			_uiCallback->showWin(logicToViewSeatNo(bWinSeatNo));
		}

		_uiCallback->showDashboard(false);
	}

	void GameTableLogic::dealSuperUserResp(void* object, INT objectSize)
	{
		SUPERSTATE* pData = (SUPERSTATE*)object;
	}

	void GameTableLogic::dealGameInfoResp(MSG_GM_S_GameInfo* pGameInfo)
	{
		_tableInfo.byGameStation = pGameInfo->bGameStation;
		_tableInfo.byWaitTime = pGameInfo->bWaitTime;
		if(_tableInfo.byMeSeatNo != INVALID_DESKNO)
		{
			_uiCallback->showWaitTime(logicToViewSeatNo(_tableInfo.byMeSeatNo), pGameInfo->bWaitTime, pGameInfo->bWaitTime);
			_uiCallback->showReady(true);
			_uiCallback->showNotice(GBKToUtf8(pGameInfo->szMessage));
		}
	}

	void GameTableLogic::dealGameStationResp(void* object, INT objectSize)
	{
		refreshParams();

		GameStationBase* pGameBase = (GameStationBase*)object;

		_tableInfo.byGameStation = pGameBase->bGameStation;

		switch (pGameBase->bGameStation)
		{
		case GS_WAIT_SETGAME:
		case GS_WAIT_ARGEE:
			{
				GameStation_2* pGameStation = (GameStation_2*)object;
				_tableInfo.iThinkTime       = pGameStation->iThinkTime;
				_tableInfo.iBeginTime       = pGameStation->iBeginTime;
				_tableInfo.iRoomBasePoint   = pGameStation->iRoomBasePoint;
				_tableInfo.iRunPublish       = pGameStation->iRunPublish;
				_tableInfo.iLimitNote       = pGameStation->iLimitNote;
				_tableInfo.iBaseNote        = pGameStation->iBaseNote;
				_tableInfo.iLimtePerNote    = pGameStation->iLimtePerNote;
				_tableInfo.iGuoDi           = pGameStation->iGuoDi;
				_tableInfo.bShowIdOrPlay    = pGameStation->bShowIdOrPlay;
				_tableInfo.iFrontNote       = 0;

				_uiCallback->showBaseNote(pGameStation->iBaseNote);
				_uiCallback->showGuoDi(pGameStation->iGuoDi);
				_uiCallback->showLimitPerNote(pGameStation->iLimtePerNote);
				_uiCallback->showLimitNote(pGameStation->iLimitNote);
				_uiCallback->showTotalNote(0);
			}
			break;
		case GS_SEND_CARD:
			{
				GameStation_3* pGameStation = (GameStation_3*)object;
				_tableInfo.iThinkTime       = pGameStation->iThinkTime;
				_tableInfo.iBeginTime       = pGameStation->iBeginTime;
				_tableInfo.iRoomBasePoint   = pGameStation->iRoomBasePoint;
				_tableInfo.iRunPublish       = pGameStation->iRunPublish;
				_tableInfo.iLimitNote       = pGameStation->iLimitNote;
				_tableInfo.iBaseNote        = pGameStation->iBaseNote;
				_tableInfo.iLimtePerNote    = pGameStation->iLimtePerNote;
				_tableInfo.iGuoDi           = pGameStation->iGuoDi;
				_tableInfo.iFrontNote       = 0;
				_tableInfo.byNtPeople       = pGameStation->iUpGradePeople;
				_tableInfo.bShowIdOrPlay    = pGameStation->bShowIdOrPlay;
				memcpy(_tableInfo.bPlayer, pGameStation->bPlayer, sizeof(_tableInfo.bPlayer));
				for(int i = 0; i < PLAY_COUNT; i++)
				{
					_tableInfo.iUserCardCount[i] = pGameStation->iUserCardCount[i];
					if(_tableInfo.iUserCardCount[i] > 0)
					{
						memcpy(_tableInfo.iUserCard[i], pGameStation->iUserCard[i], sizeof(BYTE) * _tableInfo.iUserCardCount[i]);
						std::vector<BYTE> cards(pGameStation->iUserCardCount[i]);
						std::copy(pGameStation->iUserCard[i], pGameStation->iUserCard[i] + pGameStation->iUserCardCount[i], &cards.front());
						_uiCallback->showUserHandCard(logicToViewSeatNo(i),cards);
					}

				}
				initGuoDi();

				_uiCallback->showBaseNote(pGameStation->iBaseNote);
				_uiCallback->showGuoDi(pGameStation->iGuoDi);
				_uiCallback->showLimitPerNote(pGameStation->iLimtePerNote);
				_uiCallback->showLimitNote(pGameStation->iLimitNote);
				_uiCallback->showTotalNote(_tableInfo.iTotalNote);
			}
			break;
		case GS_PLAY_GAME:
			{
				GameStation_4* pGameStation = (GameStation_4*)object;
				_tableInfo.iThinkTime       = pGameStation->iThinkTime;
				_tableInfo.iBeginTime       = pGameStation->iBeginTime;
				_tableInfo.iRoomBasePoint   = pGameStation->iRoomBasePoint;
				_tableInfo.iRunPublish       = pGameStation->iRunPublish;
				_tableInfo.iLimitNote       = pGameStation->iLimitNote;
				_tableInfo.iBaseNote        = pGameStation->iBaseNote;
				_tableInfo.iLimtePerNote    = pGameStation->iLimtePerNote;
				_tableInfo.iGuoDi           = pGameStation->iGuoDi;
				_tableInfo.bFirstGame       = pGameStation->bIsFirstNote;
				_tableInfo.byNtPeople       = pGameStation->bNtPeople;
				_tableInfo.byFirstOutPeople = pGameStation->iFirstOutPeople;
				_tableInfo.byOutCardPeople  = pGameStation->iOutCardPeople;
				_tableInfo.iThisTurnLimit   = _tableInfo.iLimitNote;
				_tableInfo.tag              = pGameStation->tag;
				_tableInfo.iTotalNote = pGameStation->tag.iTotalNote;

				memcpy(_tableInfo.bSuperUser, pGameStation->bSuperUser, sizeof(_tableInfo.bSuperUser));
				memcpy(_tableInfo.iPlayerNote, pGameStation->iTotalNote, sizeof(pGameStation->iTotalNote));
				memcpy(_tableInfo.bMing, pGameStation->iMing, sizeof(pGameStation->iMing));
				memcpy(_tableInfo.iLastNote, pGameStation->iThisGameNote, sizeof(pGameStation->iThisGameNote));
				memcpy(_tableInfo.bOpenLose, pGameStation->bOpenLose, sizeof(pGameStation->bOpenLose));
				memcpy(_tableInfo.bIsGiveUp, pGameStation->bIsGiveUp, sizeof(pGameStation->bIsGiveUp));
				memcpy(_tableInfo.byWin, pGameStation->byWin, sizeof(pGameStation->byWin));
				memcpy(_tableInfo.bPlayer, pGameStation->bPlayer,sizeof(pGameStation->bPlayer));

				for(int i = 0; i < PLAY_COUNT; i++)
				{
					_tableInfo.iUserCardCount[i] = pGameStation->iUserCardCount[i];
					if(_tableInfo.iUserCardCount[i] > 0)
					{
						memcpy(_tableInfo.iUserCard[i], pGameStation->iUserCard[i], sizeof(BYTE) * _tableInfo.iUserCardCount[i]);
						std::vector<BYTE> cards(pGameStation->iUserCardCount[i]);
						std::copy(pGameStation->iUserCard[i], pGameStation->iUserCard[i] + pGameStation->iUserCardCount[i], &cards.front());
						_uiCallback->showUserHandCard(logicToViewSeatNo(i),cards);
					}

				}

				_uiCallback->showWaitTime(logicToViewSeatNo(_tableInfo.byOutCardPeople), _tableInfo.iThinkTime, _tableInfo.iThinkTime);
				_uiCallback->showDealer(logicToViewSeatNo(_tableInfo.byNtPeople));

				_uiCallback->showBaseNote(pGameStation->iBaseNote);
				_uiCallback->showGuoDi(pGameStation->iGuoDi);
				_uiCallback->showLimitPerNote(pGameStation->iLimtePerNote);
				_uiCallback->showLimitNote(pGameStation->iLimitNote);
				_uiCallback->showTotalNote(_tableInfo.iTotalNote);
			}
			break;
		case GS_WAIT_NEXT:
			{
				GameStation_5* pGameStation = (GameStation_5*)object;
				_tableInfo.iThinkTime       = pGameStation->iThinkTime;
				_tableInfo.iBeginTime       = pGameStation->iBeginTime;
				_tableInfo.iRoomBasePoint   = pGameStation->iRoomBasePoint;
				_tableInfo.iRunPublish       = pGameStation->iRunPublish;
				_tableInfo.iLimitNote       = pGameStation->iLimitNote;
				_tableInfo.iBaseNote        = pGameStation->iBaseNote;
				_tableInfo.iLimtePerNote    = pGameStation->iLimtePerNote;
				_tableInfo.iGuoDi           = pGameStation->iGuoDi;
				_tableInfo.bShowIdOrPlay    = pGameStation->bShowIdOrPlay;
				memcpy(_tableInfo.bSuperUser, pGameStation->bSuperUser, sizeof(pGameStation->bSuperUser));

				_uiCallback->showBaseNote(pGameStation->iBaseNote);
				_uiCallback->showGuoDi(pGameStation->iGuoDi);
				_uiCallback->showLimitPerNote(pGameStation->iLimtePerNote);
				_uiCallback->showLimitNote(pGameStation->iLimitNote);
				_uiCallback->showTotalNote(_tableInfo.iTotalNote);
			}
			break;
		default:
			break;
		}
	}


	void GameTableLogic::dealUserAgreeResp(MSG_GR_R_UserAgree* agree)
	{
		if(agree->bDeskNO != _tableInfo.byDeskNo)
		{
			return;
		}

		if(agree->bAgreeGame == 1)
		{
			_uiCallback->showWaitTime(logicToViewSeatNo(agree->bDeskStation), 0, 0);
		}
	}

	void GameTableLogic::dealGameStartResp(BYTE bDeskNO)
	{
		if(bDeskNO == _tableInfo.byDeskNo)
		{
			_uiCallback->clearDesk();
		}
	}

	void GameTableLogic::dealGameEndResp(BYTE deskNo)
	{
// 		// update user's game state.
		_deskUserList->transform([](UserInfoStruct* user, INT index) 
		{
			if (user->bUserState != USER_WATCH_GAME)
			{
				user->bUserState = USER_SITTING;
			}

			//if (RoomLogic()->getRoomRule() & GRR_CONTEST)
			//{
			//	user->bDeskNO = INVALID_DESKNO;
			//	user->bDeskStation = INVALID_DESKSTATION;
			//	user->bUserState = USER_LOOK_STATE;						
			//}
		});
	}

	void GameTableLogic::dealUserSitResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user)
	{
		if(userSit->bDeskIndex != _tableInfo.byDeskNo)
		{
			return;
		}

		BYTE vSeatNo = logicToViewSeatNo(userSit->bDeskStation);

		CCLOG("BUGBUG GameStatus: %d, LogicalSeat: %d, ViewSeat: %d, PlayerId: %d", _tableInfo.byGameStation,userSit->bDeskStation, vSeatNo, user->dwUserID);
		if (!_tableInfo.bPlayer[userSit->bDeskStation])
		{		
			_uiCallback->showUser(vSeatNo, PlatformLogic()->loginResult.dwUserID == userSit->dwUserID, user->bBoy);
			_tableInfo.bPlayer[userSit->bDeskStation] = true;
		}

		_uiCallback->showUserName(vSeatNo, user->nickName);
		_uiCallback->showUserMoney(vSeatNo, user->i64Money);

		if(userSit->dwUserID == PlatformLogic()->loginResult.dwUserID)
		{
			setTableOffsetStation(userSit->bDeskStation);
			_tableInfo.byMeSeatNo = userSit->bDeskStation;
			sendGameInfo();
		}
	}

	void GameTableLogic::dealUserUpResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user)
	{
		if(_tableInfo.byDeskNo == userSit->bDeskIndex)
		{
			_uiCallback->showUserUp(logicToViewSeatNo(userSit->bDeskStation), userSit->bDeskStation == _tableInfo.byMeSeatNo);
			_tableInfo.bPlayer[userSit->bDeskStation] = false;
			if(userSit->dwUserID == PlatformLogic()->loginResult.dwUserID)
			{
				_tableInfo.byMeSeatNo = INVALID_DESKNO;
				_uiCallback->leaveDesk();
			}
		}
	}

	/************************line*********************************/

	void GameTableLogic::sendStandUp()
	{
		do 
		{
			if (!RoomLogic()->isConnect())
			{
				_uiCallback->leaveDesk();
				break;
			}

			if (INVALID_DESKNO == _tableInfo.byMeSeatNo)
			{
				_uiCallback->leaveDesk();
				break;
			}

			UserInfoStruct* myInfo = _deskUserList->getUserByDeskStation(_tableInfo.byMeSeatNo);
			if(myInfo != nullptr && myInfo->bUserState == USER_PLAY_GAME)
			{
				_uiCallback->showNotice(GBKToUtf8("游戏中，不能离开。"));
				break;
			}
			HNGameLogicBase::sendUserUp();
		} while (0);
	}

	void GameTableLogic::sendUserSit(BYTE byDeskStation)
	{
		BYTE lSeatNo = viewToLogicSeatNo(byDeskStation);
		MSG_GR_S_UserSit UserSit;
		UserSit.bDeskIndex = _tableInfo.byDeskNo;
		UserSit.bDeskStation = lSeatNo;
		RoomLogic()->sendData(MDM_GR_USER_ACTION, ASS_GR_USER_SIT, &UserSit, sizeof(UserSit));
	}

	void GameTableLogic::loadDeskUsersUI()
	{
		BYTE seatNo = INVALID_DESKNO;
		for(int i = 0; i < PLAY_COUNT; i++)
		{
			UserInfoStruct* pUser = _deskUserList->getUserByDeskStation(i);
			if(nullptr != pUser)
			{
				if(pUser->dwUserID == RoomLogic()->loginResult.pUserInfoStruct.dwUserID)
				{
					_tableInfo.byMeSeatNo = i;
				}
				seatNo = logicToViewSeatNo(i);
				_uiCallback->showUser(seatNo, i == _tableInfo.byMeSeatNo, pUser->bBoy);
				_tableInfo.bPlayer[i] = true;

				_uiCallback->showUserName(seatNo,pUser->nickName);
				_uiCallback->showUserMoney(seatNo,pUser->i64Money);
			}
		}

		//玩家在的时候，旋转
		if(_tableInfo.byMeSeatNo != INVALID_DESKNO)
		{
			setTableOffsetStation(_tableInfo.byMeSeatNo);
		}
		else
		{
			_uiCallback->showNotice(GBKToUtf8("买入成功才能进入牌局。"));
		}

		if(_tableInfo.bAutoCreate)
		{
			MSG_GM_S_ClientInfo ClientInfo;
			RoomLogic()->sendData(MDM_GM_GAME_FRAME, ASS_GM_GAME_INFO, (CHAR*)&ClientInfo, sizeof(ClientInfo));
		}

		//显示牌桌信息
		//_uiCallback->showTableInfo(HN::HNRoomInfoModule::getInstance()->getSelectedRoom()->szGameRoomName);
	}

	void GameTableLogic::clearDesk()
	{
		if(_tableInfo.byMeSeatNo != INVALID_DESKNO)
		{
			//_uiCallback->showUserAction(logicToViewSeatNo(_tableInfo.byMeSeatNo), 0);
		}
	}

	LLONG GameTableLogic::getMinBuy()
	{
		//return _tableInfo.tagDeskCfg.dz.iSubMinMoney;
		return 0;
	}

	LLONG GameTableLogic::getMaxBuy()
	{
		//return _tableInfo.tagDeskCfg.dz.iSubMaxMoney;
		return 0;
	}

	LLONG GameTableLogic::getPlanBuy()
	{
		//return _tableInfo.tagDeskCfg.dz.iSubPlanMoney;
		return 0;
	}

	BYTE  GameTableLogic::getMeStation()
	{
		//return _tableInfo.byMeStation;
		return 0;
	}

	void GameTableLogic::sendNote(LLONG note)
	{
		tagUserProcess process;
		process.iVrebType = TYPE_NOTE;
		process.iNote = note;
		process.bMing = _tableInfo.bMing[_tableInfo.byMeSeatNo];
		RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, ASS_VREB_CHECK, &process, sizeof(tagUserProcess));
	}

	void GameTableLogic::sendAdd(LLONG note)
	{
		tagUserProcess process;
		process.iVrebType = TYPE_NOTE;// TYPE_ADD;
		process.iNote = note;
		process.bMing = _tableInfo.bMing[_tableInfo.byMeSeatNo];
		RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, ASS_VREB_CHECK, &process, sizeof(tagUserProcess));
	}

	void GameTableLogic::sendFollow()
	{
		tagUserProcess process;
		process.iVrebType = TYPE_FOLLOW;
		process.iNote = 0;
		process.bMing = _tableInfo.bMing[_tableInfo.byMeSeatNo];
		RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, ASS_VREB_CHECK, &process, sizeof(tagUserProcess));
	}

	void GameTableLogic::sendGiveUp()
	{
		tagUserProcess process;
		process.iVrebType = TYPE_GIVE_UP;
		process.iNote = 0;
		process.bMing = _tableInfo.bMing[_tableInfo.byMeSeatNo];
		RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, ASS_VREB_CHECK, &process, sizeof(tagUserProcess));
	}

	void GameTableLogic::sendLook()
	{
		tagUserProcess process;
		process.iVrebType = TYPE_LOOKCARD;
		process.iNote = 0;
		process.bMing = true;
		RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, ASS_VREB_CHECK, &process, sizeof(tagUserProcess));
	}

	void GameTableLogic::sendCompare(BYTE seatNo)
	{
		tagCompareCard process;
		process.iVrebType = TYPE_COMPARE_CARD;
		process.byComparedStation = viewToLogicSeatNo(seatNo);
		process.bMing = _tableInfo.bMing[_tableInfo.byMeSeatNo];
		process.iNote = 0;
		RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, ASS_VREB_CHECK, &process, sizeof(tagCompareCard));
	}

	void GameTableLogic::compareCardReq()
	{
		std::vector<bool> seats(PLAY_COUNT);
		for(int i = 0; i < PLAY_COUNT; i++)
		{
			UserInfoStruct* userInfo = _deskUserList->getUserByDeskStation(i);
			if(userInfo != nullptr && _tableInfo.bPlayer[i] && !_tableInfo.bIsGiveUp[i] && playingGame(userInfo->bUserState) && i != _tableInfo.byMeSeatNo)
			{
				seats[logicToViewSeatNo(i)] = true;
			}
			else
			{
				seats[logicToViewSeatNo(i)] = false;
			}
		}
		_uiCallback->showCompareOption(seats);
	}

	void GameTableLogic::sendUserReady()
	{
		if(_tableInfo.byMeSeatNo == INVALID_DESKNO || _tableInfo.byGameStation == GS_PLAY_GAME)
		{
			return;
		}

		_uiCallback->showNotice(GBKToUtf8("点击开始进入下一局。"));
		_uiCallback->showReady(true);
	}

	//顺时针+，逆时针-
	void GameTableLogic::setTableOffsetStation(BYTE bDeskStation)
	{
		int oldOffset = _tableInfo.iOffset;
		_tableInfo.iOffset = -bDeskStation;
		int rotate = _tableInfo.iOffset - oldOffset;
		if(rotate > 3)
		{
			rotate = PLAY_COUNT - rotate;
		}
		else if(rotate < -3)
		{
			rotate = PLAY_COUNT + rotate;
		}

		_uiCallback->rotateSeat(rotate);
	}

	bool GameTableLogic::playingGame(int state)
	{
		return (state > GS_WAIT_ARGEE && state <= GS_PLAY_GAME);
	}
	/************************line*********************************/

	void GameTableLogic::initParams()
	{
		_tableInfo.byMeSeatNo = INVALID_DESKNO;
		_tableInfo.iOffset = 0;
		_tableInfo.byWaitTime = 0;
		memset(_tableInfo.bPlayer, 0, sizeof(_tableInfo.bPlayer));
	}

	void GameTableLogic::refreshParams()
	{
		_tableInfo.byTimeOutCount = 0;
		_tableInfo.iFrontNote = 0;
		_tableInfo.iTotalNote = 0;
		_tableInfo.iNoteTimes = 0;

		memset(_tableInfo.iLastNote, 0, sizeof(_tableInfo.iLastNote));
		memset(_tableInfo.iPlayerNote, 0, sizeof(_tableInfo.iPlayerNote));
		memset(_tableInfo.byWin, 0, sizeof(_tableInfo.byWin));
		memset(_tableInfo.iUserCardCount, 0, sizeof(_tableInfo.iUserCardCount));
		memset(_tableInfo.bIsGiveUp, 0, sizeof(_tableInfo.bIsGiveUp));
		memset(_tableInfo.bMing, 0, sizeof(_tableInfo.bMing));
		for(int i = 0; i < PLAY_COUNT; i++)
		{
			memset(_tableInfo.iUserCard[i], 0, sizeof(_tableInfo.iUserCard[i]));
		}
	}

	void GameTableLogic::stopAllWait()
	{
		for(int i = 0; i < PLAY_COUNT; i++)
		{
			BYTE vSeatNo = logicToViewSeatNo(i);
			_uiCallback->showWaitTime(vSeatNo, 0 ,0);
		}
	}

	void GameTableLogic::initGuoDi()
	{
		_tableInfo.iTotalNote = 0;
		for(int i = 0; i < PLAY_COUNT; i++)
		{
			UserInfoStruct* pUser = _deskUserList->getUserByDeskStation(i);
			if(pUser != nullptr && playingGame(pUser->bUserState))
			{
				_tableInfo.iLastNote[i] = _tableInfo.iGuoDi;
				_tableInfo.iPlayerNote[i] = _tableInfo.iGuoDi;
				_tableInfo.iTotalNote += _tableInfo.iGuoDi;

				_uiCallback->showUserNoteMoney(logicToViewSeatNo(i), _tableInfo.iGuoDi);
			}
		}
	}

	//以下变量每一局开始都会清理
	void GameTableLogic::initData()
	{
		//_tableInfo.bIsOffline = false;

		//// 记录下注玩家
		////m_iBetCounts = 0;

		//// 初始化累加金额
		//_tableInfo.nCurrentMoney = 0;
		////m_nCurrentMoney = 0;

		//// 跟注金币
		//_tableInfo.nCallMoney = 0;
		////m_nCallMoney = 0;

		////庄家位置
		//_tableInfo.byNTUser = INVALID_DESKNO;
		////m_byNTUser = 255;

		////小盲注位置
		//_tableInfo.bySmallBlind = INVALID_DESKNO;
		////m_bySmallBlind = 255;

		////大盲注位置
		//_tableInfo.byBigBlind = INVALID_DESKNO;
		////m_byBigBlind = 255;

		////每个用户可用金币
		////memset(m_nMoney, 0, sizeof(m_nMoney));

		//// 全下玩家
		//memset(_tableInfo.bAllIn, 0, sizeof(_tableInfo.bAllIn));

		//// 弃牌玩家
		//memset(_tableInfo.bFold, 0, sizeof(_tableInfo.bFold));

		//// 托管标志
		//memset(_tableInfo.bTrusteeship, 0, sizeof(_tableInfo.bTrusteeship));

		//// 上一次操作类型
		//memset(_tableInfo.emLastOperation, ET_UNKNOWN,sizeof(_tableInfo.emLastOperation));

		////底牌数
		//_tableInfo.iHandCardCounts = 0;

		////底牌数据
		//memset(_tableInfo.byHandCard, 0, sizeof(_tableInfo.byHandCard));

		////公共牌
		//_tableInfo.iCommunityCardCounts = 0;
		//memset(_tableInfo.byCommunityCard, 0, sizeof(_tableInfo.byCommunityCard));

		//// 每个玩家下注金币
		//memset(_tableInfo.nBetMoney, 0, sizeof(_tableInfo.nBetMoney));

		//// 清空栈
		//while (!_tableInfo.stackLastStation.empty())
		//{
		//	_tableInfo.stackLastStation.pop();   // 移除栈顶无素
		//}
	}

	void GameTableLogic::saveTableInfo(void* object)
	{
		//TGSBase* pGSBase = (TGSBase*)object;

		//memcpy(&_tableInfo.tagDeskCfg,&pGSBase->tagDeskCfg,sizeof(TDeskCfg));
		//memcpy(&_tableInfo.bHaveUser,pGSBase->bHaveUser,sizeof(_tableInfo.bHaveUser));
	}


	void GameTableLogic::showUserInfo()
	{
		for(int i = 0; i < PLAY_COUNT; i++)
		{
			if(_tableInfo.bPlayer[i])
			{
				BYTE vSeatNo = logicToViewSeatNo(i);
				UserInfoStruct* pUser = _deskUserList->getUserByDeskStation(i);
				if(pUser != nullptr)
				{
					_uiCallback->showUserName(vSeatNo, pUser->nickName);
					_uiCallback->showUserMoney(vSeatNo, pUser->i64Money);
				}
			}
		}
	}

	// 显示玩家筹码
	// pSubMoney：牌桌内所有玩家的筹码
	//void GameTableLogic::showUserMoney(const TSubMoney* pSubMoney)
	//{
	//	memcpy(&_tableInfo.iMoneys, pSubMoney->nMoney, sizeof(_tableInfo.iMoneys));	
	//		
	//// 	if (pSubMoney->bDeskStation == _tableInfo.byMeStation && !_tableInfo.bWatch)
	//// 	{
	//// 		_tableInfo.bAutoStartTimer = true;
	//// 	}
	//
	//	UserInfoStruct* userInfo = nullptr;
	//	for (int i = 0; i < PLAY_COUNT; i++)
	//	{
	//		userInfo = _deskUserList->getUserByDeskStation(i);
	//		if(nullptr != userInfo && pSubMoney->bReady[i])
	//		{
	//			BYTE byUser = logicToViewSeatNo(i);
	//			_uiCallback->showUserMoney(byUser,_tableInfo.iMoneys[i]);
	//		}
	//	}
	//}

	// 显示带入金额弹窗
	// min:最小带入
	// max:最大带入
	// plan:计划带入
	void GameTableLogic::showBuyMoney(LLONG min, LLONG max, LLONG plan)
	{

	}

	BYTE GameTableLogic::getNextUserStation(BYTE byStation)
	{
		//int nCount = 0;
		//BYTE nextSeatNo = byStation;
		//UserInfoStruct* userInfo = nullptr;

		//// 绕过无效玩家与已放弃的玩家
		//do 
		//{
		//	nextSeatNo = (nextSeatNo + 1 + PLAY_COUNT) % PLAY_COUNT;
		//	userInfo = _deskUserList->getUserByDeskStation(nextSeatNo);
		//	nCount++;
		//	if(nCount >= PLAY_COUNT)
		//	{
		//		return byStation;
		//	}
		//} while (userInfo == nullptr || _tableInfo.bFold[nextSeatNo] || _tableInfo.iUserStation != TYPE_PLAY_GAME);

		return byStation;
	}

	BYTE GameTableLogic::getLastUserStation(BYTE byStation)
	{
		//int nCount = 0;
		//BYTE lastSeatNo = byStation;
		//UserInfoStruct* userInfo = nullptr;

		//// 绕过无效玩家与已放弃的玩家
		//do 
		//{
		//	lastSeatNo = (lastSeatNo - 1 + PLAY_COUNT) % PLAY_COUNT;
		//	userInfo = _deskUserList->getUserByDeskStation(lastSeatNo);
		//	nCount++;
		//	if(nCount >= PLAY_COUNT)
		//	{
		//		return byStation;
		//	}
		//} while (userInfo == nullptr || _tableInfo.bFold[lastSeatNo] || _tableInfo.iUserStation != TYPE_PLAY_GAME);

		return byStation;
	}

	BYTE GameTableLogic::viewToLogicSeatNo(BYTE seatNO)
	{
		return (seatNO - _tableInfo.iOffset + PLAY_COUNT) % PLAY_COUNT;
	}

	BYTE GameTableLogic::logicToViewSeatNo(BYTE seatNO)
	{
		return (seatNO + _tableInfo.iOffset + PLAY_COUNT) % PLAY_COUNT;
	}
}