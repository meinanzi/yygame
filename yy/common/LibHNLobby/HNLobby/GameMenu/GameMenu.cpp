#include "HNLobby/GameMenu/GameMenu.h"
#include "HNLobby/GamePlatform.h"
#include "HNLobby/PlatformDefine.h"
#include "HNLobby/PlatformConfig.h"
#include "HNLobby/GamePrompt.h"
#include "HNLobby/GameExitLayer.h"
#include "HNLobby/FontConfig.h"
#include "HNLobby/HNUpdate.h"
#include "HNLobby/GameMenu/GameLandLayer.h"
#include "HNLobby/GameMenu/GameRegisterLayer.h"
#include "HNUIExport.h"
#include "HNMarketExport.h"

using namespace HN;

static const int   CHILD_TAG_LAND				= 100;
static const int   CHILD_TAG_REGISTER			= 101;

#define Word_Loading		"正在登陆..."
#define Word_Register		"正在注册..."
#define Word_Empty_Name		"账号不能为空!"
#define Word_Empty_Pass		"密码不能为空!"
#define Word_Wrong_Name		"登陆名字错误!"
#define Word_Wrong_Pass		"用户密码错误!"
#define Word_Logined		"帐号已经登录!"



bool GameMenu::isVisitor = false;

static const char* MENU_BG						= "platform/hallBg.png";	
static const char* DECORATION_LEFT				= "platform/subViewBambooLeft.png";
static const char* DECORATION_RIGHT				= "platform/subViewBambooRight.png";
static const char* GAME_LOGO					= "platform/game_logo.png";
static const char* GAME_LOGO_BG					= "platform/game_logo_Bg.png";
static const char* GAME_LOGO_TITLE				= "platform/game_logo_Title.png";

static const char* GUESTLOGIN_MENU_N			= "platform/menu/tourist1.png";

static const char* REGISTER_MENU_N				= "platform/menu/register1.png";

static const char* ACCOUNTLOGIN_MENU_N			= "platform/menu/numberLogin1.png";

//static const char* BIRDANIMATION_JSON			= "platform/animation/BirdAnimation.ExportJson";

static const int CHILD_LAYER_EXIT_TAG		    = 1000;			// 退出层标签

void GameMenu::createMenu()
{
	auto scene = Scene::create();
	auto mainlayer = GameMenu::create();
	scene->addChild(mainlayer);
	Director::getInstance()->replaceScene(TransitionCrossFade::create(0.3f, scene));
}


GameMenu::GameMenu()
{
	_logo = nullptr;
	_logoBg = nullptr;
	_logoTitle = nullptr;
	_isAccountLogin = false;
	_gameRegist = new HNPlatformRegist(this);
	_gameLogin = new HNPlatformLogin(this);
}

GameMenu::~GameMenu()
{
	HN_SAFE_DELETE(_gameRegist);
	HN_SAFE_DELETE(_gameLogin);
	//ArmatureDataManager::getInstance()->removeArmatureFileInfo(BIRDANIMATION_JSON);
}

bool GameMenu::init()
{
	if (!HNLayer::init()) 
	{
		return false;
	}		

	enableKeyboard();
	setBackGroundImage(MENU_BG);

	Size winSize = Director::getInstance()->getWinSize();	
	//ArmatureDataManager::getInstance()->addArmatureFileInfo(BIRDANIMATION_JSON);

	// 加载ICON
	loadIcon();

	//屏幕显示竹叶-左上
	auto Decoration_left = Sprite::create(DECORATION_LEFT);
	Decoration_left->setAnchorPoint(Vec2(0.0f, 1.0f));
	Decoration_left->setPosition(Vec2(0.f, winSize.height));
	addChild(Decoration_left, 1);

	//屏幕显示竹叶-右上
	auto Decoration_right = Sprite::create(DECORATION_RIGHT);
	Decoration_right->setAnchorPoint(Vec2(1.0f, 1.0f));
	Decoration_right->setPosition(Vec2(winSize.width, winSize.height));
	addChild(Decoration_right, 1);

	// 游客登录按钮
	_buttonGuestLogin = Button::create(GUESTLOGIN_MENU_N);
	_buttonGuestLogin->setPosition(Vec2(winSize.width + _buttonGuestLogin->getContentSize().width, winSize.height * 0.8f));
	_buttonGuestLogin->addTouchEventListener(CC_CALLBACK_2(GameMenu::guestLoginEventCallback, this));
	addChild(_buttonGuestLogin, 1);

	// 账号登录按钮
	_buttonAccountLogin = Button::create(ACCOUNTLOGIN_MENU_N);
	_buttonAccountLogin->setPosition(Vec2(winSize.width + _buttonAccountLogin->getContentSize().width, winSize.height * 0.55f));
	_buttonAccountLogin->addTouchEventListener(CC_CALLBACK_2(GameMenu::accountLoginEventCallback, this));
	addChild(_buttonAccountLogin, 1);

	//账号注册
	_buttonRegister = Button::create(REGISTER_MENU_N);
	_buttonRegister->setPosition(Vec2(winSize.width + _buttonRegister->getContentSize().width, winSize.height * 0.3f));
	_buttonRegister->addTouchEventListener(CC_CALLBACK_2(GameMenu::RegisterEventCallback, this));
	addChild(_buttonRegister, 1);

	// 游戏版本
	std::string version = Operator::requestChannel("sysmodule", "getversion");
	auto gameVersion = createLabel(StringUtils::format("v%s", version.c_str()), 18);
	gameVersion->setAnchorPoint(Vec2(0.0f, 0.0f));
	gameVersion->setPosition(Vec2(10.f, 10.f));
	gameVersion->setVisible(false);
	addChild(gameVersion, 2);

	// 预存储游戏配置
	auto userDefault = UserDefault::getInstance();
	int music = userDefault->getIntegerForKey(MUSIC_VALUE_TEXT, -1);
	int sound = userDefault->getIntegerForKey(SOUND_VALUE_TEXT, -1);

	if (INVALID_MUSIC_VALUE == music)
	{
		music = 60;
		userDefault->setIntegerForKey(MUSIC_VALUE_TEXT, 60);
	}
	if (INVALID_SOUND_VALUE == sound)
	{
		sound = 50;
		userDefault->setIntegerForKey(SOUND_VALUE_TEXT, 50);
	}

	// 播放背景音乐
	float musicValue = (float)(music) / 100;
	float effectValue = (float)(sound) / 100;
	if (musicValue <= 0.0f || effectValue <= 0.0f)
	{
		HNAudioEngine::getInstance()->setSwitchOfMusic(false);
	}
	HNAudioEngine::getInstance()->setBackgroundMusicVolume(musicValue);
	HNAudioEngine::getInstance()->setEffectsVolume(effectValue);

	return true;
}

void GameMenu::onEnterTransitionDidFinish()
{
	HNLayer::onEnterTransitionDidFinish();

	releaseUnusedRes();

	Size winSize = Director::getInstance()->getWinSize();
	if (nullptr != _logo && nullptr != _logoBg && nullptr != _logoTitle)
	{
		_logo->runAction(EaseBackOut::create(MoveTo::create(0.3f, Vec2(winSize.width *0.29f, winSize.height *0.61))));
		_logoBg->runAction(EaseBackOut::create(MoveTo::create(0.3f, Vec2(winSize.width *0.29f, winSize.height *0.61))));
		_logoTitle->runAction(EaseBackOut::create(MoveTo::create(0.3f, Vec2(winSize.width *0.29f, _logoTitle->getContentSize().height))));
	}
	else
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		//this->checkUpdate();
#endif
	}

	_buttonGuestLogin->runAction(EaseBackOut::create(MoveTo::create(0.2f, Vec2(winSize.width * 0.8f, winSize.height * 0.8f))));
	_buttonAccountLogin->runAction(Sequence::create(DelayTime::create(0.2f),EaseBackOut::create(MoveTo::create(0.2f, Vec2(winSize.width * 0.8f, winSize.height * 0.55f))), nullptr));
	_buttonRegister->runAction(Sequence::create(DelayTime::create(0.4f),EaseBackOut::create(MoveTo::create(0.2f, Vec2(winSize.width * 0.8f, winSize.height * 0.3f))), nullptr));
}

void GameMenu::checkUpdate()
{
	HNUpdate* update = HNUpdate::create();
	if (update != nullptr)
	{
		this->addChild(update, 100);
		update->onFinishCallback = std::bind(&GameMenu::updateCheckFinishCallback, this,
			std::placeholders::_1,
			std::placeholders::_2,
			std::placeholders::_3,
			std::placeholders::_4);
		update->checkUpdate();
		update->setName("update");
	}
}

void GameMenu::updateCheckFinishCallback(bool updated, cocos2d::Node* pSender, const std::string& message, const std::string& installPath)
{
	if (false/*updated*/)
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		HN::Operator::requestChannel("sysmodule", "installApp", installPath);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		Application::getInstance()->openURL(PlatformConfig::getInstance()->getOnlineInstallUrl_iOS(installPath));
#endif
		if (pSender != nullptr)
		{
			pSender->removeFromParent();
		}
	}
	else
	{
		std::string str(GBKToUtf8("更新失败！"));
		str.append(message);
		GamePromptLayer::create()->showPrompt(str);
		if (pSender != nullptr)
		{
			pSender->removeFromParent();
		}
	}
}

// 手机返回键监听回调函数
void GameMenu::onKeyReleased(EventKeyboard::KeyCode keyCode,Event * pEvent)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	if (EventKeyboard::KeyCode::KEY_BACK == keyCode)
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	if (EventKeyboard::KeyCode::KEY_F1 == keyCode)
#endif
	{
		auto winSize = Director::getInstance()->getVisibleSize();
		do 
		{
			auto update = this->getChildByName("update");
			if (update != nullptr)
			{
				return;
			}

			RegisterLayer* registerLayer = (RegisterLayer*)this->getChildByTag(CHILD_TAG_REGISTER);
			if (nullptr != registerLayer)
			{
				registerLayer->closeRegister();
				break;
			}

			LandLayer* landLayer = (LandLayer*)this->getChildByTag(CHILD_TAG_LAND);
			if (nullptr != landLayer)
			{
				landLayer->closeLand();
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

//游客登陆回调
void GameMenu::guestLoginEventCallback(Ref* pSender, Widget::TouchEventType type)
{
	isVisitor = true;
	Button* selectedBtn = (Button*)pSender;
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		selectedBtn->setColor(Color3B(170, 170, 170));
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		selectedBtn->setColor(Color3B(255, 255, 255));
		HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);
		LoadingLayer::createLoading(this, GBKToUtf8("游客登陆中......"), 22, LOADING);

		//游客登陆
		_gameRegist->start();
		_gameRegist->requestRegist("", "", "", true);
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		selectedBtn->setColor(Color3B(255, 255, 255));
		break;
	default:
		break;
	}
}

// 账号登陆回调
void GameMenu::accountLoginEventCallback(Ref* pSender, Widget::TouchEventType type)
{
	isVisitor = false;
	auto winSize = Director::getInstance()->getWinSize();	
	Button* selectedBtn = (Button*)pSender;

	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		selectedBtn->setColor(Color3B(170, 170, 170));
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		{
			selectedBtn->setColor(Color3B(255, 255, 255));
			HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);
			LandLayer* landLayer = LandLayer::create();
			landLayer->onLoginCallBack = [this](const std::string& name, const std::string& psd)
			{
				_userName = name;
				_userPsd = psd;
				_isAccountLogin = true;

				//账号登陆
				enterGame(_userName, _userPsd);
				LoadingLayer::createLoading(this, GBKToUtf8(Word_Loading), 30, LOADING);
			};
			landLayer->onCloseCallBack = [this]()
			{

			};
			//注册按钮接口
			landLayer->onRegistCallBack = [this]()
			{
				registerActionCallBack();
			};
			this->addChild(landLayer, 3, CHILD_TAG_LAND);
			landLayer->ignoreAnchorPointForPosition(false);
			landLayer->setAnchorPoint(Vec2( 0.5f, 0.5f));
			landLayer->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		selectedBtn->setColor(Color3B(255, 255, 255));
		break;
	default:
		break;
	}
}

// 账号注册按钮回调函数
void GameMenu::RegisterEventCallback(Ref* pSender, Widget::TouchEventType type)
{
	auto winSize = Director::getInstance()->getWinSize();	
	Button* selectedBtn = (Button*)pSender;

	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		selectedBtn->setColor(Color3B(170, 170, 170));
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		{
			selectedBtn->setColor(Color3B(255, 255, 255));
			HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);
			registerActionCallBack();
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		selectedBtn->setColor(Color3B(255, 255, 255));
		break;
	default:
		break;
	}
}

//注册按钮回调
void GameMenu::registerActionCallBack()
{
	isVisitor = false;
	auto winSize = Director::getInstance()->getWinSize();
	RegisterLayer* registerLayer = RegisterLayer::create();
	registerLayer->onRegisterCallBack = [this](const std::string& name, const std::string& psd, const std::string &agency)
	{
		_userName = name;
		_userPsd = psd;
		_agency = agency;

		//注册
		_gameRegist->start();
		_gameRegist->requestRegist(_userName, MD5_CTX::MD5String(_userPsd), _agency, false);
		LoadingLayer::createLoading(this, GBKToUtf8(Word_Register), 30, LOADING);
	};

	registerLayer->onCloseCallBack = [this]()
	{		
	};

	this->addChild(registerLayer,3, CHILD_TAG_REGISTER);
	registerLayer->ignoreAnchorPointForPosition(false);
	registerLayer->setAnchorPoint(Vec2(0.5f, 0.5f));
	registerLayer->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
}

void GameMenu::onPlatformRegistCallback(bool success, bool fastRegist, const std::string& message,const std::string&name, const std::string& pwd, int loginTimes)
{
	LoadingLayer::removeLoading(this);
	auto userDefault = UserDefault::getInstance();

	_gameRegist->stop();

	if (success)
	{
		char title[256] = {0};	
	
		if (0 == loginTimes && !fastRegist)
		{
			userDefault->setBoolForKey(SAVE_TEXT, true);
			_isAccountLogin = true;

			sprintf(title, GBKToUtf8("请牢记你的用户名以及密码。\n用户名称: %s\n用户密码: %s"), name.c_str(), _userPsd.c_str());
			auto prompt = GamePromptLayer::create();
			prompt->showPrompt(title);
			prompt->setCallBack(CC_CALLBACK_0(GameMenu::enterGame, this, name, pwd));			
		}
		else
		{
			enterGame(name, pwd);
		}
	}
	else
	{
		GamePromptLayer::create()->showPrompt(message);
	}
}

void GameMenu::onPlatformLoginCallback(bool success, const std::string& message,const std::string& name, const std::string& pwd)
{
	LoadingLayer::removeLoading(this);
	_gameLogin->stop();

	auto userDefault = UserDefault::getInstance();
	if (success)
	{
		if (_isAccountLogin)
		{
			saveUserInfo(name, pwd);
		}

		GamePlatform::createPlatform();
	}
	else
	{
		GamePromptLayer::create()->showPrompt(message);
	}
}

void GameMenu::saveUserInfo(const std::string& userName, const std::string& userPswd)
{
	auto userDefault = UserDefault::getInstance();

	if (userDefault->getBoolForKey(SAVE_TEXT))
	{
		if (!userName.empty())
		{
			userDefault->setStringForKey(USERNAME_TEXT, userName);
		}

		if (!userPswd.empty())
		{
			userDefault->setStringForKey(PASSWORD_TEXT, userPswd);
		}
	}
	else
	{
		userDefault->setStringForKey(USERNAME_TEXT, "");
		userDefault->setStringForKey(PASSWORD_TEXT, "");
	}
	userDefault->flush();
}

void GameMenu::enterGame(const std::string& userName, const std::string& userPswd)
{
	_gameLogin->start();
	_gameLogin->requestLogin(userName, userPswd);
}

void GameMenu::loadIcon()
{
	auto winSize = Director::getInstance()->getWinSize();
	bool isCustomer = false;
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

			if (doc.IsObject() && doc.HasMember("isCustomer"))
			{
				isCustomer = doc["isCustomer"].GetBool();
			}
		}
	} while (0);

	if (true)
	{
		//std::string gameLogo = PlatformConfig::getInstance()->getGameLogo();
		//std::string gameLogoBg = PlatformConfig::getInstance()->getGameLogoBg();
		//std::string gameLogoTitle = PlatformConfig::getInstance()->getGameLogoTitle();

		//if (FileUtils::getInstance()->isFileExist(gameLogo) && FileUtils::getInstance()->isFileExist(gameLogoBg) && FileUtils::getInstance()->isFileExist(gameLogoTitle))
		{
			_logoBg = Sprite::create(GAME_LOGO_BG);
			_logoBg->setPosition(Vec2(winSize.width *0.3f, winSize.height * 1.0f + _logoBg->getContentSize().height / 2));
			addChild(_logoBg, 2);

			_logo = Sprite::create(GAME_LOGO);
			_logo->setPosition(Vec2(winSize.width *0.3f, winSize.height * 1.0f + _logo->getContentSize().height / 2));
			addChild(_logo, 2);

			_logoTitle = Sprite::create(GAME_LOGO_TITLE);
			_logoTitle->setPosition(Vec2(winSize.width *0.3f, winSize.height * 1.0f + _logoTitle->getContentSize().height / 2));
			addChild(_logoTitle, 2);
		}
	}	
	else
	{
// 		Armature* armature = Armature::create("BirdAnimation");
// 		armature->setScale(0.80f);
// 		armature->setPosition(Vec2(winSize.width / 3, winSize.height * 0.48f));
// 		armature->getAnimation()->play("Animation1");
// 		addChild(armature, 2);
// 
// 		auto textAnima1 = Sprite::create("platform/text1.png");
// 		textAnima1->setAnchorPoint(Vec2(1, 0.5f));
// 		textAnima1->setScale(1.1f);
// 		textAnima1->setPosition(Vec2(0, winSize.height * 0.24f));
// 		addChild(textAnima1, 2);
// 		textAnima1->runAction(Sequence::create(DelayTime::create(0.3f),
// 			EaseBounceOut ::create(MoveBy::create(0.4f, Vec2(winSize.width * 0.34f, 0))), nullptr));
// 
// 		auto textAnima2 = Sprite::create("platform/text2.png");
// 		textAnima2->setAnchorPoint(Vec2(0, 0.5f));
// 		textAnima2->setScale(0.9f);
// 		textAnima2->setPosition(Vec2(winSize.width, winSize.height * 0.17f));
// 		addChild(textAnima2, 2);
// 		textAnima2->runAction(Sequence::create(DelayTime::create(0.5f),
// 			EaseBounceOut ::create(MoveBy::create(0.6f, Vec2(-winSize.width * 0.76f, 0))), nullptr));
	}
}
