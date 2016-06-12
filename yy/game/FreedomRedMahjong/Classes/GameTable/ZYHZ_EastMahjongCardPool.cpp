#include "ZYHZ_EastMahjongCardPool.h"
#include "ZYHZ_GameManager.h"

namespace ZYHZ
{

	EastMahjongCardPool::EastMahjongCardPool(void)
	{
	}


	EastMahjongCardPool::~EastMahjongCardPool(void)
	{
	}

	bool EastMahjongCardPool::init(INT count)
	{
		if (! MahjongCardPool::init())
		{
			return false;
		}
		_actionCardMoveV = Vec2(0.0f, 5.0f);	    // 动作牌偏移量 水平
		_actionCardMoveT = Vec2(-5.0f, 0.0f);		// 动作牌偏移量 竖直

		auto card = MahjongCard::create(mahjongCreateType::YOU_LI, sitDir::EAST_DIR);
		_cardSize = card->getCardSize();
		card->getCardSprite()->removeFromParent();
		card->removeFromParent();
		auto origin = GameManager::getInstance()->getNewOrigin();
		setStartSortPos(origin + COCOS_NODE(Layout, "handPanel4")->getPosition()
			+ Vec2(_cardSize.width/2.0f, _cardSize.height/2));
		_sortIntervel = _cardSize.height *0.4;

		card = MahjongCard::create(mahjongCreateType::DI_ZHENG_EAST, sitDir::EAST_DIR, 1);
		_outCardInterval = _actionCardIntervel = card->getCardSize().height*0.66;
		card->getCardSprite()->removeFromParent();
		card->removeFromParent();

		_sumCount = count;
		_dir = sitDir::EAST_DIR;
		return true;
	}

	void EastMahjongCardPool::setHandCardPos(Card* noCard)
	{
		if (_listCard.size()==0) { return; }
		auto zorder = 100;
		auto startPos = _startSortPos;
		if (_rbeginActionCard != nullptr)
		{
			startPos = _rbeginActionCard->getCardPos() + Vec2(0.0f, _sortIntervel) - _actionCardMoveT + _actionCardMoveV;
		}
		_currSortStartPos = startPos;
		auto count = 0;
		for (auto v : _listCard)
		{
			v->setCardPos(startPos + Vec2(0, count * _sortIntervel));
			v->setCardZorder(--zorder);
			++count;
		}
	}

}