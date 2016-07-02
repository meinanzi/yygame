#include "AppDelegate.h"

#include "HNLobbyExport.h"

#include "json/rapidjson.h"
#include "json/document.h"

/*******************************添加游戏****************************************/

// 斗地主
#include "LandlordMessageHead.h"
#include "LandlordGameTableUI.h"
    

// 金蟾捕鱼
#include "GoldenToadMessageHead.h"
#include "GoldenToadGameTableUI.h"

// 德州扑克
#include "DZPokerUpgradeMessage.h"
#include "DZPokerGameTableUI.h"

// 乐透炸金花
#include "GoldenFlowerMessageHead.h"
#include "GoldenFlowerGameTableUI.h"

// 拖拉机
#include "TRACTORMessageHead.h"
#include "TRACTORGameTableUI.h"


// 欢乐三十秒
#include "BJLMessageHead.h"
#include "BJLGameTableUI.h"


// 豪车飘移
#include "DriftingCarMessageHead.h"
#include "DriftingCarGameTableUI.h"

// 水浒传
#include "WaterMarginMessageHead.h"
#include "WaterMarginGameTableUI.h"

// 红中麻将
#include "ZYHZ_MessageHead.h"
#include "ZYHZ_GameTableUI.h"

// 血战麻将
#include "XZMJ_MessageHead.h"
#include "XZMJ_GameTableUI.h"
/******************************************************************************/

USING_NS_CC;

// 游戏设计尺寸
static cocos2d::Size designResolutionSize = cocos2d::Size(1280, 720);

// 更新授权码
static std::string APP_INFO_KEY("mixproject");

// 游戏选项
struct GameItem
{
	UINT nameId;
	HNGameCreator::GAMETYPE type;
	GAME_CREATE_SELECTOR create;
};

/*******************************添加游戏****************************************/
static GameItem GameList[] = 
{
	// 欢乐三十秒
	{ BJL::GAME_NAME_ID, HNGameCreator::NORMAL, BJL::GameTableUI::create },

	// 斗地主
	{ landlord::GAME_NAME_ID, HNGameCreator::NORMAL, landlord::GameTableUI::create },

	// 金蟾捕鱼
	{ GoldenToad::GAME_NAME_ID, HNGameCreator::NORMAL, GoldenToad::GameTableUI::create },

	// 德州扑克
	{ DZPoker::GAME_NAME_ID, HNGameCreator::NORMAL, DZPoker::GameTableUI::create },

	// 乐透炸金花
	{ goldenflower::GAME_NAME_ID, HNGameCreator::NORMAL, goldenflower::GameTableUI::create },

	// 血战麻将
	{ XZMJ::NAME_ID, HNGameCreator::NORMAL, XZMJ::GameTableUI::create },

	// 自由红中
	{ ZYHZ::NAME_ID, HNGameCreator::NORMAL, ZYHZ::GameTableUI::create },

	// 拖拉机
	{ TRACTOR::GAME_NAME_ID, HNGameCreator::NORMAL, TRACTOR::GameTableUI::create },

	// 豪车飘移
	{ DriftingCar::NAME_ID, HNGameCreator::NORMAL, DriftingCar::GameTableUI::create },

	// 水浒传
	{ WaterMargin::NAME_ID, HNGameCreator::NORMAL, WaterMargin::GameTableUI::create },

	
};
/******************************************************************************/

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
		glview = GLViewImpl::createWithRect("Ley", Rect(0, 0, size.width, size.height));
#else
		glview = GLViewImpl::create("Ley");
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

	auto userDefault = UserDefault::getInstance();
	userDefault->setBoolForKey("bEnterBackground", true);
	userDefault->flush();

	// if you use SimpleAudioEngine, it must be pause
	//SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
	//Director::getInstance()->resume();
	Director::getInstance()->startAnimation();
	HNAudioEngine::getInstance()->resumeBackgroundMusic();

	auto userDefault = UserDefault::getInstance();
	userDefault->setBoolForKey("bEnterBackground", false);
	userDefault->flush();

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
	FileUtils::getInstance()->setXXTeaKey(getXXTEA_KEY());
	PlatformConfig::getInstance()->setGameLogo("game_logo.png");

	// 添加游戏
	int gameCount = sizeof(GameList) / sizeof(GameItem);
	for (int i = 0; i < gameCount; i++)
	{
		HNGameCreator::getInstance()->addGame(GameList[i].nameId, GameList[i].type, GameList[i].create);
	}

	// 设置游戏授权码
	PlatformConfig::getInstance()->setAppKey(APP_INFO_KEY);

	// 平台设计尺寸
	PlatformConfig::getInstance()->setPlatformDesignSize(designResolutionSize);

	// 游戏设计尺寸
	PlatformConfig::getInstance()->setGameDesignSize(designResolutionSize);

	// 获取商品列表
	ProductManger::getInstance()->addProducts("mixproject");

}