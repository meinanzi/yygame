#ifndef __GAMESTORE_LAYER_H__
#define __GAMESTORE_LAYER_H__

#include "HNLobby/PlatformDefine.h"
#include "HNLobby/Shop/ShopManager.h"
#include "HNNetExport.h"
#include "HNUIExport.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "network/HttpClient.h"

USING_NS_CC;
using namespace HN;
using namespace ui;
using namespace cocostudio;


class GameStoreLayer : public HNLayer, public HNHttpDelegate
{
public:
	typedef std::function<void ()> CloseCallBack;
	CloseCallBack	onCloseCallBack;

private:
	ListView* _storeList;
    Button* _closeButton;
	Layout* createStoreItemTemplate(const PRODUCT_INFO* productleft, const PRODUCT_INFO* productright,int pos);
	Node* payLoader;
	LayerColor* colorLayer;
	EventListenerTouchOneByOne* touchListener;
public:
	static GameStoreLayer* createGameStore(MoneyChangeNotify* delegate);

public:
	GameStoreLayer();
	virtual ~GameStoreLayer();

	virtual bool init() override;

	void setChangeDelegate(MoneyChangeNotify* delegate);
	
	void openStore(Node* parent, Vec2 vec, int zorder, int tag);

	void showPayLayer(const PRODUCT_INFO* product);
	Button* showStoreMassage(const PRODUCT_INFO* product,int pos);

	void closeStore();

	// 界面消息回调函数
private:
	void buyEventCallBack(Ref* pSender, Widget::TouchEventType type);

	void closeEventCallBack(Ref* pSender, Widget::TouchEventType type);

	void closePayCallBack(Ref* pSender, Widget::TouchEventType type);

	bool payEventSelector(HNSocketMessage* socketMessage);

private:
	void payCB(const std::string& args);
    
    // 支付成功回调
    void payCallback(const std::string& data);

	// 支付成功回调
	void payCallback_IAP(const std::string& data);
    
    // http请求响应
	virtual void onHttpResponse(const std::string& requestName, bool isSucceed, const std::string &responseData) override;
	void onSDKGetMoneyCallback(const std::string& data);
	void onIAPGetMoneyCallback(const std::string& data);
	void onAddOrderCallback(const std::string& data);
    
    // 发货失败提示
    void supportAlert();

	// 获取查询订单参数
	std::string getCheckOrderParam();

	// 订单查询接口(从智付端验证）
	bool checkOrder(std::string &result);

	// 解析订单查询返回参数
	bool parseCheckOrder(const std::string result, std::string &is_success, std::string &sign, std::string& merchant_code, std::string& order_no,
		std::string& trade_status);

	// 实际发货（送往后台）
	void payMoney();

	//执行支付
	void executePayment_Dinpay(PRODUCT_INFO* productInfo);
	//执行支付
	void executePayment(PRODUCT_INFO* productInfo);
	//执行支付
	void startPayment(PRODUCT_INFO* productInfo);

	std::string addOrder_URL(const char* userName, DOUBLE price, std::string payType);

	std::string getOrder_URL(std::string orderID);

	std::string getPayTypeInfo(INT payType);
    
private:
    // 订单号
    std::string						_orderId;
    
    PRODUCT_INFO*					_product;
	MoneyChangeNotify*				_delegate;
	Text*							_selfCoin;//充值成功后刷新界面，改成成员

	// 订单号
	std::string                     _orderNo;

	// 未给金币的订单号
	std::string                     _notOfferOrderNo;
};

#endif // __GAMESTORE_LAYER_H__
