#ifndef _ZYHZ_HUCARD_H_
#define _ZYHZ_HUCARD_H_

#include "ZYHZ_poolaction.h"

namespace ZYHZ
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