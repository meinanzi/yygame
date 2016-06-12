#include "ZYHZ_CardPoolUser.h"

namespace ZYHZ
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