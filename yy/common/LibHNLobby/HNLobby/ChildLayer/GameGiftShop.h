#ifndef __GameGiftShop_LAYER_H__
#define __GameGiftShop_LAYER_H__

#include "HNLobby/PlatformDefine.h"
#include "HNLobby/Shop/ShopManager.h"
#include "network/HttpClient.h"
#include "HNNetExport.h"
#include "HNUIExport.h"
#include "HNOpenExport.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;
using namespace HN;
using namespace ui;
using namespace cocostudio;
using namespace cocos2d::network;


class GameGiftShop : public HNLayer, public HNHttpDelegate
{
	struct GiftInfo
	{
		int				giftID;			//礼品ID
		std::string		giftName;		//礼品名称
		std::string		giftInfo;		//礼品简介
		std::string		giftPic;		//礼品图片
		int				giftNum;		//礼品库存
		LLONG			lotteryNum;		//所需奖券
		int				giftType;		//礼品类型（虚拟/实物）
		std::string		addDate;		//添加时间
	};

	std::vector<GiftInfo>		_gifts;
	std::vector<Sprite*>		_pagesPrompt;

public:
	typedef std::function<void ()> CloseCallBack;
	CloseCallBack	onCloseCallBack;

public:
	static GameGiftShop* createGameGiftShop(MoneyChangeNotify* delegate, LLONG lottery);

public:
	GameGiftShop();
	virtual ~GameGiftShop();

	bool init(LLONG lottery);

	void setChangeDelegate(MoneyChangeNotify* delegate);

	//关闭商店
	void closeGiftShop();

public:
	// 获取虚拟物品兑换页面
	void getVirtualPage(GiftInfo* info, SpriteFrame* frame);

	// 获取实物兑换页面
	void getEntityPage(GiftInfo* info, SpriteFrame* frame);

public:
	// 获取查询请求数据
	void getQueryParams();

	// 创建商品页面
	void createGiftPages();

	// 查询请求回应
	void onHttpQueryCompleted(const std::string& data);

	// 获取兑换记录数据
	void getRecordParams();

	// 兑换记录回应
	void onHttpRecordCompleted(const std::string& data);

	// 下载商品图片回应
	void onHttpDownloadPicCompleted(const std::string& tag, const std::string& data);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// 获取兑换请求数据
	std::string getExchangeParams(int awardid, const std::string address, const std::string name, const std::string mobile);

	// 兑换礼品回应
	void onHttpExchangeGiftCompleted(const std::string& data);

	virtual void onHttpResponse(const std::string& requestName, bool isSucceed, const std::string &responseData) override;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// 签名组合发送数据
	std::string signParams(std::vector<std::string>& params);

	// 发送请求
	void requestWithParams(const std::string& params, const std::string& tag, const std::string& url, HttpRequest::Type type);


private:
	//pageView拖动回调函数
	void pageViewMoveCallBack(Ref* pSender, PageView::EventType type);

	//列表按钮点击回调函数
	void btnEventCallBack(Ref* pSender, Widget::TouchEventType type);

private:
	MoneyChangeNotify*				_delegate;
	Layout*							_panel_Gift;
	Layout*							_panel_GiftPage;
	Layout*							_panel_Record;
	Layout*							_panel_Virtual;
	Layout*							_panel_Entity;
	PageView*						_pageView_Gift;
	ListView*						_listView_Record;
	Button*							_btnShop;
	Button*							_btnRecord;
	INT								_currentPageIdx;



};

#endif // __GameGiftShop_LAYER_H__
