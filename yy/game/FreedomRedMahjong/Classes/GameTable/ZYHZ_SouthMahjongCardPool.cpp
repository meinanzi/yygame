#include "ZYHZ_SouthMahjongCardPool.h"
#include "ZYHZ_GameManager.h"
#include <algorithm>

namespace ZYHZ
{

	SouthMahjongCardPool::SouthMahjongCardPool(void)
	{
	}


	SouthMahjongCardPool::~SouthMahjongCardPool(void)
	{
	}

	bool SouthMahjongCardPool::init(INT count)
	{
		if (! MahjongCardPool::init())
		{
			return false;
		}
		_actionCardMoveV = Vec2(5.0f, 0.0f);	    // 动作牌偏移量 水平
		_actionCardMoveT = Vec2(0.0f, -10.0f);		// 动作牌偏移量 竖直
		_hasSelectOutCard = false;
		_sumCount = count;
		_dir = sitDir::SOUTH_DIR;
		auto card = MahjongCard::create(mahjongCreateType::ZHENG_LI, sitDir::SOUTH_DIR, 1);
		_cardSize = card->getCardSize();
		card->getCardSprite()->removeFromParent();
		card->removeFromParent();
		auto origin = GameManager::getInstance()->getNewOrigin();
		_startSortPos = origin + COCOS_NODE(Layout, "handPanel1")->getPosition() + Vec2(_cardSize.width/2.0f, COCOS_NODE(Layout, "handPanel1")->getContentSize().height/2);
		_sortIntervel = _cardSize.width;

		card = MahjongCard::create(mahjongCreateType::ZHENG_DAO, sitDir::SOUTH_DIR, 1);
		_actionCardIntervel = card->getCardSize().width;
		card->getCardSprite()->removeFromParent();
		card->removeFromParent();

		card = MahjongCard::create(mahjongCreateType::DI_ZHENG_SOUTH, sitDir::SOUTH_DIR, 1);
		_outCardInterval = card->getCardSize().width;
		card->getCardSprite()->removeFromParent();
		card->removeFromParent();

		_willRemoveCard = nullptr;
		return true;
	}

	void SouthMahjongCardPool::setHandCardPos(Card* noCard)
	{
		if (_listCard.size()==0) { return; }
		auto startPos = _startSortPos;
		if (_rbeginActionCard != nullptr)
		{
			startPos = _rbeginActionCard->getCardPos() + Vec2(15, 0) + Vec2(_sortIntervel, 0.0f) - _actionCardMoveT + _actionCardMoveV;
		}
		_currSortStartPos = startPos;
		auto count = 0;
		auto outCount = 0;
		for (auto v : _listCard)
		{
			if (noCard != nullptr)
			{
				if (outCount == 0 && v->getCardColor() == noCard->getCardColor() && v->getCardNumber() == noCard->getCardNumber())
				{
					outCount = 1;
					continue;
				}
			}
			v->setCardPos(startPos + Vec2(count * _sortIntervel, 0));
			++count;
		}
		
	}

	void SouthMahjongCardPool::sortCard()
	{
		std::vector<Card *> vecHongZhong;
		_listCard.remove_if([&](Card* card){
			if (card->getCardSumNumber() == CMjEnum::MJ_TYPE_ZHONG)
			{
				vecHongZhong.push_back(card);
				return true;
			}
			return false;
		});
		function<bool(Card*, Card*)> funSort;
		funSort = [](Card* cardA, Card* cardB){
			if (INT(cardA->getCardColor()) < INT(cardB->getCardColor()))
			{
				return true;
			}
			else if (INT(cardA->getCardColor()) == INT(cardB->getCardColor()))
			{
				if (cardA->getCardNumber() < cardB->getCardNumber())
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		};
		_listCard.sort(funSort);
		for (auto &v : vecHongZhong)
		{
			_listCard.push_front(v);
		}
		setHandCardPos();	 // 填放位置
	}

	void SouthMahjongCardPool::removeCardMsg(Card* card)
	{
		auto color = card->getCardColor();
		if (!_hasSelectOutCard && color == mahjongColor(GameManager::getInstance()->getQueMen()) || hasNoQueColor())       // 是否有缺门
		{
			//THING_OUT_PAI
			tagOutPaiEx msg;
			msg.Clear();
			msg.byUser = RoomLogic()->loginResult.pUserInfoStruct.bDeskStation;
			msg.byPs = INT(card->getCardColor()) * 10 + card->getCardNumber();
			RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, THING_OUT_PAI, &msg, sizeof(msg));

			_willRemoveCard = card;
			_outToDeskPos = card->getCardPos();
			_hasSelectOutCard = true;
		}
	}

	void SouthMahjongCardPool::removeCard()
	{
		if (_willRemoveCard != nullptr)
		{
			setCanOper(false);
			_willRemoveCard->setCardVisible(false);
			_willRemoveCard->setCardEnableTouch(false);
			_willRemoveCard->setPosition(Vec2(-9999.0f,-9999.0f));
			_listCard.remove(_willRemoveCard);
			_willRemoveCard = nullptr;
			_hasSelectOutCard = false;
			sortCard();
			setHandCardPos();
		}
	}

	void SouthMahjongCardPool::setCanOper(bool bs)
	{
		for (auto v : _listCard)
		{
			v->setCardEnableTouch(bs);
		}
	}

	Card* SouthMahjongCardPool::getAutoSendCard()
	{
		// 1. 如果有缺门的牌，从缺门的牌取
		// 2. 取剩余的花色
		sortCard();
		setHandCardPos();
		auto cardIter = std::find_if(_listCard.begin(), _listCard.end(), [&](Card * card){
			return card->getCardColor() != mahjongColor::FENG; });
		if (cardIter == _listCard.end())
		{
			cardIter = _listCard.begin();
		}
		auto cardReturn = *cardIter;
		_willRemoveCard = cardReturn;
		return cardReturn;
	}

	bool SouthMahjongCardPool::hasNoQueColor()
	{
		for (auto v : _listCard)
		{
			if (v->getCardColor() == mahjongColor(GameManager::getInstance()->getQueMen()))
			{
				return false;
			}
		}
		return true;
	}

	bool SouthMahjongCardPool::checkTouchCard(Card* card)
	{
		auto count = std::count_if(_listCard.begin(), _listCard.end(), [&](Card* cardTmp){
			return (INT(card->getCardColor()) != GameManager::getInstance()->getQueMen()
				&& card->getCardColor() == cardTmp->getCardColor() && 
				card->getCardNumber() == cardTmp->getCardNumber());
		});
		auto br = (count >= 2);

		COCOS_NODE(Button, "peng")->setEnabled(br);
		COCOS_NODE(Button, "peng")->setBright(br);

		if (br)
		{
			canBlock();
		}
		return br;
	}

	bool SouthMahjongCardPool::checkMeldedKong(Card* card)
	{
		auto count = std::count_if(_listCard.begin(), _listCard.end(), [&](Card* cardTmp){
			return (INT(card->getCardColor()) != GameManager::getInstance()->getQueMen()
				&& card->getCardColor() == cardTmp->getCardColor() && 
				card->getCardNumber() == cardTmp->getCardNumber());
		});
		auto br = (count == 3);

		COCOS_NODE(Button, "gang")->setEnabled(br);
		COCOS_NODE(Button, "gang")->setBright(br);

		if (br)
		{
			canBlock();
		}
		return br;
	}

	bool SouthMahjongCardPool::checkConcealedKong()
	{
		auto quemen = GameManager::getInstance()->getQueMen();

		for (auto iter = _listCard.begin(); iter != _listCard.end(); iter++)
		{
			auto count = std::count_if(iter, _listCard.end(), [&](Card * card){
				if ((card->getCardColor()  == (*iter)->getCardColor()) && (card->getCardNumber()== (*iter)->getCardNumber()))
				{
					return true;
				}
				return false;
			});
			if (count == 4)
			{
				auto number =	_actionNumber = INT((*iter)->getCardColor()) * 10 + (*iter)->getCardNumber();
				GameManager::getInstance()->setConcealedNumber(number);
				COCOS_NODE(Button, "gang")->setEnabled(true);
				COCOS_NODE(Button, "gang")->setBright(true);
				canBlock();
				GameManager::getInstance()->setKongType(ACTION_AN_GANG);
				sendBlockMsg();
				return true;
			}
		}
		return false;
	}

	bool SouthMahjongCardPool::checkTouchKong(Card* card)
	{
		auto bs =  find_if(_vecTouchCard.begin(), _vecTouchCard.end(), [&](Card* icard){ 
			return (card->getCardColor() == icard->getCardColor() && card->getCardNumber() == icard->getCardNumber() ); }
		)!= _vecTouchCard.end();
		if (bs)
		{
			auto number = _actionNumber = INT(card->getCardColor()) * 10 + card->getCardNumber();
			GameManager::getInstance()->setConcealedNumber(number);
			COCOS_NODE(Button, "gang")->setEnabled(true);
			COCOS_NODE(Button, "gang")->setBright(true);
			canBlock();
			GameManager::getInstance()->setKongType(ACTION_BU_GANG);
			sendBlockMsg();
		}
		return bs;
	}

	void SouthMahjongCardPool::afterAction(const INT& removeCount)
	{
		int count = 0;
		auto number = _lastActionCardNumber;
		_listCard.remove_if([&](Card* card){
			if (INT(card->getCardColor()) *10 + card->getCardNumber() == number && count < removeCount)
			{
				card->setCardVisible(false);
				card->setCardPos(Vec2(-1000, -1000));
				++count;
				return true;
			}
			return false;
		});

		sortCard();
		setHandCardPos();
		COCOS_NODE(Sprite, "dikuang")->setVisible(false);
		GameManager::getInstance()->startTimer(sitDir::SOUTH_DIR, timerType::WAIT_OUT);
	}

	void SouthMahjongCardPool::afterCatchCheckSomething(Card* card)
	{
		if (_hasHuCard || GameManager::getInstance()->isNoCardCanCatch())
		{
			return;
		}
		COCOS_NODE(Sprite, "dikuang")->setVisible(false);
		// 暗杠检测
		checkConcealedKong();
		// 检测补杠
		checkTouchKong(card);

		// 检测自摸
		HUPAI_TYPE_EX typeBase;
		HUPAI_TYPE_EX typeAdd;
		bool hasAdd;
		if (canHu(typeBase, typeAdd, hasAdd))
		{
			COCOS_NODE(Button, "hu")->setEnabled(true);
			COCOS_NODE(Button, "hu")->setBright(true);
			canBlock();
			tagHuPaiEx& hu = GameManager::getInstance()->getHuMsg();
			hu.Clear();
			hu.bZimo = true;
			hu.byUser = RoomLogic()->loginResult.pUserInfoStruct.bDeskStation;
			hu.bHaveHu[hu.byUser] = true;
			hu.byPs = card->getCardSumNumber();
			sendBlockMsg();
		}
	}

	void SouthMahjongCardPool::afterOutCheckSomething(Card* card)
	{
		if (_hasHuCard || GameManager::getInstance()->isNoCardCanCatch())
		{
			return;
		}
		if (_needCheckPeng)
		{
			if (checkTouchCard(card))  // 检查可碰
			{
				sendBlockMsg();
				_checkedCanPeng = true;
			}
			else
			{
				_checkedCanPeng = false;
			}
		}
		if (checkMeldedKong(card))  // 明杠
		{
			sendBlockMsg();
			GameManager::getInstance()->setKongType(ACTION_MING_GANG);
		}

		if (_needCheckHu)
		{
			// 检测胡牌
			HUPAI_TYPE_EX typeBase;
			HUPAI_TYPE_EX typeAdd;
			bool hasAdd;
			if (canHu(typeBase, typeAdd, hasAdd, card))
			{
				COCOS_NODE(Button, "hu")->setEnabled(true);
				COCOS_NODE(Button, "hu")->setBright(true);
				canBlock();
				GameManager::getInstance()->startTimer(sitDir::SOUTH_DIR, timerType::WAIT_BLOCK);
				tagHuPaiEx& hu = GameManager::getInstance()->getHuMsg();
				hu.Clear();
				hu.bZimo = false;
				hu.byDianPao = GTLogic()->getUserStation(card->getCardOwner());
				sendBlockMsg();
				_checkedCanHu = true;
			}
			else
			{
				_checkedCanHu = false;
			}
		}
	}

	void SouthMahjongCardPool::sortListCard(std::list<Card *>& list)
	{
		function<bool(Card*, Card*)> funSort;
		funSort = [](Card* cardA, Card* cardB){
			if (INT(cardA->getCardColor()) < INT(cardB->getCardColor()))
			{
				return true;
			}
			else if (INT(cardA->getCardColor()) == INT(cardB->getCardColor()))
			{
				if (cardA->getCardNumber() < cardB->getCardNumber())
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		};
		list.sort(funSort);
	}

	void SouthMahjongCardPool::sendBlockMsg()
	{
		tagCPGNotifyEx msg;
		msg.Clear();
		msg.byUser = RoomLogic()->loginResult.pUserInfoStruct.bDeskStation;
		RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, THING_CPG_NOTIFY, &msg, sizeof(msg));
	}

	void SouthMahjongCardPool::canBlock()
	{
		COCOS_NODE(Sprite, "dikuang")->setVisible(true);
		GameManager::getInstance()->startTimer(sitDir::SOUTH_DIR, timerType::WAIT_BLOCK);
	}

	bool SouthMahjongCardPool::isShunZi(std::list<INT>& listNum)
	{
		if (listNum.size() % 3 != 0)  // 张数为3的倍数
		{
			return false;
		}
		auto tmpList = listNum;
		while (tmpList.size() > 0)
		{
			auto first = *tmpList.begin();
			auto c1Num = count(tmpList.begin(), tmpList.end(), first+1);
			auto c2Num = count(tmpList.begin(), tmpList.end(), first+2);
			if (c1Num < 1 || c2Num < 1)
			{
				return false;
			}
			int ccCount = 0;
			tmpList.remove_if([&](int num){
				if (ccCount < 1 && num == first)
				{
					ccCount = 1;
					return true;
				}
				return false;
			});
			ccCount = 0;
			tmpList.remove_if([&](int num){
				if (ccCount < 1 && num == (first+1))
				{
					ccCount = 1;
					return true;
				}
				return false;
			});
			ccCount = 0;
			tmpList.remove_if([&](int num){
				if (ccCount < 1 && num == (first+2))
				{
					ccCount = 1;
					return true;
				}
				return false;
			});
		}
		return true;
	}

	void SouthMahjongCardPool::calcCardCount(std::list<Card *>& listCard,std::list<INT>& listCardNum, std::vector<INT>& vecCard, std::vector<INT>& vCount)
	{
		sortListCard(listCard);

		// 填充点数
		for (auto &v : listCard)
		{
			listCardNum.push_back(v->getCardSumNumber());
			vecCard.push_back(v->getCardSumNumber());
		}

		// 非重复牌
		auto iter = unique(vecCard.begin(), vecCard.end());
		vecCard.erase(iter, vecCard.end());
		// 统计重复数
		for (auto &var : vecCard)
		{
			auto num = count(listCardNum.begin(), listCardNum.end(), var);
			vCount.push_back(num);
		}
	}

	void SouthMahjongCardPool::calCOCOS_NODEumCount(std::list<INT>& listCardNum,std::vector<INT>& vecCard, std::vector<INT>& vCount)
	{
		vCount.clear();
		vecCard.clear();
		listCardNum.sort();
		auto listTmp = listCardNum;
		listTmp.unique();
		vecCard.assign(listTmp.begin(), listTmp.end());
		for (auto &v : vecCard)
		{
			auto numCount = count(listCardNum.begin(), listCardNum.end(), v);
			vCount.push_back(numCount);
		}
	}

	bool SouthMahjongCardPool::canTing()
	{
		return true;
	}

	bool SouthMahjongCardPool::canHu(HUPAI_TYPE_EX& typeBase, HUPAI_TYPE_EX& typeAdd, bool& hasAdd, Card* card)
	{
		// 过了不能胡

		// add
		if (isTianHu(card))   // 天胡
		{
			typeAdd = HUPAI_TYPE_EX::HUPAI_TYPE_TIAN_HU;
			hasAdd = true;
		}
		else if (isDiHu(card))   // 地胡
		{
			typeAdd = HUPAI_TYPE_EX::HUPAI_TYPE_DI_HU;
			hasAdd = true;
		}

		if (isPingHu(card))             // 平胡
		{
			typeBase = HUPAI_TYPE_EX::HUPAI_HU_PAI;
			return true;
		}
		return false;
	}

	void SouthMahjongCardPool::checkWillRemoveCard(INT number)
	{
		if (_willRemoveCard == nullptr || _willRemoveCard->getCardSumNumber() != number)
		{
			for (auto &v : _listCard)
			{
				if (v->getCardSumNumber() == number)
				{
					_willRemoveCard = v;
					return;
				}
			}
		}
	}

	bool SouthMahjongCardPool::isPingHu(Card* card)
	{
		auto listTmp = _listCard;
		if (card != nullptr)
		listTmp.push_back(card);
		list<INT> listCardNum;
		vector<INT> vCardUnique;
		vector<INT> vCount;
		calcCardCount(listTmp , listCardNum, vCardUnique, vCount);

		// if 无对子 ，则返回错
		if (count(vCount.begin(), vCount.end(), 1) == vCount.size())
		{
			return false;
		}
		else
		{
			// 穷举对子
			for (size_t i = 0; i < vCount.size(); i++)
			{
				list<INT> checkSZ(listCardNum.begin(), listCardNum.end());
				if (vCount.at(i) >= 2)
				{
					int ccCount = 0;
					checkSZ.remove_if([&](int num){
						if (ccCount < 2 && num == vCardUnique.at(i))
						{
							++ccCount;
							return true;
						}
						return false;
					});

					// 无牌算胡
					if (checkSZ.size() == 0)
					{
						return true;
					}

					calCOCOS_NODEumCount(checkSZ, vCardUnique, vCount);

					// 删除刻字
					for (size_t i= 0; i < vCount.size(); i++)
					{
						if (vCount.at(i) >= 3)
						{
							int maxCount = 0;
							checkSZ.remove_if([&](INT num){
								if (maxCount < 3 && num == vCardUnique.at(i))
								{
									maxCount++;
									return true;
								}
								return false;
							});
						}
					}

					if(isShunZi(checkSZ))
					{
						return true;
					}
				}
			}
			return false;
		}
		return true;
	}

	bool SouthMahjongCardPool::isDuiDuiHu(Card* card)
	{
		auto listTmp = _listCard;
		if (card != nullptr)
		listTmp.push_back(card);
		list<INT> listCardNum;
		vector<INT> vCardUnique;
		vector<INT> vCount;
		calcCardCount(listTmp , listCardNum, vCardUnique, vCount);
		for (auto &v : vCount)
		{
			if (v != 2 || v != 3)
			{
				return false;
			}
		}
		if (count(vCount.begin(), vCount.end(), 2) > 1)
		{
			return false;
		}
		return true;
	}

	bool SouthMahjongCardPool::isQingYiSe(Card* card)
	{
		auto listTmp = _listCard;
		if (card != nullptr)
		listTmp.push_back(card);
		for (auto &v : _vecKongCard)
		{
			listTmp.push_back(v);
		}
		for (auto &v : _vecTouchCard)
		{
			listTmp.push_back(v);
		}
		auto color = (*listTmp.begin())->getCardColor();
		for (auto &v : listTmp)
		{
			if (v->getCardColor() != color)
			{
				return false;
			}
		}
		if (!isPingHu(card) || !isXiaoQiDui(card))
		{
			return false;
		}
		return true;
	}

	bool SouthMahjongCardPool::isDaiJiuYao(Card* card)
	{
		for (auto &v : _vecTouchCard)
		{
			if (v->getCardNumber() != 1 || v->getCardNumber() != 9)
			{
				return false;
			}
		}
		for (auto &v : _vecKongCard)
		{
			if (v->getCardNumber() != 1 || v->getCardNumber() != 9)
			{
				return false;
			}
		}

		auto tmpList = _listCard;
		if (card != nullptr)
		{
			tmpList.push_back(card);
		}

		for (auto &v : tmpList)
		{
			if (v->getCardNumber() != 1 || v->getCardNumber() != 9)
			{
				return false;
			}
		}

		if (!isDuiDuiHu(card))
		{
			return false;
		}

		return true;
	}

	bool SouthMahjongCardPool::isXiaoQiDui(Card* card)
	{
		if (_vecTouchCard.size() > 0 || _vecKongCard.size() > 0)
		{
			return false;
		}

		auto listTmp = _listCard;
		if (card != nullptr)
		listTmp.push_back(card);
		list<INT> listCardNum;
		vector<INT> vCardUnique;
		vector<INT> vCount;
		calcCardCount(listTmp , listCardNum, vCardUnique, vCount);
		for (auto &v : vCount)
		{
			if (v != 2 || v != 4)
			{
				return false;
			}
		}
		return true;
	}

	bool SouthMahjongCardPool::isJiangDui(Card* card)
	{
		for (auto &v : _vecKongCard)
		{
			if (v->getCardNumber() != 2 || v->getCardNumber() != 5 || v->getCardNumber() != 8)
			{
				return false;
			}
		}
		for (auto &v : _vecTouchCard)
		{
			if (v->getCardNumber() != 2 || v->getCardNumber() != 5 || v->getCardNumber() != 8)
			{
				return false;
			}
		}
		auto listTmp = _listCard;
		if (card != nullptr)
		{
			listTmp.push_back(card);	
		}
		for (auto &v : listTmp)
		{
			if (v->getCardNumber() != 2 || v->getCardNumber() != 5 || v->getCardNumber() != 8)
			{
				return false;
			}if (v->getCardNumber() != 2 || v->getCardNumber() != 5 || v->getCardNumber() != 8)
			{
				return false;
			}
		}
		if (!isDuiDuiHu(card))
		{
			return false;
		}
		return true;
	}

	bool SouthMahjongCardPool::isQingDui(Card* card)
	{
		return (isQingYiSe(card) && isDuiDuiHu(card));
	}

	bool SouthMahjongCardPool::isLongQiDui(Card* card)
	{
		if (!isXiaoQiDui(card))
		{
			return false;
		}

		auto listTmp = _listCard;
		if (card != nullptr)
		listTmp.push_back(card);
		list<INT> listCardNum;
		vector<INT> vCardUnique;
		vector<INT> vCount;
		calcCardCount(listTmp , listCardNum, vCardUnique, vCount);
		if (count(vCount.begin(), vCount.end(), 4) == 0)
		{
			return false;
		}

		return true;
	}

	bool SouthMahjongCardPool::isQingQiDui(Card* card)
	{
		return (isQingYiSe(card) && isXiaoQiDui(card));
	}

	bool SouthMahjongCardPool::isQingYaoJiu(Card* card)
	{
		return (isQingYiSe(card) && isDaiJiuYao(card));
	}

	bool SouthMahjongCardPool::isLongQingQiDui(Card* card)
	{
		return (isLongQiDui(card) && isQingYiSe(card));
	}

	bool SouthMahjongCardPool::isYaoJiuQiDui(Card* card)
	{
		return (isDaiJiuYao(card) && isXiaoQiDui(card));
	}

	bool SouthMahjongCardPool::isTianHu(Card* card)
	{
		if (GameManager::getInstance()->getStartDir() == _dir && getOutCardCount() == 0)
		{
			return (isPingHu(card) || isXiaoQiDui(card));
		}
		return false;
	}
	
	bool SouthMahjongCardPool::isDiHu(Card* card)
	{
		if (GameManager::getInstance()->getStartDir() != _dir && getOutCardCount() == 0)
		{
			return (isPingHu(card) || isXiaoQiDui(card));
		}
		return false;
	}

}