#include "GoldenToadGameToolbar.h"
#include "GoldenToadGameAudio.h"

using namespace ui;

namespace GoldenToad
{
	const char* Name_Auto_Menu		= "Button_Auto";
	const char* Name_Move_Menu		= "Button_Config";
	const char* Name_Exit_Menu		= "Button_Menu0";
	const char* Name_Help_Menu		= "Button_Menu1";
	const char* Name_Chat_Menu		= "Button_Menu6";
	const char* Name_Set_Menu		= "Button_Menu2";
	const char* Name_Lock_Menu		= "Button_Menu4";
	const char* Name_UnLock_Menu	= "Button_Menu5";
	const char* Name_LockBig_Menu	= "Button_Menu3";

	const bool needLockFish = true;			//是否需要打开锁定功能

	GoldenToadToolbar::GoldenToadToolbar()
		: _isOut(false)
		, _onExit(nullptr)
		, _onChat(nullptr)
		, _onHelp(nullptr)
		, _onSet(nullptr)
		, _onLock(nullptr)
		, _onAutoFire(nullptr)
	{

	}

	GoldenToadToolbar::~GoldenToadToolbar()
	{

	}

	GoldenToadToolbar* GoldenToadToolbar::create(Layout* toolBarBG)
	{
		GoldenToadToolbar* toolbar = new GoldenToadToolbar();
		if (toolbar->init(toolBarBG))
		{
			toolbar->ignoreAnchorPointForPosition(false);
			toolbar->autorelease();
			return toolbar;
		}
		CC_SAFE_DELETE(toolbar);
		return nullptr;
	}

	bool GoldenToadToolbar::init(Layout* toolBarBG)
	{
		if (!HN::HNLayer::init()) return false;

		_toolBarBG = toolBarBG;

		auto btnConfig = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(_toolBarBG, Name_Move_Menu));
		auto btnAutoFire = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(_toolBarBG, Name_Auto_Menu));

		auto btnExit = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(_toolBarBG, Name_Exit_Menu));
		auto btnHelp = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(_toolBarBG, Name_Help_Menu));
		auto btnSetting = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(_toolBarBG, Name_Set_Menu));
		auto btnChat = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(_toolBarBG, Name_Chat_Menu));

		btnConfig->addTouchEventListener(CC_CALLBACK_2(GoldenToadToolbar::menuClickCallback, this));
		btnConfig->setLocalZOrder(10);
		btnAutoFire->addTouchEventListener(CC_CALLBACK_2(GoldenToadToolbar::menuClickCallback, this));

		btnExit->addTouchEventListener(CC_CALLBACK_2(GoldenToadToolbar::menuClickCallback, this));
		btnHelp->addTouchEventListener(CC_CALLBACK_2(GoldenToadToolbar::menuClickCallback, this));
		btnSetting->addTouchEventListener(CC_CALLBACK_2(GoldenToadToolbar::menuClickCallback, this));
		btnChat->addTouchEventListener(CC_CALLBACK_2(GoldenToadToolbar::menuClickCallback, this));

		auto btnLock = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(_toolBarBG, Name_Lock_Menu));
		auto btnUnLock = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(_toolBarBG, Name_UnLock_Menu));
		auto btnLockBig = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(_toolBarBG, Name_LockBig_Menu));

		btnLock->addClickEventListener([=](Ref*){
			if (_onLock) _onLock(true, true);
		});

		btnUnLock->addClickEventListener([=](Ref*){
			if (_onLock) _onLock(false, true);
		});

		btnLockBig->addClickEventListener([=](Ref*){
			if (_onLock) _onLock(true, false);
		});

		return true;
	}

	void GoldenToadToolbar::menuClickCallback(cocos2d::Ref* pSender, ui::Widget::TouchEventType touchtype)
	{
		if (Widget::TouchEventType::ENDED != touchtype)	return;
		Button* pMenu = dynamic_cast<Button*>(pSender);

		//GoldenToadGameAudio::playButtonEffect();

		std::string name = pMenu->getName();

		if (name.compare(Name_Exit_Menu) == 0)
		{
			if (_onExit) _onExit();
		}

		if (name.compare(Name_Auto_Menu) == 0)
		{
			if (_onAutoFire) _onAutoFire(pMenu);
		}

		if (name.compare(Name_Move_Menu) == 0)
		{
			pMenu->setEnabled(false);
			toolBarMove();
		}

		if (name.compare(Name_Help_Menu) == 0)
		{
			if (_onHelp) _onHelp();
		}

		if (name.compare(Name_Set_Menu) == 0)
		{
			if (_onSet) _onSet();
		}

		if (name.compare(Name_Chat_Menu) == 0)
		{
			if (_onChat) _onChat();
		}
	}

	void GoldenToadToolbar::toolBarMove()
	{
		auto winSize = Director::getInstance()->getWinSize();
		auto btnMove = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(_toolBarBG, Name_Move_Menu));
		btnMove->runAction(Sequence::create(RotateBy::create(0.4f, 360), CallFunc::create([=](){
			btnMove->setEnabled(true);
		}), nullptr));

		int num = needLockFish ? 7 : 4;
		char str[24];
		for (int i = 0; i < num; i++)
		{
			sprintf(str, "Image_pos%d", i);
			auto textPos = dynamic_cast<ImageView*>(ui::Helper::seekWidgetByName(_toolBarBG, str));

			sprintf(str, "Button_Menu%d", i);
			auto button = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(_toolBarBG, str));

			if (button->getPosition() == textPos->getPosition())
			{
				button->runAction(Sequence::create(RotateBy::create(0.2f, 360), MoveTo::create(0.2f, btnMove->getPosition()), Hide::create(), nullptr));
			}
			else
			{
				button->setVisible(true);
				button->runAction(Sequence::create(MoveTo::create(0.2f, textPos->getPosition()), RotateBy::create(0.2f, 360), nullptr));
			}
		}
	}
}

