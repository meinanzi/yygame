#ifndef _MAHJONGCARD_H_
#define _MAHJONGCARD_H_

#include "card.h"


namespace XZMJ
{
	class MahjongCard :
		public Card
	{
	public:
		MahjongCard(void);
		~MahjongCard(void);

		CREATE_CARD(MahjongCard);
		virtual bool init(mahjongCreateType type, sitDir sitNo, INT number) override;
	};


}

#endif	// _MAHJONGCARD_H_