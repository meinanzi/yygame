#ifndef __HNLogicBase_H__
#define __HNLogicBase_H__

#include "HNNetExport.h"

namespace HN
{
	/*
	 * redbird logic base.
	 */
	class HNLogicBase
	{
	public:
		virtual void start();
		virtual void stop();
	protected:
		HNLogicBase();
		virtual ~HNLogicBase();
	};
}

#endif