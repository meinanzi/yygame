#include "LandlordCardListBoard.h"
#include "LandlordGameAudio.h"

#include <algorithm>

using namespace HN;

/*
 * 玩家出牌操作面板
 * Tag用来记录牌是否被点击
 * Name用来记录牌是否被提起
 */

#define UP_OFFSET				30.0f
#define Self_Card_Min_Interval  32.0f		//两张牌间最小间隔
#define Self_Card_Max_Interval  80.0f		//两张牌间最大间隔
#define Other_Card_Interval     15.0f

#define Name_Up        "up"
#define Name_Down      "down"

#define Tag_Touched    1
#define Tag_Untouched  0

#define Zorder_Card_Began 1
#define Zorder_Touch      100


static float send_card_time		=	0.2f;

namespace landlord
{
	CardListBoard* CardListBoard::create(bool isMySelf, BYTE seatNo)
	{
		CardListBoard* action = new CardListBoard();
		if(action->init(isMySelf, seatNo))
		{
			action->autorelease();
			return action;
		}
		CC_SAFE_DELETE(action);
		return nullptr;
	}

	bool CardListBoard::init(bool isMySelf, BYTE seatNo)
	{
		if(!HNLayer::init())
		{
			return false;
		}
		this->ignoreAnchorPointForPosition(false);
		this->setAnchorPoint(Vec2(0.5f, 0.5f));
		this->setContentSize(Size::ZERO);

		_isMySelf = isMySelf;
		_seatNo = seatNo;

		if(_isMySelf)
		{
			auto listener = EventListenerTouchOneByOne::create();
			listener->onTouchBegan = CC_CALLBACK_2(CardListBoard::onTouchBegan, this);
			listener->onTouchMoved = CC_CALLBACK_2(CardListBoard::onTouchMoved, this);
			listener->onTouchEnded = CC_CALLBACK_2(CardListBoard::onTouchEnded, this);
			listener->setSwallowTouches(true);
			this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
		}

		return true;
	}

	void CardListBoard::addCardOneByOne(const std::vector<BYTE>& values)
	{
		if(values.empty())
		{
			return;
		}

		unschedule(schedule_selector(CardListBoard::scheduleRun));
		_undoList.clear();
		removeCardAllOnce(values);

		_undoList = values;
		schedule(schedule_selector(CardListBoard::scheduleRun), send_card_time);
	}

	void CardListBoard::addCardAllOnce(const std::vector<BYTE>& values, bool isUp)
	{
		for(BYTE value: values)
		{
			addCard(value, isUp);
		}
		reorderCard();
		resizeCardList();
	}

	void CardListBoard::getUpCards(std::vector<BYTE>* upCards)
	{
		if (nullptr == upCards) return;

		{
			std::vector<BYTE>().swap(*upCards);
		}

		for(size_t i = 0; i < _cardsPtr.size(); i++)
		{
			if(_cardsPtr[i]->getName().compare(Name_Up) == 0)
			{
				upCards->push_back(_cardsPtr[i]->getCardValue());
			}
		}
	}

	void CardListBoard::getTouchedCards(std::vector<BYTE>* outCards)
	{
		if (nullptr == outCards) return;

		{
			std::vector<BYTE>().swap(*outCards);
		}

		for(size_t i = 0; i < _cardsPtr.size(); i++)
		{
			if(_cardsPtr[i]->getTag() == Tag_Touched)
			{
				outCards->push_back(_cardsPtr[i]->getCardValue());
			}
		}
	}

	bool CardListBoard::onTouchBegan(Touch* touch, Event *event)
	{
		removeTouchedTag();

		Vec2 pos = this->convertToNodeSpace(touch->getLocation());
		touchCheck(pos);
		_startMovePoint = pos;
		_midMovePoint	= pos;

		Rect rect(0,0, this->getContentSize().width, this->getContentSize().height);
		if(rect.containsPoint(pos))
		{
			return true;
		}
		else
		{
			downCards();
			return false;
		}
	}

	/*
	 * PokerCard be used
	 * name for up or down
	 * tag for touched or not, 0 meaning untouched, 1 meaning touched.
	 */
	void CardListBoard::onTouchMoved(Touch* touch, Event *event)
	{
		Vec2 pos = this->convertToNodeSpace(touch->getLocation());
		_currentMovePoint = pos;
		bool isAhead = (_currentMovePoint.x > _midMovePoint.x && _midMovePoint.x >= _startMovePoint.x) || 
			(_currentMovePoint.x < _midMovePoint.x && _midMovePoint.x <= _startMovePoint.x);
		touchCheck(pos, isAhead);
		_midMovePoint = pos;
	}

	void CardListBoard::onTouchEnded(Touch* touch, Event *event)
	{
		Vec2 pos = this->convertToNodeSpace(touch->getLocation());
		_currentMovePoint = pos;
		bool isAhead = (_currentMovePoint.x >= _midMovePoint.x && _midMovePoint.x >= _startMovePoint.x) || 
			(_currentMovePoint.x <= _midMovePoint.x && _midMovePoint.x <= _startMovePoint.x);
		touchCheck(pos, isAhead);

		for(size_t i = 0; i < _cardsPtr.size(); i++)
		{
			if(_cardsPtr[i] != nullptr)
			{
				_cardsPtr[i]->removeTouchedLayer();
			}
		}

		std::vector<BYTE> cards;
		getUpCards(&cards);
		if(cards.empty())
		{
			if (_callfunc != nullptr)
			{
				_callfunc(this);
			}
		}
		else
		{
			changeUpDown();
		}	
	}

	void CardListBoard::scheduleRun(float delta)
	{
		if (_undoList.empty())	
		{
			unschedule(schedule_selector(CardListBoard::scheduleRun));
		}
		else
		{
			GameAudio::playDispathCard();
			addCard(_undoList[0], false);
			reorderCard();
			resizeCardList();
			_undoList.erase(_undoList.begin());
		}
	}

	bool CardListBoard::touchCheck(const Vec2& pos, bool isAhead)
	{
		for(size_t i = 0; i < _cardsPtr.size(); i++)
		{
			int index = _cardsPtr.size() - i - 1;
			Rect rect = _cardsPtr[index]->getBoundingBox();
			if(rect.containsPoint(pos))
			{
				if (!isAhead && _cardsPtr[index]->getTag() == Tag_Touched)
				{
					_cardsPtr[index]->setTag(Tag_Untouched);
					_cardsPtr[index]->removeTouchedLayer();
				}
				else if (isAhead && _cardsPtr[index]->getTag() == Tag_Untouched)
				{
					_cardsPtr[index]->setTag(Tag_Touched);
					_cardsPtr[index]->addTouchedLayer();
				}

				return true;
			}
		}
		return false;
	}

	void CardListBoard::setCallFunction(std::function<void(cocos2d::Node*)> callfunc)
	{
		_callfunc = callfunc;
	}

	void CardListBoard::clear()
	{
		unschedule(schedule_selector(CardListBoard::scheduleRun));
		_undoList.clear();
		for(size_t i = 0; i < _cardsPtr.size(); i++)
		{
			if(_cardsPtr[i] != nullptr)
			{
				_cardsPtr[i]->removeFromParent();
			}
		}

		_cardsPtr.clear();
	}

	void CardListBoard::addCard(BYTE value, bool isUp)
	{
		PokerCard* card = PokerCard::create(value);
		if(card != nullptr)
		{
			card->setScale(_isMySelf ? 0.8f : 0.3f);
			card->setPositionY(isUp ? UP_OFFSET : 0);
			card->setName(isUp ? Name_Up: Name_Down);
			card->setTag(Tag_Untouched);
			addChild(card);
			_cardsPtr.push_back(card);
		}
	}


	void CardListBoard::removeCard(BYTE value)
	{
		auto ptr = find_if(_cardsPtr.begin(), _cardsPtr.end(), [value](PokerCard* card)
			{
				return (card->getCardValue() == value);
			});
		if (ptr != _cardsPtr.end())
		{
			PokerCard* card = *ptr;
			card->removeFromParent();
			_cardsPtr.erase(ptr);
		}
	}

	void CardListBoard::removeCardAllOnce(const std::vector<BYTE>& values)
	{
		if(values.empty())
		{
			//downCards();
			return;
		}

		for(BYTE value: values)
		{
			removeCard(value);
		}
		resizeCardList();
		GameAudio::playDispathCard();
	}

	void CardListBoard::reorderCard()
	{
		std::sort(_cardsPtr.begin(), _cardsPtr.end(), [=] (PokerCard* left, PokerCard* right) -> bool
			{
				BYTE lCard = left->getCardValue();
				BYTE rCard = right->getCardValue();

				BYTE lColor = lCard  & 0xf0;
				BYTE rColor = rCard & 0xf0;
				BYTE lValue = lCard  & 0x0f;
				BYTE rValue = rCard & 0x0f;

				//2
				if(lValue == 0x01)
				{
					lValue = 0x0e;
				}
				if(rValue == 0x1)
				{
					rValue = 0x0e;
				}

				return ((rValue < lValue) || (rValue == lValue && rColor < lColor));
			});
	}

	void CardListBoard::removeTouchedTag()
	{
		for(size_t i = 0 ; i < _cardsPtr.size(); i++)
		{
			if(_cardsPtr[i] != nullptr)
			{
				_cardsPtr[i]->setTag(Tag_Untouched);
			}
		}
	}

	void CardListBoard::downCards()
	{
		float duration = _isMySelf ? 0.01f : 2.0f;

		for(size_t i = 0; i < _cardsPtr.size(); i++)
		{
			downCard(_cardsPtr[i], duration);
			_cardsPtr[i]->setTag(Tag_Untouched);
		}
	}

	void CardListBoard::downCard(PokerCard* card, float delay)
	{
		if(card->getName().compare(Name_Up) == 0)
		{			
			card->setName(Name_Down);
			card->stopAllActions();
			card->runAction(Sequence::create(DelayTime::create(delay), MoveTo::create(0.2f, Vec2(card->getPositionX(), 0)), nullptr));
		}
	}

	void CardListBoard::changeUpDown()
	{
		for(size_t i = 0; i < _cardsPtr.size(); i++)
		{
			if(_cardsPtr[i]->getTag() == Tag_Untouched)	continue;

			if(_cardsPtr[i]->getName().compare(Name_Up) == 0)
			{
				downCard(_cardsPtr[i], 0);
			}
			else
			{
				upCard(_cardsPtr[i]);
			}
			_cardsPtr[i]->setTag(Tag_Untouched);
		}
	}

	/*
	 * up the need up cards, and down the cards that not need up.
	 */
	void CardListBoard::upCards(const std::vector<BYTE>& cards)
	{
		for(size_t i = 0; i < _cardsPtr.size(); i++)
		{
			bool found = false;
			for(size_t j = 0; j < cards.size(); j++)
			{
				if(_cardsPtr[i]->getCardValue() == cards[j])
				{
					found = true;
					break;
				}
			}
			if(found)
			{
				upCard(_cardsPtr[i]);
			}
			else
			{
				downCard(_cardsPtr[i], 0);
			}
			_cardsPtr[i]->setTag(Tag_Untouched);
		}
	}

	void CardListBoard::upCard(PokerCard* card)
	{
		if(card->getName().compare(Name_Down) == 0)
		{					
			card->setName(Name_Up);
			card->stopAllActions();
			card->runAction(MoveTo::create(0.2f, Vec2(card->getPositionX(), UP_OFFSET)));
		}
	}

	void CardListBoard::resizeCardList()
	{
		if(_cardsPtr.size() <= 0) return;
	
		Size cardSize = _cardsPtr[0]->getContentSize() * _cardsPtr[0]->getScale();

		float cardInterval = Other_Card_Interval;
		if(_isMySelf)
		{
			//70 * 94
			float maxLength = cardSize.width + (20 - 1) * Self_Card_Min_Interval;
			cardInterval = maxLength / _cardsPtr.size();
			if(cardInterval > Self_Card_Max_Interval)
			{
				cardInterval = Self_Card_Max_Interval;
			}
		}

		float width  = cardSize.width + (_cardsPtr.size() - 1) * cardInterval;
		float height = cardSize.height;
	
		this->setContentSize(Size(width, height));

		for(size_t i = 0; i < _cardsPtr.size(); i++)
		{
			_cardsPtr[i]->setAnchorPoint(Vec2(0,0));
			_cardsPtr[i]->setPosition(cardInterval * i, _cardsPtr[i]->getPositionY());
			_cardsPtr[i]->setLocalZOrder(Zorder_Card_Began + i);
		}

		if(!_isMySelf)
		{
			downCards();
		}
	}


	CardListBoard::CardListBoard()
		: _callfunc(nullptr)
		, _seatNo(INVALID_DESKSTATION)
		, _isMySelf(true)
	{
	
	}

	CardListBoard::~CardListBoard()
	{

	}
}
