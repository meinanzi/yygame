#include "EastMahjongCardPool.h"
#include "GameManager.h"

namespace XZMJ
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
		_cardSize = Sprite::createWithSpriteFrameName("liYou.png")->getContentSize();
		auto sp = Sprite::createWithSpriteFrameName("DaoBeiEW.png");
		auto origin = GameManager::getInstance()->getNewOrigin();
		setStartSortPos(origin + COCOS_NODE(Layout, "handPanel4")->getPosition()
			+ Vec2(sp->getContentSize().width/2.0f, sp->getContentSize().height/2));
		_sortIntervel = sp->getContentSize().height *0.7;
		_sumCount = count;
		_dir = sitDir::EAST_DIR;
		return true;
	}

	void EastMahjongCardPool::setHandCardPos(Card* noCard)
	{
		if (_listCard.size()==0) { return; }
		auto zorder = 100;
		auto cardInterval =(*_listCard.begin())->getCardSize().height * 5.0/13.0;
		auto startPos = _startSortPos;
		startPos.y += _sortIntervel *_leftCount;
		
		auto count = 0;
		for (auto v : _listCard)
		{
			v->setCardPos(startPos + Vec2(0, count * cardInterval));
			v->setCardZorder(--zorder);
			++count;
		}
	}

}