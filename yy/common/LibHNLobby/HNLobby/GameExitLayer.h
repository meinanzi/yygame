#ifndef __GAMEXIT_LAYER_H__
#define __GAMEXIT_LAYER_H__

#include "HNNetExport.h"
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocostudio;
using namespace HN;

class GameExitLayer : public HNLayer
{
private:
	Node* _loader;
	Layout* _layout;
	Button* _btnContinue;
	Button* _btnQuit;
public:
    virtual bool init() override;  
	void ButtonCallBack(Ref* pSender, Widget::TouchEventType  type);
	void callBackYes();
	void callBackNo();
    CREATE_FUNC(GameExitLayer);
};

#endif // __GAMEXIT_LAYER_H__