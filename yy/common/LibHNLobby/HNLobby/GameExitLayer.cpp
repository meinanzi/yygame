#include "HNLobby/GameExitLayer.h"
#include "HNLobby/PlatformConfig.h"
#include "SimpleAudioEngine.h"

static const char* EXIT_GO_ON_D = "platform/exitUi/bird2.png";                      //goonBtn

bool GameExitLayer::init()
{
	if (!HNLayer::init()) return false;

	//屏蔽后面的层
	auto colorLayer = LayerColor::create(Color4B(0, 0, 0, 100));
	addChild(colorLayer);

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = [=](Touch* touch ,Event* event){
		return true;
	};
	touchListener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener,colorLayer);

	auto winSize = Director::getInstance()->getWinSize();
	//添加退出的CSB	
	_loader = CSLoader::createNode("platform/exitUi/ExitGameNode.csb");
	_loader->setPosition(winSize / 2);
	_loader->setScale(0);
	addChild(_loader, 2 );
	_loader->runAction(ScaleTo::create(0.2f,1.0f));
	_layout = ( Layout* )_loader->getChildByName("Panel_1");

	//获取两个按钮
	_btnQuit = (Button* ) _layout->getChildByName("Button_btnQuit");
	_btnQuit->addTouchEventListener(CC_CALLBACK_2(GameExitLayer::ButtonCallBack, this));
	_btnContinue = (Button* ) _layout->getChildByName("Button_btnContinue");
	_btnContinue->addTouchEventListener(CC_CALLBACK_2(GameExitLayer::ButtonCallBack, this));
	
	return true;
}

void GameExitLayer::ButtonCallBack(Ref* pSender, Widget::TouchEventType  type)
{
	auto btn = (Button* ) pSender;
	auto name = btn->getName();
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		btn->setColor(Color3B(170, 170, 170));
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED: 
		btn->setColor(Color3B(255, 255, 255));
		if (name.compare("Button_btnQuit") == 0)
		{
			callBackYes();
		}
		else if(name.compare("Button_btnContinue") == 0)
		{
			callBackNo();
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		btn->setColor(Color3B(255, 255, 255));
		break;
	default:
		break;
	}
}

void GameExitLayer::callBackYes()
{
	SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
    AnimationCache::getInstance()->destroyInstance();
    CocosDenshion::SimpleAudioEngine::getInstance()->end();
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	exit(0);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	exit(0);
#endif
}

void GameExitLayer::callBackNo()
{
	_loader->runAction(Sequence::create(ScaleTo::create(0.2f,0),
		CCCallFunc::create([&](){
		this->removeFromParent();
	}), nullptr));
}
