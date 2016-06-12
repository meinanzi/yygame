#include "HNLobby/PersionalCenter/BindPhone.h"
#include "HNLobby/FontConfig.h"
#include "HNLobby/PlatformDefine.h"
#include "HNLobby/GamePrompt.h"
#include "HNLobby/PlatformConfig.h"
#include <string>

static const char* BINDPHONE_JSON	= "platform/bindPhone/bindPhoneNode.csb";
static const char* LIMITTIME_BTN    = "platform/bindPhone/bindPhoneRes/limitTimeBtn.png";
static const char* GETCODE_BTN = "platform/bindPhone/bindPhoneRes/code_d.png";

BindPhoneLayer::BindPhoneLayer(): _restTime(60)
{
	memset(&_ui, 0x0, sizeof(_ui));
}

BindPhoneLayer::~BindPhoneLayer()
{
	PlatformLogic()->removeEventSelector(MDM_GP_SMS, ASS_GP_SMS_VCODE);
	PlatformLogic()->removeEventSelector(MDM_GP_BIND_MOBILE, ASS_GP_BIND_MOBILE);
}

void BindPhoneLayer::show(Node* parent, int zorder, int tag)
{
	CCAssert(nullptr != parent, "parent is nullptr");
	parent->addChild(this, zorder, tag);
}

void BindPhoneLayer::close()
{
	HNAudioEngine::getInstance()->playEffect(GAME_SOUND_CLOSE);
	_ui.bindLoader->runAction(Sequence::create(ScaleTo::create(0.1f, 0.3f),  CCCallFunc::create([&]()
	{
		this->removeFromParent();
	}), nullptr));
}

bool BindPhoneLayer::init()
{
    if ( !HNLayer::init()) return false;
	Size winSize = Director::getInstance()->getWinSize();

	//屏蔽后面的层
	auto colorLayer = LayerColor::create(Color4B(0, 0, 0, 100));
	addChild(colorLayer,1);

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = [=](Touch* touch ,Event* event){
		return true;
	};
	touchListener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener,colorLayer);

	// 玩家资料框背景
	_ui.bindLoader = CSLoader::createNode(BINDPHONE_JSON);
	_ui.bindLoader->setScale( 0 );
	_ui.bindLoader->runAction(ScaleTo::create(0.2f, 1.0f));
	_ui.bindLoader->setPosition(winSize/ 2);
	addChild(_ui.bindLoader, 2);
	auto panel = (Layout*)_ui.bindLoader->getChildByName("Panel_1");
	_ui.layout = (ImageView*)panel->getChildByName("Image_1");

	//关闭按钮
	_ui.Button_Cancel = (Button* )_ui.layout->getChildByName("Button_close");
	_ui.Button_Cancel ->addTouchEventListener(CC_CALLBACK_2(BindPhoneLayer::closeCallBack, this));

	//确定按钮
	_ui.Button_OK = (Button* )_ui.layout->getChildByName("Button_sure");
	_ui.Button_OK ->addTouchEventListener(CC_CALLBACK_2(BindPhoneLayer::okEventCallBack, this));

	//获取验证码
	_ui.Button_Code= (Button* )_ui.layout->getChildByName("Button_getCode");
	_ui.Button_Code ->addTouchEventListener(CC_CALLBACK_2(BindPhoneLayer::verifyCodeUIEventCallBack, this));
	auto contentSize = _ui.Button_Code->getContentSize();

	//倒计时
	_timeText = Text::create();
	_timeText->setFontSize(20);
	_timeText->setTextColor(Color4B::WHITE);
	_timeText->setPosition(Vec2(contentSize.width / 2, contentSize.height / 2));
	_ui.Button_Code->addChild(_timeText);
	_timeText->setString(GBKToUtf8("60s后再次获取"));
	
	//手机号输入框 
	auto PhoneNumber = (TextField*)_ui.layout->getChildByName("TextField_phone");
	PhoneNumber->setVisible(false);
	_ui.TextField_PhoneNumber= HNEditBox::createEditBox(PhoneNumber, this);

	//验证码输入框
	auto code = (TextField*) _ui.layout->getChildByName("TextField_code");
	code->setVisible(false);
	_ui.TextField_Code = HNEditBox::createEditBox(code, this);

	auto limitTime = UserDefault::getInstance()->getStringForKey("LimitStartTime", "noTime");
	//为"noTime"表示现在没有获取验证码
	if (limitTime != "noTime")
	{
		//获取当前时间，用来和保存的时间比较，如果时间间隔小于60s,就让获取验证码按钮不可点击，否则可点击
		time_t tt;
		time(&tt);
		struct tm * now;
		now = localtime(&tt);
		std::string nowTime = StringUtils::format("%02d%02d%02d%02d", now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec);
		int nowtime = atoi(nowTime.c_str());
		int oldtime = atoi(limitTime.c_str());
		int intervalTime = nowtime - oldtime;
		if (intervalTime > 0 &&  intervalTime <= 60)
		{
			//显示当前绑定的手机号
			_ui.TextField_PhoneNumber->setString(UserDefault::getInstance()->getStringForKey("Mobilephone", " "));
			_verifyCode = UserDefault::getInstance()->getStringForKey("curVCode", " ");
			//显示倒计时的时间
			_restTime = 60 - intervalTime;
			char str[64];
			sprintf(str, "%ds后再次获取", _restTime);
			_timeText->setString(GBKToUtf8(str));
			setLimitTime();
		}
		else
		{
			_ui.Button_Code->setTouchEnabled(true);
			_ui.Button_Code->setBright(true);
			_timeText->setVisible(false);
		}
	}
	else
	{
		_timeText->setVisible(false);
	}
	return true;
}

void BindPhoneLayer::okEventCallBack(Ref* pSender, Widget::TouchEventType type)
{
	if (Widget::TouchEventType::ENDED != type) return;

	std::string phoneNumber = _ui.TextField_PhoneNumber->getString();
	std::string verifyCode = MD5_CTX::MD5String(_ui.TextField_Code->getString());
	do
	{
		if (phoneNumber.empty())
		{
			GamePromptLayer::create()->showPrompt(GBKToUtf8("请输入电话号码。")); break;
		}

		if (verifyCode.empty())
		{
			GamePromptLayer::create()->showPrompt(GBKToUtf8("请输入验证码。")); break;
		}

		if (0 != verifyCode.compare(_verifyCode))
		{
			GamePromptLayer::create()->showPrompt(GBKToUtf8("验证码错误，请重新输入。")); break;
		}

		MSG_GP_S_BindMobile bindMobile;
		bindMobile.dwUserID = PlatformLogic()->loginResult.dwUserID;
		bindMobile.dwCommanType = 1;
		strcpy(bindMobile.szMobileNo, phoneNumber.c_str());
		PlatformLogic()->sendData(MDM_GP_BIND_MOBILE, ASS_GP_BIND_MOBILE, &bindMobile, sizeof(bindMobile),
			HN_SOCKET_CALLBACK(BindPhoneLayer::bindPhoneSelector, this));
	} while (0);
}

void BindPhoneLayer::verifyCodeUIEventCallBack(Ref* pSender, Widget::TouchEventType type)
{
	if (Widget::TouchEventType::ENDED != type) return;

	std::string phoneNumber = _ui.TextField_PhoneNumber->getString();
	do
	{
		if (phoneNumber.empty())
		{
			GamePromptLayer::create()->showPrompt(GBKToUtf8("请输入手机号码。")); break;
		}
		
		MSG_GP_SmsVCode SmsVCode;
		SmsVCode.nType = 4;
		strcpy(SmsVCode.szName, PlatformLogic()->loginResult.szName);
		strcpy(SmsVCode.szMobileNo, phoneNumber.c_str());
		PlatformLogic()->sendData(MDM_GP_SMS, ASS_GP_SMS_VCODE, &SmsVCode, sizeof(SmsVCode), HN_SOCKET_CALLBACK(BindPhoneLayer::verifyCodeSelector, this));
	
		//保存当前时间，用于关闭这个界面后，再次进入获取验证码按钮的状态
		time_t tt;
		time(&tt);
		struct tm * now;
		now = localtime(&tt);
		std::string startTime = StringUtils::format("%02d%02d%02d%02d", now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec);
		UserDefault::getInstance()->setStringForKey("LimitStartTime", startTime);
		
		_restTime = 60;
		//设置60s内获取按钮不可点
		setLimitTime();
		//暂时保存手机号
		UserDefault::getInstance()->setStringForKey("Mobilephone", _ui.TextField_PhoneNumber->getString());

	} while (0);
}

bool BindPhoneLayer::verifyCodeSelector(HNSocketMessage* socketMessage)
{
	CCAssert(sizeof (MSG_GP_SmsVCode) == socketMessage->objectSize, "MSG_GP_SmsVCode is error.");
	if (socketMessage->objectSize != sizeof(MSG_GP_SmsVCode)) return true;

	MSG_GP_SmsVCode *smsVCode = (MSG_GP_SmsVCode *)socketMessage->object;

	_verifyCode = smsVCode->szVCode;
	UserDefault::getInstance()->setStringForKey("curVCode", _verifyCode);
	return true;
}

bool BindPhoneLayer::bindPhoneSelector(HNSocketMessage* socketMessage)
{
	CCAssert(sizeof (MSG_GP_R_BindMobile) == socketMessage->objectSize, "MSG_GP_R_BindMobile is error.");
	if (socketMessage->objectSize != sizeof(MSG_GP_R_BindMobile)) return true;

	MSG_GP_R_BindMobile *bindMobile = (MSG_GP_R_BindMobile *)socketMessage->object;

	// 请求解除绑定
	if(bindMobile->dwCommanType == 0)
	{
		if(bindMobile->dwCommanResult == 0)
		{
			GamePromptLayer::create()->showPrompt(GBKToUtf8("手机解除绑定成功。"));
			PlatformLogic()->loginResult.iBindMobile = 0;
			memset(PlatformLogic()->loginResult.szMobileNo, 0x00, sizeof(PlatformLogic()->loginResult.szMobileNo));
			updateUserDetail();
		}
		else
		{
			GamePromptLayer::create()->showPrompt(GBKToUtf8("手机解除绑定失败。"));
		}
	}

	// 请求绑定
	else if(bindMobile->dwCommanType == 1)
	{
		if(bindMobile->dwCommanResult == 0)
		{
			auto prompt = GamePromptLayer::create();
			prompt->showPrompt(GBKToUtf8("手机绑定成功。"));
			PlatformLogic()->loginResult.iBindMobile = 1;
			memcpy(PlatformLogic()->loginResult.szMobileNo, _ui.TextField_PhoneNumber->getString().c_str(), sizeof(PlatformLogic()->loginResult.szMobileNo));
			updateUserDetail();
			prompt->setCallBack(CC_CALLBACK_0(BindPhoneLayer::close, this));
		}
		else
		{
			GamePromptLayer::create()->showPrompt(GBKToUtf8("手机绑定失败。"));
		}
	}

	_restTime = 0;

	return true;
}

void BindPhoneLayer::updateUserDetail()
{
	MSG_GP_UserInfo userInfo = PlatformLogic()->loginResult;
	std::string phoneNumber = _ui.TextField_PhoneNumber->getString();
	strcpy(userInfo.szMobileNo, phoneNumber.c_str());
	PlatformLogic()->sendData(MDM_GP_USERINFO, ASS_GP_USERINFO_UPDATE_DETAIL, &userInfo, sizeof(userInfo));

	PlatformLogic()->addEventSelector(MDM_GP_USERINFO, ASS_GP_USERINFO_ACCEPT, HN_SOCKET_CALLBACK(BindPhoneLayer::modifyUserInfoSelector, this));
	PlatformLogic()->addEventSelector(MDM_GP_USERINFO, ASS_GP_USERINFO_NOTACCEPT, HN_SOCKET_CALLBACK(BindPhoneLayer::modifyUserInfoSelector, this));
}

bool BindPhoneLayer::modifyUserInfoSelector(HNSocketMessage* socketMessage)
{
	if (ASS_GP_USERINFO_ACCEPT == socketMessage->messageHead.bAssistantID)	// 服务端已接受
	{
		std::string phoneNumber = _ui.TextField_PhoneNumber->getString();
		strcpy(PlatformLogic()->loginResult.szMobileNo, phoneNumber.c_str());

		UserInfoStruct* userinfo = UserInfoModule()->findUser(PlatformLogic()->loginResult.dwUserID);
		if (nullptr != userinfo)
		{
			strcpy(userinfo->szPhoneNum, phoneNumber.c_str());
			UserInfoModule()->updateUser(userinfo);
		}
	}
	else if (ASS_GP_USERINFO_NOTACCEPT == socketMessage->messageHead.bAssistantID)	// 服务端未能接受
	{
		
	}
	else
	{

	}
	return true;
}

void BindPhoneLayer::closeCallBack(Ref* pSender, Widget::TouchEventType type)
{
	Button* selectedBtn = (Button*)pSender;
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		selectedBtn->setColor(Color3B(170, 170, 170));
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		selectedBtn->setColor(Color3B(255, 255, 255));
		close();
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		selectedBtn->setColor(Color3B(255, 255, 255));
		break;
	default:
		break;
	}
}

//获取验证码的时间限制
void BindPhoneLayer::setLimitTime()
{
	this->schedule(schedule_selector(BindPhoneLayer::updateLimitTime), 1.0f);

	_ui.Button_Code->loadTextureNormal(LIMITTIME_BTN);
	_timeText->setVisible(true);
	_ui.Button_Code->setTouchEnabled(false);
	_ui.Button_Code->setBright(false);
}

void BindPhoneLayer::updateLimitTime(float dt)
{
	char str[64];
	_restTime -= 1;
	if (_restTime < 0)
	{
		this->unschedule(schedule_selector(BindPhoneLayer::updateLimitTime));
		UserDefault::getInstance()->setStringForKey("LimitStartTime", "noTime");
		UserDefault::getInstance()->setStringForKey("Mobilephone", "noMobilePhone");
		_ui.Button_Code->loadTextureNormal(GETCODE_BTN);
		_ui.Button_Code->setTouchEnabled(true);
		_ui.Button_Code->setBright(true);
		_timeText->setVisible(false);
	}
	else
	{
		sprintf(str, "%ds后再次获取", _restTime);
		_timeText->setString(GBKToUtf8(str));
	}
}