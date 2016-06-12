#include "WaterMarginGameTableUI.h"
#include "WaterMarginGameTableLogic.h"
#include "WaterMarginGameRollImage.h"
#include "WaterMarginGameBiBei.h"
#include "WaterMarginGameMaLi.h"
#include "WaterMarginGameBDBtn.h"
#include "WaterMarginGameRule.h"
#include "WaterMarginGameUserBox.h"
#include "WaterMarginGameAudio.h"

#include "HNLobbyExport.h"
#include "HNUIExport.h"

static const int   TOTAL_AYNC_PNG_FILE = 33;
const char* AYNC_PNG_FILE[TOTAL_AYNC_PNG_FILE][2] = 
{
	{"WaterMargin/cliper/cliperImage.png",			"WaterMargin/cliper/cliperImage.plist"},
	{"WaterMargin/cliper/readyImage.png",			"WaterMargin/cliper/readyImage.plist"},
	{"WaterMargin/animation/BiBei/Boss/BossLost.pvr.ccz",			"WaterMargin/animation/BiBei/Boss/BossLost.plist"},
	{"WaterMargin/animation/BiBei/Boss/BossOpen.pvr.ccz",			"WaterMargin/animation/BiBei/Boss/BossOpen.plist"},
	{"WaterMargin/animation/BiBei/Boss/BossThrow.pvr.ccz",			"WaterMargin/animation/BiBei/Boss/BossThrow.plist"},
	{"WaterMargin/animation/BiBei/Boss/BossWait.pvr.ccz",			"WaterMargin/animation/BiBei/Boss/BossWait.plist"},
	{"WaterMargin/animation/BiBei/Boss/BossWin.pvr.ccz",			"WaterMargin/animation/BiBei/Boss/BossWin.plist"},
	{"WaterMargin/animation/BiBei/Right/RightLost.pvr.ccz",        "WaterMargin/animation/BiBei/Right/RightLost.plist"},
	{"WaterMargin/animation/BiBei/Right/RightWait.pvr.ccz",        "WaterMargin/animation/BiBei/Right/RightWait.plist"},
	{"WaterMargin/animation/BiBei/Right/RightWin.pvr.ccz",         "WaterMargin/animation/BiBei/Right/RightWin.plist"},
	{"WaterMargin/animation/BiBei/Left/LeftLost.pvr.ccz",          "WaterMargin/animation/BiBei/Left/LeftLost.plist"},
	{"WaterMargin/animation/BiBei/Left/LeftWait.pvr.ccz",          "WaterMargin/animation/BiBei/Left/LeftWait.plist"},
	{"WaterMargin/animation/BiBei/Left/LeftWin.pvr.ccz",           "WaterMargin/animation/BiBei/Left/LeftWin.plist"},
	{ "WaterMargin/animation/AllAnimation/AllAnimation0.pvr.ccz",     "WaterMargin/animation/AllAnimation/AllAnimation0.plist" },
	{ "WaterMargin/animation/AllAnimation/AllAnimation1.pvr.ccz", "WaterMargin/animation/AllAnimation/AllAnimation1.plist" },
	{ "WaterMargin/animation/AllAnimation/AllAnimation2.pvr.ccz", "WaterMargin/animation/AllAnimation/AllAnimation2.plist" },
	{ "WaterMargin/animation/AllAnimation/AllAnimation3.pvr.ccz", "WaterMargin/animation/AllAnimation/AllAnimation3.plist" },
	{ "WaterMargin/animation/AllAnimation/AllAnimation4.pvr.ccz", "WaterMargin/animation/AllAnimation/AllAnimation4.plist" },
	{ "WaterMargin/animation/AllAnimation/AllAnimation5.pvr.ccz", "WaterMargin/animation/AllAnimation/AllAnimation5.plist" },
	{ "WaterMargin/animation/AllAnimation/AllAnimation6.pvr.ccz", "WaterMargin/animation/AllAnimation/AllAnimation6.plist" },
	{ "WaterMargin/animation/AllAnimation/AllAnimation7.pvr.ccz", "WaterMargin/animation/AllAnimation/AllAnimation7.plist" },
	{ "WaterMargin/animation/AllAnimation/AllAnimation8.pvr.ccz", "WaterMargin/animation/AllAnimation/AllAnimation8.plist" },
	{ "WaterMargin/animation/AllAnimation/AllAnimation9.pvr.ccz", "WaterMargin/animation/AllAnimation/AllAnimation9.plist" },
	{ "WaterMargin/animation/AllAnimation/AllAnimation10.pvr.ccz", "WaterMargin/animation/AllAnimation/AllAnimation10.plist" },
	{ "WaterMargin/animation/AllAnimation/AllAnimation11.pvr.ccz", "WaterMargin/animation/AllAnimation/AllAnimation11.plist" },
	{ "WaterMargin/animation/AllAnimation/AllAnimation12.pvr.ccz", "WaterMargin/animation/AllAnimation/AllAnimation12.plist" },
	{ "WaterMargin/animation/AllAnimation/AllAnimation13.pvr.ccz", "WaterMargin/animation/AllAnimation/AllAnimation13.plist" },
	{ "WaterMargin/animation/AllAnimation/AllAnimation14.pvr.ccz", "WaterMargin/animation/AllAnimation/AllAnimation14.plist" },
	{ "WaterMargin/animation/yaoqi/yaoqi0.pvr.ccz",         "WaterMargin/animation/yaoqi/yaoqi0.plist" },
	{ "WaterMargin/animation/yaoqi/yaoqi1.pvr.ccz",         "WaterMargin/animation/yaoqi/yaoqi1.plist" },
	{ "WaterMargin/animation/yaoqi/yaoqi2.pvr.ccz",         "WaterMargin/animation/yaoqi/yaoqi2.plist" },
	{ "WaterMargin/animation/yaoqi/yaoqi3.pvr.ccz",         "WaterMargin/animation/yaoqi/yaoqi3.plist" },
	{ "WaterMargin/animation/yaoqi/yaoqi4.pvr.ccz",         "WaterMargin/animation/yaoqi/yaoqi4.plist" },
};
namespace WaterMargin
{

	GameTableUI::GameTableUI()
		:_tableBG(nullptr)
		,_gameRule(nullptr)
		, _gameAni(nullptr)
		,_line(-1)
		,_winScore(0)
		,_btnWidget(nullptr)
		,_stop(false)
		,_secondBG(nullptr)
		,_BiBei(nullptr)
		,_MaLi(nullptr)
		,_userMoney(0)
		,_menuBg(nullptr)
		,_loading(nullptr)
		,_notic(nullptr)
		,_desk('0')
		,_bool(false)
		,_addResources(0)
		,_loadingLayer(nullptr)
		,_leftDoor(nullptr)
		,_rightDoor(nullptr)
		,_LayerIndex(0)
		,_scoreChange(0)
		,_isReply(false)
		,_connetIndex(0)
		,_isFirst(true)
		,_btnPanel(nullptr)
	{
		memset(_imageType, 0, sizeof(_imageType));
		memset(_lineToImage, 0, sizeof(_lineToImage));
		_MaLiData.m_AddScore = 0;
		_MaLiData.m_GameCount = 0;
		_MaLiData.m_NowBegin = false;
		memset(_MaLiData.m_ThreeTypeId, 0, sizeof(_MaLiData.m_ThreeTypeId));
		_MaLiData.m_TypeIndex = 1000;

	}

	GameTableUI::~GameTableUI() 
	{
		CC_SAFE_DELETE(_tableLogic);
		CC_SAFE_DELETE(_gameRule);
		CC_SAFE_DELETE(_gameAni);
		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("WaterMargin/cliper/cliperImage.plist");
		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("WaterMargin/cliper/readyImage.plist");
		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("WaterMargin/animation/BiBei/Left/LeftLost.plist");
		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("WaterMargin/animation/BiBei/Left/LeftWait.plist");
		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("WaterMargin/animation/BiBei/Left/LeftWin.plist");
		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("WaterMargin/animation/BiBei/Right/RightLost.plist");
		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("WaterMargin/animation/BiBei/Right/RightWait.plist");
		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("WaterMargin/animation/BiBei/Right/RightWin.plist");
		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("WaterMargin/animation/BiBei//Boss/BossLost.plist");
		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("WaterMargin/animation/BiBei//Boss/BossOpen.plist");
		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("WaterMargin/animation/BiBei//Boss/BossThrow.plist");
		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("WaterMargin/animation/BiBei//Boss/BossWait.plist");
		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("WaterMargin/animation/BiBei//Boss/BossWin.plist");

		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("WaterMargin/animation/AllAnimation/AllAnimation0.plist");
		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("WaterMargin/animation/AllAnimation/AllAnimation1.plist");
		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("WaterMargin/animation/AllAnimation/AllAnimation2.plist");
		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("WaterMargin/animation/AllAnimation/AllAnimation3.plist");
		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("WaterMargin/animation/AllAnimation/AllAnimation4.plist");
		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("WaterMargin/animation/AllAnimation/AllAnimation5.plist");
		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("WaterMargin/animation/AllAnimation/AllAnimation6.plist");
		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("WaterMargin/animation/AllAnimation/AllAnimation7.plist");
		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("WaterMargin/animation/AllAnimation/AllAnimation8.plist");
		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("WaterMargin/animation/AllAnimation/AllAnimation9.plist");
		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("WaterMargin/animation/AllAnimation/AllAnimation10.plist");
		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("WaterMargin/animation/AllAnimation/AllAnimation11.plist");
		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("WaterMargin/animation/AllAnimation/AllAnimation12.plist");
		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("WaterMargin/animation/AllAnimation/AllAnimation13.plist");
		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("WaterMargin/animation/AllAnimation/AllAnimation14.plist");
		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("WaterMargin/animation/yaoqi/yaoqi0.plist");
		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("WaterMargin/animation/yaoqi/yaoqi1.plist");
		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("WaterMargin/animation/yaoqi/yaoqi2.plist");
		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("WaterMargin/animation/yaoqi/yaoqi3.plist");
		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("WaterMargin/animation/yaoqi/yaoqi4.plist");
	}

	GameTableUI* GameTableUI::create(BYTE bDeskIndex, bool bAutoCreate)
	{
		GameTableUI* tableUI = new GameTableUI();
		if (tableUI->init(bDeskIndex, bAutoCreate))
		{
			tableUI->autorelease();
			return tableUI;
		}
		else
		{
			CC_SAFE_DELETE(tableUI);
			return nullptr;
		}
	}

	bool GameTableUI::init(BYTE bDeskIndex, bool bAutoCreate)
	{
		if (!HNLayer::init())
		{
			return false;
		}

		_desk = bDeskIndex;
		_bool = bAutoCreate;
		loading();

		return true;
	}

	void GameTableUI::loading()
	{
		auto winSize = Director::getInstance()->getWinSize();

		// 加载界面
		_loadingLayer = CSLoader::createNode("WaterMargin/tableui/loading.csb");
		CCAssert(_loadingLayer != nullptr, "null.");
		this->addChild(_loadingLayer);
		_loadingLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		_loadingLayer->setPosition(Vec2(winSize.width / 2, winSize.height / 2));

		// 背景图片
		auto panel = dynamic_cast<Node*>(_loadingLayer->getChildByName("Panel_2"));
		CCAssert(panel != nullptr, "null.");

		auto image_bg = dynamic_cast<ImageView*>(panel->getChildByName("bg"));
		CCAssert(image_bg != nullptr, "null.");
		image_bg->setScale(winSize.width / image_bg->getContentSize().width, winSize.height / image_bg->getContentSize().height);

		// 进度条
		_loading = dynamic_cast<LoadingBar*>(image_bg->getChildByName("ProgressBar"));
		CCAssert(_loading != nullptr, "null.");
		_loading->setPercent(1.0f);

		char strPlist[128] = { 0 };
		char strPng[128] = { 0 };
		auto textureCache = Director::getInstance()->getTextureCache();
        Texture2D::setDefaultAlphaPixelFormat(Texture2D::PixelFormat::RGBA4444);
		for (int i = 0; i < TOTAL_AYNC_PNG_FILE; i++)
		{
			textureCache->addImageAsync(AYNC_PNG_FILE[i][0], CC_CALLBACK_1(GameTableUI::loadingCallback, this, AYNC_PNG_FILE[i][1]));
		}

	}

	void GameTableUI::loadSrcUpdate(float delta)
	{
		/*if (_addResources >= TOTAL_AYNC_PNG_FILE)
		{
		_addResources = 0;
		unschedule(schedule_selector(GameTableUI::loadSrcUpdate));
		return;
		}
		auto textureCache = Director::getInstance()->getTextureCache();
		textureCache->addImageAsync(AYNC_PNG_FILE[_addResources][0] , CC_CALLBACK_1(GameTableUI::loadingCallback , this , AYNC_PNG_FILE[_addResources][1]));
		_addResources++;*/
	}

	void GameTableUI::loadingCallback(Texture2D* textureData, std::string plist)
	{
		auto spriteFrameCache = SpriteFrameCache::getInstance();
		spriteFrameCache->addSpriteFramesWithFile(plist, textureData);

		float value = _loading->getPercent();
		value += 3.0f;

		_loading->setPercent(value);

		if (value >= 100)
		{
			loadingAni();
		}
	}

	void GameTableUI::loadingAni()
	{
		_gameAni = new GameAnimation();
		_gameAni->onRender();
		//将动画加到缓存中
		for (int i = 0; i < 11; i++)
		{
			_gameAni->addAniData(_gameAni->_aniNameFirst[i], 1, i);
		}
		for (int i = 0; i < 9; i++)
		{
			_gameAni->addAniData(_gameAni->_aniNameSecond[i], 2, i);
		}
		for (int i = 0; i < 3; i++)
		{
			_gameAni->addAniData(_gameAni->_aniRight[i], 3, i);
			_gameAni->addAniData(_gameAni->_aniLeft[i], 4, i);
		}
		for (int i = 0; i < 5; i++)
		{
			_gameAni->addAniData(_gameAni->_aniBoss[i], 5, i);
		}

		gameINIT();
	}

	void GameTableUI::addLoadding(float delay)
	{
		/*float value = _loading->getPercent();
		value += 11;
		
		_loading->setPercent(value);*/
	}

	void GameTableUI::gameINIT()
	{
		if(nullptr != _loadingLayer)
		{
			_loadingLayer->removeFromParent();
			_loadingLayer = nullptr;
		}		
		
		initData();
		loadMaLiBtnPanel();
		enableKeyboard();

		_tableLogic = new GameTableLogic(this, _desk, _bool);
		_tableLogic->sendGameInfo();

		//头像信息
		auto head = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(_btnWidget, "Button_head"));
		head->setVisible(true);
		bool isBoy = PlatformLogic()->loginResult.bBoy;
		char str[40] = {0};
		sprintf(str, "%s", isBoy? "WaterMargin/useImage/men_head.png":"WaterMargin/useImage/women_head.png");
		head->loadTextures(str, str, str);
		head->addTouchEventListener(CC_CALLBACK_2(GameTableUI::headCallBack, this));	
	}

	void GameTableUI::initData()
	{
		GameAudio::playBackground();
		GameAudio::setBackgroundMusicVolume(80);
		GameAudio::setEffectsVolume(50);
		auto winSize = Director::getInstance()->getWinSize();

		char name[64] = {0};

		auto node = CSLoader::createNode("WaterMargin/tableui/tableui.csb");
		auto tableWidget = (Layout*)node->getChildByName("Panel_14");
		tableWidget->setAnchorPoint(Vec2(0.5f, 0.5f));
		tableWidget->setPosition(Vec2(winSize.width/2,winSize.height/2));
		addChild(node);
		
		auto winData = dynamic_cast<TextAtlas*>(tableWidget->getChildByName("win"));
		winData->setName("winData");
		winData->setString("");
		winData->setLocalZOrder(1000);

		//返回按钮
		auto quit = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(tableWidget, "quit"));
		quit->setPosition(Vec2(640-winSize.width/2+50, 360+winSize.height/2-50));
		quit->addTouchEventListener(CC_CALLBACK_2(GameTableUI::menuClickCallback, this));
		quit->setTag(Tag_QUIT);

		//主背景
		_tableBG = dynamic_cast<ImageView*>(ui::Helper::seekWidgetByName(tableWidget, "bg"));
		auto size = _tableBG->getCustomSize();
		_tableBG->setScale(winSize.width/size.width, winSize.height/size.height);

		_notic = dynamic_cast<ImageView*>(ui::Helper::seekWidgetByName(_tableBG, "notic"));

		auto btnNode = CSLoader::createNode("WaterMargin/tableui/btn.csb");
		_btnWidget = (Layout*)btnNode->getChildByName("Panel_7");
		_btnWidget->setAnchorPoint(Vec2(0.5f, 0.5f));
		_btnWidget->setPosition(Vec2(winSize.width/2, 40));
		addChild(btnNode);

		//8个按钮
		
		for (int i = 0; i < 9; i++)
		{
			sprintf(name, "btn%d", i+1);
			auto btn = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(_btnWidget, name));
			btn->setVisible(true);
			btn->setLocalZOrder(100);
			if (i != 5 && i != 8)
			{
				btn->setBright(false);
				btn->setEnabled(false);
			}
			if (i == 5 || i == 8)
			{
				btn->setVisible(false);
			}
			btn->addTouchEventListener(CC_CALLBACK_2(GameTableUI::menuClickCallback, this));	
			_btn.push_back(btn);
		}
		
		//裁剪区域白色背景
		auto white = dynamic_cast<ImageView*>(ui::Helper::seekWidgetByName(_tableBG, "white"));
		white->setLocalZOrder(-10);
		white->setVisible(true);

		//5个显示数据
		for (int i = 0; i < 5; i++)
		{
			sprintf(name, "data_%d", i+1);
			auto data = dynamic_cast<TextAtlas*>(ui::Helper::seekWidgetByName(_tableBG, name));
			data->setString("0");
			_dataText.push_back(data);
		}
		_dataText[2]->setString("9");
		btnRule();

		auto yaoqi = dynamic_cast<Text*>(ui::Helper::seekWidgetByName(_tableBG, "yaoqi"));
		sprintf(name, "ani_%d_%d", 1, 9);
		auto animation = dynamic_cast<Animation*>(AnimationCache::getInstance()->getAnimation(name));
		auto animate = Animate::create(animation);
		auto ani = Sprite::create();
		ani->setScale(0.99f, 0.80f);
		ani->runAction(RepeatForever::create(animate));
		ani->setAnchorPoint(Vec2(0, 0));
		ani->setPosition(Vec2(0, 0));
		yaoqi->addChild(ani, 2);

		//15个裁剪层
		for (int i = 0; i < 15; i++)
		{
			sprintf(name, "cliper%d", i+1);
			Layout* clipLayer = dynamic_cast<Layout*>(ui::Helper::seekWidgetByName(_tableBG, name));
			clipLayer->setLocalZOrder(-5);
			clipLayer->setName(name);
		}
		

		//滚动图数量初始化
		_rollImage.reserve(15);
		_readyImage.reserve(15);
		_blackImage.reserve(15);
		_Frame.reserve(15);

		
		for (int i = 0; i < 15; i++)
		{
			//结果图数据初始化
			_imageType[i] = 1;

			sprintf(name, "cliper%d", i+1);
			auto cliper = _tableBG->getChildByName(name);
			auto size = cliper->getContentSize();
			//裁剪区域结果图初始化
			auto image = RollImage::create("image1x.png");
			auto isize = image->getContentSize();
			cliper->addChild(image);
			image->setPosition(Vec2(size.width/2, size.height/2));
			image->setScale(size.width/isize.width, size.height/isize.height);
			_blackImage.push_back(image);
			image->setVisible(false);

			//第一图框初始化
			auto sprite = ImageView::create("WaterMargin/useImage/frame.png");
			auto ssize = sprite->getContentSize();
			_tableBG->addChild(sprite, 10);
			auto point = cliper->getPosition();
			sprite->setPosition(Vec2(point.x+size.width/2, point.y+size.height/2));

			sprite->setVisible(false);
			_Frame.push_back(sprite);
		}

		//游戏规则运算
		_gameRule = new GameRule();

		lineToImageFunction();
		createImage();
		startRoll();
		_isReply = true;

		//比倍、得分图片切换初始化
		 auto bibei = Sprite::create("WaterMargin/useImage/bibei.png");
		 addChild(bibei, 800);
		 bibei->setPosition(Vec2(winSize.width/2, winSize.height*5/8));
		 auto defeng = Sprite::create("WaterMargin/useImage/defeng.png");
		 addChild(defeng, 800);
		 defeng->setPosition(Vec2(winSize.width/2, winSize.height*5/8));
		 bibei->setName("bibei");
		 defeng->setName("defeng");
		 bibei->setVisible(false);
		 defeng->setVisible(false);

		 auto win = Sprite::create("WaterMargin/useImage/win.png");
		 addChild(win, 800);
		 win->setPosition(Vec2(winSize.width/2-250, winSize.height/2));
		 win->setName("win");
		 win->setVisible(false);
	}

	void GameTableUI::setNotic(float delay)
	{
		if (_notic->getTag() == 103)
		{
			_notic->loadTexture("WaterMargin/useImage/13.png");
			_notic->setTag(44);
		}
		else
		{
			_notic->loadTexture("WaterMargin/useImage/14.png");
			_notic->setTag(103);
		}
	}

	void GameTableUI::FirstResult()
	{
		//判断断网数据初始化
		_isReply = false;
		_connetIndex = 0;

		if (!_MaLiData.m_NowBegin && _MaLiData.m_TypeIndex != 1000)		return;
		_MaLiData.m_NowBegin = false;
		if (_winScore > 0 && !_MaLiData.m_NowBegin)
		{
			Director::getInstance()->getTextureCache()->removeAllTextures();

			GameAudio::playSHZWin();
			//显示图片切换与按钮
			changeBegin();
			setBtnVisible(Tag_WIN, true);
			setBtnVisible(Tag_BIBEI, true);
			//显示赢的数据
			auto win = dynamic_cast<Sprite*>(this->getChildByName("win"));
			win->setVisible(true);
			setWinData();
			setDataText(_winScore, 2);

			if (_btn[8]->isVisible() && atoi(_dataText[0]->getString().c_str()) >= atoi(_dataText[4]->getString().c_str()))
			{
				this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([&](){_tableLogic->sendGetWinMessage(0);}),nullptr));
			}
			controlBtnVisible(true);
		}
		else
		{
			auto quit = dynamic_cast<Button*>(_tableBG->getParent()->getChildByTag(Tag_QUIT));
			quit->setVisible(true);
			quit->setEnabled(true);
			_btn[0]->setBright(true);
			_btn[0]->setEnabled(true);

			if (_btn[8]->isVisible())
			{
				if (atoi(_dataText[0]->getString().c_str()) >= atoi(_dataText[4]->getString().c_str()))
				{
					_tableLogic->sendStartRollMessage();
					startRoll();
				}
				else
				{
					_btn[2]->setVisible(true);
					_btn[8]->setVisible(false);
					_btn[2]->setEnabled(false);
					_btn[8]->setEnabled(false);
					_btn[2]->setBright(false);
					_notic->loadTexture("WaterMargin/useImage/15.png");
					_notic->setVisible(true);
					_notic->setTag(103);
				}
			}
			else
			{
				if (atoi(_dataText[0]->getString().c_str()) >= atoi(_dataText[4]->getString().c_str()) && atoi(_dataText[4]->getString().c_str()) > 0)
				{
					setBtnVisible(Tag_START, true);
					schedule(schedule_selector(GameTableUI::setNotic), 0.6f);
				}
				
			}

			if (_isFirst)
			{
				_isFirst = false;
				return;
			}
			controlBtnVisible(false);
		}

	}

	void GameTableUI::viewWinGold()
	{
		auto win = dynamic_cast<Sprite*>(this->getChildByName("win"));
		win->setVisible(true);
		setWinData();
		int value = atoi(_MaLi->_data[1]->getString().c_str());
		value += _winScore;
		char name[20] = {0};
		sprintf(name, "%d", value);
		_MaLi->_data[1]->setString(name);		
	}

	void GameTableUI::hideWinGold()
	{
		auto win = dynamic_cast<Sprite*>(this->getChildByName("win"));
		win->setVisible(false);
		auto winData = dynamic_cast<TextAtlas*>(_tableBG->getParent()->getChildByName("winData"));
		winData->setString("");
	}

	void GameTableUI::setWinData()
	{
		auto winData = dynamic_cast<TextAtlas*>(_tableBG->getParent()->getChildByName("winData"));
		char name[10] = {0};
		sprintf(name, "%d", _winScore);
		winData->setString(name);
	}

	void GameTableUI::setWinScore(int value)
	{
		_winScore = value;
		
		if (_MaLi)
		{
			_MaLiData.m_AddScore = value;
		}
	}

	void GameTableUI::changeBegin()
	{
        
		schedule(schedule_selector(GameTableUI::BibeiChangeDefeng), 0.8f);
	}

	void GameTableUI::changeOver()
	{
		
		unschedule(schedule_selector(GameTableUI::BibeiChangeDefeng));
		auto bibei = dynamic_cast<Sprite*>(this->getChildByName("bibei"));
		auto defeng = dynamic_cast<Sprite*>(this->getChildByName("defeng"));
		bibei->setVisible(false);
		defeng->setVisible(false);
	}

	void GameTableUI::BibeiChangeDefeng(float delay)
	{
		auto bibei = dynamic_cast<Sprite*>(this->getChildByName("bibei"));
		auto defeng = dynamic_cast<Sprite*>(this->getChildByName("defeng"));
		if (bibei->isVisible() && !(defeng->isVisible()))
		{
			bibei->setVisible(false);
			defeng->setVisible(true);
			return;
		}
		else
		{
			bibei->setVisible(true);
			defeng->setVisible(false);
			return;
		}
		bibei->setVisible(true);
	}

	void GameTableUI::setImageData(int* str)
	{
		_blackImage.clear();
		for (int i = 0; i < 15; i++)
		{
			_imageType[i] = str[i] + 1;
			char name[30] = {0};
			sprintf(name, "cliper%d", i+1);
			auto cliper = _tableBG->getChildByName(name);
			auto size = cliper->getContentSize();
			//结果图数据转换
			sprintf(name, "image%dx.png", _imageType[i]);
			auto image = RollImage::create(name);
			auto isize = image->getContentSize();
			cliper->addChild(image);
			image->setPosition(Vec2(size.width/2, size.height/2));
			image->setScale(size.width/isize.width, size.height/isize.height);
			_blackImage.push_back(image);
			image->setVisible(false);
		}
	}

	void GameTableUI::setDataText(int value, int num)
	{
		char name[20] = {0};
		sprintf(name, "%d", value);
		_dataText[num-1]->setString(name);
	}

	void GameTableUI::EnterMaLi()
	{
		
		auto size = Director::getInstance()->getWinSize();

		_rightDoor = Sprite::create("WaterMargin/useImage/you.png");
		_leftDoor = Sprite::create("WaterMargin/useImage/zuo.png");
		_tableBG->addChild(_rightDoor, 400);
		_tableBG->addChild(_leftDoor, 400);

		_rightDoor->setPosition(Vec2(1600, 360));		
		_leftDoor->setPosition(Vec2(-320, 360));

		_btnWidget->setVisible(false);
		_btnWidget->setEnabled(false);

		auto yAction = MoveBy::create(1.0f, Vec2(-640, 0));
		auto zAction = MoveBy::create(1.0f, Vec2(640, 0));
		auto sequ = Sequence::create(DelayTime::create(1.0f),
			CallFunc::create(CC_CALLBACK_0(GameTableUI::createMaLi, this)),
			DelayTime::create(0.5f),
			RemoveSelf::create(true),
			nullptr);
		_rightDoor->runAction(Spawn::create(yAction, sequ, nullptr));
		_leftDoor->runAction(Sequence::create(zAction, DelayTime::create(0.5f), RemoveSelf::create(true), nullptr));
	}

	void GameTableUI::createMaLi()
	{
		auto size = Director::getInstance()->getWinSize();
		if (_MaLi)
		{
			_MaLi->removeFromParentAndCleanup(true);
			_MaLi = nullptr;
		}

		GameAudio::playMaLiBackground();

		_MaLi = GameMaLi::create(_tableLogic, _tableBG->getParent());
		_MaLi->setAnchorPoint(Vec2(0.5, 0.5));
		_MaLi->setPosition(Vec2(0, 0));
		_tableBG->getParent()->addChild(_MaLi, 2);

		_MaLi->_data[0]->setString(_dataText[0]->getString());
		_MaLi->_data[1]->setString(_dataText[1]->getString());
		_MaLi->_data[2]->setString(_dataText[4]->getString());

		_MaLi->_Count = _MaLiData.m_GameCount;
		_MaLi->_imageIndex = _MaLiData.m_TypeIndex;
		for (int i = 0; i < 4; i++)
		{
			_MaLi->_imageType[i] = _MaLiData.m_ThreeTypeId[i];
		}
	}

	void GameTableUI::MaLiEndBB()
	{
		//_bdBtn->_widget->setVisible(false);
		changeLayer();
	}
	void GameTableUI::MaLIEndDF()
	{
		//_bdBtn->_widget->setVisible(false);
		changeOver();
		_scoreChange = _winScore/10;
		GameAudio::playDefeng();

		schedule(schedule_selector(GameTableUI::MaLiGetScore), 0.1f);
	}

	void GameTableUI::MaLiResult(bool begin, int MaLicount, int Index, int* str)
	{

		_MaLiData.m_NowBegin = begin;
		_MaLiData.m_GameCount = MaLicount;
		_MaLiData.m_TypeIndex = Index;
		for (int i = 0; i < 4; i++)
		{
			_MaLiData.m_ThreeTypeId[i] = str[i];
		}

		if (_MaLi != nullptr)
		{
			if (begin && Index == 1000)
			{
				JudgeMaLi();
			}
			else if (begin && Index != 1000 && str[0])
			{
				_MaLi->_Count = _MaLiData.m_GameCount;
				_MaLi->_imageIndex = _MaLiData.m_TypeIndex;
				for (int i = 0; i < 4; i++)
				{
					_MaLi->_imageType[i] = _MaLiData.m_ThreeTypeId[i];

					_MaLi->_rollImage[i]->setRun(true);
					_MaLi->_readyImage[i]->setRun(true);
				}
				GameAudio::playGundong();
				hideWinGold();
				_MaLi->playgo();
			}
		}
	}

	void GameTableUI::JudgeConnect()
	{
		_isReply = true;
	}

	void GameTableUI::JudgeMaLi()
	{
		if (atoi(_MaLi->_data[1]->getString().c_str()) <= 0)
		{
			if (_MaLi != nullptr)
			{
				_MaLi->removeFromParentAndCleanup(true);
				_MaLi = nullptr;
			}


			GameAudio::playBackground();

			_btnWidget->setVisible(true);
			_btnWidget->setEnabled(true);
			auto winData = dynamic_cast<TextAtlas*>(_tableBG->getParent()->getChildByName("winData"));
			winData->setString("");
			auto win = dynamic_cast<Sprite*>(this->getChildByName("win"));
			win->setVisible(false);
		}
		else
		{
			changeBegin();
			auto winData = dynamic_cast<TextAtlas*>(_tableBG->getParent()->getChildByName("winData"));
			winData->setString(_MaLi->_data[1]->getString());
			
			showMaLiBDbtn();
		}
	}

	void GameTableUI::loadMaLiBtnPanel()
	{
		auto btnNode = CSLoader::createNode("WaterMargin/tableui/bibei.csb");
		_btnPanel = (Layout*)btnNode->getChildByName("Panel_17");
		_btnPanel->setAnchorPoint(Vec2(0.5, 0.5));
		_btnPanel->setPosition(Vec2(640, 200));
		this->addChild(btnNode, 800);

		//比倍、得分按钮
		char name[30] = {0};
		for (int i = 1; i < 3; i++)
		{
			sprintf(name, "Button_%d", i);
			auto btn = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(_btnPanel, name));
			btn->addTouchEventListener(CC_CALLBACK_2(GameTableUI::maLiClickCallback, this));
		}

		_btnPanel->setVisible(false);
	}

	void GameTableUI::showMaLiBDbtn()
	{
		_btnPanel->setVisible(true);	
	}

	void GameTableUI::maLiClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype)
	{
		if (!(_btnPanel->isVisible()))			return;
		if (Widget::TouchEventType::ENDED != touchtype)	return;
		_btnPanel->setVisible(false);

		Button* ptr = (Button*)pSender;
		switch (ptr->getTag())
		{
		case 39:
			_tableLogic->changeMaLiToBB();
			break;
		case 37:
			_tableLogic->sendGetWinMessage(0);
			_tableLogic->changeMaLiToDF();
			break;
		default:
			break;
		}
	}

	void GameTableUI::changeLayer()
	{
		//停止图片切换
		changeOver();
		auto winData = dynamic_cast<TextAtlas*>(_tableBG->getParent()->getChildByName("winData"));
		winData->setString("");
		auto win = dynamic_cast<Sprite*>(this->getChildByName("win"));
		win->setVisible(false);

		_btnWidget->setVisible(false);

		_rightDoor = Sprite::create("WaterMargin/useImage/you.png");
		_leftDoor = Sprite::create("WaterMargin/useImage/zuo.png");
		_tableBG->addChild(_rightDoor, 400);
		_tableBG->addChild(_leftDoor, 400);


		_rightDoor->setPosition(Vec2(1600, 360));		
		_leftDoor->setPosition(Vec2(-320, 360));
		
		_rightDoor->runAction(MoveBy::create(1.0f, Vec2(-640, 0)));
			
		_leftDoor->runAction(MoveBy::create(1.0f, Vec2(640, 0)));

		Action* sequ = nullptr;
		if (!_LayerIndex)
		{
			_LayerIndex = 1;
		}
		else
		{
			_LayerIndex = 0;
		}
		sequ = Sequence::create(DelayTime::create(1.5f), CallFunc::create(
			CC_CALLBACK_0(GameTableUI::asyncLoading, this)), nullptr);

		this->runAction(sequ);
	}

	void GameTableUI::asyncLoading()	
	{
		_rightDoor->removeFromParent();
		_leftDoor->removeFromParent();
		StartBiBei();
	}

	void GameTableUI::StartBiBei()
	{
		if (_LayerIndex)		
		{
			SetLayer();
		}
		else
		{
			if (_BiBei)
			{
				_BiBei->removeFromParentAndCleanup(true);
				_BiBei = nullptr;
			}
			_btnWidget->setVisible(true);
			_btnWidget->setEnabled(true);
			setBtnVisible(Tag_START, true);
            auto quit = dynamic_cast<Button*>(_tableBG->getParent()->getChildByTag(Tag_QUIT));
            quit->setVisible(true);
            quit->setEnabled(true);
            
		}
	}

	void GameTableUI::SetLayer()
	{
		auto size = Director::getInstance()->getWinSize();
		GameAudio::playBiBeiBg();
		
		if (_BiBei)
		{
			_BiBei->removeFromParentAndCleanup(true);
			_BiBei = nullptr;
		}

		_BiBei = GameBiBei::create(_tableLogic, _tableBG);
		_BiBei->setAnchorPoint(Vec2(0.5, 0.5));
		_BiBei->setPosition(Vec2(0, 0));
		addChild(_BiBei,1);
		_tableLogic->sendBibeiBtnMessage();

		if (_MaLi != nullptr)
		{
			_MaLi->removeFromParentAndCleanup(true);
			_MaLi = nullptr;
		}
		_btnWidget->setVisible(false);
		_btnWidget->setEnabled(false);

		_BiBei->_Bdata[0]->setString(_dataText[0]->getString());
		_BiBei->_Bdata[1]->setString(_dataText[1]->getString());
		_BiBei->_Bdata[2]->setString(_dataText[4]->getString());
	}

	void GameTableUI::createImage()
	{
		char name[64] = {0};
		for (int i = 0; i < 15; i++)
		{
			sprintf(name, "cliper%d", i+1);
			auto cliper = _tableBG->getChildByName(name);
			cliper->ignoreAnchorPointForPosition(false);
			auto size = cliper->getContentSize();
			for (int j = 0; j < 2; j++)
			{
				if (j == 0)
				{
					int num = CCRANDOM_0_1()*9 + 1;
					sprintf(name,"image%d.png", num);
					auto image = RollImage::create(name);
					cliper->addChild(image);
					image->setPosition(Vec2(size.width/2, size.height/2));
					auto rsize = image->getContentSize();
					image->setScale(size.width/rsize.width, size.height/rsize.height);
					_rollImage.push_back(image);
				}
				else
				{
					int num = CCRANDOM_0_1()*9 + 1;
					sprintf(name,"image%d.png", num);
					auto image = RollImage::create(name);
					cliper->addChild(image);
					image->setPosition(Vec2(size.width/2, size.height*3/2));
					auto rsize = image->getContentSize();
					image->setScale(size.width/rsize.width, size.height/rsize.height);
					_readyImage.push_back(image);
				}
				
			}
			
		}
	}

	void GameTableUI::startRoll()
	{
		if (!_stop)	GameAudio::playGundong();
		//赢数据归0
		setDataText(0, 2);
		//滚动图片属性重置
		for (int i = 0; i < 15; i++)
		{
			_rollImage[i]->setRun(true);
			_rollImage[i]->setVisible(true);
			_readyImage[i]->setRun(true);
			_readyImage[i]->setVisible(true);
			_blackImage[i]->setVisible(false);
		}
		schedule(schedule_selector(GameTableUI::Rolling), 0.01f);
		this->runAction(Sequence::create(DelayTime::create(1.0f),CallFunc::create([&](){schedule(schedule_selector(GameTableUI::queueStop), 0.2f);}),nullptr));
	
		controlBtnVisible(false);
	}

	void GameTableUI::stopRoll()
	{
		_btn[4]->setVisible(true);
		_btn[5]->setVisible(false);
		_btn[4]->setEnabled(false);
		_btn[5]->setEnabled(false);
		_btn[4]->setBright(false);

		unschedule(schedule_selector(GameTableUI::Rolling));
		unschedule(schedule_selector(GameTableUI::queueStop));
		//滚动图片处理
		for (int i = 0; i < 15; i++)
		{
			_rollImage[i]->setVisible(false);
			_blackImage[i]->setVisible(true);
		}
		//第二动画数据清空
		_playAnimate.clear();
		//判断9线
		schedule(schedule_selector(GameTableUI::JudgeRule), 0.8f);

	}

	void GameTableUI::Rolling(float delay)
	{
		for (int i = 0; i < 15; i++)
		{
			
			if (_rollImage[i]->getRun())			_rollImage[i]->imageRun();
			if (_readyImage[i]->getRun())		_readyImage[i]->imageRun();
		}
	}


	void GameTableUI::queueStop(float delay)
	{
		if (!_isReply)	
		{			
			_connetIndex++;
			if (_connetIndex >= 75)
			{
				GamePromptLayer::create()->showPrompt(GBKToUtf8("网络已断开连接"));
				leaveDesk();
			}
			return;
		}

		char name[30] = {0};
		int i = 0;
		for (; i < 15; i++)
		{
			if (_rollImage[i]->getRun() && _readyImage[i]->getRun())
			{
				_rollImage[i]->setRun(false);
				_readyImage[i]->setRun(false);
				sprintf(name, "image%d.png", _imageType[i]); 
				_rollImage[i]->imageStopAndSetFrame(name);
				_readyImage[i]->resetReadyImage();
				if (!_stop)		break;
			}
		}
		if (i == 15)
		{
			stopRoll();
		}
	}

	void GameTableUI::JudgeRule(float delay)
	{
		_line++;
		for (; _line < 9; _line++)
		{
			_gameRule->_userObject.reset();
			for (int j = 0; j < 5; j++)
			{
				_gameRule->_userObject._data[j] = _imageType[_lineToImage[_line][j]];
			}
			_gameRule->_userObject._line = _line;
			_gameRule->_userObject.WinRule();
			if (_gameRule->_userObject._value)
			{
				GameAudio::playFirstAnimate();
				useRule();
				break;
			}
		}

		if (_line == 9)
		{
			_line = -1;
			unschedule(schedule_selector(GameTableUI::JudgeRule));
			if (_playAnimate.size() == 0)
			{
				FirstResult();
			}
			else
			{
				playSecondAnimate();
			}
			
			//第一图框隐藏
			for (int i = 0; i < 15; i++)
			{
				if (_Frame[i]->isVisible())
				{
					_Frame[i]->setVisible(false);
				}
			}
		}
	}

	void GameTableUI::useRule()
	{
		for (int i = 0; i < 15; i++)
		{
			if (_Frame[i]->isVisible())
			{
				_Frame[i]->setVisible(false);
			}
		}

		int line = _gameRule->_userObject._line;
		int first = _gameRule->_userObject._firstImage;
		int last = _gameRule->_userObject._lastImage;
		for (int i = first; i <= last; i++)
		{
			_Frame[_lineToImage[line][i]]->setVisible(true);
			playFirstAnimate(_lineToImage[line][i]);
			auto start = dynamic_cast<Button*>(_btnWidget->getChildByTag(Tag_START));
			start->setBright(false);
		}
	}

	

	void GameTableUI::playFirstAnimate(int value)
	{
		//记录播放数据		
		auto it = _playAnimate.begin();
		for (; it != _playAnimate.end(); it++)
		{
			if(*it == value)	break;
		}
		if (it == _playAnimate.end())
		{
			_playAnimate.push_back(value);
		}

		//第一动画播放
		char name[30] = {0};
		sprintf(name, "cliper%d", value+1);
		auto cliper = _tableBG->getChildByName(name);

		sprintf(name, "ani_%d_%d", 1, _imageType[value] - 1);
		auto animation = dynamic_cast<Animation*>(AnimationCache::getInstance()->getAnimation(name));
		if (animation == nullptr)
		{
			_gameAni->addAniData(_gameAni->_aniNameFirst[_imageType[value] - 1], 1, _imageType[value] - 1);
			auto animation = dynamic_cast<Animation*>(AnimationCache::getInstance()->getAnimation(name));
		}
		
		auto animate = Animate::create(animation);
		auto ani = Sprite::create();
		ani->setScale(1.08f, 0.816f);
		ani->setAnchorPoint(Vec2(0, 0));
		ani->setPosition(Vec2(0, 0));
		cliper->addChild(ani, 2);
		ani->runAction(Spawn::create(animate, Sequence::create(DelayTime::create(2.7f), RemoveSelf::create(), nullptr),nullptr));
	
	}

	void GameTableUI::playSecondAnimate()
	{
		if (_playAnimate.size() == 0) return;
		for (auto it = _playAnimate.begin(); it != _playAnimate.end(); it++)
		{
			char name[30] = {0};
			sprintf(name, "cliper%d", *it+1);
			auto cliper = _tableBG->getChildByName(name);

			GameAudio::playSecondAnimate(_imageType[*it]);

			sprintf(name, "ani_%d_%d", 2, _imageType[*it] - 1);
			auto animation = dynamic_cast<Animation*>(AnimationCache::getInstance()->getAnimation(name));

			auto animate = Animate::create(animation);
			auto ani = Sprite::create();
			ani->setScale(1.08f, 0.816f);
			ani->setAnchorPoint(Vec2(0, 0));
			ani->setPosition(Vec2(0, 0));
			cliper->addChild(ani, 2);

			ani->runAction(Spawn::create(animate, Sequence::create(DelayTime::create(2.7f),
				CallFunc::create(CC_CALLBACK_0(GameTableUI::FirstResult, this)),
				RemoveSelf::create(),
				nullptr), nullptr));

			sprintf(name, "ani_%d_%d", 1, 10);
			auto animationFrame = dynamic_cast<Animation*>(AnimationCache::getInstance()->getAnimation(name));

			auto animateFrame = Animate::create(animationFrame);
			auto posFrame = _Frame[*it]->getPosition();
			auto aniFrame = Sprite::create();
			aniFrame->setScale(1.175, 0.882);
			aniFrame->setAnchorPoint(Vec2(0.5, 0.5));
			aniFrame->setPosition(Vec2(posFrame.x - 5, posFrame.y));
			_tableBG->addChild(aniFrame, 2);
			aniFrame->runAction(animateFrame);

			aniFrame->runAction(Spawn::create(animateFrame, Sequence::create(DelayTime::create(2.7f),
				RemoveSelf::create(),
				nullptr), nullptr));

			_rollImage[*it]->setVisible(true);
			_blackImage[*it]->setVisible(false);
		}
		
		//玛丽游戏判定
		if (!_MaLiData.m_NowBegin && _MaLiData.m_TypeIndex != 1000)
		{
			this->runAction(Sequence::create(DelayTime::create(2.7f), 
				CallFunc::create(CC_CALLBACK_0(GameTableUI::EnterMaLi, this)), nullptr));

		}
	}

	void GameTableUI::lineToImageFunction()
	{
		//第1线
		for (int i = 0; i < 5; i++)
		{
			_lineToImage[0][i] =3*i+1;
		}
		//第2线
		for (int i = 0; i < 5; i++)
		{
			_lineToImage[1][i] =3*i;
		}
		//第3线
		for (int i = 0; i < 5; i++)
		{
			_lineToImage[2][i] =3*i+2;
		}
		//第4线
		_lineToImage[3][0] = 0;
		_lineToImage[3][1] = 4;
		_lineToImage[3][2] = 8;
		_lineToImage[3][3] = 10;
		_lineToImage[3][4] = 12;
		//第5线
		_lineToImage[4][0] = 2;
		_lineToImage[4][1] = 4;
		_lineToImage[4][2] = 6;
		_lineToImage[4][3] = 10;
		_lineToImage[4][4] = 14;
		
		//第6线
		_lineToImage[5][0] = 0;
		_lineToImage[5][1] = 3;
		_lineToImage[5][2] = 7;
		_lineToImage[5][3] = 9;
		_lineToImage[5][4] = 12;

		//第7线
		_lineToImage[6][0] = 2;
		_lineToImage[6][1] = 5;
		_lineToImage[6][2] = 7;
		_lineToImage[6][3] = 11;
		_lineToImage[6][4] = 14;
		
		//第8线
		_lineToImage[7][0] = 1;
		_lineToImage[7][1] = 5;
		_lineToImage[7][2] = 8;
		_lineToImage[7][3] = 11;
		_lineToImage[7][4] = 13;
		
		//第9线
		_lineToImage[8][0] = 1;
		_lineToImage[8][1] = 3;
		_lineToImage[8][2] = 6;
		_lineToImage[8][3] = 9;
		_lineToImage[8][4] = 13;
	}

	void GameTableUI::setBtnVisible(int Tag, bool visible)
	{
		Button* btn = dynamic_cast<Button*>(_btnWidget->getChildByTag(Tag));
		
		if (Tag == Tag_START)
		{
			btn->setVisible(visible);
			if (visible)
			{
				btn->setBright(true);
				btn->setEnabled(true);				
			}
			auto quit = dynamic_cast<Button*>(_tableBG->getParent()->getChildByTag(Tag_QUIT));
			quit->setVisible(visible);
			quit->setEnabled(visible);

			auto Auto = dynamic_cast<Button*>(_btnWidget->getChildByTag(Tag_AUTO));
			Auto->setBright(visible);
			Auto->setEnabled(visible);
			auto  Add = dynamic_cast<Button*>(_btnWidget->getChildByTag(Tag_ADD));
			Add->setBright(visible);
			Add->setEnabled(visible);
			auto stop = dynamic_cast<Button*>(_btnWidget->getChildByTag(Tag_ALLSTOP));
			stop->setVisible(!visible);
			stop->setEnabled(!visible);
			return;
		}
		if (Tag == Tag_ALLSTOP)
		{
			btn->setVisible(visible);
			btn->setEnabled(visible);
			auto start = dynamic_cast<Button*>(_btnWidget->getChildByTag(Tag_START));
			start->setVisible(!visible);
			if (!visible)
			{
				start->setEnabled(false);
				start->setBright(false);
			}
			else
			{
				start->setEnabled(!visible);
			}			
			return;
		}

		btn->setBright(visible);
		btn->setEnabled(visible);

		btnRule();

	}

	void GameTableUI::btnRule()
	{
		//没钱处理
		if (_dataText[4] <= 0)
		{
			_btn[0]->setBright(true);
			_btn[0]->setEnabled(true);
			for (int i = 1; i < 9; i++)
			{
				if (i == 8 || i == 5)
				{
					_btn[i]->setVisible(false);
				}
				else
				{
					_btn[i]->setBright(false);
					_btn[i]->setEnabled(false);
				}
			}
		}	

		//自动状态
		else if (_btn[8]->isVisible())
		{
			for (int i = 0; i < 8; i++)
			{
				_btn[i]->setVisible(true);
				if (i == 2 || i == 5)
				{
					_btn[i]->setVisible(false);
				}
				else
				{
					_btn[i]->setBright(false);
					_btn[i]->setEnabled(false);
				}
				
			}
		}
		auto stop = dynamic_cast<Button*>(_btnWidget->getChildByTag(Tag_ALLSTOP));
		if (stop->isVisible())
		{
			auto zidong = dynamic_cast<Button*>(_btnWidget->getChildByTag(Tag_AUTO));
			zidong->setBright(false);
		}
		auto start = dynamic_cast<Button*>(_btnWidget->getChildByTag(Tag_START));
		if (start->isBright())
		{
			auto zidong = dynamic_cast<Button*>(_btnWidget->getChildByTag(Tag_AUTO));
			zidong->setBright(true);
		}
	}

	void GameTableUI::setSaiZi(int s1, int s2)
	{
		_BiBei->viewSaiZi(s1, s2);
	}

	void GameTableUI::setMoney(LLONG money)
	{
		_userMoney = money;
	}

	void GameTableUI::headCallBack(cocos2d::Ref* pSender, Widget::TouchEventType touchtype)
	{
		if (Widget::TouchEventType::ENDED != touchtype)	return;
		auto btn = (Button*)pSender;
		btn->setBright(true);
		auto size = Director::getInstance()->getWinSize();
		int userID = PlatformLogic()->loginResult.dwUserID;
		auto userBox = GameUserMessageBox::createMessageBox(this, userID);
		userBox->setPosition(Vec2(size.width/2, size.height/2));
		char name[20] = {0};
		sprintf(name, "%lld", _userMoney);
		userBox->_iMoney->setString(name);
	}

	void GameTableUI::menuClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype)
	{
		if (Widget::TouchEventType::ENDED != touchtype)	return;
		auto bibei = dynamic_cast<Sprite*>(this->getChildByName("bibei"));
		auto defeng = dynamic_cast<Sprite*>(this->getChildByName("defeng")); 
		GameAudio::playShangfeng();
		Button* ptr = (Button*)pSender;
		switch (ptr->getTag())
		{
		case Tag_UP:
			schedule(schedule_selector(GameTableUI::setNotic), 0.6f);
			_tableLogic->sendUpOrDownMessage(true);
			break;
		case Tag_DOWN:
			_tableLogic->sendUpOrDownMessage(false);
			_btn[4]->setBright(false);
			_btn[4]->setEnabled(false);
			_btn[2]->setBright(false);
			_btn[2]->setEnabled(false);
			_btn[5]->setVisible(false);
			break;
		case Tag_ADD:
			_tableLogic->sendAddNoteMessage();
			break;
		case Tag_START:
			if (atoi(_dataText[0]->getString().c_str()) < atoi(_dataText[4]->getString().c_str()))
			{
				return;
			}
			_stop = false;
			unschedule(schedule_selector(GameTableUI::setNotic));
			_notic->loadTexture("WaterMargin/useImage/11.png");
			setBtnVisible(Tag_START, false);
			_tableLogic->sendStartRollMessage();
			startRoll();
			break;
		case Tag_WIN:
			setBtnVisible(Tag_BIBEI, false);
			setBtnVisible(Tag_WIN, false);
			_tableLogic->sendGetWinMessage(0);
			break;
		case Tag_ALLSTOP:
			setBtnVisible(Tag_ALLSTOP, false);
			_notic->loadTexture("WaterMargin/useImage/12.png");
			_stop = true;
			break;
		case Tag_BIBEI:
			changeLayer();
			break;
		case Tag_AUTO:
			if (atoi(_dataText[0]->getString().c_str()) < atoi(_dataText[4]->getString().c_str()))
			{
				return;
			}
			_btn[8]->setVisible(true);
			_btn[2]->setVisible(false);
			btnRule();
			//开始按钮效果
			_stop = false;
			if (_winScore > 0)
			{
				_btn[8]->setEnabled(false);
				_btn[8]->setBright(false);
				if (bibei->isVisible() || defeng->isVisible())
				{
					setBtnVisible(Tag_BIBEI, false);
					_tableLogic->sendGetWinMessage(0);
				}
			}
			else
			{
				_tableLogic->sendStartRollMessage();
				startRoll();
			}
			break;
		case Tag_HANDLE:
			_btn[8]->setVisible(false);
			_btn[2]->setVisible(true);
			break;
		case Tag_QUIT:
			_tableLogic->SendStandUp();
			break;
		default:
			break;
		}
	}

	//设置自动手动按钮的状态
	void GameTableUI::controlBtnVisible(bool visible)
	{
		_btn[2]->setEnabled(visible);
		_btn[2]->setBright(visible);
		_btn[8]->setEnabled(visible);
		_btn[8]->setBright(visible);
	}

	void GameTableUI::getWinScore()
	{
		changeOver();
		_scoreChange = _winScore/ 10;
		schedule(schedule_selector(GameTableUI::changeScore), 0.1f);
	}

	void GameTableUI::MaLiGetScore(float delay)
	{
		
		INT value = atoi(_MaLi->_data[0]->getString().c_str());
		if (_winScore > 0)
		{
			_winScore -= _scoreChange;
			setWinData();

			value += _scoreChange;
			char name[20] = {0};
			sprintf(name, "%d", value);
			_MaLi->_data[0]->setString(name);
		}

		if (_winScore == 0)
		{
			auto winData = dynamic_cast<TextAtlas*>(_tableBG->getParent()->getChildByName("winData"));
			winData->setString("");
			auto win = dynamic_cast<Sprite*>(this->getChildByName("win"));
			win->setVisible(false);
			unschedule(schedule_selector(GameTableUI::MaLiGetScore));
			changeOver();
			setDataText(atoi(_MaLi->_data[0]->getString().c_str()), 1);
			if (_MaLi != nullptr)
			{
				_MaLi->removeFromParentAndCleanup(true);
				_MaLi = nullptr;
			}
			_btnWidget->setVisible(true);
			_btnWidget->setEnabled(true);
			setBtnVisible(Tag_START, true);
			GameAudio::playBackground();
			if (atoi(_dataText[0]->getString().c_str()) < atoi(_dataText[4]->getString().c_str()))
			{
				controlBtnVisible(false);
				_btn[4]->setEnabled(false);
				_btn[4]->setBright(false);
				_btn[5]->setEnabled(false);
				_btn[5]->setBright(false);
			}
		}
	}

	void GameTableUI::changeScore(float delay)
	{
		GameAudio::playDefeng();
		INT value = atoi(_dataText[0]->getString().c_str());
		if (_winScore > 0)
		{
			_winScore -= _scoreChange;
			setWinData();
		
			value += _scoreChange;
			setDataText(value, 1);
		}

		if (_winScore == 0)
		{
			auto winData = dynamic_cast<TextAtlas*>(_tableBG->getParent()->getChildByName("winData"));
			winData->setString("");
			auto win = dynamic_cast<Sprite*>(this->getChildByName("win"));
			win->setVisible(false);
			unschedule(schedule_selector(GameTableUI::changeScore));
			changeOver();

			controlBtnVisible(true);

			if (_btn[8]->isVisible())
			{
				this->runAction(Sequence::create(DelayTime::create(1.0f), 
					CallFunc::create([&](){
						if (atoi(_dataText[0]->getString().c_str()) < 900)
						{

						}
						else
						{
							_tableLogic->sendStartRollMessage();
							startRoll();
						}
					}), nullptr));				
			}
			else
			{
				setBtnVisible(Tag_START, true);
			}
			
		}
	}

	void GameTableUI::playSecondAnimate(int value, int dianShu)
	{
		if (value)
		{
			_BiBei->playArmature(_BiBei->LOST);
			this->runAction(Sequence::create(DelayTime::create(1.0f), 
				CallFunc::create([&](){
					GameAudio::playBiBeiWin();
					GameAudio::playDianshu(dianShu);
					_BiBei->_saizi[0]->setVisible(true);
					_BiBei->_saizi[1]->setVisible(true);}),
				DelayTime::create(3.0f), CallFunc::create([&](){ secondResult();}), nullptr));
			setDataText(_winScore, 2);
			
		}
		else
		{
			_BiBei->playArmature(_BiBei->WIN);
			this->runAction(Sequence::create(DelayTime::create(1.0f), 
				CallFunc::create([&](){
					GameAudio::playLose();
					GameAudio::playDianshu(dianShu);
					_BiBei->_saizi[0]->setVisible(true);
					_BiBei->_saizi[1]->setVisible(true);}),DelayTime::create(3.0f),CallFunc::create([&]()
			{ 
				GameAudio::stopBackground();
				changeLayer();
				GameAudio::playBackground();
				_BiBei->_secondBG->getParent()->getParent()->removeFromParentAndCleanup(true);
				_secondBG = nullptr;
			}), nullptr));
			setDataText(0,2);
			if (atoi(_dataText[0]->getString().c_str()) && (atoi(_dataText[3]->getString().c_str())))
			{
				_btn[4]->setVisible(true);
				_btn[5]->setVisible(false);
				_btn[4]->setEnabled(true);
				_btn[5]->setEnabled(false);
				_btn[4]->setBright(true);
			}
			setBtnVisible(Tag_BIBEI, false);
			setBtnVisible(Tag_WIN, false);
		}
	}

	void GameTableUI::handleSecondDeFeng()
	{
		changeOver();
		_BiBei->_btn[3]->setVisible(false);
		_BiBei->_btn[4]->setVisible(false);
		_BiBei->_btn[3]->setEnabled(false);
		_BiBei->_btn[4]->setEnabled(false);
		_scoreChange = _winScore/10;
		schedule(schedule_selector(GameTableUI::secondDeFeng), 0.1f);
	}

	void GameTableUI::secondDeFeng(float delay)
	{
		GameAudio::playDefeng();
		INT value = atoi(_BiBei->_Bdata[0]->getString().c_str());
		if (_winScore > 0)
		{
			_winScore -= _scoreChange;
			setWinData();

			value += _scoreChange;
			char name[20] = {0};
			sprintf(name, "%d", value);
			_BiBei->_Bdata[0]->setString(name);	
			setDataText(value, 1);
		}

		if (_winScore == 0)
		{
			GameAudio::stopBackground();

			auto winData = dynamic_cast<TextAtlas*>(_tableBG->getParent()->getChildByName("winData"));
			winData->setString("");
			auto win = dynamic_cast<Sprite*>(this->getChildByName("win"));
			win->setVisible(false);
			unschedule(schedule_selector(GameTableUI::secondDeFeng));
			_BiBei->_secondBG->getParent()->getParent()->removeFromParentAndCleanup(true);
			changeLayer();
			GameAudio::playBackground();			
		}
	}

	void GameTableUI::BiBeiHistory(int* history)
	{
		char name[40] = {0};
		for (int i = 0; i < 10; i++)
		{
			sprintf(name, "WaterMargin/useImage/history_%d.png", history[i]);
			_BiBei->_history[i]->loadTexture(name);
		}		
	}


	void GameTableUI::BiBeiAgain()
	{
		//停止切换
		changeOver();
		auto winData = dynamic_cast<TextAtlas*>(_tableBG->getParent()->getChildByName("winData"));
		winData->setString("");
		auto win = dynamic_cast<Sprite*>(this->getChildByName("win"));
		win->setVisible(false);
		
		_BiBei->_btn[3]->setVisible(false);
		_BiBei->_btn[4]->setVisible(false);
		_BiBei->_btn[3]->setEnabled(false);
		_BiBei->_btn[4]->setEnabled(false);

		_BiBei->_saizi[0]->setVisible(false);
		_BiBei->_saizi[1]->setVisible(false);

		
		char name[20] = {0};
		sprintf(name, "%d", _winScore);
		_BiBei->_Bdata[1]->setString(name);
		
		for (int i = 0; i < 3; i++)
		{
			if (_BiBei->_note[i]->isVisible())
			{
				_BiBei->_note[i]->setVisible(false);
			}
		}
		
		_BiBei->startPlay();

	}

	void GameTableUI::secondResult()
	{
		
		_BiBei->_saizi[0]->setVisible(false);
		_BiBei->_saizi[1]->setVisible(false);
		
		changeBegin();
		
		auto win = dynamic_cast<Sprite*>(this->getChildByName("win"));
		win->setVisible(true);
		
		setWinData();
		
		_BiBei->_btn[3]->setVisible(true);
		_BiBei->_btn[4]->setVisible(true);
		_BiBei->_btn[3]->setEnabled(true);
		_BiBei->_btn[4]->setEnabled(true);
		
		_BiBei->playArmature(_BiBei->WAIT);
	}

	void GameTableUI::leaveDesk()
	{
		RoomLogic()->close();
		GamePlatform::returnPlatform(LayerType::ROOMLIST);
	}
}
