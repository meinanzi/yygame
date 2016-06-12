#include "CardPoolUser.h"

namespace XZMJ
{

	CardPoolUser::CardPoolUser(void)
	{
		_cardPool = nullptr;
	}


	CardPoolUser::~CardPoolUser(void)
	{
	}

	void CardPoolUser::setPool(CardPool* pool)
	{
		_cardPool = pool;
		this->addChild(pool);
	}

}