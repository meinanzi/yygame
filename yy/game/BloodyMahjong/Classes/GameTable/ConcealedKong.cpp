#include "ConcealedKong.h"

namespace XZMJ
{

	ConcealedKong::ConcealedKong(void)
	{
		_runSp = Sprite::createWithSpriteFrameName("gangAction1.png");
		this->addChild(_runSp);
		_runSp->setVisible(false);
	}


	ConcealedKong::~ConcealedKong(void)
	{
	}

	void ConcealedKong::run()
	{
		_pool->addSomeOutCards(4, _cardNumber, false, false);   // 增加倒牌

		// 更新手牌
		_pool->afterAction(4);

		// 播放动画
		uiAction("gangAction");
	}
}