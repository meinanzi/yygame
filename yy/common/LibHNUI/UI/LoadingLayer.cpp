#include "LoadingLayer.h"
#include "Base/HNLayerColor.h"

namespace HN 
{
	static const int LOADING_LAYER_TAG		= 99999999;
	static const int LOADING_LAYER_ZORDER	= 99999999;


	LoadingLayer*  LoadingLayer::createLoading(Node* parent, const std::string& text, float fontSize, const std::string& image)
	{
		auto loading = (LoadingLayer*)parent->getChildByTag(LOADING_LAYER_TAG);;
		if (nullptr == loading)
		{
			loading = LoadingLayer::create();
			loading->setPosition(Size::ZERO);
			loading->createDialog(text, fontSize, image);
			parent->addChild(loading, LOADING_LAYER_ZORDER, LOADING_LAYER_TAG);
		}
		loading->setText(text);
		return loading;
	}

	LoadingLayer* LoadingLayer::createLoading(const std::string& text, float fontSize, const std::string& image, const std::string& image_bg)
	{
		auto loading = LoadingLayer::create();
		loading->createDialog(text, fontSize, image, image_bg);
		return loading;
	}

	void LoadingLayer::removeLoading(Node* parent)
	{
		CCAssert(nullptr != parent, "parent is null");
		parent->removeChildByTag(LOADING_LAYER_TAG);
	}

	//////////////////////////////////////////////////////////////////////////

	LoadingLayer::LoadingLayer()
		: _label(nullptr)
	{

	}

	LoadingLayer::~LoadingLayer()
	{

	}

	void LoadingLayer::createDialog(const std::string& text, float fontSize, const std::string& image)
	{
		// 转圈动画	
		auto sprite_load = Sprite::create();
		sprite_load->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height / 2 + sprite_load->getContentSize().width / 2));
		this->addChild(sprite_load, 2);
		sprite_load->runAction(RepeatForever::create(RotateBy::create(0.4f, 90.0f)));

		if (FileUtils::getInstance()->isFileExist(image))
		{
			sprite_load->setTexture(image);
			quicklyShade(200);
		}

		// 提示字样
		_label = Label::createWithSystemFont(text, "", fontSize);
		_label->setColor(Color3B(255, 215, 72));
		this->addChild(_label, 2);
		_label->setPosition(Vec2(this->getContentSize().width / 2, sprite_load->getPositionY() - sprite_load->getContentSize().height / 2 - 30));
	}

	void LoadingLayer::createDialog(const std::string& text, float fontSize, const std::string& image, const std::string& image_bg)
	{
		Sprite* loadingBox = Sprite::create(image_bg);
		loadingBox->setPosition(Vec2(_winSize.width / 2, _winSize.height / 2));
		addChild(loadingBox, 1);

		// 转圈动画	
		if (FileUtils::getInstance()->isFileExist(image))
		{			
			auto _loading = Sprite::create(image);
			_loading->setPosition(Vec2(loadingBox->getContentSize().width / 2, loadingBox->getContentSize().height*0.6f));
			loadingBox->addChild(_loading);
			_loading->runAction(RepeatForever::create(RotateBy::create(0.4f, 90.0f)));
		}

		// 提示字样
		_label = Label::createWithSystemFont(text, "", fontSize);
		_label->setColor(Color3B(255, 215, 72));
		_label->setPosition(Vec2(loadingBox->getContentSize().width / 2, loadingBox->getContentSize().height*0.2f));
		loadingBox->addChild(_label);

		startShade();
	}

	void LoadingLayer::setText(const std::string& text)
	{
		_label->setString(text);
	}

	bool LoadingLayer::init()
	{
		if (!HNLayer::init())
		{
			return false;
		}

		_listener = EventListenerTouchOneByOne::create();
		_listener->setSwallowTouches(true);
		_listener->onTouchBegan = [&](Touch* touch, Event* event)
		{
            return true;
//			auto target = static_cast<Sprite*>(event->getCurrentTarget());      
//			Point locationInNode = target->convertToNodeSpace(touch->getLocation());
//			Rect rect = Rect(0, 0, _winSize.width, _winSize.height);
//			return rect.containsPoint(locationInNode);
		};
		_eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);

		return true;
	}
    
    void LoadingLayer::setRemoveTimer(int timer, std::function<void()> removecall)
    {
        this->runAction(Sequence::create(
            DelayTime::create(timer),
            CallFunc::create([this, removecall]()
            {
                _listener->onTouchBegan = [this, removecall](Touch* touch, Event* event)
                {
                    removecall();
                    this->removeFromParent();
                    return true;
                };
            }),
            nullptr));
    }
}