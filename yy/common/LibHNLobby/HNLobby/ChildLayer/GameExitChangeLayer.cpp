#include "HNLobby/ChildLayer/GameExitChangeLayer.h"
#include "HNLobby/PlatformDefine.h"
#include "HNLobby/PlatformConfig.h"

GameExitChangeLayer::GameExitChangeLayer() 
{
	onExitCallBack = nullptr;
}

GameExitChangeLayer::~GameExitChangeLayer()
{

}

void GameExitChangeLayer::showExitChoose(Node* parent, int zorder, int tag)
{
	CCAssert(nullptr != parent, "parent is nullptr");
	if (0 != tag)
	{
		parent->addChild(this, zorder, tag);
	}
	else
	{
		parent->addChild(this, zorder);
	}
}

void GameExitChangeLayer::closeExitChoose()
{
	HNAudioEngine::getInstance()->playEffect(GAME_SOUND_CLOSE);
	exitChooseLoader->runAction(Sequence::create(ScaleTo::create(0.1f, 0.3f), CCCallFunc::create([&]()
	{
		this->removeFromParent();
	}), nullptr));
}

bool GameExitChangeLayer::init()
{
	if ( !HNLayer::init()) return false;

	Size winSize = Director::getInstance()->getWinSize();

	//屏蔽后面的层
	colorLayer = LayerColor::create(Color4B(0, 0, 0, 100));
	addChild(colorLayer);
	//事件吞并
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = [=](Touch* touch , Event* event){
		return true;
	};
	touchListener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, colorLayer);

	exitChooseLoader = CSLoader::createNode("platform/exitChange/exitNode.csb");
	exitChooseLoader->setPosition(winSize/2);
	exitChooseLoader->setScale(0);
	addChild(exitChooseLoader);
	exitChooseLoader->runAction(ScaleTo::create(0.2f, 1));
	auto exitChooseLayout = (Layout*)exitChooseLoader->getChildByName("Panel_1");

	//关闭
	auto chooseClose = (Button*)exitChooseLayout->getChildByName("Button_close");
	chooseClose->addTouchEventListener(CC_CALLBACK_2(GameExitChangeLayer::closeExitChooseCallBack, this));
	//退出
	auto chooseExit = (Button*)exitChooseLayout->getChildByName("Button_exit");
	chooseExit->addTouchEventListener(CC_CALLBACK_2(GameExitChangeLayer::exitCallBack, this));
	//切换帐号
	auto chooseSwitch = (Button*)exitChooseLayout->getChildByName("Button_switch");
	chooseSwitch->addTouchEventListener(CC_CALLBACK_2(GameExitChangeLayer::switchCallBack, this));
	return true;
}

void GameExitChangeLayer::closeExitChooseCallBack(Ref* pSender, Widget::TouchEventType type)
{
	Button* selectedBtn = (Button*)pSender;
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		selectedBtn->setColor(Color3B(170, 170, 170));
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		selectedBtn->setColor(Color3B(255, 255, 255));
		closeExitChoose();
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		selectedBtn->setColor(Color3B(255, 255, 255));
		break;
	default:
		break;
	}
}

void GameExitChangeLayer::exitCallBack(Ref* pSender, Widget::TouchEventType type)
{
	Button* selectedBtn = (Button*)pSender;
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		selectedBtn->setColor(Color3B(170, 170, 170));
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		selectedBtn->setColor(Color3B(255, 255, 255));

		Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		exit(0);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		exit(0);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		exit(0);
#endif
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		selectedBtn->setColor(Color3B(255, 255, 255));
		break;
	default:
		break;
	}

}

void GameExitChangeLayer::switchCallBack(Ref* pSender, Widget::TouchEventType type)
{
	Button* selectedBtn = (Button*)pSender;
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		selectedBtn->setColor(Color3B(170, 170, 170));
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		selectedBtn->setColor(Color3B(255, 255, 255));
		if (nullptr != onExitCallBack)
		{
			onExitCallBack();
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		selectedBtn->setColor(Color3B(255, 255, 255));
		break;
	default:
		break;
	}
	
}