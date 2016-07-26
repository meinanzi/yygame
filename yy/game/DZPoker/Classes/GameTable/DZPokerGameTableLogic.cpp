#include "DZPokerGameTableLogic.h"
#include "DZPokerGameTableUI.h"
#include "DZPokerUpgradeMessage.h"

namespace DZPoker
{

	/************************分割线*********************************/
	GameTableLogic::GameTableLogic(GameTableUICallback* uiCallback, BYTE bDeskIndex, bool bAutoCreate):
		_uiCallback(uiCallback), HNGameLogicBase(bDeskIndex, PLAY_COUNT, bAutoCreate, uiCallback)
		, _myInfo(nullptr)
	{
		_tableInfo.bDeskIndex = bDeskIndex;
		_tableInfo.bAutoCreate = bAutoCreate;

		_tableInfo.byMeStation = INVALID_DESKNO;
		_tableInfo.iOffsetStation = 0;
		memset(_tableInfo.bHaveUser, 0, sizeof(_tableInfo.bHaveUser));
		_myInfo = UserInfoModule()->findUser(PlatformLogic()->loginResult.dwUserID);
		initData();
	}

	GameTableLogic::~GameTableLogic()
	{

	}

	/************************分割线*********************************/
	void GameTableLogic::dealGameMessage(NetMessageHead* messageHead, void* object, INT objectSize)
	{
		switch(messageHead->bAssistantID)//通知消息
		{
		case ASS_UG_USER_SET:
			HNLOG("dealUserSetResp");
			dealUserSetResp(object,objectSize);
			break;
		case ASS_UG_USER_SET_CHANGE:
			HNLOG("dealUserSetChangeResp");
			dealUserSetChangeResp(object,objectSize);
			break;
		case ASS_GAME_BEGIN://游戏开始
			HNLOG("dealGameBeginResp");
			dealGameBeginResp(object,objectSize);
			break;
		case ASS_SYS_DESK_CFG://收到系统配置桌子的信息包，并处理
			HNLOG("dealSysDeskCfgResp");
			dealSysDeskCfgResp(object,objectSize);
			break;
		case ASS_CFG_DESK://设置底注
			HNLOG("dealCfgDeskResp");
			dealCfgDeskResp(object,objectSize);
			break;
		case ASS_DESK_CFG://收到注额信息包，并处理
			HNLOG("dealDeskCfgResp");
			dealDeskCfgResp(object,objectSize);
			break;
		case ASS_AGREE_DESK_CFG:
			HNLOG("dealAgreeDeskCfgResp");
			dealAgreeDeskCfgResp(object,objectSize);
			break;
		case ASS_BET:
			HNLOG("dealBetResp");
			dealBetResp(object,objectSize);
			break;
		case ASS_SEND_A_CARD://收到服务器发一张牌包，并处理
			HNLOG("dealHandCardResp");
			dealHandCardResp(object,objectSize);
			break;
		case ASS_SEND_3_CARD://收到服务器发一张牌包，并处理
			HNLOG("dealSend3CardResp");
			dealSend3CardResp(object,objectSize);
			break;
		case ASS_SEND_4_5_CARD://收到服务器发一张牌包，并处理
			HNLOG("dealSend4_5CardResp");
			dealSend4_5CardResp(object,objectSize);
			break;
		case ASS_SEND_CARD_FINISH://收到服务器报告所有玩家发牌完成的消息，并处理
			HNLOG("dealSendCardFinishResp");
			dealSendCardFinishResp(object,objectSize);
			break;
		case ASS_BETPOOL_UP://收到服务器边池更新包，并处理
			HNLOG("dealBetPoolUpResp");
			dealBetPoolUpResp(object,objectSize);
			break;
		case ASS_BET_REQ:
			HNLOG("dealBetReqResp");
			dealBetReqResp(object,objectSize);
			break;
		case ASS_BET_INFO://收到玩家下注的消息
			HNLOG("dealBetInfoResp");
			dealBetInfoResp(object,objectSize);
			break;
		case ASS_NEW_USER:
			HNLOG("dealNewUserResp");
			dealNewUserResp(object,objectSize);
			break;
		case ASS_PASS_USER:
			HNLOG("dealPassUserResp");
			dealPassUserResp(object,objectSize);
			break;
		case ASS_SUB_MONEY:
			HNLOG("dealSubMoneyResp");
			dealSubMoneyResp(object,objectSize);
			break;
		case ASS_COMPARE_CARD://比牌消息
			HNLOG("dealCompareCardResp");
			dealCompareCardResp(object,objectSize);
			break;
		case ASS_CALLMANDATE:
			HNLOG("dealCallMandateResp");
			dealCallMandateResp(object,objectSize);
			break;
		case ASS_CHECKMANDATE:
			HNLOG("dealCheckMandateResp");
			dealCheckMandateResp(object,objectSize);
			break;
		case ASS_CALLANYMANDATE:
			HNLOG("dealCallAnyMandateResp");
			dealCallAnyMandateResp(object,objectSize);
			break;
		case ASS_PASSABANDONMANDATE:
			HNLOG("dealPassAbandonMandateResp");
			dealPassAbandonMandateResp(object,objectSize);
			break;
		case ASS_USER_LEFT_DESK:
			HNLOG("dealUserLeftDeskResp");
			dealUserLeftDeskResp(object,objectSize);
			break;
		case ASS_SORT_OUT://系统自动清理片
			HNLOG("dealSortOutResp");
			dealSortOutResp(object,objectSize);
			break;
		case ASS_TOKEN://收到令牌消息，激活用户
			HNLOG("dealTokenResp");
			dealTokenResp(object,objectSize);
			break;
		case ASS_RESULT://收到结算消息包，并处理
			HNLOG("dealResultResp");
			dealResultResp(object,objectSize);
			break;
		case ASS_SHOW_IN_MONEY://收到代入金额信息包，并处理
			HNLOG("dealBuyMoneyResp");
			dealBuyMoneyResp(object,objectSize);
			break;
		case ASS_LOOK_CARD://玩家看牌
			HNLOG("dealLookCardResp");
			dealLookCardResp(object,objectSize);
			break;
		case ASS_CAN_SHOW_CARD://能展示牌 
			HNLOG("dealCanShowCardResp");
			dealCanShowCardResp(object,objectSize);
			break;
		case ASS_SHOW_CARD://展示牌 
			HNLOG("dealShowCardResp");
			dealShowCardResp(object,objectSize);
			break;
		case ASS_NO_PLAYER://没有玩家进行游戏，退出处理
			HNLOG("dealNoPlayerResp");
			dealNoPlayerResp(object,objectSize);
			break;
		case ASS_AGREE://玩家是否同意的游戏的，处理
			HNLOG("dealAgreeResp");
			dealAgreeResp(object,objectSize);
			break;
		case ASS_CFG_DESK_TIMEOUT://设置底注超时 
			HNLOG("dealCfgDeskTimeOutResp");
			dealCfgDeskTimeOutResp(object,objectSize);
			break;
		case ASS_NOT_ENOUGH_MONEY://玩家手上的钱不足够时，处理
			HNLOG("dealNotEnoughMoneyResp");
			dealNotEnoughMoneyResp(object,objectSize);
			break;
		case ASS_MONEY_LACK:
			HNLOG("dealMoneyLackResp");
			dealMoneyLackResp(object,objectSize);
			break;
		case ASS_NEXT_ROUND_REQ:
			HNLOG("dealNextRoundReqResp");
			dealNextRoundReqResp(object,objectSize);
			break;
		case ASS_NEW_ROUND_BEGIN://收到新的一回合开始消息包，并处理
			HNLOG("dealNewRoundBeginResp");
			dealNewRoundBeginResp(object,objectSize);
			break;
		case ASS_FINISH_ROUND_REQ:
			HNLOG("dealFinishRoundReqResp");
			dealFinishRoundReqResp(object,objectSize);
			break;
		default:
			break;
		}
	}

	/************************分割线*********************************/

	/************************************************************************/
	/* 消息处理函数                                                         */
	/************************************************************************/
	void GameTableLogic::dealUserSetResp(void* object, INT objectSize)
	{

	}

	void GameTableLogic::dealCfgDeskResp(void* object, INT objectSize)
	{
		TCfgDesk* data = (TCfgDesk*)object;

		if (objectSize != sizeof(TCfgDesk))
		{
			return;
		}
		//修正游戏的状态
		_tableInfo.bGameStatus = GS_CONFIG_NOTE;
	}

	void GameTableLogic::dealDeskCfgResp(void* object, INT objectSize)
	{
		TDeskCfg* data = (TDeskCfg*)object;

		_tableInfo.bGameStatus = GS_CONFIG_NOTE;
		::memcpy(&_tableInfo.tagDeskCfg,data,sizeof(TDeskCfg));
	}

	void GameTableLogic::dealAgreeResp(void* object, INT objectSize)
	{
		TAgree* data = (TAgree*)object;
	}

	void GameTableLogic::dealCfgDeskTimeOutResp(void* object, INT objectSize)
	{

	}

	void GameTableLogic::dealNotEnoughMoneyResp(void* object, INT objectSize)
	{
		TMoneyLack * pMoneyLack = (TMoneyLack *)object;

		if (_tableInfo.byMeStation == pMoneyLack->nbyUser)
		{
			_uiCallback->showUserLackMoney(logicToViewStation(pMoneyLack->nbyUser));
		}
	}

	void GameTableLogic::dealBetResp(void* object, INT objectSize)
	{
		TBet* data = (TBet*)object;
	}

	void GameTableLogic::dealSubMoneyResp(void* object, INT objectSize)
	{
		TSubMoney* data = (TSubMoney*)object;

	}

	void GameTableLogic::dealCompareCardResp(void* object, INT objectSize)
	{
		showAutoAction(false);

		showHelp(true);

		//错误，散牌，对子，两对，三条，最小顺子，顺子，同花，葫芦，四条，最小同花顺，同花顺，皇家同花顺
		int cardType[12] = 
		{
			0/*散牌*/ ,1/*一对*/, 2/*两对*/, 3/*三条*/, 
			4/*最小顺子*/, 4/*顺子*/, 5/*同花*/, 6/*葫芦*/, 
			7/*四条*/, 8/*最小同花顺*/, 8/*同花顺*/, 9/*皇家同花顺*/
		};

		TCompareCard* pCmd = (TCompareCard*)object;
		for (int i = 0; i < PLAY_COUNT; i++)
		{
			UserInfoStruct* User = _deskUserList->getUserByDeskStation(i);
			if (!User || pCmd->nHandCardCount[i] != 2)
			{
				continue ;
			}

			//显示玩家手上的牌
			std::vector<BYTE> byHandCards;
			for(int j = 0; j < pCmd->nHandCardCount[i]; j++)
			{
				byHandCards.push_back(pCmd->bHandCards[i][j]);
			}
			if(i != _tableInfo.byMeStation)
			{
				_uiCallback->showUserHandCard(logicToViewStation(i),byHandCards);
			}

			//提起手中的牌
			BYTE bCard[2] = {0,0};
			if (i == _tableInfo.byMeStation)
			{
				// 自己手上2张可用的牌弹起
				for (int j = 0; j < 5; j++)
				{
					if (pCmd->bHandCards[i][0] == pCmd->bCards[i][j])
					{
						bCard[0] = pCmd->bCards[i][j];
					}

					if (pCmd->bHandCards[i][1] == pCmd->bCards[i][j])
					{
						bCard[1] = pCmd->bCards[i][j];
					}

					if (bCard[0] && bCard[1])
					{
						break;
					}
				}
			}

			for (int j = 0; j < 2; j++)
			{
				if (bCard[j] != 0x00)
				{
					_uiCallback->upUserHandCard(_tableInfo.byMeStation,j);
				}
			}

			//牌型提示

			//提起公共牌
			if (i == _tableInfo.byMeStation)
			{
				BYTE bCard[5] = {0,0,0,0,0};
				BYTE bCardCount = 0;

				for (int j = 0; j < 5; j++)
				{
					for (int k = 0; k < 5; k++)
					{
						if (_tableInfo.byCommunityCard[k] == pCmd->bCards[i][j])
						{
							bCard[k] = pCmd->bCards[i][j];
							break;
						}
					}
				}

				for (int j = 0; j < 5; j++)
				{
					if (bCard[j] != 0x00)
					{
						_uiCallback->upCommunityCard(j);
					}
				}
			}

			//显示牌型
			if(pCmd->nHandCardCount[i] > 0)
			{
				_uiCallback->showUserCardType(logicToViewStation(i), cardType[pCmd->nCardKind[i]-1]);
			}
		}
	}

	void GameTableLogic::dealHandCardResp(void* object, INT objectSize)
	{
		if(_tableInfo.byNTUser == INVALID_DESKNO)
		{
			return;
		}

		TCards* data = (TCards*)object;
		_tableInfo.nTypeCard = data->nTypeCard;

		::memcpy(_tableInfo.byHandCard, data->byCards, MAX_HANDCARDS_NUM);
		_tableInfo.iHandCardCounts = data->iCardsNum;

		//从庄家位置开始，发牌
		std::vector<THandCard> bDeskStations;
		BYTE byNextUser = _tableInfo.byNTUser;
		do 
		{
			THandCard handCard;
			if(byNextUser == _tableInfo.byMeStation && data->iCardsNum == 2)
			{
				memcpy(handCard.byCards, data->byCards[byNextUser], sizeof(handCard.byCards));
			}
			handCard.byUser = logicToViewStation(byNextUser);

			bDeskStations.push_back(handCard);
			byNextUser = getNextUserStation(byNextUser);
		} while (byNextUser != _tableInfo.byNTUser);

		_uiCallback->showHandCard(bDeskStations);
	}

	void GameTableLogic::dealSend3CardResp(void* object, INT objectSize)
	{
		TCards* data = (TCards*)object;

		memcpy(_tableInfo.byCommunityCard, data->byCards, sizeof(BYTE) * (data->iCardsNum));
		_tableInfo.iCommunityCardCounts += data->iCardsNum;

		// 显示3张公共牌
		if(data->iCardsNum == 3)
		{
			std::vector<BYTE> byCards;
			byCards.push_back(data->byPubCards[0]);
			byCards.push_back(data->byPubCards[1]);
			byCards.push_back(data->byPubCards[2]);
			_uiCallback->showFlopCard(byCards);
		}
	}

	void GameTableLogic::dealSend4_5CardResp(void* object, INT objectSize)
	{
		TCards* data = (TCards*)object;
		memcpy(_tableInfo.byCommunityCard, data->byCards, sizeof(BYTE) * (data->iCardsNum));
		_tableInfo.iCommunityCardCounts = data->iCardsNum;

		// 显示4 - 5 公共牌，公用一个消息
		if(data->iCardsNum == 4)
		{
			_uiCallback->showTurnCard(data->byPubCards[3]);
		}

		if(data->iCardsNum == 5)
		{
			_uiCallback->showRiverCard(data->byPubCards[4]);
		}
	}

	void GameTableLogic::dealSendCardFinishResp(void* object, INT objectSize)
	{

	}

	void GameTableLogic::dealUserLeftDeskResp(void* object, INT objectSize)
	{
		// 	OnShowUserLogo();
		// 
		// 	UserLeftDeskStruct *pLeftData = (UserLeftDeskStruct *)buffer ; 
		// 
		// 	if(pLeftData->bClearLogo)
		// 	{
		// 		BYTE byView = m_pGameFrame->DeskStation2View(pLeftData->bDeskStation);
		// 
		// 		SAFE_CTRL_OPERATE(IImage,DZPK_READY_MAP + byView,SetControlVisible(false));
		// 		SAFE_CTRL_OPERATE(IImage,DZPK_USER_MAP + byView,SetControlVisible(false));
		// 		SAFE_CTRL_OPERATE(IText,DZPK_USER_NAME + byView,SetControlVisible(false));
		// 		SAFE_CTRL_OPERATE(IImage,DZPK_FOLD_LOGO + byView,SetControlVisible(false));
		// 		SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_USER_CHAIR + byView,SetControlVisible(true));	// 显示玩家椅子
		// 
		// 		GameUserLeft(pLeftData->bDeskStation);
		// 
	}

	void GameTableLogic::dealSortOutResp(void* object, INT objectSize)
	{

	}

	void GameTableLogic::dealTokenResp(void* object, INT objectSize)
	{
		TToken* pToken = (TToken*)object;
		if(pToken->byUser == INVALID_DESKNO)
		{
			return;
		}
		_tableInfo.byTokenUser = pToken->byUser;

		//轮到登录玩家操作
		BYTE byMeStation = _tableInfo.byMeStation;
		_uiCallback->changeToken(byMeStation == pToken->byUser, logicToViewStation(pToken->byUser));
		if(byMeStation == pToken->byUser)
		{
			showAutoAction(false);

			//是否进行了托管操作
			if(pToken->bNewTurn)
			{
				// 一轮中第一个操作人, 清空所有操作标志
				memset(_tableInfo.emLastOperation, ET_UNKNOWN, sizeof(_tableInfo.emLastOperation));

				// 清空操作人记录栈
				while (!_tableInfo.stackLastStation.empty())
				{
					_tableInfo.stackLastStation.pop();   // 移除栈顶无素
				}
			}
			else
			{
				// 如果玩家有托管，进行托管操作
				if (doUserAutoAction(object))
				{
					// 自动托管成功
					return ;
				}
			}

			showAction(true, object);
			_tableInfo.nCurrentMoney = 0;

		}
		else
		{
			showAction(false, object);

			//托管操作显示：玩家没有弃牌和全下，而且在当前玩家的前两个位置
			bool bWatch = _tableInfo.bFold[byMeStation] || _tableInfo.bAllIn[byMeStation];
			BYTE byUser1 = getNextUserStation(_tableInfo.byTokenUser);
			BYTE byUser2 = getNextUserStation(byUser1);
			showAutoAction(!bWatch && (byMeStation == byUser1 || byMeStation == byUser2));
		}

		//显示剩余操作时间
		BYTE byUser = logicToViewStation(pToken->byUser);
		_uiCallback->showUserLeftTime(byUser,
			_tableInfo.tagDeskCfg.Time.byOperate - pToken->iPassedTime, 
			_tableInfo.tagDeskCfg.Time.byOperate, 
			pToken->byUser == _tableInfo.byMeStation);
	}

	void GameTableLogic::dealBetPoolUpResp(void* object, INT objectSize)
	{
		TBetPool* data = (TBetPool*)object;
		memcpy(&_tableInfo.tagBetPool, data, sizeof(_tableInfo.tagBetPool));

		if (nullptr == _uiCallback) return;

		for (int i = 0; i < 8; i++)
		{
			if (data->iBetPools[i] > 0)
			{
				_uiCallback->showPot(i);
			}
		}
	}

	void GameTableLogic::dealBetInfoResp(void* object, INT objectSize)
	{
		TBet* pBet = (TBet*)object;

		BYTE vSeatNo = logicToViewStation(pBet->byUser);

		if (pBet->nType == ET_AUTO || pBet->nType == ET_BET || pBet->nType == ET_RAISE || pBet->nType == ET_ALLIN || pBet->nType == ET_CALL)
		{
			_tableInfo.nBetMoney[pBet->byUser] += pBet->nMoney; 
			_tableInfo.iMoneys[pBet->byUser] -= pBet->nMoney;

			_uiCallback->showUserMoney(vSeatNo,_tableInfo.iMoneys[pBet->byUser]);
		}

		// 记录全下与放弃玩家
		if (pBet->nType == ET_ALLIN)
		{
			_tableInfo.bAllIn[pBet->byUser] = true;
		}
		else if (pBet->nType == ET_FOLD)
		{
			_tableInfo.bFold[pBet->byUser] = true;		
		}


		// 记录这个玩家操作类型
		_tableInfo.emLastOperation[pBet->byUser] = pBet->nType;

		// 记录操作玩家
		_tableInfo.stackLastStation.push(pBet->byUser);

		//操作类型操作
		switch (pBet->nType)
		{
		case ET_BET:// 下注
			_uiCallback->showUserBet(vSeatNo);
			break;
		case ET_CALL:// 跟注
			_uiCallback->showUserCall(vSeatNo);
			break;
		case ET_RAISE:// 加注
			_uiCallback->showUserAdd(vSeatNo);
			break;
		case ET_CHECK:// 过牌
			_uiCallback->showUserCheck(vSeatNo);
			break;
		case ET_FOLD:// 弃牌
			_uiCallback->showUserFold(vSeatNo);
			break;
		case ET_ALLIN:// 全下
			_uiCallback->showUserAllIn(vSeatNo);
			break;
		default:
			break;
		}

		if(pBet->nMoney > 0)
		{
			_uiCallback->showUserBetMoney(vSeatNo, _tableInfo.nBetMoney[pBet->byUser]);
		}
		_uiCallback->showUserLeftTime(vSeatNo, 0, 0, pBet->byUser == _tableInfo.byMeStation);
	}

	void GameTableLogic::dealResultResp(void* object, INT objectSize)
	{
		showAction(false, nullptr);
		showAutoAction(false);

		if (_tableInfo.bGameStatus != GS_PLAY_GAME)
		{
			return;
		}

		TResult* pResult = (TResult*)object;

		//记录游戏数据
		::memcpy(&_tableInfo.tagResult,pResult,sizeof(TResult));

		//修正游戏的状态
		_tableInfo.bGameStatus = GS_WAIT_NEXT_ROUND;
		memcpy(_tableInfo.iMoneys, pResult->nSubMoney, sizeof(_tableInfo.iMoneys));

		showUserMoney(nullptr);

		//派奖
		std::vector<std::vector<LLONG>> winPool;
		winPool.resize(PLAY_COUNT);
		for(int i = 0; i < PLAY_COUNT; i++)
		{
			int seatNo = logicToViewStation(i);
			winPool[seatNo].resize(PLAY_COUNT);
			for(int j = 0; j < PLAY_COUNT; j++)
			{
				winPool.at(seatNo).at(j) = pResult->nWinPoolsMoney[i][j];
			}
		}
		_uiCallback->showWinPool(winPool);

		//停止动画
		if(_tableInfo.byTokenUser != INVALID_DESKNO)
		{
			_uiCallback->showUserLeftTime(logicToViewStation(_tableInfo.byTokenUser),
				0,
				_tableInfo.tagDeskCfg.Time.byOperate,
				_tableInfo.byTokenUser == _tableInfo.byMeStation);
		}
		_uiCallback->isWaitTime(true);
	}

	void GameTableLogic::dealBuyMoneyResp(void* object, INT objectSize)
	{
		TSubMoney* pData = (TSubMoney*)object;	

		if(_tableInfo.bHaveUser[pData->bDeskStation])
		{
			_tableInfo.iMoneys[pData->bDeskStation] = pData->nMoney[pData->bDeskStation];
		}

		//_uiCallback->showMyMoney(HNPlatformLogic::getInstance()->loginResult.i64Money);
		_uiCallback->showUserMoney(logicToViewStation(pData->bDeskStation), _tableInfo.iMoneys[pData->bDeskStation]);
	
		if (pData->bDeskStation == _mySeatNo)
		{
			_uiCallback->isWaitTime(false);
		}
	
	}

	void GameTableLogic::dealLookCardResp(void* object, INT objectSize)
	{
		// 	if(sizeof(UserLookCardStruct) != nLen)
		// 	{
		// 		return;
		// 	}
		// 
		// 	UserLookCardStruct *pLookCard = (UserLookCardStruct*)buffer;
		// 
		// 	BYTE byViewStation = m_pGameFrame->DeskStation2View(pLookCard->bDeskStation);	//视图位置
		// 
		// 	//自己的视图位置不播放看牌动画
		// 	if(byViewStation == m_pGameFrame->DeskStation2View(m_pGameFrame->GetMyDeskStation()))
		// 	{
		// 		return;
		// 	}
		// 
		// 	SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_SMALL_CARD+byViewStation,SetControlVisible(false));
		// 	SAFE_CTRL_OPERATE(IImage,DZPK_V1_PAI+byViewStation,SetControlVisible(false));
		// 	SAFE_CTRL_OPERATE(IImage,DZPK_V2_PAI+byViewStation,SetControlVisible(false));
		// 
		// 
		// 	///控件看牌动画
		// 	IExAnimate *pAnima = dynamic_cast<IExAnimate *>(m_pUI->GetIControlbyID(DZPK_LOOK_CARD_ANIMA_0 + byViewStation));
		// 	if(nullptr != pAnima)
		// 	{
		// 		pAnima->SetIsLoop(false);				//不循环
		// 		pAnima->SetPlayTotalTime(400);			//设置播放时间
		// 		pAnima->SetPlayState(true);				//设置播放
		// 		pAnima->SetIsSymmetrical(false);		//设置是否对称
		// 		pAnima->SetControlVisible(true);
		// 
	}

	void GameTableLogic::dealNextRoundReqResp(void* object, INT objectSize)
	{
		TNextRoundReq* data = (TNextRoundReq*)object;
	}

	void GameTableLogic::dealNewRoundBeginResp(void* object, INT objectSize)
	{
		TNextRoundBeginInfo* data = (TNextRoundBeginInfo*)object;

		//记录庄家的位置
		_tableInfo.byNTUser = data->byNTUser;

		//记录大小盲注
		_tableInfo.bySmallBlind = data->bSmallBlind;
		_tableInfo.byBigBlind = data->bBigBlind;

		//托管状态
		_tableInfo.bTuoGuan = false;

		//底牌清空，数量归零
		::memset(_tableInfo.byBackCard,0,MAX_BACKCARDS_NUM);
		_tableInfo.iBackCardNums = 0;

		_uiCallback->showDealer(logicToViewStation(_tableInfo.byNTUser));
		_uiCallback->showSmallBlind(logicToViewStation(_tableInfo.bySmallBlind));
		_uiCallback->showBigBlind(logicToViewStation(_tableInfo.byBigBlind));
	}

	void GameTableLogic::dealShowCardResp(void* object, INT objectSize)
	{
		// 	ShowCardStruct* pShowCard = (ShowCardStruct*)buffer;
		// 
		// 	//安全判断
		// 	if(nLen != sizeof(ShowCardStruct) || 255 == pShowCard->byDeskStation)
		// 	{
		// 		return false;
		// 	}
		// 
		// 	//显示 展示者玩家的 底牌
		// 	INoMoveCard* pCard = dynamic_cast<INoMoveCard*>(m_pUI->GetIControlbyID(DZPK_SHOW_CARD+m_pGameFrame->DeskStation2View(pShowCard->byDeskStation)));
		// 	if(nullptr != pCard)
		// 	{
		// 		pCard->SetCardList(pShowCard->byCardList,pShowCard->iCardCount);
		// 		pCard->SetControlVisible(true);
		// 
	}

	void GameTableLogic::dealNoPlayerResp(void* object, INT objectSize)
	{
		TNoPlayer* data = (TNoPlayer*)object;
	}

	void GameTableLogic::dealMoneyLackResp(void* object, INT objectSize)
	{
		TMoneyLack* data = (TMoneyLack*)object;
	}

	void GameTableLogic::dealGameBeginResp(void* object, INT objectSize)
	{
		_tableInfo.bGameStatus = GS_PLAY_GAME;
		//_tableInfo.bGameStatus = TYPE_PLAY_GAME;

		// 	if(_tableInfo.bWatch)
		// 	{
		// 		initData();
		// 		return;
		// 	}

		_tableInfo.iUserStation = TYPE_PLAY_GAME;
		showUserInfo();
		initData();
	}

	void GameTableLogic::dealSysDeskCfgResp(void* object, INT objectSize)
	{
		TDeskCfg* data = (TDeskCfg*)object;

		::memcpy(&_tableInfo.tagDeskCfg,data,sizeof(TDeskCfg));

		//系统配置桌子数据后的UI处理，主要显示游戏规则
	}

	void GameTableLogic::dealGameInfoResp(MSG_GM_S_GameInfo* pGameInfo)
	{
		HNLOG("onGameInfoMessage");
		_tableInfo.bGameStatus = pGameInfo->bGameStation;
		_tableInfo.bEableWatch = pGameInfo->bWatchOther;
		if(_tableInfo.bAutoCreate)
		{
			_uiCallback->showTableReady(false);
		}
		else
		{
			_uiCallback->noticeMessage(GBKToUtf8(pGameInfo->szMessage));
		}
	}

	void GameTableLogic::dealGameStationResp(void* object, INT objectSize)
	{
		UserInfoStruct* myInfo = _deskUserList->getUserByUserID(PlatformLogic()->loginResult.dwUserID);
		_tableInfo.byMeStation = (myInfo) ? myInfo->bDeskStation : INVALID_DESKNO;

		TGSBase* data = (TGSBase*)object;

		//备份牌桌信息
		saveTableInfo(object);

		//显示玩家信息
		showUserInfo();

		_tableInfo.bGameStatus = data->bGameStatus;

		if (_tableInfo.bGameStatus == GS_WAIT_ARGEE || GS_WAIT_SETGAME)
		{
			_uiCallback->isWaitTime(true);
		}

		//根据牌桌状态信息，更新界面
		switch (_tableInfo.bGameStatus)
		{
		case GS_WAIT_SETGAME://等待东家设置状态
		case GS_WAIT_ARGEE://等待同意设置
		case GS_WAIT_NEXT_ROUND://等待下一回合结束状态
			{
				TGSWaitAgree* pGSWaitAgree = (TGSWaitAgree*)data;
				TSubMoney cmd;
				cmd.bDeskStation = -1;
				memcpy(cmd.nMoney,pGSWaitAgree->nSubMoney,sizeof(cmd.nMoney));
				memcpy(cmd.bReady,pGSWaitAgree->bReady,sizeof(cmd.bReady));

				//更新带入金币信息
				showUserMoney(&cmd);

				//弹出带入金额框，要用到下面的信息
				showBuyMoney(pGSWaitAgree->tagDeskCfg.dz.iSubMinMoney,
					pGSWaitAgree->tagDeskCfg.dz.iSubMaxMoney,
					pGSWaitAgree->tagDeskCfg.dz.iSubPlanMoney);
			}
			break;
		case GS_PLAY_GAME://游戏过程状态
			{
				_tableInfo.bIsOffline = true;

				TGSPlaying * pGSPlaying = (TGSPlaying *)object;
				// 获得得到令牌的玩家
				_tableInfo.byTokenUser = pGSPlaying->byTokenUser;

				// 庄家大小肓
				_tableInfo.byNTUser = pGSPlaying->byNTUser;
				_tableInfo.bySmallBlind = pGSPlaying->bySmallBlind;
				_tableInfo.byBigBlind = pGSPlaying->byBigBlind;
				_tableInfo.iUserStation = pGSPlaying->iUserStation; 

				// 全下玩家
				memcpy(_tableInfo.bAllIn, pGSPlaying->bNoBet, sizeof(pGSPlaying->bNoBet));

				// 弃牌玩家
				memcpy(_tableInfo.bFold, pGSPlaying->bGiveUp, sizeof(pGSPlaying->bGiveUp));

				// 获取代入金币
				memcpy(_tableInfo.iMoneys, pGSPlaying->nSubMoney, sizeof(pGSPlaying->nSubMoney));

				if (_tableInfo.byNTUser != INVALID_DESKNO)
				{
					//显示庄家
					_uiCallback->showDealer(logicToViewStation(_tableInfo.byNTUser));
				}
				else
				{	
					//没有庄家
					HNLOG("没有庄家");
				}

				//下的小盲注和大盲注都会体现在下注里，不需要单独处理
				if (_tableInfo.bySmallBlind != INVALID_DESKNO)
				{
					// 显示小盲
					BYTE byStation = logicToViewStation(_tableInfo.bySmallBlind);
					_uiCallback->showSmallBlind(byStation);
				}
				else
				{
					//没有小盲注
					HNLOG("没有小盲");
				}

				if (_tableInfo.byBigBlind != INVALID_DESKNO)
				{
					// 显示大盲注
					BYTE byStation = logicToViewStation(_tableInfo.byBigBlind);
					_uiCallback->showBigBlind(byStation);
				}
				else
				{
					// 没有大盲注
					HNLOG("没有大盲");
				}


				// 玩家手牌处理（服务端只传回登录玩家的收牌）
				std::vector<BYTE> byCards;
				_tableInfo.iHandCardCounts = pGSPlaying->nHandCardNums;
				//memcpy(_tableInfo.byHandCard, pGSPlaying->byHandCard, sizeof(BYTE) * _tableInfo.iHandCardCounts);
				memcpy(_tableInfo.byHandCard, pGSPlaying->byHandCard, PLAY_COUNT*MAX_DEAL_CARDS);

				// 公共牌处理
				std::vector<BYTE> flopCards,leftCards;
				_tableInfo.iCommunityCardCounts = pGSPlaying->nCardsCount;
				memcpy(_tableInfo.byCommunityCard, pGSPlaying->byCards, sizeof(BYTE) *  pGSPlaying->nCardsCount);
				for(int i = 0; i < _tableInfo.iCommunityCardCounts; i++)
				{
					if(i < 3)
					{
						flopCards.push_back(_tableInfo.byCommunityCard[i]);
					}
					else
					{
						leftCards.push_back(_tableInfo.byCommunityCard[i]);
					}
				}
				if(_tableInfo.byMeStation != INVALID_DESKNO)
				{
					if(flopCards.size() > 0)
					{
						_uiCallback->showFlopCard(flopCards);
					}
					if(leftCards.size() > 1)
					{
						_uiCallback->showTurnCard(leftCards[0]);
						_uiCallback->showRiverCard(leftCards[1]);
					}
					else if(leftCards.size() > 0)
					{
						_uiCallback->showTurnCard(leftCards[0]);
					}
				}

				// 下注金币
				for (int i = 0; i < PLAY_COUNT; i++)
				{
					if (!pGSPlaying->bHaveUser[i])
					{
						continue ;
					}

					BYTE vSeatNo = logicToViewStation(i);

					// 显示代入金币
					_uiCallback->showUserMoney(vSeatNo, _tableInfo.iMoneys[i]);

					//显示昵称
					UserInfoStruct* userInfo = _deskUserList->getUserByDeskStation(i);
					if(userInfo != nullptr)
					{
						_uiCallback->showUserNickName(vSeatNo, GBKToUtf8(userInfo->nickName));
					}

					_tableInfo.nBetMoney[i] = 0;
					for (int j = 0; j < PLAY_COUNT; j++)
					{
						_tableInfo.nBetMoney[i] += pGSPlaying->nBetMoney[i][j];
					}

					if (_tableInfo.nBetMoney[i] > 0 && _tableInfo.bHaveUser[i])
					{
						// 显示下注筹码
						_uiCallback->showUserBetMoney(vSeatNo, _tableInfo.nBetMoney[i]);
					}
					else
					{
						// 隐藏下注筹码	
						HNLOG("玩家桌面上没有筹码");
					}

					//在玩的玩家
					if(userInfo != nullptr && userInfo->bUserState == USER_PLAY_GAME)
					{
						if (_tableInfo.bFold[i])
						{
							_uiCallback->showUserFold(vSeatNo);
						}

						if (_tableInfo.bAllIn[i])
						{
							_uiCallback->showUserAllIn(vSeatNo);
						}

						std::vector<BYTE> handCards;
						handCards.push_back(_tableInfo.byHandCard[i][0]);
						handCards.push_back(_tableInfo.byHandCard[i][1]);
						_uiCallback->showUserHandCard(vSeatNo, handCards);
					}
				}

				// 当前操作玩家
				TToken toKen;
				toKen.bNewTurn	 = pGSPlaying->bNewTurn;
				toKen.bNewTurns	 = pGSPlaying->bNewTurns;
				toKen.byUser	 = pGSPlaying->byTokenUser;
				toKen.byVerbFlag = pGSPlaying->byVerbFlag;
				toKen.nCallMoney = pGSPlaying->nCallMoney;
				toKen.iPassedTime = pGSPlaying->iEvPassTime;
				dealTokenResp(&toKen,sizeof(&toKen));
			}
			break;
		default:
			break;
		}

		//test send ready.
		sendUserReady();
	}


	void GameTableLogic::dealUserAgreeResp(MSG_GR_R_UserAgree* agree)
	{

	}

	void GameTableLogic::dealGameStartResp(BYTE bDeskNO)
	{
		if(bDeskNO == _tableInfo.bDeskIndex)
		{
			_uiCallback->clearDesk();
		}
	}

	void GameTableLogic::dealGameEndResp(BYTE bDeskNO)
	{
		HNLOG("dealGameEndResp");
		if(bDeskNO == _tableInfo.bDeskIndex)
		{
			// 更新用户状态
// 			UserInfoModule()->transform(bDeskNO, [](UserInfoStruct* user, INT index) 
// 			{
// 				if (user->bUserState != USER_WATCH_GAME)
// 				{
// 					user->bUserState = USER_SITTING;
// 				}
// 				//比赛场打完一局后，用户需要重新配桌，所以重置桌子号和座位号
// 				if (RoomLogic()->getSelectedRoom()->dwRoomRule & GRR_CONTEST)
// 				{
// 					user->bDeskNO = INVALID_DESKNO;
// 					user->bDeskStation = INVALID_DESKSTATION;
// 					user->bUserState = USER_LOOK_STATE;						
// 				}
// 			});

			_deskUserList->transform([](UserInfoStruct* user, INT index) 
			{
				if (user->bUserState != USER_WATCH_GAME)
				{
					user->bUserState = USER_SITTING;
				}
				//比赛场打完一局后，用户需要重新配桌，所以重置桌子号和座位号
				if (RoomLogic()->getSelectedRoom()->dwRoomRule & GRR_CONTEST)
				{
					user->bDeskNO = INVALID_DESKNO;
					user->bDeskStation = INVALID_DESKSTATION;
					user->bUserState = USER_LOOK_STATE;						
				}
			});

			//_uiCallback->clearDesk();

			if(_tableInfo.byMeStation != INVALID_DESKNO && _tableInfo.bHaveUser[_tableInfo.byMeStation])
			{
				sendUserReady();
			}
		}
	}

	void GameTableLogic::dealGamePointResp(void* object, INT objectSize)
	{
		CCAssert(sizeof (MSG_GR_R_UserPoint) == objectSize, "MSG_GR_R_UserPoint is error.");
		MSG_GR_R_UserPoint * pUserPoint = (MSG_GR_R_UserPoint*)object;

		if(_myInfo != nullptr)
		{
			_uiCallback->showMyMoney(_myInfo->i64Money);
		}
	}

	void GameTableLogic::dealUserSitResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user)
	{
		HNLOG("onUserSitMessage");
		if(_tableInfo.bDeskIndex == userSit->bDeskIndex)
		{
			BYTE seatNo = logicToViewStation(userSit->bDeskStation);
			if(!_tableInfo.bHaveUser[userSit->bDeskStation])
			{
				_uiCallback->showUser(seatNo, userSit->dwUserID == PlatformLogic()->loginResult.dwUserID, user->bBoy);
				_tableInfo.bHaveUser[userSit->bDeskStation] = true;

				UserInfoStruct* userInfo = _deskUserList->getUserByUserID(userSit->dwUserID);
				if(userInfo != nullptr)
				{
					_uiCallback->showUserNickName(seatNo, GBKToUtf8(userInfo->nickName));
					_uiCallback->showUserMoney(seatNo, 0);
				}

				if (userSit->dwUserID == PlatformLogic()->loginResult.dwUserID)
				{
					setTableOffsetStation(userSit->bDeskStation);
					_tableInfo.byMeStation = userSit->bDeskStation;

					MSG_GM_S_ClientInfo ClientInfo;
					RoomLogic()->sendData(MDM_GM_GAME_FRAME, ASS_GM_GAME_INFO, (CHAR*)&ClientInfo, sizeof(ClientInfo));
				}
			}
			else
			{

			}

		}
	}

	void GameTableLogic::dealUserUpResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user)
	{
		HNLOG("onUserUpMessage");
		if(_tableInfo.bDeskIndex == userSit->bDeskIndex)
		{
			_uiCallback->showUserUp(logicToViewStation(userSit->bDeskStation), userSit->bDeskStation == _tableInfo.byMeStation);
			_tableInfo.bHaveUser[userSit->bDeskStation] = false;

			if(userSit->dwUserID == PlatformLogic()->loginResult.dwUserID)
			{
				_tableInfo.byMeStation = INVALID_DESKNO;
				_uiCallback->leaveDesk();
			}
		}
	}

	/************************分割线*********************************/

	void GameTableLogic::sendUserRaise(LLONG money)
	{
		LLONG betMoney = money;
		if(betMoney > _tableInfo.iMoneys[_tableInfo.byMeStation])
		{
			betMoney = _tableInfo.iMoneys[_tableInfo.byMeStation];
		}
		sendUserBet(ET_RAISE,betMoney);
	}

	void GameTableLogic::sendUserBet(LLONG money)
	{
		LLONG betMoney = money;
		if(betMoney > _tableInfo.iMoneys[_tableInfo.byMeStation])
		{
			betMoney = _tableInfo.iMoneys[_tableInfo.byMeStation];
		}
		sendUserBet(ET_RAISE,betMoney);
	}

	void GameTableLogic::sendUserAllIn()
	{
		sendUserBet(ET_ALLIN,0);
	}

	void GameTableLogic::sendUserFold()
	{
		sendUserBet(ET_FOLD,0);
	}

	void GameTableLogic::sendUserCall()
	{
		sendUserBet(ET_CALL,_tableInfo.nCallMoney);
	}

	void GameTableLogic::sendUserCheck()
	{
		sendUserBet(ET_CHECK,0);
	}

	void GameTableLogic::sendUserUp()
	{
		do 
		{
			if (!RoomLogic()->isConnect())
			{
				_uiCallback->leaveDesk();
				break;
			}

			if (INVALID_DESKNO == _tableInfo.byMeStation)
			{
				_uiCallback->leaveDesk();
				break;
			}

			UserInfoStruct* myInfo = _deskUserList->getUserByDeskStation(_tableInfo.byMeStation);
			if(myInfo != nullptr && myInfo->bUserState == USER_PLAY_GAME)
			{
				_uiCallback->noticeMessage(GBKToUtf8("游戏中，不能离开。"));
				break;
			}

			HNGameLogicBase::sendUserUp();

		} while (0);
	}

	void GameTableLogic::loadDeskUsersUI()
	{		
		//自动创建的牌桌
		if(_tableInfo.bAutoCreate)
		{
			_uiCallback->showTableReady(true);
		}

		//显示在桌玩家
		BYTE seatNo = INVALID_DESKNO;
		for(int i = 0; i < PLAY_COUNT; i++)
		{
			UserInfoStruct* userInfo = _deskUserList->getUserByDeskStation(i);
			if(nullptr != userInfo)
			{
				if(userInfo->dwUserID == RoomLogic()->loginResult.pUserInfoStruct.dwUserID)
				{
					_tableInfo.byMeStation = i;
				}
				seatNo = logicToViewStation(i);
				_uiCallback->showUser(seatNo, i == _tableInfo.byMeStation, userInfo->bBoy);
				_tableInfo.bHaveUser[i] = true;

				_uiCallback->showUserNickName(seatNo, GBKToUtf8(userInfo->nickName));
				_uiCallback->showUserMoney(seatNo,0);
			}
		}

		// 玩家在的时候，旋转
		if(_tableInfo.byMeStation != INVALID_DESKNO)
		{
			setTableOffsetStation(_tableInfo.byMeStation);
		}
		else
		{
			_uiCallback->noticeMessage(GBKToUtf8("买入成功才能进入牌局。"));
		}

//		if(_tableInfo.bAutoCreate)
//		{
        sendGameInfo();
//		}

		// 显示牌桌信息
		_uiCallback->showTableInfo(RoomLogic()->getSelectedRoom()->szGameRoomName);
		if(nullptr != _myInfo)
		{
			_uiCallback->showMyMoney(_myInfo->i64Money);
		}
		//HNGameLogicBase::loadDeskUsers();
	}

	void GameTableLogic::clearDesk()
	{
		showAction(false, nullptr);
		showAutoAction(false);
	}

	LLONG GameTableLogic::getMinBuy() const
	{
		return _tableInfo.tagDeskCfg.dz.iSubMinMoney;
	}

	LLONG GameTableLogic::getMaxBuy() const
	{
		return _tableInfo.tagDeskCfg.dz.iSubMaxMoney;
	}

	LLONG GameTableLogic::getPlanBuy() const
	{
		return _tableInfo.tagDeskCfg.dz.iSubPlanMoney;
	}

	BYTE  GameTableLogic::getMeStation() const
	{
		return _tableInfo.byMeStation;
	}

	void GameTableLogic::sendUserReady()
	{
		//默认带入计划带入数量
		BYTE byMeStation = _tableInfo.byMeStation;
		do 
		{
			if (_tableInfo.byMeStation == INVALID_DESKNO) break;

			if(_tableInfo.bGameStatus == GS_PLAY_GAME)
			{
				_uiCallback->noticeMessage(GBKToUtf8("牌局进行中，下一局才能购买。"));
				break;
			}

			UserInfoStruct* myInfo = _deskUserList->getUserByDeskStation(byMeStation);
			if(nullptr != myInfo && (myInfo->i64Money < _tableInfo.tagDeskCfg.dz.iSubMinMoney))
			{
				_uiCallback->noticeMessage(GBKToUtf8("筹码不够买入，请充值后再参加。"));
				break;
			}

			LLONG min = _tableInfo.tagDeskCfg.dz.iSubMinMoney;
			LLONG max = _tableInfo.tagDeskCfg.dz.iSubMaxMoney;
			LLONG plan = _tableInfo.tagDeskCfg.dz.iSubMinMoney;
			/*if(_tableInfo.iMoneys[byMeStation] > min && _tableInfo.iMoneys[byMeStation] < max)
			{
				plan = _tableInfo.iMoneys[byMeStation];
			}*/
			_uiCallback->showBuyMoneyDialog(_tableInfo.byMeStation, min, max, plan, myInfo->i64Money);
		} while (0);
	}

	void GameTableLogic::sendUserBet(emType type, LLONG money)
	{
		TBet tagBet;
		tagBet.nMoney = money;
		tagBet.nType = type;
		RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, ASS_BET, &tagBet,sizeof(tagBet));
	}

	//顺时针+，逆时针-
	void GameTableLogic::setTableOffsetStation(BYTE bDeskStation)
	{
		int oldOffset = _tableInfo.iOffsetStation;
		_tableInfo.iOffsetStation = -bDeskStation;
		int rotate = _tableInfo.iOffsetStation - oldOffset;
		if(rotate > 3)
		{
			rotate = PLAY_COUNT - rotate;
		}
		else if(rotate < -3)
		{
			rotate = PLAY_COUNT + rotate;
		}

		_uiCallback->rotateStation(rotate);
	}
	/************************分割线*********************************/
	//以下变量每一局开始都会清理
	void GameTableLogic::initData()
	{
		_tableInfo.bIsOffline = false;

		// 记录下注玩家
		//m_iBetCounts = 0;

		// 初始化累加金额
		_tableInfo.nCurrentMoney = 0;
		//m_nCurrentMoney = 0;

		// 跟注金币
		_tableInfo.nCallMoney = 0;
		//m_nCallMoney = 0;

		//庄家位置
		_tableInfo.byNTUser = INVALID_DESKNO;
		//m_byNTUser = 255;

		//小盲注位置
		_tableInfo.bySmallBlind = INVALID_DESKNO;
		//m_bySmallBlind = 255;

		//大盲注位置
		_tableInfo.byBigBlind = INVALID_DESKNO;
		//m_byBigBlind = 255;

		//每个用户可用金币
		//memset(m_nMoney, 0, sizeof(m_nMoney));

		// 全下玩家
		memset(_tableInfo.bAllIn, 0, sizeof(_tableInfo.bAllIn));

		// 弃牌玩家
		memset(_tableInfo.bFold, 0, sizeof(_tableInfo.bFold));

		// 托管标志
		memset(_tableInfo.bTrusteeship, 0, sizeof(_tableInfo.bTrusteeship));

		// 上一次操作类型
		memset(_tableInfo.emLastOperation, ET_UNKNOWN,sizeof(_tableInfo.emLastOperation));

		//底牌数
		_tableInfo.iHandCardCounts = 0;

		//底牌数据
		memset(_tableInfo.byHandCard, 0, sizeof(_tableInfo.byHandCard));

		//公共牌
		_tableInfo.iCommunityCardCounts = 0;
		memset(_tableInfo.byCommunityCard, 0, sizeof(_tableInfo.byCommunityCard));

		// 每个玩家下注金币
		memset(_tableInfo.nBetMoney, 0, sizeof(_tableInfo.nBetMoney));

		// 清空栈
		while (!_tableInfo.stackLastStation.empty())
		{
			_tableInfo.stackLastStation.pop();   // 移除栈顶无素
		}
	}

	void GameTableLogic::saveTableInfo(void* object)
	{
		TGSBase* pGSBase = (TGSBase*)object;

		memcpy(&_tableInfo.tagDeskCfg,&pGSBase->tagDeskCfg,sizeof(TDeskCfg));
		memcpy(&_tableInfo.bHaveUser,pGSBase->bHaveUser,sizeof(_tableInfo.bHaveUser));
	}


	void GameTableLogic::showUserInfo()
	{
		for(int i = 0; i < PLAY_COUNT; i++)
		{
			if(_tableInfo.bHaveUser[i])
			{
				BYTE seatNo = logicToViewStation(i);

				//显示玩家信息
				UserInfoStruct* user = _deskUserList->getUserByDeskStation(i);
				if(user != nullptr)
				{
					_uiCallback->showUserNickName(seatNo, GBKToUtf8(user->nickName));
				}
				//if(i == _tableInfo.byMeStation && _tableInfo.iMoneys[_tableInfo.byMeStation] <= 0)
				//{
				//	_uiCallback->showUserMoney(seatNo,user->i64Money);
				//}
			}
		}
	}

	// 显示玩家筹码
	// pSubMoney：牌桌内所有玩家的筹码
	void GameTableLogic::showUserMoney(const TSubMoney* pSubMoney)
	{
		if(pSubMoney != nullptr)
		{
			memcpy(&_tableInfo.iMoneys, pSubMoney->nMoney, sizeof(_tableInfo.iMoneys));	
		}

		UserInfoStruct* userInfo = nullptr;
		for (int i = 0; i < PLAY_COUNT; i++)
		{
			userInfo = _deskUserList->getUserByDeskStation(i);
			if(nullptr != userInfo && _tableInfo.bHaveUser[i])
			{
				BYTE byUser = logicToViewStation(i);
				_uiCallback->showUserMoney(byUser,_tableInfo.iMoneys[i]);
			}
		}
	}

	// 显示带入金额弹窗
	// min:最小带入
	// max:最大带入
	// plan:计划带入
	void GameTableLogic::showBuyMoney(LLONG min, LLONG max, LLONG plan)
	{

	}

	BYTE GameTableLogic::getNextUserStation(BYTE byStation)
	{
		int nCount = 0;
		BYTE nextSeatNo = byStation;
		UserInfoStruct* userInfo = nullptr;

		// 绕过无效玩家与已放弃的玩家
		do 
		{
			nextSeatNo = (nextSeatNo + 1 + PLAY_COUNT) % PLAY_COUNT;
			userInfo = _deskUserList->getUserByDeskStation(nextSeatNo);
			nCount++;
			if(nCount >= PLAY_COUNT)
			{
				return byStation;
			}
		} while (userInfo == nullptr || _tableInfo.bFold[nextSeatNo] || _tableInfo.iUserStation != TYPE_PLAY_GAME);

		return nextSeatNo;
	}

	BYTE GameTableLogic::getLastUserStation(BYTE byStation)
	{
		int nCount = 0;
		BYTE lastSeatNo = byStation;
		UserInfoStruct* userInfo = nullptr;

		// 绕过无效玩家与已放弃的玩家
		do 
		{
			lastSeatNo = (lastSeatNo - 1 + PLAY_COUNT) % PLAY_COUNT;
			userInfo = _deskUserList->getUserByDeskStation(lastSeatNo);
			nCount++;
			if(nCount >= PLAY_COUNT)
			{
				return byStation;
			}
		} while (userInfo == nullptr || _tableInfo.bFold[lastSeatNo] || _tableInfo.iUserStation != TYPE_PLAY_GAME);

		return lastSeatNo;
	}

	BYTE GameTableLogic::viewToLogicStation(BYTE byStation)
	{
		return (byStation - _tableInfo.iOffsetStation + PLAY_COUNT) % PLAY_COUNT;
	}

	BYTE GameTableLogic::logicToViewStation(BYTE byStation)
	{
		return (byStation + _tableInfo.iOffsetStation + PLAY_COUNT) % PLAY_COUNT;
	}

	/**显示操作按钮**/
	void GameTableLogic::showAction(bool bVisible, void* object)
	{
		if(bVisible)
		{
			TToken* pToken = (TToken*)object;
			BYTE byVerbFlag = pToken->byVerbFlag;
			BYTE byMeStation = _tableInfo.byMeStation;



			LLONG nMoney = _tableInfo.nBetMoney[getLastUserStation(byMeStation)] - _tableInfo.nBetMoney[byMeStation];
			bool bRaiseable = _tableInfo.iMoneys[byMeStation] > nMoney && _tableInfo.iMoneys[byMeStation] > pToken->nCallMoney;

			BYTE tmp = getLastUserStation(byMeStation);
			CCLOG("*****************************lastNo:%d",tmp);
			CCLOG("*****************************lastBetMoney:%lld",_tableInfo.nBetMoney[tmp]);
			CCLOG("*****************************MeNo:%d",_tableInfo.byMeStation);
			CCLOG("*****************************MeBetMoney:%lld",_tableInfo.nBetMoney[byMeStation]);
			CCLOG("*****************************MeMoney:%lld",_tableInfo.iMoneys[byMeStation]);
			CCLOG("*****************************call:%lld",pToken->nCallMoney);

			//第一个按钮
			if(pToken->nCallMoney > 0)
			{
				if(!bRaiseable)
				{
					_uiCallback->showAllIn(true,true);
				}
				else
				{
					_uiCallback->showCall(true,true);
				}
			}
			else
			{
				_uiCallback->showCheck(true,true);
			}

			//第二个按钮
			LLONG max = _tableInfo.iMoneys[byMeStation];
			LLONG min = pToken->nCallMoney;
			if(bRaiseable)
			{
				LLONG tmp = _tableInfo.tagDeskCfg.dz.iBigBlindNote + pToken->nCallMoney;
				if(tmp < _tableInfo.iMoneys[byMeStation])
				{
					min = tmp;
				}
				else
				{
					min = _tableInfo.iMoneys[byMeStation];
				}

				if(pToken->bNewTurn)
				{
					_uiCallback->showBet(true,true,0);
					_uiCallback->showSlider(true,true,max,min,min);
				}
				else if(bRaiseable)
				{
					_uiCallback->showRaise(true,true);
					_uiCallback->showSlider(true,true,max,min,min);
				}
			}
			else
			{
				_uiCallback->showBet(false,false,0);
				_uiCallback->showRaise(false,false);
				_uiCallback->showSlider(false,false,0,0,0);
			}


			//第三个按钮
			_uiCallback->showFold(true,true);
		}
		else
		{
			_uiCallback->showBet(false,false,0);
			_uiCallback->showCall(false,false);
			_uiCallback->showRaise(false,false);
			_uiCallback->showCheck(false,false);
			_uiCallback->showFold(false,false);
			_uiCallback->showAllIn(false,false);
			_uiCallback->showSlider(false,false,0,0,0);

			_uiCallback->showBetMoney(false,0);
			_uiCallback->showCallMoney(false,0);
			_uiCallback->showRaiseMoney(false,0);
		}
	}

	//显示教学
	void GameTableLogic::showHelp(bool bVisible)
	{
		_uiCallback->showHelp(bVisible);
	}

	//显示托管
	void GameTableLogic::showAutoAction(bool bVisible)
	{
		_uiCallback->showAutoCall(bVisible,_tableInfo.bTrusteeship[0]);
		_uiCallback->showAutoCallAny(bVisible,_tableInfo.bTrusteeship[1]);
		_uiCallback->showAutoCheck(bVisible,_tableInfo.bTrusteeship[2]);
		_uiCallback->showAutoCheckFold(bVisible,_tableInfo.bTrusteeship[3]);
	}

	void GameTableLogic::setSliderValue()
	{
		LLONG nPercentMoney = _tableInfo.iMoneys[_tableInfo.byMeStation];
		LLONG number =  _uiCallback->getSliderValue();
		_tableInfo.nCurrentMoney = number;

		// 滑动条控件有点不标准, 可能会超出一部分, 所有在此判断金币
		if (_tableInfo.nCurrentMoney > nPercentMoney)
		{
			_tableInfo.nCurrentMoney = nPercentMoney;
		}
		else if (_tableInfo.nCurrentMoney < _tableInfo.tagDeskCfg.dz.iMinusMoney)
		{
			_tableInfo.nCurrentMoney = _tableInfo.tagDeskCfg.dz.iMinusMoney;
			_uiCallback->setSliderValue(_tableInfo.nCurrentMoney);
		}

		if (nPercentMoney < _tableInfo.tagDeskCfg.dz.iMinusMoney)
		{
			_tableInfo.nCurrentMoney = nPercentMoney;
		}

		_uiCallback->showRaiseMoney(true,_tableInfo.nCurrentMoney);
		_uiCallback->showBetMoney(true,_tableInfo.nCurrentMoney);
	}

	//托管处理
	bool GameTableLogic::doUserAutoAction(void* object)
	{
		TToken* pToken = (TToken*)object;

		// 	if(_tableInfo.bWatch)
		// 	{
		// 		return false;
		// 	}

		if (!_tableInfo.stackLastStation.empty())
		{
			// 计算可以跟注的金币, 并设置跟注按钮
			_tableInfo.nCallMoney = 0;

			// 如果上家操作是放弃, 则查找上上家的操作下注金币
			stack<BYTE> skTemp(_tableInfo.stackLastStation);
			if (!skTemp.empty())
			{
				BYTE bLastStation = skTemp.top();
				skTemp.pop();

				// 查找上家下注、加注、全下的玩家
				while (bLastStation != INVALID_DESKSTATION && _tableInfo.emLastOperation[bLastStation] == ET_FOLD)
				{
					if (!skTemp.empty())
					{
						// 从栈中取得一个元素
						bLastStation = skTemp.top();
						skTemp.pop();
					}
					else
					{
						// 栈为空
						bLastStation = -1;
						break;
					}
				}

				if (bLastStation != INVALID_DESKSTATION)
				{
					// 计算要跟注的金币
					_tableInfo.nCallMoney = _tableInfo.nBetMoney[bLastStation] - _tableInfo.nBetMoney[_tableInfo.byMeStation];
				}
				else
				{
					log("Client: AutoTrusteeship 跟注错误, 操作栈没有找到上家操作人");
				}
			}
			else
			{
				log("Client: AutoTrusteeship 跟注错误, 操作栈为空");
			}

			stack<BYTE> skTemp2(_tableInfo.stackLastStation);
			BYTE bLastStation = skTemp2.top();
			skTemp2.pop();

			if (bLastStation != INVALID_DESKNO)
			{
				if (pToken->byVerbFlag & UD_VF_BET)	// 允许下注或过牌
				{
					log("Client: 允许下注或过牌");
					// 在此进行自动托管过牌与自动托管弃牌判断
					if (_tableInfo.emLastOperation[bLastStation] == ET_CHECK)
					{
						// 自动过牌, 上家弃牌情况下才
						if (_tableInfo.bTrusteeship[2] || _tableInfo.bTrusteeship[3])
						{
							// 进行自动过牌操作
							_tableInfo.bTrusteeship[2] = false;  // (只过一次)

							log("Client: 执行自动过牌操作");

							sendUserCheck();
							return true;
						}
					}
					else if (_tableInfo.emLastOperation[bLastStation] == ET_FOLD)
					{
						// 自动弃牌
						if (_tableInfo.bTrusteeship[3])
						{
							// 进行自动弃牌操作
							log("Client: 执行自动弃牌操作");

							sendUserFold();
							return true;
						} 
					}
				}
				else
				{
					//在此进行自动托管过牌与自动托管弃牌判断
					if (_tableInfo.emLastOperation[bLastStation] == ET_CHECK)
					{
						// 自动过牌, 上家弃牌情况下才
						if (_tableInfo.bTrusteeship[2] || _tableInfo.bTrusteeship[3])
						{
							// 进行自动过牌操作
							_tableInfo.bTrusteeship[2] = false;  // (只过一次)

							log("Client: 执行自动过牌操作");

							sendUserCheck();
							return true;
						}
					}
					// 自动跟注(只限一次) 或 自动跟任何注(每次都跟)
					else if (_tableInfo.bTrusteeship[0] || _tableInfo.bTrusteeship[1])
					{
						// 在此进行自动跟注操作
						log("Client: 执行自动跟注操作");
						_tableInfo.bTrusteeship[0] = false;

						sendUserCall();
						return  true;
					}
					else if (_tableInfo.bTrusteeship[3])
					{
						// 自动过牌/弃牌
						if (_tableInfo.emLastOperation[bLastStation] == ET_FOLD)
						{
							// 进行自动弃牌操作
							log("Client: 执行自动弃牌操作2");

							sendUserFold();
							return true;
						}
					}
				}
			}
			else
			{
				log("Client: Error: 非第一个操作, 但操作人的栈为空");
			}
		}

		return false;
	}


}
