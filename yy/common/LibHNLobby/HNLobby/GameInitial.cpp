#include "HNLobby/GameInitial.h"
#include "HNLobby/PlatformConfig.h"
#include "HNLobby/GameMenu/GameMenu.h"

Scene* GameInitial::createScene()
{
    auto scene = Scene::create();
    auto layer = GameInitial::create();
    scene->addChild(layer);
    return scene;
}

void GameInitial::onEnterTransitionDidFinish()
{
	HNLayer::onEnterTransitionDidFinish();

	if(!_isCustomer)
	{
		showVideo();
	}
}

bool GameInitial::init()
{
    if ( !HNLayer::init())
    {
        return false;
    }

	Configuration::getInstance()->setValue("bControl", Value(false));
	Configuration::getInstance()->setValue("bEnterBackground", Value(false));

	do 
	{
		std::string filename = "config.json";
		if (FileUtils::getInstance()->isFileExist(filename))
		{
			std::string json = FileUtils::getInstance()->getStringFromFile(filename);
			rapidjson::Document doc;
			doc.Parse<rapidjson::kParseDefaultFlags>(json.c_str());
			if (doc.HasParseError())
			{
				break;
			}

			if (doc.IsObject() && doc.HasMember("isCustomer"))
			{
				_isCustomer = doc["isCustomer"].GetBool();
			}
		}
	} while (0);

	if (_isCustomer)
	{
		PlatformConfig::getInstance()->setSplashLogo("platform/logo.png");

		std::string logo = PlatformConfig::getInstance()->getSplashLogo();
		if (FileUtils::getInstance()->isFileExist(logo))
		{
			setBackGroundImage(logo);
			scheduleOnce(schedule_selector(GameInitial::waitting), 1.5f);
		}
		else
		{
			scheduleOnce(schedule_selector(GameInitial::waitting), 0.2f);
		}
	}
	else
	{
		setBackGroundImage("platform/video/animaiton/bird1.jpg");
	}

    return true;
}

void GameInitial::showVideo()
{
	Size size = Director::getInstance()->getVisibleSize();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	HNAudioEngine::getInstance()->playEffect("platform/video/audio.mp3");
	auto videoPlayer = cocos2d::experimental::ui::VideoPlayer::create();
	videoPlayer->setPosition(Point(size.width / 2, size.height / 2));
	videoPlayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	videoPlayer->setContentSize(Size(size.width, size.height));
	this->addChild(videoPlayer, 1);
	videoPlayer->setName("video");

	float scale = size.width / size.height;
	if (videoPlayer)
	{
		if (scale <= 1.5)
		{
			videoPlayer->setFileName("platform/video/launch_video_iphone.mp4");
		}
		else
		{
			videoPlayer->setFileName("platform/video/launch_video_iphone586.mp4");
		}
		videoPlayer->play();
		videoPlayer->setFullScreenEnabled(true);
	}
	videoPlayer->addEventListener(CC_CALLBACK_2(GameInitial::videoEventCallback, this));

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

	const int COUNT = 84;
	int namgTag[COUNT] = 
	{
		1,1,1,1,1,1,
		7,7,
		9,9,9,
		12, 12, 12,
		15, 15, 15,
		18, 18, 18,
		21, 21, 21,
		24, 24, 24,
		27, 27, 27,
		30, 30, 30,
		33, 33, 33,
		36, 36, 36,
		39, 39, 39,
		42, 42, 42,
		45, 45, 45,
		48, 48, 48,
		51, 51, 51,
		54, 54, 54,
		57, 57, 57,
		60, 60, 60,
		63, 63, 63,
		66, 66, 66,
		69, 69, 69,
		72, 72, 72,
		75,75,75,75,75,75,75,75,75,75
	};
	char name[64];
	auto birdAnimation = Animation::create();
	birdAnimation->setDelayPerUnit(0.05f);
	birdAnimation->setLoops(1);
	for(int i = 0; i < COUNT; i++)
	{
		sprintf(name, "platform/video/animaiton/bird%d.jpg",namgTag[i]);
		birdAnimation->addSpriteFrameWithFile(name);
	}

	HNAudioEngine::getInstance()->playEffect("platform/video/audio.mp3");
	auto  animate = Animate::create(birdAnimation);
	auto bird = Sprite::create();
	bird->setPosition(Vec2(size.width / 2, size.height / 2));
	bird->runAction(Sequence::create(animate, CallFunc::create(CC_CALLBACK_0(GameInitial::onPlayVideoOverCallback, this)), nullptr));
	this->addChild(bird);

#endif
}

/**
* 视频播放完成的回调函数
*/
void GameInitial::onPlayVideoOverCallback()
{
    Director::getInstance()->resume();
	GameMenu::createMenu();
}

/**
*  视频播放的状态
*  注意这里的代码，此处代码只有在android平台和Ios平台有效
*/
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
void GameInitial::videoEventCallback(Ref* sender, cocos2d::experimental::ui::VideoPlayer::EventType eventType)
{
	switch (eventType) 
	{
	case cocos2d::experimental::ui::VideoPlayer::EventType::PLAYING:
		break;
	case cocos2d::experimental::ui::VideoPlayer::EventType::PAUSED:
		break;
	case cocos2d::experimental::ui::VideoPlayer::EventType::STOPPED:
		break;
	case cocos2d::experimental::ui::VideoPlayer::EventType::COMPLETED:
	{
		Node* pNode = this->getChildByName("video");
		if(pNode != nullptr)
		{
			pNode->removeFromParent();
		}
		onPlayVideoOverCallback();
	}
		break;
	default:
		break;
	}
}
#endif

void GameInitial::waitting(float dt)
{
	GameMenu::createMenu();
}

GameInitial::GameInitial()
	: _isCustomer(false)
{
	
}

GameInitial::~GameInitial()
{

}