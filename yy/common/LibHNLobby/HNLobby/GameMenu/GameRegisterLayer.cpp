#include "HNLobby/GameMenu/GameRegisterLayer.h"
#include "HNLobby/GamePrompt.h"
#include "HNLobby/GameWebView.h"
#include "HNLobby/PlatformConfig.h"
#include "HNLobby/PlatformDefine.h"
#include "HNMarketExport.h"

static const char* REGISTERUI_PATH = "platform/LoginorRegistUi/regist/registUI.csb";


RegisterLayer::RegisterLayer()
	: onRegisterCallBack(nullptr)
	, onCloseCallBack(nullptr)
{	
	memset(&_registUI, 0x0, sizeof(_registUI));
}

RegisterLayer::~RegisterLayer()
{	

}

void RegisterLayer::closeRegister()
{
	_registUI.registBG->runAction(Sequence::create(ScaleTo::create(0.1f, 0.1f), CCCallFunc::create([&]()
	{
		if (nullptr != onCloseCallBack)
		{
			onCloseCallBack();
		}
		this->removeFromParent();
	}), nullptr));
}

bool RegisterLayer::init()
{
	if (!HNLayer::init()) return false;

	//屏蔽后面的层
	auto colorLayer = LayerColor::create(Color4B(0,0,0,100));
	addChild(colorLayer,1);

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = [=](Touch* touch , Event* event){
		return true;
	};
	touchListener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener,colorLayer);

	Size winSize = Director::getInstance()->getWinSize();
	Size frameSize = Director::getInstance()->getOpenGLView()->getFrameSize();

	// 注册界面ui
	_registUI.registUI = CSLoader::createNode(REGISTERUI_PATH);
	_registUI.registUI->setPosition(Vec2(winSize/2));
	addChild(_registUI.registUI, 2, 2);
	auto registLayout = _registUI.registUI->getChildByName("Panel_1");
	
	//登录背景
	_registUI.registBG = (ImageView*)registLayout->getChildByName("Image_registBG");

	// 关闭按钮
	Button * closeBtn = (Button*)_registUI.registBG->getChildByName("Button_close");
	closeBtn->addTouchEventListener(CC_CALLBACK_2(RegisterLayer::closeClickCallback, this));

	// 注册按钮
	Button * buttonRegister = (Button*)_registUI.registBG->getChildByName("Button_registOK");
	buttonRegister->addTouchEventListener(CC_CALLBACK_2(RegisterLayer::registerClickCallback, this));

	// 重置按钮
	Button * buttonReset = (Button*)_registUI.registBG->getChildByName("Button_reset");
	buttonReset->addTouchEventListener(CC_CALLBACK_2(RegisterLayer::resetClickCallBack, this));

	//获取代理商按钮
	Button * buttonGetAgency = (Button*)_registUI.registBG->getChildByName("Button_GetAgency");
	buttonGetAgency->addTouchEventListener(CC_CALLBACK_2(RegisterLayer::getAgencyClickCallBack, this));

	// 账号输入框
	auto textField_UserName = (TextField*)_registUI.registBG->getChildByName("TextField_userName");
	textField_UserName->setVisible(false);
	_registUI.editBoxUserName = HNEditBox::createEditBox(textField_UserName, this);

	// 密码输入框
	auto textField_PassWord = (TextField*)_registUI.registBG->getChildByName("TextField_passWord");
	textField_PassWord->setVisible(false);
	_registUI.editBoxPassWord = HNEditBox::createEditBox(textField_PassWord, this);
	_registUI.editBoxPassWord->setPasswordEnabled(true);

	//代理商输入框
	auto textField_Agency = (TextField*)_registUI.registBG->getChildByName("TextField_angency");
	textField_Agency->setVisible(false);
	_registUI.editBoxAgency = HNEditBox::createEditBox(textField_Agency, this);

	// 同意复选框
	_registUI.agree = (CheckBox*)_registUI.registBG->getChildByName("CheckBox_agree");
	_registUI.agree->setSelected(false);

	// 显示密码复选框
	_registUI.showPass = (CheckBox*)_registUI.registBG->getChildByName("CheckBox_showPass");
	_registUI.showPass->setSelected(false);
	_registUI.showPass->addEventListener(CheckBox::ccCheckBoxCallback(CC_CALLBACK_2(RegisterLayer::checkBoxCallback, this)));
	
	//协议按钮
	Button * buttonagree = (Button*)_registUI.registBG->getChildByName("Button_agree");
	buttonagree->addTouchEventListener(CC_CALLBACK_2(RegisterLayer::AgreeClickCallBack, this));

	

	return true;
}

void RegisterLayer::registerClickCallback(Ref* pSender, Widget::TouchEventType type)
{
	if (Widget::TouchEventType::ENDED != type) return;
	auto btn = (Button*)pSender;

	if (!_registUI.agree->isSelected())
	{
		GamePromptLayer::create()->showPrompt(GBKToUtf8("请勾选同意用户协议！"));
		return;
	}	

	HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);

	do 
	{
		// 获取输入框账号
		std::string userName = _registUI.editBoxUserName->getString();

		if (userName.empty())
		{
			GamePromptLayer::create()->showPrompt(GBKToUtf8("账号不能为空！"));
			break;
		}	

		if (_registUI.editBoxUserName->getStringLength() < 6 ||
			_registUI.editBoxUserName->getStringLength() > 12 ||
			!Tools::verifyNumberAndEnglish(userName))
		{
			GamePromptLayer::create()->showPrompt(GBKToUtf8("请输入6-12位英文或数字帐号！"));
			_registUI.editBoxUserName->setString("");
			break;
		}

		// 获取输入框密码
		std::string passWord = _registUI.editBoxPassWord->getString();
		if (passWord.empty())
		{
			GamePromptLayer::create()->showPrompt(GBKToUtf8("密码不能为空！"));
			break;
		}

		if (_registUI.editBoxPassWord->getStringLength() < 6 || 
			_registUI.editBoxPassWord->getStringLength() > 16 ||
			!Tools::verifyNumberAndEnglish(passWord))
		{
			GamePromptLayer::create()->showPrompt(GBKToUtf8("请输入6-16位英文或数字密码！"));
			_registUI.editBoxPassWord->setString("");
			break;
		}

		//获取代理商
		std::string agency = _registUI.editBoxAgency->getString();
		if (agency.empty())
		{
			GamePromptLayer::create()->showPrompt(GBKToUtf8("代理商账号不能为空！"));
			break;
		}
	
		if (nullptr != onRegisterCallBack)
		{
			onRegisterCallBack(userName, passWord, agency);
		}

	} while (0);
}

void RegisterLayer::resetClickCallBack(Ref* pSender, Widget::TouchEventType type)
{
	if (Widget::TouchEventType::ENDED != type) return;
	auto btn = (Button*)pSender;
	HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);

	_registUI.editBoxUserName->setText("");
	_registUI.editBoxPassWord->setText("");
	_registUI.editBoxAgency->setText("");
}

void RegisterLayer::getAgencyClickCallBack(Ref* pSender, Widget::TouchEventType type)
{
	if (Widget::TouchEventType::ENDED != type) return;
	GamePromptLayer::create()->showPrompt(GBKToUtf8("代理商名见推广二维码下方!"));
}

void RegisterLayer::closeClickCallback(Ref* pSender, Widget::TouchEventType type)
{
	Button* selectedBtn = (Button*)pSender;
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		selectedBtn->setColor(Color3B(170, 170, 170));
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		{
			HNAudioEngine::getInstance()->playEffect(GAME_SOUND_CLOSE);
			selectedBtn->setColor(Color3B(255, 255, 255));
			closeRegister();
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		selectedBtn->setColor(Color3B(255, 255, 255));
		break;
	default:
		break;
	}
}

void RegisterLayer::AgreeClickCallBack(Ref* pSender, Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)		return;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	auto webViewLayer = GameWebViewLayer::create();
	webViewLayer->setTitle(GBKToUtf8("用户协议"));
	webViewLayer->showWebView(PlatformConfig::getInstance()->getProtocolUrl());
#endif
	
}

void RegisterLayer::checkBoxCallback(Ref* pSender, CheckBox::EventType type)
{
	_registUI.editBoxPassWord->setPasswordEnabled(_registUI.editBoxPassWord->isPasswordEnabled() ? false : true);
	_registUI.editBoxPassWord->setString(_registUI.editBoxPassWord->getString());
}

