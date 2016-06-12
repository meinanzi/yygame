#ifndef __GamePasswordInput_H__
#define __GamePasswordInput_H__

#include "HNUIExport.h"
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocostudio;
using namespace ui;

class GamePasswordInput : public HNLayer, public ui::EditBoxDelegate
{
public:
	// 构造函数
	GamePasswordInput();

	// 析构函数
	virtual ~GamePasswordInput();

	// 创建弹窗
	static GamePasswordInput* create();

	// 初始化
	virtual bool init() override;

	std::function<void(const std::string& password)> onEnterCallback;
protected:	
	// 关闭弹窗回调
	void onCloseClickCallback(Ref* pSender, Widget::TouchEventType type);
	
	// 进入房间回调
	void onEnterClickCallback(Ref* pSender, Widget::TouchEventType type);

	// 密码输入
	virtual void editBoxReturn(ui::EditBox* editBox) override{};

private:
	// 密码输入框
	HNEditBox* _editbox_pass;
};

#endif // 
