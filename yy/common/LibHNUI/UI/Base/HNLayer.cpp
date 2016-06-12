#include "HNLayer.h"
#include "HNLayerColor.h"

namespace HN 
{
	HNLayer::HNLayer(void) 
		: _colorLayer(nullptr)
		, _opacity(0)
	{
	}

	HNLayer::~HNLayer(void)
	{
	}

	bool HNLayer::init()
	{
		if (!Layer::init())
		{
			return false;
		}
		
		_winSize = Director::getInstance()->getWinSize();
		this->setAnchorPoint(Vec2(0.5f, 0.5f));

		return true;
	}

	void HNLayer::setBackGroundImage(const std::string& name)
	{
		
		auto visibleSize = Director::getInstance()->getVisibleSize();
		
		//背景
		auto loadingBG = Sprite::create(name.c_str());
	
		loadingBG->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
		
		float _xScale = visibleSize.width / loadingBG->getContentSize().width;
		float _yScale = visibleSize.height/ loadingBG->getContentSize().height;

		loadingBG->setScaleX(_xScale);
		loadingBG->setScaleY(_yScale);
		loadingBG->setName("BackGround");
		this->addChild(loadingBG, -1);
	
	}

	float HNLayer::getRealScaleX(int designWidth)
	{
		auto visibleSize = Director::getInstance()->getVisibleSize();
		return visibleSize.width / designWidth;
	}

	float HNLayer::getRealScaleY(int designHeight)
	{
		auto visibleSize = Director::getInstance()->getVisibleSize();
		return visibleSize.height / designHeight;
	}

	void HNLayer::enableKeyboard()
	{
		//对手机返回键的监听
		auto listener = EventListenerKeyboard::create();
		//和回调函数绑定
		listener->onKeyReleased = CC_CALLBACK_2(HNLayer::onKeyReleased, this);
		//添加到事件分发器中
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
	}

	bool HNLayer::switchToLayoutControl(Layout* layout, const std::string& controlName, Node* newControl)
	{
		auto tmp = (Text*)Helper::seekWidgetByName(layout, controlName);
		if (nullptr == tmp) return false;
		newControl->setPosition(tmp->getPosition());
		newControl->setTag(tmp->getTag());
		newControl->setLocalZOrder(tmp->getLocalZOrder());
		newControl->setScaleX(tmp->getScaleX());
		newControl->setScaleY(tmp->getScaleY());
		newControl->setAnchorPoint(tmp->getAnchorPoint());
		layout->addChild(newControl);
		return true;
	}

	void HNLayer::startShade(int opacity)
	{
		_opacity = opacity;
		if (nullptr == _colorLayer)
		{
			_colorLayer = HNLayerColor::create();
			addChild(_colorLayer, 1);
			schedule(CC_CALLBACK_1(HNLayer::updateOpacity, this), "update");
		}
	}

	void HNLayer::quicklyShade(int opacity)
	{
		_opacity = opacity;
		if (nullptr == _colorLayer)
		{
			_colorLayer = HNLayerColor::create();
			_colorLayer->setOpacity(0);
			addChild(_colorLayer, 1);			
		}
		_colorLayer->setOpacity(_opacity);
	}
    
    void HNLayer::releaseUnusedRes()
    {
        scheduleOnce(schedule_selector(HNLayer::releaseGameRes), 0.f);
    }
    
    
    void HNLayer::releaseGameRes(float delta)
    {
        AnimationCache::destroyInstance();
        SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
        Director::getInstance()->getTextureCache()->removeUnusedTextures();
    }

	void HNLayer::updateOpacity(float dt)
	{
		GLubyte i = _colorLayer->getOpacity();
		i = i + 3;

		_colorLayer->setOpacity(i);
		if (i >= _opacity)
		{
			unschedule("update");
		}
	}
}