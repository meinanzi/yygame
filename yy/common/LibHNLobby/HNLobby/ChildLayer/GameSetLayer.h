#ifndef __GAMESET_LAYER_H__
#define __GAMESET_LAYER_H__

#include "HNUIExport.h"
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocostudio;
using namespace ui;

class GameSetLayer : public HNLayer
{
	enum UI_TYPE  
	{
		M_SLIDER = 28,                           //音乐拖动条
		E_SLIDER = 32,                           //音效拖动条
		M_CHECKBOX = 40,                         //音乐静音复选框
		E_CHECKBOX = 41,                         //音效静音复选框
	};
	Slider* _musicSlider;
	Slider* _effectSlider;
	CheckBox* _musicCheckBox;
	CheckBox* _effectCheckBox;
	Layout* _layout;
public:
	GameSetLayer();
	virtual ~GameSetLayer();

public:
    virtual bool init() override;

	void showSet(Node* parent, int zorder, int tag = -1);

	void closeSet();

private:
	LayerColor* colorLayer;
	Node* settingLoader;
	Slider* effectSlider;
	Slider* musicSlider;
	//关闭回调
	void closeSetCallBack(Ref* pSender, Widget::TouchEventType type);
	//拖动条回调函数
	void sliderCallback(Ref* pSender, Slider::EventType type, UI_TYPE num);
	//控制音效
	void setCallBack(Ref* pSender,Widget::TouchEventType type,int num);

public:
    CREATE_FUNC(GameSetLayer);
};

#endif // __GAMESET_LAYER_H__
