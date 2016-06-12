#include "LandlordExitPrompt.h"
#include "LandlordGameTableUI.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocostudio;
using namespace ui;

namespace landlord
{
	static const char* EXIT_BG = "landlord/game/studioUI/uiRes/exit_bg.png";
	static const char* EXIT_CANCEL_N = "landlord/game/studioUI/uiRes/exit_cancel_n.png";
	static const char* EXIT_CANCEL_D = "landlord/game/studioUI/uiRes/exit_cancel_d.png";
	static const char* EXIT_OK_N = "landlord/game/studioUI/uiRes/exit_ok_n.png";
	static const char* EXIT_OK_D = "landlord/game/studioUI/uiRes/exit_ok_d.png";

	//////////////////////////////////////////////////////////////////////////

	ExitPromptLayer::ExitPromptLayer() : _onExit(nullptr)
	{

	}

	ExitPromptLayer::~ExitPromptLayer()
	{

	}

	void ExitPromptLayer::setExitCallBack(const std::function<void ()>& onExit)
	{
		_onExit = onExit;
	}

	bool ExitPromptLayer::init()
	{
		if (!HNLayer::init())
		{
			return false;
		}

		auto listener = EventListenerTouchOneByOne::create();
		listener->setSwallowTouches(true);
		listener->onTouchBegan = [&](Touch* touch, Event* event)
		{
			return true;
		};
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

		quicklyShade(150);

		Size winSize = Director::getInstance()->getWinSize();

		Sprite* bg = Sprite::create(EXIT_BG);
		bg->setPosition(Vec2(winSize.width * 0.5f, winSize.height * 0.5f));
		bg->setAnchorPoint(Vec2(0.5f, 0.5f));
		this->addChild(bg, 2);
		
		Button* exitOk = Button::create(EXIT_OK_N, EXIT_OK_D);
		exitOk->setPosition(Vec2(bg->getContentSize().width * 0.25f, exitOk->getContentSize().height));
		exitOk->setAnchorPoint(Vec2(0.5f, 0.5f));
		exitOk->addTouchEventListener(CC_CALLBACK_2(ExitPromptLayer::onExitCallBack, this));
		bg->addChild(exitOk);

		Button* exitCancel = Button::create(EXIT_CANCEL_N, EXIT_CANCEL_D);
		exitCancel->setPosition(Vec2(bg->getContentSize().width * 0.75f, exitCancel->getContentSize().height));
		exitCancel->setAnchorPoint(Vec2(0.5f, 0.5f));
		exitCancel->addTouchEventListener(CC_CALLBACK_2(ExitPromptLayer::onCancelCallBack, this));
		bg->addChild(exitCancel);

		return true;
	}

	void ExitPromptLayer::onExitCallBack(Ref* pSender, Widget::TouchEventType type)
	{
		if (Widget::TouchEventType::ENDED != type) return;
		
		if(nullptr != _onExit)
		{
			_onExit();
		}
	}

	void ExitPromptLayer::onCancelCallBack(Ref* pSender, Widget::TouchEventType type)
	{
		if (Widget::TouchEventType::ENDED != type) return;

		this->runAction(RemoveSelf::create());
	}
}
