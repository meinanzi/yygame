#include "ZYHZ_GameTableLogic.h"
#include "ZYHZ_GameTableUI.h"
#include "ZYHZ_GameTableUICallback.h"
#include "ZYHZ_GameManager.h"
#include "ZYHZ_Card.h"
//#include "tinyxml2/tinyxml2.h"

namespace ZYHZ
{

using namespace cocostudio;
using namespace std;
using namespace ui;
using namespace HN;

GameTableLogic* GameTableLogic::_instance = nullptr;


	GameTableLogic::GameTableLogic(GameTableUICallBack* tableUICallBack, BYTE deskNo, bool bAutoCreate) 
		: HNGameLogicBase(deskNo, PLAY_COUNT, bAutoCreate, tableUICallBack)
		, _tableUICallBack(tableUICallBack)
	{
		_callBack = (GameTableUI *)_tableUICallBack;
		_instance = this;
		_hasSetGameStation = false;
		//_agreeCount = 0;
	}

	GameTableLogic::~GameTableLogic()
	{
	}
	/*-----------------------------------------------------------------------------------------------*/

	void GameTableLogic::dealGameStartResp(BYTE bDeskNO)
	{
//		int m = 0;
	}

	void GameTableLogic::dealGameEndResp(BYTE bDeskNO)
	{
//		int n = 0;
	}

	void GameTableLogic::dealUserAgreeResp(MSG_GR_R_UserAgree* agree)
	{
		onUserAgree(*((MSG_GR_R_UserAgree*)agree));
	}

	void GameTableLogic::dealUserSitResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user)
	{
		if (!_hasSetGameStation)
		{
			return;
		}
		if (_userInfo.bDeskStation >=0 && _userInfo.bDeskStation < PLAY_COUNT)
		{
			_callBack->addUser(getUserDir(userSit->bDeskStation), *user);
		}
	}

	// 用户站起
	void GameTableLogic::dealUserUpResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user)
	{
		if (!_hasSetGameStation)
		{
			return;
		}
		// 清除庄家显示
		for (auto i = 1; i <= PLAY_COUNT; i++)
		{
			COCOS_NODE(Sprite, StringUtils::format("zhuang%d", i))->setVisible(false);          // 庄家
		}
		// 清除本地界面的ui显示
		if (userSit->dwUserID == _userInfo.dwUserID)
		{
			_callBack->dealLeaveDesk();
		}
		else
		{
			//--_agreeCount;
			if (userSit->bDeskStation >= 0 && userSit->bDeskStation <= 3)
			_callBack->removeUser(getUserDir(userSit->bDeskStation));
		}
	}


	// 游戏状态
	void GameTableLogic::dealGameStationResp(void* object, INT objectSize)
	{
		setGameStation(object, objectSize);
	}

	// 游戏消息（游戏的主体消息来自这里）
	void GameTableLogic::dealGameMessage(NetMessageHead* messageHead, void* object, INT objectSize)
	{
		if (_hasSetGameStation == false)
		{
			return;
		}
		switch(messageHead->bAssistantID)
		{
		case ASS_AUTOCARD:				//玩家托管通知/// 托管消息
			{
				assert(sizeof(UserTouGuan) == objectSize);
				dealOnUserTuoguanResp(*((UserTouGuan*)object));
			}break;
		case THING_GAME_BEGIN:
			{
				assert(sizeof(tagBeginEx) == objectSize);
				dealOnGameStartResp(*((tagBeginEx*)object));         /// 游戏开始消息  ---
			}break;
		case THING_DING_NT:
			{
				assert(sizeof(tagDongNtEx) == objectSize);
				dealOnDingNtResp(*((tagDongNtEx*)object));
			}break;
		case THING_2SEZI_NT:        //第一次筛子，确认庄家方向
			{
				assert(sizeof(tagTwoSeziNtEx) == objectSize);
				dealOn2SeZiNTResp(*((tagTwoSeziNtEx*)object));
			}break;
		case THING_2SEZI_DIR:		//第二次筛子 确认起牌方向消息	DWJ
			{
				assert(sizeof(tagTwoSeziDirEx) == objectSize);
				dealOn2SeZiDirResp(*((tagTwoSeziDirEx*)object));
			}break;
		case THING_2SEZI_GP://	第三次筛子确认起牌墩数消息	DWJ
			{
				assert(sizeof(tagTwoSeziGetPaiEx) == objectSize);
				dealOn2SeZiGPResp(*((tagTwoSeziGetPaiEx*)object));
			}break;
		case THING_SEND_PAI:
			{
				assert(sizeof(tagSendPaiEx) == objectSize);
				dealOnSendAllCardResp(*((tagSendPaiEx*)object));
			}break;
		case THING_ALL_BUHUA:		//	所有玩家补花事件	DWJ
			{
				assert(sizeof(tagAllBuHuaEx) == objectSize);
				dealOnAllBuHuaResp(*((tagAllBuHuaEx*)object));
			}break;
		case THING_BEGIN_OUT:
			{
				assert(sizeof(tagBeginOutPaiEx) == objectSize);
				dealOnGamePlayResp(*((tagBeginOutPaiEx*)object));
			}break;
		case THING_OUT_PAI:
			{
				assert(sizeof(tagOutPaiEx) == objectSize);
				dealOnNotifyOutCardResp(*((tagOutPaiEx*)object));
			}break;
		case THING_CPG_NOTIFY:
			{
				assert(sizeof(tagCPGNotifyEx) * PLAY_COUNT == objectSize);
				dealOnNotifyBlockResp(*((tagCPGNotifyEx*)object));
			}break;
		case THING_ZHUA_PAI:
			{
				assert(sizeof(tagZhuaPaiEx) == objectSize);
				dealOnNotifyZhuaPaiResp(*((tagZhuaPaiEx*)object));
			}break;
		case THING_ONE_BUHUA:		//	单个玩家补花
			{
				assert(sizeof(tagOneBuHuaEx) == objectSize);
				dealOnNotifyOneBuHuaResp(*((tagOneBuHuaEx*)object));          
			}break;
		case THING_GUO_PAI:			//	放弃拦牌	
			{
				dealOnNotifyCanCelBlockResp();
			}break;
		case THING_CHI_PAI:
			{
				assert(sizeof(tagChiPai) == objectSize);
				dealOnNotifyChiPaiResp(*((tagChiPai*)object));
			}break;
		case THING_PENG_PAI:
			{
				assert(sizeof(tagPengPaiEx) == objectSize);
				dealOnNotifyPengPaiResp(*((tagPengPaiEx*)object));
			}break;
		case THING_GANG_PAI:
			{
				assert(sizeof(tagGangPaiEx) == objectSize);
				dealOnNotifyGangPaiResp(*((tagGangPaiEx*)object));
			}break;
		case THING_HU_PAI:
			{
				assert(sizeof(tagHuPaiEx) == objectSize);
				dealOnNotifyHuPaiResp(*((tagHuPaiEx*)object));
			}break;
        case THING_MAI_MA://			,			//买码事件:
            {
//                assert(sizeof(tagMaiMa)==objectSize);
                deal_THING_MAI_MA(object, objectSize);
                break;
            }
		case THING_ZHONG_NIAO:
			{
				assert(sizeof(ZhongNiao) == objectSize);
				dealOnNotifyZhongNiaoResp(*((ZhongNiao*)object));
			}break;
		case THING_HAVE_ACTION:
			{
				assert(sizeof(HaveAction) == objectSize);
				dealOnNotifyHaveActionResp(*((HaveAction*)object));
			}break;
		case THING_ENG_HANDLE:
			{
				assert(sizeof(tagCountFenEx) == objectSize);
				dealOnNotifyFinishResp(*((tagCountFenEx*)object));
			}break;
		default:
			break;
		}

	}


	/*MESSAGE*************************************************************************/

	void GameTableLogic::setGameStation(void* pBuffer,int nLen)
	{
		_msgGameStationDataEx.Init();
		GameStation_Base *pGameStation = (GameStation_Base *) pBuffer;
		_msgGameStationDataEx.Stationpara = pGameStation->byGameStation; //设置游戏状态

		_hasSetGameStation = true;
		_userInfo = RoomLogic()->loginResult.pUserInfoStruct;
		GameManager::getInstance()->setConnectSuccess();
		//_agreeCount = 0;
		switch (_msgGameStationDataEx.Stationpara)
		{
			case GS_SEND_CARD:		// 发牌状态(还未发牌,或者已经发牌，但是没有出牌)
			case GS_PLAY_GAME:		// 游戏中状态
				{
					assert (nLen == sizeof(GameStationDataMEx));
					GameStationDataMEx *pGameStation = (GameStationDataMEx *)pBuffer;
					assert (NULL != pGameStation);

					if (_msgGameStationDataEx.Stationpara == GS_SEND_CARD)
					{
						GameManager::getInstance()->setSendStatus(false, true);
					}

					_msgGameStationDataEx.iVersion = pGameStation->iVersion;
					_msgGameStationDataEx.iVersion2 = pGameStation->iVersion2;
					_msgGameStationDataEx.Stationpara = pGameStation->byGameStation;
					_msgGameStationDataEx.byBeginTime = pGameStation->byBeginTime;
					_msgGameStationDataEx.byOutTime = pGameStation->byOutTime;
					_msgGameStationDataEx.byThinkTime = pGameStation->byBlockTime;

					UserTouGuan msg;
					memset(&msg, 0, sizeof(msg));
					msg.byStation = RoomLogic()->loginResult.pUserInfoStruct.bDeskStation;
					msg.bIsTuoGuan = false;
					RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, ASS_AUTOCARD, &msg, sizeof(msg));

					_msgGameStationDataEx.m_bShowTax = pGameStation->bShowTax;
					_msgGameStationDataEx.byNtStation = pGameStation->byNtStation;
					for (auto i = 0; i < PLAY_COUNT; i++)
					{
						_msgGameStationDataEx.m_bistuoguan[i] = pGameStation->m_bistuoguan[i];
					}
					_msgGameStationDataEx.m_byNowOutStation = pGameStation->m_byNowOutStation;
					_msgGameStationDataEx.m_byThingDoing = pGameStation->m_byThingDoing;
					
					// 手牌
					for (auto i = 0; i < PLAY_COUNT; i++)
					{
						for (auto j = 0; j < HAND_CARD_NUM; j++)
						{
							_msgGameStationDataEx.m_byArHandPai[i][j] = pGameStation->m_byArHandPai[i][j];
						}
					}

					// 门牌
					for (auto i = 0; i < PLAY_COUNT; i++)
					{
						for (auto j = 0; j < MEN_CARD_NUM; j++)
						{
							_msgGameStationDataEx.m_iArMenPai[i][j] = pGameStation->m_iArMenPai[i][j];
						}
					}

					// 吃碰杠牌
					for (auto i = 0; i < 4; i++)
					{
						for (auto j = 0; j < 5; j++)
						{
							_msgGameStationDataEx.m_UserGCPData[i][j] = pGameStation->m_UserGCPData[i][j];
						}
					}

					// 出牌
					for (auto i = 0; i < PLAY_COUNT; i++)
					{
						for (auto j = 0; j < OUT_CARD_NUM; j++)
						{
							_msgGameStationDataEx.m_byArOutPai[i][j] = pGameStation->m_byArOutPai[i][j];
						}
					}

					reconnected();
					break;
				}
			case GS_WAIT_NEXT:		// 等待下一盘游戏开始
			case GS_WAIT_SETGAME:	// 等待东家设置状态
			case GS_WAIT_ARGEE:      // 等候玩家同意
				{
					assert (nLen == sizeof(GameStation_Wait_Agree));
					GameStation_Wait_Agree *pGameStation = (GameStation_Wait_Agree *)pBuffer;
					assert (NULL != pGameStation);
					
					_msgGameStationDataEx.iVersion = pGameStation->iVersion;
					_msgGameStationDataEx.iVersion2 = pGameStation->iVersion2;
					_msgGameStationDataEx.Stationpara = pGameStation->byGameStation;
					_msgGameStationDataEx.byBeginTime = pGameStation->byBeginTime;
					_msgGameStationDataEx.byOutTime = pGameStation->byOutTime;
					_msgGameStationDataEx.byThinkTime = pGameStation->byBlockTime;
					for (auto i = 0; i < PLAY_COUNT; i++)
					{
						_msgGameStationDataEx.m_bistuoguan[i] = pGameStation->bUserAgree[i];
					}
					_msgGameStationDataEx.m_bShowTax = pGameStation->bShowTax;

					waitAgree();
					break;
				}

		default:
			break;
		}
	}

	void GameTableLogic::dealOnUserTuoguanResp(const UserTouGuan& msg)
	{
		log("\n dealOnUserTuoguanResp  :");
		_msgUsertouguan = msg;
		GameManager::getInstance()->userTrusteeshep(getUserDir(msg.byStation), msg.bIsTuoGuan);
	}

	void GameTableLogic::onUserAgree(const MSG_GR_R_UserAgree& msg)
	{
		if (!_hasSetGameStation)
		{
			return;
		}
		_msgMSG_GR_R_UserAgree = msg;
		log("\n onUserAgree   : ");
		log("bAgreeGame  %d   ", msg.bAgreeGame);
		log("bDeskNO  %d   ", msg.bDeskNO);
		log("bDeskStation  %d   ", msg.bDeskStation);
		_callBack->agreeGame(getUserDir(msg.bDeskStation));
	}

	void GameTableLogic::dealOnGameStartResp(const tagBeginEx& msg)
	{
		log("\n dealOnGameStartResp :");
		_msgtagBeginEx = msg;
		GameManager::getInstance()->playCommonSound("Begin");
		_callBack->startGame(_msgGameStationDataEx.bAutoBegin == 0 ? false : true);

		HNAudioEngine::getInstance()->stopBackgroundMusic();
	}

	void GameTableLogic::dealOnDingNtResp(const tagDongNtEx& msg)
	{
		_msgtagDongNtEx = msg;
	}

	void GameTableLogic::dealOnSeziSureOpenPointResp(const tagTwoSeziDirAndGetPaiEx& msg)
	{
		log("\n dealOnSeziSureOpenPointResp : ");

		_msgtagTwoSeziDirAndGetPaiEx = msg;

		// 开始投骰子
		auto dir = getUserDir(msg.byUser);
		
		GameManager::getInstance()->touziAction(dir, msg.bySezi0, msg.bySezi1);
	}

	void GameTableLogic::dealOnSendAllCardResp(const tagSendPaiEx& msg)
	{
		log("\n dealOnSendAllCardResp :");
		_msgtagSendPaiEx = msg;
		
		std::vector<std::vector<INT>> vv;
		auto playerIndex = _userInfo.bDeskStation;
		std::vector<INT> v;

		// 生成手牌
		for (auto j = 0; j < HAND_CARD_NUM; j++)
		{
			v.push_back(msg.m_byArHandPai[playerIndex][j]);
		}
		vv.push_back(v);
		v.clear();

		for (auto j = 0; j < HAND_CARD_NUM; j++)
		{
			v.push_back(msg.m_byArHandPai[(playerIndex+1)%4][j]);
		}
		vv.push_back(v);
		v.clear();

		for (auto j = 0; j < HAND_CARD_NUM; j++)
		{
			v.push_back(msg.m_byArHandPai[(playerIndex+2)%4][j]);
		}
		vv.push_back(v);
		v.clear();

		for (auto j = 0; j < HAND_CARD_NUM; j++)
		{
			v.push_back(msg.m_byArHandPai[(playerIndex+3)%4][j]);
		}
		vv.push_back(v);
		v.clear();

		GameManager::getInstance()->initAllHandCard(vv);
	}	

	void GameTableLogic::dealOnGamePlayResp(const tagBeginOutPaiEx& msg)
	{
		_msgtagBeginOutPaiEx = msg;
		log("\n dealOnGamePlayResp :");
		GameManager::getInstance()->playGame();
	}

	void GameTableLogic::dealOnNotifyOutCardResp(const tagOutPaiEx& msg)
	{
		log("dealOnNotifyOutCardResp :");
		_msgtagOutPaiEx = msg;
//		auto number = msg.byPs % 10;
//		auto color = msg.byPs / 10;

		GameManager::getInstance()->playSexNumberSound(msg.byUser, msg.byPs);
		auto dir = getUserDir(msg.byUser);
		_callBack->outCard(dir, msg.byPs);  // 出牌
	}

	void GameTableLogic::dealOnNotifyZhuaPaiResp(const tagZhuaPaiEx& msg)
	{
		log("dealOnNotifyZhuaPaiResp \n");
		_msgtagZhuaPaiEx = msg;
		GameManager::getInstance()->playCommonSound("zhuapai");
		// 最后显示的牌
		std::vector<std::vector<int>> vvTmpNum;
		for (auto i = 0; i < PLAY_COUNT; i++)
		{
			std::vector<int> vt;
			for (auto j = 0; j< HAND_CARD_NUM; j++)
			{
				if (msg.m_byArHandPai[i][j] != 255)
				{
					vt.push_back(msg.m_byArHandPai[i][j]);
				}
			}
			vvTmpNum.push_back(vt);
		}
		std::vector<std::vector<int>> vvNum(PLAY_COUNT, std::vector<int>());

		for (auto i = 0; i < PLAY_COUNT; i++)
		{
			auto index = getUserVecIndex(i);
			vvNum.at(index) = vvTmpNum.at(i);
		}

		GameManager::getInstance()->setHandCardFormServer(vvNum);

		log("byPs  %d ", msg.byPs);
		log("byUser  %d ", msg.byUser);
		log("_msgtagZhuaPaiEx.byNext  %d ", _msgtagZhuaPaiEx.byNext);
		auto dir = getUserDir(msg.byUser);                // 取方向
		_callBack->catchCard(dir, msg.byPs, msg.bHead);                // 抓牌
		// 开始出牌
		GameManager::getInstance()->startTimer(getUserDir(msg.byUser), timerType::WAIT_OUT);
	}

	void GameTableLogic::dealOnNotifyBlockResp(const tagCPGNotifyEx& msg)
	{
	}

	void GameTableLogic::dealOnNotifyHaveActionResp(const HaveAction& msg)
	{
		GameManager::getInstance()->playCommonSound("Block");
		auto index = _userInfo.bDeskStation;
		if (msg.T_CPGNotify[index].bCanAction)
		{
			log("dealOnNotifyBlockResp \n");
			GameManager::getInstance()->startTimer(GameManager::getInstance()->getCurrOperDir(), timerType::WAIT_ACTION);
			if (msg.T_CPGNotify[index].bGang)
			{
				auto kongType = msg.T_CPGNotify[index].m_iGangData[0][0];
				GameManager::getInstance()->setKongType(kongType);
				GameManager::getInstance()->setCurrOperNumber(msg.T_CPGNotify[index].m_iGangData[0][1]);
				if (kongType == ACTION_AN_GANG)
				{
					GameManager::getInstance()->startTimer(GameManager::getInstance()->getCurrOperDir(), timerType::WAIT_OUT);
				}
			}
			if (msg.T_CPGNotify[index].bHu)
			{
				auto &hu = GameManager::getInstance()->getHuMsg();
				hu.Clear();
				hu.byPs = msg.T_CPGNotify[index].m_iHuPs;
				hu.byUser = _userInfo.bDeskStation;
				GameManager::getInstance()->startTimer(GameManager::getInstance()->getCurrOperDir(), timerType::WAIT_OUT);
			}	
			GameManager::getInstance()->block(msg.T_CPGNotify[index].bPeng, msg.T_CPGNotify[index].bGang, msg.T_CPGNotify[index].bHu);
		}
		else
		{
			log("dealOnNotifyHaveActionResp \n");
			log("deskstation %d \n", msg.byDeskStation);
			GameManager::getInstance()->startTimer(getUserDir(msg.byDeskStation), timerType::WAIT_BLOCK);
		}
	}

	void GameTableLogic::dealOnNotifyChiPaiResp(const tagChiPai& msg)
	{
		log("dealOnNotifyChiPaiResp \n");
		_msgtagChiPai = msg;
		GameManager::getInstance()->setLastActionCard(getUserDir(msg.byUser), msg.byPs);
	}

	void GameTableLogic::dealOnNotifyPengPaiResp(const tagPengPaiEx& msg)
	{
		log("dealOnNotifyPengPaiResp \n");
		log("byPs  %d ", msg.byPs);
		log("byUser  %d ", msg.byUser);
		_msgtagPengPaiEx = msg;
		GameManager::getInstance()->setLastActionCard(getUserDir(msg.byUser), msg.byPs);

		auto dir = getUserDir(msg.byUser);
		auto index = INT(dir) - INT(sitDir::SOUTH_DIR);
		auto action = GameManager::getInstance()->_touchCard;
		action->setInfo(GameManager::getInstance()->getVecUser().at(index)->getOwnPool(), msg.byPs);
		GameManager::getInstance()->playSexActionSound(msg.byUser, "peng");
		GameManager::getInstance()->resetCatchPos(true);
	}

	void GameTableLogic::dealOnNotifyGangPaiResp(const tagGangPaiEx& msg)
	{
		log("dealOnNotifyGangPaiResp \n");
		log("byUser  %d ", msg.byUser);
		log("byPs  %d ", msg.byPs);
		log("byType  %d ", msg.byType);

		_msgtagGangPaiEx = msg;

		GameManager::getInstance()->setLastActionCard(getUserDir(msg.byUser), msg.byPs);
		
		auto dir = getUserDir(msg.byUser);
		auto index = INT(dir) - INT(sitDir::SOUTH_DIR);

		PoolAction * action = nullptr;
		if (msg.byType == AT_QUADRUPLET_REVEALED)   // 明杠
		{
			action = GameManager::getInstance()->_meldedKong;
		}
		else if (msg.byType == AT_QUADRUPLET_CONCEALED)  // 暗杠
		{
			action = GameManager::getInstance()->_concealedKong;
		}
		else if (msg.byType == AT_QUADRUPLET)   // 补杠/碰杠
		{
			action = GameManager::getInstance()->_touchKong;
		}
		assert(action != nullptr);
		log("gang type = %d", msg.byType);
		action->setInfo(GameManager::getInstance()->getVecUser().at(index)->getOwnPool(), msg.byPs);
		GameManager::getInstance()->resetCatchPos();
	}

	void GameTableLogic::dealOnNotifyHuPaiResp(const tagHuPaiEx& msg)
	{
		log("dealOnNotifyHuPaiResp \n");
		_msgtagHuPaiEx = msg;
		GameManager::getInstance()->setPlayGame(false);
		// 设置动做牌
		auto actionNumber = 0;
		if (!msg.bZimo)
		{
			actionNumber = msg.byPs;
		}
		else
		{
			for (auto i = 0; i < HAND_CARD_NUM; i++)
			{
				if (msg.m_byArHandPai[msg.byUser][i + 1] == 255)
				{
					// 设置自摸的牌
					actionNumber = msg.m_byArHandPai[msg.byUser][i];
					break;
				}
			}
		}
		GameManager::getInstance()->setLastActionCard(getUserDir(msg.byUser), actionNumber);


		// 取得胡牌位置和手牌
		for (auto i = 0; i < PLAY_COUNT; i++)
		{
			if (msg.bHaveHu[i])
			{
				std::vector<int> huVec;
				for (auto j = 0; j < MAX_HAND_PAI; j++)
				{
					if (msg.m_byArHandPai[i][j] != 255)
					{
						huVec.push_back(msg.m_byArHandPai[i][j]);
					}
					else
					{
						break;
					}
				}
				auto dir = getUserDir(i);
				GameManager::getInstance()->showHuTmpCard(dir, huVec);
				auto index = INT(dir) - INT(sitDir::SOUTH_DIR);
				auto action = GameManager::getInstance()->_huCard;
				(dynamic_cast<HuCard *>(action))->setZiMoQiangGame(msg.bZimo, msg.bQiangGang);
				action->setInfo(GameManager::getInstance()->getVecUser().at(index)->getOwnPool(), actionNumber);
				GameManager::getInstance()->playSexActionSound(msg.byUser, "hu");
			}
		}
	}

	void GameTableLogic::dealOnNotifySuperSetPaiResp(const SuperStruct& msg)
	{
		log("dealOnNotifySuperSetPaiResp \n");
		log("bySetType  %d ", msg.bySetType);

		_msgSuperStruct = msg;
	}

	void GameTableLogic::dealOnNotifyFinishResp(const tagCountFenEx& msg)
	{
		log("dealOnNotifyFinishResp : ");

		_msgtagCountFenEx = msg;
		GameManager::getInstance()->setPlayGame(false);
		GameManager::getInstance()->hideNiao();
		// 是否黄庄
		auto bs = true;
		for (auto i = 0; i < PLAY_COUNT; i++)
		{
			
			if (msg.m_bIsHu[i] == true)
			{
				bs = false;
				break;
			}
		}
		int maxTypeNum = 6;			// 最大胡牌类型数
		string typeHuName = "";
		int picNum = 0;
		int huNumber = 0;
		for (auto i = 0; i < PLAY_COUNT; i++)
		{
			if (msg.m_bIsHu[i])
			{
				++picNum;
				++huNumber;
			}
		}
		
		if (picNum > 0)
		{
			--picNum;
		}

		auto picAddStr = StringUtils::format("%s", picNum <= 0 ? "" : StringUtils::format("%d", picNum).c_str()).c_str();
		// init
		for (auto i = 1; i <= PLAY_COUNT; i++)
		{
			COCOS_NODE(Text, StringUtils::format("gNameID%s%d%d",picAddStr, i, i))->setString("");
			COCOS_NODE(Text, StringUtils::format("gGangFen%s%d",picAddStr, i))->setString("");
			COCOS_NODE(Text, StringUtils::format("gHuFen%s%d",picAddStr, i))->setString("");
			COCOS_NODE(Text, StringUtils::format("gZhongQiang%s%d",picAddStr, i))->setString("");
			COCOS_NODE(Text, StringUtils::format("gGold%s%d",picAddStr, i))->setString("");
			COCOS_NODE(Text, StringUtils::format("gEarn%s%d",picAddStr, i))->setString("");
		}

		for (auto i = 1; i <= picNum+1; i++)
		{
			COCOS_NODE(Text, StringUtils::format("gNameIDHu%s%d", picAddStr, i))->setString("");
			for (auto j = 1; j <= maxTypeNum; j++)
			{
				COCOS_NODE(Text, StringUtils::format("gHuType%s%d_%d",picAddStr, j, i))->setString("");
			}
		}

		 // 显示胡牌的玩家和胡牌类型
		if (bs == false)    
		{
			int huCount = 1;
			for (auto i = 0; i < PLAY_COUNT; i++)
			{
				// 赋值
				if (msg.m_bIsHu[i])
				{
					// id
					COCOS_NODE(Text, StringUtils::format("gNameIDHu%s%d", picAddStr, huCount))->setString(GBKToUtf8(getUserBySeatNo(i)->nickName));
					// HuType
					// 显示玩家胡牌类型
					for (auto j = 0; j < maxTypeNum; j++)
					{
						if (msg.byHuType[msg.byUser][j] == HUPAI_HU_PAI)
						{
							typeHuName = "平胡";
						}
						else if (msg.byHuType[msg.byUser][j] == HUPAI_TYPE_GANG_KAI)
						{
							typeHuName = "杠上开花";
						}
						else if (msg.byHuType[msg.byUser][j] == HUPAI_TYPE_QIANG_GANG)
						{
							typeHuName = "抢杠胡";
						}
						else if (msg.byHuType[msg.byUser][j] == HUPAI_TYPE_FOUR_ZHONG)
						{
							typeHuName = "四红中";
						}
						else if (msg.byHuType[msg.byUser][j] == HUPAI_ZI_MO)
						{
							typeHuName = "自摸";
						}
						else
						{
							typeHuName = "";
						}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
						COCOS_NODE(Text, StringUtils::format("gHuType%s%d_%d",picAddStr, j+1, huCount))->setString(GBKToUtf8(typeHuName.c_str()));
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
						COCOS_NODE(Text, StringUtils::format("gHuType%s%d_%d",picAddStr, j+1, huCount))->setString(typeHuName);
#endif
						
					}

					++huCount;
				}
			}
		}

		// 显示是否黄庄
		COCOS_NODE(Sprite, StringUtils::format("huangzhuang%s", picAddStr))->setVisible(bs);

		// 显示胡牌在UI上
		std::vector<std::vector<int>> vvHuNumber;
		for (auto i = 0; i < PLAY_COUNT; i++)
		{
			if (msg.m_bIsHu[i])
			{
				std::vector<int> vecTmp;
				for (auto mmm = 0; mmm < HAND_CARD_NUM; mmm++)
				{
					if (msg.m_byArHandPai[i][mmm] != 255)
					{
						vecTmp.push_back(msg.m_byArHandPai[i][mmm]);
					}
				}
				vvHuNumber.push_back(vecTmp);
			}
		}

		GameManager::getInstance()->showShowCardToFinishUI(huNumber, vvHuNumber);

		// 显示玩家得分情况
		for (auto i = 0; i < PLAY_COUNT; i++)
		{
			COCOS_NODE(Text, StringUtils::format("gNameID%s%d%d",picAddStr, i + 1, i + 1))->setString(GBKToUtf8(getUserBySeatNo(i)->nickName));
			COCOS_NODE(Text, StringUtils::format("gGangFen%s%d",picAddStr, i + 1))->setString(
				StringUtils::format("%d", msg.iGangFan[i]));
			COCOS_NODE(Text, StringUtils::format("gHuFen%s%d",picAddStr, i + 1))->setString(
				StringUtils::format("%d", msg.iHuFan[i]));
			COCOS_NODE(Text, StringUtils::format("gZhongQiang%s%d",picAddStr, i + 1))->setString(
				StringUtils::format("%d", msg.iZhongFan[i]));
			COCOS_NODE(Text, StringUtils::format("gEarn%s%d",picAddStr, i + 1))->setString(
				StringUtils::format("%lld", (msg.i64Money[i])));
		}
		
		// 最后显示的牌
		std::vector<std::vector<int>> vvTmpNum;
		for (auto i = 0; i < PLAY_COUNT; i++)
		{
			std::vector<int> vt;
			for (auto j = 0; j< HAND_CARD_NUM; j++)
			{
				if (msg.m_byArHandPai[i][j] != 255)
				{
					vt.push_back(msg.m_byArHandPai[i][j]);
				}
			}
			vvTmpNum.push_back(vt);
		}
		std::vector<std::vector<int>> vvNum(PLAY_COUNT, std::vector<int>());

		for (auto i = 0; i < PLAY_COUNT; i++)
		{
			auto index = getUserVecIndex(i);
			vvNum.at(index) = vvTmpNum.at(i);
		}

		GameManager::getInstance()->finishGame(vvNum);

		COCOS_NODE(Sprite, StringUtils::format("finish%s", picAddStr))->setVisible(true);
		COCOS_NODE(Sprite, StringUtils::format("finish%s", picAddStr))->setScale(1.5f);

		GameManager::getInstance()->showMoney();

		COCOS_NODE(Sprite, StringUtils::format("finish%s", picAddStr))->runAction(Sequence::create(
			DelayTime::create(_msgGameStationDataEx.byBeginTime),
			CallFunc::create(CC_CALLBACK_0(Node::setVisible, COCOS_NODE(Sprite, StringUtils::format("finish%s", picAddStr)), false)),
			//CallFunc::create(CC_CALLBACK_0(GameManager::restartGame, GameManager::getInstance())),
			CallFunc::create(CC_CALLBACK_0(GameTableLogic::sendAgreeGame, this)),
			nullptr
			));
	}

	void GameTableLogic::dealOnForceFinishResp(const tagCountFenEx& msg)
	{
		log("dealOnNotifyFinishResp : ");

		_msgtagCountFenEx = msg;
		_callBack->setPlayGame(false);

		if (msg.byUser == _userInfo.bDeskStation)
		{
			sendUserUp();
		}
	}

	void GameTableLogic::dealOn2SeZiNTResp(const tagTwoSeziNtEx& msg)									// 2骰子定庄
	{
		log("dealOn2SeZiNTResp : ");
		auto outDir = getUserDir(msg.byUser);
		GameManager::getInstance()->touziAction(outDir, msg.bySezi0, msg.bySezi1);
	}

	void GameTableLogic::dealOn2SeZiDirResp(const tagTwoSeziDirEx& msg)									// 2骰子定庄
	{
		log("dealOn2SeZiDirResp : ");
		auto dir = getUserDir(msg.byUser);
		auto catchDir = getUserDir((msg.byUser + msg.bySezi0 + msg.bySezi1 - 1)%4);
		GameManager::getInstance()->setNt(dir);
		GameManager::getInstance()->setCatchDir(catchDir);
		GameManager::getInstance()->touziAction(dir, msg.bySezi0, msg.bySezi1);
	}

	void GameTableLogic::dealOn2SeZiGPResp(const tagTwoSeziGetPaiEx& msg)							// 2骰子定庄
	{
		log("dealOn2SeZiGPResp : ");
		auto dir = getUserDir(msg.byUser);
		GameManager::getInstance()->setCatchPos(msg.bySezi0 + msg.bySezi1);
		GameManager::getInstance()->touziAction(dir, msg.bySezi0, msg.bySezi1);
	}

	void GameTableLogic::dealOnAllBuHuaResp(const tagAllBuHuaEx& msg)												// 补花
	{
		log("dealOnAllBuHuaResp : ");
	}

	void GameTableLogic::dealOnNotifyOneBuHuaResp(const tagOneBuHuaEx& msg)									// 单人补花
	{
		log("dealOnNotifyOneBuHuaResp : ");
	}

	void GameTableLogic::dealOnNotifyCanCelBlockResp()								// 取消拦牌
	{
		log("dealOnNotifyCanCelBlockResp : ");
	}

	void GameTableLogic::dealOnNotifyZhongNiaoResp(const ZhongNiao& msg)
	{
		log("dealOnNotifyZhongNiaoResp : ");
		std::vector<INT> vNiao;
		for (auto i = 0; i < PLAY_COUNT; i++)
		{
			vNiao.push_back(msg.byCard[i]);
		}
		GameManager::getInstance()->showNiao(vNiao);
	}

	/*****************************************************************************************/
	void GameTableLogic::safeQuit()
	{
		HNAudioEngine::getInstance()->stopBackgroundMusic();
		if (RoomLogic()->isConnect())
		{
			sendUserUp();	
		}
	}

	void GameTableLogic::waitAgree()
	{
		// 启动倒计时
		GameManager::getInstance()->startTimer(sitDir::SOUTH_DIR, timerType::WAIT_START);
		_userInfo = RoomLogic()->loginResult.pUserInfoStruct;
		// 显示玩家
		vector<UserInfoStruct *> vec;
		UserInfoModule()->findDeskUsers(_userInfo.bDeskNO, vec);
		for (auto &v : vec)
		{
			_callBack->addUser(getUserDir(v->bDeskStation), *v);
		}
	}

	void GameTableLogic::showMoney()
	{
	
	}

	sitDir GameTableLogic::getUserDir(const BYTE& deskStation)
	{
		return  sitDir((deskStation - _userInfo.bDeskStation+PLAY_COUNT)%PLAY_COUNT + 1);
	}

	INT GameTableLogic::getUserStation(const sitDir& dir)
	{
		return (INT(dir) - INT(sitDir::SOUTH_DIR) + _userInfo.bDeskStation)%PLAY_COUNT;
	}

	int GameTableLogic::getUserVecIndex(const BYTE& deskStation)
	{
		return (deskStation - _userInfo.bDeskStation+PLAY_COUNT)%PLAY_COUNT;
	}

	void GameTableLogic::reconnected()
	{
		std::vector<std::vector<INT>> vvHandCard;
		vvHandCard.assign(PLAY_COUNT, std::vector<INT>());

		std::vector<std::vector<INT>> vvOutCard;
		vvOutCard.assign(PLAY_COUNT, std::vector<INT>());

		std::vector<std::vector<std::vector<INT>>> vvvActionCard;
		vvvActionCard.assign(PLAY_COUNT, std::vector<std::vector<INT>> ());

		std::vector<std::vector<INT>> vvMenPai;
		vvMenPai.assign(4, std::vector<INT>(0));

		std::vector<bool> vTrusteeshep;
		vTrusteeshep.assign(PLAY_COUNT, false);

		std::vector<INT> vQueMen;
		vQueMen.assign(PLAY_COUNT, 0);


		sitDir ntDir = getUserDir(_msgGameStationDataEx.byNtStation);

		int operIndex = 0;

		// 手牌
		for (auto i = 0; i < PLAY_COUNT; i++)
		{
			operIndex = getUserVecIndex(i);
			std::vector<INT> v;
			for (auto j = 0; j < HAND_CARD_NUM; j++)
			{
				if (_msgGameStationDataEx.m_bIsHu[i])
				{
					if (_msgGameStationDataEx.m_byArHandPai[i][j+1] == 255)
					{
						break;
					}
				}
				if (_msgGameStationDataEx.m_byArHandPai[i][j] != 255)
				{
					v.push_back(_msgGameStationDataEx.m_byArHandPai[i][j]);
				}
			}
			vvHandCard.at(operIndex) = v;
			v.clear();
		}

		// 出的牌
		for (auto i = 0; i < PLAY_COUNT; i++)
		{
			operIndex = getUserVecIndex(i);
			std::vector<INT> v;
			for (auto j = 0; j < OUT_CARD_NUM; j++)
			{
				if (_msgGameStationDataEx.m_byArOutPai[i][j] != 255)
				{
					v.push_back(_msgGameStationDataEx.m_byArOutPai[i][j]);
				}
			}
			vvOutCard.at(operIndex) = v;
			v.clear();
		}

		// 动作牌
		for (auto i = 0; i < PLAY_COUNT; i++)
		{
			operIndex = getUserVecIndex(i);
			std::vector<std::vector<INT>> vv;
			for (auto j = 0; j < 5; j++)
			{
				std::vector<INT> v;
				for (auto k = 0; k < 4; k++)
				{
					if (_msgGameStationDataEx.m_UserGCPData[i][j].byData[k] != 255)
					{
						v.push_back(_msgGameStationDataEx.m_UserGCPData[i][j].byData[k]);
					}
				}
				vv.push_back(v);
				v.clear();
			}
			vvvActionCard.at(operIndex) = vv;
			vv.clear();
		}

		// 门牌
		auto menNum = MEN_CARD_NUM;
		for (auto i = 0; i < PLAY_COUNT; i++)
		{
			operIndex = getUserVecIndex(i);
			if (getUserDir(i) == sitDir::SOUTH_DIR || getUserDir(i) == sitDir::NORTH_DIR)
			{
				menNum = MEN_CARD_NUM;
			}
			else
			{
				menNum = MEN_CARD_NUM;
			}
			std::vector<INT> v;
			for (auto j = 0; j < menNum; j++)
			{
				if (_msgGameStationDataEx.m_iArMenPai[i][j] != 255)
				{
					v.push_back(1);
				}
				else
				{
					v.push_back(0);
				}
			}
			vvMenPai.at(operIndex) = v;
			v.clear();
		}

		// 缺门
		for (auto i = 0; i < PLAY_COUNT; i++)
		{
			operIndex = getUserVecIndex(i);
			vTrusteeshep.at(operIndex) = _msgGameStationDataEx.m_bistuoguan[i];
		}

		// 托管
		for (auto i = 0; i < PLAY_COUNT; i++)
		{
			operIndex = getUserVecIndex(i);
			vQueMen.at(operIndex) = _msgGameStationDataEx.m_byDingQue[i];
		}

		// 胡的牌
		for (auto i = 0; i < PLAY_COUNT; i++)
		{
			auto dir = getUserDir(i);
			if (_msgGameStationDataEx.m_bIsHu[i])
			{
				for (auto j = 0; j < HAND_CARD_NUM; j++)
				{
					if (_msgGameStationDataEx.m_byArHandPai[i][j + 1] == 255)
					{
						GameManager::getInstance()->setHasHu(dir, true);
						GameManager::getInstance()->setLastActionCard(dir, _msgGameStationDataEx.m_byArHandPai[i][j]);
						break;
					}
				}
			}
		}

 		_callBack->reconnected(vvHandCard,  vvOutCard,  vvvActionCard, vvMenPai, vTrusteeshep, vQueMen, ntDir);
	}

    void GameTableLogic::deal_THING_MAI_MA(void* object, int objectsize)
    {
        assert(sizeof(tagMaiMa)==objectsize);
        auto data = (tagMaiMa*)object;
        
        _tableUICallBack->showMaimaOption(30, data);
    }

}
