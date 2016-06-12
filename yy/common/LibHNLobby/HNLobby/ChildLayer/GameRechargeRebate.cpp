#include "GameRechargeRebate.h"
#include "HNLobby/PlatformDefine.h"
#include "HNLobby/PlatformConfig.h"
#include "HNLobby/GamePrompt.h"


static const char* REBATE_PATH = "platform/RechargeRebate/Node_Rebate.csb";


GameRechargeRebate::GameRechargeRebate(void)
	: onUpdataUserMoney (nullptr)
	, onCloseCallBack (nullptr)
{
}

GameRechargeRebate::~GameRechargeRebate(void)
{
	
}

void GameRechargeRebate::closeRebate()
{
	HNAudioEngine::getInstance()->playEffect(GAME_SOUND_CLOSE);
	_RebateUi.ImageView_RebateBG->runAction(Sequence::create(ScaleTo::create(0.1f, 0.3f), CCCallFunc::create([&]()
	{
		PlatformLogic()->removeEventSelector(MDM_GP_NOTIFY_PAY, ASS_GP_GET_MONEY_BY_PAY_CHECK);
		PlatformLogic()->removeEventSelector(MDM_GP_NOTIFY_PAY, ASS_GP_GET_MONEY_BY_PAY_DO);
		if (nullptr != onCloseCallBack)
		{
			onCloseCallBack();
		}
		this->removeFromParent();
	}), nullptr));
}

bool GameRechargeRebate::init()
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

	auto node = CSLoader::createNode(REBATE_PATH);
	node->setPosition(Vec2(winSize.width * 0.46f, winSize.height / 2));
	node->setScale(0);
	addChild(node);
	node->runAction(ScaleTo::create( 0.2f, 1.0f));

	_RebateUi.ImageView_RebateBG = (ImageView*)node->getChildByName("Image_RebateBG");

	_RebateUi.Text_RechMoney = (Text*)_RebateUi.ImageView_RebateBG->getChildByName("Text_RechMoney");
	_RebateUi.Text_RebateMoney = (Text*)_RebateUi.ImageView_RebateBG->getChildByName("Text_RebateMoney");

	auto buttonClose = (Button*)_RebateUi.ImageView_RebateBG->getChildByName("Button_Close");
	buttonClose->addTouchEventListener(CC_CALLBACK_2(GameRechargeRebate::buttonEventCallBack, this));

	_RebateUi.Button_Get = (Button*)_RebateUi.ImageView_RebateBG->getChildByName("Button_Get");
	_RebateUi.Button_Get->addTouchEventListener(CC_CALLBACK_2(GameRechargeRebate::buttonEventCallBack, this));
	_RebateUi.Button_Get->setEnabled(false);
	_RebateUi.Button_Get->setBright(false);

	checkRebateData();

	return true;
}

//按钮回调
void  GameRechargeRebate::buttonEventCallBack(Ref* pSender, Widget::TouchEventType type)
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
			closeRebate();
		}
		if (name.compare("Button_Get") == 0)
		{
			PlatformLogic()->sendData(MDM_GP_NOTIFY_PAY, ASS_GP_GET_MONEY_BY_PAY_DO, 0, 0, 
				HN_SOCKET_CALLBACK(GameRechargeRebate::getRebateEventSelector, this));
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		selectedBtn->setColor(Color3B(255, 255, 255));
		break;
	default:
		break;
	}
}

//查询充值返利奖励信息
void GameRechargeRebate::checkRebateData()
{
	PlatformLogic()->sendData(MDM_GP_NOTIFY_PAY, ASS_GP_GET_MONEY_BY_PAY_CHECK, 0, 0, 
		HN_SOCKET_CALLBACK(GameRechargeRebate::checkRebateDataEventSelector, this));
}

//申请查询充值返利奖励信息回调
bool GameRechargeRebate::checkRebateDataEventSelector(HNSocketMessage* socketMessage)
{
	CCAssert(sizeof(MSG_GP_S_GetMoney_ByPay) == socketMessage->objectSize, "MSG_GP_S_GetMoney_ByPay is error.");
	MSG_GP_S_GetMoney_ByPay* iCheck = (MSG_GP_S_GetMoney_ByPay*)socketMessage->object;

	if (1 == socketMessage->messageHead.bHandleCode)
	{
		if (iCheck->i64Money > 0)
		{
			_RebateUi.Button_Get->setEnabled(true);
			_RebateUi.Button_Get->setBright(true);
		}
		else
		{
			_RebateUi.Button_Get->setEnabled(false);
			_RebateUi.Button_Get->setBright(false);
		}
		char str[32];
		sprintf(str, "%lld", iCheck->i64Money);
		_RebateUi.Text_RebateMoney->setString(str);
		sprintf(str, "%lld", iCheck->i64TotalMoney);
		_RebateUi.Text_RechMoney->setString(str);
	}
	else
	{
		GamePromptLayer::create()->showPrompt(GBKToUtf8("查询失败!"));
	}
	return true;
}

//申请领取充值返利奖励回调
bool GameRechargeRebate::getRebateEventSelector(HNSocketMessage* socketMessage)
{
	CCAssert(sizeof(MSG_GP_S_GetMoney_ByPay) == socketMessage->objectSize, "MSG_GP_S_GetMoney_ByPay is error.");
	MSG_GP_S_GetMoney_ByPay* iGet = (MSG_GP_S_GetMoney_ByPay*)socketMessage->object;

	char str[64];
	if (1 == socketMessage->messageHead.bHandleCode)
	{
		PlatformLogic()->loginResult.i64Money += iGet->i64Money;
		sprintf(str, "恭喜你领取成功，获得%d金币奖励！", iGet->i64Money);
		if (nullptr != onUpdataUserMoney)
		{
			onUpdataUserMoney(PlatformLogic()->loginResult.i64Money);
		}
		auto prompt = GamePromptLayer::create();
		prompt->showPrompt(GBKToUtf8(str));
		prompt->setCallBack(CC_CALLBACK_0(GameRechargeRebate::closeRebate, this));

		auto meteor = ParticleSystemQuad::create("platform/particle/bigwin_blowout_2.plist");
		meteor->setPosition(this->getPosition());
		meteor->setAutoRemoveOnFinish(true);
		Director::getInstance()->getRunningScene()->addChild(meteor, 100000001);
	}
	else
	{
		GamePromptLayer::create()->showPrompt(GBKToUtf8("领取失败!"));
	}

	return true;
}

