#include "GoldenFlowerGameCardType.h"
//#include "FontConfig.h"

namespace goldenflower
{
	GameCardType::GameCardType()
	{
		init();
	}

	GameCardType::~GameCardType()
	{

	}

	GameCardType* GameCardType::create()
	{
		auto* ptr = new GameCardType();
		if (nullptr != ptr)
		{
			ptr->autorelease();
			return ptr;
		}
		return nullptr;
	}


	bool GameCardType::init()
	{
		auto dispatcher = Director::getInstance()->getEventDispatcher();
		auto listener = EventListenerTouchOneByOne::create();
		listener->setSwallowTouches(true);
		listener->onTouchBegan = CC_CALLBACK_2(GameCardType::onTouchBegan,this);
		dispatcher->addEventListenerWithSceneGraphPriority(listener,this);

		//change size.
		const float SCALE = 0.75f;
		auto bg = Sprite::create("goldenflower/table/display_card_type.png");
		bg->setScale(SCALE);
		auto size = bg->getContentSize() * SCALE;

		this->setContentSize(size);
		bg->setPosition(size.width/2,size.height/2);
		bg->ignoreAnchorPointForPosition(false);
		this->addChild(bg,1);

		////皇家同花顺
		//Label* royalFlush = createLabel(gFontConfig_18, GBKToUtf8("皇家同花顺"), colorGold);
		//royalFlush->setAnchorPoint(Vec2(0, 0.5));
		//royalFlush->setPosition(Vec2(bg->getContentSize().width * 0.6, size.height * 0.95));
		//bg->addChild(royalFlush, 2);
		////同花顺
		//auto straightFlush = createLabel(gFontConfig_18, GBKToUtf8("同花顺"), colorGold);
		//straightFlush->setAnchorPoint(Vec2(0, 0.5));
		//straightFlush->setPosition(Vec2(bg->getContentSize().width * 0.6, size.height * 0.85));
		//bg->addChild(straightFlush, 2);
		////四条
		//auto fourOfAKind = createLabel(gFontConfig_18, GBKToUtf8("四条"), colorGold);
		//fourOfAKind->setAnchorPoint(Vec2(0, 0.5));
		//fourOfAKind->setPosition(Vec2(bg->getContentSize().width * 0.6, size.height * 0.75));
		//bg->addChild(fourOfAKind, 2);
		////葫芦
		//auto fullhouse = createLabel(gFontConfig_18, GBKToUtf8("葫芦"), colorGold);
		//fullhouse->setAnchorPoint(Vec2(0, 0.5));
		//fullhouse->setPosition(Vec2(bg->getContentSize().width * 0.6, size.height * 0.65));
		//bg->addChild(fullhouse, 2);
		////同花
		//auto flush = createLabel(gFontConfig_18, GBKToUtf8("同花"), colorGold);
		//flush->setAnchorPoint(Vec2(0, 0.5));
		//flush->setPosition(Vec2(bg->getContentSize().width * 0.6, size.height * 0.55));
		//bg->addChild(flush, 2);
		////顺子
		//auto straight = createLabel(gFontConfig_18, GBKToUtf8("顺子"), colorGold);
		//straight->setAnchorPoint(Vec2(0, 0.5));
		//straight->setPosition(Vec2(bg->getContentSize().width * 0.6, size.height * 0.45));
		//bg->addChild(straight, 2);
		////三条
		//auto threeOfAkind = createLabel(gFontConfig_18, GBKToUtf8("三条"), colorGold);
		//threeOfAkind->setAnchorPoint(Vec2(0, 0.5));
		//threeOfAkind->setPosition(Vec2(bg->getContentSize().width * 0.6, size.height * 0.35));
		//bg->addChild(threeOfAkind, 2);
		////两对
		//auto twoPairs = createLabel(gFontConfig_18, GBKToUtf8("两对"), colorGold);
		//twoPairs->setAnchorPoint(Vec2(0, 0.5));
		//twoPairs->setPosition(Vec2(bg->getContentSize().width * 0.6, size.height * 0.25));
		//bg->addChild(twoPairs, 2);
		////一对
		//auto onePair = createLabel(gFontConfig_18, GBKToUtf8("一对"), colorGold);
		//onePair->setAnchorPoint(Vec2(0, 0.5));
		//onePair->setPosition(Vec2(bg->getContentSize().width * 0.6, size.height * 0.15));
		//bg->addChild(onePair, 2);
		////高牌
		//auto highCard = createLabel(gFontConfig_18, GBKToUtf8("高牌"), colorGold);
		//highCard->setAnchorPoint(Vec2(0, 0.5));
		//highCard->setPosition(Vec2(bg->getContentSize().width * 0.6, size.height * 0.05));
		//bg->addChild(highCard, 2);

		//auto cardtype = Sprite::create("table/display_card_type.png");
		//cardtype->setAnchorPoint(Vec2(0.0f,0.5f));
		//cardtype->setPosition(10,size.height/2);
		//this->addChild(cardtype,2);

		return true;
	}

	bool GameCardType::onTouchBegan(Touch *touch, Event *unused_event)
	{
		auto pos = touch->getLocation();
		pos = this->convertToNodeSpace(pos);
		Rect rect(0, 0, this->getContentSize().width, this->getContentSize().height);
		if(!rect.containsPoint(pos))
		{
			auto winSize = Director::getInstance()->getWinSize();
			auto size = this->getContentSize();
			auto destPos = Vec2(winSize.width + size.width, 0);
			this->stopAllActions();
			auto moveTo = MoveTo::create(0.2f,destPos);
			auto seq = Sequence::create(moveTo, RemoveSelf::create(true), nullptr);	
			this->runAction(seq);
		}
		return true;
	}
}


