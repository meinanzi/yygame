#ifndef _HN_ConverCode_h__
#define _HN_ConverCode_h__

#include "cocos2d.h"
#include <string>

USING_NS_CC;

namespace HN 
{
	class HNConverCode
	{
	public:
		static std::string GB2312ToUtf8(const char *gb2313);

		static std::string Utf8ToGB2312(const char *utf8);
		
		// URL编码
		static std::string HNURLEncode(const std::string& url);

		// URL解码
		static std::string HNURLDeCode(const std::string& decodedUrl);
	};
}

#define GBKToUtf8(object) HNConverCode::GB2312ToUtf8(object).c_str()

#define Utf8ToGB(object) HNConverCode::Utf8ToGB2312(object).c_str()

// URL编码
#define URLEncode(object) HNConverCode::HNURLEncode(object).c_str()

// URL解码
#define URLDecode(object) HNConverCode::HNURLDeCode(object).c_str()

#endif // _HN_ConverCode_h__
