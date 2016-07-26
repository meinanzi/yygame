#include "ZYHZ_GameManager.h"
#include "SimpleAudioEngine.h"

namespace ZYHZ
{
	GameManager* GameManager::_instance = nullptr;

	GameManager::GameManager(void)
	{
		_instance = this;
	}

	GameManager::~GameManager(void)
	{
		ResourceLoader::clearAll();																						// 清除资源
		HNAudioEngine::getInstance()->stopBackgroundMusic();											// 停止音乐
		CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();							// 停止音效

		if (_bStartSendCard)
		{
			UserTouGuan msg;
			memset(&msg, 0, sizeof(msg));
			msg.byStation = RoomLogic()->loginResult.pUserInfoStruct.bDeskStation;
			msg.bIsTuoGuan = true;
			RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, ASS_AUTOCARD, &msg, sizeof(msg));
		}
	}

	bool GameManager::init()
	{
		if (!Node::init())
		{
			return false;
		}
		// 资源初始化
		setGlobalScale();
		this->setScale(_globalScale * 1.5f);
		loadResource();

		return true;
	}

	
	GameManager* GameManager::getInstance()
	{
		if (_instance == nullptr)
		{
			_instance = GameManager::create();
		}
		return _instance;
	}

	void GameManager::loadResource()
	{
		initData();
		addLoader();
		setText();
		setButton();
		addUI();
	}

	void GameManager::initData()
	{
		_isPlayGame = false;																		// 游戏状态是否正在4人打牌
		_isTrusteeShep = false;																	// 游戏状态是否托管
		_isSouthPlayOper = false;																// 是否轮到玩家操作
		_queMen = 4;
		_bigShowCard = nullptr;
		_clockID = 0;
		_bigShowZorder = 340;
		_niaoGlobalZorder = 999;
		_zorderWest = _zorderEast = _zorderSouth = _zorderNorth = 200;
		_bStartSendCard = false;
		_hasSendAllCard = false;

		_startDir = sitDir::MID_DIR;    // 庄家
		_startSendDir = sitDir::MID_DIR;    // 发牌方向
		_sendPos = 0;
		_huMsg.Clear();

	}

	void GameManager::restartSetData()
	{
		showMoney();
		_lastestOutCard = nullptr;
		_sendCardCount = 0;
		_vvStartHandCard.clear();
		_vecHasOutCardCount.assign(PLAY_COUNT, 0);
		_vecCardPool.at(0)->setCanOper(false);
		for (auto i = 1; i <= PLAY_COUNT; i++)
		{
			COCOS_NODE(Sprite, StringUtils::format("que_tongSP%d", i))->setVisible(false);
			COCOS_NODE(Sprite, StringUtils::format("que_wanSP%d", i))->setVisible(false);
			COCOS_NODE(Sprite, StringUtils::format("que_tiaoSP%d", i))->setVisible(false);
			COCOS_NODE(Sprite, StringUtils::format("tuoguang%d", i))->setVisible(false);
			COCOS_NODE(Sprite, StringUtils::format("ready%d", i))->setVisible(false);
		}
		COCOS_NODE(Sprite, "waiting")->setVisible(false);
		COCOS_NODE(Sprite, "timerPoint1")->setVisible(true);
		COCOS_NODE(Button, "return")->setVisible(true);
		COCOS_NODE(Button, "start")->setVisible(true);
		COCOS_NODE(Button, "trusteeshep")->setVisible(true);
		COCOS_NODE(Button, "quxiaotuoguang")->setVisible(false);

		auto remainTime = GTLogic()->getGameStation().byBeginTime;
		remainTime = std::min<int> (remainTime, 30);
		setRemainTime(remainTime);

		for (auto i = 1; i <= PLAY_COUNT; i++)
		{
			COCOS_NODE(Sprite, StringUtils::format("timerPoint%d", i))->setVisible(false);
		}
		HNAudioEngine::getInstance()->playBackgroundMusic((MUSIC_COMMON_PATH+"waiting.mp3").c_str());

		startTimer(sitDir::SOUTH_DIR, timerType::WAIT_START);

		if (_bigShowCard != nullptr)
		{
			_bigShowCard->getCardSprite()->removeFromParent();
			_bigShowCard->removeFromParent();
			_bigShowCard = nullptr;
		}

		initData();
	}

	void GameManager::addLoader()
	{
		// 创建资源管理器
		_cLoader = cocosResourceLoader::create(COCOS_PATH + "MainScene.csb");
		this->addChild(_cLoader);

		_sLoader = spriteResourceLoader::create(SPRITE_PATH+"path.xml");
		this->addChild(_sLoader);

		_pLoader = plistResourceLoader::create(PLIST_PATH + "resource.plist");
		this->addChild(_pLoader);
	}

	void GameManager::setText()
	{
		// 设置玩家信息text 控件属性
		for (auto i = 0; i < PLAY_COUNT; i++)
		{
			auto textLabel = COCOS_NODE(Text, StringUtils::format("name%d", i+1));
			textLabel->setColor(colorGold);
			textLabel->setString("");
			textLabel->ignoreAnchorPointForPosition(false);
			textLabel->setAnchorPoint(Vec2(0.5f, 0.5f));
			textLabel = COCOS_NODE(Text, StringUtils::format("money%d", i+1));
			textLabel->setColor(colorGold);
			textLabel->ignoreAnchorPointForPosition(false);
			textLabel->setAnchorPoint(Vec2(0.5f, 0.5f));
			textLabel->setString("");
		}
	}

	void GameManager::startButtonClickCallBack(Ref* ref,Widget::TouchEventType type)
	{
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			{
				GTLogic()->sendAgreeGame();
				break;
			}
		default:
			break;
		}
	}

	void GameManager::trusteeshepButtonClickCallBack(Ref* ref,Widget::TouchEventType type)
	{
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			{
				trusteeshepSendMsg(true);
				break;
			}
		default:
			break;
		}
	}

	void GameManager::quxiaotuoguangButtonClickCallBack(Ref* ref,Widget::TouchEventType type)
	{
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			{
				trusteeshepSendMsg(false);
				break;
			}
		default:
			break;
		}
	}

	void GameManager::returnButtonClickCallBack(Ref* ref,Widget::TouchEventType type)
	{
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			{
				GTLogic()->safeQuit();
				break;
			}
		default:
			break;
		}
	}

	void GameManager::queWanButtonClickCallBack(Ref* ref,Widget::TouchEventType type)
	{
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			{
				endQueMen(0);
				break;
			}
		default:
			break;
		}
	}

	void GameManager::queTongButtonClickCallBack(Ref* ref,Widget::TouchEventType type)
	{
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			{
				endQueMen(2);
				break;
			}
		default:
			break;
		}
	}

	void GameManager::queTiaoButtonClickCallBack(Ref* ref,Widget::TouchEventType type)
	{
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			{
				endQueMen(1);
				break;
			}
		default:
			break;
		}
	}

	void GameManager::chiButtonClickCallBack(Ref* ref,Widget::TouchEventType type)
	{
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			{
				endAllTimer();
				break;
			}
		default:
			break;
		}
	}

	void GameManager::pengButtonClickCallBack(Ref* ref,Widget::TouchEventType type)
	{
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			{
				endAllTimer();
				tagPengPaiEx msg;
				auto card = getLastestOutCard();
				msg.byPs = INT(card->getCardColor()) * 10 + card->getCardNumber();
				msg.byUser = RoomLogic()->loginResult.pUserInfoStruct.bDeskStation;
				RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, THING_PENG_PAI, &msg, sizeof(msg));
				break;
			}
		default:
			break;
		}
	}

	void GameManager::gangButtonClickCallBack(Ref* ref,Widget::TouchEventType type)
	{
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			{
				endAllTimer();
				tagGangPaiEx msg;
				msg.Clear();
				msg.byUser = RoomLogic()->loginResult.pUserInfoStruct.bDeskStation;
				msg.byPs = _currOperNumber;
				msg.byBeGang = GTLogic()->getUserStation(_currOperDir);
				for (auto i = 0; i < PLAY_COUNT; i++)
				{
					msg.byGangPai[i] = msg.byPs;
				}

				if (_kongType == ACTION_AN_GANG)
				{
					msg.byType = AT_QUADRUPLET_CONCEALED;
				}
				else if (_kongType == ACTION_MING_GANG)
				{
					msg.byType = AT_QUADRUPLET_REVEALED;
				}
				else if (_kongType == ACTION_BU_GANG)
				{
					msg.byType = AT_QUADRUPLET;
				}

				RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, THING_GANG_PAI, &msg, sizeof(msg));
				break;
			}
		default:
			break;
		}
	}

	void GameManager::tingButtonClickCallBack(Ref* ref,Widget::TouchEventType type)
	{
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			{
				endAllTimer();
				break;
			}
		default:
			break;
		}
	}

	void GameManager::huButtonClickCallBack(Ref* ref,Widget::TouchEventType type)
	{
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			{
				endAllTimer();
				RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, THING_HU_PAI, &_huMsg, sizeof(_huMsg));
				break;
			}
		default:
			break;
		}
	}

	void GameManager::qiButtonClickCallBack(Ref* ref,Widget::TouchEventType type)
	{
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			{
				passAction();
				break;
			}
		default:
			break;
		}
	}

	void GameManager::confirmButtonClickCallBack(Ref* ref,Widget::TouchEventType type)
	{
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			{
				COCOS_NODE(Sprite, "gfxy")->setVisible(false);
				break;
			}
		default:
			break;
		}
	}

	void GameManager::cancelButtonClickCallBack(Ref* ref,Widget::TouchEventType type)
	{
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			{
				COCOS_NODE(Sprite, "gfxy")->setVisible(false);
				break;
			}
		default:
			break;
		}
	}

	void GameManager::continueButtonClickCallBack(Ref* ref,Widget::TouchEventType type)
	{
		switch (type)
		{
            case cocos2d::ui::Widget::TouchEventType::ENDED:
			{
				COCOS_NODE(Sprite, "finish")->setVisible(false);
				COCOS_NODE(Sprite, "finish")->stopAllActions();
                
                if(GameTableLogic::getInstance()->isQueueGame())
                    ;
                else
                    GTLogic()->sendAgreeGame();
                
				break;
			}
		default:
			break;
		}
	}

	void GameManager::leaveButtonClickCallBack(Ref* ref,Widget::TouchEventType type)
	{
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			{
				COCOS_NODE(Sprite, "finish")->setVisible(false);
				COCOS_NODE(Sprite, "finish")->stopAllActions();
				GTLogic()->sendUserUp();
				break;
			}
		default:
			break;
		}
	}

	void GameManager::endQueMen(INT queColor, bool isRand)
	{
		COCOS_NODE(Sprite, "queMen")->setVisible(false);
		// 发送定缺事件
		tagDingQueEx que;
		_queMen = queColor;
		// 取牌最少的花色，否则取左边的花色
		if (isRand)
		{
			int countWan = 0;
			int countTiao = 0;
			int countTong = 0;
			for (auto v : _vecUser.at(0)->getOwnPool()->getListCard())
			{
				if (v->getCardColor() == mahjongColor::WAN)
				{
					++countWan;
				}
				else if  (v->getCardColor() == mahjongColor::TIAO)
				{
					++countTiao;
				}
				else  if  (v->getCardColor() == mahjongColor::TONG)
				{
					++countTong;
				}
			}
			
			auto min = (countWan <= countTiao ? countWan : countTiao);
			min = (min <= countTong ? min : countTong);
			if (countWan == min)
			{
				_queMen = 0;
			}
			else if (countTiao == min)
			{
				_queMen = 1;
			}
			else if (countTong == min)
			{
				_queMen = 2;
			}

		}
		que.Clear();
		que.byQue = _queMen;
		que.byUser = RoomLogic()->loginResult.pUserInfoStruct.bDeskStation;
		RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, THING_DING_QUE, &que, sizeof(tagDingQueEx));
		endAllTimer();
	}

	void GameManager::endAllTimer()
	{
		resetDiKuang();
		this->unschedule(schedule_selector(GameManager::waitTimer));
		//this->unschedule(schedule_selector(GameManager::queMenTimer));
		this->unschedule(schedule_selector(GameManager::waitOutTimer));
		this->unschedule(schedule_selector(GameManager::waitThinkTimer));
		this->unschedule(schedule_selector(GameManager::waitActionTimer));

		_pAtlasTimerNumber->setVisible(false);
		for (auto i = 0; i < PLAY_COUNT; i++)
		{
			COCOS_NODE(Sprite, StringUtils::format("timerPoint%d", i+1))->setVisible(false);
		}
	}

	int GameManager::getRd_A_N(int a, int n)
	{
		srand(time(nullptr));
		auto x = int(rand() % (n+1));
		if (x < a)
		{
			x = a;
		}
		return x;
	}

	void GameManager::startTimer(const sitDir& timerPointDir, timerType timerType)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(_clockID);
		// 终止其他计时器
		endAllTimer();
		// 配置指针
		COCOS_NODE(Sprite, StringUtils::format("timerPoint%d", INT(timerPointDir)))->setVisible(true);

		float addTm = 0.15f;
		float timerSeq = 1.0f;
		// 配置剩余时间
		switch (timerType)
		{
		case WAIT_START:
			{
				setRemainTime(GTLogic()->getGameStation().byBeginTime + addTm);
				break;
			}
		case DING_QUE:
			{
				setRemainTime(GTLogic()->getGameStation().ucDingQueTime + addTm);
				break;
			}
		case WAIT_OUT:
			{
				_isSouthPlayOper = (timerPointDir == sitDir::SOUTH_DIR);		// 是否本人操作
				setRemainTime(GTLogic()->getGameStation().byOutTime + addTm);
				break;
			}
		case WAIT_ACTION:
			{
				setRemainTime(GTLogic()->getGameStation().byThinkTime + addTm);
				break;
			}
		case WAIT_BLOCK:
			{
				setRemainTime(GTLogic()->getGameStation().byThinkTime + addTm);
				break;
			}
		
		default:
			break;
		}

		// 显示指针
		_pAtlasTimerNumber->setString(StringUtils::format("%d", INT(_remainTime)));
		_pAtlasTimerNumber->setVisible(true);
		
		// log
		// 启动计时器
		switch (timerType)
		{
		case WAIT_START:
			{
				HNAudioEngine::getInstance()->playBackgroundMusic((MUSIC_COMMON_PATH+"waiting.mp3").c_str());
				this->schedule(schedule_selector(GameManager::waitTimer), timerSeq);
				break;
			}
		case DING_QUE:
			{
				break;
			}
		case WAIT_OUT:
			{
				this->schedule(schedule_selector(GameManager::waitOutTimer), timerSeq);
				break;
			}
		case WAIT_BLOCK:
			{
				this->schedule(schedule_selector(GameManager::waitThinkTimer), timerSeq);
				break;
			}
		case WAIT_ACTION:
			{
				this->schedule(schedule_selector(GameManager::waitActionTimer), timerSeq);
				break;
			}
		default:
			break;
		}
	}

	void GameManager::waitTimer(float dt)
	{
		_pAtlasTimerNumber->setString(StringUtils::format("%d", INT(_remainTime)));
		_remainTime -= dt;
		if (_remainTime <= 0.0f)
		{
			GTLogic()->sendAgreeGame();
			HNAudioEngine::getInstance()->stopBackgroundMusic();
			endAllTimer();
		}
	}

	void GameManager::queMenTimer(float dt)
	{
		_pAtlasTimerNumber->setString(StringUtils::format("%d", INT(_remainTime)));
		_remainTime -= dt;
		if (_remainTime <= 0.0f)
		{
			endQueMen(0, true);
			endAllTimer();
		}
	}

	void GameManager::waitOutTimer(float dt)
	{
		_pAtlasTimerNumber->setString(StringUtils::format("%d", INT(_remainTime)));
		_remainTime -= dt;
		if (_remainTime <= 0.0f)
		{
			// 自动出牌
			if (_isSouthPlayOper)
			{
				autoSendCard(sitDir::SOUTH_DIR);
			}
			endAllTimer();
		}
		else
		{
			if (_isTrusteeShep)  // 托管
			{
				// 自动出牌
				if (_isSouthPlayOper)
				{
					autoSendCard(sitDir::SOUTH_DIR);
					endAllTimer();
				}
			}
		}
	}

	void GameManager::waitThinkTimer(float dt)
	{
		_pAtlasTimerNumber->setString(StringUtils::format("%d", INT(_remainTime)));
		_remainTime -= dt;
		if (_remainTime <= 0.0f)
		{
			endAllTimer();
		}
	}

	void GameManager::waitActionTimer(float dt)
	{
		_pAtlasTimerNumber->setString(StringUtils::format("%d", INT(_remainTime)));
		_remainTime -= dt;
		if (_remainTime <= 0.0f)
		{
			passAction();
		}
	}

	void GameManager::playBgTimer(float dt)
	{
		if (_isPlayGame && !CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
		{
			auto i = getRd_A_N(1, 5);
			HNAudioEngine::getInstance()->playBackgroundMusic((MUSIC_BG_PATH + StringUtils::format("bg%d.mp3", i)).c_str());
		}
	}

	void GameManager::autoReturnDeskWhenLag()
	{
		_isConnectSuccess = false;
		this->scheduleOnce(schedule_selector(GameManager::checkConnectSuccess), 1.0f);
		this->scheduleOnce(schedule_selector(GameManager::willReturnTimer), 2.0f);
	}

	void GameManager::checkConnectSuccess(float dt)
	{
		if (!_isConnectSuccess)
		{
			auto promt = GamePromptLayer::create();
			promt->showPrompt(GBKToUtf8("游戏连接已失效，即将返回..."));
			promt->removeFromParentAndCleanup(false);
			this->addChild(promt);
		}
	}

	void GameManager::willReturnTimer(float dt)
	{
		if (!_isConnectSuccess)
		{
			returnDesk();
		}
	}

	void GameManager::returnDesk(bool safeLeave)
	{
		if (RoomLogic()->isConnect())
		{
			if (safeLeave)
			{
				GTLogic()->sendUserUp();
			}
			else
			{
				trusteeshepSendMsg(true);
				GTLogic()->sendForceQuit();    // 发送强制退出消息	
			}
			GamePlatform::returnPlatform(LayerType::DESKLIST);
		}
		HNAudioEngine::getInstance()->stopBackgroundMusic();
		GamePlatform::returnPlatform(LayerType::DESKLIST);
	}

	void GameManager::trusteeshepSendMsg(bool trusteeshep)
	{
		UserTouGuan msg;
		memset(&msg, 0, sizeof(msg));
		msg.byStation = RoomLogic()->loginResult.pUserInfoStruct.bDeskStation;
		msg.bIsTuoGuan = trusteeshep;
		RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, ASS_AUTOCARD, &msg, sizeof(msg));
	}

	void GameManager::setButton()
	{
		// 设置可视可用
		resetDiKuang();

		// 设置回调函数
		COCOS_NODE(Button, "start")->addTouchEventListener(CC_CALLBACK_2(GameManager::startButtonClickCallBack, this));
		COCOS_NODE(Button, "trusteeshep")->addTouchEventListener(CC_CALLBACK_2(GameManager::trusteeshepButtonClickCallBack, this));
		COCOS_NODE(Button, "quxiaotuoguang")->addTouchEventListener(CC_CALLBACK_2(GameManager::quxiaotuoguangButtonClickCallBack, this));
		COCOS_NODE(Button, "return")->addTouchEventListener(CC_CALLBACK_2(GameManager::returnButtonClickCallBack, this));
		COCOS_NODE(Button, "chi")->addTouchEventListener(CC_CALLBACK_2(GameManager::chiButtonClickCallBack, this));
		COCOS_NODE(Button, "peng")->addTouchEventListener(CC_CALLBACK_2(GameManager::pengButtonClickCallBack, this));
		COCOS_NODE(Button, "gang")->addTouchEventListener(CC_CALLBACK_2(GameManager::gangButtonClickCallBack, this));
		COCOS_NODE(Button, "ting")->addTouchEventListener(CC_CALLBACK_2(GameManager::tingButtonClickCallBack, this));
		COCOS_NODE(Button, "hu")->addTouchEventListener(CC_CALLBACK_2(GameManager::huButtonClickCallBack, this));
		COCOS_NODE(Button, "qi")->addTouchEventListener(CC_CALLBACK_2(GameManager::qiButtonClickCallBack, this));
		COCOS_NODE(Button, "continue")->addTouchEventListener(CC_CALLBACK_2(GameManager::continueButtonClickCallBack, this));
		COCOS_NODE(Button, "leave")->addTouchEventListener(CC_CALLBACK_2(GameManager::leaveButtonClickCallBack, this));
		COCOS_NODE(Button, "continue1")->addTouchEventListener(CC_CALLBACK_2(GameManager::continueButtonClickCallBack, this));
		COCOS_NODE(Button, "leave1")->addTouchEventListener(CC_CALLBACK_2(GameManager::leaveButtonClickCallBack, this));
		COCOS_NODE(Button, "continue2")->addTouchEventListener(CC_CALLBACK_2(GameManager::continueButtonClickCallBack, this));
		COCOS_NODE(Button, "leave2")->addTouchEventListener(CC_CALLBACK_2(GameManager::leaveButtonClickCallBack, this));
	}

	void GameManager::addUI()
	{
		// 麻将
		this->setScale(_globalScale);
		auto origin = _newOrigin;
		_vecHasOutCardCount.assign(4, 0);
		_lineCount = 8;
		COCOS_NODE(Layout, "RootPanel")->setPosition(_newOrigin);
		// 增加接受牌的位置
		auto pos = COCOS_NODE(Layout, "handPanel1")->getPosition() + origin;
		auto size = COCOS_NODE(Layout, "handPanel1")->getContentSize();
		Vec2 catchPos1, catchPos2, catchPos3, catchPos4;
		auto spN = Sprite::createWithSpriteFrameName("liNorthxiao.png");
		auto spS = Sprite::createWithSpriteFrameName("ZhengDaTong1.png");
		auto spEW = Sprite::createWithSpriteFrameName("liYou.png");
		catchPos1 = pos + Vec2(size.width - spS->getContentSize().width / 4.0f, size.height/2);
		_vecCatchEndPos.push_back(catchPos1);

		
		pos = COCOS_NODE(Layout, "handPanel2")->getPosition() + origin;
		size = COCOS_NODE(Layout, "handPanel2")->getContentSize();
		catchPos2 = pos + Vec2(size.width/2, spEW->getContentSize().height / 1.5f);
		_vecCatchEndPos.push_back(catchPos2);

		pos = COCOS_NODE(Layout, "handPanel3")->getPosition() + origin;
		size = COCOS_NODE(Layout, "handPanel3")->getContentSize();
		catchPos3 = pos + Vec2(spN->getContentSize().width/3.0, size.height/2);
		_vecCatchEndPos.push_back(catchPos3);

		pos = COCOS_NODE(Layout, "handPanel4")->getPosition() + origin;
		size = COCOS_NODE(Layout, "handPanel4")->getContentSize();
		catchPos4 = pos + Vec2(COCOS_NODE(Layout, "handPanel2")->getContentSize().width, size.height - spEW->getContentSize().height/1.5);
		_vecCatchEndPos.push_back(catchPos4);

		_vecCatchPos.push_back(catchPos1);
		_vecCatchPos.push_back(catchPos2);
		_vecCatchPos.push_back(catchPos3);
		_vecCatchPos.push_back(catchPos4);

	   // 设置麻将底牌的摆放方式
		northSouthCount = southCardNum;
		westEastCount = westCardNum;

		// 增加卡片   // 顺时针编码  自身开始
		Card* card = nullptr;                     //  卡牌
		Vec2 initPos = Vec2::ZERO;           //  容器起始点
		Layout* layout = nullptr;              // 容器
		Size panelSZ;                               // 容器尺寸
		Size cardSZ;								  //  卡片尺寸
		Vec2 startAddInPos;					  //  起始增加卡牌位置
		Vec2 panelPos;                           // 容器位置
		float height = PLIST_NODE("DaoBeiEW")->getContentSize().height/4.0;                    // 东西卡牌纯背板和总高差
		INT zorder = CARD_TOTAL_NUMBER + 1;
		for (auto i = 0; i < CARD_TOTAL_NUMBER; i++)                  // 摆底牌
		{
			if (i < northSouthCount)                      // 南方最右开始摆
			{
				card = MahjongCard::create(mahjongCreateType::DI_BEI_SHU, sitDir::MID_DIR);
				card->setCardZorder(--zorder);
				layout= COCOS_NODE(Layout, "bottomPanel1");
				panelSZ = layout->getContentSize();
				cardSZ = card->getCardSize();
				panelPos = layout->getPosition() + origin;
				startAddInPos = Vec2(panelSZ.width - cardSZ.width/2, cardSZ.height/2) + panelPos;
				if (i % 2 == 1)  // 底
				{
					initPos = startAddInPos;
				}
				else
				{
					initPos = startAddInPos + Vec2(0, cardSZ.height*0.25f);
				}
				initPos.x -= i/2 * cardSZ.width;
			}
			else if (i < northSouthCount + westEastCount)     // 西方最下开始摆
			{
				card = MahjongCard::create(mahjongCreateType::DI_BEI_HENG, sitDir::MID_DIR);
				card->setCardZorder(--zorder);
				layout= COCOS_NODE(Layout, "bottomPanel2");
				panelPos = layout->getPosition() + origin;
				panelSZ = layout->getContentSize();
				cardSZ = card->getCardSize();
				startAddInPos = Vec2(panelSZ.width/2, cardSZ.height/2) + panelPos;
				if (i % 2 == 1)  // 底
				{
					initPos = startAddInPos;
				}
				else
				{
					initPos = startAddInPos + Vec2(0, 0.33*cardSZ.height);
				}
				initPos.y += (i- northSouthCount)/2* (0.66*cardSZ.height);
			}
			else if (i < northSouthCount * 2 + westEastCount)                // 北方最左开始摆
			{
				card = MahjongCard::create(mahjongCreateType::DI_BEI_SHU, sitDir::MID_DIR);
				card->setCardZorder(--zorder);
				layout= COCOS_NODE(Layout, "bottomPanel3");
				panelSZ = layout->getContentSize();
				panelPos = layout->getPosition() + origin;
				cardSZ = card->getCardSize();
				startAddInPos = Vec2(cardSZ.width/2, cardSZ.height/2) + panelPos;
				if (i % 2 == 1)  // 底
				{
					initPos = startAddInPos;
				}
				else
				{
					initPos = startAddInPos + Vec2(0, 0.25*cardSZ.height);
				}
				initPos.x += (i -northSouthCount-westEastCount)/2  * cardSZ.width;
			}
			else if (i < northSouthCount * 2 + westEastCount * 2)                      // 懂方最上开始摆
			{
				if (i ==  northSouthCount * 2 + westEastCount)
				{
					zorder -= 26;
				}
				card = MahjongCard::create(mahjongCreateType::DI_BEI_HENG, sitDir::MID_DIR);
				layout= COCOS_NODE(Layout, "bottomPanel4");
				panelPos = layout->getPosition() + origin;
				panelSZ = layout->getContentSize();
				cardSZ = card->getCardSize();
				startAddInPos = Vec2(cardSZ.width/2, panelSZ.height - cardSZ.height/2) + panelPos;
				if (i % 2 == 1)  // 底
				{
					initPos = startAddInPos + Vec2(0, -cardSZ.height*0.33);
					card->setCardZorder(zorder-1);
				}
				else
				{
					initPos = startAddInPos;
					zorder += 2;
					card->setCardZorder(zorder);
				}
				initPos.y -= (i -2 *northSouthCount-westEastCount)/2  * (0.66*cardSZ.height);
			}

			layout->addChild(card);
			card->setCardPos(initPos);
			_vecBottomCard.push_back(card);
		}

		// 制作正立牌容器
		for (auto i = 0; i < 3; i++)
		{
			std::vector<std::vector<Card *>> vVec;
			for (auto j = 1; j <= 9; j++)
			{
				std::vector<Card *> vec;
				for (auto k = 0; k < 4; k++)
				{
					auto card = MahjongCard::create(mahjongCreateType::ZHENG_LI, sitDir::MID_DIR, i * 10 + j);
					this->addChild(card);
					card->setCardTouchEvent();
					card->setCardVisible(false);
					card->setCardZorder(340);
					vec.push_back(card);
				}
				vVec.push_back(vec);
				vec.clear();
			}
			_vvvZhengLiCard.push_back(vVec);
			vVec.clear();
		}

		// 增加正大红中
		std::vector<Card *> vColor;
		for (auto i = 0; i < 4; i++)
		{
			auto card = MahjongCard::create(mahjongCreateType::ZHENG_LI, sitDir::MID_DIR, CMjEnum::MJ_TYPE_ZHONG);
			this->addChild(card);
			card->setCardTouchEvent();
			card->setCardVisible(false);
			card->setCardZorder(340);
			vColor.push_back(card);
		}
		_vvColorZhengDa.push_back(vColor);

		// 制作胡牌容器

		for (auto i = 0; i < PLAY_COUNT; i++)
		{
			std::vector<Card *> v;
			for (auto j = 0; j < 14; j++)
			{
				Card* card = nullptr;
				if (i == 0)  // 南
				{
					card = MahjongCard::create(mahjongCreateType::HU_BEI_DAO, sitDir::SOUTH_DIR);
				} 
				else if (i == 1)   // 西
				{
					card = MahjongCard::create(mahjongCreateType::DI_BEI_HENG, sitDir::WEST_DIR);
					card->setCardZorder(j+1);
				}
				else if (i == 2)  // 北
				{
					card = MahjongCard::create(mahjongCreateType::DI_BEI_SHU, sitDir::NORTH_DIR);
				}
				else if (i == 3)  // 东
				{
					card = MahjongCard::create(mahjongCreateType::DI_BEI_HENG, sitDir::EAST_DIR);
					card->setCardZorder(15-j);
				}
				
				this->addChild(card);
				card->setCardVisible(false);

				v.push_back(card);
			}
			_vvecHuCard.push_back(v);
			v.clear();
		}
		

		// 制作正倒牌容器
		for (auto i = 0; i < 3; i++)
		{
			std::vector<std::vector<Card *>> vVec;
			for (auto j = 1; j <= 9; j++)
			{
				std::vector<Card *> vec;
				for (auto k = 0; k < 4; k++)
				{
					auto card = MahjongCard::create(mahjongCreateType::ZHENG_DAO, sitDir::MID_DIR, i * 10 + j);
					this->addChild(card);
					card->setCardVisible(false);
					card->setCardZorder(_bigShowZorder);
					vec.push_back(card);
				}
				vVec.push_back(vec);
				vec.clear();
			}
			_vvvZhengDaoCard.push_back(vVec);
			vVec.clear();
		}

		// 增加倒大红中
		std::vector<Card *> vDaoDaColor;
		for (auto i = 0; i < 4; i++)
		{
			auto card = MahjongCard::create(mahjongCreateType::ZHENG_DAO, sitDir::MID_DIR, CMjEnum::MJ_TYPE_ZHONG);
			this->addChild(card);
			card->setCardTouchEvent();
			card->setCardVisible(false);
			card->setCardZorder(_bigShowZorder);
			vDaoDaColor.push_back(card);
		}
		_vvColorZhengDao.push_back(vDaoDaColor);

		
		// 制作背立牌容器
		for (auto i = 0; i < 30; i++)
		{
			auto card = MahjongCard::create(mahjongCreateType::QIAN_LI, sitDir::MID_DIR);
			this->addChild(card);
			card->setCardVisible(false);
			_vecBeiLiCard.push_back(card);
		}

		// 制作左立牌容器
		for (auto i = 0; i < 30; i++)
		{
			auto card = MahjongCard::create(mahjongCreateType::ZUO_LI, sitDir::MID_DIR);
			this->addChild(card);
			card->setCardVisible(false);
			_vecZuoLiCard.push_back(card);
		}

		// 制作右立牌容器
		for (auto i = 0; i < 30; i++)
		{
			auto card = MahjongCard::create(mahjongCreateType::YOU_LI, sitDir::MID_DIR);
			this->addChild(card);
			card->setCardVisible(false);
			_vecYouLiCard.push_back(card);
		}

		// 制作倒牌容器
		for (auto m = 0; m < 4; m++)
		{
			mahjongCreateType type;
			if (m == 0)
			{
				type = mahjongCreateType::DI_ZHENG_SOUTH;
			}
			else if (m == 1)
			{
				type = mahjongCreateType::DI_ZHENG_WEST;
			}
			else if (m == 2)
			{
				type = mahjongCreateType::DI_ZHENG_NORTH;
			}
			else if (m == 3)
			{
				type = mahjongCreateType::DI_ZHENG_EAST;
			}
			std::vector<std::vector<std::vector<Card *>>>vvVec;
			for (auto i = 0; i < 3; i++)
			{
				std::vector<std::vector<Card *>> vVec;
				for (auto j = 1; j <= 9; j++)
				{
					std::vector<Card *> vec;
					for (auto k = 0; k < 4; k++)
					{
						auto card = MahjongCard::create(type, sitDir::MID_DIR, i * 10 + j);
						this->addChild(card);
						card->setCardVisible(false);
						card->setCardZorder(350);
						vec.push_back(card);
					}
					vVec.push_back(vec);
					vec.clear();
				}
				vvVec.push_back(vVec);
				vVec.clear();
			}
			_vvvvXiaoDaoCard.push_back(vvVec);
			vvVec.clear();
		}

		// 制作花牌倒牌容器
		std::vector<std::vector<Card *>> vvColorXiaoDao;
		for (auto m = 0; m < 4; m++)
		{
			std::vector<Card *>	vColorXiaoDao;
			mahjongCreateType type;
			if (m == 0)
			{
				type = mahjongCreateType::DI_ZHENG_SOUTH;
			}
			else if (m == 1)
			{
				type = mahjongCreateType::DI_ZHENG_WEST;
			}
			else if (m == 2)
			{
				type = mahjongCreateType::DI_ZHENG_NORTH;
			}
			else if (m == 3)
			{
				type = mahjongCreateType::DI_ZHENG_EAST;
			}
			for (auto n = 0; n < 4; n++)
			{
				auto card = MahjongCard::create(type, sitDir::MID_DIR, CMjEnum::MJ_TYPE_ZHONG);
				this->addChild(card);
				card->setCardVisible(false);
				card->setCardZorder(350);
				vColorXiaoDao.push_back(card);
			}
			vvColorXiaoDao.push_back(vColorXiaoDao);
		}
		_vvvColorXiaoDao.push_back(vvColorXiaoDao);
		// 结束容器创建


		this->schedule(schedule_selector(GameManager::playBgTimer), 5.0f);

		reParent("dikuang");
		reParent("finish");
		reParent("finish1");
		reParent("finish2");
		reParent("niaoKuang");
		for (auto i = 1; i <= PLAY_COUNT; i++)
		{
			reParent(StringUtils::format("timerPoint%d", i));
		}

		pos = COCOS_NODE(Button, "start")->getPosition();
		COCOS_NODE(Button, "start")->removeFromParent();
		this->addChild(COCOS_NODE(Button, "start"));
		COCOS_NODE(Button, "start")->setLocalZOrder(99998);
		COCOS_NODE(Button, "start")->setPosition(pos + _newOrigin);

		// 启动timer
		auto spSZ = Sprite::create(FNT_PATH + "timeNumber.png")->getContentSize();
		_pAtlasTimerNumber = LabelAtlas::create("0123456789", FNT_PATH + "timeNumber.png", spSZ.width/10.0f, spSZ.height, '0');
		this->addChild(_pAtlasTimerNumber);
		_pAtlasTimerNumber->setScale(_globalScale);
		_pAtlasTimerNumber->setPosition(this->convertToNodeSpace(COCOS_NODE(Text, "timer")->convertToWorldSpace(Vec2(COCOS_NODE(Text, "timer")->getContentSize().width/2, COCOS_NODE(Text, "timer")->getContentSize().height/2))));
		_pAtlasTimerNumber->setString("0");
		_pAtlasTimerNumber->setVisible(false);
		_pAtlasTimerNumber->ignoreAnchorPointForPosition(false);
		_pAtlasTimerNumber->setAnchorPoint(Vec2(0.5f, 0.5f));
		_pAtlasTimerNumber->setLocalZOrder(topZorder);
		// 增加使用者
		for (auto i = 0; i < PLAY_COUNT; i++)
		{
			auto user = MahjongCardPoolUser::create();
			this->addChild(user);
			_vecUser.push_back(user);
		}


		//  1. 创建使用者
		//  2. 为使用者添加卡片池
		//  3. 添加卡片池，用于发牌流程
		auto southPool = Factory::createSouthPool(1);
		_vecUser.at(0)->setPool(southPool);

		auto westPool = Factory::createWestPool(1);
		_vecUser.at(1)->setPool(westPool);

		auto northPool = Factory::createNorthPool(1);
		_vecUser.at(2)->setPool(northPool);

		auto eastPool = Factory::createEastPool(1);
		_vecUser.at(3)->setPool(eastPool);

		// 添加4个方向的牌池给底牌
		addCardPool(southPool);
		addCardPool(westPool);
		addCardPool(northPool);
		addCardPool(eastPool);

		// 添加打牌的动作
		_touchCard = Factory::createTouchCardAction();
		this->addChild(_touchCard);
		_meldedKong = Factory::createMeldedKongAction();
		this->addChild(_meldedKong);
		_concealedKong = Factory::createConcealedKongAction();
		this->addChild(_concealedKong);
		_touchKong = Factory::createTouchKongAction();
		this->addChild(_touchKong);
		_huCard = Factory::createHuCardAction();
		this->addChild(_huCard);

		_touchCard->setPosition(_touchCard->getPosition() + _newOrigin);
		_meldedKong->setPosition(_meldedKong->getPosition() + _newOrigin);
		_concealedKong->setPosition(_concealedKong->getPosition() + _newOrigin);
		_touchKong->setPosition(_touchKong->getPosition() + _newOrigin);
		_huCard->setPosition(_huCard->getPosition() + _newOrigin);

		auto cardEW = MahjongCard::create(mahjongCreateType::DI_ZHENG_WEST, sitDir::MID_DIR, 1);
		_mahjongSzEW = cardEW->getCardSize();
		cardEW->getCardSprite()->removeFromParent();
		cardEW->removeFromParent();
		auto cardNS = MahjongCard::create(mahjongCreateType::DI_ZHENG_NORTH, sitDir::MID_DIR, 1);
		_mahjongSzNS = cardNS->getCardSize();
		cardNS->getCardSprite()->removeFromParent();
		cardNS->removeFromParent();
	}

	void GameManager::reParent(std::string name)
	{
		auto pos = COCOS_NODE(Sprite, name)->getPosition();
		COCOS_NODE(Sprite, name)->removeFromParentAndCleanup(false);
		this->addChild(COCOS_NODE(Sprite, name));
		COCOS_NODE(Sprite, name)->setLocalZOrder(topZorder);
		COCOS_NODE(Sprite, name)->setPosition(pos + _newOrigin);
	}

	void GameManager::resetDiKuang()
	{
		COCOS_NODE(Sprite, "dikuang")->setVisible(false);
		COCOS_NODE(Button, "chi")->setBright(false);
		COCOS_NODE(Button, "chi")->setEnabled(false);
		COCOS_NODE(Button, "peng")->setBright(false);
		COCOS_NODE(Button, "peng")->setEnabled(false);
		COCOS_NODE(Button, "gang")->setBright(false);
		COCOS_NODE(Button, "gang")->setEnabled(false);
		COCOS_NODE(Button, "ting")->setBright(false);
		COCOS_NODE(Button, "ting")->setEnabled(false);
		COCOS_NODE(Button, "hu")->setBright(false);
		COCOS_NODE(Button, "hu")->setEnabled(false);
	}

	void GameManager::addUser(const sitDir& dir, const UserInfoStruct& user)
	{
		_vecUser.at(INT(dir) - sitDir::SOUTH_DIR)->setUserInfo(user);     // 设置玩家信息
		COCOS_NODE(Text, StringUtils::format("name%d", dir))->setString(GBKToUtf8(user.nickName));
		COCOS_NODE(Text, StringUtils::format("money%d", dir))->setString(StringUtils::format("%d", user.i64Money).c_str());
		if (user.bUserState == USER_ARGEE && !_isPlayGame)
		{
			COCOS_NODE(Sprite, StringUtils::format("ready%d", dir))->setVisible(true);
		}
		else
		{
			COCOS_NODE(Sprite, StringUtils::format("ready%d", dir))->setVisible(false);
		}
	}

	void GameManager::userLeave(const sitDir& dir)
	{
		UserInfoStruct user;
		user.nickName[0] = '\0';
		if (INT(_vecUser.size()) >= INT(dir))
		{
		_vecUser.at(INT(dir) - 1)->setUserInfo(user);     // 设置玩家信息
		}
		COCOS_NODE(Text, StringUtils::format("name%d", dir))->setString("");
		COCOS_NODE(Text, StringUtils::format("money%d", dir))->setString("");
		COCOS_NODE(Sprite, StringUtils::format("ready%d", dir))->setVisible(false);
	}

	void GameManager::userTrusteeshep(const sitDir& dir, bool isTrust)
	{
		if (dir == sitDir::SOUTH_DIR)
		{
			_isTrusteeShep = isTrust;
			COCOS_NODE(Button, "trusteeshep")->setVisible(!isTrust);
			COCOS_NODE(Button, "quxiaotuoguang")->setVisible(isTrust);
		}
		COCOS_NODE(Sprite, StringUtils::format("tuoguang%d", dir))->setVisible(isTrust);
	}

	void GameManager::userAgree(const sitDir& dir)
	{
		if (dir == sitDir::SOUTH_DIR)
		{
			restartGame();
			HNAudioEngine::getInstance()->stopBackgroundMusic();
			COCOS_NODE(Button, "start")->setVisible(false);
			COCOS_NODE(Sprite, "waiting")->setVisible(true);
			_pAtlasTimerNumber->setVisible(false);

			endAllTimer();
			playCommonSound("Ready");
			for (auto i = 1; i <= PLAY_COUNT; i++)
			{
				COCOS_NODE(Sprite, StringUtils::format("timerPoint%d", i))->setVisible(false);
			}
		}
		COCOS_NODE(Sprite, StringUtils::format("ready%d", dir))->setVisible(true);

	}

	void GameManager::startGame(const bool& autoBegin)
	{
		restartGame();
		HNAudioEngine::getInstance()->stopBackgroundMusic();
		endAllTimer();
		_autoBegin = autoBegin;
		COCOS_NODE(Sprite, "waiting")->setVisible(false);
		COCOS_NODE(Button, "start")->setVisible(false);
		COCOS_NODE(Button, "return")->setVisible(false);
		
		for (auto i = 1; i <= PLAY_COUNT; i++)
		{
			COCOS_NODE(Sprite, StringUtils::format("ready%d", i))->setVisible(false);
		}
	}

	void GameManager::playGame()
	{
		setPlayGame(true);
		COCOS_NODE(Sprite, "waiting")->setVisible(false);
		getVecUser().at(0)->getOwnPool()->sortCard();							// 整理手牌	
		startOutCard(_startDir);																// 第一次出牌时
		startTimer(_startDir, timerType::WAIT_OUT);								// 开始出牌
	}

	const vector<CardPoolUser *>& GameManager::getVecUser() 
	{ 
		return _vecUser; 
	}

	void GameManager::catchCard(const sitDir& dir, const INT& number, const bool& head)
	{
		if (!_isPlayGame)
		{
			return;
		}
//		log("catch begin : _lastCardIndex = %d _currCardIndex = %d", _lastCardIndex, _currCardIndex);
		//resetDiKuang();
		//_vecUser.at(INT(dir)-INT(sitDir::SOUTH_DIR))->getOwnPool()->setCanCheckAction(); // 重置可做动作
		setCurrOperDir(dir);
		setCurrOperNumber(number);
		Card * card = nullptr;
		if (head)
		{
			card = commonCatchCard(dir, number);
			if (card == nullptr)
			{
				return;
			}
		}
		else
		{
			card = tailCatchCard(dir, number);
		}
//		log("catch end : _lastCardIndex = %d _currCardIndex = %d", _lastCardIndex, _currCardIndex);
	}

	void GameManager::outCard(const sitDir& dir, const INT& number)
	{

		if (dir == SOUTH_DIR)
		{
			auto listCard = _vecCardPool.at(0)->getListCard();
			for (auto bCard : listCard)
			{
				bCard->setCardColor(Color3B(255, 255, 255));
				bCard->setSelect(false);
			}
		}
		// 停止计时器
		for (auto i = 1; i <= PLAY_COUNT; i++)
		{
			COCOS_NODE(Sprite, StringUtils::format("timerPoint%d", i))->setVisible(false);
		}
		_pAtlasTimerNumber->setVisible(false);
		setCurrInfo(dir, number);
		// 1. 添加显示大图标
		addBigShowSp(dir, number); 

		// 2. 计算出牌容器位置
		Vec2 outPos = _vecUser.at(INT(dir)-INT(sitDir::SOUTH_DIR))->getOwnPool()->getOutToDeskPos();;

		// 3. 取得牌在出牌容器中的位置
		Vec2 setPos = getOutCardDeskPos(dir);

		// 4. 取得将出的牌
		auto card =  getXiaoDaoCard(dir, number);
		auto zorder = card->getCardZorder();
		card->setCardPos(outPos);
		card->setCardZorder(1000);   // 提高Z轴

		// 5. 移动动画
		card->getCardSprite()->runAction(Sequence::create(
				EaseSineOut::create(MoveTo::create(0.5f, setPos)),
				CallFunc::create(CC_CALLBACK_0(Card::setCardZorder, card, zorder)),
				nullptr
			) );

		// 6. 整理手牌
		afterOutCard(dir);
	}

	void GameManager::passAction()
	{
		if (_currOperDir != sitDir::SOUTH_DIR)   // 暗杠和自摸 计时器不终止，继续等待出牌
		{
			endAllTimer();
		}
		else
		{
			resetDiKuang();
		}
		RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, THING_GUO_PAI);
	}

	void GameManager::showMoney()
	{
		this->runAction(
			Sequence::createWithTwoActions(
			DelayTime::create(0.10f),
			CallFunc::create(CC_CALLBACK_0(GameManager::showMoneyCallBack, this))
			));
	}

	void GameManager::showMoneyCallBack()
	{
		auto userInfo = RoomLogic()->loginResult.pUserInfoStruct;
		// 显示玩家
		vector<UserInfoStruct *> vec;
		UserInfoModule()->findDeskUsers(userInfo.bDeskNO, vec);
		for (auto &v : vec)
		{
			addUser(GTLogic()->getUserDir(v->bDeskStation), *v);
		}
	}

	void GameManager::reconnected(
			std::vector<std::vector<INT>>& vvHandCard, 
			std::vector<std::vector<INT>>& vvOutCard, 
			std::vector<std::vector<std::vector<INT>>>& vvvActionCard,
			std::vector<std::vector<INT>>& vvMenPai,
			std::vector<bool>& vTrusteeshep,
			std::vector<INT>& vQueMen,
			sitDir& ntDir)
	{
		setPlayGame(true);
		restartGame();
		// 基础属性
		auto info = GTLogic()->getGameStation();
		_startDir = GTLogic()->getUserDir(info.byNtStation);
		_autoBegin = (info.bAutoBegin == 1);    // 自动开始
		auto userInfo = RoomLogic()->loginResult.pUserInfoStruct;
		_queMen = info.m_byDingQue[userInfo.bDeskStation];   // 缺门
		_isPlayGame = true;  // 正在玩
		_isTrusteeShep = false;   // 是否托管
		_isSouthPlayOper = (info.m_byNowOutStation == userInfo.bDeskStation);  // 正南操作
		auto nowDir = GTLogic()->getUserDir(info.m_byNowOutStation);

		COCOS_NODE(Button, "return")->setVisible(false);	// 返回大厅隐藏
		COCOS_NODE(Button, "start")->setVisible(false);	// 开始隐藏

		// 显示出牌信息
		for (auto i = 1; i <= PLAY_COUNT; i++)
		{
			COCOS_NODE(Sprite, StringUtils::format("timerPoint%d", i))->setVisible(false);
		}
		_pAtlasTimerNumber->setVisible(false);


		// 显示玩家信息
		for (auto i = 0; i < PLAY_COUNT; i++)
		{
			auto nickName = GTLogic()->getUserBySeatNo(i)->nickName;
			auto money = GTLogic()->getUserBySeatNo(i)->i64Money;
			auto index = GTLogic()->getUserVecIndex(i);
			COCOS_NODE(Text, StringUtils::format("name%d", index + 1))->setString(GBKToUtf8(nickName));
			COCOS_NODE(Text, StringUtils::format("money%d", index + 1))->setString(StringUtils::format("%lld", money));
		}

		// 显示庄家/托管
		for (auto i = 1; i <= PLAY_COUNT; i++)
		{
			COCOS_NODE(Sprite, StringUtils::format("zhuang%d", i))->setVisible(i == INT(ntDir));          // 庄家
			COCOS_NODE(Sprite, StringUtils::format("tuoguang%d", i))->setVisible(vTrusteeshep.at(i - 1));   // 托管
			if (vQueMen.at(i-1) == 0)  // 缺万
			{
				COCOS_NODE(Sprite, StringUtils::format("que_wanSP%d", i))->setVisible(true);
				COCOS_NODE(Sprite, StringUtils::format("que_tiaoSP%d", i))->setVisible(false);
				COCOS_NODE(Sprite, StringUtils::format("que_tongSP%d", i))->setVisible(false);
			}
			else if (vQueMen.at(i-1) == 1) // 缺条
			{
				COCOS_NODE(Sprite, StringUtils::format("que_wanSP%d", i))->setVisible(false);
				COCOS_NODE(Sprite, StringUtils::format("que_tiaoSP%d", i))->setVisible(true);
				COCOS_NODE(Sprite, StringUtils::format("que_tongSP%d", i))->setVisible(false);
			}
			else if (vQueMen.at(i-1) == 2)  // 缺筒
			{
				COCOS_NODE(Sprite, StringUtils::format("que_wanSP%d", i))->setVisible(false);
				COCOS_NODE(Sprite, StringUtils::format("que_tiaoSP%d", i))->setVisible(false);
				COCOS_NODE(Sprite, StringUtils::format("que_tongSP%d", i))->setVisible(true);
			}
			
		}

		showPatternBottomCard(vvMenPai);   // 显示底牌
		showActionCard(vvvActionCard);   // 显示动作牌
		showHandCard(vvHandCard);   // 显示手牌
		showOutCard(vvOutCard);  // 显示出牌

		if (_isSouthPlayOper)   // 设置可操作
		{
			startOutCard(sitDir::SOUTH_DIR);
			_vecUser.at(0)->getOwnPool()->setCanOper(true);
		}
		else
		{
			_vecUser.at(0)->getOwnPool()->setCanOper(false);
		}
		resetCatchPos(true);
	}

	
	void GameManager::hideAllBottomCard(bool hide)
	{
		for (auto &v : _vecBottomCard)
		{
			v->setCardVisible(!hide);
		}
	}

	void GameManager::showPatternBottomCard(std::vector<std::vector<INT>>& vvec)
	{
		hideAllBottomCard();  // 隐藏所有
		int count = 0;
		for (auto &v : vvec)
		{
			for (auto &var : v)
			{
				_vecBottomCard.at(count++)->setCardVisible(var == 1);
			}
		}

		// 计算起点和终点
		auto bottomSize = _vecBottomCard.size();
		if ((*_vecBottomCard.rbegin())->isCardVisible())
		{
			count = bottomSize;
			for (auto btCardIter = _vecBottomCard.rbegin(); btCardIter != _vecBottomCard.rend(); btCardIter++)
			{
				auto card = *btCardIter;
				if (!card->isCardVisible())
				{
					_currCardIndex = count;
					break;
				}
				--count;
			}
			count = 0;
			for (auto btCardIter = _vecBottomCard.begin(); btCardIter != _vecBottomCard.end(); btCardIter++)
			{
				auto card = *btCardIter;
				if (!card->isCardVisible())
				{
					_lastCardIndex = (bottomSize + count - 1) % bottomSize;
					if (_lastCardIndex % 2 == 1 && _vecBottomCard.at(_lastCardIndex - 1)->isCardVisible())
					{
						--_lastCardIndex;
					}
					break;
				}
				count++;
			}
		}
		else
		{
			count = bottomSize;
			for (auto btCardIter = _vecBottomCard.rbegin(); btCardIter != _vecBottomCard.rend(); btCardIter++)
			{
				auto card = *btCardIter;
				if (card->isCardVisible())
				{
					_lastCardIndex = count;
					if (_lastCardIndex % 2 == 1 && _vecBottomCard.at(_lastCardIndex - 1)->isCardVisible())
					{
						--_lastCardIndex;
					}
					break;
				}
				--count;
			}
			count = 0;
			for (auto btCardIter = _vecBottomCard.begin(); btCardIter != _vecBottomCard.end(); btCardIter++)
			{
				auto card = *btCardIter;
				if (card->isCardVisible())
				{
					_currCardIndex = count;
					break;
				}
				count++;
			}
		}
	}

	void GameManager::showHandCard(std::vector<std::vector<INT>>& vvec)    // 显示手牌
	{
		auto msg = GTLogic()->getGameStation();
		vector<bool> vHu(PLAY_COUNT, false);
		for (auto i = 0; i < PLAY_COUNT; i++)
		{
			auto index = GTLogic()->getUserVecIndex(i);
			if (msg.m_bIsHu[i])
			{
				vHu.at(index) = true;
			}
		}
		for (size_t i = 0; i < vvec.size(); i++)
		{
			for (auto &v : vvec.at(i))
			{
				auto card = getCard(_vecCardPool.at(i)->getSitDir(), v);
				_vecCardPool.at(i)->addCard(card);
				_vecCardPool.at(i)->sortCard();
				_vecCardPool.at(i)->setHandCardPos();
			}
			if (vHu.at(i))
			{
				_vecCardPool.at(i)->huCard(true, false);
			}
		}
	}

	void GameManager::showActionCard(std::vector<std::vector<std::vector<INT>>>& vvvec)  // 显示动作牌
	{
		for (size_t i = 0; i < vvvec.size(); i++)
		{
			for (size_t j = 0; j < vvvec.at(i).size(); j++)
			{
				// 碰
				if ( (vvvec.at(i).at(j).size() == 3) && (vvvec.at(i).at(j).at(0) == vvvec.at(i).at(j).at(1)) && (vvvec.at(i).at(j).at(1) == vvvec.at(i).at(j).at(2)))
				{
					_vecCardPool.at(i)->addSomeOutCards(3, vvvec.at(i).at(j).at(0), false);
				}
				else if (vvvec.at(i).at(j).size() == 4)  // 杠
				{
					_vecCardPool.at(i)->addSomeOutCards(4, vvvec.at(i).at(j).at(0), false);
				}
			}
		}
	}

	void GameManager::showOutCard(std::vector<std::vector<INT>>& vvec)
	{
		for (size_t i = 0; i < vvec.size(); i++)
		{
			for (size_t j = 0; j < vvec.at(i).size() ; j++)
			{
				auto pos = getOutCardDeskPos(sitDir(i+1));
				auto card = getXiaoDaoCard(sitDir(i+1), vvec.at(i).at(j));
				card->setCardPos(pos);
			}
		}
	}

	void GameManager::touziAction(const sitDir& startDir, const int& num1, const int& num2)
	{
		auto startPos = COCOS_NODE(Text, StringUtils::format("touziStart%d", startDir))->getPosition();
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
		assert(num1 > 0 && num1 < 7);
		assert(num2 > 0 && num2 < 7);

		playCommonSound("Sezi");

		auto touziA = Sprite::createWithSpriteFrameName(StringUtils::format("touzi%d.png", num1).c_str());
		auto touziB = Sprite::createWithSpriteFrameName(StringUtils::format("touzi%d.png", num2));
		this->addChild(touziA);
		this->addChild(touziB);

		touziA->setVisible(true);
		touziB->setVisible(true);
		touziA->setGlobalZOrder(9999);
		touziB->setGlobalZOrder(9999);
		touziA->setScale(0.5f);
		touziB->setScale(0.5f);
		touziA->setPosition(startPos + _newOrigin);
		touziB->setPosition(startPos + _newOrigin);

		auto animationA = getAni("touziAction");
		animationA->setDelayPerUnit(0.01f);
		animationA->setLoops(25);
		animationA->setRestoreOriginalFrame(true);
		auto animationB = getAni("touziAction");
		animationB->setDelayPerUnit(0.01f);
		animationB->setLoops(25);
		animationB->setRestoreOriginalFrame(true);
		auto animateA = Animate::create(animationA);
		auto animateB = Animate::create(animationB);

		touziA->runAction(Sequence::create(
			Spawn::create(
			EaseSineOut::create(MoveTo::create(1.0f, touzi1EndPos + _newOrigin)),
			animateA,
			nullptr),
			DelayTime::create(0.6f),
			CallFunc::create(CC_CALLBACK_0(Node::removeFromParent, touziA)),
			nullptr));

		touziB->runAction(Sequence::create(
			Spawn::create(
			EaseSineOut::create(MoveTo::create(1.0f, touzi2EndPos + _newOrigin)),
			animateB,
			nullptr),
			DelayTime::create(0.6f),
			CallFunc::create(CC_CALLBACK_0(Node::removeFromParent, touziB)),
			nullptr));

	}

	Animation* GameManager::getAni(std::string name)
	{
		auto cache = AnimationCache::getInstance();
		Animation* animation = nullptr;
		Vector<SpriteFrame *> spFrame;
		animation = cache->getAnimation(name);
		if (animation == nullptr)
		{
			for (auto i = 1; i < 50; i++)
			{
				auto fullName = StringUtils::format("%s%d.png", name.c_str(), i);
				auto spf = SpriteFrameCache::getInstance()->getSpriteFrameByName(fullName.c_str());
				if (spf != nullptr)
				{
					spFrame.pushBack(spf);
				}
				else
				{
					break;
				}
			}
			animation = Animation::createWithSpriteFrames(spFrame);
			animation->setDelayPerUnit(0.05f);
			animation->setLoops(1);
			cache->addAnimation(animation, name);
		}
		return animation;
	}

	void GameManager::setNt(const sitDir& dir)
	{
		_currOperDir = _startSendDir = _startDir = dir;
		for (auto i = 1; i <= PLAY_COUNT; i++)
		{
			COCOS_NODE(Sprite, StringUtils::format("zhuang%d", i))->setVisible(false);
		}
		COCOS_NODE(Sprite, StringUtils::format("zhuang%d", _startDir))->setVisible(true);

		for (auto i = 1; i <= PLAY_COUNT; i++)
		{
			COCOS_NODE(Sprite, StringUtils::format("que_tongSP%d", i))->setVisible(false);
			COCOS_NODE(Sprite, StringUtils::format("que_wanSP%d", i))->setVisible(false);
			COCOS_NODE(Sprite, StringUtils::format("que_tiaoSP%d", i))->setVisible(false);
			COCOS_NODE(Sprite, StringUtils::format("tuoguang%d", i))->setVisible(false);
			COCOS_NODE(Sprite, StringUtils::format("ready%d", i))->setVisible(false);
		}
		COCOS_NODE(Sprite, "waiting")->setVisible(false);
	}

	void GameManager::setCatchDir(const sitDir& dir)
	{
		_startCatchDir = dir;
	}

	void GameManager::setCatchPos(const INT& sendPos)
	{
		_sendPos = sendPos;
	}

	void GameManager::finishGame(const std::vector<std::vector<int>>& vvNum)
	{
		_vecCardPool.at(0)->setCanOper(false);  // 不可操作
		endAllTimer();

		// 隐藏托管
		for (auto i = 1; i <= PLAY_COUNT; i++)
		{
			COCOS_NODE(Sprite, StringUtils::format("tuoguang%d", i))->setVisible(false);
		}


		// 隐藏手牌
		for (auto i = 0; i < PLAY_COUNT; i++)
		{
			auto listC = _vecCardPool.at(i)->getListCard();
			for (auto &card : listC)
			{
				card->setCardVisible(false);
			}
		}

		// 清空胡消息牌容器
		for (auto &v : _vecTmpHuCard)
		{
			v->getCardSprite()->removeFromParent();
			v->removeFromParent();
		}
		_vecTmpHuCard.clear();

		for (auto v : _vecHuCardShow)
		{
			v->setCardVisible(false);
		}
		_vecHuCardShow.clear();

		for (auto &vv : _vvecHuCard)
		{
			for (auto &v : vv)
			{
				v->setCardVisible(false);
			}
		}


		//　展示手牌
		for (auto i = 0; i < PLAY_COUNT; i++)
		{
			auto listC = _vecCardPool.at(i)->getListCard();
			if (listC.size() == 0)
			{
				return;
			}
			auto iter = listC.begin();
			//auto beginCard = *listC.begin();
			float interval = _vecCardPool.at(i)->getActionCardInterval();
			std::vector<Card *> vTmp;
			for (size_t j = 0; j < listC.size(); j++)
			{
				Card* card = nullptr;
				if (i == 0)
				{
					card = getZhengDaoCard(vvNum.at(i).at(j));
				}
				else
				{
					card = getXiaoDaoCard(sitDir(i+1), vvNum.at(i).at(j));
				}
				//auto pos = beginCard->getCardPos();
				auto pos = _vecCardPool.at(i)->getCurrHandCardStartSortPos();
				if (j != 0)
				{
					if (i == 0)
					{
						pos = Vec2(pos.x + float(j)*interval, pos.y);
					}
					else if (i == 1)
					{
						pos = Vec2(pos.x, pos.y-float(j)*interval);
					}
					else if (i == 2)
					{
						pos = Vec2(pos.x-float(j)*interval, pos.y);
					}
					else
					{
						pos = Vec2(pos.x, pos.y+float(j)*interval);
					}
				}
				card->setCardPos(pos);
				log("sitdir = %d, cardSize = %d, pos.x = %f, pos.y = %f", i, listC.size(), pos.x, pos.y);
				vTmp.push_back(card);
				++iter;
			}
			_vvLastShow.push_back(vTmp);
		}
	}

	void GameManager::restartGame()
	{
		setPlayGame(true);
		for (auto &v : _vecCardPool)
		{
			v->finishGame();
		}

		// 隐藏胡牌展示区
		for (auto &v : _vecHuFinishUiCard)
		{
			v->getCardSprite()->removeFromParent();
			v->removeFromParent();
		}
		_vecHuFinishUiCard.clear();

		// 清空显示所有牌容器
		for (auto &vv : _vvLastShow)
		{
			for (auto &v : vv)
			{
				v->setCardVisible(false);
			}
		}
		_vvLastShow.clear();

		///////////// 重置数据值
		// 设置容器
		for (auto &v : _vecBottomCard)
		{
			v->setCardVisible(true);
		}
		for (auto &vvv : _vvvZhengLiCard)
		{
			for (auto &vv : vvv)
			{
				for (auto &v : vv)
				{
					v->setCardVisible(false);
					v->setCardColor(Color3B(255,255,255));
					v->setSelect(false);
				}
			}
		}
		for (auto &vvv : _vvvZhengDaoCard)
		{
			for (auto &vv : vvv)
			{
				for (auto &v : vv)
				{
					v->setCardVisible(false);
				}
			}
		}
		for (auto &v : _vecBeiLiCard)
		{
			v->setCardVisible(false);
		}
		for (auto &v : _vecZuoLiCard)
		{
			v->setCardVisible(false);
		}
		for (auto &v : _vecYouLiCard)
		{
			v->setCardVisible(false);
		}
		for (auto &vvv : _vvvvXiaoDaoCard)
		{
			for (auto &vv : vvv)
			{
				for (auto &v : vv)
				{
					for (auto &var : v)
					{
						var->setCardVisible(false);
					}
				}
			}
		}
		

		// 重置花牌
		for (auto vv : _vvColorZhengDa)
		{
			for (auto v : vv)
			{
				v->setCardVisible(false);
				v->setCardColor(Color3B(255,255,255));
				v->setSelect(false);
			}
		}
		for (auto vv : _vvColorZhengDao)
		{
			for (auto v : vv)
			{
				v->setCardVisible(false);
			}
		}
		for (auto vvv : _vvvColorXiaoDao)
		{
			for (auto vv : vvv)
			{
				for (auto v : vv)
				{
					v->setCardVisible(false);
				}
			}
		}

		restartSetData();
	}

	void GameManager::initAllHandCard(std::vector<std::vector<INT>> vvSouthCard)
	{
		// vv.at 0 表示 south . 1 表示west. 2 nortH  3.east
		_vvStartHandCard = vvSouthCard;
		// 起牌跺数
		_vecCurrSendIndex.assign(4, 0);
		// 开始发牌
		sendCard(_startCatchDir, _sendPos);

		_zorderWest = _zorderEast = _zorderSouth = _zorderNorth = 200;
	}

	Card* GameManager::commonCatchCard(const sitDir& dir, const INT& mahjongNumber)
	{
		if (_lastCardIndex % 2 == 0)
		{
			if (_currCardIndex % CARD_TOTAL_NUMBER == (_lastCardIndex+2)%CARD_TOTAL_NUMBER)
			{
				return nullptr;
			}
		}
		else
		{
			++_currCardIndex;
			if (_currCardIndex % CARD_TOTAL_NUMBER == (_lastCardIndex+1)%CARD_TOTAL_NUMBER)
			{
				return nullptr;
			}
		}

		// 插入手牌
		Card* giveCard = getCard(dir, mahjongNumber);

		// 取底牌
		if (!_vecBottomCard.at(_currCardIndex)->getCardSprite()->isVisible())  
		{
			_currCardIndex = _lastCardIndex;
		}

		auto card = _vecBottomCard.at(_currCardIndex)->getCardSprite();
		auto posStart = card->getPosition();

		card->runAction(
			Sequence::create(
			EaseSineOut::create(MoveTo::create(0.5f, _vecCatchEndPos.at(INT(dir)-INT(sitDir::SOUTH_DIR))))
			,CallFunc::create(CC_CALLBACK_0(Node::setVisible, card, false))
			,CallFunc::create(CC_CALLBACK_0(Card::setCardPos, _vecBottomCard.at(_currCardIndex), posStart))
			,CallFunc::create(CC_CALLBACK_0(CardPool::addCard, _vecCardPool.at(INT(dir)-INT(sitDir::SOUTH_DIR)), giveCard))
			,CallFunc::create(CC_CALLBACK_0(GameManager::checkHandCardData, this, dir))
			//,CallFunc::create(CC_CALLBACK_0(CardPool::afterCatchCheckSomething, _vecCardPool.at(INT(dir)-INT(sitDir::SOUTH_DIR)), giveCard))
			,nullptr
			));
		_currCardIndex = ++_currCardIndex % CARD_TOTAL_NUMBER;
		return giveCard;
	}

	Card* GameManager::tailCatchCard(const sitDir& dir, const INT& mahjongNumber)
	{
		if (_lastCardIndex % 2 == 0)
		{
			if (_currCardIndex % CARD_TOTAL_NUMBER == (_lastCardIndex+2)%CARD_TOTAL_NUMBER)
			{
				return nullptr;
			}
		}
		else
		{
			if (_currCardIndex % CARD_TOTAL_NUMBER == (_lastCardIndex+1)%CARD_TOTAL_NUMBER)
			{
				return nullptr;
			}
		}
		Card* giveCard = getCard(dir, mahjongNumber);
		auto card = _vecBottomCard.at(_lastCardIndex)->getCardSprite();
		auto posStart = card->getPosition();
		card->runAction(
			Sequence::create(
			EaseSineOut::create(MoveTo::create(0.5f, _vecCatchEndPos.at(INT(dir)-1)))
			,CallFunc::create(CC_CALLBACK_0(Node::setVisible, card, false))
			,CallFunc::create(CC_CALLBACK_0(Card::setCardPos, _vecBottomCard.at(_lastCardIndex), posStart))
			,CallFunc::create(CC_CALLBACK_0(CardPool::addCard, _vecCardPool.at(INT(dir)-1), giveCard))
			//,CallFunc::create(CC_CALLBACK_0(CardPool::afterCatchCheckSomething, _vecCardPool.at(INT(dir)-INT(sitDir::SOUTH_DIR)), giveCard))
			,nullptr
			));
		if (_lastCardIndex % 2 == 0)
		{
			_lastCardIndex = ++_lastCardIndex % CARD_TOTAL_NUMBER;
		}
		else
		{
			_lastCardIndex = (_lastCardIndex - 3 + CARD_TOTAL_NUMBER) % CARD_TOTAL_NUMBER;
		}
		return giveCard;
	}

	void GameManager::addCardPool(CardPool * pool)
	{
		_vecCardPool.push_back(pool);
		pool->setCatchPos(_vecCatchEndPos.at(_vecCardPool.size()-1));
	}

	std::vector<CardPool *>& GameManager::getUserCardPool()
	{ 
		return _vecCardPool; 
	}

	
	void GameManager::hasHu(const sitDir& dir, bool isZiMo, bool isQiangGang)
	{
		int index = INT(dir)-INT(sitDir::SOUTH_DIR);
		// 设置胡牌后的手牌状态
		auto &listCard = _vecCardPool.at(index)->getListCard();
		//auto &huList = _vecCardPool.at(index)->getHuListCard();
		int count = 0;
		Vec2 huCardShowPos = Vec2::ZERO;
		for (auto &v: listCard)       // 设置手牌不可见
		{
			v->setCardVisible(false);
			if (count == listCard.size()-1)
			{
				break;
			}
			
			_vvecHuCard.at(index).at(count)->setCardVisible(true);
			auto pos = (*listCard.begin())->getCardPos();
			auto interval = _vecCardPool.at(index)->getActionCardInterval();
			if (count != 0)
			{
				switch (dir)
				{
				case ZYHZ::SOUTH_DIR:
					pos += Vec2(interval * count, 0);
					break;
				case ZYHZ::EAST_DIR:
					pos += Vec2(0, interval * count);
					break;
				case ZYHZ::NORTH_DIR:
					pos += Vec2(-interval * count, 0);
					break;
				case ZYHZ::WEST_DIR:
					pos += Vec2(0, -interval * count);
					break;
				}
			}

			if (count == listCard.size()-2)
			{
				huCardShowPos = pos;
				switch (dir)
				{
				case ZYHZ::SOUTH_DIR:
					huCardShowPos += Vec2(interval + 5.0f, 0);
					break;
				case ZYHZ::EAST_DIR:
					huCardShowPos += Vec2(0, interval + 5.0f);
					break;
				case ZYHZ::NORTH_DIR:
					huCardShowPos += Vec2(-interval - 5.0f, 0);
					break;
				case ZYHZ::WEST_DIR:
					huCardShowPos += Vec2(0, -interval*1.3 - 5.0f);
					break;
				}
			}

			_vvecHuCard.at(index).at(count)->setCardPos(pos);
			//huList.push_back(_vvecHuCard.at(index).at(count));
			++count;
		}

		{
			auto number = _vecCardPool.at(index)->getLastActionCardNumber();
			number = getCurrOperNumber();
			Card* card = nullptr;
			if (dir == sitDir::SOUTH_DIR)
			{
				card = MahjongCard::create(mahjongCreateType::ZHENG_DAO, sitDir::SOUTH_DIR, number);
			}
			else if (dir == sitDir::EAST_DIR)
			{
				card = MahjongCard::create(mahjongCreateType::DI_ZHENG_EAST, sitDir::EAST_DIR, number);
			}
			else if (dir == sitDir::WEST_DIR)
			{
				card = MahjongCard::create(mahjongCreateType::DI_ZHENG_WEST, sitDir::WEST_DIR, number);
			}
			else if (dir == sitDir::NORTH_DIR)
			{
				card = MahjongCard::create(mahjongCreateType::DI_ZHENG_NORTH, sitDir::NORTH_DIR, number);
			}
			card->setCardVisible(true);
			card->setCardPos(huCardShowPos);
			this->addChild(card);
			_vecTmpHuCard.push_back(card);
		}

	}

	void GameManager::setLastActionCard(const sitDir& dir, const INT& number)
	{
		auto index = INT(dir) - INT(sitDir::SOUTH_DIR);
		_vecCardPool.at(index)->setLastActionCardNumber(number);
	}

	void GameManager::setHasHu(const sitDir& dir, const bool& bHasHu)
	{
		auto index = INT(dir) - INT(sitDir::SOUTH_DIR);
		_vecCardPool.at(index)->setHasHu(bHasHu);
	}

	void GameManager::showHuTmpCard(const sitDir& huDir, std::vector<int> vecHuHandCard)
	{
		auto dir = huDir;

		int index = INT(dir)-INT(sitDir::SOUTH_DIR);
		// 设置胡牌后的手牌状态
		auto &listCard = _vecCardPool.at(index)->getListCard();
		int count = 0;
		Vec2 huCardShowPos = Vec2::ZERO;
		for (auto &v : listCard)
		{
			v->setCardVisible(false);
		}

		for (auto &v: listCard)       // 设置手牌不可见
		{
			Card* showCard = nullptr;
			auto pos = (*listCard.begin())->getCardPos();
			auto interval = _vecCardPool.at(index)->getActionCardInterval();

			switch (dir)
			{
			case ZYHZ::SOUTH_DIR:
				showCard = getZhengDaoCard(vecHuHandCard.at(count));
				pos += Vec2(interval * count, 0);
				break;
			case ZYHZ::EAST_DIR:
				showCard = getXiaoDaoCard(sitDir::EAST_DIR, vecHuHandCard.at(count));
				pos += Vec2(0, interval * count);
				break;
			case ZYHZ::NORTH_DIR:
				showCard = getXiaoDaoCard(sitDir::NORTH_DIR, vecHuHandCard.at(count));
				pos += Vec2(-interval * count, 0);
				break;
			case ZYHZ::WEST_DIR:
				showCard = getXiaoDaoCard(sitDir::WEST_DIR, vecHuHandCard.at(count));
				pos += Vec2(0, -interval * count);
				break;
			}

			if (count == listCard.size()-2)
			{
				huCardShowPos = pos;
				switch (dir)
				{
				case ZYHZ::SOUTH_DIR:
					huCardShowPos += Vec2(interval + 5.0f, 0);
					break;
				case ZYHZ::EAST_DIR:
					huCardShowPos += Vec2(0, interval + 5.0f);
					break;
				case ZYHZ::NORTH_DIR:
					huCardShowPos += Vec2(-interval - 5.0f, 0);
					break;
				case ZYHZ::WEST_DIR:
					huCardShowPos += Vec2(0, -interval*1.3 - 5.0f);
					break;
				}
			}

			showCard->setCardPos(pos);
			_vecHuCardShow.push_back(showCard);
			++count;
		}
	}

	void GameManager::showShowCardToFinishUI(const int& huCount, std::vector<std::vector<int>> vvecCardNum)
	{
		_vecHuFinishUiCard;
		auto picNum = huCount - 1;
		auto picAddStr = StringUtils::format("%s", picNum <= 0 ? "" : StringUtils::format("%d", picNum).c_str()).c_str();
		Vec2 startAddPos;

		for (auto i = 1; i <= huCount; i++)
		{
			if (huCount == 1)
			{
				startAddPos = COCOS_NODE(Text, StringUtils::format("beginAdd"))->getPosition();
			}
			else 
			{
				startAddPos = COCOS_NODE(Text, StringUtils::format("beginAdd%d_%d", i-1, i))->getPosition();
			}
			
			startAddPos += getNewOrigin();

			auto vecCard = vvecCardNum.at(i-1);
			
			int count = 0;
			for (auto cardNum : vecCard)
			{
				auto card = MahjongCard::create(mahjongCreateType::ZHENG_DAO, sitDir::SOUTH_DIR, cardNum);
				this->addChild(card);
				auto cardSp = card->getCardSprite();
				cardSp->removeFromParent();
				this->addChild(cardSp);
				cardSp->setAnchorPoint(Vec2(0.5f,0.5f));
				card->setCardZorder(topZorder+2);
				card->setCardScale(0.72);
				auto sz = card->getCardSize();

				if (count == 0)
				{
					startAddPos.x -= (vecCard.size()-1)*sz.width*0.5f;
				}

				card->setCardPos(startAddPos + Vec2(0.0, sz.height/2) + Vec2(count * sz.width, 0));
				_vecHuFinishUiCard.push_back(card);
				++count;
			}

		}
	
	}


	Card* GameManager::getCard(const sitDir& dir, const INT& mahjongNumber)
	{
		Card* giveCard = nullptr;
		auto pool = _vecCardPool.at(INT(dir)-INT(sitDir::SOUTH_DIR));
		switch (dir)
		{
		case sitDir::SOUTH_DIR:
			{
				giveCard = getZhengLiCard(mahjongNumber);
				giveCard->setCardOwner(sitDir::SOUTH_DIR);
				break;
			}
		case sitDir::WEST_DIR:
			{
				giveCard = getZuoLiCard();
				giveCard->setCardOwner(sitDir::WEST_DIR);
				break;
			}
		case sitDir::NORTH_DIR:
			{
				giveCard = getBeiLiCard();
				giveCard->setCardOwner(sitDir::NORTH_DIR);
				break;
			}
		case sitDir::EAST_DIR:
			{
				giveCard = getYouLiCard();
				giveCard->setCardOwner(sitDir::EAST_DIR);
				break;
			}
		default:
			break;
		}
		if (pool->getListCard().size() > 0)
		{
			if (dir != sitDir::EAST_DIR)
			{
				giveCard->setCardZorder((*(pool->getListCard().rbegin()))->getCardZorder() + 1);
			}
			else
			{
				giveCard->setCardZorder((*(pool->getListCard().rbegin()))->getCardZorder() - 1);
			}
		}
		else
		{
			giveCard->setCardZorder(500);
		}
		giveCard->setCardPos(Vec2(-1000, -1000));
		giveCard->setCardVisible(true);
		return giveCard;
	}

	Card* GameManager::getZhengDaoCard(const INT& mahjongNumber)
	{
		//' 1. 取花色
		INT color = mahjongNumber / 10;
		if (color == INT(mahjongColor::FENG))
		{
			if (mahjongNumber == CMjEnum::MJ_TYPE_ZHONG)
			{
				for (auto v : _vvColorZhengDao.at(0))
				{
					if (!v->isCardVisible())
					{
						v->setCardVisible(true);
						v->setCardScale(1.0f);
						v->setCardOwner(sitDir::SOUTH_DIR);
						v->setCardZorder(_bigShowZorder);
						return v;
					}
				}
			}
		}
		else
		{
			INT number = mahjongNumber % 10 - 1;
			for (auto v : _vvvZhengDaoCard.at(color).at(number))
			{
				if (!v->isCardVisible())
				{
					v->setCardVisible(true);
					v->setCardScale(1.0f);
					v->setCardOwner(sitDir::SOUTH_DIR);
					v->setCardZorder(_bigShowZorder);
					return v;
				}
			}
		}
		assert("Card" == "nullptr");
		return nullptr;
	}

	Card* GameManager::getXiaoDaoCard(const sitDir& dir, const INT& mahjongNumber)
	{
		// 1. 取坐位
		INT sitNumber = INT(INT(dir)-INT(sitDir::SOUTH_DIR));
		//' 1. 取花色
		INT color = mahjongNumber / 10;

		if (color == INT(mahjongColor::FENG))
		{
			if (mahjongNumber == CMjEnum::MJ_TYPE_ZHONG)
			{
				for (auto v : _vvvColorXiaoDao.at(0).at(sitNumber))
				{
					if (!v->isCardVisible())
					{
						v->setCardVisible(true);
						v->setCardScale(1.0f);
						if (dir == sitDir::WEST_DIR)
						{
							v->setCardZorder(++_zorderWest);
						}
						else if (dir == sitDir::SOUTH_DIR)
						{
							v->setCardZorder(--_zorderSouth);
						}
						else if (dir == sitDir::NORTH_DIR)
						{
							v->setCardZorder(++_zorderNorth);
						}
						else if (dir == sitDir::EAST_DIR)
						{
							v->setCardZorder(--_zorderEast);
						}
						_lastestOutCard = v;
						v->setCardOwner(dir);
						return v;
					}
				}
			}
		}
		else
		{
			INT number = mahjongNumber % 10 - 1;
			for (auto v : _vvvvXiaoDaoCard.at(sitNumber).at(color).at(number))
			{
				if (!v->isCardVisible())
				{
					v->setCardVisible(true);
					v->setCardScale(1.0f);
					if (dir == sitDir::WEST_DIR)
					{
						v->setCardZorder(++_zorderWest);
					}
					else if (dir == sitDir::SOUTH_DIR)
					{
						v->setCardZorder(--_zorderSouth);
					}
					else if (dir == sitDir::NORTH_DIR)
					{
						v->setCardZorder(++_zorderNorth);
					}
					else if (dir == sitDir::EAST_DIR)
					{
						v->setCardZorder(--_zorderEast);
					}
					_lastestOutCard = v;
					v->setCardOwner(dir);
					return v;
				}
			}
		}
		
		assert("Card" == "nullptr");
		return nullptr;
	}

	

	
	Card* GameManager::getZhengLiCard(const INT& mahjongNumber)
	{
		//' 1. 取花色
		INT color = mahjongNumber / 10;
		if (color == INT(mahjongColor::FENG))   // 风
		{
			if (mahjongNumber == CMjEnum::MJ_TYPE_ZHONG)
			{
				for (auto v : _vvColorZhengDa.at(0))
				{
					if (!v->isCardVisible())
					{
						v->setCardVisible(true);
						return v;
					}
				}
			}
		}
		else
		{
			INT number = mahjongNumber % 10 - 1;
			for (auto v : _vvvZhengLiCard.at(color).at(number))
			{
				if (!v->isCardVisible())
				{
					v->setCardVisible(true);
					return v;
				}
			}
		}
		
		assert("Card" == "nullptr");
		return nullptr;
	}

	Card* GameManager::getBeiLiCard()
	{
		for (auto v : _vecBeiLiCard)
		{
			if (!v->isCardVisible())
			{
				v->setCardVisible(true);
				return v;
			}
		}
		assert("Card" == "nullptr");
		return nullptr;
	}
	
	Card* GameManager::getZuoLiCard()
	{
		for (auto v : _vecZuoLiCard)
		{
			if (!v->isCardVisible())
			{
				v->setCardVisible(true);
				return v;
			}
		}
		assert("Card" == "nullptr");
		return nullptr;
	}

	Card* GameManager::getYouLiCard()
	{
		for (auto v : _vecYouLiCard)
		{
			if (!v->isCardVisible())
			{
				v->setCardVisible(true);
				return v;
			}
		}
		assert("Card" == "nullptr");
		return nullptr;
	}

	
	void GameManager::autoSendCard(const sitDir& dir)
	{
		if (dir != sitDir::SOUTH_DIR)
		{
			return;
		}
		auto card = _vecCardPool.at(0)->getAutoSendCard();
		_vecCardPool.at(0)->removeCardMsg(card);
	}

	Vec2 GameManager::getOutCardDeskPos(const sitDir& dir)
	{
		auto startPos = Vec2::ZERO;
		Size outRect;
		auto index = INT(dir) - INT(sitDir::SOUTH_DIR);
		auto count = _vecHasOutCardCount.at(index);
		++_vecHasOutCardCount.at(index);
		Vec2 returnPos = startPos;
		auto origin =  _newOrigin;
		auto panelSz= COCOS_NODE(Layout, "outCardPanel")->getContentSize();
		auto panelOrigin = COCOS_NODE(Layout, "outCardPanel")->getPosition() + origin;
		
		int lineCountArr[3] = {8, 5, 3};
		auto tmpCount = count;
		int iDepth = 0;
		for (auto i = 0; i < 3; i++)
		{
			if (tmpCount >= lineCountArr[i])
			{
				tmpCount -= lineCountArr[i];
				++iDepth;
			}
			else
			{
				break;
			}
		}

		switch (dir)
		{
		case sitDir::SOUTH_DIR:
			{
				outRect = _mahjongSzNS;
				startPos = panelOrigin  + Vec2(outRect.width/2 + panelSz.width / 6.0f, outRect.height/2);
				returnPos = startPos + Vec2((tmpCount+iDepth) * outRect.width, iDepth * outRect.height*0.75);

				break;
			}
		case sitDir::WEST_DIR:
			{
				outRect = _mahjongSzEW;
				startPos = panelOrigin+Vec2(outRect.width/2, panelSz.height * 5.0f/6.0f - outRect.height/2);
				returnPos = startPos + Vec2(iDepth * outRect.width, -(tmpCount+iDepth)*outRect.height*0.66);

				break;
			}
		case sitDir::NORTH_DIR:
			{
				outRect = _mahjongSzNS;
				startPos = panelOrigin + Vec2(panelSz.width * 5.0f/6.0f - outRect.width/2, panelSz.height-outRect.height/2);
				returnPos = startPos + Vec2(-(tmpCount+iDepth) * outRect.width, -iDepth * outRect.height*0.75);

				break;
			}
		case sitDir::EAST_DIR:
			{
				outRect = _mahjongSzEW;
				startPos = panelOrigin+Vec2(Vec2(panelSz.width-outRect.width/2, outRect.height/2 + panelSz.height / 6.0f));
				returnPos = startPos + Vec2(-iDepth * outRect.width, (tmpCount+iDepth)*outRect.height*0.66);

				break;
			}
		default:
			break;
		}
		return returnPos;
	}

	void GameManager::afterOutCard(const sitDir& dir)
	{
		auto poolIndex = INT(dir)-INT(sitDir::SOUTH_DIR);
		_vecCardPool.at(poolIndex)->removeCard();
		_vecCardPool.at(poolIndex)->setOutCardCount(_vecCardPool.at(poolIndex)->getOutCardCount()+1);
	}

	void GameManager::startOutCard(const sitDir& dir)
	{
		auto index = INT(dir)-INT(sitDir::SOUTH_DIR);
		auto pool = _vecCardPool.at(index);
		auto card = pool->getAutoSendCard();
		if (index == 0)  // south
		{
			_vecCardPool.at(0)->getListCard().remove(card);
			_vecCardPool.at(0)->getListCard().push_back(card);
			_vecCardPool.at(0)->setHandCardPos();
		}
		if (dir == sitDir::SOUTH_DIR)
		{
			_vecCardPool.at(0)->setCanOper(true);
		}
		// 重置抓牌位置
		resetCatchPos(true);

		card->setCardPos(_vecCatchEndPos.at(index));
	}

	void GameManager::setMinusOutCount(const sitDir& dir)
	{
		INT index = INT(dir) - INT(sitDir::SOUTH_DIR);
		_vecHasOutCardCount.at(index) = _vecHasOutCardCount.at(index) - 1;
	}

	Card*& GameManager::getLastestOutCard() 
	{ 
		return _lastestOutCard; 
	}

	bool GameManager::isNoCardCanCatch()
	{
		return (_currCardIndex % CARD_TOTAL_NUMBER == (_lastCardIndex+2)%CARD_TOTAL_NUMBER);
	}

	void GameManager::checkHandCardData(const sitDir& outDir)
	{
		_vecCardPool.at(0)->setCanOper(outDir == sitDir::SOUTH_DIR);
		auto & listCard = _vecCardPool.at(0)->getListCard();
		auto & numCard = _vvNum.at(0);
		auto hasHu = _vecCardPool.at(0)->getHasHu();
		if (hasHu)
		{
			return;
		}
		else
		{
			auto iter = listCard.begin();
			list<int> listTmpNum(numCard.begin(), numCard.end());
			for (auto & card : listCard)
			{
				auto cardSumNumber = card->getCardSumNumber();
				int removeCount = 1;
				listTmpNum.remove_if([&removeCount, cardSumNumber](int number)->bool{
					bool needRemove = (cardSumNumber == number);
					if (needRemove)
					{
						++removeCount;
						needRemove = needRemove && removeCount <= 2;
					}
					return needRemove;
				});
			}
			if ((listCard.size() != numCard.size()) || listTmpNum.size() != 0)
			{
				for (auto &v : listCard)
				{
					v->setCardVisible(false);
				}
				listCard.clear();
				for (auto &v : numCard)
				{
					auto newCard = getCard(sitDir::SOUTH_DIR, v);
					_vecCardPool.at(0)->addCard(newCard);
				}
				_vecCardPool.at(0)->sortCard();;
				_vecCardPool.at(0)->setCanOper(true);
				return;
			}
		}
	}

	void GameManager::setHandCardFormServer(const std::vector<std::vector<int>>& vvNum) 
	{ 
		_vvNum = vvNum; 
	}

	void GameManager::showNiao(const std::vector<INT>& vCardNumber)
	{
		endAllTimer();
		COCOS_NODE(Sprite, "niaoKuang")->setVisible(true);
		auto sz = COCOS_NODE(Sprite, "niaoKuang")->getContentSize();
		auto centerPos = COCOS_NODE(Sprite, "niaoKuang")->getPosition();
		centerPos -= sz/2;
		for (auto i = 0; i < vCardNumber.size(); i++)
		{
			auto pos = COCOS_NODE(Text, StringUtils::format("niao%d", i+1))->getPosition();
			auto number =  vCardNumber.at(i);
			if (number <= 0)
			{
				continue;
			}
			auto color = number / 10;
			Card* card = getZhengLiCard(number);
			card->setCardPos(pos + centerPos + Vec2(30, 30));
			card->setCardZorder(topZorder + 1);
			_niaoGlobalZorder = card->getCardSprite()->getGlobalZOrder();
			card->getCardSprite()->setGlobalZOrder(topZorder);
			_vecNiao.push_back(card);

			if (card->getCardNumber() == 2 || card->getCardNumber() == 3 || (card->getCardNumber() == CMjEnum::MJ_TYPE_ZHONG/*5 && card->getCardColor() != mahjongColor::FENG*/))
			{
				//粒子效果
				ParticleSystem *partic = ParticleSystemQuad::create("FreedomRedMahjong/particle/majiang.plist");
				partic->setPosition(pos);
				partic->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
				partic->setScale(0.8f);
				partic->setName(StringUtils::format("partic%d", i));
				COCOS_NODE(Sprite, "niaoKuang")->addChild(partic, topZorder - 1);
			}
		}
	}

	void GameManager::hideNiao()
	{
		COCOS_NODE(Sprite, "niaoKuang")->setVisible(false);
		for (int i = 0; i < 4; i++)
		{
			auto partic = COCOS_NODE(Sprite, "niaoKuang")->getChildByName(StringUtils::format("partic%d", i));
			if (partic)
			{
				partic->removeFromParent();
			}
		}

		for (auto &v : _vecNiao)
		{
			v->setCardVisible(false);
			v->getCardSprite()->setGlobalZOrder(_niaoGlobalZorder);
		}
		_vecNiao.clear();
	}
	
	void GameManager::sendCard(const sitDir& dir, const INT& heapCount)
	{
		switch (dir)
		{
		case sitDir::SOUTH_DIR:
			{
				_currCardIndex = heapCount * 2;
				break;
			}
		case sitDir::WEST_DIR:
			{
				_currCardIndex = northSouthCount  +  heapCount * 2;
				break;
			}
		case sitDir::NORTH_DIR:
			{
				_currCardIndex = (northSouthCount + westEastCount)  +  heapCount * 2;
				break;
			}
		case sitDir::EAST_DIR:
			{
				_currCardIndex = (2 * northSouthCount + westEastCount)  +  heapCount * 2;
				break;
			}
		default:
			assert("non sitDir" == "sendCard");
			break;
		}
		_lastCardIndex = (_currCardIndex - 2 + CARD_TOTAL_NUMBER) % CARD_TOTAL_NUMBER;
		_sendCardCount = 0;
		_hasSendAllCard = false;
		_bStartSendCard = true;
		log("_lastCardIndex = %d _currCardIndex = %d", _lastCardIndex, _currCardIndex);
		this->schedule(schedule_selector(GameManager::sendCardTimer), 0.16f);
	}

	



	void GameManager::sendCardToPool(const INT& cardCount)
	{
		playCommonSound("zhuapai");
		std::vector<Card *> vecTmpCard;     // 需要发的牌
		auto startIndex = _vecCurrSendIndex.at(INT(_startSendDir)-1);     // 需要发的牌起始位置
		auto endIndex = startIndex + cardCount - 1;                                 // 需要发的牌结束位置
		for (auto i = startIndex; i <= endIndex; i++)                                  // 填充需要发的牌
		{
			auto num = _vvStartHandCard.at(INT(_startSendDir)-1).at(i);
			Card * cardSend = getCard(_startSendDir, num);
			vecTmpCard.push_back(cardSend);   // 填充要发的牌
		}
		_vecCurrSendIndex.at(INT(_startSendDir)-1) = endIndex+1;   // 设置新的起始点

		// 往该方向发牌
		switch (_startSendDir)
		{
		case ZYHZ::sitDir::SOUTH_DIR:
			_vecCardPool.at(0)->sendSomeCard(vecTmpCard);
			break;
		case ZYHZ::sitDir::EAST_DIR:
			_vecCardPool.at(3)->sendSomeCard(vecTmpCard);
			break;
		case ZYHZ::sitDir::NORTH_DIR:
			_vecCardPool.at(2)->sendSomeCard(vecTmpCard);
			break;
		case ZYHZ::sitDir::WEST_DIR:
			_vecCardPool.at(1)->sendSomeCard(vecTmpCard);
			break;
		case ZYHZ::sitDir::MID_DIR:
			break;
		default:
			break;
		}
		_startSendDir = sitDir((INT(_startSendDir))%4+1);    // 往下一个方向发
	}

	void GameManager::sendCardTimer(float dt)
	{
		if (_sendCardCount < 12)  // 小于12次, 每次发4张
		{
			for (auto i = 0; i < 4; i++)   // 一次4张
			{
				_vecBottomCard.at(_currCardIndex)->setCardVisible(false);
				_currCardIndex = (++_currCardIndex) % CARD_TOTAL_NUMBER;
			}
			sendCardToPool(4);
		}
		else if (_sendCardCount == 12)  // 第十三次，发2张给庄家
		{
			_vecBottomCard.at(_currCardIndex)->setCardVisible(false);   
			_currCardIndex = (_currCardIndex+4) % CARD_TOTAL_NUMBER;
			_vecBottomCard.at(_currCardIndex)->setCardVisible(false);
			_currCardIndex = (_currCardIndex-3+CARD_TOTAL_NUMBER) % CARD_TOTAL_NUMBER;
			sendCardToPool(2);
		}
		else if (_sendCardCount < 16)   // 发一张
		{
			_vecBottomCard.at(_currCardIndex)->setCardVisible(false);
			_currCardIndex = (++_currCardIndex) % CARD_TOTAL_NUMBER;
			if (_sendCardCount == 15)
			{
				_currCardIndex = (++_currCardIndex) % CARD_TOTAL_NUMBER;    // 定位抓牌起始点
			}
			sendCardToPool(1);
			
		}
		else
		{
			this->unschedule(schedule_selector(GameManager::sendCardTimer));
			_sendCardCount = 0;
			_hasSendAllCard = true;
			return;
		}
		++_sendCardCount;
	}

	void GameManager::setQueMen(INT queMen) 
	{ 
		_queMen = queMen; 
	};

	INT& GameManager::getQueMen()
	{
		return _queMen;
	}

	void GameManager::setPlayGame(bool bs)
	{
		_isPlayGame = bs;
	}

	const bool& GameManager::isGamePlaying()
	{ 
		return _isPlayGame; 
	} 

	void GameManager::setKongType(INT type)
	{
		_kongType = type;
	}

	void GameManager::setConcealedNumber(INT number)
	{
		_concealedNumber = number;
	}

	tagHuPaiEx& GameManager::getHuMsg()
	{
		return _huMsg;
	}

	sitDir& GameManager::getStartDir()
	{
		return _startDir;
	}

	void GameManager::setSendStatus(bool hasSendCard, bool startSendCard)
	{
		_hasSendAllCard = hasSendCard;
		_bStartSendCard = startSendCard;
	}

	void GameManager::resetCatchPos(bool addApos)
	{
		for (auto i = 0; i < PLAY_COUNT; i++)
		{
			_vecCardPool.at(i)->setHandCardPos();
			auto list = _vecCardPool.at(i)->getListCard();
			auto point = list.rbegin();
			if (addApos)
			{
				if (list.size() == 14 || (_isSouthPlayOper && i == 0))
				{
					++point;
				}
			}
			auto basePos = (*point)->getCardPos();
			
			//auto sz = (*point)->getCardSize();
			auto interval = _vecCardPool.at(i)->getSortInterval();
			if (i == 0)
			{
				_vecCatchEndPos.at(i) = basePos + Vec2(interval * (1.2f), 0);
			}
			else if (i == 1)
			{
				_vecCatchEndPos.at(i) = basePos + Vec2(0, -interval * (1.4f));
			}
			else if (i == 2)
			{
				_vecCatchEndPos.at(i) = basePos + Vec2(-interval *(1.2f), 0);
			}
			else
			{
				_vecCatchEndPos.at(i) = basePos + Vec2(0, interval*(1.4f));
			}
			_vecCardPool.at(i)->setCatchPos(_vecCatchEndPos.at(i));
		}
	}

	void GameManager::setGlobalScale()
	{
		// 适配
		auto visibleSZ = Director::getInstance()->getVisibleSize();
		auto visibleOrigin = Director::getInstance()->getVisibleOrigin();

		// 背景层
		auto bg = Sprite::create(PIC_PATH + "bg.png");
		this->addChild(bg);
		bg->setAnchorPoint(Vec2::ZERO);
		bg->setPosition(visibleOrigin);
		bg->setLocalZOrder(-111);

		auto newOrigin = visibleOrigin;
		auto uiSZ = Size(1280, 720);
		float scale = 1.0f;

		if (visibleSZ.width >= uiSZ.width && visibleSZ.height >= uiSZ.height)		// 1 . 全包含
		{
			newOrigin += Vec2((visibleSZ.width - uiSZ.width)/2.0f, (visibleSZ.height - uiSZ.height)/2.0f);
		}
		else if (visibleSZ.width >= uiSZ.width && visibleSZ.height < uiSZ.height)
		{
			scale = (visibleSZ.height / uiSZ.height);
			newOrigin.x += (visibleSZ.width - uiSZ.width * scale)/2.0f;
		}
		else if (visibleSZ.width < uiSZ.width && visibleSZ.height >= uiSZ.height)
		{
			scale = (visibleSZ.width / uiSZ.width);
			newOrigin.y += (visibleSZ.height - uiSZ.height * scale) / 2.0f;
		}
		else if (visibleSZ.width < uiSZ.width && visibleSZ.height < uiSZ.height)
		{
			auto scaleA = visibleSZ.width/uiSZ.width;
			auto scaleB = visibleSZ.height/uiSZ.height;
			scale = scaleA < scaleB ? scaleA : scaleB;
			newOrigin.x += (visibleSZ.width - uiSZ.width * scale) / 2.0f;
			newOrigin.y += (visibleSZ.height - uiSZ.height * scale) / 2.0f;
		}
		_globalScale = scale;
		_newOrigin = newOrigin;
		
	}

	const float& GameManager::getGlobalScale()
	{
		return _globalScale;
	}
	
	const Vec2& GameManager::getNewOrigin()
	{
		return _newOrigin;
	}

	void GameManager::playSexNumberSound(INT speakStartion, INT cardNumber)
	{
		auto man = isMan(speakStartion);

		// head
		auto fullName = MUSIC_SEX_PATH;
		if (man)
		{
			fullName += "Man/";
		}
		else
		{
			fullName += "Wom/";
		}

		// value init
		int maxNum = 9;
		std::string fullFileName[40];
		// 万
		auto begin = CMjEnum::MJ_TYPE_W1 %10;
		for (auto i = begin; i < begin + 9; i++)
		{
			fullFileName[CMjEnum::MJ_TYPE_W1 + i - 1] = StringUtils::format("%dwan", i);
		}
		// 条
		begin = CMjEnum::MJ_TYPE_T1 %10;
		for (auto i = begin; i < begin + 9; i++)
		{
			fullFileName[CMjEnum::MJ_TYPE_T1 + i - 1] = StringUtils::format("%dtiao", i);
		}
		// 筒
		begin = CMjEnum::MJ_TYPE_B1 %10;
		for (auto i = begin; i < begin + 9; i++)
		{
			fullFileName[CMjEnum::MJ_TYPE_B1 + i - 1] = StringUtils::format("%dtong", i);
		}
		// 红中
		fullFileName[CMjEnum::MJ_TYPE_ZHONG] = "zhong";
		// tail
		fullName +=  fullFileName[cardNumber];
		fullName += ".mp3";
		HNAudioEngine::getInstance()->playEffect(fullName.c_str());
	}

	void GameManager::playSexActionSound(INT speakStartion, std::string actionName)
	{
		auto man = isMan(speakStartion);

		// head
		auto fullName = MUSIC_SEX_PATH;
		if (man)
		{
			fullName += "Man/";
		}
		else
		{
			fullName += "Wom/";
		}

		fullName += actionName;

		fullName += ".mp3";

		HNAudioEngine::getInstance()->playEffect(fullName.c_str());
	}
	
	void GameManager::playCommonSound(std::string soundName)
	{
		// head
		auto fullName = MUSIC_COMMON_PATH;
		fullName += soundName;
		fullName += ".mp3";
		HNAudioEngine::getInstance()->playEffect(fullName.c_str());
	}

	bool GameManager::isMan(int index)
	{
		return _vecUser.at(index)->getUserInfo().bBoy;
	}

	Card* & GameManager::getBigShowSp()
	{
		return _bigShowCard;
	}

	void GameManager::addBigShowSp(const sitDir& dir, const INT& number)
	{
		if (_bigShowCard != nullptr)
		{
			_bigShowCard->getCardSprite()->removeFromParent();
			_bigShowCard->removeFromParent();
		}
		_bigShowCard = MahjongCard::create(mahjongCreateType::ZHENG_LI, dir, number);
		_bigShowCard->setCardZorder(500);
		this->addChild(_bigShowCard);
		_bigShowCard->setCardPos(COCOS_NODE(Text, StringUtils::format("outBigShow%d", INT(dir)))->getPosition()+_newOrigin);
	}

	void GameManager::block(bool canPeng, bool canGang, bool canHu)
	{
		resetDiKuang();
		COCOS_NODE(Sprite, "dikuang")->setVisible(true);
		if (canPeng)
		{
			COCOS_NODE(Button, "peng")->setBright(true);
			COCOS_NODE(Button, "peng")->setEnabled(true);
		}
		if (canGang)
		{
			COCOS_NODE(Button, "gang")->setBright(true);
			COCOS_NODE(Button, "gang")->setEnabled(true);
		}
		if (canHu)
		{
			COCOS_NODE(Button, "hu")->setBright(true);
			COCOS_NODE(Button, "hu")->setEnabled(true);
		}
	}

	void GameManager::setCurrOperNumber(const INT& number)
	{
		_currOperNumber = number;
		//setLastActionCardNumber(number);
	}
	
	const INT& GameManager::getCurrOperNumber()
	{
		return _currOperNumber;
	}
	
	void GameManager::setCurrOperDir(const sitDir& dir)
	{
		_currOperDir = dir;
		_isSouthPlayOper = (dir == sitDir::SOUTH_DIR);
	}
	
	const sitDir& GameManager::getCurrOperDir()
	{
		return _currOperDir;
	}

	void GameManager::setCurrInfo(const sitDir& dir, const INT& number)
	{
		setCurrOperDir(dir);
		setCurrOperNumber(number);
		if (dir == sitDir::SOUTH_DIR)
		{
			dynamic_cast<SouthMahjongCardPool *>(_vecCardPool.at(0))->checkWillRemoveCard(number);
		}
	}
}