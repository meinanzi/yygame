#include "ZYHZ_Card.h"
#include "ZYHZ_GameManager.h"

namespace ZYHZ
{

	Card::Card(void)
	{
		_uiSp = nullptr;
		_type = mahjongCreateType::DI_BEI_HENG;   
		_number = -999;
		_sitNo = sitDir::MID_DIR;
		_color = mahjongColor::TIAO;
		_canTouch = false;
		_isSelect = false;
		_handCardScale = 1.5f;
		_outCardScale = 1.2f;
		_doorCardScale = 1.2f;
		_zhengDaoScale = 1.4f;
	}


	Card::~Card(void)
	{
	}


	bool Card::init(mahjongCreateType type, sitDir sitNo, INT number)
	{
		if (!Node::init())
		{
			return false;
		}
		return true;
	}

	const Size& Card::getCardSize()
	{
		return _size;
	}

	void Card::setCardPos(const Vec2& pos)
	{
		_uiSp->setPosition(pos);
	}

	void Card::setCardZorder(const int zorder)
	{
		_uiSp->setLocalZOrder(zorder);
		_cardZorder = zorder;
	}

	void Card::setCardVisible(const bool& bs)
	{
		_uiSp->setVisible(bs);
	}

	void Card::setCardScale(float scale)
	{
		float scaleXX;
		switch (_type)
		{
		case ZYHZ::ZUO_LI:
		case ZYHZ::YOU_LI:
		case ZYHZ::QIAN_LI:
		case ZYHZ::ZHENG_LI:
			scaleXX = _handCardScale;
			break;
		
		case ZYHZ::DI_BEI_HENG:
		case ZYHZ::DI_BEI_SHU:
		case ZYHZ::HU_BEI_DAO:
			scaleXX = _doorCardScale;
			break;
		case ZYHZ::DI_ZHENG_SOUTH:
		case ZYHZ::DI_ZHENG_NORTH:
		case ZYHZ::DI_ZHENG_EAST:
		case ZYHZ::DI_ZHENG_WEST:
			scaleXX = _outCardScale;
			break;
		case ZYHZ::ZHENG_DAO:
			scaleXX = _zhengDaoScale;
			break;
		}
		_uiSp->setScale(scale*scaleXX);
		_size.width *= scale;
		_size.height *= scale;
	}

	bool Card::isCardVisible()
	{
		return _uiSp->isVisible();
	}

	Sprite* Card::getCardSprite()
	{
		return _uiSp;
	}

	void Card::setCardOwner(const sitDir& dir)
	{
		_sitNo = dir;
	}


	const INT& Card::getCardNumber()
	{
		return _number;
	}
		
	const mahjongColor& Card::getCardColor()
	{
		return _color;
	}

	void Card::setCardColor(Color3B color)
	{
		_uiSp->setColor(color);
	}

	void Card::setCardTouchEvent()
	{
		auto listener =EventListenerTouchOneByOne::create();
		listener->setSwallowTouches(true);
		//listener->onTouchBegan = [&](Touch* t, Event* e)->bool{
		//	if (!_canTouch)            // 是否能触摸
		//	{
		//		return false;
		//	}
		//	if (!_uiSp->getBoundingBox().containsPoint((t->getLocation())))   // 是否牌范围内
		//	{
		//		return false;
		//	}

		//	// 取触摸牌的初始状态
		//	_startPos = _uiSp->getPosition();					// 初始坐标点
		//	_startZorder = _uiSp->getLocalZOrder();		// z值
		//	_startColor = _uiSp->getColor();					// 颜色

		//	// 改变牌状态
		//	_uiSp->setScale(1.4f*_handCardScale);
		//	_uiSp->setLocalZOrder(99999);
		//	auto pool = (SouthMahjongCardPool *)(GameManager::getInstance()->getUserCardPool().at(0));
		//	if (_color != mahjongColor::FENG)   // 是否能打出
		//	{
		//		_uiSp->setColor(Color3B(200, 255, 195));   // 淡绿色
		//	}
		//	else
		//	{
		//		_uiSp->setColor(Color3B(250, 180, 180));   // 淡红色
		//	}
		//	return true;
		//};
		//listener->onTouchMoved = [&](Touch* t, Event* e){
		//	if (_canTouch)
		//	{
		//		_uiSp->setPositionY(_uiSp->getPositionY() + t->getDelta().y);           // 移动选中的牌
		//		auto pool = (SouthMahjongCardPool *)(GameManager::getInstance()->getUserCardPool().at(0));

		//		// 判断出牌条件
		//		float needMoveLength = 60.0f;  // 在y轴向上需要移动的距离
		//		if (abs(_uiSp->getPositionY() - _startPos.y) > needMoveLength
		//			&& (_color != mahjongColor::FENG))							// 达到出牌条件，给服务端发送消息
		//		{
		//			GameManager::getInstance()->getUserCardPool().at(0)->removeCardMsg(this);
		//		}
		//	}
		//};
		//listener->onTouchEnded = [&](Touch* t, Event* e){

		//	// 还原牌的起始状态
		//		if (_uiSp->isVisible())
		//		{
		//			_uiSp->setPosition(_startPos);
		//		}
		//		else
		//		{
		//			_uiSp->setPosition(Vec2(-1000, -1000));
		//		}
		//		_uiSp->setScale(_handCardScale);
		//		_uiSp->setLocalZOrder(_startZorder);
		//		_uiSp->setColor(_startColor);
		//};

		listener->onTouchBegan = [&](Touch* t, Event* e)->bool{
			if (!_canTouch || !_uiSp->isVisible())            // 是否能触摸
			{
				return false;
			}
			else
			{
				auto touchPos = t->getLocation();
				auto box = _uiSp->getBoundingBox();
				box.size = _size;
				if (box.containsPoint(touchPos))   // 是否牌范围内
				{
					// 取手牌
					auto cardVec = GameManager::getInstance()->getUserCardPool().at(0)->getListCard();
					for (auto &card : cardVec)
					{
						if (card != this)
						{
							card->setSelect(false);
							card->setCardColor(Color3B(255, 255, 255));
						}
					}
					if (_isSelect)
					{
						GameManager::getInstance()->getUserCardPool().at(0)->removeCardMsg(this);
					}
					else
					{
						if (_color != mahjongColor::FENG)   // 是否能打出
						{
							_uiSp->setColor(Color3B(200, 255, 195));   // 淡绿色
							_isSelect = true;
						}
						else
						{
							_uiSp->setColor(Color3B(250, 180, 180));   // 淡红色
							_isSelect = false;
						}
					}
					return true;
				}
			}
			return false;
		};

		listener->onTouchEnded = [&](Touch* t, Event* e){};

		listener->onTouchCancelled = [&](Touch* t, Event* e){};
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, _uiSp);
	}

	void Card::setCardEnableTouch(bool touch)
	{
		_canTouch = touch;
	}

	const INT& Card::getCardZorder()
	{
		return _cardZorder;
	}

	float Card::getScaleXX()
	{
		switch (_type)
		{
		case ZYHZ::ZUO_LI:
		case ZYHZ::YOU_LI:
			return _handCardScale - 0.2f;
			break;
		case ZYHZ::QIAN_LI:
			return _handCardScale - 0.1f;
			break;
		case ZYHZ::ZHENG_LI:
			return _handCardScale;
			break;
		
		case ZYHZ::DI_BEI_HENG:
		case ZYHZ::DI_BEI_SHU:
		case ZYHZ::HU_BEI_DAO:
			return _doorCardScale;
			break;
		case ZYHZ::DI_ZHENG_SOUTH:
		case ZYHZ::DI_ZHENG_NORTH:
		case ZYHZ::DI_ZHENG_EAST:
		case ZYHZ::DI_ZHENG_WEST:
			return _outCardScale;
			break;
		case ZYHZ::ZHENG_DAO:
			return _zhengDaoScale;
			break;
		default:
			return 0.0f;
			break;
		}
	}

}