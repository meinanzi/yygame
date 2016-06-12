#ifndef __TRACTORGameUserMessageBox_h__
#define __TRACTORGameUserMessageBox_h__


#include "cocos2d.h"
#include "HNNetExport.h"
#include "HNUIExport.h"

USING_NS_CC;

using namespace HN;

namespace TRACTOR
{
	class GameUserMessageBox : public HNDialogBase
	{
		Rect _userDataBoxRect;
	public:
		bool init(cocos2d::Node* parent, INT userID);
		static GameUserMessageBox* createMessageBox(cocos2d::Node* parent,INT userID);
	private:
		ui::Widget* wiget;
	};
}

#endif // __TRACTORGameUserMessageBox_h__