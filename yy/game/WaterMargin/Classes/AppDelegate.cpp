#include "AppDelegate.h"

#include "WaterMarginMessageHead.h"
#include "WaterMarginGameTableUI.h"

#include "HNLobbyExport.h"

#include "json/rapidjson.h"
#include "json/document.h"

USING_NS_CC;

// 游戏设计尺寸
static cocos2d::Size designResolutionSize = cocos2d::Size(1280, 720);

// 更新授权码
static std::string APP_INFO_KEY("33000401");

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
	//set OpenGL context attributions,now can only set six attributions:
	//red,green,blue,alpha,depth,stencil
	GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

	GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
	return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
	// initialize director
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	if (!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
		Size size = getConfigSize();
		glview = GLViewImpl::createWithRect("Redbird", Rect(0, 0, 960, 640));
#else
		glview = GLViewImpl::create("Redbird");
#endif
		director->setOpenGLView(glview);
	}

	// turn on display FPS
	//director->setDisplayStats(true);

	// set FPS. the default value is 1.0/60 if you don't call this
	director->setAnimationInterval(1.0f / 60);

	// Set the design resolution
	glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::FIXED_WIDTH);
	Size frameSize = glview->getFrameSize();

	register_all_packages();

	initGameConfig();

	//auto scene = GameInitial::createScene();
	//director->runWithScene(scene);
	GameMenu::createMenu();

	return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
	//Director::getInstance()->pause();
	Director::getInstance()->stopAnimation();
	HNAudioEngine::getInstance()->pauseBackgroundMusic();

	// if you use SimpleAudioEngine, it must be pause
	//SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
	//Director::getInstance()->resume();
	Director::getInstance()->startAnimation();
	HNAudioEngine::getInstance()->resumeBackgroundMusic();

	// if you use SimpleAudioEngine, it must resume here
	//SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
Size AppDelegate::getConfigSize()
{
	Size size = designResolutionSize;
	do
	{
		std::string filename("config.json");
		if (FileUtils::getInstance()->isFileExist(filename))
		{
			std::string json = FileUtils::getInstance()->getStringFromFile(filename);
			rapidjson::Document doc;
			doc.Parse<rapidjson::kParseDefaultFlags>(json.c_str());
			if (doc.HasParseError() || !doc.IsObject())
			{
				break;
			}

			if (doc.HasMember("width"))
			{
				size.width = doc["width"].GetInt();
			}
			if (doc.HasMember("height"))
			{
				size.height = doc["height"].GetInt();
			}
		}
	} while (0);

	return size;
}

void AppDelegate::initGameConfig()
{
	FileUtils::getInstance()->addSearchPath("Games");
	//FileUtils::getInstance()->setXXTeaKey(getXXTEA_KEY());
	PlatformConfig::getInstance()->setGameLogo("game_logo.png");
	HNGameCreator::getInstance()->addGame(WaterMargin::Game_ID::NAME_ID, HNGameCreator::SINGLE, GAME_CREATE_SELECTOR(WaterMargin::GameTableUI::create));

	// 设置游戏授权码
	PlatformConfig::getInstance()->setAppKey(APP_INFO_KEY);

	// 平台设计尺寸
	PlatformConfig::getInstance()->setPlatformDesignSize(designResolutionSize);

	// 游戏设计尺寸
	PlatformConfig::getInstance()->setGameDesignSize(designResolutionSize);

	// 获取商品列表
	ProductManger::getInstance()->addProducts("mixproject");

}