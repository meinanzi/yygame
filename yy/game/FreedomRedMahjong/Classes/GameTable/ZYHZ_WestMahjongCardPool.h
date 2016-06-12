#ifndef _ZYHZ_WESTMAHJONGCARDPOOL_H_
#define _ZYHZ_WESTMAHJONGCARDPOOL_H_

#include "ZYHZ_mahjongcardpool.h"

namespace ZYHZ
{
	class WestMahjongCardPool :
		public MahjongCardPool
	{
	public:
		WestMahjongCardPool(void);
		~WestMahjongCardPool(void);

		CREATE_COUNT(WestMahjongCardPool);
		virtual bool init(INT count);

		virtual void setHandCardPos(Card* noCard = nullptr) override;				 // ∞≤÷√ ÷≈∆

	};

}

#endif