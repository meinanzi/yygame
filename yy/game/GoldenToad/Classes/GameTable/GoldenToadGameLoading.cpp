#include "GoldenToadGameLoading.h"
#include "GoldenToadGameTableUI.h"

namespace GoldenToad
{
	static const char*	LAYER_BG				=	"goldenToad/common/bg.png";
	static const char*	NET_PLIST_PATH			=	"goldenToad/bullet/netImg.plist";
	static const char*	NET_IMAGE_PATH			=	"goldenToad/bullet/netImg.png";
	static const char*	HAICAO_PLIST_PATH		=	"goldenToad/Scene/haicao/haicaoBeike.plist";
	static const char*	HAICAO_IMAGE_PATH		=	"goldenToad/Scene/haicao/haicaoBeike.pvr.ccz";
	static const char*	FISH_IMAGE_PATH			=   "goldenToad/fishAnimation/fishImg.pvr.ccz";
	static const char*	FISH_PLIST_PATH			=   "goldenToad/fishAnimation/fishImg.plist";
	static const char*	FISH_PATH				=   "goldenToad/fishAnimation/bigFishAnimation.ExportJson";
	static const char*	WATER_PLIST_PATH		=	"goldenToad/Scene/water.plist";
	static const char*	WATER_IMAGE_PATH		=	"goldenToad/Scene/water.pvr.ccz";

	static const char*	LOADING_PLIST_PATH		=	"goldenToad/common/loading.plist";

	GoldenToadGameLoading::GoldenToadGameLoading()
		: fileNum (0)
		, _loading(true)
	{
	}

	GoldenToadGameLoading::~GoldenToadGameLoading()
	{
	}

	GoldenToadGameLoading* GoldenToadGameLoading::create(bool loading)
	{
		GoldenToadGameLoading* pRet = new GoldenToadGameLoading();
		if (nullptr != pRet && pRet->init(loading))
		{
			pRet->autorelease();
			return pRet;
		}
		CC_SAFE_DELETE(pRet);
		return nullptr;
	}

	bool GoldenToadGameLoading::init(bool loading)
	{
		if (!HNLayer::init())
		{
			return false;
		}

		setBackGroundImage(LAYER_BG);
		auto winSize = Director::getInstance()->getWinSize();

		_loading = loading;

		return true;
	}

	void GoldenToadGameLoading::loadRes()
	{
		if(_loading)
		{
			auto winSize = Director::getInstance()->getWinSize();
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile(LOADING_PLIST_PATH);
			//loading¶¯»­
			char name[24];
			auto loadingAnimation = Animation::create();
			loadingAnimation->setDelayPerUnit(0.4f);
			loadingAnimation->setLoops(-1);
			for (int i = 1; i <= 4; i++)
			{
				sprintf(name, "loading%d.png", i);
				loadingAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(name));
			}
			auto  animate = Animate::create(loadingAnimation);
			auto loadingSp = Sprite::createWithSpriteFrameName(name);
			loadingSp->setPosition(Vec2(winSize.width / 2, winSize.height * 0.4f));
			loadingSp->runAction(animate);
			addChild(loadingSp, WATER_ZORDER);

			Texture2D::setDefaultAlphaPixelFormat(Texture2D::PixelFormat::RGBA4444);
			Director::getInstance()->getTextureCache()->addImageAsync(FISH_IMAGE_PATH, CC_CALLBACK_1(GoldenToadGameLoading::loadingTextureCallback, this, FISH_PLIST_PATH));
			Director::getInstance()->getTextureCache()->addImageAsync(HAICAO_IMAGE_PATH, CC_CALLBACK_1(GoldenToadGameLoading::loadingTextureCallback, this, HAICAO_PLIST_PATH));
			Director::getInstance()->getTextureCache()->addImageAsync(WATER_IMAGE_PATH, CC_CALLBACK_1(GoldenToadGameLoading::loadingTextureCallback, this, WATER_PLIST_PATH));
			Director::getInstance()->getTextureCache()->addImageAsync(NET_IMAGE_PATH, CC_CALLBACK_1(GoldenToadGameLoading::loadingTextureCallback, this, NET_PLIST_PATH));

			ArmatureDataManager::getInstance()->addArmatureFileInfoAsync(FISH_PATH, this, schedule_selector(GoldenToadGameLoading::loadingCallback));
		}		
	}

	void GoldenToadGameLoading::loadingTextureCallback(Texture2D* textureData, std::string plist)
	{
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile(plist , textureData);

		loadingCallback(0.0f);
	}

	void GoldenToadGameLoading::loadingCallback(float dt)
	{
		fileNum++;
		if (fileNum >= 5)
		{
			if (nullptr != onCloseCallBack)
			{
				onCloseCallBack();
			}
			this->removeFromParent();
		}
	}	
}