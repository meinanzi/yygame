#ifndef _ZYHZ_FACTORYCARDPOOL_H_
#define _ZYHZ_FACTORYCARDPOOL_H_

#include "ZYHZ_CardPool.h"
#include "ZYHZ_MahjongCardPool.h"
#include "ZYHZ_SouthMahjongCardPool.h"
#include "ZYHZ_WestMahjongCardPool.h"
#include "ZYHZ_NorthMahjongCardPool.h"
#include "ZYHZ_EastMahjongCardPool.h"

#include "ZYHZ_TouchCard.h"
#include "ZYHZ_MeldedKong.h"
#include "ZYHZ_ConcealedKong.h"
#include "ZYHZ_TouchKong.h"
#include "ZYHZ_HuCard.h"

namespace ZYHZ
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