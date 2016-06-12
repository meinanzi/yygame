#include "ZYHZ_Factory.h"

namespace ZYHZ
{

	CardPool* Factory::createEastPool(INT count)
	{
		return EastMahjongCardPool::create(count);
	}


	CardPool* Factory::createNorthPool(INT count)
	{
		return NorthMahjongCardPool::create(count);
	}


	CardPool* Factory::createWestPool(INT count)
	{
		return WestMahjongCardPool::create(count);
	}


	CardPool* Factory::createSouthPool(INT count)
	{
		return SouthMahjongCardPool::create(count);
	}

	PoolAction* Factory::createTouchCardAction()
	{
		return TouchCard::create();
	}

	PoolAction* Factory::createMeldedKongAction()
	{
		return MeldedKong::create();
	}

	PoolAction* Factory::createConcealedKongAction()
	{
		return ConcealedKong::create();
	}

	PoolAction* Factory::createTouchKongAction()
	{
		return TouchKong::create();
	}

	PoolAction* Factory::createHuCardAction()
	{
		return HuCard::create();
	}

}