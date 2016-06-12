#ifndef __BJLPokerCard_h__
#define __BJLPokerCard_h__

#include "HNUIExport.h"
#include "HNNetExport.h"
#include "cocos2d.h"
USING_NS_CC;

namespace BJL
{
	class PokerCard : public HNSprite
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

		//获取对应的扑克图片
		std::string getCardFileName();
		void setCardValue(BYTE val);
		BYTE getCardValue();
	private:
		BYTE _Value;
	};
}

#endif // __BJLPokerCard_h__
