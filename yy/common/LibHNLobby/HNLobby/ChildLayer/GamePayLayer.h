#ifndef GamePayLayer_h__
#define GamePayLayer_h__

#include "HNLobby/Shop/ShopManager.h"
#include "HNNetExport.h"
#include "HNUIExport.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;
using namespace HN;
using namespace ui;
using namespace cocostudio;

class PaySelectedDelegate
{
public:
	enum PayMethod
	{
		UNKNOW,
		ALIPAY,
		WECHAT,
		UNIONPAY
	};
public:
	virtual ~PaySelectedDelegate() {}
	
	virtual void onPayEvent(PayMethod method) = 0;

	virtual void onPayClose() = 0;
};

class GamePayLayer : public HNLayer
{
private:
	struct PAY_UI
	{
		enum 
		{
			CLOSE_TAG = 100,
			DESCRIPTION_TAG = 101,
			PRICE_TAG = 102,
			WECHAT_TAG = 103,
			ALIPAY_TAG = 104,
			UNIONPAY = 105,
			PHONE_TAG = 106
		};
		Text* Label_Product_Description;
		Text* Label_Product_Price;
		Text* Label_Phone;
		Button* Button_WeChat;
		Button* Button_Alipay;
		Button* Button_UnionPay;
		Button* Button_Close;
	} _payUI;

private:
	INT				_opacity;
	PRODUCT_INFO	_product;
	Layer*			_colorLayer;
	Rect			_payUIRect;
	Layout*			_payUILayout;
	PaySelectedDelegate* _delegate;
public:
	static GamePayLayer* createPaySelected(PRODUCT_INFO* product);

public:
	GamePayLayer();
	virtual ~GamePayLayer();

	virtual bool initWithProduct(PRODUCT_INFO* product);

	virtual void onEnter() override;

	void setPayDelegate(PaySelectedDelegate* delegate)
	{
		_delegate = delegate;
	}

	// 界面消息回调函数
private:
	void paySelectEventCallBack(Ref* pSender, Widget::TouchEventType type, int uiTag);

	void closeEventCallBack();

	void updateOpacity(float dt);

	void closeLayer();

public:
	bool bankerEventSelector(HNSocketMessage* socketMessage);

};

#endif // GamePayLayer_h__
