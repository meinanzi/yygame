#ifndef __Landlord_GAMEUSERMESSAGE_LAYER_H__
#define __Landlord_GAMEUSERMESSAGE_LAYER_H__

#include "cocos2d.h"
#include "HNNetExport.h"

USING_NS_CC;
using namespace HN;

namespace landlord
{
	class GameUserMessageBox : public HNDialogBase
	{
		Rect _userDataBoxRect;
	public:
		bool init(cocos2d::Node* parent, INT userID);
		static GameUserMessageBox* createMessageBox(cocos2d::Node* parent,INT userID);
	};
}



#endif // __GAMEUSERMESSAGE_LAYER_H__
