#include "TRACTORGameDashboard.h"
#include "cocostudio/CocoStudio.h"
#include "TRACTORGameTableUI.h"
#include "TRACTORGameClientMessage.h"

USING_NS_CC;
namespace TRACTOR
{
	Dashboard* Dashboard::create(GameTableUI* parent)
	{
		Dashboard* dashboard = new Dashboard();
		if(dashboard->init(parent))
		{
			dashboard->ignoreAnchorPointForPosition(false);
			dashboard->autorelease();
			return dashboard;
		}
		CC_SAFE_DELETE(dashboard);
		return nullptr;
	}

	bool Dashboard::init(GameTableUI* parent)
	{
		if(! HN::HNLayer::init())
		{
			return false;
		}
		_tableUI = parent;

		Size size = Director::getInstance()->getWinSize();
		auto widgetNode = CSLoader::createNode("tractor/Dashboard/Dashboard.csb");
		auto widget = (Widget*)widgetNode->getChildByName("Panel_2");
		//auto widget = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("tractor/Dashboard/Dashboard.json");
		widget->setAnchorPoint(Vec2(0.5f, 0));
		widget->setPosition(Vec2(size.width / 2, 0));
		addChild(widgetNode);

		_lookButton    = dynamic_cast<ui::Button*>(ui::Helper::seekWidgetByName(widget, "Button_look"));
		_compareButton = dynamic_cast<ui::Button*>(ui::Helper::seekWidgetByName(widget, "Button_compare"));
		_followButton  = dynamic_cast<ui::Button*>(ui::Helper::seekWidgetByName(widget, "Button_follow"));
		_giveUpButton  = dynamic_cast<ui::Button*>(ui::Helper::seekWidgetByName(widget, "Button_giveup"));
		_addButton     = dynamic_cast<ui::Button*>(ui::Helper::seekWidgetByName(widget, "Button_add"));

		_lookButton->addTouchEventListener(CC_CALLBACK_2(Dashboard::menClickCallback,this));
		_compareButton->addTouchEventListener(CC_CALLBACK_2(Dashboard::menClickCallback,this));
		_followButton->addTouchEventListener(CC_CALLBACK_2(Dashboard::menClickCallback,this));
		_giveUpButton->addTouchEventListener(CC_CALLBACK_2(Dashboard::menClickCallback,this));
		_addButton->addTouchEventListener(CC_CALLBACK_2(Dashboard::menClickCallback,this));

		_slider    = dynamic_cast<ui::Slider*>(ui::Helper::seekWidgetByName(widget, "Slider_slider"));
		_sliderBg  = dynamic_cast<ui::ImageView*>(ui::Helper::seekWidgetByName(widget, "Image_sliderBg"));
		_noteLabel = dynamic_cast<ui::TextAtlas*>(ui::Helper::seekWidgetByName(widget, "AtlasLabel_money"));
		_slider->addEventListener(CC_CALLBACK_2(Dashboard::sliderChangedCallback,this));

		return true;
	}

	void Dashboard::setActionOption(int options)
	{
		setButtonEnabled(_followButton, (options & C_VF_FOLLOW) != 0);
		setButtonEnabled(_lookButton, (options & C_VF_LOOK) != 0);
		setButtonEnabled(_addButton, (options & C_VF_ADD) != 0);
		setButtonEnabled(_compareButton, (options & C_VF_COMPARE) != 0);
		setButtonEnabled(_giveUpButton, (options & C_VF_GIVEUP) != 0);
	}

	void Dashboard::setAddRange(LLONG min, LLONG max)
	{
		_min = min;
		_max = max;
		_addValue = min;
		setAddValue(min);
		_slider->setPercent(0);
	}

	LLONG Dashboard::getAddValue()
	{
		return _addValue;
	}

	Dashboard::Dashboard()
		:_lookButton(nullptr)
		,_compareButton(nullptr)
		,_followButton(nullptr)
		,_giveUpButton(nullptr)
		,_addButton(nullptr)
		,_slider(nullptr)
		,_sliderBg(nullptr)
		,_noteLabel(nullptr)
		,_tableUI(nullptr)
		,_addValue(0)
		,_min(0)
		,_max(0)
	{

	}

	Dashboard::~Dashboard()
	{

	}

	void Dashboard::setAddValue(LLONG addValue)
	{
		char tmp[100] = {0};
		sprintf(tmp, "%lld", addValue);
		_noteLabel->setString(tmp);
	}

	void Dashboard::setButtonEnabled(ui::Button* pButton, bool enabled)
	{
		if(pButton != nullptr)
		{
			GLubyte opacity = enabled ? 255 : 125;
			pButton->setEnabled(enabled);
			pButton->setOpacity(opacity);
		}
	}

	void Dashboard::menClickCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType touchtype)
	{
		if(touchtype != cocos2d::ui::Widget::TouchEventType::ENDED)
		{
			return;
		}

		if(pSender == _lookButton)
		{
			_tableUI->lookCard();
		}
		else if(pSender == _followButton)
		{
			_tableUI->followNote();
		}
		else if(pSender == _giveUpButton)
		{
			_tableUI->giveUpNote();
		}
		else if(pSender == _addButton)
		{
			_tableUI->addNote(_addValue);
		}
		_tableUI->hideVSButton();
		if(pSender == _compareButton)
		{
			_tableUI->compareCardReq();
		}
	}

	void Dashboard::sliderChangedCallback(cocos2d::Ref* pSender,cocos2d::ui::Slider::EventType type)
	{
		if(type != ui::Slider::EventType::ON_PERCENTAGE_CHANGED || pSender != _slider)
		{
			return;
		}

		cocos2d::ui::Slider* pSlider = (cocos2d::ui::Slider*)(pSender);
		int percent = pSlider->getPercent();
		_addValue = _min + (_max - _min) * (percent / 100.0f);
		setAddValue(_addValue);
	}

	void Dashboard::show()
	{
		this->setVisible(true);
	}

	void Dashboard::hide()
	{
		this->setVisible(false);
	}
}