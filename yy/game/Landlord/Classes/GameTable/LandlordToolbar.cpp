#include "LandlordToolbar.h"

using namespace ui;

namespace landlord
{
#define TOOLBAR_JSON_FILE "landlord/game/studioUI/toolbar.csb"

#define Name_Exit_Menu   "Button_exit"
#define Name_Set_Menu    "Button_set"
#define Name_Chat_Menu   "Button_chat"
#define Name_Auto_Menu   "Button_auto"
#define Name_Drop_Menu   "Button_drop"
#define Name_Up_Image    "Image_up"
#define Name_Down_Image  "Image_down"

	Toolbar* Toolbar::create(const std::function<void(cocos2d::Node*)>& func)
	{
		Toolbar* toolbar = new Toolbar();
		if(toolbar->init(func))
		{
			toolbar->ignoreAnchorPointForPosition(false);
			toolbar->autorelease();
			return toolbar;
		}
		CC_SAFE_DELETE(toolbar);
		return nullptr;
	}

	bool Toolbar::init(const std::function<void(cocos2d::Node*)>& func)
	{
		if(!HN::HNLayer::init())
		{
			return false;
		}
		
		_function = func;
		
		this->setName("toolbar");

		auto toolBar_Node = CSLoader::createNode(TOOLBAR_JSON_FILE);
		this->addChild(toolBar_Node);

		auto layer = (Layout*)toolBar_Node->getChildByName("Panel_toolbar");

		Size size = layer->getContentSize();
		this->setContentSize(size);

		layer->setAnchorPoint(Vec2(0.5f, 0.5f));
		layer->setPosition(Vec2(size.width / 2, size.height / 2));

		_iDropDown = dynamic_cast<ImageView*>(ui::Helper::seekWidgetByName(layer, Name_Down_Image));
		_iDropUp   = dynamic_cast<ImageView*>(ui::Helper::seekWidgetByName(layer, Name_Up_Image));

		auto btnExit = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(layer, Name_Exit_Menu));
		auto btnSet  = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(layer, Name_Set_Menu));
		auto btnDrop = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(layer, Name_Drop_Menu));
		auto btnChat = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(layer, Name_Chat_Menu));
		auto btnAuto = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(layer, Name_Auto_Menu));

		btnExit->addTouchEventListener(CC_CALLBACK_2(Toolbar::menuClickCallback, this));
		btnSet->addTouchEventListener(CC_CALLBACK_2(Toolbar::menuClickCallback, this));
		btnDrop->addTouchEventListener(CC_CALLBACK_2(Toolbar::menuClickCallback, this));
		btnAuto->addTouchEventListener(CC_CALLBACK_2(Toolbar::menuClickCallback, this));
		btnChat->addTouchEventListener(CC_CALLBACK_2(Toolbar::menuClickCallback, this));

		return true;
	}

	Toolbar::Toolbar()
		: _isUp(true)
		, _iDropDown(nullptr)
		, _iDropUp(nullptr)
		, _onAutoFunc(nullptr)
		, _onExitFunc(nullptr)
		, _onSettingFunc(nullptr)
	{

	}

	Toolbar::~Toolbar()
	{

	}

	void Toolbar::setExitCallBack(const std::function<void()>& onExitFunc)
	{
		_onExitFunc = onExitFunc;
	}

	void Toolbar::setSettingCallBack(const std::function<void()>& onSettingFunc)
	{
		_onSettingFunc = onSettingFunc;
	}

	void Toolbar::setAutoCallBack(const std::function<void()>& onAutoFunc)
	{
		_onAutoFunc = onAutoFunc;
	}

	void Toolbar::menuClickCallback(cocos2d::Ref* pSender, ui::Widget::TouchEventType touchtype)
	{
		if (Widget::TouchEventType::ENDED != touchtype)	return;

		Button* pMenu = dynamic_cast<Button*>(pSender);

		std::string name = pMenu->getName();

		if (name.compare(Name_Set_Menu) == 0)
		{
			if (nullptr != _onSettingFunc)
			{
				_onSettingFunc();
			}
		}
		else if (name.compare(Name_Exit_Menu) == 0)
		{
			if (nullptr != _onAutoFunc)
			{
				_onExitFunc();
			}
		}
		else if (name.compare(Name_Drop_Menu) == 0)
		{
			if(_isUp)
			{
				moveDown();
			}
			else
			{
				moveUp();
			}
			_isUp = !_isUp;
			_iDropUp->setVisible(!_isUp);
			_iDropDown->setVisible(_isUp);
			if(nullptr != _function)
			{
				_function(this);
			}
		}
		else if(name.compare(Name_Auto_Menu) == 0)
		{
			if (nullptr != _onAutoFunc)
			{
				_onAutoFunc();
			}
		}
		else if(name.compare(Name_Chat_Menu) == 0)
		{
			if (nullptr != onSpeakeCallBack)
			{
				onSpeakeCallBack();
			}
		}
		else
		{

		}
	}

	void Toolbar::moveUp()
	{
		this->runAction(MoveBy::create(0.2f, Vec2(0, this->getContentSize().height * this->getScaleY())));
	}

	void Toolbar::moveDown()
	{
		this->runAction(MoveBy::create(0.2f, Vec2(0, -this->getContentSize().height * this->getScaleY())));
	}
}

