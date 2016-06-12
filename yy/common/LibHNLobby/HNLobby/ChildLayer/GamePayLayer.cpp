#include "HNLobby/ChildLayer/GamePayLayer.h"
#include "HNLobby/PlatformDefine.h"
#include "HNLobby/PlatformConfig.h"
#include "HNMarketExport.h"

static const char* PAY_JSON_PATH	= "platform/pay/PayUI_1.json";					//                    
static const char* PAY_BG			= "platform/common_bg.png";						// backGround

GamePayLayer* GamePayLayer::createPaySelected(PRODUCT_INFO* product)
{
	GamePayLayer *pRet = new GamePayLayer(); 
	if (pRet && pRet->initWithProduct(product)) 
	{ 
		pRet->autorelease(); 
		return pRet; 
	}
	CC_SAFE_DELETE(pRet);
	return pRet;
}

GamePayLayer::GamePayLayer() : _opacity(0), _payUILayout(nullptr), _delegate(nullptr)
{
	memset(&_payUI, 0x0, sizeof(_payUI));
}

GamePayLayer::~GamePayLayer()
{
	
}

bool GamePayLayer::initWithProduct(PRODUCT_INFO* product)
{
	if (!HNLayer::init())
	{
		return false;
	}
	_product = *product;

	_colorLayer = HNLayerColor::create();
	_colorLayer->setTag(10);
	addChild(_colorLayer);

	Size winSize = Director::getInstance()->getWinSize();

	_payUILayout = dynamic_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile(PAY_JSON_PATH));
	_payUILayout->setAnchorPoint(Vec2(0.5, 0.5));
	_payUILayout->setPosition(Vec2(winSize.width/2, winSize.height/2));
	float _xScale = winSize.width / PlatformConfig::getInstance()->getPlatformDesignSize().width;
	float _yScale = winSize.height / PlatformConfig::getInstance()->getPlatformDesignSize().height;
	_payUILayout->setScale(_xScale, _yScale);
	addChild(_payUILayout, 2, 3);

	_payUIRect = _payUILayout->getBoundingBox();

	auto MyListener = EventListenerTouchOneByOne::create();
	//阻止触摸向下传递
	MyListener->setSwallowTouches(true);
	MyListener->onTouchBegan = [&](Touch* touch, Event* event)
	{
		auto target = dynamic_cast<GamePayLayer*>(event->getCurrentTarget());//获取的当前触摸的目标       
		Point locationInNode = target->convertToNodeSpace(touch->getLocation());
		Size s = target->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);
		if (rect.containsPoint(locationInNode))//判断触摸点是否在目标的范围内
		{
			if (_payUIRect.containsPoint(locationInNode)) return true;

			HNAudioEngine::getInstance()->playEffect(GAME_SOUND_CLOSE);
			auto size = Director::getInstance()->getWinSize();
			if (this->getPositionX() == 0)
			{
				auto winSize = Director::getInstance()->getWinSize();
				this->runAction(Sequence::create(EaseSineIn::create(MoveBy::create(0.3f, Vec2(-winSize.width, 0))),
					CCCallFunc::create(CC_CALLBACK_0(GamePayLayer::closeLayer, this)),
					RemoveSelf::create(true), nullptr));
			}
			return true;
		}
		else
		{
			return false;
		}

	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(MyListener, this);

	// 商品描述
	_payUI.Label_Product_Description = (Text*)Helper::seekWidgetByName(_payUILayout, "Label_Product_Description");
	if (nullptr != _payUI.Label_Product_Description)
	{
		//_payUI.Label_Product_Description->setString(GBKToUtf8(_product.description.c_str()));
	}

	// 商品价格
	_payUI.Label_Product_Price = (Text*)Helper::seekWidgetByName(_payUILayout, "Label_Product_Price");
	if (nullptr != _payUI.Label_Product_Price)
	{
		char buffer[32];
		sprintf(buffer, GBKToUtf8("%u 元"), _product.price);
		_payUI.Label_Product_Price->setString(buffer);
	}

	// 咨询电话
	_payUI.Label_Phone = (Text*)Helper::seekWidgetByName(_payUILayout, "Label_Phone");
	if (nullptr != _payUI.Label_Phone)
	{
		_payUI.Label_Phone->setString("");
	}

	// 关闭按钮
	_payUI.Button_Close = (Button*)Helper::seekWidgetByName(_payUILayout, "Button_Close");
	_payUI.Button_Close->addTouchEventListener(CC_CALLBACK_2(GamePayLayer::paySelectEventCallBack, this, PAY_UI::CLOSE_TAG));

	// 阿里支付按钮
	_payUI.Button_Alipay = (Button*)Helper::seekWidgetByName(_payUILayout, "Button_Alipay");
	_payUI.Button_Alipay->addTouchEventListener(CC_CALLBACK_2(GamePayLayer::paySelectEventCallBack, this, PAY_UI::ALIPAY_TAG));

	// 微信支付按钮
	_payUI.Button_WeChat = (Button*)Helper::seekWidgetByName(_payUILayout, "Button_WeChat");
	_payUI.Button_WeChat->addTouchEventListener(CC_CALLBACK_2(GamePayLayer::paySelectEventCallBack, this, PAY_UI::WECHAT_TAG));

	// 银联支付按钮
	_payUI.Button_UnionPay = (Button*)Helper::seekWidgetByName(_payUILayout, "Button_UnionPay");
	_payUI.Button_UnionPay->addTouchEventListener(CC_CALLBACK_2(GamePayLayer::paySelectEventCallBack, this, PAY_UI::UNIONPAY));

	return true;
}

void GamePayLayer::onEnter()
{
	HNLayer::onEnter();

	schedule(schedule_selector(GamePayLayer::updateOpacity), 0.01f);
}

void GamePayLayer::paySelectEventCallBack(Ref* pSender, Widget::TouchEventType type, int uiTag)
{
	if (Widget::TouchEventType::ENDED != type) return;
	switch (uiTag)
	{
	case PAY_UI::CLOSE_TAG:
		{
			HNAudioEngine::getInstance()->playEffect(GAME_SOUND_CLOSE);
			auto winSize = Director::getInstance()->getWinSize();
			this->runAction(Sequence::create(EaseSineIn::create(MoveBy::create(0.3f, Vec2(-winSize.width, 0))),
				CCCallFunc::create(CC_CALLBACK_0(GamePayLayer::closeLayer, this)),
				RemoveSelf::create(true), nullptr));
		}
		break;
	case PAY_UI::WECHAT_TAG: _delegate->onPayEvent(PaySelectedDelegate::WECHAT); break;
	case PAY_UI::ALIPAY_TAG: _delegate->onPayEvent(PaySelectedDelegate::ALIPAY); break;
	case PAY_UI::UNIONPAY: _delegate->onPayEvent(PaySelectedDelegate::UNIONPAY); break;
		{
			
		}
		break;
	default:
		break;
	}
}

void GamePayLayer::updateOpacity(float dt)
{
	_opacity = _opacity + 8;
	Layer* colorLayer = (Layer*)this->getChildByTag(10);
	colorLayer->setOpacity(_opacity);
	if (_opacity >= 100)
	{
		unschedule(schedule_selector(GamePayLayer::updateOpacity));
	}
}

void GamePayLayer::closeLayer()
{
	_delegate->onPayClose();
}