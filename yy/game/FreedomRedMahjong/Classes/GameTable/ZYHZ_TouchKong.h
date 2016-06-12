#ifndef _ZYHZ_TOUCHKONG_H_
#define _ZYHZ_TOUCHKONG_H_

#include "ZYHZ_poolaction.h"

namespace ZYHZ
{

	class TouchKong :
		public PoolAction
	{
	public:
		TouchKong(void);
		~TouchKong(void);

		virtual void run() override;

		CREATE_FUNC(TouchKong);
	};

}

#endif
