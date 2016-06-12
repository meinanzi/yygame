#ifndef _HUCARD_H_
#define _HUCARD_H_

#include "poolaction.h"

namespace XZMJ
{

	class HuCard :
		public PoolAction
	{
	public:
		HuCard(void);
		~HuCard(void);

		virtual void run() override;
		void setZiMoQiangGame(bool zimo, bool qiangGang) { _isZiMo =  zimo; _isQiangGang = qiangGang; };
		CREATE_FUNC(HuCard);

	private:
		bool _isZiMo;
		bool _isQiangGang;
	};

}

#endif