#ifndef _MELDEDKONG_H_
#define _MELDEDKONG_H_

#include "poolaction.h"

namespace XZMJ
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