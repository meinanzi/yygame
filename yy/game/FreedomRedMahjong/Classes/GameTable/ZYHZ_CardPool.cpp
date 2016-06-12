#include "ZYHZ_CardPool.h"
#include "ZYHZ_PoolAction.h"
#include "ZYHZ_GameManager.h"

namespace ZYHZ
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
		sortCard();
		setHandCardPos();
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
		setCanOper(_dir == sitDir::SOUTH_DIR);
		auto actionInterval = _actionCardIntervel;
		
		// 增加桌面显示
		_actionNumber = number;
		if (hideLastOutCard)  // 隐藏出的牌
		{
			auto &card = GameManager::getInstance()->getLastestOutCard();
			if (card != nullptr)
			{
				card->setCardVisible(false);
				GameManager::getInstance()->setMinusOutCount(card->getCardOwner());
			}
		}
		
		for (auto i = _leftCount; i < _leftCount+count; i++)
		{
			Card * card = nullptr;

			if (count == 4 || count == 3)
			{
				if ((i == _leftCount + count - 1) && count == 4)
				{
					Vec2 addPosTouch;
					switch (_dir)
					{
					case ZYHZ::SOUTH_DIR:
						card = GameManager::getInstance()->getZhengDaoCard(number);
						addPosTouch = Vec2(0, _actionCardIntervel/2.5f);
						card->setCardPos(_startSortPos -(4-_leftCount / 3)*_actionCardMoveV + Vec2((i-2) * actionInterval, 0.0f) + _actionCardMoveT + addPosTouch);
						break;
					case ZYHZ::EAST_DIR:
						card = GameManager::getInstance()->getXiaoDaoCard(_dir, number);
						addPosTouch = Vec2(0, _actionCardIntervel/1.9f);
						card->setCardPos(_startSortPos -(4-_leftCount / 3)*_actionCardMoveV + Vec2(0.0f, (i-2) * actionInterval) + _actionCardMoveT + addPosTouch);
						break;
					case ZYHZ::NORTH_DIR:
						card = GameManager::getInstance()->getXiaoDaoCard(_dir, number);
						addPosTouch = Vec2(0, _actionCardIntervel/2.8f);
						card->setCardPos(_startSortPos -(4-_leftCount / 3)*_actionCardMoveV + Vec2(-(i-2) * actionInterval, 0.0f) + _actionCardMoveT + addPosTouch);
						break;
					case ZYHZ::WEST_DIR:
						card = GameManager::getInstance()->getXiaoDaoCard(_dir, number);
						addPosTouch = Vec2(0, _actionCardIntervel/1.9f);
						card->setCardPos(_startSortPos -(4-_leftCount / 3)*_actionCardMoveV + Vec2(0.0f, -(i-2) * actionInterval) + _actionCardMoveT + addPosTouch);
						break;
					case ZYHZ::MID_DIR:
						break;
					default:
						break;
					}

					card->setCardZorder(201);
					_vecKongCard.push_back(card);
				}
				else
				{
					auto zorder = 200;
					if (count == 4 && !isMingGang)
					{
						switch (_dir)
						{
						case ZYHZ::SOUTH_DIR:
							card = MahjongCard::create(mahjongCreateType::HU_BEI_DAO, sitDir::SOUTH_DIR);
							break;
						case ZYHZ::EAST_DIR:
							card = MahjongCard::create(mahjongCreateType::DI_BEI_HENG, sitDir::EAST_DIR);
							break;
						case ZYHZ::NORTH_DIR:
							card = MahjongCard::create(mahjongCreateType::DI_BEI_SHU, sitDir::EAST_DIR);
							break;
						case ZYHZ::WEST_DIR:
							card = MahjongCard::create(mahjongCreateType::DI_BEI_HENG, sitDir::EAST_DIR);
							break;
						case ZYHZ::MID_DIR:
							break;
						default:
							break;
						}

						_vecTmpKongCard.push_back(card);
					}
					else
					{
						switch (_dir)
						{
						case ZYHZ::SOUTH_DIR:
							card = GameManager::getInstance()->getZhengDaoCard(number);
							break;
						case ZYHZ::EAST_DIR:
							card = GameManager::getInstance()->getXiaoDaoCard(_dir, number);
							break;
						case ZYHZ::NORTH_DIR:
							card = GameManager::getInstance()->getXiaoDaoCard(_dir, number);
							break;
						case ZYHZ::WEST_DIR:
							card = GameManager::getInstance()->getXiaoDaoCard(_dir, number);
							break;
						case ZYHZ::MID_DIR:
							break;
						default:
							break;
						}
					}
					switch (_dir)
					{
					case ZYHZ::SOUTH_DIR:
						card->setCardPos(_startSortPos -(4-_leftCount / 3)*_actionCardMoveV + Vec2((i) * actionInterval, 0.0f) + _actionCardMoveT);
						break;
					case ZYHZ::EAST_DIR:
						zorder = zorder-i-_leftCount;
						card->setCardPos(_startSortPos -(4-_leftCount / 3)*_actionCardMoveV + Vec2(0.0f, (i) * actionInterval) + _actionCardMoveT);
						break;
					case ZYHZ::NORTH_DIR:
						card->setCardPos(_startSortPos -(4-_leftCount / 3)*_actionCardMoveV + Vec2(-(i) * actionInterval, 0.0f) + _actionCardMoveT);
						break;
					case ZYHZ::WEST_DIR:
						card->setCardPos(_startSortPos -(4-_leftCount / 3)*_actionCardMoveV + Vec2(0.0f, -(i) * actionInterval) + _actionCardMoveT);
						break;
					case ZYHZ::MID_DIR:
						break;
					default:
						break;
					}

					card->setCardZorder(zorder);

				}
			}

			if ((count == 4 && i == _leftCount + count - 2) || (count == 3 && i == _leftCount + count - 1))
			{
				_rbeginActionCard = card;
				assert(_rbeginActionCard->getCardPos().x > 0 && _rbeginActionCard->getCardPos().y > 0);
			}


			if (count == 1)   // 补杠
			{
				Vec2 addPosTouch;
				switch (_dir)
				{
				case ZYHZ::SOUTH_DIR:
					card = GameManager::getInstance()->getZhengDaoCard(number);
					card->setCardPos(_startSortPos -(4-_leftCount / 3)*_actionCardMoveV + Vec2((i-2) * actionInterval, 0.0f) + _actionCardMoveT);
					addPosTouch = Vec2(0, _actionCardIntervel/2.5f);
					break;
				case ZYHZ::EAST_DIR:
					card = GameManager::getInstance()->getXiaoDaoCard(_dir, number);
					card->setCardPos(_startSortPos -(4-_leftCount / 3)*_actionCardMoveV + Vec2(0.0f, (i-2) * actionInterval) + _actionCardMoveT);
					addPosTouch = Vec2(0, _actionCardIntervel/1.9f);
					break;
				case ZYHZ::NORTH_DIR:
					card = GameManager::getInstance()->getXiaoDaoCard(_dir, number);
					card->setCardPos(_startSortPos -(4-_leftCount / 3)*_actionCardMoveV + Vec2(-(i-2) * actionInterval, 0.0f) + _actionCardMoveT);
					addPosTouch = Vec2(0, _actionCardIntervel/2.5f);
					break;
				case ZYHZ::WEST_DIR:
					card = GameManager::getInstance()->getXiaoDaoCard(_dir, number);
					card->setCardPos(_startSortPos -(4-_leftCount / 3)*_actionCardMoveV + Vec2(0.0f, -(i-2) * actionInterval) + _actionCardMoveT);
					addPosTouch = Vec2(0, _actionCardIntervel/1.9f);
					break;
				case ZYHZ::MID_DIR:
					break;
				default:
					break;
				}

				auto posTouchCard = std::find_if(_vecTouchCard.begin(), _vecTouchCard.end(), [&](Card * iCard){
					return (INT(iCard->getCardColor()) * 10 + iCard->getCardNumber() == number);
				});
				assert(posTouchCard != _vecTouchCard.end());
				card->setCardPos((*posTouchCard)->getCardPos() + addPosTouch);
				_vecTouchCard.erase(posTouchCard);
				card->setCardZorder(201);
				//_vecTmpKongCard.push_back(card);
				_vecKongCard.push_back(card);

			}
			if (card->getParent() == nullptr)
			{
				this->addChild(card);
			}
			// 填充碰牌的容器
			if (count == 3 && i == _leftCount+ 1)
			{
				_vecTouchCard.push_back(card);
			}
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
		//GameManager::getInstance()->hasHu(_dir, isZimo, isQiangGang);
		/*this->runAction(
			Sequence::createWithTwoActions(DelayTime::create(4.0f), CallFunc::create(CC_CALLBACK_0(Node::setVisible, COCOS_NODE(Sprite, "hubg"), false))));*/
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
				v->getCardSprite()->removeFromParent();
				v->removeFromParent();
			}
		}
		_vecTmpKongCard.clear();
		reSetData();
	}


	void CardPool::reSetData()
	{
		_poolAction = nullptr;
		_rbeginActionCard = nullptr;
		_sendCardCount = 0;
		_hasHuCard = false;
		_leftCount = 0;
	
		_needCheckHu = true;
		_needCheckPeng = true;
	}

}