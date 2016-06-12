#ifndef _WESTMAHJONGCARDPOOL_H_
#define _WESTMAHJONGCARDPOOL_H_

#include "mahjongcardpool.h"

namespace XZMJ
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