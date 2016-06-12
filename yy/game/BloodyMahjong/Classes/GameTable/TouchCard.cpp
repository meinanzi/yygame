#include "TouchCard.h"

namespace XZMJ
{


	TouchCard::TouchCard(void)
	{
		_runSp = Sprite::createWithSpriteFrameName("pengAction1.png");
		this->addChild(_runSp);
		_runSp->setVisible(false);
	}


	TouchCard::~TouchCard(void)
	{
	}

	void TouchCard::run()
	{
		_pool->addSomeOutCards(3, _cardNumber);   // 增加倒牌

		// 更新手牌
		_pool->afterAction(2);

		// 播放动画
		uiAction("pengAction");
	}

}