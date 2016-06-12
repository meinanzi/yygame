#include "CardPool.h"
#include "PoolAction.h"
#include "GameManager.h"

namespace XZMJ
{

	CardPool::CardPool(void)
	{
	}


	CardPool::~CardPool(void)
	{
	}

	void CardPool::addCard(Card* card)
	{
		_listCard.push_back(card);
		card->setCardVisible();
		card->setCardPos(_catchPos);

		 // 设置可操作
		if (_dir == sitDir::SOUTH_DIR)
		{
			setCanOper(); 
		}
	}

	void CardPool::removeCard()
	{
		(*_listCard.rbegin())->setCardVisible(false);
		_listCard.remove(*_listCard.rbegin());
		sortCard();
		setHandCardPos();
	}

	void CardPool::sendSomeCard(std::vector<Card *> vec)
	{
		for (auto v : vec)
		{
			_listCard.push_back(v);
			v->setCardVisible();
		}
		setHandCardPos();
	}

	bool CardPool::init()
	{
		if (!Node::init())
		{
			return false;
		}
		_poolAction = nullptr;
		_dir = sitDir::MID_DIR;
		_sendCardCount = 0;
		_hasHuCard = false;
		_leftCount = 0;
		_sortIntervel = 0.0f;

		_needCheckHu = true; // 检测是否可胡动作
		_needCheckPeng = true; // 可碰
		_checkedCanHu = false;  // 检测到能胡
		_checkedCanPeng = false; // 检测到能碰
		return true;
	}

	Card* CardPool::getAutoSendCard()
	{
		if (_listCard.size()==0) { return nullptr; }
		return *_listCard.rbegin();
	}

	void CardPool::setPoolAction(PoolAction* action)
	{
		_poolAction = action;
	}

	void CardPool::addLeftOutCount(const INT& count)
	{
		_leftCount += count;
	}

	const INT& CardPool::getLeftOutCount()
	{
		return _leftCount;
	}

	const sitDir& CardPool::getSitDir()
	{
		return _dir;
	}

	void CardPool::addSomeOutCards(const INT& count, const INT& number, bool hideLastOutCard, bool isMingGang)
	{
		// 增加桌面显示
		_actionNumber = number;
		if (hideLastOutCard)  // 隐藏出的牌
		{
			auto card = GameManager::getInstance()->getLastestOutCard();
			card->setCardVisible(false);
			GameManager::getInstance()->setMinusOutCount(card->getCardOwner());
		}
		
		for (auto i = _leftCount; i < _leftCount+count; i++)
		{
			Card * card = nullptr;
			
			switch (_dir)
			{
			case XZMJ::sitDir::SOUTH_DIR:
				
				if (i == _leftCount + count - 1)
				{
					card = GameManager::getInstance()->getZhengDaoCard(number);
					if (count == 4)
					{
						_vecKongCard.push_back(card);
					}
				}
				else
				{
					if (!isMingGang)
					{
						card = MahjongCard::create(mahjongCreateType::HU_BEI_DAO, sitDir::SOUTH_DIR);
						this->addChild(card);
						_vecTmpKongCard.push_back(card);
					}
					else
					{
						card = GameManager::getInstance()->getZhengDaoCard(number);
					}
				}
				if (count != 1)  // 非碰杠
				{
					if (count == 4 && i == _leftCount + count - 1)  // 杠
					{
						card->setCardPos(_startSortPos + Vec2(-45 + _leftCount / 3 *12, 12 + _cardSize.height/3.4) + Vec2((i-2) * _sortIntervel, 0));
						card->setCardZorder(201);
					}
					else
					{
						card->setCardPos(_startSortPos + Vec2(-45 + _leftCount / 3 *12, 12) + Vec2(i * _sortIntervel, 0));
						card->setCardZorder(200);
					}
				}
				break;
			case XZMJ::sitDir::EAST_DIR:
				
				if (i == _leftCount + count - 1)
				{
					card = GameManager::getInstance()->getXiaoDaoCard(_dir, number);
					if (count == 4)
					{
						_vecKongCard.push_back(card);
					}
				}
				else
				{
					if (!isMingGang)
					{
						card = MahjongCard::create(mahjongCreateType::DI_BEI_HENG, sitDir::SOUTH_DIR);
						this->addChild(card);
						_vecTmpKongCard.push_back(card);
					}
					else
					{
						card = GameManager::getInstance()->getXiaoDaoCard(_dir, number);
					}
				}
				if (count != 1)  // 非碰杠
				{
					if (count == 4 && i == _leftCount + count - 1)
					{
						card->setCardPos(_startSortPos+ Vec2(-10, -24 + _leftCount / 3 *6 + _cardSize.height/5.5) + Vec2(0, (i-2) * _sortIntervel));
						card->setCardZorder(201);
					}
					else
					{
						card->setCardPos(_startSortPos+ Vec2(-10, -24 + _leftCount / 3 *6) + Vec2(0, i * _sortIntervel));
					}
				}
				break;
			case XZMJ::sitDir::NORTH_DIR:
				if (i == _leftCount + count - 1)
				{
					card = GameManager::getInstance()->getXiaoDaoCard(_dir, number);
					if (count == 4)
					{
						_vecKongCard.push_back(card);
					}
				}
				else
				{
					if (!isMingGang)
					{
						card = MahjongCard::create(mahjongCreateType::DI_BEI_SHU, sitDir::SOUTH_DIR);
						this->addChild(card);
						_vecTmpKongCard.push_back(card);
					}
					else
					{
						card = GameManager::getInstance()->getXiaoDaoCard(_dir, number);
					}
				}
				if (count != 1)  // 非碰杠
				{
					if (count == 4 && i == _leftCount + count - 1)
					{
						card->setCardPos(_startSortPos + Vec2(24 - _leftCount / 3 *6, -15 + _cardSize.height/4.0)+ Vec2(-(i-2) * _sortIntervel, 0));
						card->setCardZorder(201);
					}
					else
					{
						card->setCardPos(_startSortPos + Vec2(24 - _leftCount / 3 *6, -15)+ Vec2(-i * _sortIntervel, 0));
						card->setCardZorder(200);
					}
				}
				break;
			case XZMJ::sitDir::WEST_DIR:
				if (i == _leftCount + count - 1)
				{
					card = GameManager::getInstance()->getXiaoDaoCard(_dir, number);
					if (count == 4)
					{
						_vecKongCard.push_back(card);
					}
				}
				else
				{
					if (!isMingGang)
					{
						card = MahjongCard::create(mahjongCreateType::DI_BEI_HENG, sitDir::SOUTH_DIR);
						this->addChild(card);
						_vecTmpKongCard.push_back(card);
					}
					else
					{
						card = GameManager::getInstance()->getXiaoDaoCard(_dir, number);
					}
				}
				if (count != 1)  // 非碰杠
				{
					if (count == 4 && i == _leftCount + count - 1)
					{
						card->setCardPos(_startSortPos+ Vec2(10, 24 - _leftCount / 3 *6 + _cardSize.height/5.5) + Vec2(0, -(i-2) * _sortIntervel));
						card->setCardZorder(201);
					}
					else
					{
						card->setCardPos(_startSortPos+ Vec2(10, 24 - _leftCount / 3 *6) + Vec2(0, -i * _sortIntervel));
						card->setCardZorder(200);
					}
				}
				break;
			case XZMJ::sitDir::MID_DIR:
				break;
			default:
				break;
			}

			if (count == 1)   // 碰杠
			{
				auto posTouchCard = std::find_if(_vecTouchCard.begin(), _vecTouchCard.end(), [&](Card * iCard){
						return (INT(iCard->getCardColor()) * 10 + iCard->getCardNumber() == number);
					});
					assert(posTouchCard != _vecTouchCard.end());
					card->setCardPos((*posTouchCard)->getCardPos() + Vec2(0, _cardSize.height/5.5));
					card->setCardZorder(201);
					_vecKongCard.push_back(*posTouchCard);
					_vecTouchCard.erase(posTouchCard);
			}

			// 填充碰牌的容器
			if (count == 3 && i == _leftCount+ 1)
			{
				_vecTouchCard.push_back(card);
			}
			//if (count == 4 && i == _leftCount)   // 杠牌
			//{
			//	_vecKongCard.push_back(card);
			//}
		}
		
		// 刷新容器
		auto moveX = std::min<int>(count, 3);
		if (count == 1)
		{
			moveX = 0;
		}
		addLeftOutCount(moveX);   // 牌缩进
	}

	const Vec2& CardPool::getActionPos()
	{
		return COCOS_NODE(Text, StringUtils::format("outBigShow%d", _dir))->getPosition();
	}

	void CardPool::huCard(bool isZimo, bool isQiangGang)
	{
		_hasHuCard = true;
		GameManager::getInstance()->hasHu(_dir, isZimo, isQiangGang);
		this->runAction(
			Sequence::createWithTwoActions(DelayTime::create(4.0f), CallFunc::create(CC_CALLBACK_0(Node::setVisible, COCOS_NODE(Sprite, "hubg"), false))));
	}

	void CardPool::afterAction(const INT& removeCount)
	{
		for (auto i = 0; i < removeCount; i++)
		{
			(*_listCard.rbegin())->setCardVisible(false);
			_listCard.remove(*_listCard.rbegin());
		}
		setHandCardPos();
		COCOS_NODE(Sprite, "dikuang")->setVisible(false);
		GameManager::getInstance()->startTimer(_dir,timerType::WAIT_OUT);
	}

	void CardPool::finishGame()
	{
		// 1. 重置各类数据
		for (auto &v : _listCard)
		{
			v->setCardVisible(false);
		}
		for (auto &v : _listHuCard)
		{
			v->setCardVisible(false);
		}
		_listCard.clear();
		_listHuCard.clear();
		for (auto &v : _vecKongCard)
		{
			v->setCardVisible(false);
		}
		_vecKongCard.clear();
		for (auto &v : _vecTouchCard)
		{
			v->setCardVisible(false);
		}
		_vecTouchCard.clear();

		for (auto &v : _vecTmpKongCard)
		{
			if (v->getCardSprite() != nullptr)
			{
				GameManager::getInstance()->getPR()->removeNode(v->getCardSprite());
			}
			if (v != nullptr)
			{
				v->removeFromParent();
			}
			
		}
		_vecTmpKongCard.clear();
		reSetData();
	}


	void CardPool::reSetData()
	{
		_poolAction = nullptr;

		_sendCardCount = 0;
		_hasHuCard = false;
		_leftCount = 0;
	
		_needCheckHu = true;
		_needCheckPeng = true;
	}

}