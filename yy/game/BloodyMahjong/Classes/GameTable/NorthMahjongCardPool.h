#ifndef _NORTHMAHJONGCARDPOOL_H_
#define _NORTHMAHJONGCARDPOOL_H_

#include "mahjongcardpool.h"

namespace XZMJ
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

