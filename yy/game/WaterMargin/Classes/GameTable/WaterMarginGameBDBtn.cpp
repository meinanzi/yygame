#include "WaterMarginGameBDBtn.h"
#include "WaterMarginGameTableLogic.h"
#include "cocostudio/CocoStudio.h"

namespace WaterMargin
{
	BDBtn::BDBtn()
		:_widget(nullptr)
		,_btnLogic(nullptr)
	{

	}

	BDBtn::~BDBtn()
	{

	}

	BDBtn* BDBtn::create(GameTableLogic* tableLogic, Node* node)
	{
		BDBtn* ptr = new BDBtn();
		if (nullptr != ptr && ptr->init(tableLogic, node))
		{
			ptr->autorelease();
			return ptr;
		}
		return nullptr;
	}

	bool BDBtn::init(GameTableLogic* tableLogic, Node* node)
	{
		_btnLogic = tableLogic;
		Size size = Director::getInstance()->getVisibleSize();
		/*_widget = cocostudio::GUIReader::getInstance()->widgetFromJsonFile*/
		auto btnNode = CSLoader::createNode("WaterMargin/tableui/bibei.csb");
		_widget = (Layout*)btnNode->getChildByName("Panel_17");
		_widget->setAnchorPoint(Vec2(0.5, 0.5));
		_widget->setPosition(Vec2(640,200));
		node->addChild(btnNode, 600);
		//_widget->setScale(size.width/1280, size.height/720);

		char name[10] = {0};
		//比倍、得分按钮
		for (int i = 1; i < 3; i++)
		{
			sprintf(name, "Button_%d", i);
			auto btn = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(_widget, name));
			btn->addTouchEventListener(CC_CALLBACK_2(BDBtn::menuClickCallback, this));
		}

		return true;
	}

	void BDBtn::menuClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype)
	{
		if (Widget::TouchEventType::ENDED != touchtype)	return;
		Button* ptr = (Button*)pSender;
		switch (ptr->getTag())
		{
		case 39:
			//_btnLogic->sendBibeiBtnMessage();
			_btnLogic->changeMaLiToBB();
			break;
		case 37:
			_btnLogic->sendGetWinMessage(0);
			_btnLogic->changeMaLiToDF();
			break;
		default:
			break;
		}

	}
}
