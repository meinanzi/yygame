#include "XZMJ_GameTableLogic.h"
#include "XZMJ_GameTableUI.h"
#include "XZMJ_GameTableUICallback.h"
#include "GameManager.h"
#include "Card.h"
#include "tinyxml2/tinyxml2.h"

namespace XZMJ
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
		int m = 0;
	}

	void GameTableLogic::dealGameEndResp(BYTE bDeskNO)
	{
		int n = 0;
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
		assert(sizeof(GameStationDataEx) == objectSize);
		setGameStation(*((GameStationDataEx*)object));
	}

	// 游戏消息（游戏的主体消息来自这里）
	void GameTableLogic::dealGameMessage(NetMessageHead* messageHead, void* object, INT objectSize)
	{

//#ifdef MY_DEBUG
		//log(StringUtils::format("msg： %d", messageHead->bAssistantID).c_str());
//#endif

		if (_hasSetGameStation == false)
		{
			return;
		}

		switch(messageHead->bAssistantID)
		{
		case ASS_GM_GAME_STATION:           ///<设置状态/// 断线重连消息
			{
				assert(sizeof(GameStationDataEx) == objectSize);
				setGameStation(*((GameStationDataEx*)object));
			}break;
		case ASS_AUTOCARD:				//玩家托管通知/// 托管消息
			{
				assert(sizeof(Usertouguan) == objectSize);
				onUserTuoguan(*((Usertouguan*)object));
			}break;
		case THING_BEGIN:
			{
				assert(sizeof(tagBeginEx) == objectSize);
				onGameStart(*((tagBeginEx*)object));         /// 游戏开始消息  ---
			}break;
		case THING_DONG_NT:
			{
				assert(sizeof(tagDongNtEx) == objectSize);
				onDongNt(*((tagDongNtEx*)object));
			}break;
		case THING_2SEZI_DIR_GP:
			{
				assert(sizeof(tagTwoSeziDirAndGetPaiEx) == objectSize);
				onSeziSureOpenPoint(*((tagTwoSeziDirAndGetPaiEx*)object));
			}break;
		case THING_SEND_PAI:
			{
				assert(sizeof(tagSendPaiEx) == objectSize);
				onSendAllCard(*((tagSendPaiEx*)object));

			}break;
		case THING_BEGIN_OUT:
			{
				assert(sizeof(tagBeginOutPaiEx) == objectSize);
				onGamePlay(*((tagBeginOutPaiEx*)object));
			}break;
		case THING_OUT_PAI:
			{
				assert(sizeof(tagOutPaiEx) == objectSize);
				onNotifyOutCard(*((tagOutPaiEx*)object));
			}break;
		case THING_ZHUA_PAI:
			{
				assert(sizeof(tagZhuaPaiEx) == objectSize);
				onNotifyZhuaPai(*((tagZhuaPaiEx*)object));
			}break;
		case THING_CPG_NOTIFY:
			{
				assert(sizeof(tagCPGNotifyEx) == objectSize);
				onNotifyBlock(*((tagCPGNotifyEx*)object));
			}break;
		case THING_CHI_PAI:
			{
				assert(sizeof(tagChiPai) == objectSize);
				onNotifyChiPai(*((tagChiPai*)object));
			}break;
		case THING_PENG_PAI:
			{
				assert(sizeof(tagPengPaiEx) == objectSize);
				onNotifyPengPai(*((tagPengPaiEx*)object));
			}break;
		case THING_GANG_PAI:
			{
				assert(sizeof(tagGangPaiEx) == objectSize);
				onNotifyGangPai(*((tagGangPaiEx*)object));
			}break;
		case THING_HU_PAI:
			{
				assert(sizeof(tagHuPaiEx) == objectSize);
				onNotifyHuPai(*((tagHuPaiEx*)object));
			}break;
		case THING_HAVE_ACTION:
			{
				assert(sizeof(SC_NotifyHaveAction) == objectSize);
				onNotifyHaveAction(*((SC_NotifyHaveAction*)object));
			}
		case ASS_SUPER:
			{
				/*assert(sizeof(SuperStruct) == objectSize);
				onNotifySuperSetPai(*((SuperStruct*)object));*/
			}break;
		case THING_ENG_HANDLE:
			{
				assert(sizeof(tagCountFenEx) == objectSize);
				onNotifyFinish(*((tagCountFenEx*)object));
			}break;
		case THING_ENG_UN_NORMAL:
			{
				assert(sizeof(tagCountFenEx) == objectSize);
				onForceFinish(*((tagCountFenEx*)object));
			}break;
		default:
			break;
		}

	}


	/*MESSAGE*************************************************************************/

	void GameTableLogic::setGameStation(const GameStationDataEx& msg)
	{
		_msgGameStationDataEx = msg;
		_hasSetGameStation = true;
		_userInfo = RoomLogic()->loginResult.pUserInfoStruct;
		log("setGameStation\n");
		GameManager::getInstance()->setConnectSuccess();
		//_agreeCount = 0;
		switch (_msgGameStationDataEx.Stationpara)
		{
			case GS_SEND_CARD:		// 发牌状态(还未发牌,或者已经发牌，但是没有出牌)
				{
					break;
				}
			case GS_PLAY_GAME:		// 游戏中状态
				{
					_callBack->setPlayGame();
					reconnected();
					break;
				}
			case GS_WAIT_NEXT:		// 等待下一盘游戏开始
				{
					break;
				}
			case GS_WAIT_ARGEE:      // 等候玩家同意
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
					break;
				}
			case GS_WAIT_SETGAME:	// 等待东家设置状态
				{
					break;
				}

		default:
			break;
		}
	}

	void GameTableLogic::onUserTuoguan(const Usertouguan& msg)
	{
		log("\n dealOnUserTuoguanResp  :");
		_msgUsertouguan = msg;
		GameManager::getInstance()->userTrusteeshep(getUserDir(msg.desk), msg.touguang);
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

	void GameTableLogic::onGameStart(const tagBeginEx& msg)
	{
		log("\n onGameStart :");
		_msgtagBeginEx = msg;
		HNAudioEngine::getInstance()->playEffect((MUSIC_PATH + "Begin.mp3").c_str());
		_callBack->startGame(_msgGameStationDataEx.bAutoBegin == 0 ? false : true);

		GameManager::getInstance()->runAction(
			Sequence::createWithTwoActions(DelayTime::create(4.0f), 
			CallFunc::create(CC_CALLBACK_0(
				GameManager::startTimer, 
				GameManager::getInstance(), 
				sitDir::SOUTH_DIR, 
				timerType::DING_QUE))));

		HNAudioEngine::getInstance()->stopBackgroundMusic();
	}

	void GameTableLogic::onDongNt(const tagDongNtEx& msg)
	{
		_msgtagDongNtEx = msg;
	}

	void GameTableLogic::onSeziSureOpenPoint(const tagTwoSeziDirAndGetPaiEx& msg)
	{
		log("\n onSeziSureOpenPoint : ");

		_msgtagTwoSeziDirAndGetPaiEx = msg;

		// 开始投骰子
		auto dir = getUserDir(msg.byUser);
		auto pos = COCOS_NODE(Text, StringUtils::format("touziStart%d", dir))->getPosition();
		auto directSZ =Director::getInstance()->getVisibleSize();
		// 屏幕中间产生2个随机的目标点
		Vec2 touzi1EndPos, touzi2EndPos;
		auto rdWidth = 50.0f;
		auto rdHeight = 50.0f;
		auto startX = directSZ.width / 1280.0f * ((1280.0f - rdWidth)/2.0f);
		auto startY = directSZ.height / 720.0f * ((720.0f - rdHeight)/2.0f);

		srand(time(0));
		touzi1EndPos.x = startX + rdWidth * CCRANDOM_0_1();
		touzi1EndPos.y = startY + rdHeight * CCRANDOM_0_1();

		touzi2EndPos.x = touzi1EndPos.x +rdWidth / 2.0f + rdWidth * CCRANDOM_0_1();
		touzi2EndPos.y = touzi1EndPos.y +rdHeight / 2.0f + rdHeight * CCRANDOM_0_1();
		// moveto
		_callBack->touziAction(dir, pos, msg.bySezi0, msg.bySezi1, touzi1EndPos, touzi2EndPos);
	}

	void GameTableLogic::onSendAllCard(const tagSendPaiEx& msg)
	{
		log("\n onSendAllCard :");
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

		_callBack->startSendBottomCard(vv);
	}	

	void GameTableLogic::onGamePlay(const tagBeginOutPaiEx& msg)
	{
		_msgtagBeginOutPaiEx = msg;
		_callBack->setPlayGame(true);
		COCOS_NODE(Sprite, "waiting")->setVisible(false);
		COCOS_NODE(Sprite, "queMen")->setVisible(false);
		GameManager::getInstance()->getVecUser().at(0)->getOwnPool()->sortCard();	// 整理手牌	
		GameManager::getInstance()->startOutCard(getUserDir(msg.byNt));				// 第一次出牌时
		log("\n onGamePlay :");
		GameManager::getInstance()->setQueMen(msg.byQuePai[_userInfo.bDeskStation]);

		// 设置缺门
		for (auto i = 0; i < PLAY_COUNT; i++)
		{
			auto dir = getUserDir(i);
			auto num = mahjongColor(msg.byQuePai[i]);
			std::string head;
			switch (num)
			{
			case WAN:
				{
					head = "que_wanSP";
					break;
				}
			case TIAO:
				{
					head = "que_tiaoSP";
					break;
				}
			case TONG:
				{
					head = "que_tongSP";
					break;
				}
			default:
				break;
			}
			COCOS_NODE(Sprite, head + StringUtils::format("%d", dir))->setVisible(true);
		}

		// 开始出牌
		GameManager::getInstance()->startTimer(getUserDir(msg.byNt), timerType::WAIT_OUT);

		log("byNt  %d   ", msg.byNt);
		log("byQuePai  : ");
		for (auto i = 0; i < PLAY_COUNT; i++)
		{
			log("%d  ", msg.byQuePai[i]);
		}
		log("byUser  %d   ", msg.byUser);
	}

	void GameTableLogic::onNotifyOutCard(const tagOutPaiEx& msg)
	{
		_msgtagOutPaiEx = msg;

		auto number = msg.byPs % 10;
		auto color = msg.byPs / 10;

		auto startNum = 0;
		auto endNum = 0;
		string head = "";
		auto index = INT(getUserDir(msg.byUser)) - INT(sitDir::SOUTH_DIR);
		int soundNameNum[3][9] = {
			{2, 0, 0, 1, 0, 0, 1, 2, 0},
			{2, 0, 1, 1, 0, 0, 2, 2, 1},
			{1, 1, 1, 0, 0, 0, 3, 1, 1}
		};
		string headArray[] = { "Wan_", "Tiao_", "Tong_"};
		head = StringUtils::format("%s%d_", headArray[color].c_str(), number);
		endNum = soundNameNum[color][number-1];
		GameManager::getInstance()->playEffect(startNum, endNum, head, index);

		log("onNotifyOutCard :");

		auto dir = getUserDir(msg.byUser);
		_callBack->outCard(dir, msg.byPs);  // 出牌

	}

	void GameTableLogic::onNotifyZhuaPai(const tagZhuaPaiEx& msg)
	{
		_msgtagZhuaPaiEx = msg;

		HNAudioEngine::getInstance()->playEffect((MUSIC_PATH + "zhuapai.mp3").c_str());

		log("onNotifyZhuaPai \n");
		
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

	void GameTableLogic::onNotifyBlock(const tagCPGNotifyEx& msg)
	{
		_msgtagCPGNotifyEx = msg;
		log("onNotifyBlock \n");
		log("byPs  %d ", msg.byPs);
		log("byUser  %d ", msg.byUser);
	}

	void GameTableLogic::onNotifyHaveAction(const SC_NotifyHaveAction& msg)
	{
		log("SC_NotifyHaveAction \n");
		log("deskstation %d \n", msg.byDeskStation);
		HNAudioEngine::getInstance()->playEffect((MUSIC_PATH + "Block.mp3").c_str());
		GameManager::getInstance()->startTimer(getUserDir(msg.byDeskStation), timerType::WAIT_BLOCK);
	}

	void GameTableLogic::onNotifyChiPai(const tagChiPai& msg)
	{
		log("onNotifyChiPai \n");
		_msgtagChiPai = msg;
		GameManager::getInstance()->setLastActionCard(getUserDir(msg.byUser), msg.byPs);
	}

	void GameTableLogic::onNotifyPengPai(const tagPengPaiEx& msg)
	{
		log("onNotifyPengPai \n");
		log("byPs  %d ", msg.byPs);
		log("byUser  %d ", msg.byUser);
		_msgtagPengPaiEx = msg;
		GameManager::getInstance()->setLastActionCard(getUserDir(msg.byUser), msg.byPs);

		auto dir = getUserDir(msg.byUser);
		auto index = INT(dir) - INT(sitDir::SOUTH_DIR);
		auto action = GameManager::getInstance()->_touchCard;
		action->setInfo(GameManager::getInstance()->getVecUser().at(index)->getOwnPool(), msg.byPs);
		GameManager::getInstance()->playEffect(0, 4, "Peng_", index);
	}

	void GameTableLogic::onNotifyGangPai(const tagGangPaiEx& msg)
	{
		log("onNotifyGangPai \n");
		log("byUser  %d ", msg.byUser);
		log("byPs  %d ", msg.byPs);
		log("byType  %d ", msg.byType);

		_msgtagGangPaiEx = msg;

		GameManager::getInstance()->setLastActionCard(getUserDir(msg.byUser), msg.byPs);
		
		auto dir = getUserDir(msg.byUser);
		auto index = INT(dir) - INT(sitDir::SOUTH_DIR);

		COCOS_NODE(Sprite, "gfxy")->setVisible(true);
		COCOS_NODE(Sprite, "gfxy")->runAction(
			Sequence::createWithTwoActions(DelayTime::create(4.0f), CallFunc::create(CC_CALLBACK_0(Node::setVisible, COCOS_NODE(Sprite, "gfxy"), false))));
		// COCOS_NODE(Text, "gName1")
		for (auto i = 0; i < PLAY_COUNT; i++)
		{
			COCOS_NODE(Text, StringUtils::format("gName%d", i+1))->setString(getUserBySeatNo(i)->nickName);
			COCOS_NODE(Text, StringUtils::format("gScore%d", i+1))->setString(StringUtils::format("%d", msg.iGangFen[i]));
		}
		if (msg.byType == ACTION_MING_GANG)
		{
			GameManager::getInstance()->playEffect(0, 1, "GuaFeng_", index);
		}
		else
		{
			GameManager::getInstance()->playEffect(0, 4, "XiaYu_", index);
		}

		PoolAction * action = nullptr;
		if (msg.byType == ACTION_MING_GANG)   // 明杠
		{
			action = GameManager::getInstance()->_meldedKong;
		}
		else if (msg.byType == ACTION_AN_GANG)  // 暗杠
		{
			action = GameManager::getInstance()->_concealedKong;
		}
		else if (msg.byType == ACTION_BU_GANG)   // 补杠/碰杠
		{
			action = GameManager::getInstance()->_touchKong;
		}
		assert(action != nullptr);
		action->setInfo(GameManager::getInstance()->getVecUser().at(index)->getOwnPool(), msg.byPs);
	}

	void GameTableLogic::onNotifyHuPai(const tagHuPaiEx& msg)
	{
		log("onNotifyHuPai \n");
		_msgtagHuPaiEx = msg;

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

		COCOS_NODE(Sprite, "hubg")->setVisible(true);
		
		COCOS_NODE(Text, "hu_name1")->setString("");
		COCOS_NODE(Text, "hu_name2")->setString("");
		COCOS_NODE(Text, "hu_name3")->setString("");
		COCOS_NODE(Text, "hu_name4")->setString("");

		COCOS_NODE(Sprite, "hu_fangpao1")->setVisible(false);
		COCOS_NODE(Sprite, "hu_hupai1")->setVisible(false);
		COCOS_NODE(Sprite, "hu_zimo1")->setVisible(false);

		COCOS_NODE(Sprite, "hu_fangpao2")->setVisible(false);
		COCOS_NODE(Sprite, "hu_hupai2")->setVisible(false);
		COCOS_NODE(Sprite, "hu_zimo2")->setVisible(false);

		COCOS_NODE(Sprite, "hu_fangpao3")->setVisible(false);
		COCOS_NODE(Sprite, "hu_hupai3")->setVisible(false);
		COCOS_NODE(Sprite, "hu_zimo3")->setVisible(false);

		COCOS_NODE(Sprite, "hu_fangpao4")->setVisible(false);
		COCOS_NODE(Sprite, "hu_hupai4")->setVisible(false);
		COCOS_NODE(Sprite, "hu_zimo4")->setVisible(false);

		if (!msg.bDuoXiang)
		{
			if (msg.bZimo)
			{
				COCOS_NODE(Sprite, "hu_zimo1")->setVisible(true);
				COCOS_NODE(Text, "hu_name1")->setString(getUserBySeatNo(msg.byUser)->nickName);
			}
			else
			{
				COCOS_NODE(Sprite, "hu_hupai1")->setVisible(true);
				COCOS_NODE(Text, "hu_name1")->setString(getUserBySeatNo(msg.byUser)->nickName);
				COCOS_NODE(Sprite, "hu_fangpao2")->setVisible(true);
				COCOS_NODE(Text, "hu_name2")->setString(getUserBySeatNo(msg.byDianPao)->nickName);
			}
			
			// parse xml
			tinyxml2::XMLDocument doc;
			auto fullXMLname = FileUtils::getInstance()->fullPathForFilename((PLIST_PATH + "HuPai.xml").c_str());

			ssize_t bufferSize;
			auto pBuffer = FileUtils::getInstance()->getFileData(fullXMLname.c_str(), "rb", &bufferSize);
			pBuffer[bufferSize] = '\0';
			doc.Parse((const char*)pBuffer);

			auto root = doc.FirstChildElement();

			// type
			int maxTypeNum = 6;  // 最多显示类型数目
			for (auto j = 0; j < maxTypeNum; j++)
			{
				string huName = getHuType(msg.byHuType[msg.byUser][j]);  
				COCOS_NODE(Text,  StringUtils::format("huType%d", j+1))->setString(StringUtils::format("%s", huName.c_str()));
			}

			auto dir = getUserDir(msg.byUser);
			auto index = INT(dir) - INT(sitDir::SOUTH_DIR);

			auto action = GameManager::getInstance()->_huCard;
			(dynamic_cast<HuCard *>(action))->setZiMoQiangGame(msg.bZimo, msg.bQiangGang);
			action->setInfo(GameManager::getInstance()->getVecUser().at(index)->getOwnPool(), msg.byPs);
		}
		else
		{
			COCOS_NODE(Text, "hu_name1")->setString(getUserBySeatNo(msg.byDianPao)->nickName);
			COCOS_NODE(Sprite, "hu_fangpao1")->setVisible(true);
			auto count = 2;
			for (auto i = 0; i < PLAY_COUNT; i++)
			{
				if (msg.bCanH[i])
				{
					COCOS_NODE(Text, StringUtils::format("hu_name%d", count))->setString(getUserBySeatNo(i)->nickName);
					COCOS_NODE(Sprite, StringUtils::format("hu_hupai%d", count))->setVisible(true);

					// type
					for (auto j = 0; j < 4; j++)
					{
						string huName = getHuType(msg.byHuType[i][j]);
						COCOS_NODE(Text,  StringUtils::format("huType%d", j+1))->setString(StringUtils::format("%s", huName.c_str()));
					}

					auto dir = getUserDir(i);
					auto index = INT(dir) - INT(sitDir::SOUTH_DIR);

					auto action = GameManager::getInstance()->_huCard;
					(dynamic_cast<HuCard *>(action))->setZiMoQiangGame(msg.bZimo, msg.bQiangGang);
					action->setInfo(GameManager::getInstance()->getVecUser().at(index)->getOwnPool(), msg.byPs);

					++count;
				}
			}
		}
	}

	void GameTableLogic::onNotifySuperSetPai(const SuperStruct& msg)
	{
		log("onNotifySuperSetPai \n");
		log("bySetType  %d ", msg.bySetType);

		_msgSuperStruct = msg;
	}

	void GameTableLogic::onNotifyFinish(const tagCountFenEx& msg)
	{
		log("onNotifyFinish : ");

		_msgtagCountFenEx = msg;

		_callBack->setPlayGame(false);

		// 是否黄庄
		auto bs = true;
		for (auto i = 0; i < PLAY_COUNT; i++)
		{
			
			if (msg.bHu[i] == true)
			{
				bs = false;
				break;
			}
		}

		// 显示玩家胡牌类型
		int maxTypeNum = 6;
		string typeHuName = "";
		for (auto i = 0; i < PLAY_COUNT; i++)
		{
			for (auto j = 0; j < maxTypeNum; j++)
			{
				if (msg.byHuType[i][j] != 255)
				{
					typeHuName = getHuType(msg.byHuType[i][j]);
				}
				else
				{
					typeHuName = "";
				}
				COCOS_NODE(Text, StringUtils::format("gHuType%d_%d", j+1, i+1))->setString(typeHuName.c_str());
			}
		}


		COCOS_NODE(Sprite, "huangzhuang")->setVisible(bs);

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

		COCOS_NODE(Sprite, "finish")->setVisible(true);
		GameManager::getInstance()->showMoney();
		COCOS_NODE(Sprite, "finish")->runAction(Sequence::create(
			DelayTime::create(_msgGameStationDataEx.byBeginTime),
			CallFunc::create(CC_CALLBACK_0(Node::setVisible, COCOS_NODE(Sprite, "finish"), false)),
			CallFunc::create(CC_CALLBACK_0(GameManager::restartGame, GameManager::getInstance())),
			nullptr
			));
		for (auto i = 0; i < PLAY_COUNT; i++)
		{
			COCOS_NODE(Text, StringUtils::format("gNameIDHu%d", i+1))->setString(GBKToUtf8(getUserBySeatNo(i)->nickName));

			COCOS_NODE(Text, StringUtils::format("gNameID%d", i + 1))->setString(GBKToUtf8(getUserBySeatNo(i)->nickName));
			COCOS_NODE(Text, StringUtils::format("gScoreHU%d", i+1))->setString(StringUtils::format("%d", msg.iHuFen[i]));
			COCOS_NODE(Text, StringUtils::format("gScoreGFXY%d", i+1))->setString(StringUtils::format("%d", msg.iGangFen[i]));
			COCOS_NODE(Text, StringUtils::format("gChaJiao%d", i+1))->setString(StringUtils::format("%d", msg.m_iChaJiao[i]));
			COCOS_NODE(Text, StringUtils::format("gChaHuaZhu%d", i+1))->setString(StringUtils::format("%d", msg.m_iHuaZhu[i]));

			COCOS_NODE(Text, StringUtils::format("gNameID%d%d", i + 1, i + 1))->setString(GBKToUtf8(getUserBySeatNo(i)->nickName));
			COCOS_NODE(Text, StringUtils::format("gScoreEarn%d", i+1))->setString(StringUtils::format("%d", msg.iZongFen[i]));
		}

	}

	void GameTableLogic::onForceFinish(const tagCountFenEx& msg)
	{
		log("onNotifyFinish : ");

		_msgtagCountFenEx = msg;
		_callBack->setPlayGame(false);

		if (msg.byUser == _userInfo.bDeskStation)
		{
			sendUserUp();
		}
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
		auto menNum = 26;
		for (auto i = 0; i < PLAY_COUNT; i++)
		{
			operIndex = getUserVecIndex(i);
			if (getUserDir(i) == sitDir::SOUTH_DIR || getUserDir(i) == sitDir::NORTH_DIR)
			{
				menNum = 28;
			}
			else
			{
				menNum = 26;
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

	std::string GameTableLogic::getHuType(INT type)
	{
		// parse xml
		tinyxml2::XMLDocument doc;
		auto fullXMLname = FileUtils::getInstance()->fullPathForFilename((PLIST_PATH + "HuPai.xml").c_str());

		ssize_t bufferSize;
		auto pBuffer = FileUtils::getInstance()->getFileData(fullXMLname.c_str(), "rb", &bufferSize);
		pBuffer[bufferSize] = '\0';
		doc.Parse((const char*)pBuffer);

		auto root = doc.FirstChildElement();

		std::string huName;
		switch (type)
		{
		case HUPAI_HU_PAI : 	//能胡（素番）1番 1分
			{
				huName = root->FirstChildElement("pingHu")->GetText();
				break;
			}
		case HUPAI_PENG_PENG_HU:		//大对子（碰碰糊）2番 2分
			{
				huName  = root->FirstChildElement("pengPengHu")->GetText();
				break;
			}
		case HUPAI_QING_YI_SE:		//清一色3番
			{
				huName = root->FirstChildElement("qingYiSe")->GetText();
				break;
			}
		case HUPAI_AN_QI_DUI:		//暗七对（即七对子）3番 4分
			{
				huName = root->FirstChildElement("xiaoQiDui")->GetText();
				break;
			}
		case HUPAI_QUAN_DAI_YAO:		//全带幺3番 4分
			{
				huName = root->FirstChildElement("daiYaoJiu")->GetText();
				break;
			}
		case HUPAI_JIANG_DA_DUI:		//将对（258碰碰糊）4番 8分
			{
				huName = root->FirstChildElement("jiangDui")->GetText();
				break;
			}
		case HUPAI_QING_DA_DUI:		//清对（清一色碰碰糊）4番 8分
			{
				huName = root->FirstChildElement("qingDui")->GetText();
				break;
			}
		case HUPAI_QING_QI_DUI:		//清七对（清一色七对子）5番 16分
			{
				huName = root->FirstChildElement("qingQiDui")->GetText();
				break;
			}
		case HUPAI_LONG_QI_DUI:		//龙七对4番  8分
			{
				huName = root->FirstChildElement("longQiDui")->GetText();
				break;
			}
		case HUPAI_QING_LONG_QI_DUI:		//清龙七对6番 32分
			{
				huName = root->FirstChildElement("qingLongQiDui")->GetText();
				break;
			}
		case HUPAI_TYPE_GANG_KAI:		// 杠开 1番
			{
				huName = root->FirstChildElement("gangKai")->GetText();
				break;
			}
		case HUPAI_TYPE_GANG_PAO:		// 杠后炮 1番
			{
				huName = root->FirstChildElement("gangHouPao")->GetText();
				break;
			}
		case HUPAI_TYPE_TIAN_HU:		// 天胡 4番
			{
				huName = root->FirstChildElement("tianHu")->GetText();
				break;
			}
		case HUPAI_TYPE_DI_HU:		// 地胡 4番
			{
				huName = root->FirstChildElement("diHu")->GetText();
				break;
			}
		case HUPAI_TYPE_QIANG_GANG:		// 抢杠 1番
			{
				huName = root->FirstChildElement("qiangGang")->GetText();
				break;
			}
		case HUPAI_YAO_JIU_QI_DUI:	// 幺九七对8番 128分
			{
				huName = root->FirstChildElement("daiYaoJiuXiaoQiDui")->GetText();
				break;
			}
		case HUPAI_QING_DAI_YAO:		// 清带幺5番 16番
			{
				huName = root->FirstChildElement("qingDaiYao")->GetText();
				break;
			}
		case HUPAI_HU_GEN:		//糊根
			{
				huName = root->FirstChildElement("huGen")->GetText();
				break;
			}
		case HUPAI_ZI_MO:							//自摸
			{
				huName = root->FirstChildElement("ziMo")->GetText();
				break;
			}
		default:
			huName = "";
			break;
		}
		return huName;
	}

}
