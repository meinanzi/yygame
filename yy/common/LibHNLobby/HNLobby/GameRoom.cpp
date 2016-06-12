#include "HNLobby/GameRoom.h"
#include "HNLobby/GamePlatform.h"
#include "HNLobby/FontConfig.h"
#include "HNLobby/PlatformDefine.h"
#include "HNLobby/GameDesk.h"
#include "HNLobby/GamePrompt.h"
#include "HNLobby/GameMenu/GameMenu.h"
#include "HNLobby/PlatformConfig.h"
#include "HNLobby/ChildLayer/GamePasswordInput.h"
#include "HNLobby/GameMatch.h"
#include "HNUIExport.h"
#include <string>

//////////////////////////////////////////////////////////////////////////
static const float enter_game_desk_outtime_timer = 30.0f;
static const float update_game_room_people_timer = 2.0f;
//////////////////////////////////////////////////////////////////////////
static const char* connect_room_text		= "连接房间......";
static const char* login_room_text			= "登陆房间......";
static const char* request_room_info_text	= "获取房间数据......";
static const char* allocation_table_please_wait_text = "正在配桌，请稍后......";
static const char* enterGame_please_wait_text = "正在进入游戏，请稍后......";
//////////////////////////////////////////////////////////////////////////
static const char* ROOM_ITEM_UI = "platform/room/roomItem.csb";
static const char* ROOM_NAME_FNT = "platform/fonts/gameName.fnt";

static const char* GAME_PAGESPROMPT_PATH = "platform/gameList/gameItemRes/page_Prompt.png";
//////////////////////////////////////////////////////////////////////////
static const Size pageViewRoomsSize(1280, 500);

//////////////////////////////////////////////////////////////////////////
GameRoom::GameRoom()
	: _currentSelectedRoom (nullptr)
	, _pageViewRooms (nullptr)
	, onCloseCallBack (nullptr)
	, onEnterDeskCallBack (nullptr)
	, _currentPageIdx (0)
	, _isTouch (true)
	,_overdueTime(false)
	,_fastEnterMatch(false)
	,_roomID(-1)
{
	_roomLogic = new HNRoomLogicBase(this);
}

GameRoom::~GameRoom()
{
	_roomLogic->stop();
	PlatformLogic()->removeEventSelector(MDM_GP_GET_CONTEST_ROOMID, 0);
	PlatformLogic()->removeEventSelector(MDM_GP_GET_CONTEST_ROOMID, 1);
	PlatformLogic()->removeEventSelector(MDM_GP_GET_TIMINGMATCH_TIME, ASS_GP_GET_TIMINGMATCH_TIME);
	HN_SAFE_DELETE(_roomLogic);
}

void GameRoom::closeRoomList()
{
	_roomLogic->stop();
	this->runAction(Sequence::create(FadeOut::create(0.3f), CallFunc::create([&](){
		if (nullptr != onCloseCallBack)
		{
			onCloseCallBack();
		}
		this->removeFromParent();
	}), nullptr));	
}

void GameRoom::onEnterPasswordClickCallback(const std::string& password)
{
	ComRoomInfo* pData = static_cast<ComRoomInfo*>(_currentSelectedRoom->getUserData());
	_roomLogic->requestRoomPasword(pData->uRoomID, password);
}

bool GameRoom::init()
{
    if (!HNLayer::init())
    {
        return false;
    }

	this->setCascadeOpacityEnabled(true);

	createRoomList();

	PlatformLogic()->addEventSelector(MDM_GP_GET_CONTEST_ROOMID, 1, HN_SOCKET_CALLBACK(GameRoom::contestInitError, this));

    return true;
}

void GameRoom::onExit()
{
	HNLayer::onExit();
}

void GameRoom::createRoomList()
{
	Size winSize = Director::getInstance()->getWinSize();

	_pageViewRooms = PageView::create();
	_pageViewRooms->setContentSize(pageViewRoomsSize);
	_pageViewRooms->setAnchorPoint(Vec2(0.5f, 0.5f));
	_pageViewRooms->setPosition(Vec2(winSize.width * 0.5f, winSize.height * 0.5f));
	_pageViewRooms->setTouchEnabled(true);
	_pageViewRooms->setCustomScrollThreshold(15);
	addChild(_pageViewRooms, 3);

	_pageViewRooms->setCascadeOpacityEnabled(true);

	int roomAllCount = RoomInfoModule()->getCountIncludeMatch();
	int currentIndex = 0; 

	std::vector<ComRoomInfo*> pages;

	int pageCount = (roomAllCount / 4);
	int remainder = (roomAllCount % 4);

	int pageLen = (remainder == 0) ? pageCount : pageCount + 1;
	int central = -pageLen / 2;
	if (abs(central) > 0)
	{
		for (int i = 0; i < pageLen; i++)
		{
			auto page = Sprite::create(GAME_PAGESPROMPT_PATH);
			addChild(page);
			_pagesPrompt.push_back(page);

			page->setPosition(Vec2(winSize.width / 2 + central * 25, winSize.height * 0.16f));
			if (i > 0)
			{
				page->setScale(0.8f);
				page->setColor(Color3B(150, 150, 150));
			}
			central++;
		}
	}

	if (pageCount > 0)
	{
		for (int currentPage  = 0; currentPage < pageCount; currentPage++)
		{
			pages.clear();
			for (int room = 0; room < 4; currentIndex ++)
			{
				ComRoomInfo* roomInfo = RoomInfoModule()->getRoom(currentIndex);

				if (!roomInfo)			continue;
				if (JudgeMatchRoom(roomInfo->iUpPeople, roomInfo->dwRoomRule))		continue;
				room ++;

				pages.push_back(roomInfo);
			}
			createRoomPage(pages);
		}
	}

	if (remainder > 0)
	{
		pages.clear();
		for (int room = 0; room < remainder; currentIndex ++)
		{
			ComRoomInfo* roomInfo = RoomInfoModule()->getRoom(currentIndex);

			if (JudgeMatchRoom(roomInfo->iUpPeople, roomInfo->dwRoomRule))		continue;
			room ++;
			pages.push_back(roomInfo);
		}
		createRoomPage(pages);
	}
}

//添加房间
void GameRoom::createRoomPage(std::vector<ComRoomInfo*> pages)
{
	//创建房间列表子页面
	auto roomItemLayout = Layout::create();
	roomItemLayout->setName("page");
	roomItemLayout->setContentSize(pageViewRoomsSize);

	roomItemLayout->setCascadeOpacityEnabled(true);

	int idx = 0;
	for (auto room : pages)
	{
		idx++;
		float posX = idx % 2 ? 0.25f : 0.75f;
		float posY = 0.0f;
		if (idx <= 2) posY = 0.60f;
		else posY = 0.20f;

		auto roomItem = createRoomItem(room);
		auto button = (Button*)roomItem->getChildByName("Button_roomItem");
		button->removeFromParentAndCleanup(false);
		if (nullptr != roomItem)
		{
			button->setPosition(Vec2(roomItemLayout->getContentSize().width * posX, roomItemLayout->getContentSize().height * posY));
			roomItemLayout->addChild(button, 3);
		}
	}	

	// 添加子页面进入列表中
	_pageViewRooms->addPage(roomItemLayout);

	// 拖动监听
	_pageViewRooms->addEventListener(PageView::ccPageViewCallback(CC_CALLBACK_2(GameRoom::pageViewMoveCallBack, this)));
}

Node* GameRoom::createRoomItem(ComRoomInfo* roomInfo)
{
	auto roomItemNode = CSLoader::createNode(ROOM_ITEM_UI);
	if (nullptr == roomItemNode) return nullptr;
	roomItemNode->setCascadeOpacityEnabled(true);

	auto roomItem = (Button*)roomItemNode->getChildByName("Button_roomItem");
	if (nullptr == roomItem) return nullptr;
	
	roomItem->setUserData(roomInfo);
	roomItem->addTouchEventListener(CC_CALLBACK_2(GameRoom::enterRoomEventCallBack, this));

	char str[128];

	// 房间名称
	sprintf(str, "%s", roomInfo->szGameRoomName);
	auto Text_name = (Text*)Helper::seekWidgetByName(roomItem, "Text_name");
	Text_name->setString(GBKToUtf8(str));

	// 房间人数
	auto Label_PeopleCount = dynamic_cast<Text*>(Helper::seekWidgetByName(roomItem, "Text_PeopleCount"));
	if (nullptr != Label_PeopleCount)
	{
		//sprintf(str, "%d人在玩", roomInfo->uPeopleCount + roomInfo->uVirtualUser);
		sprintf(str, "%d人在玩", roomInfo->uPeopleCount);
		Label_PeopleCount->setString(GBKToUtf8(str));
	}

	// 金币限制
	auto Label_MoneyLimit = dynamic_cast<Text*>(Helper::seekWidgetByName(roomItem, "Text_MoneyLimit"));
	if (nullptr != Label_MoneyLimit)
	{
		if (0 == roomInfo->iLessPoint)
		{
			sprintf(str, "无限制", roomInfo->iLessPoint);
		}
		else if (0 == roomInfo->iMaxPoint)
		{
			sprintf(str, "%d准入", roomInfo->iLessPoint);
		}
		else
		{
			sprintf(str, "%d-%d准入", roomInfo->iLessPoint, roomInfo->iMaxPoint);
		}
		Label_MoneyLimit->setString(GBKToUtf8(str));
	}

	return roomItemNode;
}

void GameRoom::pageViewMoveCallBack(Ref* pSender, PageView::EventType type)
{
	if (_currentPageIdx == _pageViewRooms->getCurPageIndex()) return;
	_currentPageIdx = _pageViewRooms->getCurPageIndex();

	auto iter = _pagesPrompt.begin();
	for (int i = 0; iter != _pagesPrompt.end(); iter++, i++)
	{
		auto page = (Sprite*)*iter;
		page->setColor(Color3B(150, 150, 150));
		page->setScale(0.8f);
		if (i == _currentPageIdx)
		{
			page->setScale(1.0f);
			page->setColor(Color3B(255, 255, 255));
		}
	}
}

//子页面按钮回调
void GameRoom::enterRoomEventCallBack(Ref* pSender, Widget::TouchEventType type)
{
	if (_isTouch == false) return;

	_currentSelectedRoom = dynamic_cast<Button*>(pSender);

	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		_currentSelectedRoom->setColor(Color3B(170, 170, 170));
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		{
			_isTouch = false;
			this->runAction(Sequence::create(DelayTime::create(15.0f), CCCallFunc::create([=]()
			{
				_isTouch = true;
			}), nullptr));

			Size winSize = Director::getInstance()->getWinSize();
			_currentSelectedRoom->setColor(Color3B(255, 255, 255));
			HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);

			ComRoomInfo* roomInfo = static_cast<ComRoomInfo*>(_currentSelectedRoom->getUserData());
			
			// 参数校验
			CCAssert(nullptr != roomInfo, "room is nullptr!");
			if (nullptr == roomInfo)
			{
				_isTouch = true;
				return;
			}

			if(roomInfo->bHasPassword)
			{
				GamePasswordInput* layer = GamePasswordInput::create();
				this->addChild(layer, 1000);
				layer->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
				layer->onEnterCallback = CC_CALLBACK_1(GameRoom::onEnterPasswordClickCallback, this);
				_isTouch = true;
				_roomLogic->start();
			}
			else
			{
				if ((roomInfo->dwRoomRule & GRR_CONTEST) || (roomInfo->dwRoomRule & GRR_TIMINGCONTEST))
				{
					//获取比赛信息
					MSG_GP_GetContestRoomID pData;
					pData.iGameID = roomInfo->uNameID;
					pData.iUserID = PlatformLogic()->loginResult.dwUserID;
					pData.iContestID = roomInfo->iContestID;
					pData.iUpPeople = roomInfo->iUpPeople;

					if (roomInfo->dwRoomRule & GRR_TIMINGCONTEST)
					{
						//获取定时赛开始时间
						PlatformLogic()->sendData(MDM_GP_GET_TIMINGMATCH_TIME, ASS_GP_GET_TIMINGMATCH_TIME, (void*)&pData, sizeof(pData), 
							HN_SOCKET_CALLBACK(GameRoom::contestBeginTime, this));
					}
				
					//初始化报名界面消息
					PlatformLogic()->sendData(MDM_GP_GET_CONTEST_ROOMID, 0, (void*)&pData, sizeof(pData), HN_SOCKET_CALLBACK(GameRoom::contestInit, this));
					return;
				}
				_roomLogic->start();
				_roomLogic->requestLogin(roomInfo->uRoomID);
				_roomID = roomInfo->uRoomID;
			}
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		_currentSelectedRoom->setColor(Color3B(255, 255, 255));
		break;
	default:
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
void GameRoom::onRoomLoginCallback(bool success, const std::string& message, UINT roomID)
{
	if (success)
	{
		HNLOG_WARNING("the user enters a room complete message!");
		
		ComRoomInfo* roomInfo = nullptr;
		if (_fastEnterMatch)
		{
			_fastEnterMatch = false;
			roomInfo = RoomInfoModule()->getByRoomID(getTimeMatchRoomID());
		}
		else
		{
			roomInfo = RoomInfoModule()->getByRoomID(_roomID);
		}
		
		RoomLogic()->setSelectedRoom(roomInfo);
		RoomLogic()->setRoomRule(roomInfo->dwRoomRule);		
			
		if ((RoomLogic()->getRoomRule() & GRR_CONTEST) || (RoomLogic()->getRoomRule() & GRR_TIMINGCONTEST))	// 定时淘汰比赛场
		{
			_roomLogic->stop();
			if (_overdueTime)
			{
				_isTouch = true;
				_overdueTime = false;
				GamePromptLayer::create()->showPrompt(GBKToUtf8("定时赛已过期"));
				RoomLogic()->close();
				return;
			}

			GameMatch::createMatch();
		}
		else if ((RoomLogic()->getRoomRule() & GRR_NOTCHEAT) && !(RoomLogic()->getRoomRule() & GRR_QUEUE_GAME)) //单防作弊
		{
			_roomLogic->stop();
			if (nullptr != onEnterDeskCallBack)
			{
				_isTouch = true;
				onEnterDeskCallBack(RoomLogic()->getSelectedRoom());
			}
		}
		else if (RoomLogic()->getRoomRule() & GRR_QUEUE_GAME)		// 单排队机或者排队机+防作弊
		{
			LoadingLayer::createLoading(this, GBKToUtf8(allocation_table_please_wait_text), 22, LOADING);
			// 进入排队游戏
			_roomLogic->requestJoinQueue();
		}
		else												// 金币场不扣积分
		{
			if (GameCreator()->getCurrentGameType() == HNGameCreator::NORMAL)
			{
				_roomLogic->stop();
				if (nullptr != onRunActionCallBack)
				{
					onRunActionCallBack();
				}
				this->runAction(Sequence::create(FadeOut::create(0.3f), CallFunc::create([&](){
					if (nullptr != onEnterDeskCallBack)
					{
						_isTouch = true;
						onEnterDeskCallBack(RoomLogic()->getSelectedRoom());
					}
					this->removeFromParent();
				}), nullptr));
			}
			else if (GameCreator()->getCurrentGameType() == HNGameCreator::BR)
			{
				LoadingLayer::createLoading(this, GBKToUtf8(enterGame_please_wait_text), 22, LOADING);
				_roomLogic->requestSit(0, 0);
			}
			else if (GameCreator()->getCurrentGameType() == HNGameCreator::SINGLE)
			{
				LoadingLayer::createLoading(this, GBKToUtf8(enterGame_please_wait_text), 22, LOADING);
				
				//_roomLogic->requestQuickSit();

				for (int i = 0; i < roomInfo->uDeskCount; i++)
				{
					std::vector<UserInfoStruct*> deskUsers;
					HNUserInfoModule::getInstance()->findGameUsers(i, deskUsers);
					// 是否锁桌
					BYTE value = RoomLogic()->deskStation.bVirtualDesk[i/8]&(1<<(i%8));
					if (deskUsers.empty() && value == 0)
					{	
						_roomLogic->requestSit(i, 0);
						break;
					}
				}
			}
			else
			{

			}
		}
	}
	else
	{
		_isTouch = true;
		_roomLogic->stop();
		GamePromptLayer::create()->showPrompt(message);
	}
}

void GameRoom::onRoomSitCallback(bool success, const std::string& message, UINT roomID, BYTE deskNo, BYTE seatNo)
{
	LoadingLayer::removeLoading(this);
	_isTouch = true;
	if (success)
	{
		_roomLogic->stop();
		if (INVALID_DESKNO != deskNo && INVALID_DESKSTATION != seatNo)
		{
			// 启动游戏
			bool ret = GameCreator()->startGameClient(RoomLogic()->getSelectedRoom()->uNameID, deskNo, true);
			if (!ret)
			{
				GamePromptLayer::create()->showPrompt(GBKToUtf8("游戏启动失败。"));
			}
		}
	}
	else
	{
		auto prompt = GamePromptLayer::create();
		prompt->showPrompt(GBKToUtf8(message.c_str()));
		prompt->setCallBack([=](){
			if (GameCreator()->getCurrentGameType() == HNGameCreator::SINGLE ||
				GameCreator()->getCurrentGameType() == HNGameCreator::BR)
			{
				RoomLogic()->close();
				_roomLogic->stop();
			}			
		});

	}
}

void GameRoom::onRoomQueueSitCallback(bool success, const std::string& message, UINT roomID, BYTE deskNo)
{
	LoadingLayer::removeLoading(this);
	_roomLogic->stop();
	_isTouch = true;
	if (success)
	{
		if (INVALID_DESKNO != deskNo)
		{
			// 启动游戏
			bool ret = GameCreator()->startGameClient(RoomLogic()->getSelectedRoom()->uNameID, deskNo, true);
			if (!ret)
			{
				GamePromptLayer::create()->showPrompt(GBKToUtf8("游戏启动失败。"));
			}
		}
	}
	else
	{
		auto prompt = GamePromptLayer::create();
		prompt->showPrompt(GBKToUtf8(message.c_str()));
		prompt->setCallBack([=](){
			if (GameCreator()->getCurrentGameType() == HNGameCreator::SINGLE ||
				GameCreator()->getCurrentGameType() == HNGameCreator::BR)
			{
				RoomLogic()->close();
				_roomLogic->stop();
			}
		});
	}
}

void GameRoom::onRoomDisConnect(const std::string &message)
{
	auto prompt = GamePromptLayer::create();
	prompt->showPrompt(message);
	prompt->setCallBack([this](){
		PlatformLogic()->close();
		GameMenu::createMenu();
	});
}

void GameRoom::onPlatformRoomPassEnter(bool success, UINT roomId)
{
	if(success)
	{
		_roomLogic->start();
		_roomLogic->requestLogin(roomId);
	}
	else
	{
		GamePromptLayer::create()->showPrompt(GBKToUtf8("房间密码错误"));
	}	
}

//更新房间人数
void GameRoom::updateRoomPeopleCount(UINT roomID, UINT userCount)
{
	Vector<Layout*> pages = _pageViewRooms->getPages();
	char buffer[64];
	for (auto iter = pages.begin(); iter != pages.end(); ++iter)
	{
		Vector<Node*> childs = (*iter)->getChildren();
		for (auto iter1 = childs.begin(); iter1 != childs.end(); ++iter1)
		{
			auto roomButton = dynamic_cast<Button*>(*iter1);
			auto Label_PeopleCount = dynamic_cast<Text*>(Helper::seekWidgetByName(roomButton, "Text_PeopleCount"));

			if (nullptr != roomButton)
			{
				ComRoomInfo* pRoom = static_cast<ComRoomInfo*>(roomButton->getUserData());
				if (pRoom->uRoomID == roomID)
				{
					pRoom->uPeopleCount = userCount;
					if (nullptr != Label_PeopleCount)
					{
						sprintf(buffer, "%u人", pRoom->uPeopleCount);
						Label_PeopleCount->setString(GBKToUtf8(buffer));
					}
					break;
				}
			}
		}
	}
}

bool GameRoom::JudgeMatchRoom(UINT upPeople, UINT roomRule)
{
	if (!((roomRule & GRR_CONTEST) || (roomRule & GRR_TIMINGCONTEST)))			return false;
	
	if (roomRule & GRR_TIMINGCONTEST)	return false;
	
	for (auto it = _vecContestRoom.begin(); it != _vecContestRoom.end(); it++)
	{
		if (*it == upPeople && *(++it) == roomRule)	return true;
	}

	_vecContestRoom.push_back(upPeople);
	_vecContestRoom.push_back(roomRule);

	return false;
}

bool GameRoom::contestInitError(HNSocketMessage* socketMessage)
{
	GamePromptLayer::create()->showPrompt(GBKToUtf8("比赛已经开始，请进入其它房间！"));
	_isTouch = true;
	RoomLogic()->close();
	return true;
}

bool GameRoom::contestInit(HNSocketMessage* socketMessage)
{
	CCAssert(socketMessage->objectSize == sizeof(DL_GetContestRoomID), "size error.");
	auto pData = (DL_GetContestRoomID*)socketMessage->object;

	_roomLogic->start();
	_roomLogic->requestLogin(pData->iRoomID);
	_roomID = pData->iRoomID;

	UserDefault::getInstance()->setIntegerForKey("iConstestNum", pData->iConstestNum);
	UserDefault::getInstance()->setIntegerForKey("iChampionCount", pData->iChampionCount);
	UserDefault::getInstance()->setIntegerForKey("iBestRank", pData->iBestRank);
	UserDefault::getInstance()->setIntegerForKey("iJoinCount", pData->iJoinCount);
	UserDefault::getInstance()->setIntegerForKey("iEntryFee", pData->iEntryFee);
	UserDefault::getInstance()->setIntegerForKey("iRankAward1", pData->iRankAward[0]);
	UserDefault::getInstance()->setIntegerForKey("iRankAward2", pData->iRankAward[1]);
	UserDefault::getInstance()->setIntegerForKey("iRankAward3", pData->iRankAward[2]);
	UserDefault::getInstance()->setIntegerForKey("iAwardType1", pData->iAwardType[0]);
	UserDefault::getInstance()->setIntegerForKey("iAwardType2", pData->iAwardType[1]);
	UserDefault::getInstance()->setIntegerForKey("iAwardType3", pData->iAwardType[2]);
	UserDefault::getInstance()->flush();
	
	return true;
}

bool GameRoom::contestBeginTime(HNSocketMessage* socketMessage)
{
	CHECK_SOCKET_DATA_RETURN(MSG_GP_GETMatchBeginTime, socketMessage->objectSize, true, "MSG_GP_GETMatchBeginTime size is error");
	auto pData = (MSG_GP_GETMatchBeginTime*)socketMessage->object;

	if (RoomLogic()->getRoomRule() & GRR_CONTEST)			return false;

	cleanTimeMatchData();

	LLONG t = pData->I64BeginTime + 8 * 3600;
	std::string BeginTime;
	while (t)
	{
		char c = t % 10 + '0';
		BeginTime = c + BeginTime;
		t = t / 10;
	}

	time_t bTime = pData->I64BeginTime;
	if (bTime < time(0))	
	{
		char str[64] = { 0 };
		bool isJoined = false;
		for (int i = 1; i < 10; i++)
		{
			sprintf(str, "MARK_%d", i);
			if (UserDefault::getInstance()->getIntegerForKey(str) == 5)
			{
				sprintf(str, "beginTime_%d", i);
				if (UserDefault::getInstance()->getStringForKey(str) == BeginTime)
				{
					isJoined = true;
					break;
				}
			}
		}

		if (isJoined)
		{
			UserDefault::getInstance()->setStringForKey("DXCL", BeginTime);
			UserDefault::getInstance()->flush();
			return true;
		}
		else
		{
			_overdueTime = true;
			return false;
		}		
	}

	char mark[64] = {0};
	std::string name = "";
	for (int i = 1; i < 10; i++)
	{
		sprintf(mark, "MARK_%d", i);
		if (UserDefault::getInstance()->getIntegerForKey(mark) == 0)
		{			
			UserDefault::getInstance()->setIntegerForKey(mark, 1);
			name = "beginTime_";
			name += (i+'0');
			break;
		}
	}
	UserDefault::getInstance()->setStringForKey(name.c_str(), BeginTime);
	UserDefault::getInstance()->flush();
	return true;
}

void GameRoom::cleanTimeMatchData()
{
	char str[64] = {0};
	for (int i = 1; i < 10; i++)
	{
		sprintf(str, "MARK_%d", i);
		if (UserDefault::getInstance()->getIntegerForKey(str) == 1 || UserDefault::getInstance()->getIntegerForKey(str) == 4)		//没有参赛的数据
		{
			UserDefault::getInstance()->setIntegerForKey(str, 0);
			sprintf(str, "beginTime_%d", i);
			UserDefault::getInstance()->setStringForKey(str, "");
			continue;
		}

		sprintf(str, "beginTime_%d", i);
		std::string beginTime = UserDefault::getInstance()->getStringForKey(str);
		time_t bTime = 0;
		for (auto iter = beginTime.begin(); iter != beginTime.end(); iter++)
		{
			bTime = bTime*10 + *iter - '0';
		}
		time_t nTime = time(0);
		if (nTime + 7*3600 > bTime)			//开赛时间过期超过1小时（留时间断线重连）
		{
			UserDefault::getInstance()->setStringForKey(str, "");
			sprintf(str, "MARK_%d", i);
			UserDefault::getInstance()->setIntegerForKey(str, 0);
		}
	}
	UserDefault::getInstance()->setStringForKey("DXCL", "");
	UserDefault::getInstance()->flush();
}

void GameRoom::fastEnterTimeMatch(UINT roomID)
{
	_roomLogic->start();
	_roomLogic->requestLogin(roomID);
}

UINT GameRoom::getTimeMatchRoomID()
{
	char str[64] = {0};
	for (int i = 0; i < 10; i++)
	{
		sprintf(str, "MARK_%d", i);
		if (UserDefault::getInstance()->getIntegerForKey(str) == 3)
		{
			sprintf(str, "RoomID_%d", i);
			UINT roomID = UserDefault::getInstance()->getIntegerForKey(str);			
			_fastEnterMatch = true;
			return roomID;
		}
	}
	return 255;
}