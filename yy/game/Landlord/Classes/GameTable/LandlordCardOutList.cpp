#include "LandlordCardOutList.h"
#include "LandlordPokerCard.h"
#include "LandlordGameTableUI.h"

namespace landlord
{

#define X_OUT_OFFSET 25.0f

#define File_Cancel_Normal   "landlord/game/btn/not_trusteeship_n.png"
#define File_Cancel_Touched  "landlord/game/btn/not_trusteeship_d.png"

//////////////////////////////////////////////////////////////////////////

	CardOutList* CardOutList::create()
	{
		CardOutList* cardList = new CardOutList();
		if(cardList->init())
		{
			cardList->autorelease();
			return cardList;
		}
		CC_SAFE_DELETE(cardList);
		return nullptr;
	}

	bool CardOutList::init()
	{
		if(!HNLayer::init())
		{
			return false;
		}	
		this->ignoreAnchorPointForPosition(false);

		return true;
	}

	void CardOutList::outCard(const std::vector<BYTE>& cards)
	{
		this->removeAllChildren();

		std::vector<Node*> cardList;
		for(size_t i = 0; i < cards.size(); i++)
		{
			PokerCard* card = PokerCard::create(cards[i]);
			card->setScale(0.35f);
			this->addChild(card, i);
			cardList.push_back(card);
		}

		if (!cardList.empty())
		{
			Size size = cardList[0]->getContentSize() * cardList[0]->getScale();
			this->setContentSize(Size(size.width + (cardList.size() - 1) * X_OUT_OFFSET, size.height));

			float startX = size.width  / 2;
			float startY = size.height / 2;

			for(size_t i = 0 ; i < cardList.size(); i++)
			{
				cardList[i]->setAnchorPoint(Vec2(0.5f, 0.5f));
				cardList[i]->setPosition(startX + i * X_OUT_OFFSET, startY);
				//cardList[i]->runAction(MoveTo::create(0.2f, Vec2(startX + i * X_OUT_OFFSET, startY)));
			}
		}
	}

	CardOutList::CardOutList()
	{

	}

	CardOutList::~CardOutList()
	{

	}


//////////////////////////////////////////////////////////////////////////

	CancelAuto* CancelAuto::create(GameTableUI* tableUI)
	{
		CancelAuto* layer = new CancelAuto();
		if(layer != nullptr && layer->init(tableUI))
		{
			layer->autorelease();
			return layer;
		}
		CC_SAFE_DELETE(layer);
		return nullptr;
	}

	bool CancelAuto::init(GameTableUI* tableUI)
	{
		if(!HNLayer::init())
		{
			return false;
		}

		_tableUI = tableUI;

		Size size = Director::getInstance()->getWinSize();

		auto layer = LayerColor::create(Color4B(51, 51, 51, 125), size.width, size.height);
		layer->setAnchorPoint(Vec2::ZERO);
		layer->setPosition(Vec2::ZERO);
		this->addChild(layer, 1);

		auto btn = ui::Button::create(File_Cancel_Normal, File_Cancel_Touched);
		btn->setAnchorPoint(Vec2(0.5f, 0.5f));
		btn->setPosition(Vec2(size.width - btn->getContentSize().width / 2, btn->getContentSize().height / 2));
		this->addChild(btn, 2);
		btn->addTouchEventListener(CC_CALLBACK_2(CancelAuto::menuClickCallback, this));
		btn->setTitleFontSize(18);

		return true;
	}

	void CancelAuto::menuClickCallback(cocos2d::Ref* pRef, cocos2d::ui::Widget::TouchEventType type)
	{
		if(type != cocos2d::ui::Widget::TouchEventType::ENDED)	return;
		_tableUI->sendAuto(false);
	}

	CancelAuto::CancelAuto()
	{
		auto listener = EventListenerTouchOneByOne::create();
		listener->onTouchBegan = CC_CALLBACK_2(CancelAuto::onTouchBegan, this);
		listener->setSwallowTouches(true);
		this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

		_touchListener = listener;
	}

	CancelAuto::~CancelAuto()
	{
		this->_eventDispatcher->removeEventListener(_touchListener);
	}

	bool CancelAuto::onTouchBegan(Touch *touch, Event *unused_event)
	{
		return true;
	}

}
