#ifndef __DZPokerPokerCard_h__
#define __DZPokerPokerCard_h__


#include "HNNetExport.h"
#include "cocos2d.h"
USING_NS_CC;

namespace DZPoker
{

	class PokerCard : public Sprite
	{
	public:
		PokerCard();
		~PokerCard();
		static PokerCard* create();
		void setStartPositin(Vec2 position);
		void standUP();
		void sitDown();
		void setCardFileName(const std::string& filename);
		void setCardFrameName(const std::string& filename);

		std::string getCardFileName();
		void setCardValue(BYTE val);
		BYTE getCardValue();
	private:
		BYTE _Value;
	};
}

#endif // __DZPokerPokerCard_h__

