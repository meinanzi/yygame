#ifndef __Landlord_CARD_TYPE_ANIMATION_H__
#define __Landlord_CARD_TYPE_ANIMATION_H__

#include "cocos2d.h"

namespace landlord
{
	class CardTypeAnimation
	{
	public:
		static void playRocket(cocos2d::Node* parent, int zorder);
		static void playBomb(cocos2d::Node* parent, int zorder);
		static void playStraight(cocos2d::Node* parent, int zorder);
		static void playDoubleStraight(cocos2d::Node* parent, int zorder);
		static void playThree(cocos2d::Node* parent, int zorder);
	};
}



#endif