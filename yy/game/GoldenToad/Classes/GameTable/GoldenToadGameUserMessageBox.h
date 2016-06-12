#ifndef __HN_GoldenToad_GAMEUSERMESSAGE_LAYER_H__
#define __HN_GoldenToad_GAMEUSERMESSAGE_LAYER_H__

#include "cocos2d.h"
#include "HNNetExport.h"
#include "HNUIExport.h"

USING_NS_CC;

using namespace HN;

namespace GoldenToad
{
	class GoldenToadUserMessageBox : public HNDialogBase
	{
		Rect _userDataBoxRect;
	public:
		bool init(cocos2d::Node* parent, INT userID, LLONG userMoney);
		static GoldenToadUserMessageBox* createMessageBox(cocos2d::Node* parent, INT userID, LLONG userMoney);
	private:
		Node* _loader;
	};
}

#endif // __GAMEUSERMESSAGE_LAYER_H__
