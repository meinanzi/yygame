#include "GoldenFlowerGameDashboard.h"
#include "cocostudio/CocoStudio.h"
#include "GoldenFlowerGameTableUI.h"
#include "GoldenFlowerMessageHead.h"

USING_NS_CC;

namespace goldenflower
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
		_visibleWidth = size.width;
		_visibleHeight = size.height;

		_dashboard_node = CSLoader::createNode("goldenflower/dashboard.csb");
		_dashboard_node->setAnchorPoint(Vec2(0.5, 0.5));
		_dashboard_node->setPosition(Vec2(_visibleWidth*0.5, _visibleHeight*0.05));
		this->addChild(_dashboard_node, 100);

		_lookButton    = dynamic_cast<ui::Button*>(_dashboard_node->getChildByName("look_button"));
		_compareButton = dynamic_cast<ui::Button*>(_dashboard_node->getChildByName("compare_button"));
		_followButton  = dynamic_cast<ui::Button*>(_dashboard_node->getChildByName("follow_button"));
		_giveUpButton  = dynamic_cast<ui::Button*>(_dashboard_node->getChildByName("giveup_button"));
		_addButton     = dynamic_cast<ui::Button*>(_dashboard_node->getChildByName("add_button"));
		_ButtonBeg	   = dynamic_cast<ui::ImageView*>(_dashboard_node->getChildByName("Button_beg"));
		_BetBegPosition = _ButtonBeg->getPosition();
		_betButton1 = dynamic_cast<ui::Button*>(_ButtonBeg->getChildByName("Bet_0"));
		_betButton2 = dynamic_cast<ui::Button*>(_ButtonBeg->getChildByName("Bet_1"));
		_betButton3 = dynamic_cast<ui::Button*>(_ButtonBeg->getChildByName("Bet_2"));
		_betButton4 = dynamic_cast<ui::Button*>(_ButtonBeg->getChildByName("Bet_3"));


		_lookButton->addTouchEventListener(CC_CALLBACK_2(Dashboard::menClickCallback,this));
		_compareButton->addTouchEventListener(CC_CALLBACK_2(Dashboard::menClickCallback,this));
		_followButton->addTouchEventListener(CC_CALLBACK_2(Dashboard::menClickCallback,this));
		_giveUpButton->addTouchEventListener(CC_CALLBACK_2(Dashboard::menClickCallback,this));
		_addButton->addTouchEventListener(CC_CALLBACK_2(Dashboard::menClickCallback,this));
		_betButton1->addTouchEventListener(CC_CALLBACK_2(Dashboard::menClickCallback, this));
		_betButton2->addTouchEventListener(CC_CALLBACK_2(Dashboard::menClickCallback, this));
		_betButton3->addTouchEventListener(CC_CALLBACK_2(Dashboard::menClickCallback, this));
		_betButton4->addTouchEventListener(CC_CALLBACK_2(Dashboard::menClickCallback, this));

		_slider    = dynamic_cast<ui::Slider*>(_dashboard_node->getChildByName("slider"));
		_sliderBg  = dynamic_cast<ui::ImageView*>(_dashboard_node->getChildByName("slider_bg"));
		_noteLabel = dynamic_cast<ui::TextAtlas*>(_dashboard_node->getChildByName("money"));
		_slider->addEventListener(CC_CALLBACK_2(Dashboard::sliderChangedCallback,this));

		return true;
	}

	void Dashboard::setLookVisible(bool visible)
	{
		setButtonEnabled(_lookButton, visible);
	}

	void Dashboard::setFollowVisible(bool visible)
	{
		setButtonEnabled(_followButton, visible);
	}

	void Dashboard::setAddVisible(bool visible)
	{
		setButtonEnabled(_addButton, visible);
		//_slider->setTouchEnabled(visible);
	}

	void Dashboard::setAddBetVisible(bool visible)
	{
		if (visible)
		{
			auto moveto = MoveTo::create(0.5f, _BetBegPosition + Vec2(0.f, 100.f));
			auto sequence = Sequence::create(EaseBackOut::create(moveto), nullptr);
			_ButtonBeg->runAction(sequence);

			setButtonEnabled(_betButton1, _CanAddOfAll[0]);
			setButtonEnabled(_betButton2, _CanAddOfAll[1]);
			setButtonEnabled(_betButton3, _CanAddOfAll[2]);
			setButtonEnabled(_betButton4, _CanAddOfAll[3]);
		}
		else
		{
			auto moveto = MoveTo::create(0.5f, _BetBegPosition);
			auto sequence = Sequence::create(EaseBackOut::create(moveto), nullptr);
			_ButtonBeg->runAction(sequence);
		}
		
	}

	void Dashboard::setOpenVisible(bool visible)
	{
		setButtonEnabled(_compareButton, visible);
	}

	void Dashboard::setGiveUpVisible(bool visible)
	{
		setButtonEnabled(_giveUpButton, visible);
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
		, _ButtonBeg(nullptr)
		,_betButton1(nullptr)
		,_betButton2(nullptr)
		,_betButton3(nullptr)
		, _betButton4(nullptr)
		,_addValue(0)
		,_min(0)
		,_max(0)
	{
		memset(_CanAddOfAll, 0, sizeof(_CanAddOfAll));
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
			setAddBetVisible(false);

		}
		else if(pSender == _compareButton)
		{
			_tableUI->compareCardReq();
			hide();
			setAddBetVisible(false);
		}
		else if(pSender == _followButton)
		{
			_tableUI->followNote();
			_tableUI->IStartTimer(0);
			setAddBetVisible(false);
			setFollowVisible(false);
		}
		else if(pSender == _giveUpButton)
		{
			_tableUI->giveUpNote();
			_tableUI->IStartTimer(0);
			setAddBetVisible(false);
		}
		else if(pSender == _addButton)
		{
			_tableUI->addNote(_addValue);
		}
		else if (pSender == _betButton1)
		{
			//选择50
			_tableUI->addBet(0);
			_tableUI->IStartTimer(0);
			setAddBetVisible(false);
		}
		else if (pSender == _betButton2)
		{
			//选择100
			_tableUI->addBet(1);
			_tableUI->IStartTimer(0);
			setAddBetVisible(false);
		}
		else if (pSender == _betButton3)
		{
			//选择200
			_tableUI->addBet(2);
			_tableUI->IStartTimer(0);
			setAddBetVisible(false);
		}
		else if (pSender == _betButton4)
		{
			//选择500
			_tableUI->addBet(3);
			_tableUI->IStartTimer(0);
			setAddBetVisible(false);
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

