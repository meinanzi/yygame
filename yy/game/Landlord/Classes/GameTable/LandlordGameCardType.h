#ifndef _Landlord_GAME_CARD_TYPE_
#define _Landlord_GAME_CARD_TYPE_

#include "cocos2d.h"
#include "HNNetExport.h"
USING_NS_CC;

namespace landlord
{
	class GameCardType :public HNLayer
	{
	public:
		GameCardType();
		~GameCardType();
	public:
		static GameCardType* create();
		virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	private:
		bool init();
	};
}



#endif // !_GAME_CARD_TYPE_
