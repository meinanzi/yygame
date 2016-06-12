#ifndef _ZYHZ_MAHJONGCARDPOOLUSER_H_
#define _ZYHZ_MAHJONGCARDPOOLUSER_H_

#include "ZYHZ_cardpooluser.h"

namespace ZYHZ
{

	class MahjongCardPoolUser :
		public CardPoolUser
	{
	public:
		MahjongCardPoolUser(void);
		~MahjongCardPoolUser(void);

		CREATE_FUNC(MahjongCardPoolUser);
		virtual bool init() override;
	};

}

#endif