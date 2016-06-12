#include "ZYHZ_HuCard.h"

namespace ZYHZ
{

	HuCard::HuCard(void)
	{
		_runSp = Sprite::createWithSpriteFrameName("huAction1.png");
		this->addChild(_runSp);
		_runSp->setVisible(false);
		_isZiMo = false;
	}


	HuCard::~HuCard(void)
	{
	}

	void HuCard::run()
	{
		_pool->huCard(_isZiMo, _isQiangGang);
		// ²¥·Å¶¯»­
		uiAction("huAction");
	}

}