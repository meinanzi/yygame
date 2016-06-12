#include "NorthMahjongCardPool.h"
#include "GameManager.h"

namespace XZMJ
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
		_sumCount = count;
		_dir = sitDir::NORTH_DIR;
		_cardSize = Sprite::createWithSpriteFrameName("liNorthxiao.png")->getContentSize();
		auto sp = Sprite::createWithSpriteFrameName("DaoBeiNS.png");
		auto origin = GameManager::getInstance()->getNewOrigin();
		setStartSortPos(origin + COCOS_NODE(Layout, "handPanel3")->getPosition() + 
			Vec2(COCOS_NODE(Layout, "handPanel3")->getContentSize().width - sp->getContentSize().width/2.0f, COCOS_NODE(Layout, "handPanel3")->getContentSize().height/2));
		_sortIntervel = sp->getContentSize().width;
		return true;
	}

	void NorthMahjongCardPool::setHandCardPos(Card* noCard)
	{
		if (_listCard.size()==0) { return;}
		auto cardInterval =(*_listCard.begin())->getCardSize().width;
		auto startPos = _startSortPos;
		startPos.x -= cardInterval * _leftCount;
		auto count = 0;
		for (auto v : _listCard)
		{
			v->setCardPos(startPos - Vec2(count * cardInterval, 0));
			++count;
		}
	}

}