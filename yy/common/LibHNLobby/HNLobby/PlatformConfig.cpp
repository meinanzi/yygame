#include "PlatformConfig.h"

static PlatformConfig* sPlatformConfig = nullptr;

PlatformConfig* PlatformConfig::getInstance()
{
	if (nullptr == sPlatformConfig)
	{
		sPlatformConfig = new (std::nothrow) PlatformConfig();
	}
	return sPlatformConfig;
}

void PlatformConfig::destroyInstance()
{
	CC_SAFE_DELETE(sPlatformConfig);
}

std::string PlatformConfig::getAppInfoUrl()
{
	return buildHttp(getWebServerUrl(), "/Public/AppInfo.ashx?");
}

std::string PlatformConfig::getPromoterUrl()
{
	return buildHttp(getAPIServerUrl(), "/api/tuiguang/Tuigung.ashx");
}

std::string PlatformConfig::getOnlineInstallUrl_iOS(const std::string& url)
{
	std::string plist = "itms-services://?action=download-manifest&url=" + url;
	return plist;
}

std::string PlatformConfig::getNoticeUrl()
{
	return buildHttp(getAPIServerUrl(), "/Public/XmlHttpUser.aspx?");
}

std::string PlatformConfig::getGoodsListUrl(const std::string& platForm_Type)
{
	auto url_prefix = StringUtils::format("/public/xmlhttpuser.aspx?type=GetMobilePayConfig&GameType=%s", platForm_Type.c_str());
	return buildHttp(getAPIServerUrl(), url_prefix.c_str());
}

std::string PlatformConfig::getOrderUrl()
{
	return buildHttp(getAPIServerUrl(), "/public/xmlhttpuser.aspx?");
}

std::string PlatformConfig::getPrizeUrl()
{
	return buildHttp(getAPIServerUrl(), "/public/prizedata.ashx?");
}

std::string PlatformConfig::getPayCallbackUrl_iOS()
{
	return buildHttp(getAPIServerUrl(), "/public/xmlhttpuser.aspx");
}

std::string PlatformConfig::getShareUrl(int id)
{
	std::string url = buildHttp(getWebServerUrl(), "/download/download2.aspx?id=");
	url.append(StringUtils::format("%d", id));
	return url;
}

std::string PlatformConfig::getProtocolUrl()
{
	std::string url = buildHttp(getWebServerUrl(), "/xy/Protocol.aspx");
	return url;
}

std::string PlatformConfig::getEditUrl()
{
	std::string url = buildHttp(getAPIServerUrl(), "/public/auditconfig.ashx");
	return url;
}

std::string PlatformConfig::buildHttp(const std::string& url, const std::string& path)
{
	std::string http("http://");
	http.append(url);
	http.append(path);
	return http;
}

std::string PlatformConfig::buildHttps(const std::string& url, const std::string& path)
{
	std::string https("https://");
	https.append(url);
	https.append(path);
	return https;
}

PlatformConfig::PlatformConfig()
	: _gameDesignSize(1280, 720)
	, _platformDesignSize(1280, 720)
	, _accountType(UNKNOWN)
	, _appId(1)
	, _isIAP(true)
{
}

PlatformConfig::~PlatformConfig()
{
}        