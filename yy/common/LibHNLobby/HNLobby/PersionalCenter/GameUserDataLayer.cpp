#include "HNLobby/PersionalCenter/GameUserDataLayer.h"
#include "HNLobby/PersionalCenter/ModifyPassword.h"
#include "HNLobby/PersionalCenter/BindPhone.h"
#include "HNLobby/FontConfig.h"
#include "HNLobby/PlatformDefine.h"
#include "HNLobby/GamePrompt.h"
#include "HNLobby/PlatformConfig.h"
#include <string>

static const char* USER_HEAD_FRAME	= "platform/head/user_head_frame.png";                  //user head box
static const char* MEN_HEAD			= "platform/head/men_head.png";							//user head 
static const char* WOMEN_HEAD		= "platform/head/women_head.png";						//user head 
static const char* MEN_SEX		    = "platform/userData/userDataRes/male.png";							//user sex 
static const char* WOMEN_SEX		= "platform/userData/userDataRes/female.png";						//user sex 

static const char* PERSIONAL_JSON	= "platform/persional/PersionalCenterUi_1.json";

static const int MODIFYPASSWORDLAYER_TAG = 100;
static const int BINDPHONELAYER_TAG = 101;

GameUserDataLayer::GameUserDataLayer() : _canClose(false)
{
	memset(&_ui, 0x0, sizeof(_ui));
}

GameUserDataLayer::~GameUserDataLayer()
{

}

void GameUserDataLayer::showUserInfo(Node* parent, int zorder, int tag)
{
	CCAssert(nullptr != parent, "parent is nullptr");
	parent->addChild(this, zorder, tag);
}

void GameUserDataLayer::closeUserInfo()
{
	HNAudioEngine::getInstance()->playEffect(GAME_SOUND_CLOSE);
	_ui.layout->runAction(Sequence::create(ScaleTo::create(0.2f, 0.3f),  CCCallFunc::create([&]()
	{
		this->removeFromParent();
	}), nullptr));
}

bool GameUserDataLayer::init()
{
    if ( !HNLayer::init()) return false;

	Size winSize = Director::getInstance()->getWinSize();

	enableKeyboard();
	//屏蔽后面的层
	colorLayer = LayerColor::create(Color4B(0, 0, 0, 150));
	addChild(colorLayer, 10);

	touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = [=](Touch* touch ,Event* event){
		return true;
	};
	touchListener ->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, colorLayer);

	userDataLoader = CSLoader::createNode("platform/userData/userDataNode.csb");
	_ui.layout = ( Layout* )userDataLoader->getChildByName("Panel_1");
	userDataLoader->setPosition(winSize/2);
	userDataLoader->setScale(0);
	addChild(userDataLoader,10);
	auto userDataLoaderShow = Sequence::create(ScaleTo::create(0.2f, 1.0f), nullptr);
	userDataLoader->runAction(userDataLoaderShow);

	//// 玩家资料框背景
	//_ui.layout = dynamic_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile(PERSIONAL_JSON));
	//if (nullptr != _ui.layout)
	//{
	//	_ui.layout->setAnchorPoint(Vec2(0.5f, 0.5f));
	//	_ui.layout->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
	//	addChild(_ui.layout, 2, 3);

	MSG_GP_R_LogonResult& LogonResult = PlatformLogic()->loginResult;
	// 玩家头像	
	auto userHead = UserHead::create(USER_HEAD_FRAME);
	if (switchToLayoutControl(_ui.layout, "Text_head", userHead))
	{
		userHead->setUserHead(LogonResult.bBoy ? MEN_HEAD : WOMEN_HEAD);
		userHead->setScale(1.8f, 1.8f);
		userHead->onHeadClickEvent = [&](UserHead* sender, Vec2 touchVec)
		{
	
		};
	}
	char buffer[64];

	// 昵称
	_ui.Label_UserNickName = ( Text* )_ui.layout->getChildByName("Text_nickName");
	sprintf(buffer, "%s", LogonResult.nickName);
	_ui.Label_UserNickName->setString(GBKToUtf8(buffer));

	// 性别
	_ui.Label_Sex = ( Text* )_ui.layout->getChildByName("Text_sex");
	_ui.Label_Sex->setString(LogonResult.bBoy ? GBKToUtf8("男") : GBKToUtf8("女"));
	_ui.Image_Sex = ( ImageView* ) _ui.layout->getChildByName("Image_sexImage");
	_ui.Image_Sex->loadTexture(LogonResult.bBoy ? MEN_SEX : WOMEN_SEX);

	// 魅力
	_ui.Label_Fascination = ( Text* )_ui.layout->getChildByName("Text_fascination");
	sprintf(buffer, "%d", LogonResult.dwFascination);
	_ui.Label_Fascination->setString(buffer);

	// 金币
	_ui.Label_Money = ( Text* )_ui.layout->getChildByName("Text_money");
	sprintf(buffer, "%lld", LogonResult.i64Money);
	_ui.Label_Money->setString(buffer);

	// 登陆名 
	_ui.Label_LandName = ( Text* ) _ui.layout->getChildByName("Text_UserName");
	sprintf(buffer, "%s", LogonResult.szName);
	_ui.Label_LandName->setString(GBKToUtf8(buffer));

	// 玩家ID
	_ui.Label_UserID = ( Text* ) _ui.layout->getChildByName("Text_ID");
	sprintf(buffer, "ID:%d", LogonResult.dwUserID);
	_ui.Label_UserID->setString(buffer);

	// 手机号
	_ui.Label_phone = ( Text* )_ui.layout->getChildByName("Text_phone");
	sprintf(buffer, "%s", LogonResult.szMobileNo);

	// 关闭按钮
	_ui.Button_Close = ( Button* )_ui.layout->getChildByName("Button_close");
	_ui.Button_Close->addTouchEventListener(CC_CALLBACK_2(GameUserDataLayer::closeCallBack, this));

	// 修改密码
	_ui.Button_ModifyPassword = ( Button* )_ui.layout->getChildByName("Button_modifyPass");
	_ui.Button_ModifyPassword->addTouchEventListener(CC_CALLBACK_2(GameUserDataLayer::modifyPasswordUIEventCallBack, this));

	// 绑定手机
	_ui.Button_BindPhone = ( Button* )_ui.layout->getChildByName("Button_bindPhone");
	_ui.Button_BindPhone->addTouchEventListener(CC_CALLBACK_2(GameUserDataLayer::bindPhoneUIEventCallBack, this));

	// 更新显示
	updateUserDetail();

	return true;
}

void GameUserDataLayer::closeCallBack(Ref* pSender, Widget::TouchEventType type)
{
	Button* selectedBtn = ( Button* )pSender;
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		selectedBtn->setColor(Color3B(170, 170, 170));
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		selectedBtn->setColor(Color3B(255, 255, 255));
		userDataLoader->runAction(Sequence::create(ScaleTo::create(0.1f, 0.3f),CCCallFunc::create([&](){
			this->removeFromParent();
		}), nullptr));
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		selectedBtn->setColor(Color3B(255, 255, 255));
		break;
	default:
		break;
	}
}

void GameUserDataLayer::modifyPasswordUIEventCallBack(Ref* pSender, Widget::TouchEventType type)
{
	Button* selectedBtn = ( Button* )pSender;
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		selectedBtn->setColor(Color3B(170, 170, 170));
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		selectedBtn->setColor(Color3B(255, 255, 255));
		userDataLoader->runAction(Sequence::create(ScaleTo::create(0.1f, 0.3f),CCCallFunc::create([&](){
			auto modifyPasswordLayer = ModifyPasswordLayer::create();
			modifyPasswordLayer->show(this->getParent(), this->getParent()->getZOrder()+1100, MODIFYPASSWORDLAYER_TAG);
			this->removeFromParent();
		}), nullptr));
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		selectedBtn->setColor(Color3B(255, 255, 255));
		break;
	default:
		break;
	}
}

bool GameUserDataLayer::unbindPhoneCallback(HNSocketMessage* socketMessage)
{
	CCAssert(sizeof (MSG_GP_R_BindMobile) == socketMessage->objectSize, "MSG_GP_R_BindMobile is error.");
	if (socketMessage->objectSize != sizeof(MSG_GP_R_BindMobile)) return true;

	MSG_GP_R_BindMobile *bindMobile = (MSG_GP_R_BindMobile *)socketMessage->object;

	// 请求解除绑定
	if(bindMobile->dwCommanType == 0)
	{
		PlatformLogic()->loginResult.iBindMobile = bindMobile->dwCommanResult;
		if(bindMobile->dwCommanResult == 0)
		{
			GamePromptLayer::create()->showPrompt(GBKToUtf8("手机解除绑定成功。"));
			memset(PlatformLogic()->loginResult.szMobileNo, 0x00, sizeof(PlatformLogic()->loginResult.szMobileNo));
			updateUserDetail();
		}
		else
		{
			GamePromptLayer::create()->showPrompt(GBKToUtf8("手机解除绑定失败。"));
		}
	}

	return true;
}

void GameUserDataLayer::updateUserDetail()
{
	MSG_GP_R_LogonResult &userInfo = PlatformLogic()->loginResult;

	// 已经绑定
	if(userInfo.iBindMobile == 1)
	{
		std::string phone = StringUtils::format("%s", userInfo.szMobileNo);
		_ui.Label_phone->setString(phone);

		_ui.Button_BindPhone->setTitleText(GBKToUtf8("解绑"));
	}
	// 未绑定
	else
	{
		_ui.Label_phone->setString(GBKToUtf8("未绑定"));
		_ui.Button_BindPhone->setTitleText(GBKToUtf8("绑定"));
		_ui.Button_BindPhone->setEnabled(true);
	}
}

void GameUserDataLayer::bindPhoneUIEventCallBack(Ref* pSender, Widget::TouchEventType type)
{
	Button* selectedBtn = ( Button* )pSender;
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		selectedBtn->setColor(Color3B(170, 170, 170));
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		// 绑定过，解绑
		if(PlatformLogic()->loginResult.iBindMobile == 1)
		{
			selectedBtn->setColor(Color3B(255, 255, 255));
			MSG_GP_S_BindMobile bindMobile;
			bindMobile.dwUserID = PlatformLogic()->loginResult.dwUserID;
			bindMobile.dwCommanType = 0;
			memset(bindMobile.szMobileNo, 0x00, sizeof(bindMobile.szMobileNo));
			PlatformLogic()->sendData(MDM_GP_BIND_MOBILE, ASS_GP_BIND_MOBILE, &bindMobile, sizeof(bindMobile),
				HN_SOCKET_CALLBACK(GameUserDataLayer::unbindPhoneCallback, this));
		}
		// 未绑定过，绑定
		else
		{
			selectedBtn->setColor(Color3B(255, 255, 255));
			userDataLoader->runAction(Sequence::create(ScaleTo::create(0.1f, 0.3f),CCCallFunc::create([&](){
				auto bindPhoneLayer = BindPhoneLayer::create();
				bindPhoneLayer->show(this->getParent(), this->getParent()->getZOrder()+1100, BINDPHONELAYER_TAG);
				this->removeFromParent();
			}), nullptr));
		}
		
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		selectedBtn->setColor(Color3B(255, 255, 255));
		break;
	default:
		break;
	}
	

}

void GameUserDataLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, Event * pEvent)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	if (EventKeyboard::KeyCode::KEY_BACK == keyCode)
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	if (EventKeyboard::KeyCode::KEY_BACKSPACE == keyCode)
#endif
	{
		do 
		{
			ModifyPasswordLayer* serviceLayer = dynamic_cast<ModifyPasswordLayer*>(this->getChildByTag(MODIFYPASSWORDLAYER_TAG));
			if (nullptr != serviceLayer)
			{
				break;
			}

			BindPhoneLayer* setLayer = dynamic_cast<BindPhoneLayer*>(this->getChildByTag(BINDPHONELAYER_TAG));
			if (nullptr != setLayer)
			{
				break;
			}
			_canClose = true;
		} while (0);
	}
}