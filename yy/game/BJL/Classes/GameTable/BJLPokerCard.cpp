#include "BJLPokerCard.h"

namespace BJL
{

	PokerCard::PokerCard()
	{
		_Value = 0;
		setCardFrameName("0x00.png");
	}

	PokerCard::~PokerCard()
	{

	}

	PokerCard* PokerCard::create()
	{
		PokerCard* card = new PokerCard();
		
		if (card->init())
		{
			card->autorelease();
			return card;
		}
		
		CC_SAFE_DELETE(card);
		
		return nullptr;
	}

	void PokerCard::setStartPositin(Vec2 position)
	{
		setPosition(position);
	}

	void PokerCard::standUP()
	{
		Vec2 pos = getPosition();
		pos.y += 20;
		setPosition(pos);
	}

	void PokerCard::sitDown()
	{
		Vec2 pos = getPosition();
		pos.y -= 20;
		setPosition(pos);
	}

	void PokerCard::setCardFileName(const std::string& filename)
	{
		initWithFile(filename);
	}

	void PokerCard::setCardFrameName(const std::string& filename)
	{
		initWithSpriteFrameName(filename);
	}

	std::string PokerCard::getCardFileName()
	{
		if(_Value == 0x00)
		{
			return std::string("0x00.png");
		}

		char str[20] = {0};
		BYTE byColor = _Value & 0xf0;
		BYTE byValue = _Value & 0x0f;
		byValue = (byValue + 1) % 13;
		if (0 == byValue)
		{
			byValue += 13;
		}

		sprintf(str,"0x%02X.png", byColor | byValue);
		return str;
	}

	void PokerCard::setCardValue(BYTE val)
	{
		_Value = val;
	}

	BYTE PokerCard::getCardValue()
	{
		return _Value;
	}

}
