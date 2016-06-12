#ifndef _HNBaseCommand_H__
#define _HNBaseCommand_H__

#include "HNBaseType.h"
#include <string>

#define		HN_TARGET_PHONE_ONLY    0x01  //手游独立平台，和PC、网页数据不互通
#define		HN_TARGET_MIX_PLATFORM  0x02  //混合平台，和PC、网页数据互通

// 此处为密钥由客户自己管理，设置在整数范围
LLONG getSecrectKey();

// 平台的登录ID地址
std::string getPlatformServerAddress();

// 平台的端口
INT getPlatformServerPort();

// 获取域名Url
std::string getWebServerUrl();

// 获取API接口地址
std::string getAPIServerUrl();

// 获取加密秘钥
std::string getXXTEA_KEY();

// 目标平台类型
#define		HN_TAREGET_PLATFORM HN_TARGET_MIX_PLATFORM

#endif	//_HNBaseCommand_H__