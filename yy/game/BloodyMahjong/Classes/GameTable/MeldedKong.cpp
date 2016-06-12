#include "MeldedKong.h"

namespace XZMJ
{

	MeldedKong::MeldedKong(void)
	{
		_runSp = Sprite::createWithSpriteFrameName("gangAction1.png");
		this->addChild(_runSp);
		_runSp->setVisible(false);
	}


	MeldedKong::~MeldedKong(void)
	{
	}

	void MeldedKong::run()
	{
		_pool->addSomeOutCards(4, _cardNumber);   // 增加倒牌

		// 更新手牌
		_pool->afterAction(3);

		// 播放动画
		uiAction("gangAction");
	}

}