#include "HNLobby/GamePlatform.h"
#include "HNLobby/GameRoom.h"
#include "HNLobby/GameDesk.h"
#include "HNLobby/GameLists.h"
#include "HNLobby/GameNotice.h"
#include "HNLobby/GameExitLayer.h"
#include "HNLobby/FontConfig.h"
#include "HNLobby/PlatformDefine.h"
#include "HNLobby/PlatformConfig.h"
#include "HNLobby/GamePrompt.h"
#include "HNLobby/GameMenu/GameMenu.h"
#include "HNLobby/ChildLayer/GameSetLayer.h"
#include "HNLobby/ChildLayer/GameExitChangeLayer.h"
#include "HNLobby/ChildLayer/GameRankingList.h"
#include "HNLobby/ChildLayer/GameSignLayer.h"
#include "HNLobby/ChildLayer/GameOnlineReward.h"
#include "HNLobby/ChildLayer/GameRechargeRebate.h"
#include "HNLobby/ChildLayer/ServiceLayer.h"
#include "HNLobby/ChildLayer/GameBankLayer.h"
#include "HNLobby/ChildLayer/NoticeList.h"
#include "HNLobby/ChildLayer/GameGiftShop.h"
#include "HNLobby/PersionalCenter/BindPhone.h"
#include "HNLobby/PersionalCenter/ModifyPassword.h"
#include "HNLobby/PersionalCenter/GameUserDataLayer.h"
#include "HNLobby/Popularise/SpreadEditLayer.h"

#include "HNUIExport.h"
#include "HNOpenExport.h"
#include <string>
#include "json/stringbuffer.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include <time.h>

USING_NS_UM_SOCIAL;
using namespace network;



// 说明：
//所有的层，节点的tag标签不能重复
/////////////////////////////////////////////////////////////////////////
static const int PLATFORM_ZORDER_UI			= 100;
static const int PLATFORM_UI_ZORDER_TOP		= PLATFORM_ZORDER_UI + 1;		//
static const int PLATFORM_UI_ZORDER_BOTTOM	= PLATFORM_ZORDER_UI + 1;		//
static const int PLATFORM_UI_ZORDER_NOTICE	= PLATFORM_ZORDER_UI + 1;		//
static const int PLATFORM_UI_ZORDER_POPULARISE	= PLATFORM_ZORDER_UI + 5;	//推广
static const int ROOM_LAYER_ZORDER			= PLATFORM_ZORDER_UI + 2;		// 房间列表层级
static const int GAMELIST_LAYER_ZORDER		= PLATFORM_ZORDER_UI + 2;		// 游戏列表层级
static const int DESKLIST_LAYER_ZORDER		= PLATFORM_ZORDER_UI + 3;		// 桌子列表层级
static const int CHILD_LAYER_ZORDER			= PLATFORM_ZORDER_UI + 4;		// 子节点弹出框层级
//////////////////////////////////////////////////////////////////////////
static const int CHILD_LAYER_TAG			= 1000;
static const int CHILD_NEXTLAYER_TAG		= 100;
static const int CHILD_LAYER_USERHEAD_TAG	= CHILD_LAYER_TAG + 1;			// 图像层标签
static const int CHILD_LAYER_STORE_TAG		= CHILD_LAYER_TAG + 2;			// 商店层标签
static const int CHILD_LAYER_SET_TAG		= CHILD_LAYER_TAG + 3;			// 设置层标签
static const int CHILD_LAYER_BANK_TAG		= CHILD_LAYER_TAG + 4;			// 银行层标签
static const int CHILD_LAYER_SERVICE_TAG	= CHILD_LAYER_TAG + 5;			// 服务层标签
static const int CHILD_LAYER_ROOM_TAG		= CHILD_LAYER_TAG + 6;			// 房间层标签
static const int CHILD_LAYER_GAMES_TAG		= CHILD_LAYER_TAG + 7;			// 游戏层标签
static const int CHILD_LAYER_DESKS_TAG		= CHILD_LAYER_TAG + 8;			// 桌子层标签
static const int CHILD_LAYER_EXIT_TAG		= CHILD_LAYER_TAG + 9;			// 退出层标签
static const int CHILD_LAYER_RANK_TAG		= CHILD_LAYER_TAG + 12;			// 排行榜层标签
static const int CHILD_LAYER_SIGN_TAG		= CHILD_LAYER_TAG + 13;			// 签到层标签
static const int CHILD_LAYER_ONLINE_TAG		= CHILD_LAYER_TAG + 14;			// 在线奖励层标签
static const int CHILD_LAYER_REBATE_TAG		= CHILD_LAYER_TAG + 15;			// 充值返利层标签
static const int CHILD_LAYER_EXITCHANGE_TAG	= CHILD_LAYER_TAG + 16;		    // 退出选择层标签
static const int CHILD_LAYER_PASS_TAG		= CHILD_NEXTLAYER_TAG;			// 修改密码层标签
static const int CHILD_LAYER_PHONE_TAG		= CHILD_NEXTLAYER_TAG + 1;		// 绑定手机层标签


//////////////////////////////////////////////////////////////////////////
static const float request_game_room_timer_time	= 0.5f;
//////////////////////////////////////////////////////////////////////////
static const char* PLATFORM_BG				= "platform/hallBg.png";   
static const char* DECORATION_LEFT			= "platform/subViewBambooLeft.png";
static const char* DECORATION_RIGHT			= "platform/subViewBambooRight.png";

static const char* USER_HEAD_FRAME			= "platform/head/user_head_frame.png";
static const char* USER_MEN_HEAD			= "platform/head/men_head.png";					
static const char* USER_WOMEN_HEAD			= "platform/head/women_head.png";

static const char* PLATFORM_NOTIC_BG		= "platform/notice/MsgManager_BG.png";
static const char* PLATFORM_NOTIC_SPEAKER	= "platform/notice/Speaker.ExportJson";
static const char*  Notice_Bg_Stencil		= "platform/notice/Msg_BG.png";

//////////////////////////////////////////////////////////////////////////

static const char* PLATFORMUI_BOTTOM_CSB	= "platform/lobbyUi/PlatformUi_BOTTOM/PlatformUi_BOTTOM.csb";
static const char* PLATFORMUI_TOP_CSB		= "platform/lobbyUi/PlatformUi_TOP/PlatformUi_TOP.csb";



//////////////////////////////////////////////////////////////////////////

void GamePlatform::createPlatform()
{
    auto scene = Scene::create();
    auto mainlayer = GamePlatform::create();
	mainlayer->requestGameRoomTimerCallBack(0.1f);
    scene->addChild(mainlayer);
	Director::getInstance()->replaceScene(TransitionSlideInR::create(0.3f, scene));

	auto userDefault = UserDefault::getInstance();
	userDefault->setIntegerForKey("gamePageIdx", 0);
	userDefault->flush();
}

void GamePlatform::returnPlatform(LayerType type)
{
	auto scene = Scene::create();
	auto mainlayer = GamePlatform::create();
	mainlayer->createLayerChoose(type);
	scene->addChild(mainlayer);
	//Director::getInstance()->replaceScene(TransitionSlideInL::create(0.3f, scene));
	Director::getInstance()->replaceScene(scene);
}

void GamePlatform::createLayerChoose(LayerType type)
{
	_lobbyTop.UserHead_Head->setVisible(false);
	_lobbyTop.Button_Return->setVisible(true);

	switch (type)
	{
	case ROOMLIST:
		{
			_roomListLogic->start();
			_roomListLogic->requestRoomList();
			if (GameCreator()->getRegistGameCount() <= 1)
			{
				_lobbyTop.UserHead_Head->setVisible(true);
				_lobbyTop.Button_Return->setVisible(false);
			}		
		}
		break;
	case DESKLIST:
		createDeskListLayer(RoomLogic()->getSelectedRoom());
		break;
	default:
		break;
	}
}

GamePlatform::GamePlatform()
	: _Time (0)
	, _isTouch (true)
	, _isCustomer (false)
	,_iValue(-1)
{
	memset(&_lobbyBotttom, 0x0, sizeof(_lobbyBotttom));
	memset(&_lobbyTop, 0x0, sizeof(_lobbyTop));
	_roomListLogic = new HNPlatformRoomList(this);
	_gameListLogic = new HNPlatformGameList(this);
}

GamePlatform::~GamePlatform()
{
	HNHttpRequest::getInstance()->removeObserver(this);

	HN_SAFE_DELETE(_roomListLogic);
	HN_SAFE_DELETE(_gameListLogic);
}


void GamePlatform::plantSeed()
{
	timeval tv;
	gettimeofday(&tv, NULL);
	unsigned long reed = tv.tv_sec*1000 + tv.tv_usec/1000;
	
	//srand()中传入一个随机数种子
	srand(reed);
}

bool GamePlatform::init()
{
    if ( !HNLayer::init())
	{
		return false;
	}

	enableKeyboard();

	_spreadLogic.init(this);

	// 背景图
	setBackGroundImage(PLATFORM_BG);

	plantSeed();

	ArmatureDataManager::getInstance()->addArmatureFileInfo(PLATFORM_NOTIC_SPEAKER);//喇叭动画

	Size winSize = Director::getInstance()->getWinSize();

	// 玩家信息结构体
	MSG_GP_R_LogonResult& LogonResult = PlatformLogic()->loginResult;

	float _xScale = winSize.width / PlatformConfig::getInstance()->getPlatformDesignSize().width;
	float _yScale = winSize.height / PlatformConfig::getInstance()->getPlatformDesignSize().height;

	_Decoration_left = Sprite::create(DECORATION_LEFT);
	_Decoration_left->setAnchorPoint(Vec2(0.0f, 1.0f));
	_Decoration_left->setPosition(Vec2(0.f, winSize.height));
	addChild(_Decoration_left, 1);

	_Decoration_right = Sprite::create(DECORATION_RIGHT);
	_Decoration_right->setAnchorPoint(Vec2(1.0f, 1.0f));
	_Decoration_right->setPosition(Vec2(winSize.width, winSize.height));
	addChild(_Decoration_right, 1);

	_lobbyTop.node = CSLoader::createNode(PLATFORMUI_TOP_CSB);
	_lobbyTop.layout = (Layout*)_lobbyTop.node->getChildByName("Panel_top");

	if (nullptr != _lobbyTop.layout)
	{
		_lobbyTop.node->setPosition(Vec2(winSize.width / 2, winSize.height));
		addChild(_lobbyTop.node, PLATFORM_UI_ZORDER_TOP);

		_lobbyTop.UserHead_Head = UserHead::create(USER_HEAD_FRAME);
		_lobbyTop.UserHead_Head->setScale(1.8f, 1.8f);
		//_lobbyTop.UserHead_Head->setVisible(false);
		if (switchToLayoutControl(_lobbyTop.layout, "Text_headPos", _lobbyTop.UserHead_Head))
		{
			_lobbyTop.UserHead_Head->setUserHead(LogonResult.bBoy ? USER_MEN_HEAD : USER_WOMEN_HEAD);
			_lobbyTop.UserHead_Head->onHeadClickEvent = [this](UserHead* sender, Vec2 touchVec)
			{
				GameUserDataLayer* userDataLayer = dynamic_cast<GameUserDataLayer*>(this->getChildByTag(CHILD_LAYER_USERHEAD_TAG));
				if (nullptr == userDataLayer)
				{
					userDataLayer = GameUserDataLayer::create();
					userDataLayer->showUserInfo(this, CHILD_LAYER_ZORDER, CHILD_LAYER_USERHEAD_TAG);
				}
			};
		}
		

		// 用户昵称
		_lobbyTop.Label_UserName = (Text*)Helper::seekWidgetByName(_lobbyTop.layout, "Text_nickName");
		std::string nickName(LogonResult.nickName);
		if (!nickName.empty())
		{
			_lobbyTop.Label_UserName->setString(GBKToUtf8(LogonResult.nickName));
		}
		else
		{
			_lobbyTop.Label_UserName->setString(GBKToUtf8("未知"));
		}
		// 用户金币
		_lobbyTop.Label_UserMoney = (Text*)Helper::seekWidgetByName(_lobbyTop.layout, "Text_money");
		char buffer[64] = {0};
		sprintf(buffer, "%lld", LogonResult.i64Money);
		_lobbyTop.Label_UserMoney->setString(buffer);

		// 返回
		_lobbyTop.Button_Return = (Button*)Helper::seekWidgetByName(_lobbyTop.layout, "Button_return");
		_lobbyTop.Button_Return->addTouchEventListener(CC_CALLBACK_2(GamePlatform::menuLobbyTopCallBack, this));
		_lobbyTop.Button_Return->setVisible(true);

		// 公告
		_lobbyTop.Button_notice = (Button*)Helper::seekWidgetByName(_lobbyTop.layout, "Button_notice");
		_lobbyTop.Button_notice->addTouchEventListener(CC_CALLBACK_2(GamePlatform::menuLobbyTopCallBack, this));
		_lobbyTop.Button_notice->setVisible(true);

		// 签到
		_lobbyTop.Button_sign = (Button*)Helper::seekWidgetByName(_lobbyTop.layout, "Button_sign");
		_lobbyTop.Button_sign->addTouchEventListener(CC_CALLBACK_2(GamePlatform::menuLobbyTopCallBack, this));
		_lobbyTop.Button_sign->setVisible(true);

		// 退出
		_lobbyTop.Button_exit = (Button*)Helper::seekWidgetByName(_lobbyTop.layout, "Button_exit");
		_lobbyTop.Button_exit->addTouchEventListener(CC_CALLBACK_2(GamePlatform::menuLobbyTopCallBack, this));
		_lobbyTop.Button_exit->setVisible(true);
		// 客服
		_lobbyTop.Button_service = (Button*)Helper::seekWidgetByName(_lobbyTop.layout, "Button_service");
		_lobbyTop.Button_service->addTouchEventListener(CC_CALLBACK_2(GamePlatform::menuLobbyTopCallBack, this));
		_lobbyTop.Button_service->setVisible(true);

		// 设置
		_lobbyTop.Button_setting = (Button*)Helper::seekWidgetByName(_lobbyTop.layout, "Button_set");
		_lobbyTop.Button_setting->addTouchEventListener(CC_CALLBACK_2(GamePlatform::menuLobbyTopCallBack, this));	
		_lobbyTop.Button_setting->setVisible(true);
	
		// 推广
		_lobbyTop.Button_popularize = (Button*)Helper::seekWidgetByName(_lobbyTop.layout, "Button_popularize");
		_lobbyTop.Button_popularize->addTouchEventListener(CC_CALLBACK_2(GamePlatform::menuLobbyTopCallBack, this));
		_lobbyTop.Button_popularize->setVisible(true);

		// 兑换
		_lobbyTop.Button_exchange = (Button*)Helper::seekWidgetByName(_lobbyTop.layout, "Button_exchange");
		_lobbyTop.Button_exchange->addTouchEventListener(CC_CALLBACK_2(GamePlatform::menuLobbyTopCallBack, this));
		_lobbyTop.Button_exchange->setVisible(true);
	}

	//////////////////////////////////////////////////////////////////////////////////
	

	_lobbyBotttom.node = CSLoader::createNode(PLATFORMUI_BOTTOM_CSB);
	_lobbyBotttom.layout = (Layout*)_lobbyBotttom.node->getChildByName("Panel_bottom");

	if (nullptr != _lobbyBotttom.layout)
	{
		_lobbyBotttom.node->setPosition(Vec2(winSize.width / 2, 0.0f));
		addChild(_lobbyBotttom.node, PLATFORM_UI_ZORDER_BOTTOM);

		_lobbyBotttom.Button_Friend = (Button*)Helper::seekWidgetByName(_lobbyBotttom.layout, "Button_friend");
		_lobbyBotttom.Button_Friend->addTouchEventListener(CC_CALLBACK_2(GamePlatform::menuLobbyBottomCallBack, this));
		_lobbyBotttom.Button_Friend->setVisible(true);

		_lobbyBotttom.Button_Task = (Button*)Helper::seekWidgetByName(_lobbyBotttom.layout, "Button_task");
		_lobbyBotttom.Button_Task->addTouchEventListener(CC_CALLBACK_2(GamePlatform::menuLobbyBottomCallBack, this));
		_lobbyBotttom.Button_Task->setVisible(true);

		_lobbyBotttom.Button_Activity = (Button*)Helper::seekWidgetByName(_lobbyBotttom.layout, "Button_activity");
		_lobbyBotttom.Button_Activity->addTouchEventListener(CC_CALLBACK_2(GamePlatform::menuLobbyBottomCallBack, this));
		_lobbyBotttom.Button_Activity->setVisible(true);

		_lobbyBotttom.Button_Store = (Button*)Helper::seekWidgetByName(_lobbyBotttom.layout, "Button_store");
		_lobbyBotttom.Button_Store->addTouchEventListener(CC_CALLBACK_2(GamePlatform::menuLobbyBottomCallBack, this));
		_lobbyBotttom.Button_Store->setVisible(true);

		_lobbyBotttom.Button_Rank = (Button*)Helper::seekWidgetByName(_lobbyBotttom.layout, "Button_rank");
		_lobbyBotttom.Button_Rank->addTouchEventListener(CC_CALLBACK_2(GamePlatform::menuLobbyBottomCallBack, this));
		_lobbyBotttom.Button_Rank->setVisible(true);

		_lobbyBotttom.Button_Bank = (Button*)Helper::seekWidgetByName(_lobbyBotttom.layout, "Button_bank");
		_lobbyBotttom.Button_Bank->addTouchEventListener(CC_CALLBACK_2(GamePlatform::menuLobbyBottomCallBack, this));
		_lobbyBotttom.Button_Bank->setVisible(true);
	}

	HNAudioEngine::getInstance()->playBackgroundMusic(GAME_BACKGROUND_MUSIC, true);

	//////////////////////////////////////////////////////////////////////////////////
	//  广播
	_NoticeBG = Sprite::create(PLATFORM_NOTIC_BG);
	_NoticeBG->setPosition(Vec2(winSize.width / 2, winSize.height * 0.74f));
	_NoticeBG->setCascadeOpacityEnabled(true);
	this->addChild(_NoticeBG, PLATFORM_UI_ZORDER_NOTICE);
	_NoticeBG->setVisible(false);
	_NoticeBG->setCascadeOpacityEnabled(true);

	Armature* armature = Armature::create("Speaker");
	armature->setScale(0.70f);
	armature->getAnimation()->play("speaker");
	armature->setPosition(Vec2(5, _NoticeBG->getContentSize().height / 2));
	_NoticeBG->addChild(armature);

	_PlatformNotic = GameNotice::createGameNotice(Notice_Bg_Stencil);
	_PlatformNotic->setPosition(Vec2(_NoticeBG->getContentSize().width * 0.51f, _NoticeBG->getContentSize().height / 2));
	_PlatformNotic->setCascadeOpacityEnabled(true);
	_PlatformNotic->setCallBack([this](){_NoticeBG->setVisible(false);});
	_NoticeBG->addChild(_PlatformNotic);
	_NoticeBG->setVisible(true);
	_NoticeBG->setVisible(false);

	requestNotice();
	requestModule();

	checkGiftMessages();

	schedule(schedule_selector(GamePlatform::checkTimeMatchStart), 1.0);		//定时赛时间检测

	return true;
}

//查询各种奖励信息
void GamePlatform::checkGiftMessages()
{
	// 在线奖励
	PlatformLogic()->sendData(MDM_GP_ONLINE_AWARD, ASS_GP_ONLINE_AWARD_CHECK, 0, 0, 
		HN_SOCKET_CALLBACK(GamePlatform::checkGiftMessagesEventSelector, this));

	// 充值奖励
	PlatformLogic()->sendData(MDM_GP_NOTIFY_PAY, ASS_GP_GET_MONEY_BY_PAY_CHECK, 0, 0, 
		HN_SOCKET_CALLBACK(GamePlatform::checkGiftMessagesEventSelector, this));

	// 签到奖励
	PlatformLogic()->sendData(MDM_GP_SIGN, ASS_GP_SIGN_CHECK, 0, 0, 
		HN_SOCKET_CALLBACK(GamePlatform::checkGiftMessagesEventSelector, this));
}

bool GamePlatform::checkGiftMessagesEventSelector(HNSocketMessage* socketMessage)
{
	switch (socketMessage->messageHead.bMainID)
	{
	case MDM_GP_ONLINE_AWARD:
		{
			CCAssert(sizeof(MSG_GP_S_ONLINE_AWARD_CHECK_RESULT) == socketMessage->objectSize, "MSG_GP_S_ONLINE_AWARD_CHECK_RESULT is error.");
			MSG_GP_S_ONLINE_AWARD_CHECK_RESULT* iCheck = (MSG_GP_S_ONLINE_AWARD_CHECK_RESULT*)socketMessage->object;

			if (1 == socketMessage->messageHead.bHandleCode)
			{
				auto Prompt_online = (ImageView*)_lobbyBotttom.Button_Task->getChildByName("Image_Prompt");
				bool bShow = (iCheck->iLeftTime == 0 && iCheck->iOnLineTimeMoney > 0) ? true : false;
				Prompt_online->setVisible(bShow);

				playOrStopJump(_lobbyBotttom.Button_Task, bShow);

				if (iCheck->iLeftTime > 0)
				{
					_Time = iCheck->iLeftTime;
					schedule(schedule_selector(GamePlatform::updataGetRewardTimeMessage), 1.0f);
				}				
			}
		}
		break;
	case MDM_GP_NOTIFY_PAY:
		{
			CCAssert(sizeof(MSG_GP_S_GetMoney_ByPay) == socketMessage->objectSize, "MSG_GP_S_GetMoney_ByPay is error.");
			MSG_GP_S_GetMoney_ByPay* iCheck = (MSG_GP_S_GetMoney_ByPay*)socketMessage->object;

			if (1 == socketMessage->messageHead.bHandleCode)
			{
				auto Prompt_online = (ImageView*)_lobbyBotttom.Button_Activity->getChildByName("Image_Prompt");
				bool bShow = iCheck->i64Money > 0 ? true : false;
				Prompt_online->setVisible(bShow);

				playOrStopJump(_lobbyBotttom.Button_Activity, bShow);
			}
		}
		break;
	case MDM_GP_SIGN:
		{
			CCAssert(sizeof(MSG_GP_S_SIGN_CHECK_RESULT) == socketMessage->objectSize, "MSG_GP_S_SIGN_CHECK_RESULT is error.");
			MSG_GP_S_SIGN_CHECK_RESULT* iCheck = (MSG_GP_S_SIGN_CHECK_RESULT*)socketMessage->object;

			auto Prompt_online = (ImageView*)_lobbyTop.Button_sign->getChildByName("Image_Prompt");
			bool bShow = iCheck->iRs == 0 ? true : false;
			Prompt_online->setVisible(bShow);
			Prompt_online->stopAllActions();
			Prompt_online->runAction(RepeatForever::create(Sequence::create(FadeIn::create(1.0f), FadeOut::create(1.0f), nullptr)));
		}
		break;
	default:
		break;
	}

	return true;
}

//卸载奖励监听
void GamePlatform::unSocketCallBack()
{
	PlatformLogic()->removeEventSelector(MDM_GP_ONLINE_AWARD, ASS_GP_ONLINE_AWARD_CHECK);
	PlatformLogic()->removeEventSelector(MDM_GP_NOTIFY_PAY, ASS_GP_GET_MONEY_BY_PAY_CHECK);
	PlatformLogic()->removeEventSelector(MDM_GP_SIGN, ASS_GP_SIGN_CHECK);
}

//领奖倒计时
void GamePlatform::updataGetRewardTimeMessage(float dt)
{
	_Time--;
	if (0 == _Time)
	{
		unschedule(schedule_selector(GamePlatform::updataGetRewardTimeMessage));
		auto Prompt_online = (ImageView*)_lobbyBotttom.Button_Task->getChildByName("Image_Prompt");
		Prompt_online->setVisible(true);

		playOrStopJump(_lobbyBotttom.Button_Task, true);
	}
}

void GamePlatform::requestGameRoomTimerCallBack(float dt)
{
	_gameListLogic->start();
	_gameListLogic->requestGameList();
}

void GamePlatform::onPlatformRoomListCallback(bool success, const std::string& message)
{
	if (success)
	{
		createRoomListLayer();
	}
	else
	{
		GamePromptLayer::create()->showPrompt(message);
	}	
}

void GamePlatform::onPlatformGameListCallback(bool success, const std::string& message)
{
	if (success)
	{
		if(GameCreator()->getRegistGameCount() > 1)
		{
			createGameListLayer();
		}
		else
		{
			_gameListLogic->stop();
			_roomListLogic->start();
			_roomListLogic->requestRoomList();
		}
	}
	else
	{
		GamePromptLayer::create()->showPrompt(message);
	}
}

//创建房间列表层
void GamePlatform::createRoomListLayer()
{
	LoadingLayer::removeLoading(this);
	Size winSize = Director::getInstance()->getWinSize();
	auto gameRoomLayer = GameRoom::create();
	gameRoomLayer->setPosition(Vec2(0.0f, 0.0f));
	gameRoomLayer->setName("gameRoomLayer");
	gameRoomLayer->onCloseCallBack = [this](){
		LoadingLayer::createLoading(this, "", 22, "");
		_roomListLogic->stop();
		_gameListLogic->start();
		_gameListLogic->requestGameList();

		if (_lobbyBotttom.layout->getPositionY() < 0)
		{
			showTopAndBottomAction(false, false);
		}

		checkGiftMessages();
	};
	gameRoomLayer->onEnterDeskCallBack = [this](ComRoomInfo* roomInfo){
		createDeskListLayer(roomInfo);
	};  

	gameRoomLayer->onRunActionCallBack = [this](){
		showTopAndBottomAction(true, true);
	};

	addChild(gameRoomLayer, ROOM_LAYER_ZORDER, CHILD_LAYER_ROOM_TAG);
	gameRoomLayer->setOpacity(0);
	gameRoomLayer->runAction(Sequence::create(FadeIn::create(0.3f), CallFunc::create([&]()
	{
		_lobbyTop.Button_Return->setEnabled(true);
		auto gameRoomLayer = dynamic_cast<GameRoom*>(this->getChildByName("gameRoomLayer"));
		UINT roomID = gameRoomLayer->getTimeMatchRoomID();
		auto room = RoomInfoModule()->getByRoomID(roomID);
		if (room == nullptr)	return;
		if (roomID != 255)
		{
			gameRoomLayer->fastEnterTimeMatch(roomID);
		}
	}), nullptr));
}

//创建游戏列表层
void GamePlatform::createGameListLayer()
{
	LoadingLayer::removeLoading(this);
	_lobbyTop.Button_Return->setEnabled(true);

	Size winSize = Director::getInstance()->getWinSize();
	auto gamesLayer = GameLists::create();
	gamesLayer->setName("gamesLayer");
	gamesLayer->setPosition(Vec2(0.0f, 0.0f));

	gamesLayer->onEnterGameCallBack = [this](){
		LoadingLayer::createLoading(this, "", 22, "");
		_gameListLogic->stop();
		_roomListLogic->start();
		_roomListLogic->requestRoomList();

		if (GameCreator()->getRegistGameCount() > 1 && _lobbyBotttom.layout->getPositionY() < 0)
		{
			showTopAndBottomAction(false, true);
		}
	};

	gamesLayer->onRunActionCallBack = [this](){
		showTopAndBottomAction(true, false);
	};
	addChild(gamesLayer, GAMELIST_LAYER_ZORDER, CHILD_LAYER_GAMES_TAG);
	gamesLayer->setOpacity(0);
	gamesLayer->runAction(FadeIn::create(0.3f));

	enterTimeMatch();
}

//创建桌子列表层
void GamePlatform::createDeskListLayer(ComRoomInfo* roomInfo)
{
	Size winSize = Director::getInstance()->getWinSize();
	auto gameDeskLayer = GameDesk::createDesk(roomInfo);
	gameDeskLayer->setPosition(Vec2(0.0f, 0.0f));
	gameDeskLayer->setName("gameDeskLayer");
	gameDeskLayer->onCloseCallBack = [this](){
		_roomListLogic->start();
		_roomListLogic->requestRoomList();
		LoadingLayer::createLoading(this, "", 22, "");

		if (_lobbyBotttom.layout->getPositionY() < 0)
		{
			if (GameCreator()->getRegistGameCount() > 1)
			{
				showTopAndBottomAction(false, true);
			}
			else
			{
				showTopAndBottomAction(false, false);
			}
		}
	};

	addChild(gameDeskLayer, DESKLIST_LAYER_ZORDER, CHILD_LAYER_DESKS_TAG);
	gameDeskLayer->setOpacity(0);
	gameDeskLayer->runAction(Sequence::create(FadeIn::create(0.3f), CallFunc::create([=](){
		_lobbyTop.Button_Return->setEnabled(true);
	}), nullptr));

	if (_lobbyBotttom.layout->getPositionY() < 0)
	{
		showTopAndBottomAction(false, true);
	}
}

void GamePlatform::onPlatformRoomUserCountCallback(UINT roomID, UINT userCount)
{
	GameRoom* roomLayer = dynamic_cast<GameRoom*>(this->getChildByTag(CHILD_LAYER_ROOM_TAG));
	if (nullptr != roomLayer)
	{
		roomLayer->updateRoomPeopleCount(roomID, userCount);
	}
}

void GamePlatform::onPlatformGameUserCountCallback(UINT Id, UINT count)
{
	GameLists* gameListLayer = dynamic_cast<GameLists*>(this->getChildByTag(CHILD_LAYER_GAMES_TAG));
	if (nullptr != gameListLayer)
	{
		gameListLayer->updateGamePeopleCount(Id, count);
	}
}

void GamePlatform::onExit()
{
	_roomListLogic->stop();
	_gameListLogic->stop();
	HNLayer::onExit();
}

void GamePlatform::onEnterTransitionDidFinish()
{
	HNLayer::onEnterTransitionDidFinish();

	releaseUnusedRes();
}

void GamePlatform::menuLobbyTopCallBack(Ref* pSender, Widget::TouchEventType type)
{
	if (Widget::TouchEventType::ENDED != type || _isTouch == false) return;
	auto btn = (Button*)pSender;
	_isTouch = false;
	btn->runAction(Sequence::create(DelayTime::create(0.5f),CCCallFunc::create([=]()
	{
		_isTouch = true;
	}), nullptr));
	HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);
	auto winSize = Director::getInstance()->getWinSize();

	std::string name(btn->getName());
	
	// 公告
	if (name.compare("Button_notice") == 0)
	{
		NoticeList* noticeList = NoticeList::create();
		this->addChild(noticeList, CHILD_LAYER_ZORDER);
		noticeList->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
	}

	// 返回
	else if (name.compare("Button_return") == 0)
	{
		btn->setEnabled(false);
		showTopAndBottomAction(true, true);
		auto gameDeskLayer = (GameDesk*)this->getChildByName("gameDeskLayer");
		if (nullptr != gameDeskLayer)
		{
			gameDeskLayer->closeDeskList();
			return;
		}
		auto gameRoomLayer = (GameRoom*)this->getChildByName("gameRoomLayer");
		if (nullptr != gameRoomLayer)
		{
			gameRoomLayer->closeRoomList();
		}
	}

	// 签到
	else if (name.compare("Button_sign") == 0)
	{
		unSocketCallBack();
		GameSignLayer* signLayer = GameSignLayer::create();

		signLayer->onUpdataUserMoney = [this](LLONG money)
		{
			walletChanged(money);
		};

		signLayer->onCloseCallBack = [this]()
		{
			checkGiftMessages();
		};

		auto winSize = Director::getInstance()->getWinSize();
		this->addChild(signLayer, CHILD_LAYER_ZORDER, CHILD_LAYER_SIGN_TAG);
		signLayer->setName("signLayer");
		signLayer->ignoreAnchorPointForPosition(false);
		signLayer->setAnchorPoint(Vec2(0.5, 0.5));
		signLayer->setPosition(Vec2(winSize/2));
	}

	// 客服
	else if (name.compare("Button_service") == 0)
	{
		auto serviceLayer = ServiceLayer::create();
		serviceLayer->showService(this, Vec2(990, winSize.height - 55), CHILD_LAYER_ZORDER, CHILD_LAYER_SERVICE_TAG);
	}

	// 退出
	else if (name.compare("Button_exit") == 0)
	{
		auto exitChangeLayer = GameExitChangeLayer::create();
		exitChangeLayer->showExitChoose(this, CHILD_LAYER_ZORDER, CHILD_LAYER_EXITCHANGE_TAG);
		exitChangeLayer->onExitCallBack = [this](){
			_gameListLogic->stop();
			_roomListLogic->stop();
			RoomLogic()->close();
			PlatformLogic()->close();
			GameMenu::createMenu();
		};
	}

	// 设置
	else if (name.compare("Button_set") == 0)
	{
		auto setLayer = GameSetLayer::create();
		setLayer->showSet(this, CHILD_LAYER_ZORDER, CHILD_LAYER_SET_TAG);
	}

	// 推广员
	else if (name.compare("Button_popularize") == 0)
	{
		LoadingLayer::createLoading(this, GBKToUtf8("正在获取信息，请稍后..."), 22, LOADING);
		char tmpstr[100];
		sprintf(tmpstr, "option=GetTuiGuangInfo&tgUserId=%d", HNPlatformLogic::getInstance()->loginResult.dwUserID);
		_spreadLogic.requestSpreadInfo(PlatformConfig::getInstance()->getPromoterUrl(), tmpstr);
	}

	// 兑换
	else if (name.compare("Button_exchange") == 0)
	{
		auto giftShopLayer = GameGiftShop::createGameGiftShop(this, PlatformLogic()->loginResult.iLotteries);
		giftShopLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		giftShopLayer->setPosition(winSize / 2);
		giftShopLayer->setName("giftShopLayer");
		addChild(giftShopLayer, 1000);
	}

	// 其他
	else
	{
		
	}
}

//截屏保存
void GamePlatform::screenShoot()
{
	std::string filename = FileUtils::getInstance()->getWritablePath() + "umeng_share.png";
	if (!FileUtils::getInstance()->isFileExist(filename))
	{
		utils::captureScreen([this](bool succeed, const std::string& outputFile){
			if (succeed)
			{
				openUMengShare();
			}
			else
			{
				MessageBox(GBKToUtf8("获取截屏失败！"), "error");
				return;
			}			
		}, "umeng_share.png");
	}
	else
	{
		openUMengShare();
	}
}

void GamePlatform::openUMengShare()
{
	CCUMSocialSDK* sdk = CCUMSocialSDK::create("555bfe6767e58e7dcc0051d5");
	//sdk->setQQAppIdAndAppKey("1104765773", "bDr0ZySCI8PKiwQj");

	sdk->setQQAppIdAndAppKey("1105327702", "KEYatXeEOd2p9sF8rtH");
	
	
	// 企业帐号
	//sdk->setWeiXinAppInfo("wx532ff503c3a59845", "ef3960302a4a3b9e7faf34b8f1f7f4bf");
	
	// 个人帐号
	//sdk->setWeiXinAppInfo("wx5bd8039e2dff85e7", "371b885868c938455726339da41d0eab");
	
	// 设置友盟分享面板上显示的平台
	vector<int>* platforms = new vector<int>();
	platforms->push_back(QZONE);
	platforms->push_back(QQ) ;
	platforms->push_back(SINA);
	platforms->push_back(DOUBAN) ;		

	// 微信分享和包名有关，不同游戏包名不一致
	if (GameCreator()->getRegistGameCount() > 1)
	{
		//platforms->push_back(WEIXIN);
		//platforms->push_back(WEIXIN_CIRCLE);
	}

	sdk->setPlatforms(platforms) ;

	std::string	path = FileUtils::getInstance()->getWritablePath() + "umeng_share.png";
	if (!FileUtils::getInstance()->isFileExist(path))
	{
		MessageBox(GBKToUtf8("获取截屏失败！"), "error");
		return;
	}

	std::string shareUrl    = _shareUrl;
	std::string shareTitile = GBKToUtf8("欢迎下载使用。");

	sdk->setPlatformShareContent(WEIXIN,        _shareContent.c_str(), path.c_str(), shareTitile.c_str(), shareUrl.c_str());
	sdk->setPlatformShareContent(WEIXIN_CIRCLE, _shareContent.c_str(), path.c_str(), shareTitile.c_str(), shareUrl.c_str());
	sdk->setPlatformShareContent(SINA,          _shareContent.c_str(), path.c_str(), shareTitile.c_str(), shareUrl.c_str());
	sdk->setPlatformShareContent(QQ,            _shareContent.c_str(), path.c_str(), shareTitile.c_str(), shareUrl.c_str());
	sdk->setPlatformShareContent(QZONE,         _shareContent.c_str(), path.c_str(), shareTitile.c_str(), shareUrl.c_str());

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
	sdk->openShare(GBKToUtf8("欢迎下载使用。"), path.c_str(), nullptr);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) 
	sdk->openShare(GBKToUtf8("欢迎下载使用。"), path.c_str(), nullptr);
#else
	GamePromptLayer::create()->showPrompt(GBKToUtf8("仅支持android和ios分享。"));
#endif 
}

void GamePlatform::UMengShareCallback(int platform, int stCode,const std::string& errorMsg)
{
	//log("************************share");
	//log("platform>%d stCode:%d error>%s", platform, stCode, errorMsg.c_str());

	//Scene* scene = Director::getInstance()->getRunningScene();
	//Director::getInstance()->getScheduler()->schedule(nullptr, scene, 0, 0, 1.0f, false);
}

void GamePlatform::menuLobbyBottomCallBack(Ref* pSender, Widget::TouchEventType type)
{
	if (Widget::TouchEventType::ENDED != type || _isTouch == false) return;
	auto btn = (Button*)pSender;
	_isTouch = false;
	btn->runAction(Sequence::create(DelayTime::create(0.5f),CCCallFunc::create([=]()
	{
		_isTouch = true;
	}), nullptr));
	auto winSize = Director::getInstance()->getWinSize();

	HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);

	std::string name(btn->getName());

	if (name.compare("Button_friend") == 0)
	{
		//截屏分享
		requestShare();
		screenShoot();
	}
	else if (name.compare("Button_task") == 0)
	{
		unSocketCallBack();
		GameOnlineReward* onlineReward = GameOnlineReward::create();

		onlineReward->onUpdataUserMoney = [this](LLONG money)
		{
			walletChanged(money);
		};

		onlineReward->onCloseCallBack = [this]()
		{
			checkGiftMessages();
		};

		auto winSize = Director::getInstance()->getWinSize();
		this->addChild(onlineReward, CHILD_LAYER_ZORDER, CHILD_LAYER_ONLINE_TAG);
		onlineReward->setName("onlineReward");
		onlineReward->ignoreAnchorPointForPosition(false);
		onlineReward->setAnchorPoint(Vec2(0.5f, 0.5f));
		onlineReward->setPosition(Vec2(winSize/2));
	}
	else if (name.compare("Button_activity") == 0)
	{
		unSocketCallBack();
		GameRechargeRebate* rebateLayer = GameRechargeRebate::create();

		rebateLayer->onUpdataUserMoney = [this](LLONG money)
		{
			walletChanged(money);
		};

		rebateLayer->onCloseCallBack = [this]()
		{
			checkGiftMessages();
		};

		auto winSize = Director::getInstance()->getWinSize();
		this->addChild(rebateLayer, CHILD_LAYER_ZORDER, CHILD_LAYER_REBATE_TAG);
		rebateLayer->setName("rebateLayer");
		rebateLayer->ignoreAnchorPointForPosition(false);
		rebateLayer->setAnchorPoint(Vec2(0.5f, 0.5f));
		rebateLayer->setPosition(Vec2(winSize/2));
	}
	else if (name.compare("Button_store") == 0)
	{
		auto storeLayer = GameStoreLayer::createGameStore(this);
		storeLayer->setChangeDelegate(this);
		storeLayer->openStore(this, Vec2(winSize.width, 0), CHILD_LAYER_ZORDER, CHILD_LAYER_STORE_TAG);

		storeLayer->onCloseCallBack = [this]()
		{
			checkGiftMessages();
		};
	}
	else if (name.compare("Button_bank") == 0)
	{
		auto bankLayer = GameBankLayer::createGameBank();
		bankLayer->setChangeDelegate(this);
		bankLayer->openBank(this, Vec2(winSize.width, 0), CHILD_LAYER_ZORDER, CHILD_LAYER_BANK_TAG);
	}
	else if (name.compare("Button_rank") == 0)
	{
		auto rankLayer = GameRankingList::create();
		rankLayer->openRank(this, Vec2(winSize.width, 0 ), CHILD_LAYER_ZORDER, CHILD_LAYER_RANK_TAG);
	}
	else
	{
		GamePromptLayer::create()->showPrompt(GBKToUtf8("程序猿使劲开发中。"));
	}

}

void GamePlatform::onKeyReleased(EventKeyboard::KeyCode keyCode,Event * pEvent)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	if (EventKeyboard::KeyCode::KEY_BACK == keyCode)
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	if (EventKeyboard::KeyCode::KEY_F1 == keyCode)
#endif
	{
		do 
		{
			ServiceLayer* serviceLayer = dynamic_cast<ServiceLayer*>(this->getChildByTag(CHILD_LAYER_SERVICE_TAG));
			if (nullptr != serviceLayer)
			{
				serviceLayer->closeService();
				break;
			}

			GameSetLayer* setLayer = dynamic_cast<GameSetLayer*>(this->getChildByTag(CHILD_LAYER_SET_TAG));
			if (nullptr != setLayer)
			{
				setLayer->closeSet();
				break;
			}

			GameUserDataLayer* userDataLayer = dynamic_cast<GameUserDataLayer*>(this->getChildByTag(CHILD_LAYER_USERHEAD_TAG));
			if (nullptr != userDataLayer)
			{
				userDataLayer->closeUserInfo();
				break;
			}

			BindPhoneLayer* bindPhone = dynamic_cast<BindPhoneLayer*>(this->getChildByTag(CHILD_LAYER_PHONE_TAG));
			if (nullptr != bindPhone)
			{
				bindPhone->close();
				break;
			}

			ModifyPasswordLayer* modifyPass = dynamic_cast<ModifyPasswordLayer*>(this->getChildByTag(CHILD_LAYER_PASS_TAG));
			if (nullptr != modifyPass)
			{
				modifyPass->close();
				break;
			}

			GameExitChangeLayer* exitChange = dynamic_cast<GameExitChangeLayer*>(this->getChildByTag(CHILD_LAYER_EXITCHANGE_TAG));
			if (nullptr != exitChange)
			{
				exitChange->closeExitChoose();
				break;
			}

			GameSignLayer* gameSign = dynamic_cast<GameSignLayer*>(this->getChildByTag(CHILD_LAYER_SIGN_TAG));
			if (nullptr != gameSign)
			{
				gameSign->closeSign();
				break;
			}

			GameRankingList* gameRank = dynamic_cast<GameRankingList*>(this->getChildByTag(CHILD_LAYER_RANK_TAG));
			if (nullptr != gameRank)
			{
				gameRank->closeRanking();
				break;
			}

			GameBankLayer* bankLayer = dynamic_cast<GameBankLayer*>(this->getChildByTag(CHILD_LAYER_BANK_TAG));
			if (nullptr != bankLayer)
			{
				bankLayer->closeBank();
				break;
			}

			GameStoreLayer* storeLayer = dynamic_cast<GameStoreLayer*>(this->getChildByTag(CHILD_LAYER_STORE_TAG));
			if (nullptr != storeLayer)
			{
				storeLayer->closeStore();
				break;
			}

			GameOnlineReward* OnlineReward = dynamic_cast<GameOnlineReward*>(this->getChildByTag(CHILD_LAYER_ONLINE_TAG));
			if (nullptr != OnlineReward)
			{
				OnlineReward->closeOnlineReward();
				break;
			}

			GameRechargeRebate* rebateLayer = dynamic_cast<GameRechargeRebate*>(this->getChildByTag(CHILD_LAYER_REBATE_TAG));
			if (nullptr != rebateLayer)
			{
				rebateLayer->closeRebate();
				break;
			}

			// 调用离开游戏层
			GameExitLayer* exitLayer = dynamic_cast<GameExitLayer*>(this->getChildByTag(CHILD_LAYER_EXIT_TAG));
			if (nullptr == exitLayer)
			{
				auto exitLayer = GameExitLayer::create();
				exitLayer->setPosition(Size::ZERO);
				addChild(exitLayer, 100000000, CHILD_LAYER_EXIT_TAG);
			}
			else
			{
				exitLayer->callBackNo();
			}
		} while (0);
	}
}

void GamePlatform::walletChanged(LLONG money)
{
	if (nullptr != _lobbyTop.Label_UserMoney)
	{
		char buffer[32];
		sprintf(buffer, "%lld", money);
		_lobbyTop.Label_UserMoney->setString(buffer);
	}
}


void GamePlatform::onHttpResponse(const std::string& requestName, bool isSucceed, const std::string &responseData)
{
	if (!isSucceed)
	{
		return;
	}

	if (requestName.compare("notice") == 0)
	{
		dealNoticeResp(responseData);
	}
	else if (requestName.compare("module") == 0)
	{
		dealModuleResp(responseData);
	}
	else if (requestName.compare("share") == 0)
	{
		dealShareResp(responseData);
	}
	else
	{

	}
}

void GamePlatform::requestNotice()
{
	std::string url = PlatformConfig::getInstance()->getNoticeUrl();
	url.append("Type=GetSystemMsg");
	url.append("&pageSize=1");
	url.append("&pageindex=1");

	HNHttpRequest::getInstance()->addObserver(this);
	HNHttpRequest::getInstance()->request("notice", HttpRequest::Type::GET, url);

}

void GamePlatform::dealNoticeResp(const std::string& data)
{
	rapidjson::Document doc;
	doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());

	if (doc.HasParseError() || !doc.IsObject())
	{
		return;
	}

	if (doc.HasMember("list") && doc["list"].IsArray() && doc["list"].Capacity() > 0)
	{
		rapidjson::Value& value = doc["list"][(rapidjson::SizeType)0];
		std::string message = value["MContent"].GetString();

		CCAssert(_PlatformNotic != nullptr, "null");

		if (_NoticeBG && _PlatformNotic)
		{
			_NoticeBG->setVisible(true);
			_PlatformNotic->postMessage(message);
		}
	}
}

//顶部和底部变幻动作
void GamePlatform::showTopAndBottomAction(bool isOut, bool canReturn)
{
	int s = isOut ? 1 : -1;
	int k = isOut ? -1 : 1;
	_lobbyTop.UserHead_Head->setVisible(!canReturn);
	_lobbyTop.Button_Return->setVisible(canReturn);

	_Decoration_right->runAction(MoveBy::create(0.3f, Vec2(_Decoration_right->getContentSize().width * s, 0)));

	_lobbyBotttom.layout->runAction(MoveBy::create(0.3f, Vec2(0, _lobbyBotttom.layout->getContentSize().height * k)));
	_Decoration_left->runAction(MoveBy::create(0.3f, Vec2(_Decoration_left->getContentSize().width * k, 0)));

	if (!isOut)
	{
		_lobbyTop.layout->runAction(Sequence::create(MoveBy::create(0.3f, Vec2(0, _lobbyTop.layout->getContentSize().height * s)), 
			CallFunc::create([this](){
				LoadingLayer::removeLoading(this);
		}), nullptr));
	}
	else
	{
		_lobbyTop.layout->runAction(Sequence::create(MoveBy::create(0.3f, Vec2(0, _lobbyTop.layout->getContentSize().height * s)),
			CallFunc::create([this](){
				_lobbyBotttom.Button_Bank->setVisible(!RoomLogic()->isConnect());
		}), nullptr));
	}

	if (_NoticeBG->isVisible())
	{
		if (isOut)
		{
			_NoticeBG->runAction(FadeOut::create(0.3f));
		}
		else
		{
			_NoticeBG->runAction(FadeIn::create(0.3f));
		}
	}
}

//显示广播通知
void GamePlatform::onPlatformNewsCallback(const std::string& message)
{
	_NoticeBG->setVisible(true);
	_PlatformNotic->postMessage(GBKToUtf8(message.c_str()));
}

void GamePlatform::onPlatformDisConnectCallback(const std::string& message)
{
	auto prompt = GamePromptLayer::create();
	prompt->showPrompt(message);
	prompt->setCallBack([this](){
		GameMenu::createMenu();
	});
}

std::string GamePlatform::showHint()
{
	do 
	{
		std::string filename = "config.json";
		if (FileUtils::getInstance()->isFileExist(filename))
		{
			std::string json = FileUtils::getInstance()->getStringFromFile(filename);
			rapidjson::Document doc;
			doc.Parse<rapidjson::kParseDefaultFlags>(json.c_str());
			if (doc.HasParseError())
			{
				break;
			}

			if (doc.IsObject() && doc.HasMember("slogans"))
			{
				rapidjson::Value& _array = doc["slogans"];
				if(_array.IsArray())
				{
					int maxNum = _array.Size();
					int randNum = rand() % maxNum;

					rapidjson::Value& arraydoc = _array[randNum];
					if(arraydoc.HasMember("ad"))
					{
						std::string ad = arraydoc["ad"].GetString();
						return ad;
					}
				}
			}
		}
	} while (0);

	return "";
}

void GamePlatform::requestModule()
{
	std::string param = StringUtils::format("action=GetConfigByKey");
	HNHttpRequest::getInstance()->request("module", HttpRequest::Type::POST, PlatformConfig::getInstance()->getEditUrl(), param);
	HNHttpRequest::getInstance()->addObserver(this);
}

void GamePlatform::dealModuleResp(const std::string& data)
{
	rapidjson::Document doc;
	doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());

	if (doc.HasParseError() || !doc.IsObject() || !doc.HasMember("content") || !doc["content"].IsArray())
	{
		return;
	}

	// 解析
	for (auto i = 0; i < doc["content"].Size(); i++)
	{		
		if (!doc["content"][i].HasMember("AttrKey") || !doc["content"][i].HasMember("IosOpen") || !doc["content"][i].HasMember("AdrOpen"))
		{
			continue;
		}

		std::string key = doc["content"][i]["AttrKey"].GetString();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		bool value = doc["content"][i]["IosOpen"].GetBool();
#else
		bool value = doc["content"][i]["AdrOpen"].GetBool();
#endif
		Configuration::getInstance()->setValue(key, Value(value));
	}

	// 顶部位置(从右到左）
	const int TOP_COUNT = 5;
	Node* topNode[TOP_COUNT] =
	{ 
		_lobbyTop.Button_service, _lobbyTop.Button_sign, _lobbyTop.Button_notice, _lobbyTop.Button_exchange, _lobbyTop.Button_popularize
	};

	// 获取位置
	Vec2 top_position[TOP_COUNT];
	for (int i = 0; i < TOP_COUNT; i++)
	{
		top_position[i] = topNode[i]->getPosition();
	}

	// 设置显示
	std::string topKey[] = {"service", "signin", "notice", "exchange", "promotion"};
	for (int i = 0, index = 0; i < TOP_COUNT; i++)
	{
		if (Configuration::getInstance()->getValue(topKey[i], Value(false)).asBool())
		{
			topNode[i]->setVisible(true);
			topNode[i]->setPosition(top_position[index++]);
		}
	}

	// 底部位置（从左到右）
	const int BOTTOM_COUNT = 6;
	Node* bottomNode[BOTTOM_COUNT] =
	{
		_lobbyBotttom.Button_Friend, _lobbyBotttom.Button_Task, _lobbyBotttom.Button_Activity, _lobbyBotttom.Button_Store,
		_lobbyBotttom.Button_Rank, _lobbyBotttom.Button_Bank
	};

	// 底部位置
	Vec2 bottom_position[BOTTOM_COUNT];
	for (int i = 0; i < BOTTOM_COUNT; i++)
	{
		bottom_position[i] = bottomNode[i]->getPosition();
	}

	std::string bottomKey[] = {"share", "task", "activity", "mall", "ranklist", "coffer"};
	for (int i = 0, index = 0; i < 6; i++)
	{
		if (Configuration::getInstance()->getValue(bottomKey[i], Value(false)).asBool())
		{
			bottomNode[i]->setVisible(true);
			bottomNode[i]->setPosition(bottom_position[index++]);
		}
	}
}

void GamePlatform::requestShare()
{
	std::string param = StringUtils::format("action=GetShareInfo&key=");
	HNHttpRequest::getInstance()->request("share", network::HttpRequest::Type::POST, PlatformConfig::getInstance()->getEditUrl(), param);
	HNHttpRequest::getInstance()->addObserver(this);
}

void GamePlatform::dealShareResp(const std::string& data)
{
	rapidjson::Document doc;
	doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
	if (doc.HasParseError() || !doc.IsObject() || !doc.HasMember("content") || !doc["content"].IsArray())
	{
		return;
	}

	for (auto i = 0; i < doc["content"].Size(); i++)
	{
		if (!doc["content"][i].IsObject() || !doc["content"][i].HasMember("ParaName") || !doc["content"][i].HasMember("ParaValue"))
		{
			continue;
		}

		std::string name = doc["content"][i]["ParaName"].GetString();
		std::string value = doc["content"][i]["ParaValue"].GetString();

		if (name.compare("ShareUrl") == 0)
		{
			_shareUrl = value;
		}
		else if (name.compare("ShareContent") == 0)
		{
			_shareContent = value;
		}
		else if (name.compare("ShareBgPic") == 0)
		{
			_shareImage = value;
		}
		else
		{

		}
	}	
}

void GamePlatform::onSpreadInfo(bool result, bool bInfo, const std::string& msg)
{
	LoadingLayer::removeLoading(this);
	if(result)
	{
		if(bInfo)//有推广信息，不一定就是推广员
		{
			auto layer = SpreadEditLayer::create();//显示资料界面
			layer->show(this, PLATFORM_UI_ZORDER_POPULARISE, PLATFORM_UI_ZORDER_POPULARISE);
		}
		else//肯定不是推广员，直接申请
		{
			auto layer = SpreadEditLayer::create();//申请推广员界面
			layer->show(this, PLATFORM_UI_ZORDER_POPULARISE, PLATFORM_UI_ZORDER_POPULARISE);
		}
	}
	else
	{
		GamePromptLayer::create()->showPrompt(msg);
	}
}

//定时赛即将开始
bool GamePlatform::enterTimeMatch()
{
	char str[64] = {0};
	for (int i = 0; i < 10; i++)
	{
		sprintf(str, "MARK_%d", i);
		if (UserDefault::getInstance()->getIntegerForKey(str) == 3)
		{
			if (isScheduled(schedule_selector(GamePlatform::checkTimeMatchStart)))
			{
				unschedule(schedule_selector(GamePlatform::checkTimeMatchStart));
			}
				fastEnterRoom();
				return true;
		}
	}
	return false;
}

//检测报名的定时赛是否开始
void GamePlatform::checkTimeMatchStart(float dt)
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
			if (nTime + 60 + 8*3600 >= bTime && nTime + 8*3600 <= bTime)
			{
				sprintf(str, "UserID_%d", i);
				_iValue = i;
				if (UserDefault::getInstance()->getIntegerForKey(str) != PlatformLogic()->loginResult.dwUserID)		continue;
				unschedule(schedule_selector(GamePlatform::checkTimeMatchStart));

				auto prompt = GamePromptLayer::create();
				prompt->showPrompt(GBKToUtf8("定时赛马上开始，是否进入？"));
				prompt->setPromptCanSelect();
				prompt->setCallBack([&](){
					char str[64] = {0};
					sprintf(str, "MARK_%d", _iValue);
					UserDefault::getInstance()->setIntegerForKey(str, 3);
					UserDefault::getInstance()->flush();

					auto desk = dynamic_cast<GameDesk*>(this->getChildByName("gameDeskLayer"));
					if (desk)
					{
						_isTouch = true;				
						RoomLogic()->close();
						desk->removeFromParent();	
						requestGameRoomTimerCallBack(0.1);
						return;
					}

					auto room = dynamic_cast<GameRoom*>(this->getChildByName("gameRoomLayer"));
					if (room)
					{
						_isTouch = true;				
						RoomLogic()->close();
						room->removeFromParent();
						
						requestGameRoomTimerCallBack(0.1);
					}
					else
					{
						fastEnterRoom();
					}
				});
				prompt->setCancelCallBack([&](){
					char str[64] = {0};
					sprintf(str, "MARK_%d", _iValue);
					UserDefault::getInstance()->setIntegerForKey(str, 4);
					UserDefault::getInstance()->flush();
					schedule(schedule_selector(GamePlatform::checkTimeMatchStart), 1.0);
				});
			}
		}
	}
}

//在游戏列表位置快捷进入房间比赛
void GamePlatform::fastEnterRoom()
{
	GameCreator()->setCurrentGame(10100003);//斗地主
	auto gamelist = dynamic_cast<GameLists*>(this->getChildByName("gamesLayer"));
	if (gamelist == nullptr)		return;
	gamelist->onRunActionCallBack();
	gamelist->onEnterGameCallBack();
	_isTouch = true;
	gamelist->removeFromParent();
}

void GamePlatform::playOrStopJump(cocos2d::Node* pNode, bool bJump)
{
	if(!pNode) return;

	if (pNode->getUserData())
	{
		pNode->stopAllActions();
		auto data = (cocos2d::String*)pNode->getUserData();
		pNode->setPositionY(data->floatValue());
	}

	if (bJump)
	{
		_Data.insert(pair<std::string, cocos2d::String>(pNode->getName(), *String::createWithFormat("%f", pNode->getPositionY())));
		
		auto data = _Data.find(pNode->getName());
		if (data != _Data.end())
		{
			pNode->setUserData(&data->second);
		}

		auto moveBy = MoveBy::create(0.5f, Vec2(0.f, 40.f));
		auto sequence = Sequence::create(EaseBackIn::create(moveBy), DelayTime::create(0.1f),
			EaseElasticOut::create(moveBy->reverse()), DelayTime::create(0.5f), nullptr);
		pNode->runAction(RepeatForever::create(sequence));
	}	
}


