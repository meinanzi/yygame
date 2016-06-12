#ifndef _ZYHZ_MELDEDKONG_H_
#define _ZYHZ_MELDEDKONG_H_

#include "ZYHZ_poolaction.h"

namespace ZYHZ
{

	class MeldedKong :
		public PoolAction
	{
	public:
		MeldedKong(void);
		~MeldedKong(void);

		virtual void run() override;

		CREATE_FUNC(MeldedKong);
	};

}

#endif