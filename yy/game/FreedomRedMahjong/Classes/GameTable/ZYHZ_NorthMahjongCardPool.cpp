#include "ZYHZ_NorthMahjongCardPool.h"
#include "ZYHZ_GameManager.h"

namespace ZYHZ
{

	NorthMahjongCardPool::NorthMahjongCardPool(void)
	{
	}


	NorthMahjongCardPool::~NorthMahjongCardPool(void)
	{
	}

	bool NorthMahjongCardPool::init(INT count)
	{
		if (! MahjongCardPool::init())
		{
			return false;
		}
		_actionCardMoveV = Vec2(-5.0f, 0.0f);	    // 动作牌偏移量 水平
		_actionCardMoveT = Vec2(0.0f, -5.0f);		// 动作牌偏移量 竖直
		_sumCount = count;
		_dir = sitDir::NORTH_DIR;
		auto card = MahjongCard::create(mahjongCreateType::QIAN_LI, sitDir::NORTH_DIR);
		_cardSize = card->getCardSize();
		card->getCardSprite()->removeFromParent();
		card->removeFromParent();

		auto origin = GameManager::getInstance()->getNewOrigin();
		setStartSortPos(origin + COCOS_NODE(Layout, "handPanel3")->getPosition() + 
			Vec2(COCOS_NODE(Layout, "handPanel3")->getContentSize().width - _cardSize.width/2.0f, COCOS_NODE(Layout, "handPanel3")->getContentSize().height/2));
		_sortIntervel = _cardSize.width;

		card = MahjongCard::create(mahjongCreateType::DI_ZHENG_NORTH, sitDir::NORTH_DIR, 1);
		_outCardInterval = _actionCardIntervel = card->getCardSize().width;
		card->getCardSprite()->removeFromParent();
		card->removeFromParent();
		return true;
	}

	void NorthMahjongCardPool::setHandCardPos(Card* noCard)
	{
		if (_listCard.size()==0) { return;}
		auto startPos = _startSortPos;
		if (_rbeginActionCard != nullptr)
		{
			startPos = _rbeginActionCard->getCardPos() + Vec2(-_sortIntervel, 0.0f) - _actionCardMoveT + _actionCardMoveV;
		}
		_currSortStartPos = startPos;
		auto count = 0;
		for (auto v : _listCard)
		{
			v->setCardPos(startPos - Vec2(count * _sortIntervel, 0));
			++count;
		}
	}

}