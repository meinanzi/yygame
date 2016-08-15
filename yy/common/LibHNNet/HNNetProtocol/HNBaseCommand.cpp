#include "HNBaseCommand.h"
#include "HNSocket/HNSocket.h"

// 线上地址
static const std::string PLATFORM_SERVER_ADDRESS("222.189.228.167");  // 游戏服务器地址
static const std::string API_SERVER_URL("www.dzpk886.com");              // Web接口地址


// app更新专用地址（iOS需要SSL证书）
static const std::string WEB_SERVER_URL("www.dzpk886.com");

LLONG getSecrectKey()
{
	return 20160606;
}

std::string getPlatformServerAddress()
{
	std::string address(PLATFORM_SERVER_ADDRESS);
	if(HN::HNSocket::isValidIP(address))
	{
		return address;
	}
	else
	{
		std::vector<std::string> ips = HN::HNSocket::getIpAddress(address.c_str());
		return ips.empty() ? "" : ips.at(0);
	}
}

INT getPlatformServerPort()
{
	return 3015;
}

std::string getWebServerUrl()
{
	return WEB_SERVER_URL;
}

std::string getAPIServerUrl()
{
	return API_SERVER_URL;
}

std::string getXXTEA_KEY()
{
	// 防止进入静态内存区域
	char key[] = { 'G', '9', 'w', '0', 'B', 'A', 'Q', 'E', 'F', 'A', 'A', 'S', 'C', 'A', 'l', '8', 'w', 'g', 'g' };
	return std::string(key);
}