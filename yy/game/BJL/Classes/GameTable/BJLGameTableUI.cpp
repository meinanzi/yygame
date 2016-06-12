#include "BJLGameTableUI.h"
#include "HNUIExport.h"
#include "BJLGameTableLogic.h"
#include "HNLobby/FontConfig.h"
#include "BJLNotice.h"
#include "HNLobby/GamePlatform.h"

#define  ZORDER_DASHBOARD	50			//控制面板
#define  ZORDER_GAMEBG		20
#define  ZORDER_GAMETABLE	40
#define  ZORDER_MAX			200

namespace BJL
{
	static const char* BJL_GAME_MUSIC_LOSE			= "BJL/game/music/Lose.mp3";
	static const char* BJL_GAME_MUSIC_PLAYGAME		= "BJL/game/music/PlayGame.mp3";
	static const char* BJL_GAME_MUSIC_WIN			= "BJL/game/music/Win.mp3";
	static const char* BJL_GAME_MUSIC_BET			= "BJL/game/music/Bet.WAV";
	static const char* BJL_GAME_MUSIC_WARNING		= "BJL/game/music/Warning.wav";
	static const char* BJL_GAME_MUSIC_FAPAI			= "BJL/game/music/fapai.mp3";

	static const char* NO_BANKER		            = "无庄";


	GameTableUI::~GameTableUI()
	{
		_logic->stop();

		CC_SAFE_DELETE(_logic);
		CC_SAFE_DELETE(_resultCard);
		CC_SAFE_DELETE(_gameView);
		CC_SAFE_DELETE(_dealerList);
	}

	GameTableUI::GameTableUI() 
		: _gameResult(nullptr)
		, _currentDealerLimit(0)
		, _resultCard(nullptr)
	{
		
	}

	HNGameUIBase* GameTableUI::create(BYTE bDeskIndex, bool bAutoCreate)
	{
		GameTableUI* ptr = new GameTableUI();
		if (nullptr != ptr)
		{
			ptr->init(bDeskIndex,bAutoCreate);
			ptr->autorelease();
			return ptr;
		}
		CC_SAFE_DELETE(ptr);
		return nullptr;
	}

	bool GameTableUI::init(BYTE bDeskIndex, bool bAutoCreate)
	{
		if (!HNGameUIBase::init())
		{
			return false;
		}

		HNAudioEngine::getInstance()->playBackgroundMusic(BJL_GAME_MUSIC_PLAYGAME, true);
		
		loadRes();
		initUI();
		initSprite();
		initTheNewGameData();

		_logic = new GameTableLogic(this, bDeskIndex, bAutoCreate);
		_logic->sendGameInfo();

		return true;
	}

	bool GameTableUI::initUI()
	{
		auto cache = SpriteFrameCache::getInstance();
		cache->addSpriteFramesWithFile("BJL/game/table/jettonbtn.plist");
		cache->addSpriteFramesWithFile("BJL/game/table/cards.plist");
		cache->addSpriteFramesWithFile("BJL/game/table/cardtype.plist");
		
		auto winSize = Director::getInstance()->getWinSize();
	
		//游戏界面
		auto dashbroad_loader = CSLoader::createNode("Games/BJL/game/BJLStudio/dashbroad.csb");
		
		//一个控件
		auto dashbroad_layout = (Layout* ) dashbroad_loader->getChildByName("Panel_1");

		// 操作面板
		_gameView->dashbroad_layout = dashbroad_layout;
		dashbroad_layout->setPosition(Vec2(winSize.width / 2 , 0));
		this->addChild(dashbroad_loader,ZORDER_DASHBOARD);

		// 游戏背景
		auto gamebg_loader = CSLoader::createNode("Games/BJL/game/BJLStudio/gamebg.csb");
		auto gamebg_layout = (Layout* ) gamebg_loader->getChildByName("Panel_1");
		gamebg_layout->setAnchorPoint(Vec2(0.5f , 0.5f));
		gamebg_layout->setPosition(Vec2(winSize.width / 2 , winSize.height / 2));
		this->addChild(gamebg_loader,ZORDER_GAMEBG);
		auto visibleSize = Director::getInstance()->getVisibleSize();
		_fxScale = visibleSize.width / gamebg_layout->getContentSize().width;
		_fyScale = visibleSize.height/ gamebg_layout->getContentSize().height;
		gamebg_layout->setScaleX(_fxScale);
		gamebg_layout->setScaleY(_fyScale);

		//桌子
		auto gametable_loader = CSLoader::createNode("Games/BJL/game/BJLStudio/gametable.csb");
		auto gametable_layout = (Layout* ) gametable_loader->getChildByName("Panel_50");
		_gameView->gametable_layout = gametable_layout;
		gametable_layout->setAnchorPoint(Vec2(0.5f , 0.5f));
		gametable_layout->setPosition(Vec2(winSize.width / 2 , winSize.height / 2));
		this->addChild(gametable_loader,ZORDER_GAMETABLE);

		float fmaxScale = _fxScale < _fyScale ? _fxScale : _fyScale;
		gametable_layout->setScale(fmaxScale);

		_resultCard = new ResultCard(this);

		//////////////////////////////////////////////////////////////////////////
		//适配缩放
		auto dash_bg_imgview = (ImageView* ) dashbroad_layout->getChildByName("dashbroad_0");
		_gameView->dash_bg_imgview =  dash_bg_imgview;
		if (_fyScale < 1 || _fxScale < 1)
		{
			dashbroad_layout->setScaleX(_fxScale);
			dashbroad_layout->setScaleY(_fyScale);
		}
		else
		{
			dashbroad_layout->setScaleX(_fxScale > _fyScale ? _fxScale: _fyScale);
		}

		_gameView->ListView_NTList = (ListView* ) gamebg_layout->getChildByName("ListView_dealerList");
		_gameView->ImageView_ListItem = (ImageView* ) _gameView->ListView_NTList->getChildByName("Image_item");
		_gameView->ImageView_ListItem->setLocalZOrder(100);


		//初始化上庄列表控制器
		_dealerList->initDealerList(_gameView->ListView_NTList, _gameView->ImageView_ListItem);
	
		char str[50];
		for (BYTE i = 0 ; i < 8 ; i++ )
		{
			sprintf(str, "area__%d", i);
			_gameView->Button_BetArea[i] = (Button*) gametable_layout->getChildByName(str);
			_gameView->Button_BetArea[i]->addTouchEventListener(CC_CALLBACK_2(GameTableUI::onBetAreaClickCallback,this));
			
			sprintf(str, "betbg__%d", i);
			_gameView->ImageView_BetWarning[i] = (ImageView* ) gametable_layout->getChildByName(str);
			
			sprintf(str, "winbg__%d", i);
			_gameView->ImageView_BetWin[i] = (ImageView* ) gametable_layout->getChildByName(str);
			
			sprintf(str, "allmoney__%d", i);
			_gameView->TextAtlas_TotalBet[i] = (TextAtlas* ) gametable_layout->getChildByName(str);
			_gameView->TextAtlas_TotalBet[i]->setString("0");
			
			sprintf(str, "mymoney___%d", i);
			_gameView->TextAtlas_UserBet[i] = (TextAtlas* ) gametable_layout->getChildByName(str);
			_gameView->TextAtlas_UserBet[i]->setString("0");

			//初始化下注区域
			_gameView->pnoteview[i].init(_gameView->Button_BetArea[i],_gameView->TextAtlas_TotalBet[i],_gameView->TextAtlas_UserBet[i]);
		}
		for (BYTE i = 0 ; i < 7 ; i++ )
		{
			sprintf(str, "jettonbtn__%d", i);
			_gameView->Button_Bet[i] = (Button* ) dashbroad_layout->getChildByName(str);
			_gameView->Button_Bet[i]->addTouchEventListener(CC_CALLBACK_2(GameTableUI::onBetClickCallback,this));
			
			sprintf(str, "jettonbg__%d", i);
			_gameView->ImageView_BetBg[i] = (ImageView* ) dashbroad_layout->getChildByName(str);
		}

		//下拉框
		auto xialakuangbg = (ImageView* ) gamebg_layout->getChildByName("topbg_0");  

		//下庄按钮
		_gameView->Button_applyNoNT = (Button* ) xialakuangbg->getChildByName("quitdealer");
		_gameView->Button_applyNoNT->addTouchEventListener(CC_CALLBACK_2(GameTableUI::onApplyNTClickCallback,this));
		_gameView->Button_applyNoNT->setVisible(false);

		//上庄按钮
		_gameView->Button_applyNT   = (Button* ) xialakuangbg->getChildByName("getdealer");		 
		_gameView->Button_applyNT->addTouchEventListener(CC_CALLBACK_2(GameTableUI::onApplyNTClickCallback,this));
		//_gameView->Button_applyNT->setVisible(false);

		_gameView->Button_Exit = (Button* ) xialakuangbg->getChildByName("exit");  
		_gameView->Button_Exit->addTouchEventListener(CC_CALLBACK_2(GameTableUI::onCommonClickCallback,this));
		_gameView->Button_Records = (Button* ) dashbroad_layout->getChildByName("luzi");
		_gameView->Button_Records->addTouchEventListener(CC_CALLBACK_2(GameTableUI::onCommonClickCallback,this));

		//庄家 和玩家信息初始化
		_gameView->ImageView_NTContainer = (ImageView* ) gamebg_layout->getChildByName("dealerbg_0");
		_gameView->ImageView_NTNickName  = (ImageView* ) _gameView->ImageView_NTContainer->getChildByName("dealer_name");
		_gameView->ImageView_NTMoney     = (ImageView* ) _gameView->ImageView_NTContainer->getChildByName("dealer_money");

		//状态提示图片
		_gameView->ImageView_betHint  = (ImageView* ) gametable_layout->getChildByName("current_state__0");
		_gameView->ImageView_WaitHint = (ImageView* ) gametable_layout->getChildByName("current_state__1");
		_gameView->ImageView_betHint->setVisible(false);		
		_gameView->ImageView_WaitHint->setVisible(false);

		//计时器
		_gameView->ImageView_TimerBg   = (ImageView* ) gametable_layout->getChildByName("timerbg_0");
		_gameView->TextAtlas_TimerText =  (TextAtlas*) _gameView->ImageView_TimerBg->getChildByName("timer");


		return true;
	}

	void GameTableUI::luziscrollview_scroll(cocos2d::Ref* pobj, ui::ScrollView::EventType type)
	{
	}

	void GameTableUI::onBetAreaClickCallback(cocos2d::Ref* pSender,Widget::TouchEventType touchtype)
	{
		if (touchtype != Widget::TouchEventType::ENDED)
		{
			return ;
		}

		if(_betIndex < 0 || _betIndex > 6)
		{
			IShowNoticeMessage(GBKToUtf8("请选择下注筹码"));
			return;
		}

		for (int i = 0; i < 8; i++)
		{
			if (pSender == _gameView->Button_BetArea[i])
			{
				// 请求下注
				_logic->requestBet(_betIndex, i);
				break;				
			}
		}
	}

	void GameTableUI::onBetClickCallback(cocos2d::Ref* pSender,Widget::TouchEventType touchtype)
	{
		if (touchtype!=Widget::TouchEventType::ENDED)
		{
			return;
		}

		if (_betIndex >= 0 && _betIndex < 7)
		{
			_gameView->ImageView_BetBg[_betIndex]->stopAllActions();
			_gameView->ImageView_BetBg[_betIndex]->setVisible(true);
		}
		
		for (int i = 0; i < CHOUMANUM; i++)
		{
			if (_gameView->Button_Bet[i] == pSender)
			{
				_betIndex = i;
				break;
			}
		}

		_gameView->ImageView_BetBg[_betIndex]->setVisible(true);
		_gameView->ImageView_BetBg[_betIndex]->runAction(RepeatForever::create(Blink::create(0.5f, 1)));
	}

	void GameTableUI::onCommonClickCallback(cocos2d::Ref* pSender,Widget::TouchEventType touchtype)
	{
		if (touchtype != Widget::TouchEventType::ENDED)
		{
			return ;
		}

		if (pSender == _gameView->Button_Exit)			//退出按钮
		{
			if (GS_NOTE_STATE == _logic->getGameStatus() || GS_SEND_CARD == _logic->getGameStatus())
			{
				IShowNoticeMessage(GBKToUtf8("正在游戏中，禁止退出。"));
				return;
			}

			_logic->requestStandUp();
		}
		else if (pSender == _gameView->Button_Records)		//路子按钮  
		{
			auto dialog = GameHistoryDialog::create(this, _logic->getRecords());
			this->reorderChild(dialog,ZORDER_MAX);
			dialog->show();
		}
	}


	void GameTableUI::onApplyNTClickCallback(cocos2d::Ref* pSender,Widget::TouchEventType touchtype)
	{
		if (touchtype != Widget::TouchEventType::ENDED)
		{
			return;
		}		

		// 申请上庄
		if (pSender == _gameView->Button_applyNT)		
		{
			_logic->requestBeNT(true);
		}

		// 申请下庄
		else if(pSender == _gameView->Button_applyNoNT)	
		{
			_logic->requestBeNT(false);
		}
	}

	void GameTableUI::stopBetAnimation()
	{		 
		_betIndex = -1;
		for (BYTE i = 0; i < CHOUMANUM; i++)
		{
			_gameView->ImageView_BetBg[i]->stopAllActions();
			_gameView->ImageView_BetBg[i]->setVisible(false);
		}
	}


	void GameTableUI::leaveTheGame()
	{
		//退出当前房间	
		RoomLogic()->close();
		GamePlatform::returnPlatform(ROOMLIST);
	}

	void GameTableUI::betAnimationCallback(cocos2d::Node* pSender)
	{
		void* pData = pSender->getUserData();
		BET_TYPE* data = (BET_TYPE*)pData;
		auto pos = pSender->getPosition();
		if (data->whichJetton < 7  && data->whichJetton >= 0)
		{
			CurJtInfo cji;
			int jts[] = { 100, 1000, 10000, 100000, 1000000, 5000000, 10000000 };
			cji.val=jts[data->whichJetton];
			cji.xpos=data->whichJetton;			
			_gameView->pnoteview[data->whichType].dropone(cji,0);
		}
		CC_SAFE_DELETE(data);
		return ;
	}

	void GameTableUI::initSprite()
	{
		//庄家信息
		_dealerName = Label::createWithSystemFont(GBKToUtf8("无庄"), "", 18);
		_dealerName->setColor(Color3B::RED);
		_gameView->ImageView_NTContainer->addChild(_dealerName);
		_dealerName->setAnchorPoint(Vec2(0,0.5f));
		_dealerName->setPosition(_gameView->ImageView_NTNickName->getPosition());

		_dealerMoney = Label::createWithSystemFont("", "", 18);
		_dealerMoney->setColor(Color3B::RED);
		_gameView->ImageView_NTContainer->addChild(_dealerMoney);
		_dealerMoney->setAnchorPoint(Vec2(0,0.5f));
		_dealerMoney->setPosition(_gameView->ImageView_NTMoney->getPosition());

		//玩家信息
		std::string nickName = PlatformLogic()->loginResult.nickName;
		_ownName = (Text* ) _gameView->dashbroad_layout ->getChildByName("Text_name");
		_ownName->setLocalZOrder(1000);
		_ownName->setString(GBKToUtf8(nickName.c_str()));

		_ownMoney = (Text* ) _gameView->dashbroad_layout ->getChildByName("Text_money");
		_ownMoney->setLocalZOrder(1000);

	}

	void GameTableUI::loadRes()
	{
		_gameView=new GameView();
		_dealerList = new T_DEALER_LIST();
		_betIndex = -1;
	}

	void GameTableUI::initTheNewGameData()
	{
		_resultCard->hideCardBorad();
		for (BYTE i = 0; i < NUMOFBETAREA ; ++i)
		{
			_gameView->TextAtlas_TotalBet[i]->setString("0");
			_gameView->TextAtlas_TotalBet[i]->setVisible(false);   //下注的金币数
			_gameView->TextAtlas_UserBet[i]->setString("0");
			_gameView->TextAtlas_UserBet[i]->setVisible(false);   //自己下注的金币数

			_gameView->ImageView_BetWarning[i]->setVisible(false);   //赢牌区域动画
			_gameView->ImageView_BetWin[i]->setVisible(false);   //赢牌区域动画

			_gameView->pnoteview[i].hide();
		}
		memset(_currentMyZhu , 0 ,sizeof(_currentMyZhu));
		_betIndex = -1;
		_resultCard->initNewGame();
		
		// 中间区域
		IShowWinArea(false);

		//清空下注筹码回收池
		while (!spriteArr.empty())
		{
			spriteArr.back()->removeFromParent();
			spriteArr.popBack();
		}
	}


	//开始游戏
	void GameTableUI::OnHandleBegin(S_C_GameBegin * pGameBeginData)
	{
		//重新初始化数据
		initTheNewGameData();	
	}

	void GameTableUI::IShowNoticeMessage(const std::string& message)
	{
		HNAudioEngine::getInstance()->playEffect(BJL_GAME_MUSIC_WARNING);
		auto notice = GameNotice::create(_gameView->gametable_layout, message , _gameView->dash_bg_imgview);
		_gameView->gametable_layout->reorderChild(notice , ZORDER_MAX);
		notice->show();
	}

	void GameTableUI::IApplyNT(bool isNT)
	{
		_gameView->Button_applyNT->setVisible(!isNT);
		_gameView->Button_applyNoNT->setVisible(isNT);
	}

	void GameTableUI::IUpdateNTList()
	{
		std::vector<std::string> names = _logic->requestNTList();

		std::vector<T_DEALER> ntList;
		for(auto item: names)
		{
			T_DEALER name;
			name.dealerName = item;
			ntList.push_back(name);
		}
		_dealerList->resetDealerList(ntList);
	}


	void GameTableUI::IUpdateNTInfo(const std::string name, LLONG money)
	{
		// 庄家名字
		_dealerName->setString(name);
		
		// 庄家筹码
		if(money > 0)
		{
			char str[50] = {0};
			sprintf(str,"%lld",money);
			_dealerMoney->setVisible(true);
			_dealerMoney->setString(str);
		}
		else
		{
			_dealerMoney->setVisible(false);
		}		
	}

	// 显示自己下注额
	void GameTableUI::IDisplayMyBetMoney(int noteArea, LLONG money)
	{
		CCAssert(noteArea <= 7 || noteArea >= 0, "noteArea error.");
		_currentMyZhu[noteArea] += money;
		char str[50] = { 0 };
		sprintf(str, "%lld", _currentMyZhu[noteArea]);
		_gameView->TextAtlas_UserBet[noteArea]->setString(str);
		_gameView->TextAtlas_UserBet[noteArea]->setVisible(true);
	}

	// 更新玩家金币
	void GameTableUI::IUpdateUserMoney(LLONG money)
	{
		char str[50] = {0};
		sprintf(str,"%lld",money);
		_ownMoney->setString(str);
	}

	// 显示玩家下注动画
	void GameTableUI::IPlayNoteAnimation(BYTE noteType, int noteArea)
	{
		HNAudioEngine::getInstance()->playEffect(BJL_GAME_MUSIC_BET);
		showMyBetAnimation(noteType, noteArea);
	}

	void GameTableUI::ISetBetEnable(bool enabled[], int count)
	{
		CCAssert(count >= 7, "count error.");

		for(int i = 0; i < 7; i++)
		{
			if(!enabled[i])
			{
				_gameView->ImageView_BetBg[i]->stopAllActions();
			}			
			_gameView->ImageView_BetBg[i]->setVisible(enabled[i]);
			_gameView->Button_Bet[i]->setEnabled(enabled[i]);
		}
	}


	void GameTableUI::IStartTimer(int second)
	{
		_remainTime = second;

		char str[50] = {0};
		sprintf(str,"%d", second);
		_gameView->TextAtlas_TimerText->setString(str);
		_gameView->TextAtlas_TimerText->setScale(0.8f);
		unschedule(schedule_selector(GameTableUI::timerUpdate));

		if(second > 0)
		{
			_gameView->ImageView_TimerBg->setVisible(true);
			schedule(schedule_selector(GameTableUI::timerUpdate), 1.0f);
		}
		else
		{
			_gameView->ImageView_TimerBg->setVisible(false);
		}		
	}
	

	void GameTableUI::IAreaBet(int area, LLONG betMoney)
	{
		int rd = rand() % 7;
		CurJtInfo data;
		data.val  = betMoney;
		data.xpos = rd;	
		HNAudioEngine::getInstance()->playEffect(BJL_GAME_MUSIC_BET);
		_gameView->pnoteview[area].dropone(data, 0);
	}

	void GameTableUI::IInitCard(BYTE cards[][3], int playerCount, int NTCount, int playerCardType[], int NTCardType[])
	{
		_resultCard->initCard(cards, NTCount, playerCount, NTCardType, playerCardType);
	}

	void GameTableUI::IShowCards(bool animate)
	{
		if(animate)
		{

		}
		else
		{
			_resultCard->showCardBoradWithOutAnimation();
		}
	}

	void GameTableUI::ISendCard(bool animate)
	{
		if(animate)
		{
			_resultCard->initNewGame();
			_resultCard->showCardBorad();

			this->unschedule(schedule_selector(GameTableUI::timerUpdate));
			this->schedule(schedule_selector(GameTableUI::dispatchCard), 1);
		}
		else
		{
			_resultCard->dispatchCardWithoutAnimation();
		}
	}

	void GameTableUI::IShowBetHint(bool visible)
	{
		if(visible)
		{
			HNAudioEngine::getInstance()->playEffect("BJL/game/music/place.mp3");
			_gameView->ImageView_betHint->runAction(RepeatForever::create(Blink::create(0.5f, 1)));
		}
		else
		{
			_gameView->ImageView_betHint->stopAllActions();
			_gameView->ImageView_betHint->setVisible(false);
		}
	}

	void GameTableUI::IShowWaitHint(bool visible)
	{
		if(visible)
		{
			_gameView->ImageView_WaitHint->runAction(RepeatForever::create(Blink::create(0.5f, 1)));
		}
		else
		{
			_gameView->ImageView_WaitHint->stopAllActions();
			_gameView->ImageView_WaitHint->setVisible(false);
		}
	}

	void GameTableUI::IGameStart()
	{
		HNAudioEngine::getInstance()->playEffect("BJL/game/music/gamestart.mp3");
	}

	void GameTableUI::IShowWinArea(bool visible)
	{
		if(visible)
		{
			int* winArea = _logic->getWinArea();
			for (BYTE i = 0; i < NUMOFBETAREA ; ++i)
			{
				if (*(winArea + i) > 1)
				{
					_gameView->ImageView_BetWin[i]->setVisible(true);
					_gameView->ImageView_BetWin[i]->runAction(RepeatForever::create(Blink::create(0.5f, 1)));
				}
				else
				{
					_gameView->ImageView_BetWin[i]->stopAllActions();
					_gameView->ImageView_BetWin[i]->setVisible(false);
				}
			}
		}
		else
		{
			for (BYTE i = 0; i < NUMOFBETAREA ; ++i)
			{
				_gameView->ImageView_BetWin[i]->stopAllActions();
				_gameView->ImageView_BetWin[i]->setVisible(false);
			}
		}
	}

	void GameTableUI::IShowOpenBoard(bool visible)
	{
		if(visible)
		{

		}
		else
		{
			_resultCard->hideCardBorad();
		}		

	}

	void GameTableUI::showMyBetAnimation(BYTE whichBTN , BYTE whichArea)
	{
		//创建动画
		//int whichBTN = pos;
		if (whichBTN < 7)
		{
			char str[30] ={0};
			sprintf(str,"BT_JETTON_%d.png", whichBTN + 1);
			auto sp = Sprite::createWithSpriteFrameName(str);
			if (nullptr != sp)
			{
				_gameView->gametable_layout->addChild(sp, 5);
				sp->setScale(0.7f);
				sp->setVisible(true);
				auto pos1 = _gameView->Button_Bet[whichBTN]->getPosition();
				sp->setPosition(pos1);
				BET_TYPE *data = new BET_TYPE();
				data->whichType = whichArea;
				data->whichJetton = whichBTN;
				sp->setUserData((void*)data);
				auto seq = Sequence::create(
					Spawn::create(
					EaseOut::create(EaseSineOut::create(MoveTo::create(0.4f, _gameView->pnoteview[whichArea].getRandPositionInTheRect())), 1),
					nullptr
					),
					CallFuncN::create(CC_CALLBACK_1(GameTableUI::betAnimationCallback, this)),
					nullptr
					);
				sp->runAction(seq);
				spriteArr.pushBack(sp);
			}
		}
	}

	//结算
	void GameTableUI::showSettlement(S_C_GameResult* pGameResult)
	{
		//显示结算框
		showSettlementDialog(pGameResult);
	}

	//重置庄家列表
	void GameTableUI::resetDealerList(BYTE dealerStation[] , SHORT count)
	{
		char str[62] = {0};
		std::vector<T_DEALER> dealerlist;
		for (BYTE i = 0 ; i < count ; i++)
		{
			SHORT a = dealerStation[i];
			auto user = _logic->getUserBySeatNo(dealerStation[i]);
			if (nullptr != user)
			{
				strcpy(str, GBKToUtf8(user->nickName));
				T_DEALER tdealer;
				tdealer.dealerName = str;
				if (tdealer.dealerName.empty())
				{
					continue; 
				}
				dealerlist.push_back(tdealer);
			}
		}
		_dealerList->resetDealerList(dealerlist);
	}
	
	void GameTableUI::showSettlementDialog(S_C_GameResult* pData)
	{
		CCAssert(pData != nullptr, "nullptr.");
		
		if (nullptr != _gameResult)
		{
			_gameResult->hide();
			_gameResult = nullptr;
		}

		//结算框
		_gameResult = ResultDialog::create(this,pData->i64NtScoreSum, pData->i64UserScoreSum);	
		this->reorderChild(_gameResult , ZORDER_MAX + 1);
		_gameResult->show();

		if (pData->i64UserWin > 0)
		{
			HNAudioEngine::getInstance()->playEffect(BJL_GAME_MUSIC_WIN);
		}
		else
		{
			HNAudioEngine::getInstance()->playEffect(BJL_GAME_MUSIC_LOSE);
		}
	}

	void GameTableUI::hideXiaZhuBTN()
	{
		stopBetAnimation();
		for(BYTE i=0; i < CHOUMANUM ; ++i)		//下注按钮
		{
			_gameView->ImageView_BetBg[i]->setBright(true);
			_gameView->Button_Bet[i]->setEnabled(false);
		}
	}


	void GameTableUI::timerUpdate(float delat)
	{
		if (_remainTime < 1)
		{
			unschedule(schedule_selector(GameTableUI::timerUpdate));
			hideXiaZhuBTN();
			return ;
		}

		//隐藏结算框
		if (_remainTime == _logic->getFreeTime() && _logic->getGameStatus() == GS_WAIT_NEXT && nullptr!=_gameResult)
		{
			_gameResult->hide();
			_gameResult = nullptr;
		}

		_remainTime--;
		char str[15];
		sprintf(str,"%d",_remainTime);
		_gameView->TextAtlas_TimerText->setString(str);
	}

	void GameTableUI::dispatchCard(float delta)
	{
		if (!_resultCard->dispatchCard())
		{
			this->unschedule(schedule_selector( GameTableUI::dispatchCard));
		}
	}

	void GameTableUI::onExit()
	{
		auto cache = SpriteFrameCache::getInstance();
		cache->removeSpriteFramesFromFile("BJL/game/table/jettonbtn.plist");
		cache->removeSpriteFramesFromFile("BJL/game/table/cards.plist");
		cache->removeSpriteFramesFromFile("BJL/game/table/cardtype.plist");
		
		HNGameUIBase::onExit();
	}
	void GameTableUI::onEnter()
	{
		HNGameUIBase::onEnter();
	}

	void GameTableUI::SetImageTimeMoveNewPostion()
	{
		//auto postion = _gameView->ImageView_TimerBg->getPosition();

		//_gameView->ImageView_TimerBg->setPosition(Vec2(postion.x - 80, postion.y + 80));
		_gameView->ImageView_TimerBg->runAction(CCEaseExponentialOut::create(MoveBy::create(1.0f,Vec2(-80, 80))));
	}

	void GameTableUI::SetImageTimeMoveOldPostion()
	{
		//auto postion = _gameView->ImageView_TimerBg->getPosition();

		_gameView->ImageView_TimerBg->runAction(CCEaseExponentialOut::create(MoveBy::create(1.0f,Vec2(80, -80))));
		//_gameView->ImageView_TimerBg->setPosition(Vec2(postion.x + 80, postion.y - 80));

	}


}
