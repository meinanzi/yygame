#ifndef _FACTORYCARDPOOL_H_
#define _FACTORYCARDPOOL_H_

#include "CardPool.h"
#include "MahjongCardPool.h"
#include "SouthMahjongCardPool.h"
#include "WestMahjongCardPool.h"
#include "NorthMahjongCardPool.h"
#include "EastMahjongCardPool.h"

#include "TouchCard.h"
#include "MeldedKong.h"
#include "ConcealedKong.h"
#include "TouchKong.h"
#include "HuCard.h"

namespace XZMJ
{

	class Factory
	{
	public:
		static CardPool* createEastPool(INT count);
		static CardPool* createNorthPool(INT count);
		static CardPool* createWestPool(INT count);
		static CardPool* createSouthPool(INT count);

		static PoolAction* createTouchCardAction();
		static PoolAction* createMeldedKongAction();
		static PoolAction* createConcealedKongAction();
		static PoolAction* createTouchKongAction();
		static PoolAction* createHuCardAction();
	};

}

#endif