#ifndef _ZYHZ_EASTMAHJONGCARDPOOL_H_
#define _ZYHZ_EASTMAHJONGCARDPOOL_H_


#include "ZYHZ_mahjongcardpool.h"

namespace ZYHZ
{
	class EastMahjongCardPool :
		public MahjongCardPool
	{
	public:
		EastMahjongCardPool(void);
		~EastMahjongCardPool(void);

		CREATE_COUNT(EastMahjongCardPool);
		virtual bool init(INT count);

		virtual void setHandCardPos(Card* noCard = nullptr) override;					 // ∞≤÷√ ÷≈∆

	};

}

#endif