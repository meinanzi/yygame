#ifndef __HNUpdate_H__
#define __HNUpdate_H__

#include "HNUIExport.h"
#include "HNNetExport.h"
#include "cocos2d.h"
#include "cocos-ext.h"
#include <string>

#include "network/HttpClient.h"
using namespace cocos2d::network;

USING_NS_CC;
USING_NS_CC_EXT;

namespace HN
{
	/*
	 * redbird app update.
	 */
	class HNUpdate: public HN::HNLayer, public HNHttpDelegate
	{
	public:
		static HNUpdate* create();
		void checkUpdate();
		HNUpdate();
		virtual ~HNUpdate();
		virtual bool init();
		std::function<void(bool updated, cocos2d::Node* pSender, const std::string& message, const std::string& storePath)> onFinishCallback;
	protected:
		bool getOnlineVersion();
		bool hasNewVersion();
		void onProgressCallback(double total, double downloaded, const std::string &url, const std::string &customId);
		void onSuccessCallback(const std::string &url, const std::string &tmp, const std::string &customId);
		void onErrorCallback(const Downloader::Error &error);

		// 发送请求
		void requestWithParams(const std::string& params, const std::string& tag, const std::string& url, HttpRequest::Type type);

		virtual void onHttpResponse(const std::string& requestName, bool isSucceed, const std::string &responseData) override;
		
		//有更新
		void needUpdate();

		// 下载回调
		void onClickDownloadCallback();
	private:
		std::string _onlineVersion;
		std::string _localVersion;
		std::string _storePath;
		std::string _jsonPath;
		std::string _downloadUrl;
		std::string _plistUrl;
		std::shared_ptr<Downloader> _downloader;
		ProgressTimer* _progressTimer;
		Sprite* _progressBg;
		Label* _labelPercent;
		Label* _labelMessage;
		bool _isMustUpdate;//判断是否必须更新
	};
}

#endif