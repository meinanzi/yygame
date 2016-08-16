#include "GoldenFlowerGameTableUI.h"
#include "GoldenFlowerGameTableLogic.h"
#include "GoldenFlowerGameDashboard.h"
#include "GoldenFlowerGameUserMessageBox.h"

#include "json/rapidjson.h"
#include "json/document.h"
#include "HNLobbyExport.h"
#include "HNUIExport.h"

namespace goldenflower
{
	static const int	Dashboard_Zorder	=	10;
	static const int	TableInfo_Zorder	=   10;
	static const int	EndBox_Zorder       =   12;
	static const int	Max_Zorder			=   100;

	static const int	MAX_ZORDER			=   100; //最大层级
	static const int	MEMBAN_ZORDER		=	300;

	static const char* JSON_NODE_POSITION	= "Point8";
	static const char* JSON_NODE_DEALER     = "Dealer";


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

bool GameTableUI::init(BYTE bDeskIndex, bool bAutoCreate)
{
	if (!HNLayer::init())
	{
		return false;
	}

	_remainTime = 0;
	Size size = Director::getInstance()->getVisibleSize();
	_visibleWidth = size.width;
	_visibleHeight = size.height;
	this->ignoreAnchorPointForPosition(false);
	this->setPosition(size.width / 2, size.height / 2);

	this->setBackGroundImage("goldenflower/table/gameRoomBG.png");

	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(GameTableUI::onTouchBegan, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	initPosition();
	initGameData();
	inflateLayout();

	//荷官处理类
	_dealer = GameDelearUI::create();
	_dealer->setPosition(0, 0);
	addChild(_dealer, MAX_ZORDER - 1);
	
	std::vector<Vec2> seatPos(PLAY_COUNT);
	std::vector<Vec2> BetFlyToPos(PLAY_COUNT);
	std::vector<Vec2> tablePos(PLAY_COUNT);
	for (int i = 0; i < PLAY_COUNT; i++)
	{
		auto card_pos = _tableUI.iCard[i]->getPosition();
		auto bet = _tableUI.Nbet[i]->getPosition();
		seatPos[i] = _tableUI.lOutline->convertToWorldSpace(card_pos);
		BetFlyToPos[i] = _tableUI.lOutline->convertToWorldSpace(bet);
		//seatPos[i] = _tableUI.lOutline->convertToWorldSpace(_seatPosition[i]);
		tablePos[i] = _tableUI.lOutline->convertToWorldSpace(_jettonPosition[i]);
	}

	_dealer->setTablePoint(tablePos);
	_dealer->setSeatPoint(seatPos);
	_dealer->setBetFlyToPoint(BetFlyToPos);
	_dealer->setDealerPoint(Vec2(_visibleWidth * 0.5, _visibleHeight * 0.68));

	//牌桌逻辑类
	_tableLogic = new GameTableLogic(this, bDeskIndex, bAutoCreate);
	_tableLogic->clearDesk();
	

	_tableLogic->enterGame();

	_QueIngNotice = nullptr;

	return true;
}

void GameTableUI::onExit()
{
	SpriteFrameCache* cache = SpriteFrameCache::getInstance();
	cache->removeSpriteFramesFromFile("goldenflower/table/cards.plist");

	HNGameUIBase::onExit();
}

bool GameTableUI::onTouchBegan(Touch *touch, Event *unused_event)
{
	Vec2 touchPos = touch->getLocation();
	auto size = Director::getInstance()->getWinSize();

	//对下注筹码进行操作
	if (_dashboard->_ButtonBeg->getPosition().y > _dashboard->_BetBegPosition.y)
	{
		_dashboard->setAddBetVisible(false);
		_dashboard->setAddVisible(true);
		return false;
	}

	if (_tableUI.nPullDownMenu->getPosition().y < size.height - 50)
	{
		Size rt = _tableUI.nPullDownMenu->getContentSize();
		_tableUI.nPullDownMenu->stopAllActions();
		_tableUI.nPullDownMenu->runAction(MoveTo::create(0.2f, Vec2(0, _visibleHeight)));

		return false;
	}


	for (int i = 0; i < PLAY_COUNT; i++)
	{
		Rect rt = _tableUI.bSeat[i]->getBoundingBox();
		if (rt.containsPoint(touchPos))
		{
			if (nullptr != _players[i])
			{
				
			}
			return false;
		}
	}

	return true;
}

void GameTableUI::menuClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype)
{
	if (Widget::TouchEventType::ENDED != touchtype)
	{
		return;
	}

	 
	bool bRet = PoolManager::getInstance()->getCurrentPool()->contains(this);

	Button* ptr = (Button*)pSender;
	std::string name = ptr->getName();
	if (name.compare("btn_start") == 0)
	{
		// 排队机 就加入排队
		if ( RoomLogic()->getRoomRule() & GRR_QUEUE_GAME)
		{
			// 进入排队游戏
			_tableLogic->sendQueue();
		}
		else
		{
			ptr->setVisible(false);
			_tableLogic->sendAgreeGame();
			IStartTimer(0);
		}
	}
	else if(name.compare("btn_cardtype") == 0)
	{
		showCardType();
	}
	else if(name.compare("btn_menu") == 0)
	{
		Size rt = _tableUI.nPullDownMenu->getContentSize();
		_tableUI.nPullDownMenu->stopAllActions();
		_tableUI.nPullDownMenu->runAction(MoveTo::create(0.2f, Vec2(0, _visibleHeight-rt.height*1.3+2)));
	}
}

void GameTableUI::EndLayerClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype)
{
	if (Widget::TouchEventType::ENDED != touchtype)
	{
		return;
	}
	
	for (auto TempCard : _vecCard)
	{
		TempCard->removeFromParent();
	}
	_vecCard.clear();

	_endLayer->getChildByName("Image_win")->setVisible(false);
	_endLayer->getChildByName("Image_lose")->setVisible(false);
	//隐藏结算框
	_endLayer->getParent()->setVisible(false);
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
		if (pSender == _tableUI.iCard[i])
		{
			_tableLogic->sendCompare(_tableLogic->viewToLogicSeatNo(i));
			break;
		}
	}

	for (int i = 0; i < PLAY_COUNT; i++)
	{
		_tableUI.iCard[i]->setVisible(false);
		_tableUI.bCompare[i]->setVisible(false);
	}
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
	else if (pButton == _tableUI.bJumpMenu[2])			//商城子按钮
	{
	}
	else if (pButton == _tableUI.bJumpMenu[3])			//商城子按钮
	{
		showCardType();
	}

	Size rt = _tableUI.nPullDownMenu->getContentSize();
	_tableUI.nPullDownMenu->stopAllActions();
	_tableUI.nPullDownMenu->runAction(MoveTo::create(0.2f, Vec2(0, _visibleHeight-rt.height*1.3+2)));
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

//加注
void GameTableUI::addNote(LLONG money)
{
	//_tableLogic->sendAdd(money);
	_dashboard->setAddVisible(false);
	_dashboard->setAddBetVisible(true);
}

//选择下注筹码
void GameTableUI::addBet(int multiple)
{
	_dashboard->setAddBetVisible(false);
	_tableLogic->sendAddBet(multiple);
}

void GameTableUI::initGameData()
{	
	//memset(_pendData, 0, sizeof(_pendData));
	memset(_players, 0, sizeof(_players));
	memset(_viewSeat, INVALID_SEAT_NO, sizeof(_viewSeat));	
	_viewOfWiner = INVALID_SEAT_NO;
}

void GameTableUI::initPosition()
{
	std::string filename = "goldenflower/position.json";
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
: _dashboard(nullptr), _endLayer(nullptr)
{
	memset(_players, 0, sizeof(_players));
	_iContestNum = -1;
}

GameTableUI::~GameTableUI()
{
	HN_SAFE_DELETE(_tableLogic);
}

PlayerUI* GameTableUI::getPlayer(int seatNo)
{
	return (seatNoIsOk(seatNo) ? _players[seatNo] : nullptr);
}

PlayerUI* GameTableUI::getPlayer(LLONG userId)
{
	for(int i = 0; i< PLAY_COUNT; ++i)
	{
		if(nullptr != _players[i])
		{
			if(userId == _players[i]->getUserID())
			{
				return _players[i];
			}
		}
	}

	return nullptr;
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
	// all displayed users enter the game
	for(int i = 0; i< PLAY_COUNT; ++i)
	{
		if(nullptr!= _players[i])
		{
			_players[i]->enterGame();
		}
	}

	_dealer->dealHandCard(cards);
}

/************************************************************************/
void GameTableUI::showUser(BYTE seatNo, bool bMe, bool sex)
{
	if (!seatNoIsOk(seatNo))
	{
		return;
	}

	PlayerUI* player = PlayerUI::create(_tableLogic->getUserId(_tableLogic->viewToLogicSeatNo(seatNo)), seatNo);
	player->setAnchorPoint(Vec2(0.5f, 0.5f));
	
	player->setPosition(_seatPosition[seatNo]);

	player->setTableUI(this);

	player->setSex(sex, _tableLogic->viewToLogicSeatNo(seatNo));

	player->setScale(0.8);

	_tableUI.lOutline->addChild(player, MAX_ZORDER - 1);

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
	_tableUI.iDealer[seatNo]->setVisible(true);
}

void GameTableUI::showReady(bool visible)
{
	if (_tableUI.bReady != nullptr)
	{
		_tableUI.bReady->setVisible(visible);
	}
}

void GameTableUI::showNextGame(bool visible)
{
	if (_tableUI.next_game != nullptr)
	{
		_tableUI.next_game->setVisible(visible);
	}
}

void GameTableUI::showAddRange(LLONG min, LLONG max)
{
	_dashboard->setAddRange(min, max);
}


void GameTableUI::showUserNoteMoney(BYTE seatNo, LLONG money, int DiZhu)
{
	//筹码的个数
	int betNum = 0;
	if (0!=money && 0!=DiZhu)
	{
		betNum = money / DiZhu;
	}
	
	for ( int i = 0; i < betNum; i++)
	{
		_dealer->betMoney(seatNo, DiZhu);
	}
	//_dealer->betMoney(seatNo, money);
	auto player = getPlayer(seatNo);
	if(player != nullptr)
	{
		player->note(money);
	}
}

void GameTableUI::showWin(BYTE winnerSeatNo)
{
	// recycle all chips to the winner
	_dealer->recycleChips(winnerSeatNo);
}


void GameTableUI::startEndBox(S_C_GameEnd *pdata, BYTE *viewSeat, BYTE viewWiner)
{
	_endLayer->getParent()->setVisible(true);
	_vecCard.clear();
	//记录当局玩家数
	int playerCount = 0;

	//结算框
	ImageView *WinOrLoseImage = nullptr;

	//如果赢家是自己
	if (0 == viewWiner)
	{
		WinOrLoseImage = dynamic_cast<ImageView*>(_endLayer->getChildByName("Image_win"));
		WinOrLoseImage->setVisible(true);
		ImageView *FlashLight = dynamic_cast<ImageView*>(WinOrLoseImage->getChildByName("Flash_light"));
		(_endLayer->getChildByName("Image_lose"))->setVisible(false);
		FlashLight->runAction(RepeatForever::create(RotateBy::create(0.05f, 4)));
	}
	else
	{
		WinOrLoseImage = dynamic_cast<ImageView*>(_endLayer->getChildByName("Image_lose"));
		WinOrLoseImage->setVisible(true);
		(_endLayer->getChildByName("Image_win"))->setVisible(false);

		//粒子效果
		auto Light = ParticleSystemQuad::create("goldenflower/animation/FlashLight.plist");
		ImageView *FlashLight = (ImageView*)Helper::seekWidgetByName(_endLayer, "Flash_light");
		Light->setPosition(FlashLight->getPosition());
		WinOrLoseImage->addChild(Light, 11);
	}

	//结算框信息
	for (int i = 0; i < PLAY_COUNT; i++)
	{
		if (pdata->i64ChangeMoney[i] != 0)
		{
			playerCount++;
			string player = StringUtils::format("player%d", playerCount);
			ImageView *ImagePlayer = dynamic_cast<ImageView*>(WinOrLoseImage->getChildByName(player));
			ImagePlayer->setVisible(true);

			((Text *)ImagePlayer->getChildByName("nick_name"))->setString(GBKToUtf8(_tableLogic->_userName[i].c_str()));

			bool IsWin = (pdata->i64ChangeMoney[i] > 0);
			((ImageView *)ImagePlayer->getChildByName("win_sign"))->setVisible(IsWin);
			((ImageView *)ImagePlayer->getChildByName("lose_sign"))->setVisible(!IsWin);

			string money = StringUtils::format("%lld", pdata->i64ChangeMoney[i]);
			((Text *)ImagePlayer->getChildByName("win_lose_num"))->setString(money);

			string gameTimes = StringUtils::format("%d", pdata->iOperateCount[i]);
			((Text *)ImagePlayer->getChildByName("game_times"))->setString(gameTimes);

			//显示扑克
			for (int j = 0; j < MAX_CARD_COUNT; j++)
			{
				PokerCard* tmpCard = PokerCard::create(pdata->byCard[i][j]);
				Vec2 cardPosition = ((Node *)ImagePlayer->getChildByName("card_position"))->getPosition();
				tmpCard->setPosition(cardPosition + Vec2(20 * (j - 1), -16));
				tmpCard->setScale(0.8f);
				ImagePlayer->addChild(tmpCard);
				_vecCard.push_back(tmpCard);
			}
		}
	}

	//如果是中途进来的玩家就没有结算框
// 	if (0 == playerCount)
// 	{
// 		return;
// 	}

	string player = StringUtils::format("player%d", playerCount);
	ImageView *LastPlayer = dynamic_cast<ImageView*>(WinOrLoseImage->getChildByName(player));

	//空白页
	ImageView *NothingPlayer = dynamic_cast<ImageView*>(WinOrLoseImage->getChildByName("player_nothing"));
	float ImageHeight = LastPlayer->getContentSize().height;
	NothingPlayer->setPosition(LastPlayer->getPosition() + Vec2(0, -ImageHeight));

	//隐藏不存在的玩家
	for (int i = playerCount; i < PLAY_COUNT; i++)
	{
		playerCount++;
		string player = StringUtils::format("player%d", playerCount);
		ImageView *ImagePlayer = dynamic_cast<ImageView*>(WinOrLoseImage->getChildByName(player));
		ImagePlayer->setVisible(false);
	}

	scheduleOnce(schedule_selector(GameTableUI::releaseGameEndBox), 5.f);
}

void GameTableUI::showEndBox(S_C_GameEnd *pdata, BYTE *viewSeat, BYTE viewWiner)
{
	memcpy(_viewSeat, viewSeat, sizeof(_viewSeat));
	_pendData = *pdata;

	BYTE _viewOfWiner = viewWiner;

	_endLayer->runAction(Sequence::create(
		DelayTime::create(2.0f),
		CallFunc::create([=](){
		startEndBox(&_pendData, _viewSeat, _viewOfWiner);
	}),
		nullptr));
}


void GameTableUI::releaseGameEndBox(float dt)
{
	//判断玩家输赢
	bool isWin = _endLayer->getChildByName("Image_win")->isVisible();
	_endLayer->getChildByName("Image_win")->setVisible(false);
	_endLayer->getChildByName("Image_lose")->setVisible(false);

	for (auto TempCard : _vecCard)
	{
		TempCard->removeFromParent();
	}
	_vecCard.clear();

	if (isWin)
	{
		//粒子效果
		auto gold = ParticleSystemQuad::create("goldenflower/animation/gold.plist");
		gold->setPosition(_seatPosition[0]+Vec2(0,-100));
		Sprite *goldSprite = Sprite::create("goldenflower/animation/gold.png");
		gold->setTexture(goldSprite->getTexture());
		_endLayer->addChild(gold, 10);

		auto star = ParticleSystemQuad::create("goldenflower/animation/star.plist");
		star->setPosition(_seatPosition[0] + Vec2(0, -100));
		Sprite *starSprite = Sprite::create("goldenflower/animation/star.png");
		star->setTexture(starSprite->getTexture());
		_endLayer->addChild(star, 10);

		scheduleOnce(schedule_selector(GameTableUI::disapearEndBox), 4.0f);
	}
	else
	{
		//隐藏结算框
		_endLayer->getParent()->setVisible(false);
	}
}

void GameTableUI::disapearEndBox(float dt)
{
	//隐藏结算框
	_endLayer->getParent()->setVisible(false);
}

void GameTableUI::showWatchCard(BYTE seatNo,bool visible)
{
	if (INVALID_SEAT_NO == seatNo)
	{
		for (int i = 0; i < PLAY_COUNT; i++)
		{
			_tableUI.WatchSign[i]->setVisible(visible);
		}
	}
	else
	{
		_tableUI.WatchSign[seatNo]->setVisible(visible);
	}
}

void GameTableUI::showGiveUpCard(BYTE seatNo, bool visible)
{
	if (INVALID_SEAT_NO == seatNo)
	{
		for (int i = 0; i < PLAY_COUNT; i++)
		{
			_tableUI.GiveUpSign[i]->setVisible(visible);
		}
	}
	else
	{
		_tableUI.GiveUpSign[seatNo]->setVisible(visible);
	}
}


//显示玩家手牌
void GameTableUI::showUserHandCard(BYTE seatNo, const std::vector<BYTE>& cards)
{
	//LLONG currentUserID = PlatformLogic()->loginResult.dwUserID;
	for (int i = 0; i < PLAY_COUNT; ++i)
	{
		if (nullptr != _players[i])
		{
			_players[i]->enterGame();
		}
	}

	if (seatNoIsOk(seatNo) && _players[seatNo] != nullptr)
	{
		if(!_players[seatNo]->isPlaying())
		{
			return;
		}

		_players[seatNo]->setHandCard(0, cards[0]);
		_players[seatNo]->setHandCard(1, cards[1]);
		_players[seatNo]->setHandCard(2, cards[2]);
	}
}

//翻牌动画
void GameTableUI::showUserFlipCard(BYTE seatNo, const std::vector<BYTE>& cards)
{
	if (seatNoIsOk(seatNo) && _players[seatNo] != nullptr)
	{
		if (!_players[seatNo]->isPlaying())
		{
			return;
		}

		const int iCardNum = 3;
		for (int i = 0; i < iCardNum;i++)
		{
			_players[seatNo]->_handCard[i]->setCardValue(0);
			auto orbit = OrbitCamera::create(0.1f, 1, 0, 0, -90, 0, 0);
			auto orbit1 = OrbitCamera::create(0.2f, 1, 0, 90, -90, 0, 0);
			auto seq = Sequence::create(orbit,
				CallFuncN::create(CC_CALLBACK_1(PlayerUI::changeHandCardValue, _players[seatNo], i, cards[i])),
				orbit1,
				nullptr
				);
			_players[seatNo]->_handCard[i]->runAction(seq);
		}
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
		_tableUI.bCompare[i]->setVisible(false);
		_tableUI.bSeat[i]->setVisible(false);
		_tableUI.iDealer[i]->setVisible(false);
	}
	_dealer->clear();
}

void GameTableUI::clearInvalidUser()
{
	for (BYTE i = 0; i < PLAY_COUNT; ++i)
	{
		if (nullptr == _players[i])
		{
			_tableUI.bCompare[i]->setVisible(false);
			_tableUI.bSeat[i]->setVisible(false);
			_tableUI.iDealer[i]->setVisible(false);		
		}	
	}
}


void GameTableUI::leaveDesk()
{
	if ((RoomLogic()->getRoomRule() & GRR_QUEUE_GAME) || RoomLogic()->getRoomRule() & GRR_CONTEST || (RoomLogic()->getRoomRule() & GRR_TIMINGCONTEST))
	{
		RoomLogic()->close();
		GamePlatform::returnPlatform(LayerType::ROOMLIST);
	}
	else
	{
		GamePlatform::returnPlatform(LayerType::DESKLIST);
	}
}

//显示牌桌准备
void GameTableUI::showLoading(bool bVisible)
{
	if (bVisible)
	{
		_loadSprite = Sprite::create("goldenflower/table/loading_view.jpg");
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
	ui::ImageView* newCompare[PLAY_COUNT] = {0};
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
			newPlayers[index]->setViewSeatNo(index);
			_players[i]->setHandCardPosition();
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

void GameTableUI::showNotice(const std::string &message, bool bAction)
{
	GameNotice* notice = GameNotice::create(this, message);
	this->reorderChild(notice, MAX_ZORDER);
	notice->show(bAction);
}

void GameTableUI::showQueNotice(const std::string &message, bool bShow)
{
	if (bShow)
	{
		if (nullptr == _QueIngNotice)
		{
			_QueIngNotice = GameNotice::create(this, message);
			this->reorderChild(_QueIngNotice, MAX_ZORDER);
			_QueIngNotice->show(false);
		}
		else
		{
			_QueIngNotice->show(false);
		}
	}
	else
	{
		if (_QueIngNotice)
		{
			_QueIngNotice->hide();
			_QueIngNotice = nullptr;
		}
	}
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
	if (_lRoomInfo)
	{
		removeChild(_lRoomInfo);
	}
	char str[50] = { 0 };
	sprintf(str, "房间:%s", tableName.c_str());
	_lRoomInfo = Label::createWithSystemFont(GBKToUtf8(str), "", 18);
	_lRoomInfo->setAnchorPoint(Vec2(1, 1));
	auto size = Director::getInstance()->getWinSize();
	_lRoomInfo->setPosition(Vec2(size.width - 10, size.height - 10));
	this->addChild(_lRoomInfo, TableInfo_Zorder);
}

void GameTableUI::IStartTimer(int second)
{
	_remainTime = second;

	char str[50] = { 0 };
	sprintf(str, "%d", second);
	_tableUI.TextAtlas_TimerText->setString(str);
	_tableUI.TextAtlas_TimerText->setScale(0.8f);
	unschedule(schedule_selector(GameTableUI::timerUpdate));

	if (second > 0)
	{
		_tableUI.ImageView_TimerBg->setVisible(true);
		schedule(schedule_selector(GameTableUI::timerUpdate), 1.0f);
	}
	else
	{
		_tableUI.ImageView_TimerBg->setVisible(false);
	}
}

void GameTableUI::timerUpdate(float delat)
{
	if (_remainTime < 1)
	{
		unschedule(schedule_selector(GameTableUI::timerUpdate));
		_tableUI.ImageView_TimerBg->setVisible(false);
		return;
	}

	//隐藏结算框
// 	if (_remainTime == _logic->getFreeTime() && _logic->getGameStatus() == GS_WAIT_NEXT && nullptr != _gameResult)
// 	{
// 		_gameResult->hide();
// 		_gameResult = nullptr;
// 	}

	_remainTime--;
	char str[15];
	sprintf(str, "%d", _remainTime);
	_tableUI.TextAtlas_TimerText->setString(str);
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
	sprintf(tmp, "%s:%lld", GBKToUtf8("上限"), note);
	_tableUI.lLimitNote->setString(tmp);
}

void GameTableUI::showBaseNote(LLONG note)
{
	char tmp[50] = { 0 };
	sprintf(tmp, "%s:%lld", GBKToUtf8("底注"), note);
	_tableUI.lNote->setString(tmp);
}

void GameTableUI::showLimitPerNote(LLONG note)
{
	char tmp[50] = { 0 };
	sprintf(tmp, "%s:%lld", GBKToUtf8("顶注"), note);
	_tableUI.lTopNote->setString(tmp);
}

void GameTableUI::showGuoDi(LLONG note)
{
	char tmp[50] = { 0 };
	sprintf(tmp, "%s:%lld", GBKToUtf8("锅底"), note);
	_tableUI.lGuoDi->setString(tmp);
}


void GameTableUI::showTotalNote(LLONG note)
{
	char tmp[50] = { 0 };
	sprintf(tmp, "%s:%lld", GBKToUtf8("总注"), note);
	_tableUI.lTotalNote->setString(tmp);
}

void GameTableUI::showMyTotalNote(LLONG note)
{
	char tmp[50] = { 0 };
	sprintf(tmp, "%s:%lld", GBKToUtf8("个人总注"), note);
	_tableUI.lMyTotalNote->setString(tmp);
}

void GameTableUI::showReadySign(BYTE seatNo,bool visible)
{
	if (INVALID_SEAT_NO == seatNo)
	{
		for (int i = 0; i < PLAY_COUNT;i++)
		{
			_tableUI.ReadySign[i]->setVisible(visible);
		}
	}
	else
	{
		_tableUI.ReadySign[seatNo]->setVisible(visible);
	}	
}

void GameTableUI::showUserLookCard(BYTE seatNo, bool isMe)
{
	if (seatNoIsOk(seatNo) && _players[seatNo] != nullptr)
	{
		_players[seatNo]->playAction(PlayerUI::Action::eLookCard);
		if(!isMe)
		{
			//_players[seatNo]->lookHandCard();
			_tableUI.WatchSign[seatNo]->setVisible(true);
		}
	}
	
}

void GameTableUI::showUserGiveUp(BYTE seatNo)
{
	if (seatNoIsOk(seatNo) && _players[seatNo] != nullptr)
	{
		_players[seatNo]->playAction(PlayerUI::Action::eGiverUp);
		_players[seatNo]->lose();
		_tableUI.WatchSign[seatNo]->setVisible(false);
		_tableUI.GiveUpSign[seatNo]->setVisible(true);
		_dashboard->setAddBetVisible(false);
	}

	//隐藏比牌提示标志
	for (BYTE i = 0; i < PLAY_COUNT; ++i)
	{
		if (nullptr != _players[i])
		{
			_tableUI.bCompare[i]->setVisible(false);
		}
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


void GameTableUI::showUserOpenCard(BYTE seatNo)
{

}

void GameTableUI::showDashboard(bool visible)
{
	visible ? _dashboard->show() : _dashboard->hide();
}


void GameTableUI::setLookVisible(bool visible) 
{
	_dashboard->setLookVisible(visible);
}

void GameTableUI::setFollowVisible(bool visible) 
{
	_dashboard->setFollowVisible(visible);
}

void GameTableUI::setAddVisible(bool visible, bool *CanAdd)
{
	memcpy(_dashboard->_CanAddOfAll, CanAdd, sizeof(_dashboard->_CanAddOfAll));
	//当前玩家是自己并且可以下注才显示加注按钮	
	_dashboard->setAddVisible( visible  &&  (CanAdd[0] || CanAdd[1] || CanAdd[2] || CanAdd[3]) );
}

void GameTableUI::setOpenVisible(bool visible) 
{
	_dashboard->setOpenVisible(visible);
}

void GameTableUI::setGiveUpVisible(bool visible) 
{
	_dashboard->setGiveUpVisible(visible);
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

//炸弹效果
void GameTableUI::moveBomb(bool leftWiner)
{
	//炸弹
	ImageView *bomb = ImageView::create("goldenflower/res/tableSrc/bomb.png");
	int setposition = 0;
	Size headSize = _tableUI.head_image[0]->getContentSize();
	//移动的距离
	int MoveDistance = (_tableUI.head_image[1]->getPosition()).x - (_tableUI.head_image[0]->getPosition()).x;
	//MoveDistance += headSize.width/2;

	if (leftWiner)
	{
		_tableUI.head_image[0]->addChild(bomb, 101);
		bomb->ignoreAnchorPointForPosition(false);
		bomb->setPosition(Vec2(headSize.width / 2, headSize.height / 2));
		setposition = 1;
	}
	else
	{
		_tableUI.head_image[1]->addChild(bomb, 101);
		bomb->ignoreAnchorPointForPosition(false);
		bomb->setPosition(Vec2(headSize.width / 2, headSize.height / 2));
		setposition = -1;
	}
	bomb->setVisible(true);

	//炸弹移动轨迹
	ccBezierConfig tr0;
	tr0.endPosition = Vec2(MoveDistance*setposition, 0);
	tr0.controlPoint_1 = Vec2(0, 260);
	tr0.controlPoint_2 = Vec2(MoveDistance*setposition, 260);
	CCActionInterval* bezierForward = CCBezierBy::create(0.8f, tr0);

	auto rotateBy = RotateBy::create(0.8f, 360);
	auto spawn = Spawn::create(bezierForward, rotateBy, nullptr);

	bomb->runAction(Sequence::create(
	spawn,
	RemoveSelf::create(true),
	CallFunc::create(CC_CALLBACK_0(GameTableUI::playBombAnimate, this, 1.6f, leftWiner)),
		nullptr));

}

void GameTableUI::moveHead(bool winnerIsLeft, BYTE winSeatNo)
{
	if (winnerIsLeft)
	{
		auto moveLeftHead = MoveTo::create(0.8f, _seatPosition[winSeatNo]);

		ccBezierConfig tr0;
		tr0.endPosition = Vec2(500, 0);
		tr0.controlPoint_1 = Vec2(0, 260);
		tr0.controlPoint_2 = Vec2(500, 260);
		CCActionInterval* bezierForward = CCBezierBy::create(0.5f, tr0);

		auto rotateBy = RotateBy::create(0.8f, 360);

		_tableUI.head_image[0]->runAction(Sequence::create(
			moveLeftHead,
			CallFunc::create([=](){
			_tableUI.head_image[0]->setVisible(false);
		}),
			nullptr));

		auto Spawn = Spawn::create(bezierForward, rotateBy, nullptr);
		_tableUI.head_image[1]->runAction(Sequence::create(
			Spawn,
			CallFunc::create([=](){
			_tableUI.head_image[1]->setVisible(false);
		}),
			nullptr));

	}
	else
	{
		auto moveRightHead = MoveTo::create(0.8f, _seatPosition[winSeatNo]);
		ccBezierConfig tr1;
		tr1.endPosition = Vec2(-500, 0);
		tr1.controlPoint_1 = Vec2(0, 260);
		tr1.controlPoint_2 = Vec2(-500, 260);
		CCActionInterval* bezierForwardLeft = CCBezierBy::create(0.5f, tr1);
		auto RotateByLeft = RotateBy::create(0.8f, 360);

		_tableUI.head_image[1]->runAction(Sequence::create(
			moveRightHead,
			CallFunc::create([=](){
			_tableUI.head_image[0]->setVisible(false);
		}),
			nullptr));


		auto RightSpawn = Spawn::create(bezierForwardLeft, RotateByLeft,nullptr);
		_tableUI.head_image[0]->runAction(Sequence::create(
			RightSpawn,
			CallFunc::create([=](){
			_tableUI.head_image[1]->setVisible(false);
		}),
			nullptr));
	}
}

void GameTableUI::playThunderAnimate(float duration)
{
	std::string audio = GOLD_PK_THUNDER;
	HNAudioEngine::getInstance()->playEffect(audio.c_str());

	SpriteFrameCache *frameCache = SpriteFrameCache::getInstance();
	frameCache->addSpriteFramesWithFile("goldenflower/animation/versus.plist");

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
		action,
		nullptr);

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto sp = Sprite::create();
	sp->setScale(0.8f);
	sp->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
	sp->runAction(Sequence::create(spwn, NULL));
	this->addChild(sp, 100);
}

void GameTableUI::playBombAnimate(float duration, bool leftWin)
{

	SpriteFrameCache *frameCache = SpriteFrameCache::getInstance();
	frameCache->addSpriteFramesWithFile("goldenflower/table/bombPlist.plist");

	auto animation = Animation::create();
	for (int i = 1; i < 17;i++)
	{
		string szName = StringUtils::format("boom_%02d.png", i);
		auto frame = frameCache->getSpriteFrameByName(szName);
		animation->addSpriteFrame(frame);
	}

	animation->setDelayPerUnit(duration / 16.0f);
	animation->setRestoreOriginalFrame(true);

	auto action = Animate::create(animation);

	//爆炸的位置
	Vec2 bombPosition;
	if (leftWin)
	{
		bombPosition = _tableUI.head_image[1]->getPosition();
	}
	else
	{
		bombPosition = _tableUI.head_image[0]->getPosition();
	}

	auto sp = Sprite::create();
	sp->setScale(0.8f);
	sp->setPosition(bombPosition);
	sp->runAction(action);
	this->addChild(sp, 102);
}

void GameTableUI::showCompareResult(BYTE winSeatNo, BYTE loseSeatNo)
{
	auto sex = false; // Lady
	auto audio = sex ? GOLD_PK_MAN : GOLD_PK_LADY;
	HNAudioEngine::getInstance()->playEffect(audio);

	auto winner = this->getPlayer(winSeatNo);
	auto loser = this->getPlayer(loseSeatNo);
	loser->lose();
	loser->setHandCardBroken();

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
		winnerCards[i]->setScale(0.86);
		Vec2 tmp = winner->convertToWorldSpace(winner->getAnchorPointInPoints());
		winnerCards[i]->setPosition(tmp.x + offset*i, tmp.y);
		winnerLayer->addChild(winnerCards[i], i);
	
		loserCards[i] = PokerCard::create(0x00);
		loserCards[i]->setScale(0.86);
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

	//赢家是否在左边
	bool winnerIsLeft = true;

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
		winnerIsLeft = false;

		rightLayer = winnerLayer;
		rightPlayer = winner;
	}
	//-----------
	winnerLayer = nullptr;
	loserLayer = nullptr;
	winner = nullptr;
	loser = nullptr;
	//-----------

	auto widthOfCards = winnerCards[0]->getContentSize().width;

	// target position 1
	auto leftPosition1 = leftLayer->getPosition();
	auto rightPosition1 = rightLayer->getPosition();

	auto Lp1 = leftPlayer->convertToWorldSpace(leftPlayer->getAnchorPointInPoints());
	auto Lp2 = Vec2(origin.x + visibleSize.width * 1 / 3, origin.y + visibleSize.height / 2);
	auto Lo1 = leftPosition1;
	auto Lo2 = Lp2 - Lp1 + Lo1;
	auto leftPosition2 = Lo2;

	auto Rp1 = rightPlayer->convertToWorldSpace(rightPlayer->getAnchorPointInPoints());
	auto Rp2 = Vec2(origin.x + visibleSize.width * 2 / 3, origin.y + visibleSize.height / 2);
	auto Ro1 = rightPosition1;
	auto Ro2 = Rp2 - Rp1 + Ro1;
	auto rightPosition2 = Ro2+Vec2(-30,0);

	//创建头像
	ImageView *leftHead = ImageView::create(leftPlayer->getHeadImage());
	ImageView *rightHead = ImageView::create(rightPlayer->getHeadImage());
	_tableUI.head_image[0]->addChild(leftHead, 100);
	_tableUI.head_image[0]->setVisible(true);
	_tableUI.head_image[0]->setPosition(_headImagePostion[0]);
	_tableUI.head_image[1]->addChild(rightHead, 100);
	_tableUI.head_image[1]->setVisible(true);
	_tableUI.head_image[1]->setPosition(_headImagePostion[1]);


	Size headSize = _tableUI.head_image[0]->getContentSize();
	leftHead->setPosition(Vec2(headSize.width / 2, headSize.height / 2));
	rightHead->setPosition(Vec2(headSize.width  / 2, headSize.height / 2));

	// move
	auto duration = 0.2f;
	auto leftmove12 = MoveTo::create(duration, leftPosition2);
	auto rightmove12 = MoveTo::create(duration, rightPosition2);

	auto leftmove21 = MoveTo::create(duration, leftPosition1);
	auto rightmove21 = MoveTo::create(duration, rightPosition1);

	// left
	float duration2 = 1.5;
	float duration3 = 1.2;
	leftLayer->runAction(Sequence::create(
		leftmove12,
		CallFunc::create(CC_CALLBACK_0(GameTableUI::playThunderAnimate, this, 1.5f)),
		CallFunc::create(CC_CALLBACK_0(GameTableUI::moveBomb, this, winnerIsLeft)),
		DelayTime::create(duration3+1),
		CallFunc::create(CC_CALLBACK_0(GameTableUI::moveHead, this, winnerIsLeft, winSeatNo)),
		//DelayTime::create(duration2),
		CallFuncN::create(CC_CALLBACK_1(GameTableUI::boom, this)),
		//DelayTime::create(duration3),
		leftmove21,

		CallFunc::create([=](){
		leftPlayer->hideHandCard(true);
		leftLayer->removeAllChildren();
	}),
		nullptr));

	// right
	rightLayer->runAction(Sequence::create(
		rightmove12,
		DelayTime::create(duration2+0.4),
		CallFuncN::create(CC_CALLBACK_1(GameTableUI::boom, this)),
		//DelayTime::create(duration3),
		rightmove21,

		CallFunc::create([=](){
		rightPlayer->hideHandCard(true);
		rightLayer->removeAllChildren();
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
		bool flippedX = false; // indicate the compare imageview is flipedX
		//auto logicNO = _tableLogic->viewToLogicSeatNo(i);
		if(seats[i] && nullptr!= _players[i] && _players[i]->isPlaying() && !_players[i]->isQuit())
		{	
			_tableUI.iCard[i]->setVisible(true);

			if(i < 4 ){
				if(_tableUI.bCompare[i]->isFlippedX() == false){
					_tableUI.bCompare[i]->setFlippedX(true);
					flippedX = true;
				}
			}	
			_tableUI.bCompare[i]->setVisible(true);
			Vec2 pos = _tableUI.bCompare[i]->getPosition();
			// run action
			int factor = flippedX ? -1 : 1;
			auto moveBy = MoveBy::create(0.5, Vec2(factor*9, 0));	
			auto sequence = Sequence::create(moveBy, moveBy->reverse(), nullptr);
			auto repeat = RepeatForever::create(sequence);
			_tableUI.bCompare[i]->runAction(repeat);
		}
	}
}

/************************分割线*********************************/
void GameTableUI::inflateLayout()
{
	Size winSize = Director::getInstance()->getWinSize();

	//add dashboard
	_dashboard = Dashboard::create(this);
	_dashboard->setAnchorPoint(Vec2(0.5f, 0));
	_dashboard->setPosition(Vec2(winSize.width / 2, 0));
	_dashboard->setVisible(false);
	addChild(_dashboard, Dashboard_Zorder);

	//结算框
	Node *endBosNode = CSLoader::createNode("goldenflower/endBox.csb");
	endBosNode->setZOrder(EndBox_Zorder);
	_endLayer = dynamic_cast<Layout*>(endBosNode->getChildByName("Panel_4"));
	endBosNode->setVisible(false);
	_endLayer->getChildByName("Image_win")->setVisible(false);
	_endLayer->getChildByName("Image_lose")->setVisible(false);
	endBosNode->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
	_endLayer->addTouchEventListener(CC_CALLBACK_2(GameTableUI::EndLayerClickCallback, this));
	addChild(endBosNode);


	auto root = CSLoader::createNode("goldenflower/goldui.csb");
	root->setAnchorPoint(Vec2(0.5f, 0.5f));
	root->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
	this->addChild(root, 2);

	_tableUI.next_game = dynamic_cast<ImageView*>(root->getChildByName("next_game"));
	_tableUI.next_game->setVisible(false);

	//计时器
	_tableUI.ImageView_TimerBg = dynamic_cast<ImageView*>(root->getChildByName("timerbg_0"));
	_tableUI.TextAtlas_TimerText = dynamic_cast<TextAtlas*>(_tableUI.ImageView_TimerBg->getChildByName("timer"));
	_tableUI.ImageView_TimerBg->setVisible(false);

	//比牌头框
	for (int i = 0; i < 2; i++)
	{
		std::string head = StringUtils::format("head_Image_%d", i);
		_tableUI.head_image[i] = dynamic_cast<ImageView*>(root->getChildByName(head));
		_tableUI.head_image[i]->setVisible(false);
		_headImagePostion[i] = _tableUI.head_image[i]->getPosition();
	}

	char tmp[100] = { 0 };
	for (int i = 0; i < PLAY_COUNT; i++)
	{
		sprintf(tmp, "Image_card_%d", i);
		_tableUI.iCard[i] = dynamic_cast<ImageView*>(root->getChildByName(tmp));
		_tableUI.iCard[i]->addTouchEventListener(CC_CALLBACK_2(GameTableUI::compareClickCallback, this));
		_tableUI.iCard[i]->setOpacity(0);
		_tableUI.iCard[i]->setVisible(false);

		sprintf(tmp, "ready_%d", i);
		_tableUI.ReadySign[i] = dynamic_cast<ImageView*>(root->getChildByName(tmp));
		_tableUI.ReadySign[i]->setVisible(false);

		sprintf(tmp, "Node_%d", i);
		_tableUI.Nbet[i] = dynamic_cast<Node*>(root->getChildByName(tmp));
		_tableUI.Nbet[i]->setVisible(false);

		sprintf(tmp, "card_watch_%d", i);
		_tableUI.WatchSign[i] = dynamic_cast<ImageView*>(root->getChildByName(tmp));
		_tableUI.WatchSign[i]->setVisible(false);

		sprintf(tmp, "card_giveup_%d", i);
		_tableUI.GiveUpSign[i] = dynamic_cast<ImageView*>(root->getChildByName(tmp));
		_tableUI.GiveUpSign[i]->setVisible(false);

	}

	_tableUI.lOutline = dynamic_cast<Layout*>(root->getChildByName("gold_Poker"));
	auto layout_buttons = dynamic_cast<ui::Layout*>(_tableUI.lOutline->getChildByName("layout_middle"));

	_tableUI.bReady = dynamic_cast<Button*>(layout_buttons->getChildByName("btn_start"));
	_tableUI.bReady->addTouchEventListener(CC_CALLBACK_2(GameTableUI::menuClickCallback, this));
	_tableUI.bReady->setVisible(false);

	auto layout_top_note = dynamic_cast<ui::Layout*>(_tableUI.lOutline->getChildByName("layout_top_note"));
	layout_top_note->setAnchorPoint(Vec2(0.5, 1.0));
	layout_top_note->setPosition(_tableUI.lOutline->convertToNodeSpace(Vec2(_visibleWidth*0.5, _visibleHeight)));
	_tableUI.lGuoDi = dynamic_cast<Text*>(layout_top_note->getChildByName("label_guodi"));
	_tableUI.lNote = dynamic_cast<Text*>(layout_top_note->getChildByName("label_base"));
	_tableUI.lTopNote = dynamic_cast<Text*>(layout_top_note->getChildByName("label_top"));
	_tableUI.lLimitNote = dynamic_cast<Text*>(layout_top_note->getChildByName("label_limit"));

	auto image_note = dynamic_cast<ImageView*>(_tableUI.lOutline->getChildByName("image_note"));
	_tableUI.lTotalNote = dynamic_cast<Text*>(image_note->getChildByName("label_total"));

	auto My_image_note = dynamic_cast<ImageView*>(_tableUI.lOutline->getChildByName("image_note_0"));
	_tableUI.lMyTotalNote = dynamic_cast<Text*>(My_image_note->getChildByName("label_total"));

	for (int i = 0; i < PLAY_COUNT; i++)
	{
		sprintf(tmp, "image_compare_%d", i);
		_tableUI.bCompare[i] = dynamic_cast<ImageView*>(layout_buttons->getChildByName(tmp));
		_tableUI.bCompare[i]->setVisible(false);

		sprintf(tmp, "btn_sit_pos_%d", i);
		_tableUI.bSeat[i] = dynamic_cast<Button*>(layout_buttons->getChildByName(tmp));
		_tableUI.bSeat[i]->addTouchEventListener(CC_CALLBACK_2(GameTableUI::seatClickCallback, this));
		_tableUI.bSeat[i]->setVisible(false);

		sprintf(tmp, "image_dealer_%d", i);
		_tableUI.iDealer[i] = dynamic_cast<ImageView*>(layout_buttons->getChildByName(tmp));
		_tableUI.iDealer[i]->setVisible(false);

		_seatPosition[i] = _tableUI.bSeat[i]->getPosition();
	}

	//牌类型提示
	_tableUI.bCardType = dynamic_cast<Button*>(layout_buttons->getChildByName("btn_cardtype"));
	_tableUI.bCardType->setAnchorPoint(Vec2(1, 0));
	_tableUI.bCardType->setPosition(_tableUI.lOutline->convertToNodeSpace(Vec2(winSize.width, winSize.height * 0.25)));
	_tableUI.bCardType->addTouchEventListener(CC_CALLBACK_2(GameTableUI::menuClickCallback, this));


	_tableUI.bBackMenu = dynamic_cast<Button*>(layout_buttons->getChildByName("btn_menu"));
	_tableUI.bBackMenu->setAnchorPoint(Vec2(0, 1));
	_tableUI.bBackMenu->setPosition(_tableUI.lOutline->convertToNodeSpace(Vec2(0, winSize.height)));
	_tableUI.bBackMenu->addTouchEventListener(CC_CALLBACK_2(GameTableUI::menuClickCallback, this));

	//下拉菜单
	auto root_pulldown = CSLoader::createNode("goldenflower/pulldownmenu.csb");
	root_pulldown->setAnchorPoint(Vec2(0, 0));
	this->addChild(root_pulldown, 102);
	_tableUI.nPullDownMenu = dynamic_cast<Layout*>(root_pulldown->getChildByName("layout_pulldown"));
	_tableUI.nPullDownMenu->setAnchorPoint(Vec2(0, 0));
	auto size = _tableUI.nPullDownMenu->getContentSize();
	_tableUI.nPullDownMenu->setPosition(Vec2(0, _visibleHeight));
	for (BYTE i = 0; i < 4; i++)
	{
		_tableUI.bJumpMenu[i] = dynamic_cast<Button*>(_tableUI.nPullDownMenu->getChildByTag(i + 349));
		_tableUI.bJumpMenu[i]->addTouchEventListener(CC_CALLBACK_2(GameTableUI::pullDownItemCallback, this));
	}

	SpriteFrameCache* cache = SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile("goldenflower/table/cards.plist");
}

bool GameTableUI::seatNoIsOk(BYTE seatNo)
{
	return (seatNo < PLAY_COUNT && seatNo >= 0);
}

void GameTableUI::getReady()
{
	 if(_tableUI.bReady->isVisible())
	 {
		_tableLogic->sendStandUp();
	 }
}

Vec2 GameTableUI::getCardPosition(BYTE seatNo)
{
	return _tableUI.iCard[seatNo]->getPosition();
}


/*--------------------------------比赛系列-------------------------------*/
void GameTableUI::addContestUI()
{
	ArmatureDataManager::getInstance()->addArmatureFileInfo("landlord/game/animation/huPaiAnimation.ExportJson");

	auto size = Director::getInstance()->getWinSize();
	auto contestIndex = Text::create();
	contestIndex->setAnchorPoint(Vec2(1.0, 0.5));
	contestIndex->setPosition(Vec2(size.width - 10, size.height - 20));
	addChild(contestIndex);
	contestIndex->setFontSize(23);
	contestIndex->setName("TextJu");
	contestIndex->setString(GBKToUtf8("第 1 局"));

	auto PaiMing = Text::create();
	PaiMing->setAnchorPoint(Vec2(1.0, 0.5));
	PaiMing->setPosition(Vec2(size.width - 10, size.height - 50));
	addChild(PaiMing);
	PaiMing->setFontSize(23);
	PaiMing->setName("PaiMing");
}


void GameTableUI::showGameContestKick()
{
	auto size = Director::getInstance()->getWinSize();
	auto kickImg = Sprite::create("landlord/game/contest/ContestKick.png");
	kickImg->setPosition(Vec2(size.width /2, size.height /2));
	kickImg->setName("kickPtr");
	kickImg->setScale(0.0f);
	addChild(kickImg, 25);

	kickImg->runAction(Sequence::create(DelayTime::create(3.7f),
		Spawn::create(ScaleTo::create(1.0f, 1.0f),
		RotateBy::create(1.0f, 360.0f), nullptr), DelayTime::create(3.0f),
		CallFunc::create([=](){leaveDesk(); }), nullptr));
}

void GameTableUI::showGameContestWaitOver()
{
	auto size = Director::getInstance()->getWinSize();
	auto waitPtr = Sprite::create("landlord/game/contest/ContestWait.png");
	waitPtr->setPosition(Vec2(size.width / 2, size.height / 2));
	waitPtr->setName("waitPtr");
	addChild(waitPtr, 25);
}

void GameTableUI::updateMyRankNum(int iValue) 						//更新自己的排名	
{
	_iContestNum = iValue;
}


void GameTableUI::showContsetJuShu(int &Index) 					//显示排名
{
	if (_lConstJuShu)
	{
		removeChild(_lConstJuShu);
	}
	
	char str[50] = { 0 };
	sprintf(str, "第%d局", Index);
	_lConstJuShu = Label::createWithSystemFont(GBKToUtf8(str), "", 18);
	_lConstJuShu->setAnchorPoint(Vec2(1, 1));
	auto size = Director::getInstance()->getWinSize();
	_lConstJuShu->setPosition(Vec2(size.width - 30, size.height - 30));
	this->addChild(_lConstJuShu, TableInfo_Zorder);
	
}

void GameTableUI::ShowConstRank(int iRankNum, int iRemainPeople)					//显示排名
{
	if (_lConstRank)
	{
		removeChild(_lConstRank);
	}
	char str[50] = { 0 };
	sprintf(str, "排名：%d/%d", iRankNum, iRemainPeople);
	_lConstRank = Label::createWithSystemFont(GBKToUtf8(str), "", 18);
	_lConstRank->setAnchorPoint(Vec2(1, 1));
	auto size = Director::getInstance()->getWinSize();
	_lConstRank->setPosition(Vec2(size.width - 10, size.height - 50));
	this->addChild(_lConstRank, TableInfo_Zorder);
}

void GameTableUI::showGameContestOver(MSG_GR_ContestAward* contestAward) //比赛结束
{
	//隐藏开始按钮
	showReady(false);
	//隐藏倒计时
	IStartTimer(0);
	//全部清空座位 包括自己
	for (BYTE i = 0; i < PLAY_COUNT; i++)
	{
		showUserUp(i, true);
		showReadySign(i, false);
		showWatchCard(i, false);
		showGiveUpCard(i, false);
	}
	clearInvalidUser();
	//移除等待排名
	auto waitPtr = dynamic_cast<Sprite*>(this->getChildByName("waitPtr"));
	if (waitPtr != nullptr)
	{
		waitPtr->removeFromParent();
	}

	//没有奖励 那就是被淘汰了
	if (!contestAward->iAward)
	{
		showGameContestKick();
		return;
	}
	Sprite* awardDlg = nullptr;
	if (contestAward->iAwardType)
	{
		awardDlg = Sprite::create("landlord/game/contest/ContestAwardsX.png");
	}
	else
	{
		awardDlg = Sprite::create("landlord/game/contest/ContestAwards.png");
	}
	auto size = Director::getInstance()->getWinSize();
	awardDlg->setPosition(Vec2(size.width/2, size.height/2));
	addChild(awardDlg, TableInfo_Zorder);

	awardDlg->setName("awardDlg");
	ParticleSystem *meteor = ParticleSystemQuad::create("landlord/game/contest/huoyan00.plist");
	meteor->setPosition(Vec2(awardDlg->getContentSize().width / 2, 10));
	meteor->setAutoRemoveOnFinish(true);
	awardDlg->addChild(meteor);

	awardDlg->setScale(0.0f);
	awardDlg->runAction(Sequence::create(DelayTime::create(3.7f), ScaleTo::create(0.2f, 1.0f), nullptr));

	char str[64] = { 0 };
	sprintf(str, "%d", _iContestNum);
	auto rankText = TextAtlas::create(str, "landlord/game/contest/js_win_num.png", 23, 28, "0");
	rankText->setPosition(Vec2(awardDlg->getContentSize().width * 0.73, awardDlg->getContentSize().height * 0.555));
	awardDlg->addChild(rankText);

	sprintf(str, "%d", contestAward->iAward);
	auto awardText = TextAtlas::create(str, "landlord/game/contest/js_win_num.png", 23, 28, "0");
	awardText->setPosition(Vec2(awardDlg->getContentSize().width * 0.45, awardDlg->getContentSize().height * 0.345));
	awardText->setAnchorPoint(Vec2(0, 0.5));
	awardDlg->addChild(awardText);

	auto MyListener = EventListenerTouchOneByOne::create();
	MyListener->setSwallowTouches(true);
	MyListener->onTouchBegan = [&](Touch* touch, Event* event)
	{
		auto awardDlg = (Sprite*)getChildByName("awardDlg");
		auto dlgSize = awardDlg->getBoundingBox();
		Size s = this->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);
		if (rect.containsPoint(touch->getLocation()))
		{
			if (dlgSize.containsPoint(touch->getLocation())) return true;
			awardDlg->runAction(Sequence::create(ScaleTo::create(0.2f, 0.0f), CallFunc::create([&](){leaveDesk(); }), RemoveSelf::create(true), nullptr));
			return true;
		}
		else
			return false;
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(MyListener, awardDlg);
}
}



