#include "HNLobby/ChildLayer/GameSetLayer.h"
#include "HNLobby/PlatformDefine.h"
#include "HNLobby/PlatformConfig.h"

static const int COLOR_LAYER_TAG	= 10000;

GameSetLayer::GameSetLayer() : _layout(nullptr)
{

}

GameSetLayer::~GameSetLayer()
{

}

void GameSetLayer::showSet(Node* parent, int zorder, int tag)
{
	CCAssert(nullptr != parent, "parent is nullptr");
	if (0 != tag)
	{
		parent->addChild(this, zorder, tag);
	}
	else
	{
		parent->addChild(this, zorder);
	}
}

void GameSetLayer::closeSet()
{
	HNAudioEngine::getInstance()->playEffect(GAME_SOUND_CLOSE);
	colorLayer->removeFromParent();
	this->runAction(Sequence::create(ScaleTo::create(0.1f , 0.3f), CCCallFunc::create([&]()
	{
		this->removeFromParent();
	}), nullptr));

}
bool GameSetLayer::init()
{
	if ( !HNLayer::init()) return false;

	Size winSize = Director::getInstance()->getWinSize();

	//屏蔽后面的层
	colorLayer = LayerColor::create(Color4B(0, 0, 0, 100));
	addChild(colorLayer);

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = [=](Touch* touch ,Event* event){
		return true;
	};
	touchListener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener,colorLayer);

	settingLoader = CSLoader::createNode("platform/setting/settingNode.csb");
	settingLoader->setPosition(winSize/2);
	settingLoader->setScale(0);
	addChild(settingLoader);
	settingLoader->runAction(ScaleTo::create( 0.2f, 1));

	auto setLayout = (Layout*)settingLoader->getChildByName("Panel_1");
	auto settingClose = (Button*)setLayout->getChildByName("Button_Close");
	settingClose->addTouchEventListener(CC_CALLBACK_2(GameSetLayer::closeSetCallBack, this));

	auto user = UserDefault::getInstance();
    //音效滑动条
	effectSlider = (Slider*)setLayout->getChildByName("Slider_Effect");
	effectSlider->setPercent(user->getIntegerForKey(SOUND_VALUE_TEXT));
	effectSlider->addEventListener(Slider::ccSliderCallback(CC_CALLBACK_2(GameSetLayer::sliderCallback, this, E_SLIDER)));

    //音乐滑动条
    musicSlider = (Slider*)setLayout->getChildByName("Slider_Music");
	musicSlider->setPercent(user->getIntegerForKey(MUSIC_VALUE_TEXT));
    musicSlider->addEventListener(Slider::ccSliderCallback(CC_CALLBACK_2(GameSetLayer::sliderCallback, this, M_SLIDER)));

	//音效按钮
	 auto effectSmallBtn = (Button*)setLayout->getChildByName("Button_EffectSmall");
	 effectSmallBtn->addTouchEventListener(CC_CALLBACK_2(GameSetLayer::setCallBack, this, 1));
	 auto effectBigBtn = (Button*)setLayout->getChildByName("Button_EffectBig");
	 effectBigBtn->addTouchEventListener(CC_CALLBACK_2(GameSetLayer::setCallBack, this, 2));
	//音乐按钮
	 auto musicSmallBtn = (Button*)setLayout->getChildByName("Button_MusicSmall");
	 musicSmallBtn->addTouchEventListener(CC_CALLBACK_2(GameSetLayer::setCallBack, this, 3));
	 auto musicBigBtn = (Button*)setLayout->getChildByName("Button_MusicBig");
	 musicBigBtn->addTouchEventListener(CC_CALLBACK_2(GameSetLayer::setCallBack, this, 4));

	return true;
}

void GameSetLayer::setCallBack(Ref* pSender,Widget::TouchEventType type,int num)
{
	if (Widget::TouchEventType::ENDED != type) return;
	auto user = UserDefault::getInstance();
	switch (num)
	{
	case 1:
		effectSlider->setPercent(0);
		user->setIntegerForKey(SOUND_VALUE_TEXT,effectSlider->getPercent());
		HNAudioEngine::getInstance()->setSwitcjofEffect(false);
		HNAudioEngine::getInstance()->setEffectsVolume(0.0f);
		break;
	case 2:
		if (effectSlider->getPercent()==0)
		{
			effectSlider->setPercent(50);
			user->setIntegerForKey(SOUND_VALUE_TEXT, effectSlider->getPercent());
			HNAudioEngine::getInstance()->setSwitcjofEffect(true);
			HNAudioEngine::getInstance()->setEffectsVolume(0.5f);
		}
		break;
	case 3:
		musicSlider->setPercent(0);
		user->setIntegerForKey(MUSIC_VALUE_TEXT,musicSlider->getPercent());
		HNAudioEngine::getInstance()->setSwitchOfMusic(false);
		//HNAudioEngine::getInstance()->stopBackgroundMusic();
		HNAudioEngine::getInstance()->pauseBackgroundMusic();
		HNAudioEngine::getInstance()->setBackgroundMusicVolume(0.0f);
		break;
	case 4:
		if (musicSlider->getPercent()==0)
		{
			musicSlider->setPercent(50);
			user->setIntegerForKey(MUSIC_VALUE_TEXT,musicSlider->getPercent());
			HNAudioEngine::getInstance()->setSwitchOfMusic(true);
			HNAudioEngine::getInstance()->resumeBackgroundMusic();
			//HNAudioEngine::getInstance()->playBackgroundMusic(GAME_BACKGROUND_MUSIC, true);
			HNAudioEngine::getInstance()->setBackgroundMusicVolume(0.5f);
		}
		break;
	default:
		break;
	}
}

void GameSetLayer::closeSetCallBack(Ref* pSender, Widget::TouchEventType type)
{
	Button* selectedBtn = (Button*)pSender;
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		selectedBtn->setColor(Color3B(170, 170, 170));
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		selectedBtn->setColor(Color3B(255, 255, 255));
		closeSet();
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		selectedBtn->setColor(Color3B(255, 255, 255));
		break;
	default:
		break;
	}
}

void GameSetLayer::sliderCallback(Ref* pSender, Slider::EventType type, UI_TYPE num)
{
	Slider* pSlider = static_cast<Slider*>(pSender);
    float l = (float)(pSlider->getPercent()) / 100;
	auto user = UserDefault::getInstance();
    
	switch (num)
	{
	case M_SLIDER://音乐音量条
		{
			if (pSlider->getPercent() == 0)
			{
				HNAudioEngine::getInstance()->setSwitchOfMusic(false);
				HNAudioEngine::getInstance()->pauseBackgroundMusic();
			}
			else
			{
				HNAudioEngine::getInstance()->setSwitchOfMusic(true);
				HNAudioEngine::getInstance()->resumeBackgroundMusic();
			}
			HNAudioEngine::getInstance()->setBackgroundMusicVolume(l);
			user->setIntegerForKey(MUSIC_VALUE_TEXT, pSlider->getPercent());
	    }
		user->flush();
		break;
	case E_SLIDER://音效音量条
		{
			if (pSlider->getPercent() == 0)
			{
				HNAudioEngine::getInstance()->setSwitcjofEffect(false);
			}
			else
			{
				HNAudioEngine::getInstance()->setSwitcjofEffect(true);
			}
			HNAudioEngine::getInstance()->setEffectsVolume(l);
			user->setIntegerForKey(SOUND_VALUE_TEXT, pSlider->getPercent());
	    }
		user->flush();
		break;
	default:
		break;
	}
}


