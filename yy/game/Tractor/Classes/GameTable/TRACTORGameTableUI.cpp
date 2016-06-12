#include "TRACTORGameTableUI.h"
#include "TRACTORGameTableLogic.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "HNLobbyExport.h"
#include "HNLogicExport.h"
#include "TRACTORGameUserMessageBox.h"
#include "HNUIExport.h"
#include "TRACTORGameNotice.h"
#include "TRACTORGameDashboard.h"
#include "TRACTORGameResult.h"

namespace TRACTOR
{
	enum{
		Dashboard_Zorder    =    10,
		TableInfo_Zorder	=    10,
		Max_Zorder          =    100,

		MAX_ZORDER          =    100, //最大层级
		MEMBAN_ZORDER		=	 300,
	};


	static const  char*   JSON_NODE_POSITION	=	"Point8";
	static const  char*   JSON_NODE_DEALER		=	"Dealer";

	//////////////////////////////////////////////////////////////////////////
	//牌值-牌图片相互转换 
	//牌图片中的值（如方块2）   -    游戏中表示这张牌的值（0x01）
	//牌的花色： {方块，梅花，红桃，黑桃， 大/小王}    -  {0 ，1，2，3，4}
	//牌的值（以方块为例）： {2 ， A}  -  {0X01  ，0X0D }
	//特殊  大/小王    0X4E / 0X4F 
	//////////////////////////////////////////////////////////////////////////

	namespace HN
	{
		// 启动游戏客户端
		HNGameUIBase* startGameClient(BYTE bDeskIndex, bool bAutoCreate)
		{
			auto table = GameTableUI::create(bDeskIndex, bAutoCreate);
			if (nullptr != table)
			{
				auto scene = Scene::create();
				scene->addChild(table);
				Director::getInstance()->replaceScene(scene);
			}
			return table;
		}
	}

	HNGameUIBase* GameTableUI::create(BYTE bDeskIndex, bool bAutoCreate)
	{
		GameTableUI* tableUI = new GameTableUI();
		if (tableUI->init(bDeskIndex, bAutoCreate))
		{
			tableUI->autorelease();
			return tableUI;
		}
		else
		{
			CC_SAFE_DELETE(tableUI);
			return nullptr;
		}
	}

	void GameTableUI::showResulte(const std::vector<TGameResult>& results , bool isWin_me)
	{
		for (BYTE i = 0 ; i < results.size(); ++i)
		{
			BYTE seatNo = results[i].viewSeatNo;
			if (seatNoIsOk(seatNo) && _players[seatNo] != nullptr)
			{
				_players[seatNo]->setChip(results[i].i64Money + results[i].llMoney);
			}
		}

		auto wsize = Director::getInstance()->getWinSize();
		auto resulte = GameResulte::create(results , isWin_me);
		resulte->setAnchorPoint(Vec2(0.5f , 0.5f));
		resulte->setPosition(wsize.width / 2 , wsize.height / 2 + 40);
		this->addChild(resulte , 400);
		return ;
	}

	bool GameTableUI::init(BYTE bDeskIndex, bool bAutoCreate)
	{
		if (!HNLayer::init())
		{
			return false;
		}

		Size size = Director::getInstance()->getVisibleSize();
		this->ignoreAnchorPointForPosition(false);
		this->setPosition(size.width / 2, size.height / 2);

		auto dispatcher = Director::getInstance()->getEventDispatcher();
		auto listener = EventListenerTouchOneByOne::create();
		listener->onTouchBegan = CC_CALLBACK_2(GameTableUI::onTouchBegan, this);
		dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
		initPosition();
		inflateLayout();
		initGameData();

		//荷官处理类
		_dealer = GameDelearUI::create();
		_dealer->setPosition(0, 0);
		addChild(_dealer, MAX_ZORDER - 1);

		std::vector<Vec2> seatPos(PLAY_COUNT);
		std::vector<Vec2> tablePos(PLAY_COUNT);
		for (int i = 0; i < PLAY_COUNT; i++)
		{
			seatPos[i] = _tableUI.wOutline->convertToWorldSpace(_seatPosition[i]);
			tablePos[i] = _tableUI.wOutline->convertToWorldSpace(_jettonPosition[i]);
		}

		_dealer->setTablePoint(tablePos);
		_dealer->setSeatPoint(seatPos);
		_dealer->setDealerPoint(_tableUI.wOutline->convertToWorldSpace(_dealerPostion));

		//牌桌逻辑类
		_tableLogic = new GameTableLogic(this, bDeskIndex, bAutoCreate);
		_tableLogic->clearDesk();
		_tableLogic->loadDeskUsersUI();

		return true;
	}

	void GameTableUI::onExit()
	{
		SpriteFrameCache* cache = SpriteFrameCache::getInstance();
		cache->removeSpriteFramesFromFile("tractor/table/cards.plist");
		HNGameUIBase::onExit();
	}

	bool GameTableUI::onTouchBegan(Touch *touch, Event *unused_event)
	{
		Vec2 touchPos = touch->getLocation();
		auto size = Director::getInstance()->getWinSize();
		if (_tableUI.wPullDownMenu->getPosition().y < size.height)
		{
			auto des_pos = Vec2(_tableUI.wPullDownMenu->getPositionX(), size.height + 10);
			_tableUI.wPullDownMenu->stopAllActions();
			auto mveTo = MoveTo::create(0.2f, des_pos);
			_tableUI.wPullDownMenu->runAction(mveTo);

			return false;
		}
		return true;
	}

	void GameTableUI::menuClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype)
	{
		if (Widget::TouchEventType::ENDED != touchtype)
		{
			return;
		}

		Button* ptr = (Button*)pSender;
		std::string name = ptr->getName();
		if (name.compare("start_button") == 0)
		{
			ptr->setVisible(false);
			_tableLogic->sendAgreeGame();
		}
		else if(name.compare("btn_cardtype") == 0)
		{
			showCardType();
		}
		else if(name.compare("btn_menu") == 0)
		{
			auto size = Director::getInstance()->getWinSize();
			Size rt = _tableUI.wPullDownMenu->getContentSize();
			auto des_pos = Vec2(_tableUI.wPullDownMenu->getPositionX(), size.height - rt.height - 2);
			_tableUI.wPullDownMenu->stopAllActions();
			auto mveTo = MoveTo::create(0.2f, des_pos);
			_tableUI.wPullDownMenu->runAction(mveTo);
		}
	}

	void GameTableUI::seatClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype)
	{
		if (Widget::TouchEventType::ENDED != touchtype)
		{
			return;
		}

		for (int i = 0; i < PLAY_COUNT; i++)
		{
			if (pSender == _tableUI.bSeat[i])
			{
				_tableLogic->sendUserSit(i);
				break;
			}
		}
	}

	void GameTableUI::compareClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype)
	{
		if (Widget::TouchEventType::ENDED != touchtype)
		{
			return;
		}

		for (int i = 0; i < PLAY_COUNT; i++)
		{
			if (pSender == _tableUI.bCompare[i])
			{
				_tableLogic->sendCompare(i);
				break;
			}
		}
		hideVSButton();
	}


	void GameTableUI::pullDownItemCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype)
	{
		if (Widget::TouchEventType::ENDED != touchtype)
		{
			return;
		}
		Size winSize = Director::getInstance()->getWinSize();
		auto pButton = (Button*)pSender;

		if (pButton == _tableUI.bJumpMenu[0])			//站起子按钮      
		{
			_tableLogic->sendStandUp();
		}
		else if (pButton == _tableUI.bJumpMenu[1])			//设置子按钮
		{
			pButton->setTouchEnabled(false);
			auto box = GameSetLayer::create();
			box->setPosition(Vec2(winSize.width, 0));
			addChild(box, 105);
			box->runAction(Sequence::create(EaseBackOut::create(MoveBy::create(0.2f, Vec2(-winSize.width, 0))),
				CallFunc::create(CC_CALLBACK_0(GameTableUI::menuTouchReset, this, pButton)), nullptr));
		}
		else if (pButton == _tableUI.bJumpMenu[2])			//牌型
		{
			showCardType();
		}

		auto des_pos = Vec2(0, winSize.height  + 10);
		_tableUI.wPullDownMenu->stopAllActions();
		auto mveTo = MoveTo::create(0.2f, des_pos);
		_tableUI.wPullDownMenu->runAction(mveTo);
	}

	//按钮点击功能复位
	void GameTableUI::menuTouchReset(Button* btn)
	{
		btn->setTouchEnabled(true);
	}

	void GameTableUI::compareCardReq()
	{
		_tableLogic->compareCardReq();
	}

	void GameTableUI::lookCard()
	{
		_tableLogic->sendLook();
	}

	void GameTableUI::compareCard()
	{
		//_tableLogic->sendCompare();
	}

	void GameTableUI::followNote()
	{
		_tableLogic->sendFollow();
	}

	void GameTableUI::giveUpNote()
	{
		_tableLogic->sendGiveUp();
	}

	void GameTableUI::hideVSButton()
	{
		for(auto pMenu: _tableUI.bCompare)
		{
			pMenu->setVisible(false);
		}
	}

	void GameTableUI::addNote(LLONG money)
	{
		_tableLogic->sendAdd(money);
	}

	void GameTableUI::initGameData()
	{
		memset(_players, 0, sizeof(_players));
	}

	void GameTableUI::initPosition()
	{
		std::string filename = "tractor/position.json";
		if (!FileUtils::getInstance()->isFileExist(filename))
		{
			return;
		}
		std::string json = FileUtils::getInstance()->getStringFromFile(filename);
		rapidjson::Document doc;
		doc.Parse<rapidjson::kParseDefaultFlags>(json.c_str());
		if (doc.HasParseError())
		{
			return;
		}
		if (doc.IsObject() && doc.HasMember("Position"))
		{
			const rapidjson::Value &val = doc["Position"];
			if (val.IsArray())
			{
				for (rapidjson::SizeType i = 0; i < val.Size(); ++i)
				{
					const rapidjson::Value &node = val[i];

					//读取玩家下注筹码的位置
					if (node.HasMember(JSON_NODE_POSITION))
					{
						const rapidjson::Value &nd = node[JSON_NODE_POSITION];
						if (nd.IsArray())
						{
							for (rapidjson::SizeType j = 0; j < nd.Size(); ++j)
							{
								const rapidjson::Value &vitem = nd[j];
								const rapidjson::Value & vx = vitem["x"];
								const rapidjson::Value & vy = vitem["y"];
								_jettonPosition[j].x = vx.GetDouble();
								_jettonPosition[j].y = vy.GetDouble();
							}
						}
						continue;
					}

					//读取荷官位子
					if (node.HasMember(JSON_NODE_DEALER))
					{
						const rapidjson::Value &nd = node[JSON_NODE_DEALER];
						const rapidjson::Value &ndx = nd["x"];
						const rapidjson::Value &ndy = nd["y"];
						_dealerPostion.x = ndx.GetDouble();
						_dealerPostion.y = ndy.GetDouble();
					}
				}
			}
		}

	}

	int GameTableUI::getPlayerCount()
	{
		int count = 0;
		for (BYTE i = 0; i < PLAY_COUNT; ++i)
		{
			if (nullptr == _players[i])
			{
				continue;
			}
			count++;
		}
		return count;
	}


	GameTableUI::GameTableUI()
		: _dashboard(nullptr)
	{
		memset(_players, 0, sizeof(_players));
	}

	GameTableUI::~GameTableUI()
	{
		HN_SAFE_DELETE(_tableLogic);
	}

	PlayerUI* GameTableUI::getPlayer(int seatNo)
	{
		return (seatNoIsOk(seatNo) ? _players[seatNo] : nullptr);
	}

	void GameTableUI::showCardType()
	{
		auto layer = this->getChildByName("card_type_layer");
		if(layer != nullptr)
		{
			return;
		}

		auto winSize = Director::getInstance()->getWinSize();
		auto cardTypeLayer = GameCardType::create();
		auto size = cardTypeLayer->getContentSize();
		cardTypeLayer->setVisible(true);
		cardTypeLayer->setAnchorPoint(Vec2(1, 0));
		cardTypeLayer->ignoreAnchorPointForPosition(false);
		cardTypeLayer->setPosition(Vec2(winSize.width + size.width, 0));
		this->addChild(cardTypeLayer, 101);
		cardTypeLayer->setName("card_type_layer");

		auto dest = Vec2(winSize.width, 0);
		auto moveTo = MoveTo::create(0.2f, dest);
		cardTypeLayer->runAction(moveTo);
	}

	/**************************************************************/
	void GameTableUI::showHandCard(std::vector<THandCard>& cards)
	{
		_dealer->dealHandCard(cards);
	}

	/************************************************************************/
	void GameTableUI::showUser(BYTE seatNo, bool bMe, bool sex)
	{
		if (!seatNoIsOk(seatNo))
		{
			return;
		}

		PlayerUI* player = PlayerUI::create(_tableLogic->getUserId(seatNo));
		player->setAnchorPoint(Vec2(0.5f, 0.5f));

		player->setPosition(_seatPosition[seatNo]);
		player->setSex(sex);
		_tableUI.wOutline->addChild(player, MAX_ZORDER - 1);

		player->setTableUI(this);

		_players[seatNo] = player;

		_tableUI.bSeat[seatNo]->setVisible(false);
		if (bMe)
		{
			for (BYTE i = 0; i < PLAY_COUNT; i++)
			{
				_tableUI.bSeat[i]->setTouchEnabled(false);
			}
		}
	}

	void GameTableUI::showUserUp(BYTE seatNo, bool bMe)
	{
		if (seatNoIsOk(seatNo) && nullptr != _players[seatNo])
		{
			_players[seatNo]->removeFromParentAndCleanup(true);
			_players[seatNo] = nullptr;
		}
		_tableUI.bSeat[seatNo]->setVisible(true);

		if (bMe)
		{
			for (BYTE i = 0; i < PLAY_COUNT; i++)
			{
				// 人走了  箭头跳起来
				if (nullptr == _players[i])
				{
					_tableUI.bSeat[i]->setTouchEnabled(true);
				}

			}
		}
	}

	void GameTableUI::showUserName(BYTE seatNo, CHAR* name)
	{
		if (seatNoIsOk(seatNo) && _players[seatNo] != nullptr)
		{
			_players[seatNo]->setName(GBKToUtf8(name));
		}
	}

	void GameTableUI::showUserMoney(BYTE seatNo, LLONG MONEY)
	{
		if (seatNoIsOk(seatNo) && _players[seatNo] != nullptr)
		{
			_players[seatNo]->setChip(MONEY);
		}
	}

	void GameTableUI::showDealer(BYTE seatNo)
	{
		_dealer->dealButton(seatNo);
	}

	void GameTableUI::showReady(bool visible)
	{
		if (_tableUI.bReady != nullptr)
		{
			_tableUI.bReady->setVisible(visible);
		}
	}

	void GameTableUI::getReady()
	{
		if (_tableUI.bReady->isVisible())
		{
			_tableLogic->sendStandUp();
		}
	}

	void GameTableUI::showAddRange(LLONG min, LLONG max)
	{
		_dashboard->setAddRange(min, max);
	}


	void GameTableUI::showUserNoteMoney(BYTE seatNo, LLONG money)
	{
		_dealer->betMoney(seatNo, money);
		auto player = getPlayer(seatNo);
		if(player != nullptr)
		{
			player->note(money);
		}
	}

	void GameTableUI::showWin(BYTE winnerSeatNo)
	{
		//std::vector<std::vector<LLONG>> winPool;
		//_dealer->dealWinPool(winPool);

		// recycle all chips to the winner
		_dealer->recycleChips(winnerSeatNo);
	}


	//显示玩家手牌
	void GameTableUI::showUserHandCard(BYTE seatNo, const std::vector<BYTE>& cards)
	{
		if (seatNoIsOk(seatNo) && _players[seatNo] != nullptr)
		{
			_players[seatNo]->setHandCard(0, cards[0]);
			_players[seatNo]->setHandCard(1, cards[1]);
			_players[seatNo]->setHandCard(2, cards[2]);
		}
	}


	void GameTableUI::clearDesk()
	{
		for (BYTE i = 0; i < PLAY_COUNT; ++i)
		{
			if (nullptr != _players[i])
			{
				_players[i]->hideHandCard(false);
				_players[i]->stopWait();
				_players[i]->stopPlayWin();
			}
		}
		_dealer->clear();
	}


	void GameTableUI::leaveDesk()
	{
		//GameDesk::createDesk(RoomLogic()->getSelectedRoom());
		GamePlatform::returnPlatform(LayerType::DESKLIST);
	}

	//显示牌桌准备
	void GameTableUI::showLoading(bool bVisible)
	{
		if (bVisible)
		{
			_loadSprite = Sprite::create("table/loading_view.jpg");
			_loadSprite->setVisible(true);
			Size frameSize = Director::getInstance()->getOpenGLView()->getFrameSize();
			_loadSprite->setAnchorPoint(Vec2(0, 0));
			_loadSprite->setPosition(Vec2(0, 0));
			this->addChild(_loadSprite, MEMBAN_ZORDER);
		}
		else
		{
			auto fadeOut = FadeOut::create(2.0f);
			_loadSprite->runAction(fadeOut);
		}
	}

	/*
	* rotate player's postion
	* 1.player
	* 2.seat
	* 3.compare button
	*/
	void GameTableUI::rotateSeat(int offset)
	{
		PlayerUI* oldPlayers[PLAY_COUNT] = { 0 };
		PlayerUI* newPlayers[PLAY_COUNT] = { 0 };
		ui::Button* oldSeat[PLAY_COUNT] = { 0 };
		ui::Button* newSeat[PLAY_COUNT] = { 0 };
		ui::Button* newCompare[PLAY_COUNT] = {0};
		ui::Button* oldCompare[PLAY_COUNT] = {0};

		memcpy(oldPlayers, _players, sizeof(_players));
		memcpy(oldSeat, _tableUI.bSeat, sizeof(_tableUI.bSeat));
		memcpy(oldCompare, _tableUI.bCompare, sizeof(_tableUI.bCompare));

		//先更新位置，防止消息处理后位置出错
		for (unsigned int i = 0; i < PLAY_COUNT; i++)
		{
			int index = (i + offset + PLAY_COUNT) % PLAY_COUNT;
			if (_players[i] != nullptr)
			{
				newPlayers[index] = _players[i];
			}
			newCompare[index] = _tableUI.bCompare[i];
			newSeat[index]  = _tableUI.bSeat[i];
		}
		memcpy(_players, newPlayers, sizeof(_players));
		memcpy(_tableUI.bSeat, newSeat, sizeof(_tableUI.bSeat));
		memcpy(_tableUI.bCompare, newCompare, sizeof(_tableUI.bCompare));

		//play animation with old pointer.
		int rotateTimes = abs(offset);
		for (int i = 0; i < PLAY_COUNT; i++)
		{
			Vector<FiniteTimeAction*> actionArray;
			Vector<FiniteTimeAction*> compareArray;
			if (offset > 0)
			{//逆时针转
				for (int j = 0; j <= rotateTimes; j++)
				{
					int newPos = (i + j + PLAY_COUNT) % PLAY_COUNT;
					actionArray.pushBack(MoveTo::create(0.3f, _seatPosition[newPos]));
					compareArray.pushBack(MoveTo::create(0.3f, oldCompare[newPos]->getPosition()));
				}
			}
			else
			{//顺时针转
				for (int j = 0; j <= rotateTimes; j++)
				{
					int newPos = (i - j + PLAY_COUNT) % PLAY_COUNT;
					actionArray.pushBack(MoveTo::create(0.2f, _seatPosition[newPos]));
					compareArray.pushBack(MoveTo::create(0.3f, oldCompare[newPos]->getPosition()));
				}
			}
			auto sequence = Sequence::create(actionArray);
			auto compareSequence = Sequence::create(compareArray);
			if (oldPlayers[i] != nullptr)
			{
				oldPlayers[i]->runAction(sequence->clone());
			}
			oldSeat[i]->runAction(sequence);
			oldCompare[i]->runAction(compareSequence);
		}
	}


	void GameTableUI::alertDialog(const std::string& title, const std::string& message)
	{

	}

	void GameTableUI::showNotice(const std::string &message)
	{
		GameNotice* notice = GameNotice::create(this, message);
		this->reorderChild(notice, MAX_ZORDER);
		notice->show();
	}

	void GameTableUI::showUserProfit(BYTE seatNo, LLONG money)
	{
		auto player = getPlayer(seatNo);
		if(player != nullptr)
		{
			player->profit(money);
		}
	}

	void GameTableUI::showTableInfo(const std::string& tableName)
	{
		char str[50] = { 0 };
		sprintf(str, "房间:%s", tableName.c_str());
		_lRoomInfo = Label::createWithSystemFont(GBKToUtf8(str), "", 18);
		_lRoomInfo->setAnchorPoint(Vec2(1, 1));
		auto size = Director::getInstance()->getWinSize();
		_lRoomInfo->setPosition(Vec2(size.width - 10, size.height - 10));
		this->addChild(_lRoomInfo, TableInfo_Zorder);
	}

	void GameTableUI::showWaitTime(BYTE seatNo, BYTE byTime, BYTE byTotalTime)
	{
		if (seatNoIsOk(seatNo) && _players[seatNo] != nullptr)
		{
			if (byTime > 0)
			{
				_players[seatNo]->startWait(byTime, byTotalTime);
			}
			else
			{
				_players[seatNo]->stopWait();
			}
		}
	}

	void GameTableUI::showLimitNote(LLONG note)
	{
		char tmp[50] = { 0 };
		sprintf(tmp, "%lld",  note);
		_tableUI.lLimitNote->setString(tmp);
	}

	void GameTableUI::showBaseNote(LLONG note)
	{
		char tmp[50] = { 0 };
		sprintf(tmp, "%lld", note);
		_tableUI.lNote->setString(tmp);
	}

	void GameTableUI::showLimitPerNote(LLONG note)
	{
		char tmp[50] = { 0 };
		sprintf(tmp, "%lld", note);
		_tableUI.lTopNote->setString(tmp);
	}

	void GameTableUI::showGuoDi(LLONG note)
	{
		char tmp[50] = { 0 };
		sprintf(tmp, "%lld", note);
		_tableUI.lGuoDi->setString(tmp);
	}

	void GameTableUI::showNT(BYTE seatNo)
	{

	}

	void GameTableUI::showTotalNote(LLONG note)
	{
		char tmp[50] = { 0 };
		sprintf(tmp, "%lld", note);
		_tableUI.lTotalNote->setString(tmp);
	}

	void GameTableUI::showUserLookCard(BYTE seatNo, bool isMe)
	{
		if (seatNoIsOk(seatNo) && _players[seatNo] != nullptr)
		{
			_players[seatNo]->playAction(PlayerUI::Action::eLookCard);
			if(!isMe)
			{
				_players[seatNo]->lookHandCard();
			}
		}

	}

	void GameTableUI::showUserGiveUp(BYTE seatNo)
	{
		if (seatNoIsOk(seatNo) && _players[seatNo] != nullptr)
		{
			_players[seatNo]->playAction(PlayerUI::Action::eGiverUp);
			_players[seatNo]->lose();

		}
	}

	void GameTableUI::showUserNote(BYTE seatNo)
	{
		if (seatNoIsOk(seatNo) && _players[seatNo] != nullptr)
		{
			_players[seatNo]->playAction(PlayerUI::Action::eNote);
		}
	}

	void GameTableUI::showUserCompare(BYTE fromSeatNo, BYTE toSeatNo, BYTE winSeatNo)
	{
		//if(seatNoIsOk(fromSeatNo) && _players[fromSeatNo] != nullptr)
		//{
		//	_players[fromSeatNo]->playAction(PlayerUI::Action::eCompareCard);
		//}

		//if(seatNoIsOk(toSeatNo) && _players[toSeatNo] != nullptr)
		//{
		//	_players[toSeatNo]->playAction(PlayerUI::Action::eCompareCard);
		//}
	}

	void GameTableUI::showUserFollow(BYTE seatNo)
	{
		if (seatNoIsOk(seatNo) && _players[seatNo] != nullptr)
		{
			_players[seatNo]->playAction(PlayerUI::Action::eFollow);
		}
	}

	//void GameTableUI::showUserAdd(BYTE seatNo)
	//{
	//	if (seatNoIsOk(seatNo) && _players[seatNo] != nullptr)
	//	{
	//		auto userid = _tableLogic->getUserId(seatNo);
	//		auto userinfo = HNUserInfoModule::getInstance()->findUser(userid);
	//		
	//		// true-man, false-lady
	//		auto sex = userinfo->bBoy;
	//		auto audio = sex ? GOLD_ADD_MAN : GOLD_ADD_LADY;
	//		HNAudioEngine::getInstance()->playEffect(audio);
	//
	//		//_players[seatNo]->setStatus(GBKToUtf8("加注").c_str(), 0);
	//	}
	//}

	void GameTableUI::showUserOpenCard(BYTE seatNo)
	{

	}

	void GameTableUI::showUserAction(BYTE seatNo, BYTE byFlag)
	{
		_dashboard->setActionOption(byFlag);
	}

	void GameTableUI::showDashboard(bool visible)
	{
		visible ? _dashboard->show() : _dashboard->hide();
	}


	void GameTableUI::boom(Ref* pRef)
	{
		Layer* tempLayer = (Layer*)pRef;
		auto losercard0 = (PokerCard*)tempLayer->getChildByTag(0);
		auto losercard1 = (PokerCard*)tempLayer->getChildByTag(1);
		auto losercard2 = (PokerCard*)tempLayer->getChildByTag(2);

		if (losercard0 == nullptr || losercard1 == nullptr || losercard2 == nullptr)
		{
			return;
		}

		losercard0->setGray(true);
		losercard1->setGray(true);
		losercard2->setGray(true);
	}

	void GameTableUI::playThunderAnimate(float duration)
	{
		std::string audio = GOLD_PK_THUNDER;
		//HNAudioEngine::getInstance()->playEffect(audio.c_str(), false, 1.0f);
		HNAudioEngine::getInstance()->playEffect(audio.c_str());

		SpriteFrameCache *frameCache = SpriteFrameCache::getInstance();
		frameCache->addSpriteFramesWithFile("tractor/animation/versus.plist");

		auto animation = Animation::create();
		for (int i = 1; i < 17; i++)
		{
			char szName[100] = { 0 };
			sprintf(szName, "versus%02d.png", i);
			auto frame = frameCache->getSpriteFrameByName(szName);
			animation->addSpriteFrame(frame);
		}

		animation->setDelayPerUnit(duration / 16.0f);
		animation->setRestoreOriginalFrame(true);

		auto action = Animate::create(animation);
		auto spwn = Spawn::create(
			//	CallFunc::create([=](){
			//	std::string audio = GOLD_PK_THUNDER;
			//	HNAudioEngine::getInstance()->playEffect(audio.c_str(), false, 1.0f);
			//}),
			action,
			nullptr);

		Size visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();
		auto sp = Sprite::create();
		sp->setScale(0.8f);
		sp->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
		//sp->runAction(Sequence::create(action, action->reverse(), NULL));
		sp->runAction(Sequence::create(spwn, NULL));
		this->addChild(sp, 100);
	}

	void GameTableUI::showCompareResult(BYTE winSeatNo, BYTE loseSeatNo)
	{
		auto sex = false; // Lady
		auto audio = sex ? GOLD_PK_MAN : GOLD_PK_LADY;
		HNAudioEngine::getInstance()->playEffect(audio);

		auto winner = this->getPlayer(winSeatNo);
		auto loser = this->getPlayer(loseSeatNo);

		auto winnerLayer = Layer::create();
		auto loserLayer = Layer::create();

		this->addChild(winnerLayer, 100);
		this->addChild(loserLayer, 100);

		PokerCard* winnerCards[3];
		PokerCard* loserCards[3];

		////////////////////////////////////////////////////////////////////////
		// init handcards
		Size visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();

		float offset = 15;
		winner->hideHandCard(false);
		loser->hideHandCard(false);
		for (int i = 0; i < 3; i++)
		{
			winnerCards[i] = PokerCard::create(0x00);
			Vec2 tmp = winner->convertToWorldSpace(winner->getAnchorPointInPoints());
			winnerCards[i]->setPosition(tmp.x + offset*i, tmp.y);
			winnerLayer->addChild(winnerCards[i], i);

			loserCards[i] = PokerCard::create(0x00);
			tmp = loser->convertToWorldSpace(loser->getAnchorPointInPoints());
			loserCards[i]->setPosition(tmp.x + offset*i, tmp.y);
			loserLayer->addChild(loserCards[i], i);
		}

		loserCards[0]->setTag(0);
		loserCards[1]->setTag(1);
		loserCards[2]->setTag(2);

		////////////////////////////////////////////////////////////////////////
		// run action, move to  1 -> 2 -> 3 -> 4 -> 2 -> 1	
		Layer* leftLayer;
		Layer* rightLayer;
		PlayerUI* leftPlayer;
		PlayerUI* rightPlayer;

		if (winnerCards[0]->getPosition().x < loserCards[0]->getPosition().x)
		{
			leftLayer = winnerLayer;
			leftPlayer = winner;

			rightLayer = loserLayer;
			rightPlayer = loser;
		}
		else
		{
			leftLayer = loserLayer;
			leftPlayer = loser;

			rightLayer = winnerLayer;
			rightPlayer = winner;
		}

		auto widthOfCards = winnerCards[0]->getContentSize().width;

		// target position 1
		auto leftPosition1 = leftLayer->getPosition();
		auto rightPosition1 = rightLayer->getPosition();

		// target position 2 -> world position
		// Lp-> left poker, Lo-> left layer anchor point
		// Lp1->Lp2=Lp2-Lp1=Lo2-Lo1 > Lo2=Lp2-Lp1+Lo1
		auto Lp1 = leftPlayer->convertToWorldSpace(leftPlayer->getAnchorPointInPoints());
		auto Lp2 = Vec2(origin.x + visibleSize.width * 1 / 3, origin.y + visibleSize.height / 2);
		auto Lo1 = leftPosition1;
		auto Lo2 = Lp2 - Lp1 + Lo1;
		auto leftPosition2 = Lo2;

		auto Rp1 = rightPlayer->convertToWorldSpace(rightPlayer->getAnchorPointInPoints());
		auto Rp2 = Vec2(origin.x + visibleSize.width * 2 / 3, origin.y + visibleSize.height / 2);
		auto Ro1 = rightPosition1;
		auto Ro2 = Rp2 - Rp1 + Ro1;
		auto rightPosition2 = Ro2;

		// move
		auto duration = 0.2f;
		auto leftmove12 = MoveTo::create(duration, leftPosition2);
		auto rightmove12 = MoveTo::create(duration, rightPosition2);

		auto leftmove21 = MoveTo::create(duration, leftPosition1);
		auto rightmove21 = MoveTo::create(duration, rightPosition1);

		// left
		float duration2 = 1.5;
		float duration3 = 1.0;
		leftLayer->runAction(Sequence::create(
			leftmove12,
			CallFunc::create(CC_CALLBACK_0(GameTableUI::playThunderAnimate, this, 2.5f)),
			DelayTime::create(duration2),
			CallFuncN::create(CC_CALLBACK_1(GameTableUI::boom, this)),
			DelayTime::create(duration3),
			leftmove21,

			CallFunc::create([=](){
				leftPlayer->hideHandCard(false);
				leftLayer->removeAllChildren();
				winner->hideHandCard(true);
		}),
			nullptr));

		// right
		rightLayer->runAction(Sequence::create(
			rightmove12,
			DelayTime::create(duration2),
			CallFuncN::create(CC_CALLBACK_1(GameTableUI::boom, this)),
			DelayTime::create(duration3),
			rightmove21,

			CallFunc::create([=](){
				rightPlayer->hideHandCard(false);
				rightLayer->removeAllChildren();
				winner->hideHandCard(true);
		}),
			nullptr));
	}

	void GameTableUI::showCompareOption(const std::vector<bool> seats)
	{
		if (seats.size() != PLAY_COUNT)
		{
			return;
		}
		for (int i = 0; i < PLAY_COUNT; i++)
		{
			_tableUI.bCompare[i]->setVisible(seats[i]);

		}

	}

	/************************分割线*********************************/
	void GameTableUI::inflateLayout()
	{
		this->setBackGroundImage("tractor/table/table_bg.png");

		Size winSize = Director::getInstance()->getWinSize();

		//add dashboard
		_dashboard = Dashboard::create(this);
		_dashboard->setAnchorPoint(Vec2(0.5f, 0));
		_dashboard->setPosition(Vec2(winSize.width / 2, 0));
		_dashboard->setVisible(false);
		addChild(_dashboard, Dashboard_Zorder);

		auto wOutlineNode = CSLoader::createNode("tractor/goldui/goldui.csb");
		_tableUI.wOutline = (Widget*)wOutlineNode->getChildByName("gold_Poker");
		//_tableUI.wOutline = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("tractor/goldui/goldui.json");
		_tableUI.wOutline->setAnchorPoint(Vec2(0.5f, 0.5f));
		_tableUI.wOutline->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
		this->addChild(wOutlineNode, 2);

		auto layer = _tableUI.wOutline;
		_tableUI.bReady = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(layer, "start_button"));
		_tableUI.bReady->addTouchEventListener(CC_CALLBACK_2(GameTableUI::menuClickCallback, this));
		_tableUI.bReady->setVisible(false);

		_tableUI.lGuoDi = dynamic_cast<TextAtlas*>(ui::Helper::seekWidgetByName(layer, "label_guodi"));
		_tableUI.lNote = dynamic_cast<TextAtlas*>(ui::Helper::seekWidgetByName(layer, "label_note"));
		_tableUI.lTopNote = dynamic_cast<TextAtlas*>(ui::Helper::seekWidgetByName(layer, "label_top_note"));
		_tableUI.lLimitNote = dynamic_cast<TextAtlas*>(ui::Helper::seekWidgetByName(layer, "label_limit_note"));
		_tableUI.lTotalNote = dynamic_cast<TextAtlas*>(ui::Helper::seekWidgetByName(layer, "label_total_note"));
		_tableUI.iTable = dynamic_cast<ImageView*>(ui::Helper::seekWidgetByName(layer, "table"));

		auto topNoteInfo = dynamic_cast<ImageView*>(ui::Helper::seekWidgetByName(layer, "image_note_bg"));
		topNoteInfo->setAnchorPoint(Vec2(0.5f, 1));
		topNoteInfo->setPosition(layer->convertToNodeSpace(Vec2(winSize.width / 2, winSize.height)));


		char tmp[100] = { 0 };
		for (int i = 0; i < PLAY_COUNT; i++)
		{
			sprintf(tmp, "button_compare_%d", i);
			auto btn = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(layer, tmp));
			btn->setVisible(false);
			auto pos = btn->getPosition();
			//_tableUI.bCompare[i] = btn;
			_tableUI.bCompare[i] = dynamic_cast<Button*>(btn->clone());
			this->addChild(_tableUI.bCompare[i] , MEMBAN_ZORDER + 1);
			_tableUI.bCompare[i]->setPosition(layer->convertToWorldSpace(pos));
			_tableUI.bCompare[i]->addTouchEventListener(CC_CALLBACK_2(GameTableUI::compareClickCallback, this));
			_tableUI.bCompare[i]->setVisible(false);

			sprintf(tmp, "btn_sit_pos_%d", i);
			_tableUI.bSeat[i] = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(layer, tmp));
			_tableUI.bSeat[i]->addTouchEventListener(CC_CALLBACK_2(GameTableUI::seatClickCallback, this));

			_seatPosition[i] = _tableUI.bSeat[i]->getPosition();
		}


		//牌类型提示
		_tableUI.bCardType = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(layer, "btn_cardtype"));
		_tableUI.bCardType->setAnchorPoint(Vec2(1, 0));
		_tableUI.bCardType->setPosition(layer->convertToNodeSpace(Vec2(winSize.width, winSize.height * 0.25)));
		_tableUI.bCardType->addTouchEventListener(CC_CALLBACK_2(GameTableUI::menuClickCallback, this));


		_tableUI.bBackMenu = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(layer, "btn_menu"));
		_tableUI.bBackMenu->setAnchorPoint(Vec2(0, 1));
		_tableUI.bBackMenu->setPosition(layer->convertToNodeSpace(Vec2(0, winSize.height)));
		_tableUI.bBackMenu->addTouchEventListener(CC_CALLBACK_2(GameTableUI::menuClickCallback, this));

		//下拉菜单
		auto wPullDownMenuNode = CSLoader::createNode("tractor/pulldownmenu/pulldownmenu.csb");
		_tableUI.wPullDownMenu = (Widget*)wPullDownMenuNode->getChildByName("Panel_4");
		//_tableUI.wPullDownMenu = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("tractor/pulldownmenu/pulldownmenu.json");
		_tableUI.wPullDownMenu->setAnchorPoint(Vec2(0, 0));
		_tableUI.wPullDownMenu->setPosition(Vec2(5, winSize.height + 10));
		this->addChild(wPullDownMenuNode, 102);
		for (BYTE i = 0; i < 3; i++)
		{
			_tableUI.bJumpMenu[i] = dynamic_cast<Button*>(ui::Helper::seekWidgetByTag(_tableUI.wPullDownMenu, i + 62));
			_tableUI.bJumpMenu[i]->addTouchEventListener(CC_CALLBACK_2(GameTableUI::pullDownItemCallback, this));
		}

		SpriteFrameCache* cache = SpriteFrameCache::getInstance();
		cache->addSpriteFramesWithFile("tractor/table/cards.plist");
	}

	bool GameTableUI::seatNoIsOk(BYTE seatNo)
	{
		return (seatNo < PLAY_COUNT && seatNo >= 0);
	}
}