#include "LandlordPokerCard.h"
#include "LandlordGameAudio.h"

namespace landlord
{
	#define Tag_Color_Layer 100
	#define File_Card_Mark "landlord/game/studioUI/uiRes/card_mark.png"

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
			(cardValue >= 0x31 && cardValue <= 0x3D) ||
			(cardValue >= 0x4E && cardValue <= 0x4F)))
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
		if (!Sprite::initWithSpriteFrameName(getCardTextureFileName(cardValue)))
		{
			return false;
		}

		_Value = cardValue;
		return true;
	}

	void PokerCard::setCardValue(BYTE cardValue)
	{
		// out of range
		if (cardValue < 0x00 || cardValue > 0x3D)
		{
			if(cardValue != 0x4E && cardValue != 0x4F)
			{
				return;
			}			
		}

		this->setSpriteFrame(getCardTextureFileName(cardValue));
		_Value = cardValue;
	}

	BYTE PokerCard::getCardValue()
	{
		return _Value;
	}

	void PokerCard::addTouchedLayer()
	{
		if (this->getChildByTag(Tag_Color_Layer) == nullptr)
		{
			auto mark = Sprite::create(File_Card_Mark);
			mark->setAnchorPoint(Vec2::ZERO);
			mark->setPosition(Vec2::ZERO);
			this->addChild(mark, 1, Tag_Color_Layer);

			GameAudio::playDispathCard();
		}
	}

	void PokerCard::removeTouchedLayer()
	{
		Node* pNode = this->getChildByTag(Tag_Color_Layer);
		if(pNode != nullptr)
		{
			pNode->removeFromParent();
		}
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
		if (isGray)
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
			this->setSpriteFrame(getCardTextureFileName(_Value));
		}
	}
}

