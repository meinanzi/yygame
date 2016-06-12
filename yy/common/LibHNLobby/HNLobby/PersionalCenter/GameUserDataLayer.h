#ifndef __GAMEUSERDATA_LAYER_H__
#define __GAMEUSERDATA_LAYER_H__

#include "HNUIExport.h"
#include "HNNetExport.h"
#include "cocos2d.h"

USING_NS_CC;

class GameUserDataLayer : public HNLayer
{
	struct UI
	{
		Text* Label_LandName;
		Text* Label_UserNickName;
		Text* Label_Sex;
		ImageView*	Image_Sex;
		Text* Label_UserID;
		Text* Label_Money;
		Text* Label_Fascination;
		Text* Label_Bank;
		Text* Label_phone;

		Button*	Button_BindPhone;
		Button*	Button_ModifyPassword;
		Button*	Button_Close;

		Layout*	layout;
	} _ui;

	Rect _userRect;

private:
	std::string		_verifyCode;
	bool			_canClose;

public:
	CREATE_FUNC(GameUserDataLayer);

	GameUserDataLayer();
	virtual ~GameUserDataLayer();
	LayerColor* colorLayer;
	EventListenerTouchOneByOne* touchListener;

public:
    virtual bool init() override;  

	virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event * pEvent) override;

public:
	void showUserInfo(Node* parent, int zorder, int tag);

	void closeUserInfo();
	
	bool canClose() const { return _canClose; }

private:
	Node* userDataLoader;
	void closeCallBack(Ref* pSender, Widget::TouchEventType type);

	void modifyPasswordUIEventCallBack(Ref* pSender, Widget::TouchEventType type);

	void bindPhoneUIEventCallBack(Ref* pSender, Widget::TouchEventType type);

	// 解绑回调
	bool unbindPhoneCallback(HNSocketMessage* socketMessage);

	// 更新用户信息
	void updateUserDetail();

};

#endif // __GAMEUSERDATA_LAYER_H__
