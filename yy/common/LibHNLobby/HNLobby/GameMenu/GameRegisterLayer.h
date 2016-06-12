#ifndef GameRegisterLayer_h__
#define GameRegisterLayer_h__

#include "HNNetExport.h"
#include "HNUIExport.h"
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "ui/UICheckBox.h"

USING_NS_CC;

using namespace cocostudio;
using namespace ui;
using namespace HN;

class RegisterLayer : public HNLayer, public ui::EditBoxDelegate
{

public:
	typedef std::function<void (const std::string& name, const std::string& psd)> RegisterCallBack;
	typedef std::function<void ()> CloseCallBack;

private:

	struct REGISTUI
	{
		ImageView*  registBG;
		HNEditBox*	editBoxUserName;
		HNEditBox*	editBoxPassWord;
		CheckBox*   agree;
		CheckBox*   showPass;
		Node*		registUI;


	}_registUI;

	std::string	_userName;
	std::string	_userPassword;

public:
	RegisterCallBack onRegisterCallBack;
	CloseCallBack onCloseCallBack;

public:
	CREATE_FUNC(RegisterLayer);

public:
	RegisterLayer();
	virtual ~RegisterLayer();

	virtual bool init() override;

public:
	void closeRegister();
private:
	void closeClickCallback(Ref* pSender, Widget::TouchEventType type);
	void registerClickCallback(Ref* pSender, Widget::TouchEventType type);
	void resetClickCallBack(Ref* pSender, Widget::TouchEventType type);
	// 复选框回调函数
	void checkBoxCallback(Ref* pSender,CheckBox::EventType type);
	//协议
	void AgreeClickCallBack(Ref* pSender, Widget::TouchEventType type);


	virtual void editBoxReturn(ui::EditBox* editBox) {};
};

#endif // GameRegisterLayer_h__
