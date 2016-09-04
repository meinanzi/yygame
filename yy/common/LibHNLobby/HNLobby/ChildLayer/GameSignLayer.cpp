#include "GameSignLayer.h"
#include "HNLobby/PlatformDefine.h"
#include "HNLobby/PlatformConfig.h"
#include "HNLobby/GamePrompt.h"


static const char* SIGNUI_PATH = "platform/Sign/signUi.csb";

static const char* BOX_CLOSE = "platform/Sign/box_close.png";
static const char* BOX_OPEN = "platform/Sign/box_open.png";
static const char* BOX_EMPTY = "platform/Sign/box_empty.png";

GameSignLayer::GameSignLayer(void)
	: onUpdataUserMoney (nullptr)
	, onCloseCallBack (nullptr)
{
}


GameSignLayer::~GameSignLayer(void)
{	
}

void GameSignLayer::closeSign()
{
	HNAudioEngine::getInstance()->playEffect(GAME_SOUND_CLOSE);
	_SignUi.ImageView_SignBG->runAction(Sequence::create(ScaleTo::create(0.1f, 0.3f), CCCallFunc::create([&]()
	{
		PlatformLogic()->removeEventSelector(MDM_GP_SIGN, ASS_GP_SIGN_DO);
		PlatformLogic()->removeEventSelector(MDM_GP_SIGN, ASS_GP_SIGN_CHECK);
		if (onCloseCallBack != nullptr)
		{
			onCloseCallBack();
		}
		this->removeFromParent();
	}), nullptr));
}

bool GameSignLayer::init()
{
	if ( !HNLayer::init()) return false;
	Size winSize = Director::getInstance()->getWinSize();

	//屏蔽后面的层
	auto colorLayer = LayerColor::create(Color4B(0, 0, 0, 100));
	addChild(colorLayer);

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = [=](Touch* touch ,Event* event){
		return true;
	};
	touchListener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, colorLayer);

	auto node = CSLoader::createNode(SIGNUI_PATH);
	node->setPosition(winSize/2);
	node->setScale(0);
	addChild(node);
	node->runAction(ScaleTo::create( 0.2f, 1.0f));

	_SignUi.ImageView_SignBG = (ImageView*)node->getChildByName("Image_SignBG");
	_SignUi.ImageView_Light = (ImageView*)_SignUi.ImageView_SignBG->getChildByName("Image_Light");
	_SignUi.ImageView_Light->runAction(RepeatForever::create(RotateBy::create(2.0f, 90.0f)));

	_SignUi.Text_Prompt = (Text*)_SignUi.ImageView_SignBG->getChildByName("Text_Prompt");
	_SignUi.Text_Money = (Text*)_SignUi.ImageView_SignBG->getChildByName("Text_Money");

	auto buttonClose = (Button*)_SignUi.ImageView_SignBG->getChildByName("Button_Close");
	buttonClose->addTouchEventListener(CC_CALLBACK_2(GameSignLayer::buttonEventCallBack, this));

	_SignUi.Button_Get = (Button*)_SignUi.ImageView_SignBG->getChildByName("Button_Get");
	_SignUi.Button_Get->addTouchEventListener(CC_CALLBACK_2(GameSignLayer::buttonEventCallBack, this));
	_SignUi.Button_Get->setBright(false);
	_SignUi.Button_Get->setEnabled(false);

	checkSignData();
	return true;
}

//按钮回调
void  GameSignLayer::buttonEventCallBack(Ref* pSender, Widget::TouchEventType type)
{
	Button* selectedBtn = (Button*)pSender;
	auto name = selectedBtn->getName();
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		selectedBtn->setColor(Color3B(170, 170, 170));
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		selectedBtn->setColor(Color3B(255, 255, 255));
		if (name.compare("Button_Close") == 0)
		{
			closeSign();
		}
		if (name.compare("Button_Get") == 0)
		{
			PlatformLogic()->sendData(MDM_GP_SIGN, ASS_GP_SIGN_DO, 0, 0, 
				HN_SOCKET_CALLBACK(GameSignLayer::getSignRewardEventSelector, this));
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		selectedBtn->setColor(Color3B(255, 255, 255));
		break;
	default:
		break;
	}
}

//查询签到信息
void GameSignLayer::checkSignData()
{
	PlatformLogic()->sendData(MDM_GP_SIGN, ASS_GP_SIGN_CHECK, 0, 0, HN_SOCKET_CALLBACK(GameSignLayer::checkSignDataEventSelector, this));
}

//申请查询签到信息回调
bool GameSignLayer::checkSignDataEventSelector(HNSocketMessage* socketMessage)
{
	CCAssert(sizeof(MSG_GP_S_SIGN_CHECK_RESULT) == socketMessage->objectSize, "MSG_GP_S_SIGN_CHECK_RESULT is error.");
	MSG_GP_S_SIGN_CHECK_RESULT* iCheck = (MSG_GP_S_SIGN_CHECK_RESULT*)socketMessage->object;

	char str[64];

	if (iCheck->byCountDay > 7)
	{
		auto prompt = GamePromptLayer::create();
		prompt->showPrompt(GBKToUtf8("查询奖励失败！"));
		prompt->setCallBack(CC_CALLBACK_0(GameSignLayer::closeSign, this));
		return true;
	}
	
	sprintf(str, "您已连续登陆%d天了，获得奖励尚币", iCheck->byCountDay);
	_SignUi.Text_Prompt->setString(GBKToUtf8(str));

	sprintf(str, "%d", iCheck->iGetMoney);
	_SignUi.Text_Money->setString(GBKToUtf8(str));
	for (int i = 1; i <= 7; i++)
	{
		sprintf(str, "Image_box%d", i);
		auto box = (ImageView*)_SignUi.ImageView_SignBG->getChildByName(str);
		if (i < iCheck->byCountDay)
		{ 
			box->loadTexture(BOX_EMPTY);
		}
		else if (i == iCheck->byCountDay)
		{
			if (0 == iCheck->iRs)
			{
				box->loadTexture(BOX_OPEN);
				_SignUi.Button_Get->setBright(true);
				_SignUi.Button_Get->setEnabled(true);
				_SignUi.ImageView_Light->setPosition(box->getPosition());
			}
			else 
			{
				_SignUi.Button_Get->setBright(false);
				_SignUi.Button_Get->setEnabled(false);
				_SignUi.ImageView_Light->setVisible(false);
				box->loadTexture(BOX_EMPTY);
			}
		}
		else
		{
			box->loadTexture(BOX_CLOSE);
		}
	}
	return true;
}

//申请领取签到奖励回调
bool GameSignLayer::getSignRewardEventSelector(HNSocketMessage* socketMessage)
{
	CCAssert(sizeof(MSG_GP_S_SIGN_DO_RESULT) == socketMessage->objectSize, "MSG_GP_S_SIGN_DO_RESULT is error.");
	MSG_GP_S_SIGN_DO_RESULT* iGet = (MSG_GP_S_SIGN_DO_RESULT*)socketMessage->object;

	char str[64];
	if (1 == iGet->iRs)
	{
		PlatformLogic()->loginResult.i64Money += iGet->iGetMoney;
		sprintf(str, "恭喜你领取成功，获得%d尚币奖励！", iGet->iGetMoney);
		if (nullptr != onUpdataUserMoney)
		{
			onUpdataUserMoney(PlatformLogic()->loginResult.i64Money);
		}
		
		auto meteor = ParticleSystemQuad::create("platform/particle/bigwin_blowout_2.plist");
		meteor->setPosition(this->getPosition());
		meteor->setAutoRemoveOnFinish(true);
		Director::getInstance()->getRunningScene()->addChild(meteor, 100000001);
	}
	else
	{
		sprintf(str, "领取失败！", iGet->iGetMoney);
	}

	auto prompt = GamePromptLayer::create();
	prompt->showPrompt(GBKToUtf8(str));
	prompt->setCallBack(CC_CALLBACK_0(GameSignLayer::closeSign, this));

	return true;
}
