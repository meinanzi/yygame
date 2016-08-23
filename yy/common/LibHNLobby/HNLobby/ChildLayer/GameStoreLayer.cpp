#include "HNLobby/ChildLayer/GameStoreLayer.h"
#include "HNLobby/FontConfig.h"
#include "HNLobby/PlatformDefine.h"
#include "HNLobby/GamePrompt.h"
#include "HNLobby/PlatformConfig.h"
#include "HNMarketExport.h"
#include <string>
#include "external/tinyxml2/tinyxml2.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/stringbuffer.h"
#include "json/writer.h"

#define JSON_RESOVE(obj, key) (obj.HasMember(key) && !obj[key].IsNull())

using namespace cocos2d::network;

static const char* STORE_BG			= "platform/store/second_payBg.png";                  //backGround
static const char* CLOSE_MENU_N		= "platform/store/second_back.png";                         //closeMenu
static const char* CLOSE_MENU_D		= "platform/store/second_back.png";                      //closeMenu
static const char* CLOSE_PAY		= "platform/store/windowsClose.png";                      //closePay
static const char* PAY_FRAME		= "platform/store/payPopu/payBg.png";               //payBg
static const char* WEIXIN_PAY		= "platform/store/payPopu/weixin.png";               //weixinPay
static const char* ZHIFUBAO_PAY	    = "platform/store/payPopu/zhifubao.png";               //zhifubaoPay
static const char* YINLIAN_PAY		= "platform/store/payPopu/yinlian.png";               //yinlianPay
static const char* XINYONGKA_PAY	= "platform/store/payPopu/xinyongka.png";           //xinyongkaPay
static const char* SELF_FRAME	    = "platform/store/common_opaque.png";           //frame
static const char* SELF_COIN	    = "platform/store/coinMid.png";   

static const char* LIST_BG			= "platform/store/listItem_bg.png";               //listBG
static const char* COIN_ICO			= "platform/store/coin_1.png";                       //chip
static const char* STORE_BUY		= "platform/store/good_bg.png";                       //buy
static const char* PAY_BG			= "platform/store/shop.png";                       //pay
static const char* LEAF_LEFT	    = "platform/subViewBambooLeft.png";               
static const char* LEAF_RIGHT	    = "platform/subViewBambooRight.png";                 

static const char* CHIP_BG			= "platform/chip.png";                      
static const char* STORE_TITLE		= "platform/store/title_btn.png";

static const char* SCENE_GITFCARD = "platform/store/giftcard.csb";

static const char* NOTIFY_URL_DINPAY				= "http://%s/manage/pay/zhifu/mobilereceive.aspx";
static const char* MECHANT_ID_DINPAY				= "1111110166";
static const char* MECHANT_KEY_DINPAY				= "123456789a123456789_";

GameStoreLayer* GameStoreLayer::createGameStore(MoneyChangeNotify* delegate)
{
	GameStoreLayer *pRet = new GameStoreLayer(); 
	if (pRet && pRet->init()) 
	{ 
		pRet->setChangeDelegate(delegate);
		pRet->autorelease(); 
		return pRet; 
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		return nullptr;
	}
}

GameStoreLayer::GameStoreLayer() 
: _product(nullptr)
, _selfCoin(nullptr)
, onCloseCallBack (nullptr)
{

}

GameStoreLayer::~GameStoreLayer()
{
	PlatformLogic()->removeEventSelector(MDM_GP_NOTIFY_PAY, ASS_GP_NOTIFY_PAY);
	HNHttpRequest::getInstance()->removeObserver(this);
}

void GameStoreLayer::openStore(Node* parent, Vec2 vec, int zorder, int tag)
{
	CCAssert(nullptr != parent, "parent is nullptr");

	Size winSize = Director::getInstance()->getWinSize();
	this->setPosition(vec);
	parent->addChild(this, zorder, tag);
	parent->runAction(MoveBy::create(0.3f, Vec2(-winSize.width, 0)));
}

void GameStoreLayer::closeStore()
{
	HNAudioEngine::getInstance()->playEffect(GAME_SOUND_CLOSE);
	auto winSize = Director::getInstance()->getWinSize();
	this->getParent()->runAction(Sequence::create(MoveBy::create(0.3f, Vec2(winSize.width, 0)),
		CCCallFunc::create([&]()
	{
		this->removeFromParent();
	}), nullptr));
}

bool GameStoreLayer::init()
{
	if (!HNLayer::init())
	{
		return false;
	}
	Size winSize = Director::getInstance()->getWinSize();
	//商店背景
	setBackGroundImage(STORE_BG);

	//商店标题
	auto titleBtn = Button::create(STORE_TITLE,STORE_TITLE);
	titleBtn->setAnchorPoint(Point(0.5, 1));
	titleBtn->setPosition(Vec2(winSize.width / 2, winSize.height));
	auto titleSize = titleBtn->getContentSize();
	titleBtn->setScale((0.8f*1280.0f/winSize.width)/(720/winSize.height),(0.8f*1280.0f/winSize.width)/(720/winSize.height));
	this->addChild(titleBtn, 2);

	//竹叶
	auto leftLeaf = Sprite::create(LEAF_LEFT);
	leftLeaf->setAnchorPoint(Point(0,1));
	leftLeaf->setPosition(0,winSize.height);
	addChild(leftLeaf,5);
	auto rightleaf = Sprite::create(LEAF_RIGHT);
	rightleaf->setAnchorPoint(Point(1,1));
	rightleaf->setPosition(winSize.width,winSize.height);
	addChild(rightleaf,5);

	auto MyListener = EventListenerTouchOneByOne::create();
	//阻止触摸向下传递
	MyListener->setSwallowTouches(true);
	MyListener->onTouchBegan = [&](Touch* touch, Event* event)
	{
		auto target = dynamic_cast<GameStoreLayer*>(event->getCurrentTarget());//获取的当前触摸的目标       
		Point locationInNode = target->convertToNodeSpace(touch->getLocation());
		Size s = target->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);
		return rect.containsPoint(locationInNode);//判断触摸点是否在目标的范围内

	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(MyListener, this);

	// 关闭按钮
	_closeButton = Button::create(CLOSE_MENU_N,CLOSE_MENU_N);
	_closeButton->setPosition(Vec2(winSize.width * 0.05f, winSize.height * 0.94f));
	addChild(_closeButton,5);
	_closeButton->addTouchEventListener(CC_CALLBACK_2(GameStoreLayer::closeEventCallBack,this));

	//商品背景底色
	auto storebg = cocos2d::ui::Scale9Sprite::create(SELF_FRAME);
	storebg->setPreferredSize(Size(1300,(580.0f*(1300.0f/winSize.width))/(720.0f/winSize.height)));
	storebg->setPosition(Vec2(winSize.width / 2, winSize.height/2-10));
	addChild(storebg);

	// 商品列表
	_storeList = ListView::create();
	_storeList->setItemsMargin(5);//列表间隙
	_storeList->setBounceEnabled(true);//回弹
	_storeList->setGravity(ListView::Gravity::CENTER_HORIZONTAL);//滑动方向(上下)
	_storeList->setContentSize(Size(1280.0f, (580.0f* (1280.0f / winSize.width)) / ( 720.0f/winSize.height)));//列表范围
	_storeList->setAnchorPoint(Vec2(0.5f, 0.5f));
	_storeList->setPosition(Vec2(winSize.width / 2, winSize.height / 2 - 10));
	this->addChild(_storeList);

	for (UINT i = 0; i < ProductManger::getInstance()->getSize(); i += 2)
	{		
		const PRODUCT_INFO* productLeft  = i < ProductManger::getInstance()->getSize() ? ProductManger::getInstance()->getProducts(i) : nullptr;
		const PRODUCT_INFO* productRight = (i + 1) < ProductManger::getInstance()->getSize() ? ProductManger::getInstance()->getProducts(i+1) : nullptr;
		if (nullptr != productLeft||nullptr != productRight)
		{
			auto storeItem = createStoreItemTemplate(productLeft,productRight, i);
			_storeList->pushBackCustomItem(storeItem);
		}
	}

	//财富
	char bufferCoin[128];
	auto moneyLoader = CSLoader::createNode("platform/store/moneyNode.csb");
	moneyLoader->setPosition(winSize.width/2, 40);
	addChild(moneyLoader, 10);
	auto moneyLayout = (Layout*)moneyLoader->getChildByName("Panel_1");
	auto imageBg= (ImageView*)moneyLayout->getChildByName("Image_bg");
	imageBg->setContentSize(Size(1280.0f , (75.0f*( 1280.0f/winSize.width))/(720.0f/winSize.height)));

	_selfCoin = (Text*)moneyLayout->getChildByName("Text_money");
	_selfCoin->setColor(colorGold);
	sprintf(bufferCoin, GBKToUtf8("%lld"), PlatformLogic()->loginResult.i64Money);
	_selfCoin->setString(bufferCoin);

	auto selfQuan= (Text*)moneyLayout->getChildByName("Text_quan");
	selfQuan->setColor(colorGold);
	sprintf(bufferCoin, GBKToUtf8("%d"), PlatformLogic()->loginResult.iLotteries);
	selfQuan->setString(bufferCoin);

	auto promptText = (Text* ) moneyLayout->getChildByName("Text_3");
	promptText->setString("");

	//不显示元亨卡数量
	auto lotteryBg = (ImageView* ) moneyLayout->getChildByName("moneyBg_3");
	auto lotteryBtn = (Button* ) moneyLayout->getChildByName("Button_addQuan");
	//selfQuan->setVisible(false);
	//lotteryBg->setVisible(false);
	//lotteryBtn->setVisible(false);
    
    // 充值兑换
    auto giftcardBtn = (Button*)moneyLayout->getChildByName("Button_card");
    giftcardBtn->addTouchEventListener([this](Ref* sender, Widget::TouchEventType touch)
    {
        if(touch == Widget::TouchEventType::ENDED)
        {
            auto node = (Widget*)CSLoader::createNode(SCENE_GITFCARD);
            this->addChild(node, 20);
            
            auto bg = (Layout*)Helper::seekWidgetByName(node, "Panel_bg");
            
            auto cancelBtn = (Button*)Helper::seekWidgetByName(node, "Button_cancel");
            cancelBtn->addTouchEventListener([node](Ref* sender, Widget::TouchEventType touch)
            {
                if(touch == Widget::TouchEventType::ENDED)
                {
                    node->removeFromParent();
                }
            });
        }
    });

	return true;
}

void GameStoreLayer::setChangeDelegate(MoneyChangeNotify* delegate)
{
	_delegate = delegate;
}

Layout* GameStoreLayer::createStoreItemTemplate(const PRODUCT_INFO* productLeft,const PRODUCT_INFO* productRight, int pos)
{
	// 商品列表背景图
	auto storeItem = Layout::create();
	storeItem->setContentSize(Size(1280, 155));
	auto itemWidth = storeItem->getContentSize().width;
	auto itemHeight = storeItem->getContentSize().height;

	auto btnLeft = showStoreMassage(productLeft,pos+1);
	storeItem->addChild(btnLeft,10);
	btnLeft->setPosition(Vec2(itemWidth*0.26f, itemHeight * 0.5f));
	if (productRight!=nullptr)
	{
		auto btnRight = showStoreMassage(productRight,pos+2);
		storeItem->addChild(btnRight,10);
		btnRight->setPosition(Vec2(itemWidth*0.74f, itemHeight * 0.5f));
	}
	return storeItem;
}

Button* GameStoreLayer::showStoreMassage(const PRODUCT_INFO* product,int pos)
{
	auto storeBtn = Button::create(STORE_BUY, STORE_BUY);
	storeBtn->setScale(0.8f);
	auto btnW= storeBtn->getContentSize().width;
	auto btnH = storeBtn->getContentSize().height;
	storeBtn->setUserData(const_cast<PRODUCT_INFO*>(product));
	storeBtn->addTouchEventListener(Widget::ccWidgetTouchCallback(CC_CALLBACK_2(GameStoreLayer::buyEventCallBack, this)));

	char buffer[128];
	// 商品图片
	auto itemImage = ImageView::create(COIN_ICO);
	itemImage->setPosition(Vec2(btnW*0.15f, btnH*0.5f));
	sprintf(buffer, "platform/store/coin_%d.png", pos);
	itemImage->loadTexture(buffer);
	storeBtn->addChild(itemImage, 1, 1);
	// 文字介绍
	auto storeText = Text::create("", "", 25);
	storeText->setAnchorPoint(Vec2(0, 0.5));
	storeText->setPosition(Vec2(btnW*0.3f, btnH*0.7f));
	sprintf(buffer, GBKToUtf8("%lld 金币"), product->number);
	storeText->setString(buffer);
	storeBtn->addChild(storeText, 1, 2);
	// 价格
	auto priceText = Text::create("", "", 25);
	priceText->setColor(colorGold);
	priceText->setAnchorPoint(Vec2(1, 0.5));
	priceText->setPosition(Vec2(btnW*0.85f, btnH*0.7f));
	sprintf(buffer, GBKToUtf8("价格 %.2f元"), product->price);
	priceText->setString(buffer);
	storeBtn->addChild(priceText, 1, 3);

	//兑换价格
	if((int)product->price != 0)
	{
		sprintf(buffer, GBKToUtf8(" 1 元 = %d 金币"),(int)product->number/(int)product->price);
		auto replacePrice = Text::create("", "", 24);	
		replacePrice->setString(buffer);
		replacePrice->setAnchorPoint(Vec2(0, 0.5));
		replacePrice->setPosition(Vec2(btnW*0.3f, btnH*0.3f));
		storeBtn->addChild(replacePrice, 1, 4);
	}

	return storeBtn;
}

void GameStoreLayer::buyEventCallBack(Ref* pSender, Widget::TouchEventType type)
{
	
	switch (type)
	{
	case Widget::TouchEventType::ENDED:
		{
			HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);
            
            Button* selectedBtn = (Button*)pSender;
            
            _product = (PRODUCT_INFO*)selectedBtn->getUserData();
            if (nullptr != _product)
            {
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//				if(PlatformConfig::getInstance()->isIAP())
//				{
//                    _closeButton->setEnabled(false);
//                    _product->payType = PRODUCT_PAY_TYPE_IAP;
//					startPayment(_product);
//				}
//				else
//				{
//					//showPayLayer(_product);
//				}
//#else
				showPayLayer(_product);
//#endif								
            }
		}
		break;
	default:
		break;
	}
}

void GameStoreLayer::showPayLayer(const PRODUCT_INFO* product)
{
	auto winSize = Director::getInstance()->getWinSize();
	//屏蔽后面的层
	colorLayer = LayerColor::create(Color4B(0,0,0,100));
	addChild(colorLayer,10);

	touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = [=](Touch* touch ,Event* event){
		return true;
	};
	touchListener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener,colorLayer);
	//支付框
	payLoader = CSLoader::createNode("platform/store/Node.csb");
	auto payLayout = (Layout*)payLoader->getChildByName("Panel_1");
	auto payLayoutSize = payLayout->getContentSize();
	auto payBtnClose= (Button*)payLayout->getChildByName("Button_close");
	auto payBtnWeixin= (Button*)payLayout->getChildByName("Button_weixin");
	auto payBtnZhifubao= (Button*)payLayout->getChildByName("Button_zhifubao");
	auto payBtnPPQianBao= (Button*)payLayout->getChildByName("Button_ppqianbao");	
	auto payBtnZhiFu= (Button*)payLayout->getChildByName("Button_zhifu");
	if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	{
		payBtnPPQianBao->setVisible(false);
		payBtnZhiFu->setPosition(payBtnPPQianBao->getPosition());
	}

	payBtnZhiFu->setUserData(const_cast<PRODUCT_INFO*>(product));
	payLoader->setPosition(winSize/2);
	payLoader->setScale(0);
	addChild(payLoader,10);
	payLoader->runAction(ScaleTo::create(0.2f,1.0f));

	// 仅显示支付宝支付
	//payBtnWeixin->setEnabled(false);
	//payBtnPPQianBao->setEnabled(false);
	//payBtnZhiFu->setEnabled(false);

	payBtnClose->addTouchEventListener(CC_CALLBACK_2(GameStoreLayer::closePayCallBack, this));
	payBtnWeixin->addTouchEventListener(CC_CALLBACK_2(GameStoreLayer::closePayCallBack, this));
	payBtnZhifubao->addTouchEventListener(CC_CALLBACK_2(GameStoreLayer::closePayCallBack, this));
	payBtnPPQianBao->addTouchEventListener(CC_CALLBACK_2(GameStoreLayer::closePayCallBack, this));
	payBtnZhiFu->addTouchEventListener(CC_CALLBACK_2(GameStoreLayer::closePayCallBack, this));

	char buffer[128];
	// 文字信息
	auto storeText = Text::create("", "", 25);
	storeText->setPosition(Vec2(payLayoutSize.width/4, payLayoutSize.height/2+90));
	sprintf(buffer, GBKToUtf8("商品: %lld"), product->number);
	storeText->setString(buffer);
	payLayout->addChild(storeText,10);

	// 价格信息
	auto priceText= Text::create("", "", 25);
	priceText->setPosition(Vec2(payLayoutSize.width*3/4,payLayoutSize.height/2+90));
	sprintf(buffer, GBKToUtf8("价格：%.2f元"), product->price);
	priceText->setString(buffer);
	payLayout->addChild(priceText, 10);
}

//支付回调
void GameStoreLayer::payCB(const std::string& args)
{
	if ((PRODUCT_PAY_TYPE_IAP == _product->payType) && PlatformConfig::getInstance()->isIAP())
	{
        _closeButton->setEnabled(true);
		std::string ext;
		bool ret = ProductManger::getInstance()->parsePayResult(args, ext);
		if (ret)
		{
			payCallback_IAP(ext);
		}
	}
	else
	{
		std::string ext;
		bool ret = ProductManger::getInstance()->parsePayResult(args);
		if (ret)
		{
			payCallback(ext);
		}
	}
}

void GameStoreLayer::payCallback(const std::string& data)
{
	std::string url = getOrder_URL(_orderId);

	HNHttpRequest::getInstance()->addObserver(this);
	HNHttpRequest::getInstance()->request("SDKGetMoney", HttpRequest::Type::GET, url);
}

//IAP支付回调
void GameStoreLayer::payCallback_IAP(const std::string& data)
{
	rapidjson::Document doc;
	doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());

	if (doc.HasParseError())
	{
		return;
	}

	// check code send to app store for server.
	std::string receiptData;
	if (doc.IsObject() && doc.HasMember("receipt-data"))
	{
		receiptData = doc["receipt-data"].GetString();
	}

	// orderid get from app store.
	std::string orderId;
	if(doc.IsObject() && doc.HasMember("order-id"))
	{
		orderId  = doc["order-id"].GetString();
		_orderId = orderId;
	}

	std::string requestData;

	// 类型
	requestData.append("type=PaySucceedByIOS");

	// 用户名
	requestData.append("&userName=");
	requestData.append(PlatformLogic()->loginResult.szName);

	// 验证数据
	requestData.append("&receipt_data=");
	requestData.append(receiptData);

	// 订单号
	requestData.append("&OrderID=");
	requestData.append(orderId);

	// 支付金额
	requestData.append("&payMoney=");
	char ch[20] = {0};
	sprintf(ch, "%lf", _product->price);
	requestData.append(ch);

	// 支付方式
	requestData.append("&typeInfoLog=IAP");

	log("send>%s", requestData.c_str());
    auto url_temp = PlatformConfig::getInstance()->getPayCallbackUrl_iOS().c_str();

	HNHttpRequest::getInstance()->addObserver(this);
	HNHttpRequest::getInstance()->request("IAPGetMoney", HttpRequest::Type::POST, url_temp, requestData);
}

void GameStoreLayer::onIAPGetMoneyCallback(const std::string& data)
{
	std::string cache = data;

	if (_product->payType == PRODUCT_PAY_TYPE_ZHIFU)//智付
	{
		if (checkOrder(cache) && !_notOfferOrderNo.empty())
		{
			payMoney();
		}
		return;
	}

	if (cache.empty())
	{
		supportAlert();
		return;
	}
	rapidjson::Document doc;
	doc.Parse<rapidjson::kParseDefaultFlags>(cache.c_str());

	if (doc.HasParseError())
	{
		supportAlert();
		return;
	}

	// 解析结果
	if (!doc.IsObject() || !doc.HasMember("rs") || !doc.HasMember("msg"))
	{
		supportAlert();
		return;
	}

	int rs = doc["rs"].GetInt();
	if (rs == 1)
	{
		if (doc.HasMember("WalletMoney"))
		{
			LLONG money = doc["WalletMoney"].GetUint64();
			char buffer[100];
			sprintf(buffer, "购买成功 , 获得%lld筹码！", _product->number);
			GamePromptLayer::create()->showPrompt(GBKToUtf8(buffer));
			PlatformLogic()->loginResult.i64Money = money;
			sprintf(buffer, "%lld", money);
			_selfCoin->setString(buffer);
			_delegate->walletChanged(money);
		}
		else
		{
			supportAlert();
		}

	}
	else
	{
		supportAlert();
	}
}


void GameStoreLayer::onSDKGetMoneyCallback(const std::string& data)
{
	if (data.empty())
	{
		log("data is empty");
		supportAlert();
		return;
	}

	rapidjson::Document doc;
	doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());

	if (doc.HasParseError())
	{
		log("data HasParseError");
		supportAlert();
		return;
	}

	// 解析结果
	if (!doc.IsObject() || !doc.HasMember("rs") || !doc.HasMember("msg"))
	{
		log("data HasParseError2");
		supportAlert();
		return;
	}

	int rs = doc["rs"].GetInt();
	if (rs == 1)
	{
		if (doc.HasMember("WalletMoney"))
		{
			LLONG money = atol(doc["WalletMoney"].GetString());
			char buffer[100];
			sprintf(buffer, "购买成功 , 获得%lld筹码！", _product->number);
			GamePromptLayer::create()->showPrompt(GBKToUtf8(buffer));
			PlatformLogic()->loginResult.i64Money = money;
			sprintf(buffer, "%lld", money);
			_selfCoin->setString(buffer);
			_delegate->walletChanged(money);
		}
		else
		{
			log("data HasParseError3");
			supportAlert();
		}
	}
	else
	{
		log("data HasParseError4");
		supportAlert();
	}
}

void GameStoreLayer::onAddOrderCallback(const std::string& data)
{	
	rapidjson::Document doc;
	doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
	if (!doc.IsObject())
	{
		GamePromptLayer::create()->showPrompt(GBKToUtf8("数据格式不对！"));
		return;
	}

	int rs = 0;
	if (JSON_RESOVE(doc, "rs")) rs = doc["rs"].GetInt();
	if (JSON_RESOVE(doc, "OrderID")) _orderId = doc["OrderID"].GetString();

	if (rs)
	{
		_product->orderID.assign(_orderId);
		if (PRODUCT_PAY_TYPE_ZHIFU == _product->payType)
		{
			executePayment_Dinpay(_product); //调用微信支付
			return;
		}
		startPayment(_product);
	}
}


void GameStoreLayer::onHttpResponse(const std::string& requestName, bool isSucceed, const std::string &responseData)
{
	if (requestName.compare("IAPGetMoney") == 0)
	{
		if (!isSucceed)
		{
			GamePromptLayer::create()->showPrompt(GBKToUtf8("查询失败！"));
			return;
		}

		onIAPGetMoneyCallback(responseData);
	}
	else if (requestName.compare("SDKGetMoney") == 0)
	{
		if (!isSucceed)
		{
			GamePromptLayer::create()->showPrompt(GBKToUtf8("查询失败！"));
			return;
		}

		onSDKGetMoneyCallback(responseData);
	}
	else if (requestName.compare("addOrder") == 0)
	{
		if (!isSucceed)
		{
			GamePromptLayer::create()->showPrompt(GBKToUtf8("查询失败！"));
			return;
		}
		onAddOrderCallback(responseData);
	}
}

void GameStoreLayer::supportAlert()
{
    char buffer[100] = {0};
    sprintf(buffer, "发货失败（%s），请联系客服。", _orderId.c_str());
    GamePromptLayer::create()->showPrompt(GBKToUtf8(buffer));
}

//关闭支付按钮回调
void GameStoreLayer::closePayCallBack(Ref* pSender, Widget::TouchEventType type)
{
	Button* selectedBtn = (Button*)pSender;
	auto seqPay = Sequence::create(ScaleTo::create(0.1f, 0.3f),CCCallFunc::create([&](){
		payLoader->removeFromParentAndCleanup(true);
		colorLayer->removeFromParentAndCleanup(true);
	}), nullptr);
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
	{	
		selectedBtn->setColor(Color3B(170, 170, 170));
		break;
	}
	case cocos2d::ui::Widget::TouchEventType::ENDED:
	{
		selectedBtn->setColor(Color3B(255, 255, 255));
		payLoader->runAction(seqPay);
		
		_product->payType = PRODUCT_PAY_TYPE_NONE;
		if (selectedBtn->getName().compare("Button_weixin") == 0)
			_product->payType = PRODUCT_PAY_TYPE_WEIXIN;
		else if (selectedBtn->getName().compare("Button_zhifubao") == 0)
			_product->payType = PRODUCT_PAY_TYPE_ZHIFUBAO;
		else if (selectedBtn->getName().compare("Button_ppqianbao") == 0)
		{
			_product->payType = PRODUCT_PAY_TYPE_PPQIANBAO;
			GamePromptLayer::create()->showPrompt(GBKToUtf8("需要提供商户信息"));
			break;
		}		
		else if (selectedBtn->getName().compare("Button_zifu") == 0)
		{
			_product->payType = PRODUCT_PAY_TYPE_ZHIFU;
			GamePromptLayer::create()->showPrompt(GBKToUtf8("需要提供商户信息"));
			break;
		}

		switch (_product->payType)
		{
		case PRODUCT_PAY_TYPE_NONE:
			break;
		case PRODUCT_PAY_TYPE_WEIXIN:
		{
			if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
			{
				GamePromptLayer::create()->showPrompt(GBKToUtf8("目前只支持支付宝，智付。"));
			}
			else
			{
				GamePromptLayer::create()->showPrompt(GBKToUtf8("目前只支持支付宝，PP钱包，智付。"));
			}
		}
		break;
		default:
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
			GamePromptLayer::create()->showPrompt(GBKToUtf8("支付只支持android和ios"));
#else
			executePayment(_product);
#endif
			break;
		}
	}
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
	{
		selectedBtn->setColor(Color3B(255, 255, 255));
		break;
	}
	default:
		break;
	}
}

//关闭按钮回调
void GameStoreLayer::closeEventCallBack(Ref* pSender, Widget::TouchEventType type)
{
	Button* selectedBtn = (Button*)pSender;
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		selectedBtn->setColor(Color3B(170, 170, 170));
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		selectedBtn->setColor(Color3B(255, 255, 255));
		closeStore();
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		selectedBtn->setColor(Color3B(255, 255, 255));
		break;
	default:
		break;
	}
}

bool GameStoreLayer::payEventSelector(HNSocketMessage* socketMessage)
{
	if (1 == socketMessage->messageHead.bHandleCode)
	{
		CCAssert(sizeof(MSG_GP_CS_NOTIFY_IOSPayResult) == socketMessage->objectSize, "MSG_GP_CS_NOTIFY_IOSPayResult is error.");
		MSG_GP_CS_NOTIFY_IOSPayResult* pIOSPayResult = (MSG_GP_CS_NOTIFY_IOSPayResult*)socketMessage->object;
		PlatformLogic()->loginResult.i64Money = pIOSPayResult->llMoney;
		RoomLogic()->loginResult.pUserInfoStruct.i64Money = pIOSPayResult->llMoney;

		if(_selfCoin)
		{
			_selfCoin->setText(StringUtils::format("%lld", pIOSPayResult->llMoney));
		}
		_delegate->walletChanged(pIOSPayResult->llMoney);

		if (nullptr != onCloseCallBack)
		{
			onCloseCallBack();
		}

		char buffer[256];
		sprintf(buffer, "购买成功 , 获得%lld筹码！", _product->number);
		GamePromptLayer::create()->showPrompt(GBKToUtf8(buffer));
		//支付类型设置为0
		_product->payType = PRODUCT_PAY_TYPE_NONE;
	}
	return true;
}

std::string GameStoreLayer::getCheckOrderParam()
{
	std::string data;	

	// 接口版本
	data.append("interface_version=V3.0");

	// 商家号
	data.append("&merchant_code=1111110166");

	// 商户网站唯一订单号
	data.append("&order_no=");
	data.append(_orderNo);

	// 业务类型
	data.append("&service_type=single_trade_query");

	std::string sign = data + "&key=123456789a123456789_";
	log("sign before>%s", sign.c_str());

	sign = MD5_CTX::MD5String(sign);
	log("sign after>%s", sign.c_str());	

	// 智付交易订单号
	data.append("&trade_no=");

	// 签名方式
	data.append("&sign_type=MD5");

	// 签名
	data.append("&sign=");
	data.append(sign);

	return data;
}

bool GameStoreLayer::checkOrder(std::string &result)
{
	log("checkOrder>%s", result.c_str());

	std::string is_success, sign, merchant_code, order_no, trade_status;
	bool bRet = parseCheckOrder(result, is_success, sign, merchant_code, order_no, trade_status);

	log("is_success>%s", is_success.c_str());
	log("sign>%s", sign.c_str());
	log("merchant_code>%s", merchant_code.c_str());
	log("order_no>%s", order_no.c_str());

	if(bRet)
	{
		if(_orderNo != order_no || _orderNo.empty() || order_no.empty() || is_success.empty())
		{
			GamePromptLayer::create()->showPrompt(GBKToUtf8("订单错误"));
			_orderNo.clear();
			return false;
		}

		_orderNo.clear();
		if(is_success.compare("T") != 0 && is_success.compare("F") != 0)
		{
			GamePromptLayer::create()->showPrompt(GBKToUtf8("订单查询未知错误"));
			return false;
		}

		if(is_success.compare("F") == 0)
		{
			GamePromptLayer::create()->showPrompt(GBKToUtf8("订单查询失败"));
			return false;
		}

		if(trade_status.empty())
		{
			GamePromptLayer::create()->showPrompt(GBKToUtf8("订单状态异常"));
			return false;
		}
		else if(trade_status.compare("SUCCESS") == 0)
		{
			_notOfferOrderNo = order_no;
			return true;
		}
		else if(trade_status.compare("FAILED") == 0)
		{
			GamePromptLayer::create()->showPrompt(GBKToUtf8("交易失败"));
			return false;
		}
		else if(trade_status.compare("UNPAY") == 0)
		{
			GamePromptLayer::create()->showPrompt(GBKToUtf8("未支付"));
			return false;
		}
		else
		{
			GamePromptLayer::create()->showPrompt(GBKToUtf8("订单状态未知错误"));
			return false;
		}		
	}
	else
	{
		GamePromptLayer::create()->showPrompt(GBKToUtf8("数据解析失败"));
		_orderNo.clear();
		return false;
	}	
}

bool GameStoreLayer::parseCheckOrder(const std::string result, std::string &is_success, std::string &sign, std::string& merchant_code, std::string& order_no, std::string& trade_status)
{
	bool bRet = false;
	do 
	{
		tinyxml2::XMLDocument *doc = new tinyxml2::XMLDocument();
		tinyxml2::XMLError error   = doc->Parse(result.c_str(), result.size());

		// 数据格式
		if(error != tinyxml2::XMLError::XML_SUCCESS)
		{
			break;
		}

		tinyxml2::XMLElement* dinpay = doc->FirstChildElement("dinpay");
		if(dinpay == nullptr)
		{
			break;
		}

		tinyxml2::XMLElement* response = dinpay->FirstChildElement("response");
		if(response == nullptr)
		{
			break;
		}

		// is_success
		tinyxml2::XMLElement* pRet = response->FirstChildElement("is_success");
		if(pRet == nullptr)
		{
			break;
		}
		is_success = pRet->GetText();
		bRet = true;

		// sign
		pRet = response->FirstChildElement("sign");
		if(pRet == nullptr)
		{
			break;
		}
		sign = pRet->GetText();

		tinyxml2::XMLElement* trade = response->FirstChildElement("trade");
		if(trade == nullptr)
		{
			break;
		}

		pRet = trade->FirstChildElement("merchant_code");
		if(pRet == nullptr)
		{
			break;
		}
		merchant_code = pRet->GetText();

		pRet = trade->FirstChildElement("order_no");
		if(pRet == nullptr)
		{
			break;
		}
		order_no = pRet->GetText();

		pRet = trade->FirstChildElement("trade_status");
		if(pRet == nullptr)
		{
			break;
		}
		trade_status = pRet->GetText();

	} while (0);

	return bRet;
}

void GameStoreLayer::payMoney()
{
	MSG_GP_CS_NOTIFY_IOSPayResult data = {0};
	data.productPrice = (UINT)_product->price;
	data.llMoney      = _product->number;
	data.userID       = PlatformLogic()->loginResult.dwUserID;
#if (HN_TAREGET_PLATFORM == HN_TARGET_PHONE_ONLY)
	data.uNameID      = GameCreator()->getCurrentGameNameID();
#endif
	strcpy(data.productSerialNumber, _product->identifier.c_str());
	PlatformLogic()->sendData(MDM_GP_NOTIFY_PAY, ASS_GP_NOTIFY_PAY, &data, sizeof(data), 
		HN_SOCKET_CALLBACK(GameStoreLayer::payEventSelector, this));
}

void GameStoreLayer::executePayment_Dinpay(PRODUCT_INFO* productInfo)
{
	if (nullptr == productInfo)
	{
		return;
	}

	time_t tt;
	time(&tt);			
	struct tm * now;
	//获得本地时间
	now = localtime(&tt);
	std::string dateTime = StringUtils::format("%d-%02d-%02d %02d:%02d:%02d", now->tm_year+1900,now->tm_mon+1,
		now->tm_mday,now->tm_hour,now->tm_min,now->tm_sec);

	std::string notify_url = StringUtils::format(NOTIFY_URL_DINPAY, getAPIServerUrl().c_str());

	std::string sign = StringUtils::format("interface_version=V3.0&merchant_code=%s&notify_url=%s", MECHANT_ID_DINPAY, notify_url.c_str());
	sign.append(StringUtils::format("&order_amount=%.2f", productInfo->price));
	sign.append(StringUtils::format("&order_no=%s", productInfo->orderID.c_str()));
	sign.append(StringUtils::format("&order_time=%s", dateTime.c_str()));
	sign.append("&product_name=");
	sign.append(GBKToUtf8("金币"));
	sign.append(StringUtils::format("&key=%s", MECHANT_KEY_DINPAY));

	auto md5_sign = MD5_CTX::MD5String(sign);

	productInfo->xmlFile = StringUtils::format("<?xml version=\"1.0\" encoding=\"utf-8\"?>"\
		"<dinpay><request><merchant_code>%s</merchant_code>"\
		"<notify_url>%s</notify_url>"\
		"<interface_version>V3.0</interface_version>"\
		"<sign_type>MD5</sign_type>"\
		"<sign>%s</sign>"\
		"<trade><order_no>%s</order_no>"\
		"<order_time>%s</order_time>"\
		"<order_amount>%.2f</order_amount>"\
		"<product_name>%s</product_name>"\
		"</trade></request></dinpay>", MECHANT_ID_DINPAY, notify_url.c_str(), md5_sign.c_str(), productInfo->orderID.c_str(), 
		dateTime.c_str(), productInfo->price/*0.1*/, GBKToUtf8("金币"));
    
    /*
     "<redo_flag></redo_flag>"\
     "<product_code></product_code>"\
     "<product_num></product_num>"\
     "<product_desc></product_desc>"\
     "<extra_return_param></extra_return_param>"\*/

	startPayment(productInfo);
}

void GameStoreLayer::executePayment(PRODUCT_INFO* productInfo)
{
	std::string url = addOrder_URL(PlatformLogic()->loginResult.szName, productInfo->price, getPayTypeInfo(productInfo->payType));

	HNHttpRequest::getInstance()->addObserver(this);
	HNHttpRequest::getInstance()->request("addOrder", HttpRequest::Type::GET, url);
}

void GameStoreLayer::startPayment(PRODUCT_INFO* productInfo)
{
	CALLBACK_PRAGMA pragma(HN_PAY_CALLBACK(GameStoreLayer::payCB, this));
	ProductManger::getInstance()->startPay(productInfo, &pragma);
}

std::string GameStoreLayer::addOrder_URL(const char* userName, DOUBLE price, std::string payTypeInfo)
{
	std::string url = PlatformConfig::getInstance()->getOrderUrl();
	url.append(StringUtils::format("type=AddOrder&userName=%s", userName));
	url.append(StringUtils::format("&payMoney=%.2f", price));
	url.append(StringUtils::format("&typeInfoLog=%s", payTypeInfo.c_str()));

	return url;
}

std::string GameStoreLayer::getOrder_URL(std::string orderID)
{
	std::string url = PlatformConfig::getInstance()->getOrderUrl();
	url.append(StringUtils::format("type=GetOrder&OrderID=%s", orderID.c_str()));

	return url;
}

std::string GameStoreLayer::getPayTypeInfo(INT payType)
{
	switch (payType)
	{
	case PRODUCT_PAY_TYPE_ZHIFUBAO:
		{
			return std::string("AliPay");
		}		
		break;
	case PRODUCT_PAY_TYPE_PPQIANBAO:
		{
			return std::string("PPWallet");
		}
		break;
	case PRODUCT_PAY_TYPE_ZHIFU:
		{
			return std::string("ZhiFu");
		}
		break;
	default:
		return "";
		break;
	}
}








