#ifndef _ZYHZ_CONCEALEDKONG_H_
#define _ZYHZ_CONCEALEDKONG_H_

#include "ZYHZ_poolaction.h"

namespace ZYHZ
{

	class ConcealedKong :
		public PoolAction
	{
	public:
		ConcealedKong(void);
		~ConcealedKong(void);

		virtual void run() override;

		CREATE_FUNC(ConcealedKong);

	};

}
#endif