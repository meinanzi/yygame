#include "DZPokerBuyDialog.h"
#include "DZPokerUpgradeMessage.h"
#include "HNLobbyExport.h"

namespace DZPoker
{
	BuyDialog* BuyDialog::create(Node* parent, LLONG max, LLONG min, LLONG plan, LLONG total, BYTE deskStation)
	{
		BuyDialog* dialog = new BuyDialog();
		if(dialog->init(parent, max, min, plan, total, deskStation))
		{
			dialog->ignoreAnchorPointForPosition(false);
			dialog->autorelease();
			return dialog;
		}
		else
		{
			CC_SAFE_DELETE(dialog);
			return nullptr;
		}
	}

	bool BuyDialog::init(Node* parent, LLONG max, LLONG min, LLONG plan, LLONG total, BYTE deskStation)
	{
		if(!HNDialogBase::init(parent))
		{
			return false;
		}

		auto distpatch = Director::getInstance()->getEventDispatcher();
		auto listener = EventListenerTouchOneByOne::create();
		listener->onTouchBegan = CC_CALLBACK_2(BuyDialog::onTouchBegan,this);
		listener->setSwallowTouches(true);
		distpatch->addEventListenerWithSceneGraphPriority(listener,this);

		_max = max;
		_min = min;
		_plan = min > total ? total : min;
		_total = total;
		_deskStation = deskStation;

		_node = CSLoader::createNode("dzpoker/buydialog.csb");;
		locateMid(_node);
		this->addChild(_node);

		resetLayout();

		return true;
	}

	bool BuyDialog::onTouchBegan(Touch *touch, Event *unused_event)
	{
		return false;
	}

	void  BuyDialog::sliderChangedCallback(Ref* pRef,ui::Slider::EventType eventType)
	{
		if(eventType == ui::Slider::EventType::ON_PERCENTAGE_CHANGED)
		{
			LLONG min = _min;
			LLONG max = _total > _max ? _max : _total;
			int percent = _slider->getPercent();
			_plan = min + LLONG(percent / 100.0 * (max - min));
			char str[50] = {0};
			sprintf(str,"%lld",_plan);
			_planTextAtlas->setString(str);
		}
	}

	void BuyDialog::sendMoneyPlan()
	{
		TSubMoney cmd;
		cmd.bDeskStation = _deskStation;
		cmd.nMoney[_deskStation] = _plan;
		RoomLogic()->sendData(MDM_GM_GAME_NOTIFY,ASS_SUB_MONEY, (CHAR*)&cmd, sizeof(cmd));
	}

	void BuyDialog::sendStandUp()
	{
		RoomLogic()->sendData(MDM_GR_USER_ACTION, ASS_GR_USER_UP);
	}

	void  BuyDialog::submitCallback(Ref* pSender, Widget::TouchEventType type)
	{
		if (ui::Widget::TouchEventType::ENDED != type)
		{
			return ;
		}
		auto btn = (ui::Button*)pSender;
		if (btn == _btnMax)				//直接按最大携带
		{
			if (_total >= _max)
			{
				_plan = _max;
				sendMoneyPlan();
				hide();
			}
			else if (_total >= _min)
			{
				_plan = _total;
				sendMoneyPlan();
				hide();
			}
			else
			{
				GamePromptLayer::create()->showPrompt(GBKToUtf8("金币不足"));
			}
		}
		else if (btn == _btnMin)		//直接按最小携带
		{
			if (_total >= _min)
			{
				_plan = _min;
				sendMoneyPlan();
				hide();
			}

		}
		else if (btn == _btnOk)			//提交按钮
		{
			sendMoneyPlan();
			hide();
		}
		else if (btn == _btnCancle)		//取消
		{
			sendStandUp();
			hide();
		}
	}


	void BuyDialog::resetLayout()
	{
		LLONG min = _min;
		LLONG max = _total > _max ? _max : _total;
		char str[50] = {0};

		auto layout_buy_dialog = dynamic_cast<Layout*>(_node->getChildByName("Panel_39"));

		_allMoney = dynamic_cast<TextAtlas*>(layout_buy_dialog->getChildByName("currentmoney"));

		auto imageview_bg = dynamic_cast<ImageView*>(layout_buy_dialog->getChildByName("bg"));
		_slider = dynamic_cast<Slider*>(imageview_bg->getChildByName("slider"));
		_slider->addEventListener(CC_CALLBACK_2(BuyDialog::sliderChangedCallback,this));

		_btnMax = dynamic_cast<Button*>(layout_buy_dialog->getChildByName("max"));
		_btnMax->addTouchEventListener(CC_CALLBACK_2(BuyDialog::submitCallback,this));
		sprintf(str,"%lld", max);
		_btnMax->setTitleText(str);

		_btnMin = dynamic_cast<Button*>(layout_buy_dialog->getChildByName("min"));
		_btnMin->addTouchEventListener(CC_CALLBACK_2(BuyDialog::submitCallback,this));
		sprintf(str,"%lld",min);
		_btnMin->setTitleText(str);

		_btnOk = dynamic_cast<Button*>(layout_buy_dialog->getChildByName("ok"));
		_btnOk->addTouchEventListener(CC_CALLBACK_2(BuyDialog::submitCallback,this));

		_btnCancle = dynamic_cast<Button*>(layout_buy_dialog->getChildByName("cancel"));
		_btnCancle->addTouchEventListener(CC_CALLBACK_2(BuyDialog::submitCallback,this));

		_planTextAtlas = dynamic_cast<TextAtlas*>(_btnOk->getChildByName("money"));


		sprintf(str,"%lld",_total);
		_allMoney->setString(str);

		sprintf(str,"%lld",min);
		_planTextAtlas->setString(str);

		_slider->setPercent((_plan - min) * 1.0f / (max - min));
	}

}
