#include "WestMahjongCardPool.h"
#include "GameManager.h"

namespace XZMJ
{


	WestMahjongCardPool::WestMahjongCardPool(void)
	{
	}


	WestMahjongCardPool::~WestMahjongCardPool(void)
	{
	}

	bool WestMahjongCardPool::init(INT count)
	{
		if (! MahjongCardPool::init())
		{
			return false;
		}
		_sumCount = count;
		_dir = sitDir::WEST_DIR;
		_cardSize = Sprite::createWithSpriteFrameName("liYou.png")->getContentSize();
		auto sp = Sprite::createWithSpriteFrameName("DaoBeiEW.png");
		auto origin = GameManager::getInstance()->getNewOrigin();
		setStartSortPos(origin + COCOS_NODE(Layout, "handPanel2")->getPosition() + Vec2(sp->getContentSize().width/2.0f, 
			COCOS_NODE(Layout, "handPanel2")->getContentSize().height-sp->getContentSize().height/2));
		_sortIntervel = sp->getContentSize().height *0.7;
		return true;
	}

	void WestMahjongCardPool::setHandCardPos(Card* noCard)
	{
		if (_listCard.size()==0) { return;}
		auto zorder = 100;
		auto cardInterval =(*_listCard.begin())->getCardSize().height * 5.0/13.0;
		auto origin = GameManager::getInstance()->getNewOrigin();
		auto startPos = origin + COCOS_NODE(Layout, "handPanel2")->getPosition() + Vec2((*_listCard.begin())->getCardSize().width/2.0f, 
			COCOS_NODE(Layout, "handPanel2")->getContentSize().height-(*_listCard.begin())->getCardSize().height/2);
		startPos.y -= _sortIntervel*_leftCount;
		auto count = 0;
		for (auto v : _listCard)
		{
			v->setCardPos(startPos - Vec2(0, count * cardInterval));
			v->setCardZorder(++zorder);
			++count;
		}
	}

}