#ifndef _EASTMAHJONGCARDPOOL_H_
#define _EASTMAHJONGCARDPOOL_H_


#include "mahjongcardpool.h"

namespace XZMJ
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