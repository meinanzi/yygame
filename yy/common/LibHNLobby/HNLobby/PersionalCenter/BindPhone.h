#ifndef __BindPhone_h__
#define __BindPhone_h__

#include "HNUIExport.h"
#include "HNNetExport.h"
#include "cocos2d.h"
USING_NS_CC;

class BindPhoneLayer : public HNLayer, public ui::EditBoxDelegate
{
	struct UI
	{
		Node* bindLoader;
		HNEditBox* TextField_PhoneNumber;
		HNEditBox* TextField_Code;

		Button*	Button_Code;
		Button*	Button_OK;
		Button*	Button_Cancel;

		ImageView*	layout;
	} _ui;

	Rect _userRect;
	Text* _timeText;

	int _restTime;//倒计时时间
private:
	std::string		_verifyCode;

public:
	CREATE_FUNC(BindPhoneLayer);

	BindPhoneLayer();
	virtual ~BindPhoneLayer();

public:
	virtual bool init() override;  

	void show(Node* parent, int zorder, int tag);

	void close();

private:
	void verifyCodeUIEventCallBack(Ref* pSender, Widget::TouchEventType type);

	void okEventCallBack(Ref* pSender, Widget::TouchEventType type);

	void closeCallBack(Ref* pSender, Widget::TouchEventType type);

private:
	bool verifyCodeSelector(HNSocketMessage* socketMessage);

	bool bindPhoneSelector(HNSocketMessage* socketMessage);

	bool modifyUserInfoSelector(HNSocketMessage* socketMessage);


	virtual void editBoxReturn(ui::EditBox* editBox) {};

	// 更新用户信息
	void updateUserDetail();

	//获取验证码的时间限制
	void setLimitTime();
	void updateLimitTime(float dt);
};

#endif // __BindPhone_h__
