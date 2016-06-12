#ifndef __TRACTORGameCardType_h__
#define __TRACTORGameCardType_h__


#include "cocos2d.h"
#include "HNNetExport.h"
USING_NS_CC;
namespace TRACTOR
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

#endif // __TRACTORGameCardType_h__