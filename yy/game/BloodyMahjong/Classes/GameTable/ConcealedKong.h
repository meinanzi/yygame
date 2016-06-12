#ifndef _CONCEALEDKONG_H_
#define _CONCEALEDKONG_H_

#include "poolaction.h"

namespace XZMJ
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