#ifndef _TOUCHKONG_H_
#define _TOUCHKONG_H_

#include "poolaction.h"

namespace XZMJ
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
