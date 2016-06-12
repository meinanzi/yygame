#ifndef _TOUCHCARD_H_
#define _TOUCHCARD_H_

#include "poolaction.h"

namespace XZMJ
{
	class TouchCard :
		public PoolAction
	{
	public:
		TouchCard(void);
		~TouchCard(void);

		virtual void run() override;

		CREATE_FUNC(TouchCard);
	};

}

#endif