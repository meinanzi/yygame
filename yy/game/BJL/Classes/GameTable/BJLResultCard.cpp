#include "BJLGameTableUI.h"
#include "BJLResultCard.h"


namespace BJL
{
	static const int ZORDER_MAX  = 200;
	
	static const int PAI_JIANJU  = 50;     //牌间距

	ResultCard::~ResultCard()
	{
	}

	ResultCard::ResultCard(GameTableUI* gameTableUi)
		: _bSendCardIndex(0)
	{
		_gameTableUi = gameTableUi;
		
		auto size = gameTableUi->getContentSize();
		_csLoader = CSLoader::createNode("BJL/game/BJLStudio/cardbroad.csb");
		//_csLoader->ignoreAnchorPointForPosition(false);

		auto panel = _csLoader->getChildByName("Panel_2");
		background = dynamic_cast<ui::ImageView*>(panel->getChildByName("bg"));
		_csLoader->setContentSize(background->getContentSize());
		_panel = panel;

		_csLoader->setAnchorPoint(Vec2(0, 0));
		Vec2 position = Vec2(size.width/2, size.height - background->getContentSize().height / 2);
		_csLoader->setPosition(position);
		_csLoader->setVisible(false);
		gameTableUi->addChild(_csLoader,ZORDER_MAX);

		char chs[30] = {0};
		for (BYTE i = 0 ; i < 2 ; i++  )
		{
			sprintf(chs, "cardsss_%d", i);
			cardTest[i] = dynamic_cast<ImageView*>(panel->getChildByName(chs));
		}

		cardsss = dynamic_cast<ImageView*>(panel->getChildByName("cardsss"));

		// 庄家、闲家点数
		leisure = dynamic_cast<ImageView*>(panel->getChildByName("leisure"));
		dealer = dynamic_cast<ImageView*>(panel->getChildByName("dealer"));

		// 比赛结果
		leisure_win = dynamic_cast<ImageView*>(panel->getChildByName("leisure_win"));
		dealer_win = dynamic_cast<ImageView*>(panel->getChildByName("dealer_win"));
		dealer_leisure = dynamic_cast<ImageView*>(panel->getChildByName("equal"));

		for ( BYTE i = 0 ; i < 2 ; i++)
		{
			for (BYTE j = 0 ; j  < 3 ; j++)
			{
				_spriteCard[i][j] = PokerCard::create();
				_spriteCard[i][j]->setAnchorPoint(Vec2(0,1));
				_panel->addChild(_spriteCard[i][j],10);
				_spriteCard[i][j]->setScale(1.4f);
				auto des_pos = cardTest[i]->getPosition();
				des_pos.x += j * PAI_JIANJU;
				destinationPositon[i][j]  =	des_pos;
			}
		}

		for (BYTE i = 0 ; i < 2 ; i++)
		{
			spArr[i] = nullptr;
		}
	}


	void ResultCard::dispatchCardAnimation(PokerCard* pokerCard , BYTE state)
	{
		auto orbit = OrbitCamera::create(0.1f, 1, 0, 0, -90, 0, 0);
		auto orbit1 = OrbitCamera::create(0.2f, 1, 0, 90, -90, 0, 0);
		auto des_pos = destinationPositon[(state + 1) % 2 ][(state - 1) / 2];
		auto seq = Sequence::create(
			Show::create(),
			EaseOut::create(EaseSineOut::create(MoveTo::create(0.5f, des_pos)), 0.3f),
			orbit,
			CallFuncN::create(CC_CALLBACK_1(ResultCard::cardFlipCallback,this)),
			orbit1,
			nullptr
			);
		pokerCard->runAction(seq);
	}

	void ResultCard::cardFlipCallback(Node* sender)
	{
		auto card = (PokerCard*)sender;
		card->setCardFrameName(card->getCardFileName());
	}

	bool ResultCard::dispatchCard()
	{
		if (_bSendCardIndex >= 6)
		{
			//TODO   添加结果
			calculatePoint();
			return false;
		}
		++_bSendCardIndex;
		
		// 发牌，从闲家开始发
		if (0 == _cards[(_bSendCardIndex + 1)%2][(_bSendCardIndex - 1) / 2])
		{
			return dispatchCard();
		}
		else
		{
			dispatchCardAnimation(_spriteCard[(_bSendCardIndex + 1)%2][(_bSendCardIndex - 1) / 2],_bSendCardIndex);
		}
		return true;
	}

	void ResultCard::showCardBorad()
	{
		auto winSize  = _gameTableUi->getContentSize();
		auto size = _csLoader->getContentSize();
		auto seq = Sequence::create(
			Show::create(),
			EaseSineOut::create(MoveTo::create(0.5f, Vec2(winSize.width / 2 , winSize.height - size.height / 2))),
			nullptr
			);
		_csLoader->runAction(seq);
	}

	void ResultCard::showCardBoradWithOutAnimation()
	{
		auto winSize  = _gameTableUi->getContentSize();
		auto size = _csLoader->getContentSize();
		_csLoader->setVisible(true);
		_csLoader->setPosition( Vec2(winSize.width / 2 , winSize.height - size.height / 2));
	}

	bool ResultCard::dispatchCardWithoutAnimation()
	{
		//TODO   添加结果
		calculatePoint();
		//发牌 == 直接显示牌
		for (BYTE i = 0 ; i < 3 ; i++)
		{
			if (0 != _cards[0][i])
			{
				auto des_pos = destinationPositon[0][i];
				_spriteCard[0][i]->setCardFrameName(_spriteCard[0][i]->getCardFileName());
				_spriteCard[0][i]->setPosition(des_pos);
				_spriteCard[0][i]->setVisible(true);
			}
			if (0 != _cards[1][i])
			{
				auto des_pos = destinationPositon[1][i];
				_spriteCard[1][i]->setCardFrameName(_spriteCard[1][i]->getCardFileName());
				_spriteCard[1][i]->setPosition(des_pos);
				_spriteCard[1][i]->setVisible(true);
			}
		}
		return true;
	}

	void ResultCard::calculatePoint()
	{
		// 闲家
		char str[50];
		sprintf(str,"game_cardtype_%d.png",_leisureCardtype[1]);
		auto spl = Sprite::createWithSpriteFrameName(str);
		spl->setAnchorPoint(Vec2(0.5f , 0));
		spl->setPosition(Vec2(leisure->getPosition().x , leisure->getPosition().y - leisure->getContentSize().height / 2));
		_panel->addChild(spl,10);
		spArr[0] = spl;


		// 庄家
		sprintf(str,"game_cardtype_%d.png",_dealerCardtype[1]);
		auto spd = Sprite::createWithSpriteFrameName(str);
		spd->setAnchorPoint(Vec2(0.5f , 0));
		spd->setPosition(Vec2(dealer->getPosition().x , dealer->getPosition().y - dealer->getContentSize().height / 2));
		_panel->addChild(spd,10);
		spArr[1] = spd;

		if (_leisureCardtype[1] > _dealerCardtype[1])
		{
			leisure_win->setVisible(true);
			leisure->setVisible(false);
		}
		else if(_leisureCardtype[1] < _dealerCardtype[1])
		{
			dealer_win->setVisible(true);
			dealer->setVisible(false);
		}
		else
		{
			dealer_leisure->setVisible(true);
		}
	}

	void ResultCard::hideCardBorad()
	{
		auto winSize = _gameTableUi->getContentSize();
		auto seq = Sequence::create(
			EaseSineIn::create(MoveTo::create(0.5f, Vec2(winSize.width / 2 , winSize.height))),
			Hide::create(),
			nullptr
			);
		_csLoader->runAction(seq);
	}


	void ResultCard::initCard(BYTE card[2][3], BYTE dealerCardNum, BYTE leisureCardNum,int dealerCardtype[5],int leisureCardtype[5])
	{
		memcpy(_dealerCardtype,  dealerCardtype, sizeof(int) * 5);
		memcpy(_leisureCardtype, leisureCardtype, sizeof(int) * 5);
		memcpy(_cards, card, sizeof(_cards));

		_dealerCardNum  = dealerCardNum;
		_leisureCardNum = leisureCardNum;

		for (int i = 0; i < 2; i++)
		{
			for (int j = 0 ; j < 3 ; j++ )
			{
				_spriteCard[i][j]->setCardValue(card[i][j]);
				_spriteCard[i][j]->setCardFrameName("0x00.png");
				
				_spriteCard[i][j]->setPosition(cardsss->getPosition());
				_spriteCard[i][j]->setVisible(false);
			}
		}
	}

	void ResultCard::initNewGame()
	{
		_bSendCardIndex = 0;
		leisure_win->setVisible(false);
		dealer_win->setVisible(false);
		leisure->setVisible(true);
		dealer->setVisible(true);
		dealer_leisure->setVisible(false);
		for ( BYTE i =  0 ; i < 2; i++)
		{
			if (nullptr != spArr[i])
			{
				spArr[i]->removeFromParent();
				spArr[i] = nullptr;
			}
		}
	}


	BYTE ResultCard::getCardFileValue(const WORD& card)
	{
		BYTE value = BYTE( 0x000F & card ) - 1;
		BYTE color = BYTE( 0x00F0 & card );
		//大小王
		if (4 == color || 0x0E == value)
		{
			value = 0x4E;
		}
		else if (4 == color || 0x0F == value)
		{
			value = 0x4F;
		}
		else
		{
			color = color>>4;
			color %= 4;
			//花色校准
			if (0 == color)
			{
				color = 3;
			}
			else
			{
				color -= 1;
			}
			color = color<<4;
			value = color | value; 
			if (0x00 == value)
			{
				value = 0x3D;
			}
		}
		return value;
	}
}