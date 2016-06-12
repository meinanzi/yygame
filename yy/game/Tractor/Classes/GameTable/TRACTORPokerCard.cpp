#include "TRACTORPokerCard.h"

namespace TRACTOR
{
	PokerCard::PokerCard()
	{
		_ignoreAnchorPointForPosition = false;
	}

	PokerCard::~PokerCard()
	{
	}

	PokerCard* PokerCard::create(BYTE cardValue)
	{
		// out of range
		if (!(
			(cardValue >= 0x00 && cardValue <= 0x0D) ||
			(cardValue >= 0x11 && cardValue <= 0x1D) ||
			(cardValue >= 0x21 && cardValue <= 0x2D) ||
			(cardValue >= 0x31 && cardValue <= 0x3D)))
		{
			return nullptr;
		}

		PokerCard* card = new PokerCard();
		if (card && card->initWithCardValue(cardValue))
		{
			card->autorelease();
			return card;
		}
		CC_SAFE_DELETE(card);
		return nullptr;
	}

	bool PokerCard::initWithCardValue(BYTE cardValue)
	{
		if (!Sprite::init())
		{
			return false;
		}

		_Value = cardValue;
		return this->initWithSpriteFrameName(getCardTextureFileName(_Value));
	}

	void PokerCard::setCardValue(BYTE cardValue)
	{
		//´óÐ¡Íõ
		if (cardValue==0x4E || cardValue==0x4F)
		{
			this->setSpriteFrame(PokerCard::getCardTextureFileName(cardValue));
			_Value = cardValue;
			return;
		}
		// out of range
		if (cardValue<0x00 || cardValue>0x3D)
		{
			return;
		}

		this->setSpriteFrame(PokerCard::getCardTextureFileName(cardValue));
		_Value = cardValue;
	}

	BYTE PokerCard::getCardValue()
	{
		return _Value;
	}

	string PokerCard::getCardTextureFileName(BYTE cardValue)
	{
		char filename[10];
		sprintf(filename, "0x%02X.png", cardValue);

		return filename;
	}

	void PokerCard::setGray(bool isGray)
	{
		// true - set to gray
		if (isGray == true)
		{
			Sprite* sprite_chess = Sprite::createWithTexture(this->getTexture());
			sprite_chess->setPosition(sprite_chess->getContentSize().width / 2, sprite_chess->getContentSize().height / 2);

			RenderTexture *render = RenderTexture::create(sprite_chess->getContentSize().width, sprite_chess->getContentSize().height, Texture2D::PixelFormat::RGBA8888);
			render->beginWithClear(0.0f, 0.0f, 0.0f, 0.0f);
			sprite_chess->visit();
			render->end();
			Director::getInstance()->getRenderer()->render();

			Image *finalImage = render->newImage();

			unsigned char *pData = finalImage->getData();

			int iIndex = 0;

			for (int i = 0; i < finalImage->getHeight(); i++)
			{
				for (int j = 0; j < finalImage->getWidth(); j++)
				{
					// gray
					int iBPos = iIndex;

					unsigned int iB = pData[iIndex];
					iIndex++;

					unsigned int iG = pData[iIndex];
					iIndex++;

					unsigned int iR = pData[iIndex];
					iIndex++;
					iIndex++;

					unsigned int iGray = 0.3 * iR + 0.6 * iG + 0.1 * iB;

					pData[iBPos] = pData[iBPos + 1] = pData[iBPos + 2] = (unsigned char)iGray;
				}
			}

			Texture2D *texture = new Texture2D;
			texture->initWithImage(finalImage);
			this->setTexture(texture);

			delete finalImage;
			texture->release();
		}
		// false - set to colored
		else
		{
			this->setSpriteFrame(PokerCard::getCardTextureFileName(_Value));
		}
	}

	//#include "PokerCard.h"
	//
	//
	//PokerCard::PokerCard()
	//{
	//	_Value = 0;
	//	setCardFrameName("0x00.png");
	//}
	//
	//PokerCard::~PokerCard()
	//{
	//
	//}
	//
	//PokerCard* PokerCard::create()
	//{
	//	PokerCard* card = new PokerCard();
	//	if (card->init())
	//	{
	//		card->autorelease();
	//		return card;
	//	}
	//	CC_SAFE_DELETE(card);
	//	return nullptr;
	//}
	//
	//void PokerCard::setStartPositin(Vec2 position)
	//{
	//	setPosition(position);
	//}
	//
	//void PokerCard::standUP()
	//{
	//	Vec2 pos = getPosition();
	//	pos.y += 20;
	//	setPosition(pos);
	//}
	//
	//void PokerCard::sitDown()
	//{
	//	Vec2 pos = getPosition();
	//	pos.y -= 20;
	//	setPosition(pos);
	//}
	//
	//void PokerCard::setCardFileName(const std::string& filename)
	//{
	//	initWithFile(filename);
	//}
	//
	//void PokerCard::setCardFrameName(const std::string& filename)
	//{
	//	initWithSpriteFrameName(filename);
	//}
	//
	//std::string PokerCard::getCardFileName()
	//{
	//	if(_Value == 0x00)
	//	{
	//		return std::string("0x00.png");
	//	}
	//
	//	char str[20] = {0};
	//	BYTE byColor = _Value & 0xf0;
	//	BYTE byValue = _Value & 0x0f;
	//	byValue = (byValue + 1) % 13;
	//	if (0 == byValue)
	//	{
	//		byValue += 13;
	//	}
	//	sprintf(str,"0x%02X.png", byColor | byValue);
	//	return str;
	//}
	//
	//void PokerCard::setCardValue(BYTE val)
	//{
	//	_Value = val;
	//}
	//
	//BYTE PokerCard::getCardValue()
	//{
	//	return _Value;
	//}
}