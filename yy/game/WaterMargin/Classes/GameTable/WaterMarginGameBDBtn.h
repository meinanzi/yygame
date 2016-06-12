#ifndef __WaterMargin_GAME_BDBtn_H__
#define __WaterMargin_GAME_BDBtn_H__

#include "HNNetExport.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "HNUIExport.h"
#include "ui/UICheckBox.h"
#include <vector>

USING_NS_CC;

using namespace cocostudio;
using namespace std;
using namespace ui;
using namespace HN;


namespace WaterMargin
{
	class GameTableLogic;
	class BDBtn : public  HNLayer
	{
	protected:
		BDBtn();
		~BDBtn();
	public:
		static BDBtn* create(GameTableLogic* tableLogic, Node* node);
		//	CREATE_FUNC(GameBiBei);
		bool init(GameTableLogic* tableLogic, Node* node);

		void menuClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);		//按钮回调函数

		vector<Button*> _btn;
		GameTableLogic* _btnLogic;
		Layout* _widget;

	};
}
#endif