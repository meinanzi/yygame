#include "HNLobby/GameMatch.h"
#include "HNLobby/PlatformConfig.h"
#include "HNLobby/PlatformDefine.h"
#include "HNLobby/GamePlatform.h"
#include "HNLobby/GamePrompt.h"
#include "HNLobby/FontConfig.h"
#include "HNLobby/GameMenu/GameMenu.h"

//////////////////////////////////////////////////////////////////////////
static const char* MATCH_BG	= "platform/hallBg.png";
static const char* LOADING_BG = "platform/match/loadingBG.png";
static const char* RETURN_N	= "platform/room/return_n.png";
static const char* RETURN_D	= "platform/room/return_d.png";

static const char* JOIN_MATHCH_N_PNG	= "platform/room/join_mathch_n.png";
static const char* JOIN_MATHCH_D_PNG	= "platform/room/join_mathch_d.png";
static const char* UNJOIN_MATHCH_N_PNG = "platform/room/unjoin_mathch_n.png";
static const char* UNJOIN_MATHCH_D_PNG = "platform/room/unjoin_mathch_d.png";

static const char* MATHCH_UI_JSON = "platform/room/MatchUI_1.json";
static const char* MATHCH_UI_OPERATOR_JSON = "platform/room/MatchUI_2.json";

//////////////////////////////////////////////////////////////////////////

void GameMatch::createMatch()
{
	auto matchScene = Scene::create();
	auto gameMatch = GameMatch::create();
	matchScene->addChild(gameMatch);
	Director::getInstance()->replaceScene(TransitionSlideInR::create(0.3f, matchScene));
}

GameMatch::GameMatch()
	:_btnIndex (0),
	_Time (nullptr),
	_isOver(false),
	_isJoin(false),
	_beginTimeText(nullptr),
	_timeMatchIndex(-1),
	_iValue(-1)
{
	memset(&_matchLayout, 0x0, sizeof(_matchLayout));
	memset(&_operatorLayout, 0x0, sizeof(_operatorLayout));
	
	RoomLogic()->addRoomObserver(this);
}

GameMatch::~GameMatch()
{
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("platform/match/timeImage.plist");

	RoomLogic()->removeEventSelector(MDM_GR_ROOM, ASS_GR_CONTEST_APPLY);
	PlatformLogic()->removeEventSelector(MDM_GP_PROP, ASS_PROP_BUY);
	RoomLogic()->removeRoomObserver(this);
}

bool GameMatch::init()
{
    if ( !HNLayer::init())
    {
        return false;
    }

	setBackGroundImage(MATCH_BG);
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("platform/match/timeImage.plist");
	
	auto visibleSize = Director::getInstance()->getVisibleSize();
	
	auto matchNode1 = CSLoader::createNode("platform/match/MatchUI_1.csb");
	_matchLayout.MatchInfoLayout = (Layout*)matchNode1->getChildByName("Panel_14");
	if (nullptr != _matchLayout.MatchInfoLayout)
	{
		_matchLayout.MatchInfoLayout->setAnchorPoint(Vec2(0.5f, 0.5f));
		_matchLayout.MatchInfoLayout->setPosition(Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.55f));
		this->addChild(matchNode1, 1);
		_matchLayout.Label_ChampionCount = (Text*)Helper::seekWidgetByName(_matchLayout.MatchInfoLayout, "Label_ChampionCount");
		_matchLayout.Label_RankingCount = (Text*)Helper::seekWidgetByName(_matchLayout.MatchInfoLayout, "Label_RankingCount");
		_matchLayout.Label_MatchCount = (Text*)Helper::seekWidgetByName(_matchLayout.MatchInfoLayout, "Label_MatchCount");
		
		_matchLayout.ListView_Reward = (ListView*)Helper::seekWidgetByName(_matchLayout.MatchInfoLayout, "ListView_Reward");
		_matchLayout.ListView_Reward->setItemsMargin(10.0);
	}

	auto matchNode2 = CSLoader::createNode("platform/match/MatchUI_2.csb");
	auto operatorLayout = (Layout*)matchNode2->getChildByName("Panel_15");
	if (nullptr != operatorLayout)
	{
		operatorLayout->setAnchorPoint(Vec2(0.5f, 0.5f));
		Size size = operatorLayout->getContentSize();
		operatorLayout->setPosition(Vec2(visibleSize.width - size.width * 0.5f, size.height * 0.5f));
		this->addChild(matchNode2, 1);
		
		_operatorLayout.Label_SignedCount = (Text*)Helper::seekWidgetByName(operatorLayout, "Label_SignedCount");
		_operatorLayout.Label_NeedCount = (Text*)Helper::seekWidgetByName(operatorLayout, "Label_NeedCount");
		// 加入比赛
		_operatorLayout.Button_JoinMatch = (Button*)Helper::seekWidgetByName(operatorLayout, "Button_JoinMatch");
		_operatorLayout.Button_JoinMatch->addTouchEventListener(CC_CALLBACK_2(GameMatch::joinMatchCallBack, this));

		// 取消比赛
		_operatorLayout.Button_UnJoinMatch = (Button*)Helper::seekWidgetByName(operatorLayout, "Button_UnJoinMatch");
		_operatorLayout.Button_UnJoinMatch->addTouchEventListener(CC_CALLBACK_2(GameMatch::joinMatchCallBack, this));
		_operatorLayout.Button_UnJoinMatch->setVisible(false);

		//购买参赛卷
		_operatorLayout.Button_BuyTicket = (Button*)Helper::seekWidgetByName(operatorLayout, "Button_BuyTicket");
		_operatorLayout.Button_BuyTicket->addTouchEventListener(CC_CALLBACK_2(GameMatch::buyMatchTicketCallBack, this));

		//如果是周、月、年赛才显示
		if (RoomLogic()->getRoomRule() & GRR_CONTEST_WEEK || 
			RoomLogic()->getRoomRule() & GRR_CONTEST_MONTH || 
			RoomLogic()->getRoomRule() & GRR_CONTEST_YEAR)
		{
			_operatorLayout.Button_BuyTicket->setVisible(true);
		}
		else
		{
			_operatorLayout.Button_BuyTicket->setVisible(false);
		}
	}

	// 返回按钮
	auto returnBtn = Button::create(RETURN_N, RETURN_D);
	returnBtn->addTouchEventListener(CC_CALLBACK_2(GameMatch::returnRoom, this));
	returnBtn->setPosition(Vec2(returnBtn->getContentSize().width / 2, visibleSize.height - returnBtn->getContentSize().height));
	this->addChild(returnBtn);

	contestInit();

	if (waitTimeMatch())			return true;

	schedule(schedule_selector(GameMatch::checkTimeMatchStart), 1.0);
	this->scheduleOnce(schedule_selector(GameMatch::overdue), 0.5);
	//报名、退赛消息回调
	RoomLogic()->addEventSelector(MDM_GR_ROOM, ASS_GR_CONTEST_APPLY, HN_SOCKET_CALLBACK(GameMatch::contestRegistrationResult, this));
	//购买道具消息
	PlatformLogic()->addEventSelector(MDM_GP_PROP, ASS_PROP_BUY, HN_SOCKET_CALLBACK(GameMatch::constBuyPropResult, this));
	return true;
}

void GameMatch::overdue(float dt)
{
	if (UserDefault::getInstance()->getStringForKey("DXCL") != "")		//断线重连
	{
		char str[64] = { 0 };
		for (int i = 0; i < 10; i++)
		{
			sprintf(str, "MARK_%d", i);
			if (UserDefault::getInstance()->getIntegerForKey(str) == 5)
			{
				UserDefault::getInstance()->setIntegerForKey(str, 1);
				break;
			}
		}
		UserDefault::getInstance()->setStringForKey("DXCL", "");
		UserDefault::getInstance()->flush();
		GamePromptLayer::create()->showPrompt(GBKToUtf8("比赛已过期！"));
		exitMatchRoom();
	}
}

void GameMatch::checkTimeMatchStart(float dt)
{
	char str[64] = {0};
	for (int i = 0; i < 10; i++)
	{
		sprintf(str, "MARK_%d", i);
		if (UserDefault::getInstance()->getIntegerForKey(str) == 2)
		{
			sprintf(str, "beginTime_%d", i);
			std::string beginTime = UserDefault::getInstance()->getStringForKey(str);
			time_t bTime = 0;
			for (auto iter = beginTime.begin(); iter != beginTime.end(); iter++)
			{
				bTime = bTime*10 + *iter - '0';
			}
			time_t nTime = time(0);
			if ((nTime + 60 + 8*3600 >= bTime) && (nTime + 8*3600 <= bTime))
			{
				sprintf(str, "UserID_%d", i);
				_iValue = i;
				if (UserDefault::getInstance()->getIntegerForKey(str) != PlatformLogic()->loginResult.dwUserID)		continue;
				unschedule(schedule_selector(GameMatch::checkTimeMatchStart));
				
				auto prompt = GamePromptLayer::create();
				prompt->showPrompt(GBKToUtf8("定时赛马上开始，是否进入？"));
				prompt->setPromptCanSelect();
				prompt->setCallBack([&](){
					char str[64] = {0};
					sprintf(str, "MARK_%d", _iValue);
					UserDefault::getInstance()->setIntegerForKey(str, 3);
					UserDefault::getInstance()->flush();
					cleanTimeMatchData();
					UserInfoModule()->clear();
					RoomLogic()->close();
					GamePlatform::createPlatform();
					//waitTimeMatch();
				});
				prompt->setCancelCallBack([&](){
					char str[64] = {0};
					sprintf(str, "MARK_%d", _iValue);
					UserDefault::getInstance()->setIntegerForKey(str, 4);
					UserDefault::getInstance()->flush();
					schedule(schedule_selector(GameMatch::checkTimeMatchStart), 1.0);
				});
			}
		}
	}
}

bool GameMatch::waitTimeMatch()
{
	char str[64] = {0};
	for (int i = 0; i < 10; i++)
	{
		sprintf(str, "MARK_%d", i);
		if (UserDefault::getInstance()->getIntegerForKey(str) == 3)
		{
			UserDefault::getInstance()->setIntegerForKey(str, 5);
			UserDefault::getInstance()->flush();
			setBackGroundImage(LOADING_BG);
			_matchLayout.MatchInfoLayout->setVisible(false);
			_operatorLayout.Label_SignedCount->getParent()->setVisible(false);
			_beginTimeText->setText(GBKToUtf8("定时赛60秒后开始，请耐心等待"));
			auto winSize = Director::getInstance()->getWinSize(); 
			_beginTimeText->setPosition(Vec2(winSize.width/2, winSize.height/2));
			_beginTimeText->setTag(59);
			_beginTimeText->setFontSize(40);
			schedule(schedule_selector(GameMatch::timeMatchCountDown), 1.0);
			return true;
		}
	}
	return false;
}

void GameMatch::timeMatchCountDown(float delay)
{
	char name[200] = {0};
	int tag = _beginTimeText->getTag();
	sprintf(name, "定时赛%d秒后开始，请耐心等待", tag);
	_beginTimeText->setText(GBKToUtf8(name));

	if (tag == 0)
	{
		sprintf(name, "定时赛已开始，等待配桌...");
		_beginTimeText->setText(GBKToUtf8(name));
		unschedule(schedule_selector(GameMatch::timeMatchCountDown));
		return;
	}	
	_beginTimeText->setTag(tag-1);
}

std::string GameMatch::stringToTime(std::string beginTime)
{
	char name[64] = {0};
	char mark[64] = {0};
	std::string time;
	if (beginTime == "")
	{	
		for (int i = 1; i < 10; i++)
		{
			sprintf(name, "beginTime_%d", i);
			if (UserDefault::getInstance()->getStringForKey(name) != "")
			{
				sprintf(mark, "MARK_%d", i);
				if (UserDefault::getInstance()->getIntegerForKey(mark) == 1)
				{
					_timeMatchIndex = i;
					break;
				}
			}
		}
		time = UserDefault::getInstance()->getStringForKey(name);
	}
	else
	{
		time = UserDefault::getInstance()->getStringForKey(beginTime.c_str());
	}

	if (UserDefault::getInstance()->getStringForKey("DXCL") != "")		//追加断线重连
	{
		time = UserDefault::getInstance()->getStringForKey("DXCL");
	}

	time_t Time = 0;
	for (auto iter = time.begin(); iter != time.end(); iter++)
	{
		Time = Time*10 + *iter - '0';
	}

	struct tm *p = gmtime(&Time);
	char BeginTime[64];
	strftime(BeginTime, sizeof(BeginTime), "%Y-%m-%d %H:%M:%S", p);
	return BeginTime;
}

bool GameMatch::contestInit()
{
	auto winSize = Director::getInstance()->getWinSize();
	auto Ddata = UserDefault::getInstance();
	
	int value = Ddata->getIntegerForKey("iConstestNum");
	char Num[10] = {0};
	if (RoomLogic()->getRoomRule() & GRR_TIMINGCONTEST)
	{
		std::string text = "比赛开始时间 :	";
		std::string time = stringToTime("");	
		text.append(time);

		_beginTimeText = Text::create(GBKToUtf8(text.c_str()), "", 40);
		this->addChild(_beginTimeText);
		_beginTimeText->setPosition(Vec2(winSize.width/2, 30));
		_beginTimeText->setColor(Color3B::YELLOW);
		
	}
	sprintf(Num,"%d", value);
	_operatorLayout.Label_SignedCount->setString(Num);

	int needPeople = RoomLogic()->getSelectedRoom()->iUpPeople - value;
	sprintf(Num, "%d", needPeople);
	_operatorLayout.Label_NeedCount->setString(Num);

	if (needPeople <= 1)
	{
		_isOver = true;
	}
	
	char buffer[64];
	// 夺冠次数
	sprintf(buffer, "%d", Ddata->getIntegerForKey("iChampionCount"));
	_matchLayout.Label_ChampionCount->setText(buffer);

	// 最佳排名
	value = Ddata->getIntegerForKey("iBestRank");
	if ((value <= 3) && (value > 0))
	{
		sprintf(buffer, "platform/match/paiMing_%d.png", value);
		auto sprite = Sprite::create(buffer);
		_matchLayout.MatchInfoLayout->addChild(sprite);
		sprite->setPosition(Vec2(568, 190));
	}
	else
	{
		sprintf(buffer, "%d", value);
		_matchLayout.Label_RankingCount->setText(buffer);
	}

	// 比赛次数
	sprintf(buffer, "%d", Ddata->getIntegerForKey("iJoinCount"));
	_matchLayout.Label_MatchCount->setText(buffer);

	//奖励方案
	int str[3];
	str[0] = Ddata->getIntegerForKey("iRankAward1");
	str[1] = Ddata->getIntegerForKey("iRankAward2");
	str[2] = Ddata->getIntegerForKey("iRankAward3");
	
	int ptr[3];
	ptr[0] = Ddata->getIntegerForKey("iAwardType1");
	ptr[1] = Ddata->getIntegerForKey("iAwardType2");
	ptr[2] = Ddata->getIntegerForKey("iAwardType3");

	for (int i = 0; i < 3; i++)
	{
		if (!ptr[i])
		{
			sprintf(buffer, GBKToUtf8("第 %d 名:奖励%9d金币"), i + 1, str[i]);
		}
		else
		{
			sprintf(buffer, GBKToUtf8("第 %d 名:奖励%9d元亨卡"), i + 1, str[i]);
		}
		Text * tmp = Text::create(GBKToUtf8(buffer), "", 25);
		_matchLayout.ListView_Reward->pushBackCustomItem(tmp);
	}

	sprintf(buffer, GBKToUtf8("报名费:%8d金币"), Ddata->getIntegerForKey("iEntryFee"));
	Text * tmp = Text::create(GBKToUtf8(buffer), "", 25);
	_matchLayout.ListView_Reward->pushBackCustomItem(tmp);

	return true;
}

void GameMatch::I_R_M_ContestNotic(MSG_GR_I_ContestInfo* contestInfo)
{
	char Num[10] = {0};
	sprintf(Num,"%d", contestInfo->iContestNum);
	_operatorLayout.Label_SignedCount->setString(Num);
	
	int needPeople = RoomLogic()->getSelectedRoom()->iUpPeople - contestInfo->iContestNum;
	sprintf(Num, "%d", needPeople);
	_operatorLayout.Label_NeedCount->setString(Num);

	if (needPeople <= 1)
	{
		_isOver = true;
	}

	if (needPeople == 0)
	{	
		if (!_isJoin)
		{
			exitMatchRoom();
			return;
		}
		if (RoomLogic()->getRoomRule() & GRR_TIMINGCONTEST)			return;
		
		setBackGroundImage(LOADING_BG);

		_matchLayout.MatchInfoLayout->setVisible(false);
		_operatorLayout.Label_SignedCount->getParent()->setVisible(false);

		schedule(schedule_selector(GameMatch::countDown), 1.0f);
	}
}

void GameMatch::countDown(float delay)
{
	if (!_Time)
	{
		_Time = ImageView::create("platform/match/10.png");
		addChild(_Time);
		_Time->setPosition(Vec2(640, 400));
		_Time->setTag(10);
		_Time->setScale(2.0);
	}
	else
	{
		_Time->setScale(2.0);
		if (_Time->getTag() > 0)
		{
			_Time->setTag(_Time->getTag() - 1);
		}
		else
		{
			auto text = Text::create(GBKToUtf8("比赛已开始，等待配桌..."), "", 40);
			text->setPosition(_Time->getPosition());
			addChild(text,10);
			text->setColor(Color3B::YELLOW);
			unschedule(schedule_selector(GameMatch::countDown));
			return;
		}
		char name[40] = {0};
		sprintf(name,"platform/match/%d.png", _Time->getTag());
		_Time->loadTexture(name);
	}
	_Time->runAction(ScaleTo::create(1.0, 0.0));
}

bool GameMatch::contestRegistrationResult(HNSocketMessage* socketMessage)
{
	CHECK_SOCKET_DATA_RETURN(MSG_GR_ContestApply, socketMessage->objectSize, true, "MSG_GR_ContestApply size is error");
	MSG_GR_ContestApply* applyResult = (MSG_GR_ContestApply*)socketMessage->object;

		if(_btnIndex == 0)
	{
		_operatorLayout.Button_JoinMatch->setVisible(false);
		_operatorLayout.Button_JoinMatch->setEnabled(false);
		_operatorLayout.Button_UnJoinMatch->setVisible(true);
		_operatorLayout.Button_UnJoinMatch->setEnabled(true);
		_btnIndex = 1;
	}
	else
	{
		_operatorLayout.Button_UnJoinMatch->setVisible(false);
		_operatorLayout.Button_UnJoinMatch->setEnabled(false);
		_operatorLayout.Button_JoinMatch->setVisible(true);
		_operatorLayout.Button_JoinMatch->setEnabled(true);
		_btnIndex = 0;
	}

	std::string prompt("");
	char str[64] = {0};
	switch (applyResult->iApplyResult)
	{
	case 1:
		{
			if (RoomLogic()->getRoomRule() & GRR_TIMINGCONTEST)
			{
				sprintf(str, "MARK_%d", _timeMatchIndex);
				UserDefault::getInstance()->setIntegerForKey(str, 2);
				sprintf(str, "RoomID_%d", _timeMatchIndex);
				UserDefault::getInstance()->setIntegerForKey(str, RoomLogic()->getSelectedRoom()->uRoomID);
				sprintf(str, "UserID_%d", _timeMatchIndex);
				UserDefault::getInstance()->setIntegerForKey(str, PlatformLogic()->loginResult.dwUserID);
				prompt = "报名成功，比赛开始时间为	";
				sprintf(str, "beginTime_%d", _timeMatchIndex);				
				prompt.append(stringToTime(str));
			}
			else
			{
				prompt = "报名成功，等待比赛开始。";
			}
			_isJoin = true;
		} break;
	case 2: 
		{
			if (RoomLogic()->getRoomRule() & GRR_TIMINGCONTEST)
			{
				sprintf(str, "MARK_%d", _timeMatchIndex);
				UserDefault::getInstance()->setIntegerForKey(str, 1);
			}
			prompt = "退赛成功。";
			_isJoin = false;
		} break;
	case 3:
		{
			prompt = "已经报名，无需再次报名。";
			_isJoin = true;
		} break;
	case 4:
		{
			prompt = "比赛已经开始，无法报名。";
		} break;
	case 5:
		{
			prompt = "未报名，退赛失败。";
		} break;
	case 6:
		{
			prompt = "比赛已开始，无法退赛。";
		} break;
	case 7:
		{
			prompt = "达到最大报名人数，无法报名。";
		} break;
	case 8:
		{
			prompt = "金币不足，无法报名。";
		} break;
	case 10:
	{
		prompt = "周参赛卷不足，无法报名！";
	} break;
	case 11:
	{
		prompt = "月参赛卷不足，无法报名！";
	} break;
	case 12:
	{
		prompt = "年参赛卷不足，无法报名！";
	} break;
	default:
		break;
	}

	UserDefault::getInstance()->flush();

	if (!prompt.empty())
	{
		if ((RoomLogic()->getRoomRule() & GRR_TIMINGCONTEST) || (!_isOver))
		{
			GamePromptLayer::create()->showPrompt(GBKToUtf8(prompt.c_str()));
		}
	}

	return true;
}

//购买道具消息
bool GameMatch::constBuyPropResult(HNSocketMessage* socketMessage)
{
	CHECK_SOCKET_DATA_RETURN(_TAG_USERPROP, socketMessage->objectSize, true, "_TAG_USERPROP size is error");
	_TAG_USERPROP* pBuyPropResult = (_TAG_USERPROP*)socketMessage->object;

	std::string prompt("");
	switch (socketMessage->messageHead.bHandleCode)
	{
	case  DTK_GP_PROP_BUY_ERROR:	//
	{
		/*if (RoomLogic()->getRoomRule() & GRR_CONTEST_WEEK)
		{
			prompt = "购买周参赛卡失败";
		}
		else if (RoomLogic()->getRoomRule() & GRR_CONTEST_MONTH)
		{
			prompt = "购买月参赛卡失败";
		}	
		else if (RoomLogic()->getRoomRule() & GRR_CONTEST_YEAR)
		{
			prompt = "购买年参赛卡失败";
		}
		else*/
		{
			prompt = "购买参赛卡失败";
		}
		break;
	}
	case  DTK_GP_PROP_BUY_NOMONEY:	//
	{
		prompt = "银行金币不足,购买参赛卡失败";
		break;
	}
	case  DTK_GP_PROP_BUY_SUCCEED:	//
	{
		prompt = "购买参赛卡成功";
		break;
	}
	default:
		break;
	}

	if (!prompt.empty())
	{
		GamePromptLayer::create()->showPrompt(GBKToUtf8(prompt.c_str()));
	}

}

void GameMatch::cleanTimeMatchData()
{
	if (_timeMatchIndex == -1)		return;
	char str[64] = {0};
	sprintf(str, "MARK_%d", _timeMatchIndex);
	if (UserDefault::getInstance()->getIntegerForKey(str) == 1)
	{
		UserDefault::getInstance()->setIntegerForKey(str, 0);
		sprintf(str, "beginTime_%d", _timeMatchIndex);
		UserDefault::getInstance()->setStringForKey(str, "");
	}
}

void GameMatch::exitMatchRoom()
{
	if (isScheduled(schedule_selector(GameMatch::countDown)))
	{
		unschedule(schedule_selector(GameMatch::countDown));
	}

	if (_beginTimeText != nullptr)
	{
		_beginTimeText->stopAllActions();
	}

	cleanTimeMatchData();
	HNAudioEngine::getInstance()->playEffect(GAME_SOUND_CLOSE);
	UserInfoModule()->clear();
	RoomLogic()->close();
	GamePlatform::returnPlatform(LayerType::ROOMLIST);
}

void GameMatch::returnRoom(Ref* pSender, Widget::TouchEventType type)
{
	if (Widget::TouchEventType::ENDED != type) return;
	
	exitMatchRoom();
}

void GameMatch::joinMatchCallBack(Ref* pSender, Widget::TouchEventType type)
{
	if (Widget::TouchEventType::ENDED != type) return;
	MSG_GR_I_ContestApply registration;
	registration.iUserID = RoomLogic()->loginResult.pUserInfoStruct.dwUserID;
	registration.iTypeID = _btnIndex;

	RoomLogic()->sendData(MDM_GR_USER_ACTION, ASS_GR_CONTEST_APPLY, (void*)&registration, sizeof(registration));
}

void GameMatch::buyMatchTicketCallBack(Ref* pSender, Widget::TouchEventType type)
{
	if (Widget::TouchEventType::ENDED != type) return;

	if (RoomLogic()->getRoomRule() & GRR_CONTEST_WEEK ||
		RoomLogic()->getRoomRule() & GRR_CONTEST_MONTH ||
		RoomLogic()->getRoomRule() & GRR_CONTEST_YEAR)
	{
		_TAG_PROP_BUY	TBuyProp;
		TBuyProp.dwUserID = RoomLogic()->loginResult.pUserInfoStruct.dwUserID;
		TBuyProp.nPropID = 10;						//道具ID
		TBuyProp.iPropPayMoney = 0;				//总共的金币
		TBuyProp.nPropBuyCount = 1;				//道具数量
		PlatformLogic()->sendData(MDM_GP_PROP, ASS_PROP_BUY, (void*)&TBuyProp, sizeof(TBuyProp));
	}
}



// 排队用户坐下
void GameMatch::I_R_M_QueueUserSit(BYTE deskNo, const std::vector<QUEUE_USER_SIT_RESULT*>& users)
{
	CCAssert(INVALID_DESKNO != deskNo, "invalid deskno.");
	if (INVALID_DESKNO == deskNo) return;
	if ((_Time != nullptr) && (RoomLogic()->getRoomRule() & GRR_CONTEST))
	{
		if (_Time->getTag() > 0)  
		{
			unschedule(schedule_selector(GameMatch::countDown));
		}
	}
	if (isScheduled(schedule_selector(GameMatch::timeMatchCountDown)))
	{
		unschedule(schedule_selector(GameMatch::timeMatchCountDown));
	}

	bool find = false;
	for (auto iter = users.begin(); iter != users.end(); ++iter)
	{
		if((*iter)->dwUserID == PlatformLogic()->loginResult.dwUserID)
		{
			find = true;
			break;
		}
	}
	if (find)
	{
		LoadingLayer::removeLoading(this);
		// 启动游戏
		bool ret = GameCreator()->startGameClient(RoomLogic()->getSelectedRoom()->uNameID, deskNo, true);
		if (!ret)
		{
			GamePromptLayer::create()->showPrompt(GBKToUtf8("游戏启动失败。"));
		}
	}
}

void GameMatch::I_R_M_ContestOver(MSG_GR_ContestAward* contestAward)
{
	if (contestAward->iAward == -1)
	{
		GamePromptLayer::create()->showPrompt(GBKToUtf8("有人退出比赛，游戏结束"));
	
		this->runAction(Sequence::create(DelayTime::create(2.0), CallFunc::create([&](){
		exitMatchRoom();
		}), nullptr));		
	}
}