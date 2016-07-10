#ifndef _ZYHZ_MAHJONGCARD_H_
#define _ZYHZ_MAHJONGCARD_H_

#include "ZYHZ_card.h"


namespace ZYHZ
{
	class MahjongCard :
		public Card
	{
	public:
		MahjongCard(void);
		~MahjongCard(void);

		CREATE_CARD(MahjongCard);
		virtual bool init(mahjongCreateType type, sitDir sitNo, INT number) override;
        
        std::string getSpriteframename() {return _spriteframename;};
        
    private:
        std::string _spriteframename;
	};


}

#endif	// _MAHJONGCARD_H_