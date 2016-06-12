#ifndef __ModifyPassword_h__
#define __ModifyPassword_h__

#include "HNUIExport.h"
#include "HNNetExport.h"
#include "cocos2d.h"

USING_NS_CC;

class ModifyPasswordLayer : public HNLayer, public ui::EditBoxDelegate
{
	struct UI
	{
		Node* modifyLoader;
		HNEditBox* TextField_oldPassword;
		HNEditBox* TextField_NewPassword;
		HNEditBox* TextField_SurePassword;

		Button*	Button_OK;
		Button*	Button_Cancel;
		ImageView*	layout;
	} _ui;

	Rect _userRect;
	std::string _userPswd;

public:
	CREATE_FUNC(ModifyPasswordLayer);

	ModifyPasswordLayer();
	virtual ~ModifyPasswordLayer();

public:
	virtual bool init() override;  

	void show(Node* parent, int zorder, int tag);

	void close();

private:
	void okEventCallBack(Ref* pSender, Widget::TouchEventType type);

	void closeCallBack(Ref* pSender, Widget::TouchEventType type);

private:
	bool modifyPasswordSelector(HNSocketMessage* socketMessage);


	virtual void editBoxReturn(ui::EditBox* editBox) {};
};

#endif // __ModifyPassword_h__
