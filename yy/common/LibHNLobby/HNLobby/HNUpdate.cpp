#include "HNUpdate.h"
#include "HNLobby/PlatformConfig.h"
#include "HNMarketExport.h"
#include "HNNetExport.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "GamePrompt.h"

USING_NS_CC_EXT;
using namespace std;

namespace HN
{
	static const std::string PROGRESS_BAR("platform/setting/settingRes/progress_bar.png");
	static const std::string PROGRESS_BAR_BACK("platform/setting/settingRes/progress_bar_bg.png");

	HNUpdate* HNUpdate::create()
	{
		HNUpdate* update = new HNUpdate();
		if(update != nullptr && update->init())
		{
			update->autorelease();
			return update;
		}
		CC_SAFE_DELETE(update);
		return nullptr;
	}

	void HNUpdate::checkUpdate()
	{
		getOnlineVersion();
	}

	HNUpdate::HNUpdate()
		: _progressTimer(nullptr)
		, _labelPercent(nullptr)
		, _labelMessage(nullptr)
		, _progressBg(nullptr)
		, _isMustUpdate(false)
	{
		auto listener = EventListenerTouchOneByOne::create();
		listener->onTouchBegan = [](Touch* pTouch, Event* pEvent){
			return true;
		};
		listener->setSwallowTouches(true);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

		_downloader = std::make_shared<Downloader>();
		_downloader->setProgressCallback(std::bind(&HNUpdate::onProgressCallback, this, 
			std::placeholders::_1,
			std::placeholders::_2,
			std::placeholders::_3,
			std::placeholders::_4));
		_downloader->setSuccessCallback(std::bind(&HNUpdate::onSuccessCallback, this,
			std::placeholders::_1,
			std::placeholders::_2,
			std::placeholders::_3));
		_downloader->setErrorCallback(std::bind(&HNUpdate::onErrorCallback, this,
			std::placeholders::_1));

		_localVersion  = HN::Operator::requestChannel("sysmodule", "getversion");
		_onlineVersion = _localVersion;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		_storePath = "/sdcard/";
#else
		_storePath = FileUtils::getInstance()->getWritablePath();
#endif
	}

	HNUpdate::~HNUpdate()
	{
		HNHttpRequest::getInstance()->removeObserver(this);
	}

	bool HNUpdate::init()
	{
		if(!HNLayer::init())
		{
			return false;
		}

		Size winSize = Director::getInstance()->getWinSize();

		auto layer = LayerColor::create(Color4B(0, 0, 0, 125));
		this->addChild(layer, 1);
		layer->setPosition(Vec2::ZERO);

		Sprite* bar_bg = Sprite::create(PROGRESS_BAR_BACK);
		if(bar_bg != nullptr)
		{
			this->addChild(bar_bg, 2);
			bar_bg->setAnchorPoint(Vec2(0.5f, 0.5f));
			bar_bg->setPosition(winSize.width / 2, winSize.height / 2);
			bar_bg->setVisible(false);
			_progressBg= bar_bg;
		}

		Sprite* bar = Sprite::create(PROGRESS_BAR);
		if(bar != nullptr)
		{			
			_progressTimer = ProgressTimer::create(bar);
			this->addChild(_progressTimer, 3);
			_progressTimer->setAnchorPoint(Vec2(0.5f, 0.5f));
			_progressTimer->setPosition(winSize.width / 2, winSize.height  / 2);
			_progressTimer->setPercentage(0);
			_progressTimer->setType(ProgressTimer::Type::BAR);
			_progressTimer->setMidpoint(Vec2(0,0));
			_progressTimer->setBarChangeRate(Vec2(1, 0));
			_progressTimer->setVisible(false);
		}		

		_labelPercent = Label::createWithSystemFont("", "Arial", 24);
		this->addChild(_labelPercent,4);
		_labelPercent->setAnchorPoint(Vec2(0.5f, 0.5f));
		_labelPercent->setPosition(winSize.width / 2, winSize.height / 2);

		_labelMessage = Label::createWithSystemFont("", "Arial", 24);
		this->addChild(_labelMessage, 4);
		_labelMessage->setAnchorPoint(Vec2(0.5f, 0.5f));
		_labelMessage->setPosition(winSize.width / 2, winSize.height / 2 + 60);
		_labelMessage->setString(GBKToUtf8("版本更新检测中"));

		return true;
	}

	static const int BUFFER_SIZE = 200;
	static unsigned char buffer[BUFFER_SIZE] = {0};

	bool HNUpdate::getOnlineVersion()
	{
		memset(buffer, 0, sizeof(buffer));
		_labelMessage->setString(GBKToUtf8("版本更新检测中"));
		//_downloader->downloadAsync(PlatformConfig::getInstance()->getUpdateCheckUrl(), _jsonPath, "check");

		//发送请求，获取版本号和安装包地址
		requestWithParams("action=GetInfoByKey", "update", PlatformConfig::getInstance()->getAppInfoUrl().c_str(), HttpRequest::Type::POST);

		return true;
	}

	bool HNUpdate::hasNewVersion()
	{
		return _localVersion.compare(_onlineVersion) != 0;
	}

	void HNUpdate::onProgressCallback(double total, double downloaded, const std::string &url, const std::string &customId)
	{
		if(customId.compare("download") == 0)
		{
			if(_progressTimer != nullptr)
			{
				float percent = (float)(downloaded / total) * 100;
				_progressTimer->setPercentage(percent);
			}

			if(_labelPercent != nullptr)
			{
				char str[50] = {0};
				sprintf(str, "%.2fKB / %.2fKB", downloaded / 1024, total / 1024);
				_labelPercent->setString(str);
			}
		}
		else
		{

		}		
	}

	void HNUpdate::onSuccessCallback(const std::string &url, const std::string &tmp, const std::string &customId)
	{
		if(customId.compare("download") == 0)
		{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
			do 
			{
				CC_BREAK_IF(_progressTimer == nullptr);
				CC_BREAK_IF(onFinishCallback == nullptr);
				if(_progressTimer->getPercentage() >= 100.f)
				{
					onFinishCallback(true, this, GBKToUtf8("下载完成"), _storePath);
				}
			} while (0);
#endif
		}
		else
		{

		}

	}

	void HNUpdate::onErrorCallback(const Downloader::Error &error)
	{
		onFinishCallback(false, this, error.message, "");
	}

	//发送数据接口
	void HNUpdate::requestWithParams(const std::string& params, const std::string& tag, const std::string& url, HttpRequest::Type type)
	{
		std::string requestData;
		requestData.append(params);
		requestData.append("&key=");
		requestData.append(PlatformConfig::getInstance()->getAppKey());

		HNHttpRequest::getInstance()->addObserver(this);
		HNHttpRequest::getInstance()->request(tag, type, url, requestData);
	}

	void HNUpdate::onHttpResponse(const std::string& requestName, bool isSucceed, const std::string &responseData)
	{
		if (!isSucceed)
		{
			std::string errorMsg = GBKToUtf8("更新查询失败(") + responseData + ")";
			auto prompt = GamePromptLayer::create();
			prompt->showPrompt(errorMsg);
			prompt->setCallBack([this](){
				this->removeFromParent();
			});
			return;
		}
		if (requestName.compare("update") == 0)
		{
			HNLOG("appInfo>%s", responseData.c_str());

			rapidjson::Document doc;
			doc.Parse<rapidjson::kParseDefaultFlags>(responseData.c_str());
			if (doc.HasParseError() || !doc.IsObject())
			{
				auto prompt = GamePromptLayer::create();
				prompt->showPrompt(GBKToUtf8("数据解析失败"));
				prompt->setCallBack([this](){
					this->removeFromParent();
				});
				return;
			}
			do
			{
				rapidjson::Value& array = doc["content"];
				if (array.IsArray())
				{
					//解析返回的数据
					for (UINT i = 0; i < array.Size(); i++)
					{
						rapidjson::Value& arraydoc = array[i];
						do
						{
							if (arraydoc.HasMember("AutoLaunch"))
							{
								_isMustUpdate = arraydoc["AutoLaunch"].GetBool();
							}
							if (arraydoc.HasMember("PlistUrl"))
							{
								_plistUrl = arraydoc["PlistUrl"].GetString();
								PlatformConfig::getInstance()->setDownloadPlistUrl(_plistUrl);
							}
							if (arraydoc.HasMember("id"))
							{
								PlatformConfig::getInstance()->setAppId(arraydoc["id"].GetInt());
							}
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
							if (arraydoc.HasMember("AndroidVersion"))
							{
								_onlineVersion = arraydoc["AndroidVersion"].GetString();
							}
							if (arraydoc.HasMember("AndroidName"))
							{
								_downloadUrl = arraydoc["AndroidName"].GetString();
							}
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
							if (arraydoc.HasMember("AppleVersion"))
							{
								_onlineVersion = arraydoc["AppleVersion"].GetString();
							}
							if (arraydoc.HasMember("AppleName"))
							{
								_downloadUrl = arraydoc["AppleName"].GetString();
							}
#endif
						} while (0);
					}
				}
			} while (0);

			if (hasNewVersion())
			{
				needUpdate();
			}
			else
			{
				this->removeFromParent();
			}
		}
	}

	void HNUpdate::onClickDownloadCallback()
	{
		if(_progressTimer != nullptr)
		{
			_progressBg->setVisible(true);
			_progressTimer->setVisible(true);
			_progressTimer->setPercentage(0.f);
		}
		if(_labelMessage != nullptr)
		{
			_labelMessage->setString(GBKToUtf8("新版下载中..."));
		}

		size_t locate = _downloadUrl.find_last_of("/");
		if(locate == std::string::npos)
		{
			GamePromptLayer::create()->showPrompt(GBKToUtf8("文件名出错"));
			return;
		}
		std::string filename = _downloadUrl.substr(locate + 1);

		_storePath.append(filename);
		_downloader->downloadAsync(_downloadUrl, _storePath, "download");
	}

	//有更新
	void HNUpdate::needUpdate()
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		auto prompt = GamePromptLayer::create();
		if (_isMustUpdate)
		{
			prompt->showPrompt(GBKToUtf8("发现新版本，更新进入游戏"));
		}
		else
		{
			prompt->showPrompt(GBKToUtf8("发现新版本，是否现在下载？"));
			prompt->setPromptCanSelect();
		}
		prompt->setCallBack(CC_CALLBACK_0(HNUpdate::onClickDownloadCallback, this));
		prompt->setCancelCallBack([this](){
			this->removeFromParent();
		});

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
				onFinishCallback(true, this, GBKToUtf8("更新成功"), _plistUrl);
#else

#endif
	}
}