#include "HNLobby/GameDesk.h"
#include "HNLobby/FontConfig.h"
#include "HNLobby/PlatformDefine.h"
#include "HNLobby/GamePrompt.h"
#include "HNLobby/PlatformConfig.h"
#include "HNLobby/ChildLayer/GamePasswordInput.h"
#include "HNUIExport.h"

//////////////////////////////////////////////////////////////////////////
static const char* DESK_ITEM_UI = "platform/DeskList/deskListUi.csb";

static const char* GAME_PAGESPROMPT_PATH = "platform/gameList/gameItemRes/page_Prompt.png";


static const Size pageViewDesksSize(1280, 500);

//////////////////////////////////////////////////////////////////////////

GameDesk* GameDesk::createDesk(ComRoomInfo* roomInfo)
{
	GameDesk* deskList = new GameDesk();
	if (deskList->initData(roomInfo))
	{
		deskList->autorelease();
		return deskList;
	}
	else
	{
		CC_SAFE_DELETE(deskList);
		return nullptr;
	}
}

GameDesk::GameDesk() 
	: _currentSelectedDesk(nullptr)
	, _deskNO (0)
	, _pageLen (0)
	, _canCreate (true)
	, _pageEven (false)
	, _currentPageIdx (0)
	, _isTouch (true)
{
	_deskLogic = new HNRoomDeskLogic(this);
	_deskLogic->start();
}

GameDesk::~GameDesk()
{
	_deskLogic->stop();
	HN_SAFE_DELETE(_deskLogic);

	for (auto desk : _deskinfos)
	{
		HN_SAFE_DELETE(desk);
	}
}

void GameDesk::closeDeskList()
{
	_deskLogic->stop();
	RoomLogic()->close();
	this->runAction(Sequence::create(FadeOut::create(0.5f), CallFunc::create([&](){
		if (nullptr != onCloseCallBack)
		{
			onCloseCallBack();
		}
		this->removeFromParent();
	}), nullptr));
}

void GameDesk::onEnterPasswordCallback(const std::string& password)
{
	DeskInfo* deskInfo = static_cast<DeskInfo*>(_currentSelectedDesk->getUserData());
	_deskLogic->requestSit(deskInfo->deskID, password);
}

bool GameDesk::initData(ComRoomInfo* roomInfo)
{
	if (!HNLayer::init()) return false;

	auto winSize = Director::getInstance()->getWinSize();

	this->setCascadeOpacityEnabled(true);

	createDeskList(roomInfo);

	return true;
}

void GameDesk::onExit()
{
	_deskLogic->stop();
	HNLayer::onExit();
}

// 创建牌桌列表
void GameDesk::createDeskList(ComRoomInfo* roomInfo)
{
	bool isFull = true;
	if (RoomLogic()->getRoomRule() & GRR_NOTCHEAT)
	{
		//防作弊场，跳过桌子界面
		int count = roomInfo->uDeskCount;

		for (int num = roomInfo->uDeskPeople - 1; num >= 0; num--)//遍历人数
		{
			for (int i = 0; i < count; i++)//遍历桌子
			{
				// 获取桌子玩家信息
				std::vector<UserInfoStruct*> deskUsers;
				UserInfoModule()->findDeskUsers(i, deskUsers);
				//桌上已有人数
				int peopleCount = deskUsers.size();
				//判断是否封桌
				BYTE value = RoomLogic()->deskStation.bVirtualDesk[i / 8] & (1 << (i % 8));
				if (0 != value)
				{
					//如果是封桌，此桌人数为人数上限
					peopleCount = roomInfo->uDeskPeople;
				}
				//如果此桌人数未满
				if (peopleCount == num)
				{
					_deskLogic->requestSit(i);
					isFull = false;
					break;
				}
			}
			if (!isFull)
				break;
		}
		if (isFull)
		{
			auto prompt = GamePromptLayer::create();
			prompt->showPrompt(GBKToUtf8("房间满员！"));

			if (RoomLogic()->getRoomRule() & GRR_NOTCHEAT)
			{
				prompt->setCallBack([=](){
					_deskLogic->stop();
					RoomLogic()->close();
					removeFromParent();
				});
			}
		}
		return;
	}

	_roomInfo = roomInfo;
	auto winSize = Director::getInstance()->getWinSize();

	// 桌子列表
	_pageViewDesks = PageView::create();
	_pageViewDesks->setContentSize(pageViewDesksSize);
	_pageViewDesks->setAnchorPoint(Vec2(0.5f, 0.5f));
	_pageViewDesks->setPosition(Vec2(winSize.width * 0.5f, winSize.height * 0.5f));
	_pageViewDesks->setTouchEnabled(true);
	_pageViewDesks->setCustomScrollThreshold(15);
	_pageViewDesks->addEventListener(PageView::ccPageViewCallback(CC_CALLBACK_2(GameDesk::pageViewMoveCallBack, this)));
	addChild(_pageViewDesks, 3);

	_pageViewDesks->setCascadeOpacityEnabled(true);

	_pageEven = ((_roomInfo->uDeskCount % 6) == 0);
	_pageLen = _roomInfo->uDeskCount / 6;

	int pageCount = _pageEven ? _pageLen : _pageLen + 1;
	int central = -pageCount / 2;
	if (abs(central) > 0)
	{
		for (int i = 0; i < pageCount; i++)
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

	createDeskPage();
}

void GameDesk::createDeskPage()
{
	if (!_canCreate) return;

	if (_pageLen > 0)
	{
		_deskinfos.clear();
		for (int m = 0; m < 6; m++)
		{
			createDeskPageInfo(_roomInfo);
			_deskNO++;
		}
		createDeskListPage(_deskinfos);
		_pageLen--;
		if (_pageEven && 0 == _pageLen)
		{
			_canCreate = false;
		}
	}

	if (0 == _pageLen && !_pageEven)
	{
		_deskinfos.clear();
		INT num = _roomInfo->uDeskCount % 6;
		for (int i = 0; i < num; i++)
		{
			createDeskPageInfo(_roomInfo);
			_deskNO++;
		}
		createDeskListPage(_deskinfos);
		_canCreate = false;
	}
	_deskinfos.clear();

	this->runAction(Sequence::create(DelayTime::create(0.4f), CallFunc::create([this](){
		createDeskPage();
	}), nullptr));
}

// 创建牌桌页面数据
void GameDesk::createDeskPageInfo(ComRoomInfo* roomInfo)
{
	// 获取桌子玩家信息
	std::vector<UserInfoStruct*> deskUsers;
	UserInfoModule()->findDeskUsers(_deskNO, deskUsers);

	DeskInfo* desk = new DeskInfo;
	memset(desk, 0x0, sizeof(desk));

	if (nullptr != desk)
	{
		desk->deskID				= _deskNO;
		desk->multiple			= roomInfo->iBasePoint;
		desk->goldMin			= roomInfo->iLessPoint;
		desk->goldMax			= roomInfo->iMaxPoint;
		desk->peopleMax			= roomInfo->uDeskPeople;

		// 虚拟桌子（封桌）
		BYTE isVirtual = ((RoomLogic()->deskStation.bVirtualDesk[desk->deskID / 8]) & (1 << (desk->deskID % 8)) != 0);
		desk->peopleValue = isVirtual ? roomInfo->uDeskPeople : deskUsers.size();

		// 锁桌（加密）
		BYTE isLocked = RoomLogic()->deskStation.bDeskLock[desk->deskID / 8] & (1 << (desk->deskID % 8));
		desk->isLocked = (isLocked != 0);
	}
	_deskinfos.push_back(desk);
	_allDeskInfo.push_back(desk);
}

// 创建牌桌列表page
void GameDesk::createDeskListPage(std::vector<DeskInfo*> deskinfos)
{
	//创建房间列表子页面
	auto deskListLayout = Layout::create();
	deskListLayout->setName("page");
	deskListLayout->setContentSize(pageViewDesksSize);
	deskListLayout->setPassFocusToChild(true);

	float pageWidth = _pageViewDesks->getContentSize().width;
	float pageHeight = _pageViewDesks->getContentSize().height;

	deskListLayout->setCascadeOpacityEnabled(true);

	int idx = 0;
	for (auto desk : deskinfos)
	{
		idx++;
		auto deskNode = addDesk(desk);
		auto button = deskNode->getChildByName("Button_deskItem");
		button->removeFromParentAndCleanup(false);

		float posX = idx % 2 ? 0.25f : 0.75f;
		float posY = 0.0f;
		if (idx <= 2) posY = 0.8f;
		else if (idx > 2 && idx <= 4) posY = 0.5f;
		else posY = 0.2f;

		button->setPosition(Vec2(pageWidth * posX, pageHeight * posY));
		deskListLayout->addChild(button);
	}

	// 添加子页面进入列表中
	_pageViewDesks->addPage(deskListLayout);
}

// 添加桌子
Node* GameDesk::addDesk(DeskInfo* deskInfo)
{
	auto deskItemNode = CSLoader::createNode(DESK_ITEM_UI);
	if (nullptr == deskItemNode) return nullptr;
	deskItemNode->setCascadeOpacityEnabled(true);

	auto deskItem = (Button*)deskItemNode->getChildByName("Button_deskItem");
	if (nullptr == deskItem) return nullptr;

	deskItem->setScale(0.9f);
	deskItem->setUserData(deskInfo);
	deskItem->setPropagateTouchEvents(true);
	deskItem->addTouchEventListener(CC_CALLBACK_2(GameDesk::enterTableEventCallBack, this));

	char str[128];

	// 桌名
	sprintf(str, "%d号桌", deskInfo->deskID + 1);
	auto Text_deskName = (Text*)Helper::seekWidgetByName(deskItem, "Text_deskName");
	Text_deskName->setString(GBKToUtf8(str));

	// 桌上人数
	sprintf(str, "%d / %d", deskInfo->peopleValue, deskInfo->peopleMax);
	auto Label_people = (Text*)Helper::seekWidgetByName(deskItem, "Text_count");
	Label_people->setString(str);

	// 人数进度条
	float count =  deskInfo->peopleValue * 100.f / deskInfo->peopleMax;
	auto progress = (LoadingBar*)Helper::seekWidgetByName(deskItem, "LoadingBar_count");
	progress->setPercent(count);

	// 房间人数
	auto Label_PeopleCount = (Text*)Helper::seekWidgetByName(deskItem, "Text_people");
	sprintf(str, "%d人在玩", deskInfo->peopleValue);
	Label_PeopleCount->setString(GBKToUtf8(str));

	// 金币限制
	auto Label_MoneyLimit = (Text*)Helper::seekWidgetByName(deskItem, "Text_Limit");
	if (nullptr != Label_MoneyLimit)
	{
		if (0 == deskInfo->goldMin)
		{
			sprintf(str, "无限制", deskInfo->goldMin);
		}
		else if (0 == deskInfo->goldMax)
		{
			sprintf(str, "%d准入", deskInfo->goldMin);
		}
		else
		{
			sprintf(str, "%d-%d准入", deskInfo->goldMin, deskInfo->goldMin);
		}
		Label_MoneyLimit->setString(GBKToUtf8(str));
	}
	return deskItemNode;
}

// 进入游戏桌点击回调
void GameDesk::enterTableEventCallBack(Ref* pSender, Widget::TouchEventType type)
{
	if (_isTouch == false) return;

	_currentSelectedDesk = dynamic_cast<Button*>(pSender);

	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		_currentSelectedDesk->setColor(Color3B(170, 170, 170));
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
			_currentSelectedDesk->setColor(Color3B(255, 255, 255));
			HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);

			DeskInfo* deskInfo = static_cast<DeskInfo*>(_currentSelectedDesk->getUserData());

			// 参数校验
			CCAssert(nullptr != deskInfo, "desk is nullptr!");
			if (nullptr == deskInfo)
			{
				_isTouch = true;
				return;
			}

			if (deskInfo->peopleValue == deskInfo->peopleMax)
			{
				GamePromptLayer::create()->showPrompt(GBKToUtf8("桌子人数已满。"));
				_isTouch = true;
				return;
			}

			// 密码桌子
			if(deskInfo->isLocked)
			{
				GamePasswordInput* layer = GamePasswordInput::create();
				layer->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
				this->addChild(layer, 1000);
				layer->onEnterCallback = CC_CALLBACK_1(GameDesk::onEnterPasswordCallback, this);
				_isTouch = true;
				return;
			}

			_deskLogic->requestSit(deskInfo->deskID);
			//_deskLogic->requestQuickSit();
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		_currentSelectedDesk->setColor(Color3B(255, 255, 255));
		break;
	default:
		break;
	}
}

void GameDesk::pageViewMoveCallBack(Ref* pSender, PageView::EventType type)
{
	if (_currentPageIdx == _pageViewDesks->getCurPageIndex()) return;
	_currentPageIdx = _pageViewDesks->getCurPageIndex();

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

void GameDesk::updateLockDesk()
{
	for(auto &deskInfo: _allDeskInfo)
	{
		BYTE isLocked = RoomLogic()->deskStation.bDeskLock[deskInfo->deskID / 8] & (1 << (deskInfo->deskID % 8));
		deskInfo->isLocked = (isLocked != 0);
	}
}

void GameDesk::onRoomSitCallback(bool success, const std::string& message, UINT roomID, BYTE deskNo, BYTE seatNo)
{
	_isTouch = true;
	if(success)
	{
		ComRoomInfo* pRoomInfo = RoomLogic()->getSelectedRoom();
		if(nullptr == pRoomInfo)
		{
			auto prompt = GamePromptLayer::create();
			prompt->showPrompt(GBKToUtf8("查找房间失败。"));

			if (RoomLogic()->getRoomRule() & GRR_NOTCHEAT)
			{
				prompt->setCallBack([=](){
					_deskLogic->stop();
					RoomLogic()->close();
					removeFromParent();
				});
			}
			return;
		}
		bool bRet = GameCreator()->startGameClient(pRoomInfo->uNameID, deskNo, true);
		if (!bRet)
		{
			auto prompt = GamePromptLayer::create();
			prompt->showPrompt(GBKToUtf8("游戏启动失败。"));

			if (RoomLogic()->getRoomRule() & GRR_NOTCHEAT)
			{
				prompt->setCallBack([=](){
					_deskLogic->stop();
					RoomLogic()->close();
					removeFromParent();
				});
			}
		}
	}
	else
	{
		auto prompt = GamePromptLayer::create();
		prompt->showPrompt(message);

		if (RoomLogic()->getRoomRule() & GRR_NOTCHEAT)
		{
			prompt->setCallBack([=](){
				_deskLogic->stop();
				RoomLogic()->close();
				removeFromParent();
			});
		}
	}
}

void GameDesk::onRoomDeskLock(BYTE deskNo)
{
	updateDeskPeopleCount(deskNo, true);
}

void GameDesk::onRoomDeskUnLock(BYTE deskNo)
{
	updateDeskPeopleCount(deskNo, false);
}

void GameDesk::onRoomDeskUserCountChanged()
{
	updateLockDesk();
	updateDeskPeopleCount(INVALID_DESKNO, false);
}

//更新游戏人数
void GameDesk::updateDeskPeopleCount(BYTE deskNo, bool isLock)
{
	Vector<Layout*> pages = _pageViewDesks->getPages();

	char buffer[64];
	for (auto iter = pages.begin(); iter != pages.end(); ++iter)
	{
		Vector<Node*> childs = (*iter)->getChildren();
		for (auto iter1 = childs.begin(); iter1 != childs.end(); ++iter1)
		{
			auto gameButton = dynamic_cast<Button*>(*iter1);
			if (nullptr != gameButton)
			{
				DeskInfo* info = static_cast<DeskInfo*>(gameButton->getUserData());

				BYTE value = RoomLogic()->deskStation.bVirtualDesk[info->deskID/8]&(1<<(info->deskID%8));

				if (info->deskID == deskNo && isLock)
				{
					info->peopleValue = info->peopleMax;
				}
				else if (0 != value)
				{
					info->peopleValue	= info->peopleMax;
				}
				else
				{
					std::vector<UserInfoStruct*> deskUsers;
					UserInfoModule()->findDeskUsers(info->deskID, deskUsers);
					info->peopleValue = deskUsers.size();
				}

				// 桌上人数
				auto Label_people = (Text*)Helper::seekWidgetByName(gameButton, "Text_count");
				sprintf(buffer, "%d / %d", info->peopleValue, info->peopleMax);
				Label_people->setString(buffer);

				// 人数进度条
				auto progress = (LoadingBar*)Helper::seekWidgetByName(gameButton, "LoadingBar_count");
				float count =  info->peopleValue * 100.f / info->peopleMax;
				progress->setPercent(count);

				// 房间人数
				auto Label_PeopleCount = (Text*)Helper::seekWidgetByName(gameButton, "Text_people");
				sprintf(buffer, "%d人在玩", info->peopleValue);
				Label_PeopleCount->setString(GBKToUtf8(buffer));
			}
		}
	}
}