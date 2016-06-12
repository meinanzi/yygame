#ifndef _ZYHZ_NORTHMAHJONGCARDPOOL_H_
#define _ZYHZ_NORTHMAHJONGCARDPOOL_H_

#include "ZYHZ_mahjongcardpool.h"

namespace ZYHZ
{
	class NorthMahjongCardPool :
		public MahjongCardPool
	{
	public:
		NorthMahjongCardPool(void);
		~NorthMahjongCardPool(void);

		CREATE_COUNT(NorthMahjongCardPool);
		virtual bool init(INT count);

		virtual void setHandCardPos(Card* noCard = nullptr) override;     // ∞≤÷√ ÷≈∆

	};

}

#endif

