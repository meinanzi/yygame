#ifndef _MAHJONGCARDPOOLUSER_H_
#define _MAHJONGCARDPOOLUSER_H_

#include "cardpooluser.h"

namespace XZMJ
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