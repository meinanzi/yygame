#include "DZPokerGameCardType.h"

namespace DZPoker
{
	static const char* CARD_TYPE_BG		= "dzpoker/table/card_typebg_new.png";
	static const char* CARD_TYPE		= "dzpoker/table/display_card_type_new.png";

	GameCardType::GameCardType()
	{
	}

	GameCardType::~GameCardType()
	{
	}

	bool GameCardType::init()
	{
		if (!HNLayer::init())
		{
			return false;
		}

		//背景
		auto bg = Sprite::create(CARD_TYPE_BG);
		auto size = bg->getContentSize();
		this->setContentSize(size);
		bg->setPosition(size.width/2, size.height/2);
		bg->ignoreAnchorPointForPosition(false);
		this->addChild(bg, 1);

		auto dispatcher = Director::getInstance()->getEventDispatcher();
		auto listener = EventListenerTouchOneByOne::create();
		listener->setSwallowTouches(true);
		listener->onTouchBegan = CC_CALLBACK_2(GameCardType::onTouchBegan, this);
		dispatcher->addEventListenerWithSceneGraphPriority(listener, this);

		//皇家同花顺
		Label* royalFlush = createLabel(GBKToUtf8("皇家同花顺"), 20);
		royalFlush->setColor(colorGold);
		royalFlush->setAnchorPoint(Vec2(0, 0.5));
		royalFlush->setPosition(Vec2(bg->getContentSize().width * 0.6, size.height * 0.95));
		bg->addChild(royalFlush, 2);

		//同花顺
		auto straightFlush = createLabel(GBKToUtf8("同花顺"), 20);
		straightFlush->setColor(colorGold);
		straightFlush->setAnchorPoint(Vec2(0, 0.5));
		straightFlush->setPosition(Vec2(bg->getContentSize().width * 0.6, size.height * 0.85));
		bg->addChild(straightFlush, 2);

		//四条
		auto fourOfAKind = createLabel(GBKToUtf8("四条"), 20);
		fourOfAKind->setColor(colorGold);
		fourOfAKind->setAnchorPoint(Vec2(0, 0.5));
		fourOfAKind->setPosition(Vec2(bg->getContentSize().width * 0.6, size.height * 0.75));
		bg->addChild(fourOfAKind, 2);

		//葫芦
		auto fullhouse = createLabel(GBKToUtf8("葫芦"), 20);
		fullhouse->setColor(colorGold);
		fullhouse->setAnchorPoint(Vec2(0, 0.5));
		fullhouse->setPosition(Vec2(bg->getContentSize().width * 0.6, size.height * 0.65));
		bg->addChild(fullhouse, 2);

		//同花
		auto flush = createLabel(GBKToUtf8("同花"), 20);
		flush->setColor(colorGold);
		flush->setAnchorPoint(Vec2(0, 0.5));
		flush->setPosition(Vec2(bg->getContentSize().width * 0.6, size.height * 0.55));
		bg->addChild(flush, 2);

		//顺子
		auto straight = createLabel(GBKToUtf8("顺子"), 20);
		straight->setColor(colorGold);
		straight->setAnchorPoint(Vec2(0, 0.5));
		straight->setPosition(Vec2(bg->getContentSize().width * 0.6, size.height * 0.45));
		bg->addChild(straight, 2);

		//三条
		auto threeOfAkind = createLabel(GBKToUtf8("三条"), 20);
		threeOfAkind->setColor(colorGold);
		threeOfAkind->setAnchorPoint(Vec2(0, 0.5));
		threeOfAkind->setPosition(Vec2(bg->getContentSize().width * 0.6, size.height * 0.35));
		bg->addChild(threeOfAkind, 2);

		//两对
		auto twoPairs = createLabel(GBKToUtf8("两对"), 20);
		twoPairs->setColor(colorGold);
		twoPairs->setAnchorPoint(Vec2(0, 0.5));
		twoPairs->setPosition(Vec2(bg->getContentSize().width * 0.6, size.height * 0.25));
		bg->addChild(twoPairs, 2);

		//一对
		auto onePair = createLabel(GBKToUtf8("一对"), 20);
		onePair->setColor(colorGold);
		onePair->setAnchorPoint(Vec2(0, 0.5));
		onePair->setPosition(Vec2(bg->getContentSize().width * 0.6, size.height * 0.15));
		bg->addChild(onePair, 2);

		//高牌
		auto highCard = createLabel(GBKToUtf8("高牌"), 20);
		highCard->setColor(colorGold);
		highCard->setAnchorPoint(Vec2(0, 0.5));
		highCard->setPosition(Vec2(bg->getContentSize().width * 0.6, size.height * 0.05));
		bg->addChild(highCard, 2);

		auto cardtype = Sprite::create(CARD_TYPE);
		cardtype->setAnchorPoint(Vec2(0.0f, 0.5f));
		cardtype->setPosition(10, size.height/2);
		this->addChild(cardtype, 2);

		return true;
	}

	bool GameCardType::onTouchBegan(Touch *touch, Event *unused_event)
	{
		auto pos = this->getPosition();
		if (pos.x < 0)
		{
			return false;
		}
		auto size = this->getContentSize();
		auto des_pos = Vec2(-size.width,0.0f);
		this->stopAllActions();
		auto mveTo = MoveTo::create(0.2f,des_pos);
		auto funcall = CallFuncN::create(CC_CALLBACK_1(GameCardType::moveByAnimationCallback,this));
		auto seq = Sequence::create(mveTo,funcall, nullptr);
		this->runAction(seq);
		return false;
	}

	void GameCardType::moveByAnimationCallback(Node* sender)
	{
		sender->setVisible(false);
	}

}
