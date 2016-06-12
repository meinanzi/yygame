#include "HNLobby/GamePrompt.h"
#include "HNLobby/FontConfig.h"
#include "HNLobby/PlatformDefine.h"
#include "HNLobby/PlatformConfig.h"
#include <string>

static const char* PROMPT_CSB		= "platform/prompt/promptNode.csb";

static const int GAMEPROMPT_LAYER_ZORDER	= 100000000;		// 子节点弹出框层级

static const int GAMEPROMPT_LAYER_TAG		= 100000000;		// 子节点弹出框层级


GamePromptLayer::GamePromptLayer() 
	: promptLoader (nullptr)
	, promptText (nullptr)
	, _sure (nullptr)
	, _cancel (nullptr)
	, promptButtonSure1 (nullptr)
	, promptButtonSure2 (nullptr)
	, promptButtonCancel (nullptr)
{

}

GamePromptLayer::~GamePromptLayer()
{

}

void GamePromptLayer::showPrompt(const std::string& prompt)
{
	Node* root = Director::getInstance()->getRunningScene();
	CCAssert(nullptr != root, "root is null");

	root->addChild(this, GAMEPROMPT_LAYER_ZORDER, GAMEPROMPT_LAYER_TAG);
	setPrompt(prompt);
}

void GamePromptLayer::closePrompt()
{
	HNAudioEngine::getInstance()->playEffect(GAME_SOUND_CLOSE);
	promptLoader->runAction(Sequence::create(ScaleTo::create(0.1f, 0.3f), CCCallFunc::create([&]()
	{
		this->removeFromParent();
	}), nullptr));
}

void GamePromptLayer::setCallBack(std::function<void ()> sure)
{
	_sure = sure;
}

void GamePromptLayer::setCancelCallBack(std::function<void ()> cancel)
{
	_cancel = cancel;
}

void GamePromptLayer::setVerticalScreenShow()
{
	promptLoader->setRotation(-90);			//横屏改为竖屏显示
}

bool GamePromptLayer::init()
{
    if ( !HNLayer::init()) return false;
	//屏蔽后面的层
	auto colorLayer = LayerColor::create(Color4B(0, 0, 0, 100));
	addChild(colorLayer, 1);

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = [=](Touch* touch ,Event* event){
		return true;
	};
	touchListener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener,colorLayer);

	Size winSize = Director::getInstance()->getWinSize();

	promptLoader = CSLoader::createNode(PROMPT_CSB);
	promptLoader->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
	promptLoader->setScale(0);
	addChild(promptLoader, 2);
	promptLoader->runAction(ScaleTo::create(0.2f, 1.0f));
	auto promptLayout = (Layout* )promptLoader->getChildByName("Panel_1");
	//显示的文本
	promptText = (Text* )promptLayout->getChildByName("Text_prompt");
	promptText->setString("");

	//确定按钮
	promptButtonSure1 = (Button* )promptLayout->getChildByName("Button_sure");
	promptButtonSure1->addTouchEventListener(CC_CALLBACK_2(GamePromptLayer::menuEventCallBack, this));

	//确定按钮
	promptButtonSure2 = (Button* )promptLayout->getChildByName("Button_sure2");
	promptButtonSure2->addTouchEventListener(CC_CALLBACK_2(GamePromptLayer::menuEventCallBack, this));
	promptButtonSure2->setVisible(false);

	//确定按钮
	promptButtonCancel = (Button* )promptLayout->getChildByName("Button_cancel");
	promptButtonCancel->addTouchEventListener(CC_CALLBACK_2(GamePromptLayer::menuEventCallBack, this));
	promptButtonCancel->setVisible(false);

	return true;
}

void GamePromptLayer::setPrompt(const std::string& prompt)
{
	if (nullptr != promptText)
	{
		promptText->setString(prompt);
	}
}

void GamePromptLayer::setPrompt(const std::string& prompt, const std::string& font, int size)
{
	if (nullptr != promptText)
	{
		promptText->setFontName(font);
		promptText->setFontSize(0);
		promptText->setString(prompt);
	}
}

void GamePromptLayer::setPromptCanSelect()
{
	promptButtonSure1->setVisible(false);
	promptButtonSure2->setVisible(true);
	promptButtonCancel->setVisible(true);
}

void GamePromptLayer::menuEventCallBack(Ref* pSender, Widget::TouchEventType type)
{
	Button* selectedBtn = (Button*)pSender;
	auto name = selectedBtn->getName();

	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		selectedBtn->setColor(Color3B(170,170,170));
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		{
			selectedBtn->setColor(Color3B(255,255,255));
			closePrompt();
			if (name.compare("Button_sure") == 0 || name.compare("Button_sure2") == 0)
			{
				if (nullptr != _sure)
				{
					_sure();
				}
			}
			if (name.compare("Button_cancel") == 0)
			{
				if (nullptr != _cancel)
				{
					_cancel();
				}
			}
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		selectedBtn->setColor(Color3B(255,255,255));
		break;
	default:
		break;
	}
}
