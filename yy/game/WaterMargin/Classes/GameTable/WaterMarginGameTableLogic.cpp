#include "WaterMarginGameTableLogic.h"
#include "WaterMarginGameTableUI.h"
#include "WaterMarginGameTableUICallBack.h"
#include "WaterMarginMessageHead.h"


namespace WaterMargin
{
	GameTableLogic::GameTableLogic(GameTableUICallback* uiCallback, BYTE deskNo, bool bAutoCreate)
		: _uiCallback(uiCallback)
		, HNGameLogicBase(deskNo, 1, bAutoCreate, uiCallback)
	{
		test2 = 12;
		/*memset(_iUserStation, 0, sizeof(_iUserStation));
		memset(_playerSitted, false, sizeof(_playerSitted));*/
		
		//initParams();
	}

	GameTableLogic::~GameTableLogic()
	{

	}

	void GameTableLogic::SendStandUp()
	{
		do 
		{/*
			if (!RoomLogic()->connected())
			{
				_uiCallback->leaveDesk();
				break;
			}*/

			HNGameLogicBase::sendUserUp();
		} while (0);
	}

	void GameTableLogic::dealUserUpResp(MSG_GR_R_UserUp * userSit, UserInfoStruct* user)
	{
		if(userSit->dwUserID == PlatformLogic()->loginResult.dwUserID)
		{
			_mySeatNo = INVALID_DESKNO;
			_uiCallback->leaveDesk();
		}
	}

	void GameTableLogic::dealGameStationResp(void* object, INT objectSize)
	{
		CHECK_SOCKET_DATA(GameStation_Base, objectSize, "GameStation_Base size is error!");

		GameStation_Base* pData = (GameStation_Base*)object;

		//初始化线数
		_uiCallback->setDataText(pData->iLineCount, 3);
		_uiCallback->setMoney(pData->i64UserMoney);
		
	}

	void GameTableLogic::dealGameMessage(NetMessageHead* messageHead, void* object, INT objectSize)
	{
		switch(messageHead->bAssistantID)
		{
		case ASS_S_GAME_BEGIN:		//游戏开始
			break;
		case ASS_S_BUY_COIN_RESULT:		//玩家上下分结果
			dealScoreResult(object,objectSize);
			break;
		case ASS_S_ADD_NOTE_RESULT:		//玩家加注结果
			dealAddNoteResult(object, objectSize);
			break;
		case ASS_S_ROLL_RESULT:		//玩家滚动结果
			dealRollResult(object,objectSize);
			break;
		case SUB_S_MALI_START:	//马力开始
			dealMaLiResult(object,objectSize);
			break;
		case SUB_S_GETBIBEI_RESULT:		//获取比倍结果
			dealBibeiHistoryResult(object,objectSize);
			break;
		case ASS_S_GETSCORE_RESULT:		//获取得分结果
			dealWinScoreResult(object,objectSize);
			break;
		case ASS_S_BIBEI_RESULT:		//比倍押注结果
			dealBiBeiResult(object,objectSize);
			break;
		}
			
	}

	void GameTableLogic::dealMaLiResult(void* object, INT objectSize)
	{
		CHECK_SOCKET_DATA(CMD_S_MaLi, objectSize, "CMD_S_MaLi size is error");
		HNLOG("dealMaLiResult");
		CMD_S_MaLi* pData = (CMD_S_MaLi*)object;
		
		//数据处理
		if (pData->m_TypeIndex != 1000)
		{
			if (pData->m_TypeIndex > 2)
			{
				pData->m_TypeIndex = pData->m_TypeIndex - 2;
			}
			else
			{
				pData->m_TypeIndex = 22 + pData->m_TypeIndex;
			}
		}
		//
		_uiCallback->setWinScore(pData->m_AddScore);
		_uiCallback->MaLiResult(pData->m_NowBegin, pData->m_GameCount, pData->m_TypeIndex, pData->m_ThreeTypeId);
		
		/*test2 = 1000;*/
		
	}

	void GameTableLogic::dealBiBeiResult(void* object, INT objectSize)
	{
		CHECK_SOCKET_DATA(CMD_S_BiBeiRelult, objectSize, "CMD_S_BiBeiRelult size is error");
		HNLOG("dealBiBeiResult");
		CMD_S_BiBeiRelult* pData = (CMD_S_BiBeiRelult*)object;
		
		INT dianShu = pData->iS1 + pData->iS2;
		_uiCallback->setSaiZi(pData->iS1, pData->iS2);
		_uiCallback->setWinScore(pData->iWinScore);
		_uiCallback->playSecondAnimate(pData->iWinScore, dianShu);
	}

	void GameTableLogic::dealWinScoreResult(void* object, INT objectSize)
	{
		CHECK_SOCKET_DATA(CMD_S_WinScore, objectSize, "CMD_S_WinScore size is error");
		HNLOG("dealAddNoteResult");
		CMD_S_WinScore* pData = (CMD_S_WinScore*)object;

		_uiCallback->getWinScore();
	}

	void GameTableLogic::dealAddNoteResult(void* object, INT objectSize)
	{
		CHECK_SOCKET_DATA(CMD_S_AddScore, objectSize, "CMD_S_AddScore size is error");
		HNLOG("dealAddNoteResult");
		CMD_S_AddScore* pData = (CMD_S_AddScore*)object;
		_uiCallback->setDataText(pData->iAddScore, 4);
		_uiCallback->setDataText(pData->iAddScore*9, 5);
		//_uiCallback->setDataText(pData->iUserScore, 1);
		if (pData->iAddScore > 0)
		{
			_uiCallback->setBtnVisible(Tag_START, true);
			_uiCallback->setBtnVisible(Tag_AUTO, true);
		}
		else
		{
			_uiCallback->setBtnVisible(Tag_ADD, false);
		}
		
	}

	void GameTableLogic::dealScoreResult(void* object, INT objectSize)
	{
		CHECK_SOCKET_DATA(CMD_S_BulletCount, objectSize, "CMD_S_BulletCount size is error");
		HNLOG("dealScoreResult");
		CMD_S_BulletCount* pData = (CMD_S_BulletCount*)object;
		_uiCallback->setDataText(pData->iUserScore, 1);
		_uiCallback->setDataText(pData->iAllCellScore, 4);
		_uiCallback->setDataText(pData->iAllCellScore*9, 5);
		_uiCallback->setMoney(pData->i64UserMoney);
		if (pData->i64UserMoney == 0)	
		{
			_uiCallback->setBtnVisible(Tag_UP, false);
		}
		else
		{
			_uiCallback->setBtnVisible(Tag_UP, true);
		}

		if (pData->iAllCellScore >= 0 && pData->iUserScore > 0)
		{
			_uiCallback->setBtnVisible(Tag_ADD, true);
			_uiCallback->setBtnVisible(Tag_DOWN, true);
			if (pData->iAllCellScore > 0)
			{
				_uiCallback->setBtnVisible(Tag_START,true);
				_uiCallback->setBtnVisible(Tag_AUTO, true);
			}
		}

	}

	void GameTableLogic::dealRollResult(void* object, INT objectSize)
	{
		CHECK_SOCKET_DATA(CMD_S_TypeScroll, objectSize, "CMD_S_TypeScroll size is error");
		HNLOG("dealScoreResult");
		CMD_S_TypeScroll* pData = (CMD_S_TypeScroll*)object;

		_uiCallback->JudgeConnect();

		int* str = DataSwitch(pData->iTypeImgid);
		_uiCallback->setImageData(str);
		_uiCallback->setDataText(pData->iUserScore, 1);
		_uiCallback->setWinScore(pData->iWinScore);

	}

	void GameTableLogic::handleSecond()
	{
		_uiCallback->handleSecondDeFeng();
	}

	void GameTableLogic::handleBiBeiAgain()
	{
		_uiCallback->BiBeiAgain();
	}

	void GameTableLogic::handleMaLiViewGold()
	{
		_uiCallback->viewWinGold();
	}

	void GameTableLogic::MaLiJudge()
	{
		_uiCallback->JudgeMaLi();
	}

	void GameTableLogic::changeMaLiToBB()
	{
		_uiCallback->MaLiEndBB();
	}

	void GameTableLogic::changeMaLiToDF()
	{
		_uiCallback->MaLIEndDF();
	}

	int* GameTableLogic::DataSwitch(int* str)
	{
		int ptr[15] = {0};
		
		ptr[0] = str[0];
		ptr[1] = str[5];
		ptr[2] = str[10];
		ptr[3] = str[1];
		ptr[4] = str[6];
		ptr[5] = str[11];
		ptr[6] = str[2];
		ptr[7] = str[7];
		ptr[8] = str[12];
		ptr[9] = str[3];
		ptr[10] = str[8];
		ptr[11] = str[13];
		ptr[12] = str[4];
		ptr[13] = str[9];
		ptr[14] = str[14];

		for (int i = 0; i < 15; i++)
		{
			str[i] = ptr[i];
		}

		return str;
	}

	void GameTableLogic::dealBibeiHistoryResult(void* object, INT objectSize)
	{
		CHECK_SOCKET_DATA(CMD_S_BiBeiData, objectSize, "CMD_S_BiBeiData size is error");
		HNLOG("dealScoreResult");
		CMD_S_BiBeiData* pData = (CMD_S_BiBeiData*)object;

		_uiCallback->BiBeiHistory(pData->iBiBeiData);

	}

	//客户端------------->服务端

	void GameTableLogic::sendUpOrDownMessage(bool value)
	{
		CMD_C_BuyCoin pData;
		pData.bAddOrMove = value;
		RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, ASS_C_BUY_COIN, &pData, sizeof(pData));
	}

	void GameTableLogic::sendAddNoteMessage()
	{
		CMD_S_AddScore pData;
		pData.bUp = true;
		RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, ASS_C_ADD_NOTE, &pData, sizeof(pData));
	}

	void GameTableLogic::sendGetWinMessage(int value)
	{
		CMD_C_GetScore pData;
		pData.iGetType = value;
		RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, ASS_C_GET_SCORE, &pData, sizeof(pData));
	}

	void GameTableLogic::sendStartRollMessage()
	{
		CMD_C_StartRoll pData;
		RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, ASS_C_START_ROLL, &pData, sizeof(pData));
	}

	void GameTableLogic::sendBibeiBtnMessage()
	{
		CMD_C_GetBiBei pData;
		RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, ASS_C_GET_BIBEI, &pData, sizeof(pData));
	}

	void GameTableLogic::sendDaXiaoMessage(int iBibeiType,int iBibeiMax)
	{
		CMD_C_BiBeiStart pData;
		pData.iBiBeiType = iBibeiType;
		pData.iMaxOrOther = iBibeiMax;
		RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, ASS_C_BIBEI, &pData, sizeof(pData));
	}

	void GameTableLogic::sendMaLiMessage()
	{
		CMD_C_GetMaLi pData;
		RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, ASS_C_GET_MALI, &pData, sizeof(pData));
	}

}