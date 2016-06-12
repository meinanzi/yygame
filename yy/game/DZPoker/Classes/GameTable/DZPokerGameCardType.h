#ifndef __DZPokerGameCardType_h__
#define __DZPokerGameCardType_h__


#include "cocos2d.h"
#include "HNNetExport.h"
#include "HNLobbyExport.h"
USING_NS_CC;

namespace DZPoker
{

	class GameCardType :public HNLayer
	{
	public:
		CREATE_FUNC(GameCardType)

	public:
		GameCardType();
		virtual ~GameCardType();

	private:
		bool init() override;
		void moveByAnimationCallback(Node* sender);	

	public:
		virtual bool onTouchBegan(Touch *touch, Event *unused_event);

	};
}


#endif // __DZPokerGameCardType_h__
