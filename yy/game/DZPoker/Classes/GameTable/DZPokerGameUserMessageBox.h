#ifndef __DZPokerGameUserMessageBox_h__
#define __DZPokerGameUserMessageBox_h__


#include "cocos2d.h"
#include "HNNetExport.h"
//#include "HNUIExport.h"

USING_NS_CC;

using namespace HN;

namespace DZPoker
{
	class GameUserMessageBox : public HNDialogBase
	{
		Rect _userDataBoxRect;
	public:
		bool init(cocos2d::Node* parent, int userID);
		static GameUserMessageBox* createMessageBox(cocos2d::Node* parent, int userID);
	};

}


#endif // __DZPokerGameUserMessageBox_h__
