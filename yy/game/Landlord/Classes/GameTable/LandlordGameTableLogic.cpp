#include "LandlordGameTableLogic.h"
#include "LandlordGameTableUI.h"
#include "HNNetExport.h"

using namespace HN;

namespace landlord
{

#define ENABLE_MING_PAI

/************************line*********************************/
GameTableLogic::GameTableLogic(GameTableUICallback* uiCallback, BYTE deskNo, bool bAutoCreate)
	: _uiCallback(uiCallback)
	, HNGameLogicBase(deskNo, PLAY_COUNT, bAutoCreate, uiCallback)
{
	initParams();
}

GameTableLogic::~GameTableLogic()
{

}

/************************line*********************************/
void GameTableLogic::dealGameMessage(NetMessageHead* messageHead, void* object, INT objectSize)
{
	switch(messageHead->bAssistantID)
	{
	case ASS_GAME_BEGIN:
		HNLOG("dealGameBeginResp");
		dealGameBeginResp(object, objectSize);
		break;
	case ASS_SEND_ALL_CARD:
		HNLOG("dealSendAllCardResp");
		dealSendAllCardResp(object, objectSize);
		break;
	case ASS_SEND_FINISH:
		HNLOG("dealSendFinishResp");
		dealSendFinishResp(object, objectSize);
		break;
	case ASS_CALL_SCORE:
		HNLOG("dealCallScoreResp");
		dealCallScoreResp(object, objectSize);
		break;
	case ASS_CALL_SCORE_RESULT:
		HNLOG("dealCallScoreResultResp");
		dealCallScoreResultResp(object, objectSize);
		break;
	case ASS_CALL_SCORE_FINISH:
		HNLOG("dealCallScoreFinishResp");
		dealCallScoreFinishResp(object, objectSize);
		break;
	case ASS_ROB_NT:
		HNLOG("dealRobNTResp");
		dealRobNTResp(object, objectSize);
		break;
	case ASS_ROB_NT_RESULT:
		HNLOG("dealRobNTResultResp");
		dealRobNTResultResp(object, objectSize);
		break;
	case ASS_ROB_NT_FINISH:
		HNLOG("dealRobNTFinishResp");
		dealRobNTFinishResp(object, objectSize);
		break;
	case ASS_BACK_CARD_EX:
		HNLOG("dealBackCardExResp");
		dealBackCardExResp(object, objectSize);
		break;
	case ASS_ADD_DOUBLE:
		HNLOG("dealAddDoubleResp");
		dealAddDoubleResp(object, objectSize);
		break;
	case ASS_ADD_DOUBLE_RESULT:
		HNLOG("dealAddDoubleResultResp");
		dealAddDoubleResultResp(object, objectSize);
		break;
	case ASS_ADD_DOUBLE_FINISH:
		HNLOG("dealAddDoubleFinishResp");
		dealAddDoubleFinishResp(object, objectSize);
		break;
	case ASS_SHOW_CARD:
		HNLOG("dealShowCardResp");
		dealShowCardResp(object, objectSize);
		break;
	case ASS_SHOW_CARD_RESULT:
		HNLOG("dealShowCardResultResp");
		dealShowCardResultResp(object, objectSize);
		break;
	case ASS_SHOW_CARD_FINISH:
		HNLOG("dealShowCardFinishResp");
		dealShowCardFinishResp(object, objectSize);
		break;
	case ASS_GAME_PLAY:
		HNLOG("dealGamePlayResp");
		dealGamePlayResp(object, objectSize);
		break;
	case ASS_OUT_CARD_RESULT:
		HNLOG("dealOutCardResultResp");
		dealOutCardResultResp(object, objectSize);
		break;
	case ASS_OUT_CARD:
		HNLOG("dealOutCardResp");
		dealOutCardResp(object, objectSize);
		break;
	case ASS_ONE_TURN_OVER:
		HNLOG("dealOneTurnOverResp");
		dealOneTurnOverResp(object, objectSize);
		break;
	case ASS_NEW_TURN:
		HNLOG("dealNewTurnResp");
		dealNewTurnResp(object, objectSize);
		break;
	case ASS_AWARD_POINT:
		HNLOG("dealAwardPointResp");
		dealAwardPointResp(object, objectSize);
		break;
	case ASS_NO_CONTINUE_END:
	case ASS_CONTINUE_END:
		HNLOG("dealContinueEndResp");
		dealContinueEndResp(object, objectSize);
		break;
	case ASS_NO_CALL_SCORE_END:
		HNLOG("dealNoCallScoreEndResp");
		dealNoCallScoreEndResp(object, objectSize);
		break;
	case ASS_CUT_END:
	case ASS_SAFE_END:
		HNLOG("dealSafeEndResp");
		dealSafeEndResp(object, objectSize);
		break;
	case ASS_TERMINATE_END:
	case ASS_AHEAD_END:
		HNLOG("dealNotSafeEndResp");
		dealNotSafeEndResp(object, objectSize);
		break;
	case ASS_AUTO:
		HNLOG("dealAutoResp");
		dealAutoResp(object, objectSize);
		break;
	default:
		HNLOG("unknow game command.");
		break;
	}
}

/*
 * just as you see, it's game message below
 */	
void GameTableLogic::dealGameBeginResp(void* object, INT objectSize)
{
	// 校验数据包大小
	CHECK_SOCKET_DATA(GameBeginStruct, objectSize, "GameBeginStruct size of error!");

	clearDesk();

	_gameStatus = GS_SEND_CARD;

	GameBeginStruct* pData = (GameBeginStruct*)object;

	_multipleInfo.IniData(_baseMultiple);
	std::copy(pData->byUserMingBase, pData->byUserMingBase + PLAY_COUNT, _multipleInfo.sMingPaiMutiple);
	showMultiple();

	for(int i = 0; i < PLAY_COUNT; i++)
	{
		if(_existPlayer[i]/* && _userReady[i]*/)
		{
			BYTE seatNo = logicToViewSeatNo(i);
			_uiCallback->hideUserStatus(seatNo);
		}
	}
}

void GameTableLogic::dealSendAllCardResp(void* object, INT objectSize)
{
	// 校验数据包大小
	CHECK_SOCKET_DATA(SendAllStruct, objectSize, "SendAllStruct size of error!");

	//logic.
	SendAllStruct* pData = (SendAllStruct*)object;
	//再次移除等待动画
	_uiCallback->removeContestAnimate();

	int startIndex = 0;
	for(int i = 0; i < PLAY_COUNT; i++)
	{
		_userCardCount[i] = pData->iUserCardCount[i];
		std::copy(pData->iUserCardList + startIndex, pData->iUserCardList + startIndex +pData->iUserCardCount[i], _userCardList[i]);
		startIndex += pData->iUserCardCount[i];
		_gameLogic.SortCard(_userCardList[i], nullptr, _userCardCount[i]);
	}

	//view.
	for(int i = 0; i < PLAY_COUNT; i++)
	{
		if(!_existPlayer[i])
		{
			continue;
		}

		BYTE seatNo = logicToViewSeatNo(i);

		std::vector<BYTE> cards(_userCardCount[i]);
		std::copy(_userCardList[i], _userCardList[i] + _userCardCount[i], cards.begin());

		if(isFrontCard(cards))
		{
			_uiCallback->showUserHandCard(seatNo, cards, false, false);
		}
		
		if(i != _mySeatNo)
		{
			_uiCallback->setUserHandCardCount(seatNo, _userCardCount[i]);
			_uiCallback->showUserHandCardCount(seatNo, true);
			_uiCallback->showUserMoney(seatNo, true);
		}
	}
}

void GameTableLogic::dealSendFinishResp(void* object, INT objectSize)
{
	// 校验数据包大小
	CHECK_SOCKET_DATA(SendCardFinishStruct, objectSize, "SendCardFinishStruct size of error!");

	_gameStatus = GS_WAIT_BACK;
	SendCardFinishStruct* pData = (SendCardFinishStruct*)object;
	
	std::vector<BYTE> cards;
	_uiCallback->showLandlordCard(true, cards);
}

void GameTableLogic::dealCallScoreResp(void* object, INT objectSize)
{
	// 校验数据包大小
	CHECK_SOCKET_DATA(CallScoreStruct, objectSize, "CallScoreStruct size of error!");

	CallScoreStruct* pData = (CallScoreStruct*)object;
	//show user call score.
	_uiCallback->showUserCallScore(logicToViewSeatNo(pData->bDeskStation));
}

void GameTableLogic::dealCallScoreResultResp(void* object, INT objectSize)
{
	// 校验数据包大小
	CHECK_SOCKET_DATA(CallScoreStruct, objectSize, "CallScoreStruct size of error!");

	CallScoreStruct* pData = (CallScoreStruct*)object;
	_userCallScore[pData->bDeskStation] = pData->iValue;

	//show user call score result.
	_uiCallback->showUserCallScoreResult(logicToViewSeatNo(pData->bDeskStation), pData->iValue);
}

void GameTableLogic::dealCallScoreFinishResp(void* object, INT objectSize)
{
	// 校验数据包大小
	CHECK_SOCKET_DATA(CallScoreStruct, objectSize, "CallScoreStruct size of error!");

	CallScoreStruct* pData = (CallScoreStruct*)object;
}

void GameTableLogic::dealRobNTResp(void* object, INT objectSize)
{
	// 校验数据包大小
	CHECK_SOCKET_DATA(RobNTStruct, objectSize, "RobNTStruct size of error!");

	RobNTStruct* pData = (RobNTStruct*)object;
	
	BYTE seatNo = logicToViewSeatNo(pData->byDeskStation);

	_uiCallback->hideUserStatus(seatNo);
	_uiCallback->setWaitTime(seatNo, _robLordTime, true);
	if(pData->byDeskStation == _mySeatNo)
	{
		if(0 == pData->iValue)
		{
			_uiCallback->showActionButton(SHOW_CALL_LORD | SHOW_NO_CALL_LORD);
		}
		else if(1 == pData->iValue)
		{
			_uiCallback->showActionButton(SHOW_ROB_LORD | SHOW_NO_ROB_LORD);
		}
	}
}

/*
 * RobNTStruct iValue
 * 3: rob NT
 * 2: not rob NT
 * 1: call NT
 * 0: not call NT
 */
void GameTableLogic::dealRobNTResultResp(void* object, INT objectSize)
{
	// 校验数据包大小
	CHECK_SOCKET_DATA(RobNTStruct, objectSize, "RobNTStruct size of error!");

	RobNTStruct* pData = (RobNTStruct*)object;

	BYTE seatNo = logicToViewSeatNo(pData->byDeskStation);
	
	switch (pData->iValue)
	{
	case 3:
		_uiCallback->showUserRobNT(seatNo, true);
		_multipleInfo.sRobNtMutiple[pData->byDeskStation] = pData->byRobCount;
		break;
	case 2:
		_uiCallback->showUserNotRobNT(seatNo, true);
		break;
	case 1:
		_uiCallback->showUserCallNT(seatNo, true);
		break;
	case 0:
		_uiCallback->showUserNotCallNT(seatNo, true);
		break;
	default:
		break;
	}
	_uiCallback->setWaitTime(seatNo, 0, false);
	showMultiple();
}

void GameTableLogic::dealRobNTFinishResp(void* object, INT objectSize)
{
	// 校验数据包大小
	CHECK_SOCKET_DATA(RobNTStruct, objectSize, "RobNTStruct size of error!");

	RobNTStruct* pData = (RobNTStruct*)object;

	_lordSeatNo = pData->byDeskStation;
	
	BYTE seatNo = logicToViewSeatNo(pData->byDeskStation);
	
	_uiCallback->showLandlord(seatNo, true);
	_uiCallback->showActionButton(0);
	for(int i = 0; i < PLAY_COUNT; i++)
	{
		showPlayerHead(i);
	}
}

void GameTableLogic::dealBackCardExResp(void* object, INT objectSize)
{
	// 校验数据包大小
	CHECK_SOCKET_DATA(BackCardExStruct, objectSize, "BackCardExStruct size of error!");

	BackCardExStruct* pData = (BackCardExStruct*)object;	
	
	std::copy(pData->iBackCard, pData->iBackCard + pData->iBackCardCount, 
		_userCardList[pData->iGiveBackPeople] + _userCardCount[pData->iGiveBackPeople]);
	_userCardCount[pData->iGiveBackPeople] += pData->iBackCardCount;

	_gameLogic.SortCard(_userCardList[pData->iGiveBackPeople], nullptr, _userCardCount[pData->iGiveBackPeople]);

	_taskInfo.IniData();
	_taskInfo = pData->gameTask;

	_lordSeatNo = pData->iGiveBackPeople;
	_multipleInfo.sBackCardMutiple = _taskInfo.byBackCardMutiple;
	showMultiple();

	for(int i = 0; i < PLAY_COUNT; i++)
	{
		if(_existPlayer[i])
		{
			_uiCallback->hideUserStatus(logicToViewSeatNo(i));
		}
	}

	BYTE seatNo = logicToViewSeatNo(pData->iGiveBackPeople);

	std::vector<BYTE> cards(pData->iBackCardCount);
	std::copy(pData->iBackCard, pData->iBackCard + pData->iBackCardCount, cards.begin());
	
	if (cards.size() > 0)
	{
		_uiCallback->showLeftUpPNG(_taskInfo);
	}
	_uiCallback->showLandlordCard(true, cards);
	_uiCallback->showBackCardTypeAndMutiple(_taskInfo.byBackCardType, _taskInfo.byBackCardMutiple);
	
	if(pData->iGiveBackPeople != _mySeatNo)
	{
		_uiCallback->setUserHandCardCount(seatNo, _userCardCount[pData->iGiveBackPeople]);
	}
	else
	{
		_uiCallback->showUserHandCard(seatNo, cards, true, true);
	}	
}

void GameTableLogic::dealAddDoubleResp(void* object, INT objectSize)
{
	// 校验数据包大小
	CHECK_SOCKET_DATA(AddDoubleStruct, objectSize, "AddDoubleStruct size of error!");

	AddDoubleStruct* pData = (AddDoubleStruct*)object;
	
	BYTE seatNo = logicToViewSeatNo(pData->bDeskStation);
	_uiCallback->hideUserStatus(seatNo);
	_uiCallback->setWaitTime(seatNo, _doubleTime, true);
	//if(pData->bDeskStation == _mySeatNo)
	if(_mySeatNo != INVALID_DESKSTATION)
	{
		_uiCallback->showActionButton(SHOW_DOUBLE | SHOW_NO_DOUBLE);
	}
}

void GameTableLogic::dealAddDoubleResultResp(void* object, INT objectSize)
{
	// 校验数据包大小
	CHECK_SOCKET_DATA(AddDoubleStruct, objectSize, "AddDoubleStruct size of error!");

	AddDoubleStruct* pData = (AddDoubleStruct*)object;

	_multipleInfo.sAddGameMutiple[pData->bDeskStation] = pData->iValue;
	showMultiple();
	
	BYTE seatNo = logicToViewSeatNo(pData->bDeskStation);
	
	if (pData->iValue > 0)
	{
		_uiCallback->showUserDouble(seatNo, true);
	}
	else
	{
		_uiCallback->showUserNotDouble(seatNo, true);
	}
	
	_uiCallback->setWaitTime(seatNo, 0, false);
}

void GameTableLogic::dealAddDoubleFinishResp(void* object, INT objectSize)
{
	// 校验数据包大小
	CHECK_SOCKET_DATA(AddDoubleStruct, objectSize, "AddDoubleStruct size of error!");

	AddDoubleStruct* pData = (AddDoubleStruct*)object;
	_uiCallback->showActionButton(0);
}

void GameTableLogic::dealShowCardResp(void* object, INT objectSize)
{
	// 校验数据包大小
	CHECK_SOCKET_DATA(ShowCardStruct, objectSize, "AddDoubleStruct size of error!");

	ShowCardStruct* pData = (ShowCardStruct*)object;

#ifdef ENABLE_MING_PAI
	for(int i = 0; i < PLAY_COUNT; i++)
	{
		BYTE seatNo = logicToViewSeatNo(i);
		_uiCallback->hideUserStatus(seatNo);
	}

	if(pData->bDeskStation == _mySeatNo)
	{
		_uiCallback->showActionButton(SHOW_SHOW_CARD);
	}
#endif // !ENABLE_MING_PAI

}

void GameTableLogic::dealShowCardResultResp(void* object, INT objectSize)
{
#ifdef ENABLE_MING_PAI
	// 校验数据包大小
	CHECK_SOCKET_DATA(ShowCardStruct, objectSize, "AddDoubleStruct size of error!");

	ShowCardStruct* pData = (ShowCardStruct*)object;

	if(pData->iValue == 0)
	{
		return;
	}

	_userCardCount[pData->bDeskStation] = pData->iCardCount;

	if(pData->bDeskStation != _mySeatNo)
	{
		std::copy(pData->iCardList, pData->iCardList + pData->iCardCount, _userCardList[pData->bDeskStation]);
	}

	_multipleInfo.sMingPaiMutiple[pData->bDeskStation] = pData->iValue;
	showMultiple();

	BYTE seatNo = logicToViewSeatNo(pData->bDeskStation);

	_uiCallback->showUserPublishCard(seatNo, true);

	if(pData->bDeskStation != _mySeatNo)
	{
		_userCardCount[pData->bDeskStation] = pData->iCardCount;

		std::vector<BYTE> cards(pData->iCardCount);
		std::copy(pData->iCardList, pData->iCardList + pData->iCardCount, cards.begin());

		_uiCallback->showUserHandCard(seatNo, cards, false, true);
	}
#endif // !ENABLE_MING_PAI

}

void GameTableLogic::dealShowCardFinishResp(void* object, INT objectSize)
{
	// 校验数据包大小
	CHECK_SOCKET_DATA(ShowCardStruct, objectSize, "AddDoubleStruct size of error!");

	ShowCardStruct* pData = (ShowCardStruct*)object;
	
	_uiCallback->showActionButton(0);
}

void GameTableLogic::dealGamePlayResp(void* object, INT objectSize)
{
	// 校验数据包大小
	CHECK_SOCKET_DATA(BeginPlayStruct, objectSize, "BeginPlayStruct size of error!");

	BeginPlayStruct* pData = (BeginPlayStruct*)object;
	_outCardSeatNo = pData->iOutDeskStation;

	for(int i = 0; i < PLAY_COUNT; i++)
	{
		BYTE seatNo = logicToViewSeatNo(i);
		_uiCallback->hideUserStatus(seatNo);
	}

	// 是自己出牌
	BYTE seatNo = logicToViewSeatNo(pData->iOutDeskStation);
	_uiCallback->downUserCards(seatNo, pData->iOutDeskStation == _mySeatNo);

	playerOut(pData->iOutDeskStation);
}

void GameTableLogic::dealOutCardResultResp(void* object, INT objectSize)
{
	// 校验数据包大小
	CHECK_SOCKET_DATA(OutCardMsg, objectSize, "OutCardMsg size of error!");

	OutCardMsg* pData = (OutCardMsg*)object;

	memset(_deskCardList[pData->bDeskStation], 0, sizeof(_deskCardList[pData->bDeskStation]));
	_deskCardCount[pData->bDeskStation] = pData->iCardCount;
	std::copy(pData->iCardList, pData->iCardList + pData->iCardCount, _deskCardList[pData->bDeskStation]);

	_outCardSeatNo = INVALID_DESKSTATION;

	if(pData->iCardCount > 0)
	{
		_gameLogic.RemoveCard(pData->iCardList, pData->iCardCount, _userCardList[pData->bDeskStation], _userCardCount[pData->bDeskStation]);
		_userCardCount[pData->bDeskStation] -= pData->iCardCount;

		_gameLogic.SortCard(_userCardList[pData->bDeskStation], nullptr, _userCardCount[pData->bDeskStation]);
		
		_lastOutCardCount = pData->iCardCount;
		std::copy(pData->iCardList, pData->iCardList + pData->iCardCount, _lastOutCardList);
		_gameLogic.SortCard(_lastOutCardList, nullptr, _lastOutCardCount);
	}	

	BYTE seatNo = logicToViewSeatNo(pData->bDeskStation);
	
	_uiCallback->setWaitTime(seatNo, 0, false);

	std::vector<BYTE> cards(pData->iCardCount);
	std::copy(pData->iCardList, pData->iCardList + pData->iCardCount, cards.begin());
	_uiCallback->showUserHandCardOut(seatNo, cards);
	_uiCallback->showUserNotOut(seatNo, cards.empty()/* && (_mySeatNo != pData->bDeskStation)*/);

	_uiCallback->showUserOutCardType(seatNo, _gameLogic.GetCardShape(pData->iCardList, pData->iCardCount));
	_uiCallback->hideOutCardTip();

	if(pData->bDeskStation == _mySeatNo)
	{
		_uiCallback->setOutVisible(false, _lastOutCardCount == 0);
	}
	else
	{
		_uiCallback->setUserHandCardCount(seatNo, _userCardCount[pData->bDeskStation]);
	}
    
    if(pData->iCardCount > 0)
    {
        BYTE cardType = _gameLogic.GetCardShape(pData->iCardList, pData->iCardCount);
        int cardNum = _gameLogic.GetCardNum(pData->iCardList[0]);
        
        switch(cardType)
        {
        case UG_ONLY_ONE:
            _uiCallback->playSingle(seatNo, cardNum);
            break;
        case UG_DOUBLE:
            _uiCallback->playDouble(seatNo, cardNum);
            break;
        case UG_THREE:
            _uiCallback->playThree(seatNo, 0);
            break;
		case UG_THREE_ONE:
			_uiCallback->playThree(seatNo, 1);
			break;
		case UG_THREE_DOUBLE:
			_uiCallback->playThree(seatNo, 2);
			break;
		case UG_STRAIGHT_FLUSH:
		case UG_STRAIGHT:
			_uiCallback->playStraight(seatNo);
			break;
		case UG_DOUBLE_SEQUENCE:
			_uiCallback->playDoubleSequence(seatNo);
			break;
		case UG_THREE_DOUBLE_SEQUENCE:
		case UG_THREE_TWO_SEQUENCE:
		case UG_THREE_ONE_SEQUENCE:
		case UG_THREE_SEQUENCE:
			_uiCallback->playPlan(seatNo);
			break;
		case UG_FOUR_TWO_DOUBLE:
			_uiCallback->playFourTake(seatNo, true);
			break;
		//case UG_FOUR_ONE_DOUBLE:
		case UG_FOUR_TWO:
			_uiCallback->playFourTake(seatNo, false);
			break;
		//case UG_FOUR_ONE:
		//case UG_FOUR_TWO_DOUBLE_SEQUENCE:
		//case UG_FOUR_TWO_SEQUENCE:
		//case UG_FOUR_SEQUENCE:

		case UG_KING_BOMB:
			_uiCallback->playRocket(seatNo);
			break;
		case UG_BOMB:
			_uiCallback->playBomb(seatNo);
        default:
            ;
        }

		if(_userCardCount[pData->bDeskStation] == 1)
		{
			_uiCallback->playCardLast(seatNo, 1);
		}
		else if(_userCardCount[pData->bDeskStation] == 2)
		{
			_uiCallback->playCardLast(seatNo, 2);
		}
    }
    
}

void GameTableLogic::dealOutCardResp(void* object, INT objectSize)
{
	// 校验数据包大小
	CHECK_SOCKET_DATA(OutCardMsg, objectSize, "OutCardMsg size of error!");

	OutCardMsg* pData = (OutCardMsg*)object;
	_outCardSeatNo = pData->iNextDeskStation;

	playerOut(pData->iNextDeskStation);
}

void GameTableLogic::dealOneTurnOverResp(void* object, INT objectSize)
{
	
}

void GameTableLogic::dealNewTurnResp(void* object, INT objectSize)
{
	// 校验数据包大小
	CHECK_SOCKET_DATA(NewTurnStruct, objectSize, "NewTurnStruct size of error!");

	NewTurnStruct* pData = (NewTurnStruct*)object;
	_outCardSeatNo = pData->bDeskStation;
	_lastOutCardCount = 0;
	memset(_lastOutCardList, 0, sizeof(_lastOutCardList));

	memset(_deskCardCount, 0, sizeof(_deskCardCount));
	memset(_deskCardList, 0 , sizeof(_deskCardList));

	playerOut(pData->bDeskStation);
}

void GameTableLogic::dealAwardPointResp(void* object, INT objectSize)
{
	if(objectSize != sizeof(AwardPoINTStruct))	return;
	
	//AwardPoINTStruct* pData = (AwardPoINTStruct*)object;
	_multipleInfo.sBombCount++;
	showMultiple();
}

void GameTableLogic::dealContinueEndResp(void* object, INT objectSize)
{
	// 校验数据包大小
	CHECK_SOCKET_DATA(GameEndStruct, objectSize, "GameEndStruct size of error!");

	_gameStatus = GS_WAIT_NEXT;

	GameEndStruct* pData = (GameEndStruct*)object;

	_multipleInfo = pData->gameMutiple;
	showMultiple();

	std::vector<BYTE> cards;
	for(int i = 0; i < PLAY_COUNT; i++)
	{
		if(!_existPlayer[i])	continue;
		UserInfoStruct* pUser = _deskUserList->getUserByDeskStation(i);
		if(pUser == nullptr)	continue;

		BYTE seatNo = logicToViewSeatNo(i);

		if(pData->iUserCardCount[i] > 0)
		{
			cards.clear();
			cards.resize(pData->iUserCardCount[i]);
			std::copy(pData->iUserCard[i], pData->iUserCard[i] + pData->iUserCardCount[i], cards.begin());
			_uiCallback->showUserHandCard(seatNo, cards, false, true);
		}
	
		if(pData->iChangeMoney[i] != 0)
		{
			_uiCallback->showUserProfit(logicToViewSeatNo(i), pData->iChangeMoney[i]);
		}
        
		char str[64] = {0};

        if(i == _mySeatNo)
        {
            if(pData->iChangeMoney[i] >0)
            {
                _uiCallback->playWin();
            }
            else if(pData->iChangeMoney[i] < 0)
            {
                _uiCallback->playLose();
            }
        }

		showPlayerHead(i);
        
		_uiCallback->showUserHandCardCount(seatNo, false);
		_uiCallback->showUserMoney(seatNo, true);
		
		if (!_bContestRoom)
		{
			sprintf(str, GBKToUtf8("金币：%lld"), pUser->i64Money);
			_uiCallback->setUserMoney(seatNo, str);
			_uiCallback->showUserMoney(seatNo, true);
		}
	}

	if (_bContestRoom)
	{
		_oneTurnEnd = true;
		_uiCallback->playContestAnimate();
	}

	if(_mySeatNo != INVALID_DESKSTATION)
	{
#ifdef ENABLE_MING_PAI
		_uiCallback->showActionButton(SHOW_SHOW_CARD | SHOW_START);
#else
		_uiCallback->showActionButton(SHOW_START);
#endif // !ENABLE_MING_PAI
	}
}

void GameTableLogic::dealNoCallScoreEndResp(void* object, INT objectSize)
{
	// 校验数据包大小
	CHECK_SOCKET_DATA(GameEndStruct, objectSize, "GameEndStruct size of error!");

	GameEndStruct* pData = (GameEndStruct*)object;
	_gameStatus = GS_WAIT_ARGEE;
}

void GameTableLogic::dealSafeEndResp(void* object, INT objectSize)
{
	// 校验数据包大小
	CHECK_SOCKET_DATA(GameCutStruct, objectSize, "GameCutStruct size of error!");

	GameCutStruct* pData = (GameCutStruct*)object;
	_gameStatus = GS_WAIT_ARGEE;
}

void GameTableLogic::dealNotSafeEndResp(void* object, INT objectSize)
{
	// 校验数据包大小
	CHECK_SOCKET_DATA(GameCutStruct, objectSize, "GameCutStruct size of error!");

	GameCutStruct* pData = (GameCutStruct*)object;
	_gameStatus = GS_WAIT_ARGEE;
}

void GameTableLogic::dealAutoResp(void* object, INT objectSize)
{
	// 校验数据包大小
	CHECK_SOCKET_DATA(AutoStruct, objectSize, "AutoStruct size of error!");

	AutoStruct* pData = (AutoStruct*)object;
	_playerAuto[pData->bDeskStation] = pData->bAuto;
	
	showPlayerHead(pData->bDeskStation);
	if(pData->bDeskStation == _mySeatNo)
	{
		_uiCallback->showCancelAutoMask(pData->bAuto);
	}
}

/*
 * just what you see, framework message below.
 */
void GameTableLogic::dealUserAgreeResp(MSG_GR_R_UserAgree* agree)
{
	_userReady[agree->bDeskStation] = (agree->bAgreeGame == 1);
	if (GS_WAIT_NEXT == _gameStatus)
	{
		_gameStatus = GS_WAIT_ARGEE;
		clearDesk();
	}	

	if (1 == agree->bAgreeGame)
	{
		BYTE seatNo = logicToViewSeatNo(agree->bDeskStation);
		
		if(agree->bDeskStation == _mySeatNo && !_bContestRoom)
		{
			_uiCallback->showUserReady(seatNo, true);
			_uiCallback->setWaitTime(seatNo, 0, false);
		}
	}
}

void GameTableLogic::dealGameStartResp(BYTE bDeskNO)
{
	//TODO
	//_uiCallback->clearDesk();
}

void GameTableLogic::dealGameEndResp(BYTE deskNo)
{
	HNGameLogicBase::dealGameEndResp(deskNo);
	//TODO
}

void GameTableLogic::dealUserSitResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user)
{
	//比赛移除等待动画
	_uiCallback->removeContestAnimate();
	HNGameLogicBase::dealUserSitResp(userSit, user);
	//TODO
	bool isMe = (user->dwUserID == RoomLogic()->loginResult.pUserInfoStruct.dwUserID);
	if(isMe)
	{
		setSeatOffset(userSit->bDeskStation);
		loadUsers();
		//sendGameInfo();
	}
	else
	{
		if (_playerSitted[userSit->bDeskStation]) return;

		_playerSitted[userSit->bDeskStation] = true;
		BYTE seatNo = logicToViewSeatNo(userSit->bDeskStation);
		_uiCallback->addUser(seatNo, userSit->bDeskStation == _mySeatNo);
		_uiCallback->setUserName(seatNo, user->nickName);
		char str[64] = {0};

		if (_bContestRoom)
		{
			if (seatNo == 0)
			{
				_uiCallback->updateMyRankNum(user->iRankNum);
				sprintf(str, GBKToUtf8("排名：%d/ %d"), user->iRankNum, user->iRemainPeople);
				_uiCallback->setPaiMing(str);

				if (!user->iContestCount)
				{
					user->iContestCount = 1;
				}

				sprintf(str, GBKToUtf8("第 %d 局"), user->iContestCount);
				_uiCallback->setJuShu(str);
			}		
			sprintf(str, GBKToUtf8("积分：%lld"), user->i64ContestScore);
			_uiCallback->setUserMoney(seatNo, str);
			_uiCallback->showUserMoney(seatNo, true);
		}
		else
		{
			sprintf(str, GBKToUtf8("金币：%lld"), user->i64Money);
			_uiCallback->setUserMoney(seatNo, str);
			_uiCallback->showUserMoney(seatNo, true);
		}
		
		_uiCallback->showUserHandCardCount(seatNo, false);
	}	
}

void GameTableLogic::dealQueueUserSitMessage(BYTE deskNo, const std::vector<QUEUE_USER_SIT_RESULT*>& user)
{
	//比赛移除等待动画
	_uiCallback->removeContestAnimate();

	HNGameLogicBase::dealQueueUserSitMessage(deskNo, user);

	if(_mySeatNo != INVALID_DESKSTATION)
	{
		setSeatOffset(_mySeatNo);
		loadUsers();
		sendGameInfo();
	}
}

void GameTableLogic::dealUserUpResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user)
{
	HNGameLogicBase::dealUserUpResp(userSit, user);

	if (_isReadyQueue && user->dwUserID == RoomLogic()->loginResult.pUserInfoStruct.dwUserID)
	{
		RoomLogic()->sendData(MDM_GR_USER_ACTION, ASS_GR_JOIN_QUEUE);
		_isReadyQueue = false;
		return;
	}

	if (RoomLogic()->getRoomRule() & GRR_CONTEST || (RoomLogic()->getRoomRule() & GRR_TIMINGCONTEST))
	{
		char name[64] = {0};
		sprintf(name, "用户 %d 被淘汰", user->dwUserID);
		_uiCallback->showNotice(GBKToUtf8(name));
		clearDesk();
		cleanUserInfo();
		_uiCallback->playContestAnimate();
	}

	if(!_playerSitted[userSit->bDeskStation]) return;
	_playerSitted[userSit->bDeskStation] = false;

	//TODO
	BYTE seatNo = logicToViewSeatNo(userSit->bDeskStation);

	_uiCallback->setUserName(seatNo, "");
	_uiCallback->showUserMoney(seatNo, true);
	_uiCallback->showUserHandCardCount(seatNo, false);
	_uiCallback->setUserMoney(seatNo, "");
	_uiCallback->setUserHandCardCount(seatNo, -1);
	_uiCallback->hideUserStatus(seatNo);
	_uiCallback->removeUser(logicToViewSeatNo(userSit->bDeskStation), userSit->bDeskStation == _mySeatNo);
	if (INVALID_DESKSTATION == _mySeatNo)
	{
		_uiCallback->leaveDesk(false);
	}
}

void GameTableLogic::dealGameInfoResp(MSG_GM_S_GameInfo* pGameInfo)
{
	_uiCallback->showNotice(pGameInfo->szMessage);
}

void GameTableLogic::dealGameStationResp(void* object, INT objectSize)
{
	HNLOG("dealGameStationResp");
	clearDesk();

	switch(_gameStatus)
	{
	case GS_WAIT_SETGAME:
	case GS_WAIT_ARGEE:
		{
			// 效验数据
			CHECK_SOCKET_DATA(GameStation_2, objectSize, "GameStation_2 size is error.");

			GameStation_2* pGameStation = (GameStation_2*)object;
			_beginTime = pGameStation->iBegINTime;
			_thinkTime = pGameStation->iThinkTime;
			_robLordTime = pGameStation->iRobNTTime;
			_callScoreTime = pGameStation->iCallScoreTime;
			_doubleTime = pGameStation->iAddDoubleTime;
			_baseMultiple = pGameStation->iGameMutiple;
			_doubleLimit = pGameStation->iAddDoubleLimit;
			_maxLimit = pGameStation->iGameMaxLimit;
			_multipleInfo.IniData(_baseMultiple);

			std::copy(pGameStation->bUserReady, pGameStation->bUserReady + PLAY_COUNT, _userReady);
			_gameLogic.SetCardShape(pGameStation->iCardShape);

			for(int i = 0; i < PLAY_COUNT; i++)
			{
				if(_userReady[i])
				{
					BYTE seatNo = logicToViewSeatNo(i);
					_uiCallback->showUserReady(seatNo, true);
				}
			}

			if(_mySeatNo != INVALID_DESKSTATION && !_userReady[_mySeatNo])
			{
				_uiCallback->setWaitTime(logicToViewSeatNo(_mySeatNo), _beginTime, true);
#ifdef ENABLE_MING_PAI
				_uiCallback->showActionButton(SHOW_SHOW_CARD | SHOW_START);
#else
				if(RoomLogic()->getRoomRule() & GRR_QUEUE_GAME)
				{

				}
				else
				{
					_uiCallback->showActionButton(SHOW_START);
				}				
#endif // !ENABLE_MING_PAI
			}
			//_uiCallback->setBasePoint(pGameStation->iDeskBasePoINT);
			_uiCallback->setBasePoint(pGameStation->iGameMutiple);
		}
		break;
	case GS_SEND_CARD:
	case GS_WAIT_BACK:
		{
			// 效验数据
			CHECK_SOCKET_DATA(GameStation_3, objectSize, "GameStation_3 size is error.");
			GameStation_3* pGameStation = (GameStation_3*)object;

			_beginTime = pGameStation->iBegINTime;
			_thinkTime = pGameStation->iThinkTime;
			_robLordTime = pGameStation->iRobNTTime;
			_callScoreTime = pGameStation->iCallScoreTime;
			_doubleTime = pGameStation->iAddDoubleTime;
			_baseMultiple = pGameStation->iGameMutiple;
			_doubleLimit = pGameStation->iAddDoubleLimit;
			_maxLimit = pGameStation->iGameMaxLimit;

			//收到发牌消息，隐藏所有玩家的准备按钮
			for (int i = 0; i < PLAY_COUNT; i++)
			{
				if (_existPlayer[i])
				{
					BYTE seatNo = logicToViewSeatNo(i);
					_uiCallback->hideUserStatus(seatNo);
				}
			}

			std::copy(pGameStation->bAuto, pGameStation->bAuto + PLAY_COUNT, _playerAuto);
			if(_playerAuto[_mySeatNo])
			{
				_uiCallback->showCancelAutoMask(true);
			}
			_gameLogic.SetCardShape(pGameStation->iCardShape);

			int iPos = 0;
			for(int i = 0;i < PLAY_COUNT; i ++)
			{
				_userCardCount[i] = pGameStation->iUserCardCount[i] ; 
				std::copy(pGameStation->iUserCardList + iPos, pGameStation->iUserCardList + iPos + _userCardCount[i], _userCardList[i]);

				iPos += _userCardCount[i] ;
				_gameLogic.SortCard(_userCardList[i], nullptr ,_userCardCount[i]) ;
			}

			_backCardCount = pGameStation->iBackCardCount;
			std::copy(pGameStation->iGameBackCard, pGameStation->iGameBackCard + _backCardCount, _backCardList);

			_taskInfo = pGameStation->gameTask;
			_status = pGameStation->iGameFlag;
			_curActionSeatNo = pGameStation->iCurOperator;

			_multipleInfo = pGameStation->gameMutiple;
			showMultiple();
			//_uiCallback->setBasePoint(pGameStation->iDeskBasePoINT);
			_uiCallback->setBasePoint(pGameStation->iGameMutiple);

			_lordSeatNo = pGameStation->iUpGradePeople;
			if(_lordSeatNo != INVALID_DESKSTATION)
			{
				std::vector<BYTE> cards;
				_uiCallback->showLandlordCard(true, cards);
			}

			for(int i = 0; i < PLAY_COUNT; i++)
			{
				if(_userCardCount[i] > 0)
				{
					BYTE seatNo = logicToViewSeatNo(i);
					std::vector<BYTE> cards(_userCardCount[i]);
					std::copy(_userCardList[i], _userCardList[i] + _userCardCount[i], cards.begin());
					if(isFrontCard(cards))
					{
						_uiCallback->showUserHandCard(seatNo, cards, false, true);
					}
					
					_uiCallback->setUserHandCardCount(seatNo, _userCardCount[i]);
					_uiCallback->showUserHandCardCount(seatNo, true);
					_uiCallback->showUserMoney(seatNo ,true);
				}
			}

			switch (pGameStation->iGameFlag)
			{
			case GS_FLAG_ROB_NT:
				{
					std::copy(pGameStation->iRobNT, pGameStation->iRobNT + PLAY_COUNT, _robLord);
					bool bCall = true;
					for(int i = 0; i < PLAY_COUNT; i++)
					{
						if(i != _mySeatNo && _robLord[i] > 0)
						{
							_uiCallback->showUserRobNT(logicToViewSeatNo(i), true);
							bCall = false;
						}
					}
					BYTE seatNo = logicToViewSeatNo(pGameStation->iCurOperator);
					if(_mySeatNo == pGameStation->iCurOperator)
					{
						if(bCall)
						{
							_uiCallback->showActionButton(SHOW_CALL_LORD | SHOW_NO_CALL_LORD);
						}
						else
						{
							_uiCallback->showActionButton(SHOW_ROB_LORD | SHOW_NO_ROB_LORD);
						}
					}
					_uiCallback->setWaitTime(seatNo, _robLordTime, true);
				}
				break;
			case GS_FLAG_ADD_DOUBLE:
				{
					std::copy(pGameStation->iUserDoubleValue, pGameStation->iUserDoubleValue + PLAY_COUNT, _doubleValue);
					if(pGameStation->iCurOperator == _mySeatNo)
					{
						if(pGameStation->iUserDoubleValue[_mySeatNo] == 0)
						{
							_uiCallback->showActionButton(SHOW_DOUBLE | SHOW_NO_DOUBLE);
						}
					}
					
					_uiCallback->setWaitTime(logicToViewSeatNo(pGameStation->iCurOperator), _doubleTime, true);
				}
				break;
			case GS_FLAG_SHOW_CARD:
				{
					_uiCallback->setWaitTime(logicToViewSeatNo(pGameStation->iCurOperator), _thinkTime, true);
				}
				break;
			default:
				break;
			}

			
		}
		
		break;
	case GS_PLAY_GAME:
		{
			// 效验数据
			CHECK_SOCKET_DATA(GameStation_4, objectSize, "GameStation_4 size is error.");
			GameStation_4* pGameStation = (GameStation_4*)object;

			_beginTime = pGameStation->iBegINTime;
			_thinkTime = pGameStation->iThinkTime;
			_robLordTime = pGameStation->iRobNTTime;
			_callScoreTime = pGameStation->iCallScoreTime;
			_doubleTime = pGameStation->iAddDoubleTime;
			_baseMultiple = pGameStation->iGameMutiple;
			_doubleLimit = pGameStation->iAddDoubleLimit;
			_maxLimit = pGameStation->iGameMaxLimit;
			_lastOutCardCount = pGameStation->iBaseOutCount;
			std::copy(pGameStation->iBaseCardList, pGameStation->iBaseCardList + _lastOutCardCount, _lastOutCardList);

			std::copy(pGameStation->iDeskCardCount, pGameStation->iDeskCardCount + PLAY_COUNT, _deskCardCount);
			
			_gameLogic.SetCardShape(pGameStation->iCardShape);
			std::copy(pGameStation->bAuto, pGameStation->bAuto + PLAY_COUNT, _playerAuto);
			if(_playerAuto[_mySeatNo])
			{
				_uiCallback->showCancelAutoMask(true);
			}

			int iPos = 0;
			for(int i = 0;i < PLAY_COUNT; i ++)
			{
				_userCardCount[i] = pGameStation->iUserCardCount[i] ; 
				std::copy(pGameStation->iUserCardList + iPos, pGameStation->iUserCardList + iPos + _userCardCount[i], _userCardList[i]);

				iPos += _userCardCount[i] ;
				_gameLogic.SortCard(_userCardList[i], nullptr ,_userCardCount[i]) ;

				std::copy(pGameStation->iDeskCardList[i],pGameStation->iDeskCardList[i] + pGameStation->iDeskCardCount[i], _deskCardList[i]);
			}

			_backCardCount = pGameStation->iBackCardCount;
			std::copy(pGameStation->iGameBackCard, pGameStation->iGameBackCard + _backCardCount, _backCardList);

			_taskInfo = pGameStation->gameTask;
			_multipleInfo = pGameStation->gameMutiple;
			showMultiple();
			//_uiCallback->setBasePoint(pGameStation->iDeskBasePoINT);
			_uiCallback->setBasePoint(pGameStation->iGameMutiple);

			std::copy(pGameStation->bPass, pGameStation->bPass + PLAY_COUNT, _userPass);
			std::copy(pGameStation->bLastTurnPass, pGameStation->bLastTurnPass + PLAY_COUNT, _lastTurnPass);
			std::copy(pGameStation->iLastCardCount, pGameStation->iLastCardCount + PLAY_COUNT, _lastTurnCardCount);

			for(int i = 0; i < PLAY_COUNT; i++)
			{
				std::copy(pGameStation->iLastOutCard[i], pGameStation->iLastOutCard[i] + pGameStation->iLastCardCount[i], _lastTurnCardList[i]);
			}

			_haveLastTurnData = pGameStation->bIsLastCard;
			_outCardSeatNo = pGameStation->iOutCardPeople;
			_lordSeatNo = pGameStation->iUpGradePeople;
			_baseCardCount = pGameStation->iBaseOutCount;
			std::copy(pGameStation->iBaseCardList, pGameStation->iBaseCardList + PLAY_COUNT, _baseCardList);


			if(pGameStation->iOutCardPeople == _mySeatNo)
			{
				BYTE seatNo = logicToViewSeatNo(pGameStation->iOutCardPeople);
				_uiCallback->setWaitTime(seatNo, 10, true);
				_uiCallback->setOutVisible(true, 0 == _lastOutCardCount);
			}

			std::vector<BYTE> cards(_backCardCount);
			std::copy(_backCardList, _backCardList + _backCardCount, cards.begin());
			_uiCallback->showLandlordCard(true, cards);

			for(int i = 0; i < PLAY_COUNT; i++)
			{
				BYTE seatNo = logicToViewSeatNo(i);

				if(_userCardCount[i] > 0)
				{
					std::vector<BYTE> cards(_userCardCount[i]);
					std::copy(_userCardList[i], _userCardList[i] + _userCardCount[i], cards.begin());	
					if(isFrontCard(cards))
					{
						_uiCallback->showUserHandCard(seatNo, cards, false, false);
					}
					
					_uiCallback->setUserHandCardCount(seatNo, _userCardCount[i]);
					_uiCallback->showUserHandCardCount(seatNo, true);
					_uiCallback->showUserMoney(seatNo ,true);
				}

				if(_userPass[i])
				{
					_uiCallback->showUserNotOut(seatNo, true);
				}
				else
				{					
					if(_deskCardCount[i] > 0)
					{
						std::vector<BYTE> deskCards(_deskCardCount[i]);
						std::copy(_deskCardList[i], _deskCardList[i] + _deskCardCount[i], deskCards.begin());
						_uiCallback->showUserHandCardOut(seatNo, deskCards);
					}
					
				}

				showPlayerHead(i);
			}

			if(_lordSeatNo != INVALID_DESKSTATION)
			{
				_uiCallback->showLandlord(logicToViewSeatNo(_lordSeatNo), true);
			}
		}

		break;
	case GS_WAIT_NEXT:
		{
			// 效验数据
			CHECK_SOCKET_DATA(GameStation_5, objectSize, "GameStation_5 size is error.");
			GameStation_5* pGameStation = (GameStation_5*)object;

			_beginTime = pGameStation->iBegINTime;
			_thinkTime = pGameStation->iThinkTime;
			_robLordTime = pGameStation->iRobNTTime;
			_callScoreTime = pGameStation->iCallScoreTime;
			_doubleTime = pGameStation->iAddDoubleTime;
			_baseMultiple = pGameStation->iGameMutiple;
			_doubleLimit = pGameStation->iAddDoubleLimit;
			_maxLimit = pGameStation->iGameMaxLimit;
			_gameLogic.SetCardShape(pGameStation->iCardShape);
			std::copy(pGameStation->bUserReady, pGameStation->bUserReady + PLAY_COUNT, _userReady);
			_multipleInfo.IniData(pGameStation->iGameMutiple);
			//_uiCallback->setBasePoint(pGameStation->iDeskBasePoINT);
			_uiCallback->setBasePoint(pGameStation->iGameMutiple);

			for(int i = 0; i < PLAY_COUNT; i++)
			{
				if(_userReady[i])
				{
					BYTE seatNo = logicToViewSeatNo(i);
					_uiCallback->showUserReady(seatNo, _userReady[i]);
				}
			}

			if(_mySeatNo != INVALID_DESKSTATION && !_userReady[_mySeatNo])
			{
				_uiCallback->setWaitTime(logicToViewSeatNo(_mySeatNo), _beginTime, true);
#ifdef ENABLE_MING_PAI
				_uiCallback->showActionButton(SHOW_SHOW_CARD | SHOW_START);	
#else
				if(RoomLogic()->getRoomRule() & GRR_QUEUE_GAME)
				{

				}
				else
				{
					_uiCallback->showActionButton(SHOW_START);
				}				
#endif // !ENABLE_MING_PAI
			}
		}
		
		break;
	default:
		;
	}
}

/************************line*********************************/
void GameTableLogic::sendForceQuitPlatform()
{
	if (RoomLogic()->isConnect())
	{
		HNGameLogicBase::sendForceQuit();
	}
}

void GameTableLogic::sendForceQuit()
{
	if (RoomLogic()->isConnect())
	{
		HNGameLogicBase::sendForceQuit();
	}

	if ((!_bContestEnd) && _bContestRoom)
	{
		_uiCallback->contestleaveTableDlg();	
	}
	else
	{
		_uiCallback->leaveDesk(true);
	}
	
}

void GameTableLogic::sendQueue()
{
	_isReadyQueue = true;
	//排队机准备，先发送站起，再开始游戏
	RoomLogic()->sendData(MDM_GR_USER_ACTION, ASS_GR_USER_UP);
}

void GameTableLogic::sendUserUp()
{
	if (RoomLogic()->isConnect())
	{
		HNGameLogicBase::sendUserUp();
	}
	else
	{
		_uiCallback->leaveDesk(false);
	}
}

void GameTableLogic::sendShowStart()
{
	if(INVALID_DESKSTATION == _mySeatNo) return;

	UserMingStruct data;
	data.bDeskStaion = _mySeatNo;
	data.bMing = true;
	data.bStart = true;
	RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, ASS_GM_AGREE_GAME, (void*)&data, sizeof(data));
}

void GameTableLogic::sendOutCard(const std::vector<BYTE> cards)
{
	if(_mySeatNo == INVALID_DESKSTATION)
	{
		return;
	}

	OutCardStruct data;
	data.iCardCount = cards.size();
	for(size_t i = 0; i < cards.size(); i++)
	{
		data.iCardList[i] = cards[i];
	}
	RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, ASS_OUT_CARD, (void*)&data, sizeof(data));
}

void GameTableLogic::sendRobNT(int value)
{
	if(_mySeatNo == INVALID_DESKSTATION)
	{
		return;
	}

	RobNTStruct robnt;
	memset(&robnt,0,sizeof(robnt));

	robnt.byDeskStation =  _mySeatNo;		
	robnt.iValue  = value;	
	RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, ASS_ROB_NT, (void*)&robnt, sizeof(robnt));
}

void GameTableLogic::sendDouble(int value)
{
	if(_mySeatNo == INVALID_DESKSTATION)
	{
		return;
	}

	AddDoubleStruct AddDouble;
	memset(&AddDouble,0,sizeof(AddDouble));

	AddDouble.bDeskStation = _mySeatNo;		
	AddDouble.iValue  = value;	
	RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, ASS_ADD_DOUBLE, (void*)&AddDouble, sizeof(AddDouble));
}

void GameTableLogic::sendShowCard(int value)
{
	if(_mySeatNo == INVALID_DESKSTATION)
	{
		return;
	}

	ShowCardStruct show ;
	memset(&show,0,sizeof(show)) ;
	show.bDeskStation = _mySeatNo ;		
	show.iValue  = value ;	
	RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, ASS_SHOW_CARD, (void*)&show, sizeof(show));
}

bool GameTableLogic::sendAuto(bool bAuto)
{
	if(_mySeatNo == INVALID_DESKSTATION)	return false;

	AutoStruct TrustData ; 
	TrustData.bAuto = bAuto ; 
	TrustData.bDeskStation = _mySeatNo; 
	RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, ASS_AUTO, (void*)&TrustData, sizeof(AutoStruct));
	return true;
}

void GameTableLogic::enterGame()
{
	if (RoomLogic()->getRoomRule() & GRR_CONTEST || (RoomLogic()->getRoomRule() & GRR_TIMINGCONTEST))	// 定时淘汰比赛场
	{
		_bContestRoom = true;
		_uiCallback->addContestUI();
	}
	else if (RoomLogic()->getRoomRule() & GRR_QUEUE_GAME)		// 排队机
	{
		_bContestRoom = false;
	}
	else					// 金币场不扣积分
	{
		_bContestRoom = false;
	}

	if ((INVALID_DESKSTATION == _mySeatNo) && !_autoCreate)
	{
		for(int i = 0; i < PLAY_COUNT; i++)
		{
			if(!_existPlayer[i])
			{
				sendUserSit(logicToViewSeatNo(i));
				break;
			}
		}
	}
	else
	{
		//setSeatOffset(_mySeatNo);
		loadUsers();
				
		if ((INVALID_DESKSTATION != _mySeatNo) && _autoCreate)
		{
			sendGameInfo();
		}
	}
}

void GameTableLogic::loadUsers()
{
	BYTE seatNo = INVALID_DESKNO;
	char str[64] = {0};
	for(int i = 0; i < PLAY_COUNT; i++)
	{
		UserInfoStruct* pUser = _deskUserList->getUserByDeskStation(i);
		if(_existPlayer[i] && pUser != nullptr)
		{
			_playerSitted[i] = true;
			seatNo = logicToViewSeatNo(i);
			_uiCallback->addUser(seatNo, i == _mySeatNo);
			_uiCallback->setUserName(seatNo, pUser->nickName);
			if (_bContestRoom)
			{
				if (seatNo == 0)
				{
					_uiCallback->updateMyRankNum(pUser->iRankNum);
					sprintf(str, GBKToUtf8("排名：%d/ %d"), pUser->iRankNum, pUser->iRemainPeople);
					_uiCallback->setPaiMing(str);
					
					if (!pUser->iContestCount)
					{
						pUser->iContestCount = 1;
					}
						
					sprintf(str, GBKToUtf8("第 %d 局"), pUser->iContestCount);
					_uiCallback->setJuShu(str);
				}	
			
				sprintf(str, GBKToUtf8("积分：%lld"), pUser->i64ContestScore);
				_uiCallback->setUserMoney(seatNo, str);
				_uiCallback->showUserMoney(seatNo, true);
			}
			else
			{
				sprintf(str, GBKToUtf8("金币：%lld"), pUser->i64Money);
				_uiCallback->setUserMoney(seatNo, str);
				_uiCallback->showUserMoney(seatNo, true);
			}
			_uiCallback->showUserHandCardCount(seatNo, false);

			//test
			//_uiCallback->showUserMoney(seatNo, 1000);
		}
	}
}

void GameTableLogic::clearDesk()
{
	HNGameLogicBase::clearDesk();

	refreshParams();

	if(_mySeatNo != INVALID_DESKNO)
	{
		//_uiCallback->showUserAction(logicToViewSeatNo(_tableInfo.byMeSeatNo), 0);
	}

	//clear view.
	
	_uiCallback->clearDesk(_oneTurnEnd);
	for(int i = 0; i < PLAY_COUNT; i++)
	{
		BYTE seatNo = logicToViewSeatNo(i);
		_uiCallback->showLandlord(seatNo, false);
		
		if (!_bContestRoom)
		{
			showPlayerHead(i);
		}

		_uiCallback->setWaitTime(seatNo, 0, false);
	}
}

bool GameTableLogic::playing()
{
	return (_gameStatus == GS_SEND_CARD)
			|| (_gameStatus == GS_WAIT_BACK)
			|| (_gameStatus == GS_PLAY_GAME);
}

BYTE GameTableLogic::getMySeatNo()
{
	return (INVALID_DESKSTATION == _mySeatNo) ? INVALID_DESKSTATION : logicToViewSeatNo(_mySeatNo);
}

bool GameTableLogic::canOutCard(const std::vector<BYTE>& cards)
{	
	BYTE outCardList[54] = {0};
	std::copy(cards.begin(), cards.end(), outCardList);

	return _gameLogic.CanOutCard(outCardList, cards.size(), _lastOutCardList, _lastOutCardCount,
		_userCardList[_outCardSeatNo], _userCardCount[_outCardSeatNo], 0 == _lastOutCardCount);
}

BYTE GameTableLogic::getCardType(const std::vector<BYTE>& cards)
{
	BYTE cardList[54] = {0};
	std::copy(cards.begin(), cards.end(), cardList);
	return _gameLogic.GetCardShape(cardList, cards.size());
}

void GameTableLogic::outCard(const std::vector<BYTE> cards)
{
	BYTE outcardList[54] = {0};
	std::copy(cards.begin(), cards.end(), outcardList);
	if(_gameLogic.CanOutCard(outcardList, cards.size(), _lastOutCardList, _lastOutCardCount,
		_userCardList[_outCardSeatNo], _userCardCount[_outCardSeatNo], 0 == _lastOutCardCount))
	{
		sendOutCard(cards);
		_uiCallback->setOutVisible(false, 0 == _lastOutCardCount);
		_uiCallback->hideOutCardTip();
	}
	else
	{
		_uiCallback->showUserInvalidOut(true);
	}
}

void GameTableLogic::autoOutCheck(BYTE vSeatNo, const std::vector<BYTE>& cards)
{
	BYTE lSeatNo = viewToLogicSeatNo(vSeatNo);

	int  upCardCount = cards.size();
	BYTE upCardList[54] = {0};

	std::copy(cards.begin(), cards.end(), upCardList);

	int  resultCardCount = 0;
	BYTE resultCardList[54] = {0};

	if(upCardCount >= 5 && 0 == _lastOutCardCount)
	{
		_gameLogic.DragCardAutoSetValidCard(upCardList, upCardCount, resultCardList, resultCardCount);
		if(resultCardCount >= 5)
		{
			std::vector<BYTE> cards1(resultCardCount);
			std::copy(resultCardList, resultCardList + resultCardCount, cards1.begin());
			_uiCallback->setUpCardList(vSeatNo, cards1);
		}
		else
		{
			_uiCallback->setUpCardList(vSeatNo, cards);
		}
	}
	else
	{
		if (1 == upCardCount)
		{
			_gameLogic.AITrackOutCard(_userCardList[lSeatNo], _userCardCount[lSeatNo], upCardList, upCardCount,
				_lastOutCardList, _lastOutCardCount, resultCardList, resultCardCount);

			if (resultCardCount > 0 && _gameLogic.CanOutCard(resultCardList, resultCardCount, _lastOutCardList, _lastOutCardCount,
				_userCardList[lSeatNo], _userCardCount[lSeatNo], 0 == _lastOutCardCount))
			{
				std::vector<BYTE> card2(resultCardCount);
				std::copy(resultCardList, resultCardList + resultCardCount, card2.begin());
				_uiCallback->setUpCardList(vSeatNo, card2);
			}
			else
			{
				_uiCallback->setUpCardList(vSeatNo, cards);
			}
		}
		else
		{
			_uiCallback->setUpCardList(vSeatNo, cards);
		}
	}
}

void GameTableLogic::outCheck(BYTE vSeatNo, const std::vector<BYTE>& cards)
{
	int  upCardCount = cards.size();
	BYTE upCardList[54] = {0};
	std::copy(cards.begin(), cards.end(), upCardList);

	int  resultCardCount = 0;
	BYTE resultCardList[54] = {0};

	if (0 == upCardCount)
	{
		_gameLogic.AutoOutCard(_userCardList[_mySeatNo], _userCardCount[_mySeatNo], _lastOutCardList, _lastOutCardCount,
			resultCardList,	resultCardCount, 0 == _lastOutCardCount);
	}
	else
	{
		if(_gameLogic.CanOutCard(upCardList, upCardCount, _lastOutCardList, _lastOutCardCount,
			_userCardList[_mySeatNo], _userCardCount[_mySeatNo],
			0 == _lastOutCardCount))
		{
			_gameLogic.AutoOutCard(_userCardList[_mySeatNo],_userCardCount[_mySeatNo], upCardList, upCardCount,
				resultCardList,	resultCardCount, 0 == _lastOutCardCount);
		}
		// 没有比手中牌更大的, 回归
		if (0 == resultCardCount)
		{
			_gameLogic.AutoOutCard(_userCardList[_mySeatNo], _userCardCount[_mySeatNo], _lastOutCardList, _lastOutCardCount,
				resultCardList,	resultCardCount, 0 == _lastOutCardCount);
		}
	}
	
	// 提示出牌
	if (resultCardCount > 0)
	{
		std::vector<BYTE> cards1(resultCardCount);
		std::copy(resultCardList, resultCardList + resultCardCount, cards1.begin());
		_uiCallback->setUpCardList(vSeatNo, cards1);
	}
	else // 不出
	{
		std::vector<BYTE> values;
		sendOutCard(values);
	}
}

void GameTableLogic::setSeatOffset(BYTE seatNo)
{
	_seatOffset = -seatNo;
}

bool GameTableLogic::playingGame(int state)
{
	return (state > GS_WAIT_ARGEE && state <= GS_PLAY_GAME);
}

void GameTableLogic::playerOut(BYTE seatNo)
{
	BYTE vSeatNo = logicToViewSeatNo(seatNo);
	
	std::vector<BYTE> cards;
	_uiCallback->showUserHandCardOut(vSeatNo, cards);
	_uiCallback->hideOutCardTip();
	for(int i = 0; i < PLAY_COUNT; i++)
	{
		if(_existPlayer[i])
		{
			_uiCallback->hideUserStatus(logicToViewSeatNo(i));
		}
	}

	_uiCallback->setWaitTime(vSeatNo, _thinkTime, true);
	if(seatNo == _mySeatNo)
	{
		_uiCallback->setOutVisible(true, 0 == _lastOutCardCount);
		
		BYTE resultCardList[54] = {0};
		int resultCardCount = 0;
		_gameLogic.AutoOutCard(_userCardList[_mySeatNo], _userCardCount[_mySeatNo], 
			_lastOutCardList, _lastOutCardCount, 
			resultCardList, resultCardCount,
			0 == _lastOutCardCount);

		if(resultCardCount <= 0)
		{
			_uiCallback->showUserCannotOut(true);
		}

		if(_playerAuto[_mySeatNo])
		{
			std::vector<BYTE> cards(resultCardCount);
			std::copy(resultCardList, resultCardList + resultCardCount, cards.begin());
			sendOutCard(cards);
		}
	}
}


bool GameTableLogic::isFrontCard(const std::vector<BYTE>& cards)
{
	for(size_t i = 0; i < cards.size(); i++)
	{
		if(cards[i] == 0x00)
		{
			return false;
		}
	}
	return true;
}

void GameTableLogic::showMultiple()
{
	int iTotleMutiple ; 
	int iNtAddMutiple ; 
	int iMyAddMutiple = _multipleInfo.sAddGameMutiple[_mySeatNo]>0 ?2 :1 ; 

	if(INVALID_DESKSTATION == _lordSeatNo)
	{
		iNtAddMutiple =  1 ;

		iTotleMutiple = _multipleInfo.GetPublicMutiple() *iNtAddMutiple * iMyAddMutiple ; 
	}
	else
	{
		iNtAddMutiple = _multipleInfo.sAddGameMutiple[_lordSeatNo]>0 ?2 :1 ;

		if(_mySeatNo == _lordSeatNo)
		{
			int iCount = MAX(_multipleInfo.sAddGameMutiple[(_lordSeatNo + 1)%PLAY_COUNT]>0 ?2 :1 ,
				_multipleInfo.sAddGameMutiple[(_lordSeatNo + 2)%PLAY_COUNT]>0 ?2 :1);

			iTotleMutiple = _multipleInfo.GetPublicMutiple() *iNtAddMutiple * iCount; 
		}
		else
		{
			iTotleMutiple = _multipleInfo.GetPublicMutiple() * iNtAddMutiple * iMyAddMutiple ; 
		}
	}
	_uiCallback->setMultiple(iTotleMutiple);
}

/*
 * 显示玩家头像逻辑
 * 1.没有玩家
 * 2.有玩家，托管
 * 3.有玩家，没有托管，游戏中(区分地主和农民)
 * 4.有玩家，没有托管，游戏未开始()
 */
void GameTableLogic::showPlayerHead(BYTE lSeatNo)
{
	do 
	{
		//是否没人
		if(!_existPlayer[lSeatNo])
		{
			_uiCallback->setUserHead(logicToViewSeatNo(lSeatNo), true);
			break;
		}

		//是否托管
		if(_playerAuto[lSeatNo])
		{
			_uiCallback->setUserAutoHead(logicToViewSeatNo(lSeatNo));
			break;
		}

		//是否游戏中(有地主、在牌局中、不是游戏结束)
		if(_lordSeatNo != INVALID_DESKSTATION && _userCardCount[lSeatNo] > 0 && _gameStatus != GS_WAIT_NEXT)
		{
			_uiCallback->setUserGameHead(logicToViewSeatNo(lSeatNo), _lordSeatNo == lSeatNo);
		}
		else
		{
			_uiCallback->setUserHead(logicToViewSeatNo(lSeatNo), false);
			//地主
			_uiCallback->showLandlord(logicToViewSeatNo(lSeatNo), false);
		}

		
		
	} while (0);
}

////////////////////////////////////////////////////////////////////
////聊天接口
////////////////////////////////////////////////////////////////////
void GameTableLogic::dealUserChatMessage(MSG_GR_RS_NormalTalk* normalTalk)
{
	auto user = _deskUserList->getUserByUserID(normalTalk->dwSendID);
	BYTE seatNo = logicToViewSeatNo(user->bDeskStation);
	_uiCallback->showUserChatMsg(seatNo, normalTalk->szMessage);
}

//比赛接口

void GameTableLogic::dealGameContestNotic(MSG_GR_I_ContestInfo* contestInfo)
{
	char message[64] = {0};
	sprintf(message, "比赛场报名人数已有%d人，高手在这里！", contestInfo->iContestNum);
	_uiCallback->showNotice(GBKToUtf8(message));
}

void GameTableLogic::dealGameUserContset(MSG_GR_ContestChange* contestChange)
{
	char str[50] = {0};

	for (int i = 0; i < PLAY_COUNT; i++)
	{
		BYTE seatNo = logicToViewSeatNo(i);

		if ((contestChange->dwUserID == PlatformLogic()->loginResult.dwUserID) && (seatNo == 0))
		{
			_uiCallback->updateMyRankNum(contestChange->iRankNum);
			sprintf(str, GBKToUtf8("排名：%d/ %d"), contestChange->iRankNum, contestChange->iRemainPeople);
			_uiCallback->setPaiMing(str);

			sprintf(str, GBKToUtf8("积分：%d"), contestChange->i64ContestScore);
			_uiCallback->setUserMoney(seatNo, str);
			_uiCallback->showUserMoney(seatNo, true);

			continue;
		}
		
		if (seatNo == 0) continue;

		UserInfoStruct* UserInfo = _deskUserList->getUserByDeskStation(i);

		if (nullptr == UserInfo || !_playerSitted[i])			continue;

		if (contestChange->dwUserID != UserInfo->dwUserID)		continue;

		sprintf(str, GBKToUtf8("积分：%d"), contestChange->i64ContestScore);
		_uiCallback->setUserMoney(seatNo, str);
		_uiCallback->showUserMoney(seatNo, true);
		
	}	
	if (_oneTurnEnd)
	{
		clearDesk();
		cleanUserInfo();
		_oneTurnEnd = false;
	}
}

void GameTableLogic::cleanUserInfo()
{
	for (int i = 0; i < PLAY_COUNT; i++)
	{
		BYTE seatNo = logicToViewSeatNo(i);
		UserInfoStruct* UserInfo = _deskUserList->getUserByDeskStation(i);

		if (seatNo == 0)		continue;

		if (nullptr == UserInfo || !_playerSitted[i])
		{
			_uiCallback->setUserName(seatNo, "");
			_uiCallback->setUserMoney(seatNo, "");
			_uiCallback->showUserMoney(seatNo, true);
			_uiCallback->showUserHandCardCount(seatNo, false);
			_uiCallback->setUserHandCardCount(seatNo, -1);
			_uiCallback->hideUserStatus(seatNo);
			_uiCallback->removeUser(seatNo, false);

		}
	}
}

// 比赛淘汰
void GameTableLogic::dealGameContestKick()
{
	_uiCallback->showGameContestKick();
}

// 等待比赛结束
void GameTableLogic::dealGameContestWaitOver()
{
	_uiCallback->showGameContestWaitOver();
}

// 比赛结束
void GameTableLogic::dealGameContestOver(MSG_GR_ContestAward* contestAward)
{
	_bContestEnd = true;
	_uiCallback->showGameContestOver(contestAward);	
}

bool GameTableLogic::getbContestRoom()
{
	return _bContestRoom;
}
/************************line*********************************/

void GameTableLogic::initParams()
{
	_beginTime = 0;
	_thinkTime = 0;
	_robLordTime = 0;
	_callScoreTime = 0;
	_doubleLimit = 0;
	_baseMultiple = 1;
	_doubleLimit = 0;
	_maxLimit = 0;

	//比赛
	_bContestRoom = false;
	_bContestEnd = false;
	_oneTurnEnd = false;

	memset(_userReady, 0 , sizeof(_userReady));
	memset(_playerAuto, 0, sizeof(_playerAuto));
	memset(_playerSitted, 0, sizeof(_playerSitted));

	_multipleInfo.IniData(_baseMultiple);
	_taskInfo.IniData();
}


void GameTableLogic::refreshParams()
{
	HNGameLogicBase::refreshParams();

	_outCardSeatNo = INVALID_DESKSTATION;
	
	memset(_userCallScore, 0, sizeof(_userCallScore));

	_baseCardCount = 0;
	memset(_baseCardList, 0, sizeof(_baseCardList));

	_status = 0;
	_curActionSeatNo = INVALID_DESKSTATION;
	_lordSeatNo = INVALID_DESKSTATION;
	_outCardSeatNo = INVALID_DESKSTATION;

	memset(_robLord, 0, sizeof(_robLord));
	memset(_doubleValue, 0, sizeof(_doubleValue));
	memset(_userPass, 0, sizeof(_userPass));
	memset(_lastTurnPass, 0, sizeof(_lastTurnPass));

	memset(_deskCardCount, 0, sizeof(_deskCardCount));
	memset(_deskCardList, 0, sizeof(_deskCardList));
	
	memset(_lastTurnCardCount, 0, sizeof(_lastTurnCardCount));
	memset(_lastTurnCardList, 0, sizeof(_lastTurnCardList));

	memset(_deskCardCount, 0, sizeof(_deskCardCount));
	memset(_deskCardList, 0 ,sizeof(_deskCardList));

	_haveLastTurnData = false;

	_baseCardCount = 0;
	memset(_baseCardList, 0, sizeof(_baseCardList));

	_lastOutCardCount = 0;
	memset(_lastOutCardList, 0, sizeof(_lastOutCardList));

	memset(_userCardCount, 0 ,sizeof(_userCardCount));
	for(int i = 0; i < PLAY_COUNT; i++)
	{
		memset(_userCardList[i], 0, sizeof(_userCardList[i]));
	}
	_isReadyQueue = false;
}
}
