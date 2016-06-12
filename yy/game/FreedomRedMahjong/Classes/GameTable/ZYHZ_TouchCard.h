#ifndef _ZYHZ_TOUCHCARD_H_
#define _ZYHZ_TOUCHCARD_H_

#include "ZYHZ_poolaction.h"

namespace ZYHZ
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