#include "HNLobby/GameWebView.h"
#include "HNLobby/FontConfig.h"
#include "HNLobby/PlatformDefine.h"
#include "HNLobby/PlatformConfig.h"
#include <string>

static const int LAYER_ZORDER	= 100000000;		// 子节点弹出框层级
static const int LAYER_TAG				= 100000000;		// 子节点弹出框层级

static const char* CLOSE_MENU_N		= "platform/store/second_back.png";                       
static const char* LEAF_LEFT				= "platform/subViewBambooLeft.png";           
static const char* LEAF_RIGHT				= "platform/subViewBambooRight.png";

GameWebViewLayer::GameWebViewLayer() 
	: _titleText (nullptr)
	, _layerColor (nullptr)
{

}

GameWebViewLayer::~GameWebViewLayer()
{

}

void GameWebViewLayer::showWebView(const std::string& URL)
{
	Node* root = Director::getInstance()->getRunningScene();
	CCAssert(nullptr != root, "root is null");

	root->addChild(this, LAYER_ZORDER, LAYER_TAG);
	setURL(URL);
}

void GameWebViewLayer::setTitle(const std::string& title)
{
	if (nullptr != _titleText)
	{
		_titleText->setString(title);
	}
}

void GameWebViewLayer::setColor(Color3B color)
{
	_layerColor->setColor(color);
}

void GameWebViewLayer::closeWebView()
{
	HNAudioEngine::getInstance()->playEffect(GAME_SOUND_CLOSE);
	this->runAction(Sequence::create(ScaleTo::create(0.1f, 0.3f), CCCallFunc::create([&]()
	{
		this->removeFromParent();
	}), nullptr));
}

bool GameWebViewLayer::init()
{
    if ( !HNLayer::init()) return false;
	
	Size winSize = Director::getInstance()->getWinSize();
	//屏蔽后面的层
	_layerColor = LayerColor::create(Color4B(0, 0, 205, 100));
	_layerColor->setContentSize(Size(winSize));
	_layerColor->setPosition(Vec2(0, 0));
	addChild(_layerColor);

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = [=](Touch* touch ,Event* event){
		return true;
	};
	touchListener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener,_layerColor);

	//竹叶
	auto leftLeaf = Sprite::create(LEAF_LEFT);
	leftLeaf->setAnchorPoint(Point(0,1));
	leftLeaf->setPosition(0,winSize.height);
	addChild(leftLeaf,5);
	auto rightleaf = Sprite::create(LEAF_RIGHT);
	rightleaf->setAnchorPoint(Point(1,1));
	rightleaf->setPosition(winSize.width,winSize.height);
	addChild(rightleaf,5);

	//标题
	_titleText = Text::create("", "", 60);
	addChild(_titleText, 5);
	_titleText->setPosition(Vec2(winSize.width/2, winSize.height-40));
	_titleText->setString("");

	// 关闭按钮
	auto closeBtn = Button::create(CLOSE_MENU_N,CLOSE_MENU_N);
	closeBtn->setPosition(Vec2(40, winSize.height-40));
	addChild(closeBtn,5);
	closeBtn->addTouchEventListener(CC_CALLBACK_2(GameWebViewLayer::closeEventCallBack,this));

	return true;
}

void GameWebViewLayer::setURL(const std::string& URL)
{
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		Size winSize = Director::getInstance()->getWinSize();
		auto myWebView = cocos2d::experimental::ui::WebView::create();
		addChild(myWebView);
		myWebView->setContentSize(Size(winSize.width, winSize.height-80));
		myWebView->setPosition(Vec2(winSize.width/2, winSize.height/2 - 40));
		myWebView->loadURL(URL);
		myWebView->setScalesPageToFit(true); 
	#endif		
}

void GameWebViewLayer::closeEventCallBack(Ref* pSender, Widget::TouchEventType type)
{
	if (Widget::TouchEventType::ENDED != type) return;

			_layerColor->setColor(Color3B(255,255,255));
			closeWebView();
}
