#include "GameOnlineReward.h"
#include "HNLobby/PlatformDefine.h"
#include "HNLobby/PlatformConfig.h"
#include "HNLobby/GamePrompt.h"


static const char* ONLINEUI_PATH = "platform/onlineReward/Node_onlineReward.csb";

GameOnlineReward::GameOnlineReward(void)
	: onUpdataUserMoney (nullptr)
	, onCloseCallBack (nullptr)
{

}

GameOnlineReward::~GameOnlineReward(void)
{
	
}

void GameOnlineReward::closeOnlineReward()
{
	HNAudioEngine::getInstance()->playEffect(GAME_SOUND_CLOSE);
	_OnlineRewardUi.ImageView_OnlineBG->runAction(Sequence::create(ScaleTo::create(0.1f, 0.3f), CCCallFunc::create([&]()
	{
		PlatformLogic()->removeEventSelector(MDM_GP_ONLINE_AWARD, ASS_GP_ONLINE_AWARD_CHECK);
		PlatformLogic()->removeEventSelector(MDM_GP_ONLINE_AWARD, ASS_GP_ONLINE_AWARD_DO);
		if (nullptr != onCloseCallBack)
		{
			onCloseCallBack();
		}
		this->removeFromParent();
	}), nullptr));
}

bool GameOnlineReward::init()
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

	auto node = CSLoader::createNode(ONLINEUI_PATH);
	node->setPosition(winSize/2);
	node->setScale(0);
	addChild(node);
	node->runAction(ScaleTo::create( 0.2f, 1.0f));

	_OnlineRewardUi.ImageView_OnlineBG = (ImageView*)node->getChildByName("Image_RewardBG");

	_OnlineRewardUi.Layout_Wait = (Layout*)_OnlineRewardUi.ImageView_OnlineBG->getChildByName("Panel_Wait");
	_OnlineRewardUi.Layout_Get = (Layout*)_OnlineRewardUi.ImageView_OnlineBG->getChildByName("Panel_Get");

	_OnlineRewardUi.ImageView_Light = (ImageView*)_OnlineRewardUi.Layout_Get->getChildByName("Image_Light");
	_OnlineRewardUi.ImageView_Light->runAction(RepeatForever::create(RotateBy::create(2.5f, 90.0f)));

	_OnlineRewardUi.Text_Time = (Text*)_OnlineRewardUi.Layout_Wait->getChildByName("Text_Time");
	_OnlineRewardUi.Text_Money = (Text*)_OnlineRewardUi.Layout_Wait->getChildByName("Text_Money");

	auto buttonClose = (Button*)_OnlineRewardUi.ImageView_OnlineBG->getChildByName("Button_Close");
	buttonClose->addTouchEventListener(CC_CALLBACK_2(GameOnlineReward::buttonEventCallBack, this));

	_OnlineRewardUi.Button_Get = (Button*)_OnlineRewardUi.ImageView_OnlineBG->getChildByName("Button_Get");
	_OnlineRewardUi.Button_Get->addTouchEventListener(CC_CALLBACK_2(GameOnlineReward::buttonEventCallBack, this));
	_OnlineRewardUi.Button_Get->setEnabled(false);
	_OnlineRewardUi.Button_Get->setBright(false);

	checkRewardData();
	return true;
}

//按钮回调
void  GameOnlineReward::buttonEventCallBack(Ref* pSender, Widget::TouchEventType type)
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
			closeOnlineReward();
		}
		if (name.compare("Button_Get") == 0)
		{
			PlatformLogic()->sendData(MDM_GP_ONLINE_AWARD, ASS_GP_ONLINE_AWARD_DO, 0, 0, 
				HN_SOCKET_CALLBACK(GameOnlineReward::getOnlineRewardEventSelector, this));
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		selectedBtn->setColor(Color3B(255, 255, 255));
		break;
	default:
		break;
	}
}

//查询在线奖励信息
void GameOnlineReward::checkRewardData()
{
	PlatformLogic()->sendData(MDM_GP_ONLINE_AWARD, ASS_GP_ONLINE_AWARD_CHECK, 0, 0, 
		HN_SOCKET_CALLBACK(GameOnlineReward::checkRewardDataEventSelector, this));
}

//申请查询在线奖励信息回调
bool GameOnlineReward::checkRewardDataEventSelector(HNSocketMessage* socketMessage)
{
	CCAssert(sizeof(MSG_GP_S_ONLINE_AWARD_CHECK_RESULT) == socketMessage->objectSize, "MSG_GP_S_ONLINE_AWARD_CHECK_RESULT is error.");
	MSG_GP_S_ONLINE_AWARD_CHECK_RESULT* iCheck = (MSG_GP_S_ONLINE_AWARD_CHECK_RESULT*)socketMessage->object;

	if (1 == socketMessage->messageHead.bHandleCode)
	{
		_Time = iCheck->iLeftTime;

		if (_Time > 0)
		{
			char str[12];
			sprintf(str, "%u", iCheck->iOnLineTimeMoney);
			_OnlineRewardUi.Text_Money->setString(str);
			_OnlineRewardUi.Button_Get->setEnabled(false);
			_OnlineRewardUi.Button_Get->setBright(false);
			schedule(schedule_selector(GameOnlineReward::updataGetRewardTimeMessage), 1.0f);
		}
		else if (0 == _Time && iCheck->iOnLineTimeMoney > 0)
		{
			_OnlineRewardUi.Layout_Wait->setVisible(false);
			_OnlineRewardUi.Layout_Get->setVisible(true);
			_OnlineRewardUi.Button_Get->setEnabled(true);
			_OnlineRewardUi.Button_Get->setBright(true);
		}		
	}
	else
	{
		GamePromptLayer::create()->showPrompt(GBKToUtf8("查询失败!"));
	}
	
	return true;
}

//申请领取在线奖励回调
bool GameOnlineReward::getOnlineRewardEventSelector(HNSocketMessage* socketMessage)
{
	CCAssert(sizeof(MSG_GP_S_ONLINE_AWARD_DO_RESULT) == socketMessage->objectSize, "MSG_GP_S_ONLINE_AWARD_DO_RESULT is error.");
	MSG_GP_S_ONLINE_AWARD_DO_RESULT* iGet = (MSG_GP_S_ONLINE_AWARD_DO_RESULT*)socketMessage->object;

	char str[64];
	if (1 == socketMessage->messageHead.bHandleCode)
	{
		_Time = iGet->iNextTime;
		sprintf(str, "%u", iGet->iNextMoney);
		_OnlineRewardUi.Text_Money->setString(str);
		_OnlineRewardUi.Button_Get->setEnabled(false);
		_OnlineRewardUi.Button_Get->setBright(false);
		_OnlineRewardUi.Layout_Wait->setVisible(true);
		_OnlineRewardUi.Layout_Get->setVisible(false);
		schedule(schedule_selector(GameOnlineReward::updataGetRewardTimeMessage), 1.0f);

		PlatformLogic()->loginResult.i64Money += iGet->iCurrentGetMoney;
		sprintf(str, "恭喜你领取成功，获得%u金币奖励！", iGet->iCurrentGetMoney);
		if (nullptr != onUpdataUserMoney)
		{
			onUpdataUserMoney(PlatformLogic()->loginResult.i64Money);
		}
		auto prompt = GamePromptLayer::create();
		prompt->showPrompt(GBKToUtf8(str));

		auto meteor = ParticleSystemQuad::create("platform/particle/bigwin_blowout_2.plist");
		meteor->setPosition(this->getPosition());
		meteor->setAutoRemoveOnFinish(true);
		Director::getInstance()->getRunningScene()->addChild(meteor, 100000001);
		//prompt->setCallBack(CC_CALLBACK_0(GameOnlineReward::closeOnlineReward, this));
	}
	else
	{
		GamePromptLayer::create()->showPrompt(GBKToUtf8("领取失败!"));
	}

	return true;
}

//领奖倒计时
void GameOnlineReward::updataGetRewardTimeMessage(float dt)
{
	char str[32];
	INT h = _Time / 3600;
	INT m = (_Time % 3600) / 60;
	INT s = (_Time % 3600) % 60;
	sprintf(str, "%02d:%02d:%02d", h, m, s);

	_OnlineRewardUi.Text_Time->setString(str);

	_Time--;
	if (_Time < 0)
	{
		unschedule(schedule_selector(GameOnlineReward::updataGetRewardTimeMessage));
		_OnlineRewardUi.Layout_Wait->setVisible(false);
		_OnlineRewardUi.Layout_Get->setVisible(true);
		_OnlineRewardUi.Button_Get->setEnabled(true);
		_OnlineRewardUi.Button_Get->setBright(true);
	}
}
