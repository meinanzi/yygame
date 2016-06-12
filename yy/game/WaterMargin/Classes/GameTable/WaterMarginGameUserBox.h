#ifndef __WaterMargin_GAMEUSERBOX_LAYER_H__
#define __WaterMargin_GAMEUSERBOX_LAYER_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "HNNetExport.h"

USING_NS_CC;

using namespace cocostudio;
using namespace std;
using namespace ui;
using namespace HN;

namespace WaterMargin
{
	class GameUserMessageBox : public HNDialogBase
	{
		Rect _userDataBoxRect;
	public:
		Text* _iMoney;
	public:

		GameUserMessageBox();
		bool init(cocos2d::Node* parent, INT userID);
		static GameUserMessageBox* createMessageBox(cocos2d::Node* parent,INT userID);
	};
}



#endif